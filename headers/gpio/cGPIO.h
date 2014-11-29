/*
 * cGPIO_int.h
 *
 *  Created on: 1/08/2014
 *      Author: juan
 */

#ifndef CGPIO_INT_H_
#define CGPIO_INT_H_

#include <iostream>
#include <fstream>
#include <string>
#include <unistd.h>
#include <fcntl.h> //open, read y write
#include <time.h>


using namespace std;

#define TIME_MESURE 10000000
#define PROM 10

class GPIO
{
	public:
		GPIO(string gpio, string dir = "in"); // Por defecto el pin sera una entrada
		~GPIO();
		bool is_open(void); //Funcion que retorna un booleano

		void 	setValue (int val);
		void 	setDir (string val);
		void 	setEdge (string edge);

		int 	getValue (void);
		string 	getDir (void);
		string 	getEdge (void);

		float 	read_time(void);
		float 	write_time(void);

	private:
		fstream fdir,fedge; 		/* Miembros con tiempo de acceso no critico */
		ofstream fexport,funexport;	/* Miembros con tiempo de acceso no critico */
		string gpioN;
		string pathValue, pathDirection, pathEdge; /* Direccion para cada fichero
													  de /sys/class/gpio/gpioN/...  */
		bool success;

	protected:
		int fd_value;				/* Miembro con tiempo de acceso critico */
};

#endif /* CGPIO_INT_H_ */
