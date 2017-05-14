#pragma once

#define DONT_SET_USING_JUCE_NAMESPACE 1
#include "JuceHeader.h"

#include <exception>
#include <functional>
#include <memory>
#include <map>
#include <type_traits>
#include <utility>
#include <initializer_list>
#include <typeinfo>

#define RXJUCE_NAMESPACE_BEGIN namespace rxjuce {
#define RXJUCE_NAMESPACE_END }

#define RXJUCE_SOURCE_PREFIX namespace rxjuce { \
using std::placeholders::_1; \
using namespace juce; \
\
template<class T> \
using shared_ptr = std::shared_ptr<T>; \
template<typename T, typename D = std::default_delete<T>> \
using unique_ptr = std::unique_ptr<T, D>; \
}

typedef std::exception_ptr Error;


#warning Move this to source prefix
#if __cplusplus == 201103L
// make_unique by Stephan T. Lavavej, Revision 1
// https://isocpp.org/files/papers/N3656.txt
namespace std {
	template <class T>
	struct _Unique_if {
		typedef std::unique_ptr<T> _Single_object;
	};
	
	template <class T>
	struct _Unique_if<T[]> {
		typedef std::unique_ptr<T[]> _Unknown_bound;
	};
	
	template <class T, size_t N>
	struct _Unique_if<T[N]> {
		typedef void _Known_bound;
	};
	
	template <class T, class... Args>
	typename _Unique_if<T>::_Single_object
	make_unique(Args&&... args) {
		return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
	}
	
	template <class T>
	typename _Unique_if<T>::_Unknown_bound
	make_unique(size_t n) {
		typedef typename std::remove_extent<T>::type U;
		return std::unique_ptr<T>(new U[n]());
	}
	
	template <class T, class... Args>
	typename _Unique_if<T>::_Known_bound
	make_unique(Args&&...) = delete;
}
#endif
