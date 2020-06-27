// sequence.h - interators having operator bool() const
#pragma once
#include <compare>
#include <initializer_list>
#include <iterator>
#include <vector>

namespace seq {

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

	template<class I>
	class range : public I {
		I e;
	public:
		// convert STL range to sequence
		range(I i, I e)
			: I(i), e(e)
		{ }
		// sequence over container elements
		template<class C>
		range(C& c)
			: range(c.begin(), c.end())
		{ }
		operator bool() const
		{
			return !I::operator==(e);
		}
	};

	// class extrapolate
	// class memoize -> random access iterator

	//
	// Functions
	//

	// template<class C> inline auto reverse(C& c)
	// { return sentinal(c.rbegi(), c.rend()); }

	template<class F, class I>
	class apply : public std::iterator_traits<I> {
		const F& f;
		I i;
	public:
		//typedef typename std::invoke_result<F,typename std::iterator_traits<I>::value_type>::type value_type;
		apply(const F& f, I i)
			: f(f), i(i)
		{ }
		operator bool() const
		{
			return i;
		}
		auto operator*() const -> decltype(f(*i))
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
}