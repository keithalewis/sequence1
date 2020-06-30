// sequence.cpp - test sequence
#include <cassert>
#include <vector>
#include "sequence.h"

using namespace seq;

template<class T>
int test_array() 
{
	{
		array<int> a({ 1, 2, 3 });
		assert(a.size() == 3);
		assert(a);
		assert(*a == 1);

		++a;
		assert(a);
		assert(*a == 2);
		
		++a;
		assert(a);
		assert(*a == 3);
		
		++a;
		assert(!a);
		
		--a;
		assert(a);
		assert(*a == 3);
		assert(a.size() == 1);
		
		a -= 2;
		assert(a);
		assert(*a == 1);
		assert(a.size() == 3);
		
		a += 1;
		assert(a);
		assert(*a == 2);
		assert(a.size() == 2);
	}
	{
		T i[] = { 1, 2, 3 };
		array<T> a(i);
		assert(a);
		assert(a.size() == 3);
	}

	return 0;
}
int test_array_int = test_array<int>();

int test_concatenate()
{
	{
		auto a = array({ 1, 2 });
		auto b = array({ 3, 4 });

		assert(equal(concatenate(a, b), array({ 1, 2, 3, 4 })));
	}
	{
		auto a = array({ 1 });
		auto b = array({ 2, 3, 4 });

		assert(equal(concatenate(a, b), array({ 1, 2, 3, 4 })));
	}
	{
		auto a = array<int>();
		auto b = array({ 1, 2, 3, 4 });

		assert(equal(concatenate(a, b), array({ 1, 2, 3, 4 })));
	}
	{
		auto b = array<int>();
		auto a = array({ 1, 2, 3, 4 });

		assert(equal(concatenate(a, b), array({ 1, 2, 3, 4 })));
	}
	/*
	{
		auto ab = array({ 1, 2 }), array({ 3, 4 };
		assert(equal(ab, array({ 1, 2, 3, 4 }));
	}
	*/


	return 0;
}
int test_concatenate_ = test_concatenate();

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

int test_range()
{
	std::vector<int> v = { 1, 2, 3 };
	auto s = make_range(v);
	assert(s);
	assert(*s == 1);

	++s;
	assert(s);
	assert(*s == 2);

	++s;
	assert(s);
	assert(*s == 3);

	++s;
	assert(!s);

	return 0;
}
int test_range_ = test_range();

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

int test_epsilon()
{
	auto s = power(0.5);
	auto e = epsilon(s);
	auto n = length(e);
	n = n;

	return 0;
}
int test_epsilon_ = test_epsilon();

int test_extrapolate()
{
	auto s = extrapolate(array({ 1, 2 }), 3);
	assert(s);
	assert(*s == 1);

	++s;
	assert(s);
	assert(*s == 2);

	++s;
	assert(s);
	assert(*s == 3);

	++s;
	assert(s);
	assert(*s == 3);

	return 0;
}
int test_extrapolate_ = test_extrapolate();

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

int test_null()
{
	{
		auto n = null("abc");
		assert(n);
		assert(*n == 'a');
		++n;
		assert(*n == 'b');
		++n;
		assert(*n == 'c');
		++n;
		assert(!n);
	}

	return 0;
}
int test_null_ = test_null();

int test_factorial()
{
	{
		pochhammer n;
		assert(n && *n == 1);
		assert(++n && *n == 1);
		assert(++n && *n == 2);
		assert(++n && *n == 6);
		assert(++n && *n == 24);
	}
	{
		double x = 1.23;
		pochhammer n(x);
		assert(n && *n == x);
		assert(++n && *n == x * (x + 1));
		assert(++n && *n == x * (x + 1) * (x + 2));
		assert(++n && *n == x * (x + 1) * (x + 2) * (x + 3));
	}
	{
		double x = 1.23;
		pochhammer n(x, -1);
		assert(n && *n == x);
		assert(++n && *n == x * (x - 1));
		assert(++n && *n == x * (x - 1) * (x - 2));
		assert(++n && *n == x * (x - 1) * (x - 2) * (x - 3));
	}

	return 0;
}
int test_factorial_ = test_factorial();

int test_sum()
{
	int i[] = { 1, 2, 3 };
	assert(6 == sum(counted(3, i)));
	
	return 0;
}
int test_sum_ = test_sum();

template<class T>
int test_exp()
{
	{
		T x = 1;
		auto xn = power(x);
		auto f = factorial<T>();
		auto ex = xn/f;
		/*
		assert(ex);
		assert(*ex == 1);
		++ex;
		assert(*ex == x);
		++ex;
		assert(*ex == x * x / 2);
		*/
		auto eex = epsilon(ex);
		auto n = length(eex);
		n = n;
		T expx = sum(eex);
		expx = expx;
	}

	return 0;
}
int test_exp_double = test_exp<double>();

int main()
{
	return 0;
}