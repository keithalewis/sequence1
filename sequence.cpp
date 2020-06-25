// sequence.cpp - test sequence
#include <cassert>
#include "sequence.h"
using namespace seq;

int main()
{
	//test_reverse();

	{
		int i[] = { 1, 2, 3 };
		//ptr pi(i);
		//counted c(3, pi);
		counted c(3, ptr(i));
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
		/*
		--c;
		assert(c.size() == 1);
		assert(c);
		assert(*c == 3);
		*/
	}

	return 0;
}