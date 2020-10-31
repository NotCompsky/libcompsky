#pragma once


#include <boost/preprocessor/seq/for_each.hpp>
#include <boost/preprocessor/tuple/elem.hpp>

#define IS_CONTAINED_IN_VARARGS__X(z, n, data) or (x == data)

#define IS_CONTAINED_IN_VARARGS(type, the_compared, the_comparing_tuple) \
	/* lambda function only used to control the name of the compared object */ \
	[](type const x)->bool { \
		return (false BOOST_PP_SEQ_FOR_EACH(IS_CONTAINED_IN_VARARGS__X, _, the_comparing_tuple)); \
	}(the_compared)
