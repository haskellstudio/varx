//
//  rxjuce_Prefixes.h
//  RxJUCE
//
//  Created by Martin Finke on 27.04.17.
//
//

#define DONT_SET_USING_JUCE_NAMESPACE 1
#include "JuceHeader.h"

#include <functional>
#include <memory>

#define RXJUCE_NAMESPACE_BEGIN namespace rxjuce {
#define RXJUCE_NAMESPACE_END }

#define RXJUCE_SOURCE_PREFIX namespace rxjuce { \
using namespace juce; \
\
template<class T> \
using shared_ptr = std::shared_ptr<T>; \
}
