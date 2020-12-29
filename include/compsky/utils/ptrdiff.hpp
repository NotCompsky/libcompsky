#pragma once


namespace compsky {
namespace utils {


template<typename P,  typename Q>
std::size_t ptrdiff(P* p,  Q* q){
	// WARNING: Assumes p >= q  - hence why the type is not std::ptrdiff_t
	return reinterpret_cast<std::uintptr_t>(p) - reinterpret_cast<std::uintptr_t>(q);
}


}
}
