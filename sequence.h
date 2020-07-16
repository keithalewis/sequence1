// sequence.h - interators having operator bool() const
#pragma once
#include <cmath>
#include <compare>
#include <functional>
//#include <initializer_list>
#include <iterator>
#include <limits>
#include <type_traits>
#include <vector>

namespace seq {

	template<class I>
	concept sequence = requires (I i) {
		i.operator bool();
	};

	template<class... I>
	struct common_iterator_traits {
		using iterator_category = std::common_type_t<typename std::iterator_traits<I>::iterator_category...>;
		using value_type = std::common_type_t<typename std::iterator_traits<I>::value_type...>;
		using difference_type = std::common_type_t<typename std::iterator_traits<I>::difference_type...>;
		using pointer = std::common_type_t<typename std::iterator_traits<I>::pointer...>;
		using reference = std::common_type_t<typename std::iterator_traits<I>::reference...>;
	};

	// {a[0], a[1], ... }
	template<class T>
	class array : public std::iterator_traits<const T*> {
		size_t n;
		const T* a;
	public:
		array(size_t n = 0, const T* a = nullptr)
			: n(n), a(a)
		{ }
		template<size_t N>
		array(const T(&a)[N])
			: n(N), a(a)
		{ }
		const auto operator<=>(const array&) const = default;

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
		const auto operator<=>(const concatenate&) const = default;
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
		const auto operator<=>(const constant&) const = default;
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
		const auto operator<=>(const counter&) const = default;
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
		const auto operator<=>(const counted&) const = default;
		// remaining size to end
		size_t size() const
		{
			auto i = std::reverse_iterator<I>;
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

	// convert iterator to sequence based on sentinal
	template<class I, class S>
	class sentinal : public I {
		S s;
	public:
		sentinal(I i, S s)
			: I(i), s(s)
		{ }
		operator bool() const
		{
			return !I::operator==(s);
		}
	};

	template<class I, class S>
	class split {
		I i;
		S s;
		sentinal<I, S> is;
	public:
		split(I i, S s)
			: i(i), s(s), is(i,s)
		{ }
		operator bool() const
		{
			return i;
		}
		auto operator*()
		{
			return sentinal<I&,S>(i, s);
		}
		split& operator++()
		{
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
		//const auto operator<=>(const memoize&) const = default;

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

	template<class T>
	class power : public std::iterator_traits<const T*> {
		T t, tn;
	public:
		power(T t = T(1))
			: t(t), tn(T(1))
		{ }
		const auto operator<=>(const power&) const = default;
		operator bool() const
		{
			return true;
		}
		T operator*() const
		{
			return tn;
		}
		power& operator++()
		{
			tn *= t;

			return *this;
		}
	};

	// Pochhammer sequence
	// rising  x^(n) = x(x + 1) ....
	// falling (x)_n = x(x - 1) ....
	template<class T = double>
	class pochhammer : public std::iterator_traits<const T*> {
		T x, xn;
		long n;
	public:
		// rising (n > 0) or falling (n < 0) Pochhammer sequence
		pochhammer(T x = 0, long n = 1)
			: x(x), xn(x), n(n)
		{ }
		const auto operator<=>(const pochhammer&) const = default;
		operator bool() const
		{
			return true;
		}
		T operator*() const
		{
			return xn == 0 ? T(1) : xn;
		}
		pochhammer& operator++()
		{
			if (x == 0) {
				x = xn = 1;
			}
			else {
				xn *= (x + n);
				n > 0 ? ++n : --n;
			}

			return *this;
		}
	};
	template<class T>
	inline auto factorial(T t = 0)
	{
		return pochhammer(t);
	}

	template<class I, class J>
	class range : public I {
		J e;
	public:
		// convert STL range to sequence
		range(I i, J e)
			: I(i), e(e)
		{ }
		const auto operator<=>(const range& i_) const
		{
			const auto cmp = I::operator<=>(i_);

			return cmp != 0 ? cmp : e <=> i_.e;
		}
		operator bool() const
		{
			return !I::operator==(e);
		}
	};
	// sequence over container elements
	template<class C>
	inline auto make_range(C& c)
	{
		return seq::range(begin(c), end(c));
	}
	template<class C>
	inline auto make_reverse(C& c)
	{
		return seq::range(rbegin(c), rend(c));
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
		F f;
		I i;
	public:
		typedef std::invoke_result_t<F, typename std::iterator_traits<I>::value_type> value_type;
		apply(F&& f, I i)
			: f(f), i(i)
		{ }
		operator bool() const
		{
			return i;
		}
		value_type operator*() const
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
		const F& f; // no const ref???
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
		F f;
		I i;
	public:
		typedef typename std::iterator_traits<I>::value_type value_type;
		truncate(F&& f, I i)
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
	template<class I, class T = typename std::iterator_traits<I>::value_type>
	inline auto epsilon(I i, T eps = std::numeric_limits<T>::epsilon())
	{
		return truncate([=](T t) { return fabs(t) <= eps; }, i);
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
	class binop : public common_iterator_traits<I,J> { 
		I i;
		J j;
	public:
		binop(I i, J j)
			: i(i), j(j)
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
			return Binop{}(*i, *j);
		}
	};
}


template<seq::sequence I, seq::sequence J>
inline auto operator+(I i, J j)
{
	using T = decltype((*i) + (*j));

	return seq::binop<std::plus<T>, I, J>(i, j);
}
template<seq::sequence I, seq::sequence J>
inline auto operator-(I i, J j)
{
	using T = decltype((*i) - (*j));

	return seq::binop<std::minus<T>, I, J>(i, j);
}
template<seq::sequence I, seq::sequence J>
inline auto operator*(I i, J j)
{
	using T = decltype((*i) * (*j));

	return seq::binop<std::multiplies<T>, I, J>(i, j);
}
template<seq::sequence I, seq::sequence J>
inline auto operator/(I i, J j)
{
	using T = decltype((*i)/(*j));

	return seq::binop<std::divides<T>,I,J>(i, j);
}

/*
template<class I, class J>
inline auto operator,(I i, J j)
{
	return seq::concatenate(i, j);
}
*/