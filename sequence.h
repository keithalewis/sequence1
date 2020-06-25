// sequence.h - interators having operator bool() const
#pragma once
#include <iterator>
#include <vector>

namespace seq {

	//!!! shouldn't need this
	template<class T>
	class ptr : public std::iterator_traits<T*> {
		T* p;
	public:
		ptr(T* p)
			: p(p)
		{
			std::vector<int> v(10);
			auto r = v.rbegin();
		}
		ptr& operator++()
		{
			++p;

			return *this;
		}
		ptr& operator++(int)
		{
			ptr tmp(&this);

			++p;

			return tmp;
		}
		T operator*()
		{
			return *p;
		}
	};

	template<class I>
	class counted : public I {
		size_t n;
	public:
		typename std::iterator_traits<I>::iterator_category iterator_category;
//		I::reference ref;
		counted(size_t n, I i)
			: I{ i }, n(n)
		{ }
		size_t size() const
		{
			return n;
		}
		operator bool() const {
			return 0 != n;
		}
		counted& operator++() // override
		{
			if (0 != n) {
				--n;
				I::operator++();
			}

			return *this;
		}
		counted operator++(int) // override
		{
			counted tmp = *this;

			if (0 != n) {
				--n;
				I::operator++();
			}

			return tmp;
		}
		/*
		counted& operator--() // override
		{
			if (0 != n) {
				++n;
				I::operator--();
			}

			return *this;
		}
		*/
	};

	template<class I>
	class sentinal : public I {
		I e;
	public:
		sentinal(I i, I e)
			: I{ i }, e(e)
		{ }
		operator bool() const
		{
			return !I::operator==(e);
		}
	};

	template<typename I>
	inline I skip(int n, I i)
	{
		std::advance(i, n);

		return i;
	}

	template<typename I>
	inline auto take(int n, I i)
	{
		return count(n, i);
	}


}