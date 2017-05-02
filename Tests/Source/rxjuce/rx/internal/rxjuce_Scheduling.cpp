/*
  ==============================================================================

    rxjuce_Scheduling.cpp
    Created: 2 May 2017 11:12:37pm
    Author:  Martin Finke

  ==============================================================================
*/

#include "rxjuce_Scheduling.h"

RXJUCE_SOURCE_PREFIX

RXJUCE_NAMESPACE_BEGIN

namespace {
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

namespace scheduling {
	rxcpp::observe_on_one_worker juceMessageThread()
	{
		static const JUCEDispatcher dispatcher;
		return dispatcher.createWorker();
	}
	
	rxcpp::synchronize_in_one_worker rxcppEventLoop()
	{
		return rxcpp::synchronize_event_loop();
	}
	
	rxcpp::synchronize_in_one_worker newThread()
	{
		return rxcpp::synchronize_new_thread();
	}
}

RXJUCE_NAMESPACE_END
