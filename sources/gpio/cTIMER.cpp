/*
 * cTIMER.cpp
 *
 *  Created on: 26/08/2014
 *      Author: juan
 */

#include "cTIMER.h"


/* Crea el timer con los parametros establecidos en segundos y microsegundos, este no
 * comienza a funcionar hasta no usar el metodo start() */
TIMER::TIMER(int s_time, int ns_time)
{
	mySignum = TIMER::StackSignal(); //Asigna la señal apartir de SIGRTMIN

	myTimer_event.sigev_notify = SIGEV_SIGNAL; 	/* Tipo de notificacion */
	myTimer_event.sigev_signo = mySignum; 		/* Señal a enviar */

	timer_create(CLOCK_REALTIME, &myTimer_event, &myTimer );

	/* Configuración del primer disparo */
	myTimer_propieties.it_value.tv_sec        = s_time;
	myTimer_propieties.it_value.tv_nsec       = ns_time;
	/* Configuración de los disparos periodicos */
	myTimer_propieties.it_interval.tv_sec     = s_time;
	myTimer_propieties.it_interval.tv_nsec    = ns_time;
}


/* Elimina el Timer */
TIMER::~TIMER()
{
	timer_delete(&myTimer);
}


/* Asigna el valor de la señal a emitir a partir de SIGRTMIN, cuando se cree un nuevo timer
 * tendra este valor +1, y asi, cuando se cree un timer N, tendra asignado SIGRTMIN+N
 * Nota: Tener cuidado con el maximo permitido SIGRTMAX
 */
int TIMER::StackSignal()
{
	static int signum = SIGRTMIN; //Stack para la señal emitida
	return signum++; // Postincremento, primero retorna el valor actual y luego lo incrementa
}


/* Funcion para obtener la señal que emitira "Este" timer */
int TIMER::get_Signum(void)
{
	return mySignum;
}


/* Funcion para darle inicio al timer */
void TIMER::start(void)
{
	/* Creacion del timer */
	timer_settime( myTimer, 0, &myTimer_propieties, NULL );
}


/* Funcion para darle un nuevo valor de periodicidad al timer, NO requiere llamar la
 * funcion start() */
void TIMER::setNewTime(int s_time, int ns_time)
{
	/* Configuración del primer disparo */
		myTimer_propieties.it_value.tv_sec        = s_time;
		myTimer_propieties.it_value.tv_nsec       = ns_time;
		/* Configuración de los disparos periodicos */
		myTimer_propieties.it_interval.tv_sec     = s_time;
		myTimer_propieties.it_interval.tv_nsec    = ns_time;

	/* Creacion del timer */
	timer_settime( myTimer, 0, &myTimer_propieties, NULL );
}
