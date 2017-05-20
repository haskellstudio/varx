/*
  ==============================================================================

    varx_Disposable.cpp
    Created: 27 Apr 2017 7:09:19am
    Author:  Martin Finke

  ==============================================================================
*/

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


