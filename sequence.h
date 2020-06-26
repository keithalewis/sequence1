// sequence.h - interators having operator bool() const
#pragma once
#include <compare>
#include <iterator>
#include <vector>

namespace seq {

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
		const T& operator*() const
		{
			return t;
		}
		T& operator*()
		{
			return t;
		}
		counter& operator++()
		{
			++t;

			return *this;
		}
		counter operator++(int)
		{
			counter tmp = *this;

			++t;

			return tmp;
		}
		counter& operator--()
		{
			--t;

			return *this;
		}
		counter operator--(int)
		{
			counter tmp = *this;

			--t;

			return tmp;
		}
		// ??? operator+=
		// ??? operator-=
	};

	template<class I>
	class counted : public std::iterator_traits<I> {
		size_t n;
		I i;
	public:
		counted(size_t n, I i)
			: n(n), i(i)
		{ }
		size_t size() const
		{
			return n;
		}
		operator bool() const {
			return 0 != n;
		}
		bool operator!() const {
			return !operator bool();
		}
		
		operator I()
		{
			return i;
		}
		operator const I& () const
		{
			return i;
		}

		//
		// overrides
		//
		counted& operator++() // override
		{
			if (0 != n) {
				--n;
				i++;
			}

			return *this;
		}
		counted operator++(int) // override
		{
			counted tmp = *this;

			if (0 != n) {
				--n;
				i++;
			}

			return tmp;
		}
		counted& operator--() // override
		{
			++n;
			--i;

			return *this;
		}
		counted operator--(int) // override
		{
			counted tmp = *this;

			++n;
			i--;

			return tmp;
		}
		// operator+=
		// operator-=
	};

	template<class I>
	class sentinal : public std::iterator_traits<I> {
		I i, e;
	public:
		sentinal(I i, I e)
			: i(i), e(e)
		{ }
		operator bool() const
		{
			return i != e;
		}
		bool operator!() const
		{
			return !operator bool();
		}
		operator I()
		{
			return i;
		}
		operator const I& () const
		{
			return i;
		}

	};

	// class extrapolate
	// class memoize -> random access iterator

	//
	// Functions
	//

	// template<class C> inline auto reverse(C& c)
	// { return sentinal(c.rbegi(), c.rend()); }

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
	inline I skip(int n, I i)
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


}