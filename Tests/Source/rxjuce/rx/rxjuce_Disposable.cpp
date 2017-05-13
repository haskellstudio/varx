/*
  ==============================================================================

    rxjuce_Disposable.cpp
    Created: 27 Apr 2017 7:09:19am
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Disposable.h"

#include "rxjuce_DisposeBag.h"

#include "rxjuce_Disposable_Impl.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

Disposable::Disposable(const std::shared_ptr<Impl>& impl)
: impl(impl) {}

void Disposable::dispose() const
{
	impl->wrapped.unsubscribe();
}

void Disposable::disposedBy(DisposeBag& disposeBag)
{
	disposeBag.insert(*this);
}

RXJUCE_NAMESPACE_END
