//
//  rxjuce_ValueSource.h
//  RxJUCE-Tests
//
//  Created by Martin Finke on 23.04.17.
//
//

#pragma once

#include "rxjuce_Observable.h"

namespace rxjuce {
	class ValueSource : public juce::Value::ValueSource
	{
	public:
		
		rxjuce::Observable getObservable() const;
	};
}
