# Sequence

A _sequence_ is an iterator that has an `operator bool() const` member to
indicate the end instead of checking for equality with an `end` sentinal.
This allows for cleaner, composable algorithms.

When Raymond Chen points out 
[the STL sucks](https://devblogs.microsoft.com/oldnewthing/20200228-00/) you
know it's bad. 

> I’ve got one type of collection and I want to apply a function to each member of the collection, thereby producing a new collection.
> Surely there’s a standard pattern for this?

Using sequences the solution is `apply(f, s)` to create a new (lazy) sequence.
This can be used with `copy` to populate a container. For example
```
	std::vector<int> src{1,2,3}, dest;
	copy(apply(f, seq(src)), std::back_inserter(dest));
```
will fill dest with `{f(1),f(2),f(3)}`. Note how sequences play nicely with STL.
Even though `back_insterter` does not have `operator bool() const` this still
works since copying is controlled by `apply` which uses that from `seq(src)`.

```
expx = sum(epsilon(pow(x)/factorial<>{}));
```