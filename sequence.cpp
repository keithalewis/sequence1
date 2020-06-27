// sequence.cpp - test sequence
#include <cassert>
#include <vector>
#include "sequence.h"

using namespace seq;

int test_apply()
{
	{
		int i[] = { 1, 2, 3 };
		auto s = counted(3, i);
		auto fs = apply([](int i) { return i * i; }, s);
		assert(fs);
		assert(1 == *fs);
		++fs;
		assert(4 == *fs);
		assert(9 == *++fs);
		assert(!++fs);
	}
	{
		int i[] = { 1, 2, 3 };
		auto s = counted(3, i);
		auto fs = apply([](int i) { return i * i; }, s);
		std::vector<int> t;
		copy(fs, std::back_inserter(t));
		assert(t.size() == 3);
		assert(t[0] == 1);
		assert(t[1] == 4);
		assert(t[2] == 9);
	}
	return 0;
}
int test_apply_ = test_apply();

int test_filter()
{
	{
		auto i = counter<int>{};
		auto fi = filter([](int i) { return i % 2 == 1;  }, i);
		assert(fi);
		assert(1 == *fi);
		++fi;
		assert(3 == *fi);
	}
	{
		auto i = take(5,counter<int>{});
		auto fi = filter([](int i) { return i % 2 == 1;  }, i);
		assert(fi);
		assert(1 == *fi);
		++fi;
		assert(3 == *fi);
		++fi;
		assert(!fi);
	}
	{
		auto i = counter<int>{};
		auto fi = take(2, filter([](int i) { return i % 2 == 1;  }, i));
		assert(fi);
		assert(1 == *fi);
		++fi;
		assert(3 == *fi);
		++fi;
		assert(!fi);
	}

	return 0;
}
int test_filter_ = test_filter();

template<class T>
int test_constant()
{
	{
		constant<T> c(1);
		constant<T> c2(c);
		c = c2;

		assert(c);
		assert(*c == T(1));

		++c;
		assert(c);
		assert(*c == T(1));

	}

	return 0;
}
int test_constant_int = test_constant<int>();

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

int test_sum()
{
	int i[] = { 1, 2, 3 };
	assert(6 == sum(counted(3, i)));
	
	return 0;
}
int test_sum_ = test_sum();

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

	}

	return 0;
}