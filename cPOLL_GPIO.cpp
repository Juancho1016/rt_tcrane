/*
 * cPOLL_GPIO.cpp
 *
 *  Created on: 15/08/2014
 *      Author: juan
 */

/* Posibles banderas para activar en events
		POLLIN		0x001		There is data to read.
		POLLPRI		0x002		There is urgent data to read.
		POLLOUT		0x004		Writing now will not block.
		POLLRDNORM	0x040		Normal data may be read.
		POLLRDBAND	0x080		Priority data may be read.
		POLLWRNORM	0x100		Writing now will not block.
		POLLWRBAND	0x200		Priority data may be written.
		Event types always implicitly polled for.  These bits need not be set in
		events', but they will appear in `revents' to indicate the status of
		the file descriptor.
		POLLERR		0x008		Error condition.
		POLLHUP		0x010		Hung up.
		POLLNVAL	0x020		Invalid polling request.*/

#include "cPOLL_GPIO.h"


/* Constructor */
POLL_GPIO:: POLL_GPIO(string gpio, string int_edge) : GPIO(gpio,"in")
{
	setEdge(int_edge); /* Se establece el flanco que disparará la interrupción */
	GPIO_int.fd = fd_value; /* Se asigna el manejador de fichero a la estructura */
	GPIO_int.events = POLLPRI | POLLERR; /* Flags activadas -> There is urgent data to read */

	fcn_return = -1; 	/* Se inicializa en -1 que es el valor de error */
	Read_Value = '0';	/* Variable que almacenara el resultado de la funcion read */

	fcn_return = read(GPIO_int.fd, &Read_Value, 1); /* Lectura antes del primer poll
	para evitar un disparo de interrupcion falso 	 	 */
	lseek(GPIO_int.fd, 0, SEEK_SET);/* Se debe reiniciar el puntero
	de lectura de lo contrario read retorna error */

}


/* Destructor */
POLL_GPIO::~POLL_GPIO()
{
	close(GPIO_int.fd);
}


/* Bloqueo del programa hasta que ocurra una interrupcion */
int POLL_GPIO::wait_until_edge(void)
{
	fcn_return = poll(&GPIO_int,1,2); //Valor negativo significa que espera un tiempo infinito
	//cout << fcn_return << endl;

	if(fcn_return > 0)
			{
				if(GPIO_int.revents & POLLPRI)
				{
					read(GPIO_int.fd, &Read_Value, 1); /* Se debe leer en la interrupcion de lo contrario
					la interrupcion de dispara constantemente */
					lseek(GPIO_int.fd, 0, SEEK_SET); /* Se debe reiniciar el puntero
					de lectura de lo contrario read retorna error */
				}
			}
	return fcn_return;
}

