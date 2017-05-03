/*
  ==============================================================================

    rxjuce_Scheduler.cpp
    Created: 3 May 2017 10:18:06pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Scheduler.h"

#include "rxjuce_Scheduler_Impl.h"

RXJUCE_SOURCE_PREFIX


namespace {
	using namespace juce;
	
	class JUCEDispatcher : private Timer {
	public:
		JUCEDispatcher()
		: runLoop(createRunLoop())
		{
			// The run loop didn't get initialized! Please report this as a bug.
			jassert(runLoop);
			
			startTimerHz(60);
		}
		
		rxcpp::observe_on_one_worker createWorker() const
		{
			return rxcpp::observe_on_run_loop(*runLoop);
		}
		
	private:
		typedef std::unique_ptr<rxcpp::schedulers::run_loop> RunLoop_ptr;
		const RunLoop_ptr runLoop;
		
		static RunLoop_ptr createRunLoop()
		{
			// Create the run loop on the message thread:
			RunLoop_ptr rl;
			static const auto create = [](void* rl) {
				// Not called from the JUCE message thread! Please report this as a bug.
				jassert(MessageManager::getInstance()->isThisTheMessageThread());
				static_cast<RunLoop_ptr*>(rl)->reset(new rxcpp::schedulers::run_loop);
				return static_cast<void*>(nullptr);
			};
			
			// This will block if this thread isn't the message thread:
			MessageManager::getInstance()->callFunctionOnMessageThread(create, &rl);
			
			return rl;
		}
		
		void timerCallback() override
		{
			// Run any scheduled actions
			while(!runLoop->empty() && runLoop->peek().when < runLoop->now())
				runLoop->dispatch();
		}
	};
}

RXJUCE_NAMESPACE_BEGIN

Scheduler::Scheduler(const std::shared_ptr<Impl>& impl)
: impl(impl) {}

Scheduler Scheduler::messageThread()
{
	static const JUCEDispatcher dispatcher;
	const auto worker = dispatcher.createWorker();
	return std::make_shared<Scheduler::Impl>([worker](const rxcpp::observable<juce::var>& observable) {
		return observable.observe_on(worker);
	});
}

Scheduler Scheduler::backgroundThread()
{
	return std::make_shared<Scheduler::Impl>([](const rxcpp::observable<juce::var>& observable) {
		return observable.observe_on(rxcpp::serialize_event_loop());
	});
}

Scheduler Scheduler::newThread()
{
	return std::make_shared<Scheduler::Impl>([](const rxcpp::observable<juce::var>& observable) {
		return observable.observe_on(rxcpp::serialize_new_thread());
	});
}

RXJUCE_NAMESPACE_END
