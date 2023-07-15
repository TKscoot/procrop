#include <tuple>
#include <utility>
#include <algorithm>
#include <iostream>

//This is a C++14 answer.  Everything can be translated to C++11, just less pretty.

template<class F, class Base=std::less<>>
auto order_by( F&& f, Base&& b={} ) {
	return
			[f=std::forward<F>(f), b = std::forward<Base>(b)]
					(auto const& lhs, auto const& rhs)
					->bool
			{
				return b( f(lhs), f(rhs) );
			};
}
//`order_by` takes a projection and optionally a comparison function object, and returns a comparison function object that applies the projection then either `std::less<>` or the comparison function object.

//This is useful when sorting or searching, as C++ algorithms require comparison function objects, while projections are easy to write.

template<class A, class B>
struct binary_overload_t:A,B{
	using A::operator();
	using B::operator();
	binary_overload_t( A a, B b ):A(std::move(a)), B(std::move(b)) {}
};
template<class A, class B>
binary_overload_t< A, B >
binary_overload( A a, B b ) {
	return { std::move(a), std::move(b) };
}
//`binary_overload` lets you overload function objects.

template<class T>
struct valid_range_t {
	T start, finish;
};
//This represents a valid range.  I could just use `std::pair`, but I prefer types with meaning.

template<class T, class V>
struct ranged_value_t {
	valid_range_t<T> range;
	V value;
};
template<class T, class It>
auto find_value( It begin, It end, T const& target )
-> decltype(std::addressof( begin->value ))
{
	// project target into target
	// and a ranged value onto the lower end of the range
	auto projection = binary_overload(
			[]( auto const& ranged )->T const& {
				return ranged.range.finish;
			},
			[]( T const& t )->T const& {
				return t;
			}
	);
	//
	auto it = std::upper_bound( begin, end,
			target,
			order_by( projection )
	);
	if (it == end) return nullptr;
	if (target < it->range.start) return nullptr;
	return std::addressof( it->value );
}