//
//  rxjuce_Subject.h
//  RxJUCE
//
//  Created by Martin Finke on 27.04.17.
//
//

#pragma once

class Subject
{
public:
	Subject();
	
	void onNext(const juce::var& next) const;
	
	Observable getObservable() const;
	
private:
	class Internal;
	std::shared_ptr<Internal> internal;
};
