#ifndef SIGUNUSED_HANDLER_H_
#define SIGUNUSED_HANDLER_H_

#include <signal.h>
#include <assert.h>
#include <iostream>
#include "EventHandler.h"

class SIGUNUSED_Handler : public EventHandler {

	private:
		sig_atomic_t gracefulQuit;

	public:

        SIGUNUSED_Handler () {
			this->gracefulQuit = 0;
		}

        ~SIGUNUSED_Handler () {
		}

		virtual int handleSignal ( int signum ) {
            assert ( signum == SIGUNUSED );
			this->gracefulQuit = 1;
			return 0;
		}

		sig_atomic_t getGracefulQuit () {
			return this->gracefulQuit;
		}

};

#endif /* SIGUNUSED_HANDLER_H_ */
