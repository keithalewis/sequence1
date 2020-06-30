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
Even though `back_inserter` does not have `operator bool() const` this still
works since `copy` is controlled by `apply` which uses `seq(src)` to determine
the end of the sequence.

Sequences are handy for numerical computations. For example `pow(x)` is the
sequence `{1,x,x^2,...}` and `factorial` is {0!,1!,2!,...}. The function
`epsilon` truncates a sequence when it encounters a term less than
machine epsilon. Since the exponential function
<math>e<sup>x</sup> = &sum; <sub>n &ge; 0</sub> x<sup>n</sup>/n!</math> 
we can approximate this with

```
template<floating_point T>
T exp(T x) { return sum(epsilon(pow(x)/factorial<T>{})); }
```
where
```
template<typename T> concept floating_point = std::is_floating_point_v<T>;
```