/*
 * cSoft_PWM.cpp
 *
 *  Created on: 28/08/2014
 *      Author: juan
 */
#include "cSoft_PWM.h"

/* Se inicializa el GPIO  gpio_out como salida, y su respectivo timer
 * es nulo hasta que se calculas los tiempos
 * El Periodo corresponde a la señal PWM, que es muy diferente al perido del Timer,
 * esté esta relaciona con la resolucion
 */
Soft_PWM::Soft_PWM
				(string gpio_out, int us_per, int us_res) :
														GPIO(gpio_out,"out"),
														TIMER(0,0)
{
	us_resolution = us_res; 	//Resolución (usegundos) inicializada en 10us - 10.000ns
	us_period = us_per;

	/* El periodo debe ser multiplo de la resolucion*/
	/* El perido del PWM debe ser mayor a la resolucion */
	/* El GPIO fue correctamente exportado*/
	//if( (us_period > us_resolution) && ((us_period%us_resolution) == 0) && (is_open()))
	if( (us_period > us_resolution) && ((us_period%us_resolution) == 0))
	{
		correct = true;

		TOP_value = us_period/us_resolution; /*Valor maximo del contreo */
		count = 0; /* Se inicializa el valor de conteo */
		compare_value = 0;
		atom_compare_value.store(0);

		signal_emited = 0;
	}
	else
		correct = false;

}


/* Pone en funcionamiento el TIMER y la salida de comparación
 * FUNCION GENERADORA DE SEÑAL PWM
 */
void Soft_PWM::run(void)
{
	/* Se prepara la mascara de señales para esperar la señal del timer */
		sigemptyset(&signal_set);
		sigaddset(&signal_set,get_Signum());
		sigprocmask(SIG_SETMASK, &signal_set, NULL);// SIG_SETMASK->termina la llamada, otro controlador puede invocarse 
		signal(get_Signum(),dummy); //Explicacion en la funcion dummy

	/* Se crea un hilo desde este mismo objeto, que correra una funcion miembro
	 * la cual tiene acceso a los miembros de la clase */
		thread_calculate = thread(&Soft_PWM::calculate, this);

	/* Se dispara el Timer y empieza tod.....*/
		setNewTime(0,us_resolution*1000);
}


/* Funcion para calcular la salida del pin */
void Soft_PWM::calculate(void)
{
	while(true)
	{
		sigwait(&signal_set,&signal_emited);

			if (count == 0)
				setValue(1);// Escritura del valor en el gpio

			if(count == compare_value)
				setValue(0);// Escritura del valor en el gpio

			/* Control y actualizacion del conteo para calcular el valor del PWM */
			count ++;

			if(count == TOP_value)
				{
				count = 0;
				compare_value = atom_compare_value.load();
				}
	}
}


/* Destructor */
Soft_PWM::~Soft_PWM()
{
	thread_calculate.~thread();
}


/* Funcion para modificar asincronamente el valor de OC */
void Soft_PWM::setAsync_OC(int OC)
{
	atom_compare_value.store(OC);
}

/* Funcion para leer asincronamente el valor de OC */
int Soft_PWM::getAsync_OC(void)
{
	return atom_compare_value.load();
}


/* Funcion para leer el valor maximo de conteo */
int Soft_PWM::getTOP(void)
{
	return TOP_value;
}

/* Descripcion en el interior de la funcion */
void Soft_PWM::dummy(int sig)
{
	/* Esta funcion es un dummy al cual se conectan todas las señales,
	 * ya que cuando se declara mas de un objeto de tipo Soft_PWM
	 * las señales emitidas por cada uno causan interferencia entre si, puesto que
	 * un hilo determina que una señal emita no fue tenida en cuenta y aborta el programa
	 */
}
