// sequence.cpp - test sequence
#include <cassert>
#include <vector>
#include "sequence.h"
using namespace seq;

template<class T>
int test_counter()
{
	{
		counter<T> c;
		assert(c);
		assert(!!c);
		counter<T> c2(c);
		c = c2;
		assert(*c == 0);

		++c;
		assert(c);
		assert(*c == 1);

		assert(*c++ == 1);
		assert(c);
		assert(*c == 2);

		--c;
		assert(c);
		assert(*c == 1);
		assert(*c-- == 1);

		assert(c);
		assert(*c == 0);
		*c = T(3); // reset to 3
		assert(*c == 3);
	}
	{
		T i[] = { T(1), T(2), T(3) };
		T j[3];
		copy(take(3,i), j);
	}

	return 0;
}
int test_counter_int = test_counter<int>();
int test_counter_double = test_counter<double>();
int test_counter_float = test_counter<float>();

int main()
{

	{
		int i[] = { 1, 2, 3 };
		//ptr pi(i);
		//counted c(3, pi);
		//counted c(3, ptr(i));
		counted c(3, i);
		assert(c.size() == 3);
		assert(1 == *c);
		++c;
		assert(c.size() == 2);
		assert(2 == *c);
		assert(3 == *++c);
		assert(c.size() == 1);
		assert(c);
		++c;
		assert(c.size() == 0);
		assert(!c);
		++c;
		assert(c.size() == 0);
		assert(!c);
		--c;
		assert(c.size() == 1);
		assert(c);
		assert(*c == 3);

		assert(*c-- == 3);
		assert(c);
		assert(c.size() == 2);
		assert(*c == 2);
	}

	return 0;
}