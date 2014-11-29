/*
 * cTIMER.h
 *
 *  Created on: 26/08/2014
 *      Author: juan
 */

#ifndef CTIMER_H_
#define CTIMER_H_

/* NOTA IMPORTANTE */
/* Se debe usar -lrt o incluirlo en las opciones para que compile */

#include <time.h>
#include <signal.h>

class TIMER {

	public:
		TIMER(int s_time = 1, int ns_time = 0);
		~TIMER();
		int get_Signum (void);
		void start(void);
		void setNewTime(int s_time, int ns_time);

	private:
		int mySignum;	// Señal emitida por el "this" timer
		struct itimerspec myTimer_propieties; 	/* Propiedades del Timer */
		struct sigevent myTimer_event;			/* Eventos del Timer */
		timer_t myTimer;						/* Timer */
		static int StackSignal();				/* Stack de Señales */
};

#endif /* CTIMER_H_ */
