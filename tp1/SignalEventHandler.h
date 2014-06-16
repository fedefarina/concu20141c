#ifndef SIGNALEVENT_HANDLER_H_
#define SIGNALEVENT_HANDLER_H_

#include <signal.h>
#include <assert.h>
#include <iostream>
#include "EventHandler.h"

class SignalEventHandler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

        SignalEventHandler () {
			this->gracefulQuit = 0;
		}

        ~SignalEventHandler () {
		}

		virtual int handleSignal ( int signum ) {
            assert(signum>0);
			this->gracefulQuit = 1;
			return 0;
		}

        void reset(){
            this->gracefulQuit=0;
        }

		sig_atomic_t getGracefulQuit () {
			return this->gracefulQuit;
		}

};

#endif /* SIGNALEVENT_HANDLER_H_ */
