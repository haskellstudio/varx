#include "JuceHeader.h"

#include "RxCpp/Rx/v2/src/rxcpp/rx.hpp"

namespace rxjuce {
	using namespace juce;
	
	template<class T>
	using shared_ptr = std::shared_ptr<T>;

#include "gui/rxjuce_Extensions.cpp"
#include "gui/rxjuce_Reactive.cpp"

#include "rx/internal/rxjuce_Disposable_Impl.cpp"
#include "rx/internal/rxjuce_DisposeBag_Impl.h"
#include "rx/internal/rxjuce_Observable_Impl.cpp"
#include "rx/internal/rxjuce_Observer_Impl.cpp"
#include "rx/internal/rxjuce_Scheduler_Impl.cpp"
#include "rx/internal/rxjuce_Subjects_Impl.cpp"

#include "rx/rxjuce_Disposable.cpp"
#include "rx/rxjuce_DisposeBag.cpp"
#include "rx/rxjuce_Observable.cpp"
#include "rx/rxjuce_Observer.cpp"
#include "rx/rxjuce_Scheduler.cpp"
#include "rx/rxjuce_Subjects.cpp"

#include "util/rxjuce_PrintFunctions.cpp"
#include "util/rxjuce_VariantConverters.cpp"

}
