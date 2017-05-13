/*
  ==============================================================================

    rxjuce_DisposeBag.cpp
    Created: 13 May 2017 2:58:11pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_DisposeBag.h"

#include "rxjuce_DisposeBag_Impl.h"
#include "rxjuce_Disposable_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

DisposeBag::DisposeBag()
: impl(std::make_shared<Impl>()) {}

DisposeBag::~DisposeBag()
{
	impl->wrapped.unsubscribe();
}

void DisposeBag::insert(const Disposable& disposable)
{
	impl->wrapped.add(disposable.impl->wrapped);
}

RXJUCE_NAMESPACE_END
