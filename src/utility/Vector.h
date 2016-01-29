#ifndef VECTOR_H_INCLUDED
#define VECTOR_H_INCLUDED

#include <boost/range/algorithm.hpp>
#include <boost/range/algorithm_ext.hpp>

template<typename T>
using vector = std::vector<T>;//boost::container::vector<T, boost::container::allocator<T>>;

#endif // VECTOR_H_INCLUDED
