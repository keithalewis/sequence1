// sequence.h - interators having operator bool() const
#pragma once
#include <compare>
//#include <initializer_list>
#include <iterator>
#include <type_traits>
#include <vector>

namespace seq {

	template<class... I>
	struct common_iterator_traits {
		using iterator_category = std::common_type<typename std::iterator_traits<I>::iterator_category...>;
		using value_type = std::common_type<typename std::iterator_traits<I>::value_type...>;
		using difference_type = std::common_type<typename std::iterator_traits<I>::difference_type...>;
		using pointer = std::common_type<typename std::iterator_traits<I>::pointer...>;
		using reference = std::common_type<typename std::iterator_traits<I>::reference...>;
	};

	// array<T>({a0, ...});
	template<class T>
	class array : public std::iterator_traits<const T*> {
		size_t n;
		const T* a;
	public:
		array()
			: n(0), a(nullptr)
		{ }
		template<size_t N>
		array(const T(&a)[N])
			: n(N), a(a)
		{ }
		// remaining size to end
		size_t size() const
		{
			return n;
		}

		operator bool() const
		{
			return 0 != n;
		}
		T operator*() const
		{
			return *a;
		}
		array& operator++()
		{
			--n;
			++a;

			return *this;
		}
		array operator++(int)
		{
			array a_ = *this;

			--n;
			++a;

			return a_;
		}
		array& operator--()
		{
			++n;
			--a;

			return *this;
		}
		array operator--(int)
		{
			array a_ = *this;

			++n;
			--a;

			return a_;
		}
		array& operator+=(ptrdiff_t m)
		{
			n -= m;
			a += m;

			return *this;
		}
		array& operator-=(ptrdiff_t m)
		{
			n += m;
			a -= m;

			return *this;
		}
	};

	template<class I, class J>
	class concatenate : public common_iterator_traits<I,J> {  
		I i;
		J j;
	public:
		concatenate(I i, J j)
			: i(i), j(j)
		{ }
		operator bool() const
		{
			return i || j;
		}
		auto operator*()
		{
			return i ? *i : *j;
		}
		concatenate& operator++()
		{
			i ? ++i : ++j;

			return *this;
		}
	};

	template<class T>
	class constant : public std::iterator_traits<const T*> {
		T t;
	public:
		constant(T t = 0)
			: t(t)
		{ }
		operator bool() const
		{
			return true;
		}
		T operator*() const
		{
			return t;
		}
		constant& operator++()
		{
			return *this;
		}
	};

	// similar to iota
	// use a + b*counter<>{} for arithmetic series
	template<class T>
	class counter : public std::iterator_traits<T*> {
		T t;
	public:
		counter(T t = 0)
			: t(t)
		{ }
		operator bool() const
		{
			return true;
		}
		T operator*() const
		{
			return t;
		}
		counter& operator++()
		{
			++t;

			return *this;
		}
	};
	template<class I>
	class counted : public std::iterator_traits<I> {
		size_t n;
		I i;
	public:
		typedef typename std::iterator_traits<I>::value_type value_type;
		counted(size_t n, I i)
			: n(n), i(i)
		{ }
		// remaining size to end
		size_t size() const
		{
			return n;
		}

		operator bool() const
		{
			return 0 != n;
		}
		value_type operator*() const
		{
			return *i;
		}
		counted& operator++()
		{
			--n;
			++i;

			return *this;
		}
	};

	template<class I, class T = typename std::iterator_traits<I>::value_type>
	class memoize : public std::iterator_traits<typename std::vector<T>::iterator> {
		I i;
		std::vector<T> v;
		typename std::vector<T>::iterator vi; // current iterator
	public:
		memoize(I i)
			: i(i), vi(v.begin())
		{ }
		operator bool() const
		{
			return vi != v.end() ? i : false;
		}
		T operator*() const
		{
			if (vi != v.end()) {
				return *vi;
			}
			
			T t = *i;
			v.push_back(t);
			vi = v.end();
			
			return t;
		}
		memoize& operator++()
		{
			if (vi != v.end()) {
				++vi;
			}
			else {
				++i;
			}

			return *this;
		}
	};

