/*
  ==============================================================================

    varx_DisposeBag.cpp
    Created: 13 May 2017 2:58:11pm
    Author:  Martin Finke

  ==============================================================================
*/

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