	template<class I, class J>
	class range : public I {
		J e;
	public:
		// convert STL range to sequence
		range(I i, J e)
			: I(i), e(e)
		{ }
		operator bool() const
		{
			return !I::operator==(e);
		}
	};
	// sequence over container elements
	template<class C>
	inline auto make_range(C& c)
	{
		return seq::range(c.begin(), c.end());
	}


	// class extrapolate
	// class memoize -> random access iterator

	//
	// Functions
	//

	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline auto extrapolate(I i, T t)
	{
		return concatenate(i, constant(t));
	}

	// template<class C> inline auto reverse(C& c)
	// { return range(rbegin(c), c.rend(c)); }

	template<class F, class I>
	class apply : public std::iterator_traits<I> {
		const F& f;
		I i;
	public:
		apply(const F& f, I i)
			: f(f), i(i)
		{ }
		operator bool() const
		{
			return i;
		}
		auto operator*() const
		{
			return f(*i);
		}
		apply& operator++()
		{
			++i;

			return *this;
		}
	};


	template<class F, class I>
	class filter : public std::iterator_traits<I> {
		const F& f;
		I i;
	public:
		typedef typename std::iterator_traits<I>::value_type value_type;
		filter(const F& f, I i)
			: f(f), i(i)
		{
			next();
		}
		operator bool() const
		{
			return i;
		}
		value_type operator*() const
		{
			return *i;
		}
		filter& operator++()
		{
			++i;
			next();

			return *this;
		}
	private:
		void next()
		{
			while (i && !f(*i)) {
				++i;
			}
		}
	};

	template<class F, class I>
	class truncate : public std::iterator_traits<I> {
		const F& f;
		I i;
	public:
		typedef typename std::iterator_traits<I>::value_type value_type;
		truncate(const F& f, I i)
			: f(f), i(i)
		{
		}
		operator bool() const
		{
			return !f(*i);
		}
		value_type operator*() const
		{
			return *i;
		}
		truncate& operator++()
		{
			++i;

			return *this;
		}
	};

	template<class I>
	inline auto null(I i)
	{
		return truncate([](auto t) { return t == 0; }, i);
	}
	template<class I>
	inline auto epsilon(I i,
		decltype(*i) epsilon = std::numeric_limits<decltype(*i)>::epsilon())
	{
		return truncate([epsilon](auto t) { return fabs(t) <= epsilon; }, i);
	}

	template<class I, class J>
	inline J copy(I i, J j)
	{
		while (i) {
			*j = *i;
			++i;
			++j;
		}

		return j;
	}

	template<class I, class J>
	inline bool equal(I i, J j)
	{
		while (i && j) {
			if (*i != *j) {
				return false;
			}
			++i;
			++j;
		}

		return !i && !j;
	}

	template<typename I>
	inline I drop(int n, I i)
	{
		std::advance(i, n);

		return i;
	}

	template<class I>
	inline size_t length(I i, size_t n = 0)
	{
		while (i) {
			++n;
			++i;
		}

		return n;
	}

	// make_counted
	template<typename I>
	inline auto take(int n, I i)
	{
		return counted(n, i);
	}

	template<typename I>
	inline auto sum(I i, typename std::iterator_traits<I>::value_type i0 = 0)
	{
		while (i) {
			i0 += *i;
			++i;
		}

		return i0;
	}

	template<class Binop, class I, class J>
	class binop { // common type *I, *J
		static const Binop& op;
		I i;
		J j;
	public:
		binop(const Binop& op, I i, J j)
			: op(op), i(i), j(j)
		{ }
		operator bool() const 
		{
			return i && j;
		}
		binop& operator++()
		{
			++i;
			++j;

			return *this;
		}
		auto operator*() const
		{
			return op(*i, *j);
		}
	};
}

/*
template<class I, class J>
inline auto operator+(I i, J j)
{
	return seq::binop(std::plus<T>{}, i, j);
}
*/
/*
template<class I, class J>
inline auto operator,(I i, J j)
{
	return seq::concatenate(i, j);
}
*/