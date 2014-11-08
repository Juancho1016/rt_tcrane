/*
 * GPIO_int.cpp
 *
 *  Created on: 1/08/2014
 *      Author: juan
 */

#include "cGPIO.h"


/* Constructor, recibe el numero del gpio, por ejemplo "120" con comillas y la
 * direccion del GPIO "in" o "out"*/
GPIO::GPIO(string gpio, string dir)
{
	gpioN = gpio; //Variable para almacenar el numero del GPIO que manejara este objeto

	/* Exportando el GPIO */
		string tempFolder("/sys/class/gpio/export"); /* Export */
		fexport.open(tempFolder.c_str());
		fexport << gpioN; //Lo mismo que echo gpioN > export en consola
		if(!fexport.is_open())
		{
			success = false;
		}
		else
			success = true;

		fexport.close(); //OJO NO SE EXPORTA HASTA QUE NO SE CIERRA!!!

		/* Creando los manejadores de archivos para value, direction, edge */

		pathValue = "/sys/class/gpio/gpio"+gpioN+"/value"; //Value
		/* El fichero value es abierto de forma que no se requiera reabrir y cerrar */
		/* Perimosos de lectura y escritura, ademas de operacion en modo no bloqueo*/
		fd_value = open(pathValue.c_str(), O_RDWR | O_NONBLOCK);

		pathDirection = "/sys/class/gpio/gpio"+gpioN+"/direction"; //Direction

		pathEdge = "/sys/class/gpio/gpio"+gpioN+"/edge"; //Edge

		/* Asignando la direccion del gpio */
		setDir(dir);
}


/* Destructor, al culminar el programa cierra todos los ficheros y ademas realiza
 * el unexport del gpio
 */
GPIO::~GPIO()
{
	close(fd_value);
	funexport.open("/sys/class/gpio/unexport");
	funexport << gpioN; //Lo mismo que echo gpioN > unexport en consola
	funexport.close();
}


/* Si todos los ficheros se abrieron retornara un TRUE, de lo contrario retornara un FALSE*/
bool GPIO::is_open(void)
{
	return success;
}


/* Escribe el valor en el gpio, deberia ser un pin de salida */
void GPIO::setValue (int val)
{
	static int temp_val;
	temp_val = val + 48; /* adicionandole 48 del ASCII */
	write(fd_value,&temp_val,1); /* Se escribe el valor
	PRECAUCION: Asegurar escritura binaria en este archivo */
	lseek(fd_value, 0, SEEK_SET); /* Se debe reiniciar el puntero de escritura */
}


/* Lee el valor en el gpio, deberia ser un pin de entrada */
int GPIO::getValue (void)
{
	static char value;
	read(fd_value, &value, 1); /* Lectura del GPIO    */
	lseek(fd_value, 0, SEEK_SET); /* Se debe reiniciar el puntero de lectura */
	return (int(value) - 48); //Solo podra regresar 0 o 1, cualquier otro valor sera erroneo
}


/* Configura la dirreccion del gpio "in" o "out" */
void GPIO::setDir (string dir)
{
	fdir.open(pathDirection.c_str());
	fdir << dir;
	fdir.close();
}


/* Retorna la dirreccion del gpio "in" o "out" */
string GPIO::getDir (void)
{
	string direc;
	fdir.open(pathDirection.c_str());
	fdir >> direc;
	fdir.close();
	return direc;
}


/* Asigna el valor de edge para utilizar el gpio por interrupcion, gpio deberia ser "in"
 * Posibles valores:
 * rising
 * falling
 * both
 */
void GPIO::setEdge (string edge)
{
	fedge.open(pathEdge.c_str());
	fedge << edge;
	fedge.close();
}


/* Retorna el valor de edge para utilizar el gpio por interrupcion, gpio deberia ser "in"
 * Posibles valores:
 * rising
 * falling
 * both
 */
string GPIO::getEdge (void)
{
	string edge;
	fedge.open(pathEdge.c_str());
	fedge >> edge;
	fedge.close();
	return edge;
}


/* Convierte el GPIO en entrada y realiza la medicion de tiempo, ademas de esto realiza
 * un promedio realizando PROM veces el mismo procedimiento
 */
float GPIO::read_time()
{
	time_t start,end;
	setDir("in");
	/* Calculando el tiempo..... */
	cout << "Calculando el tiempo....." << endl;
	float times[PROM], result=0;
	for(int j = 0; j<PROM;j++)
	{
		time(&start);
		for(int i= 0; i<TIME_MESURE;i++)
				{
					getValue();
				}
		time(&end);
		times[j] = (difftime(end,start)/TIME_MESURE);
	}
	for (int i = 0; i<PROM; i++)
		result+=times[i];
	return (result/(PROM*1.0));
}

/* Convierte el GPIO en salida y realiza la medicion de tiempo, ademas de esto realiza
 * un promedio realizando PROM veces el mismo procedimiento
 */
float GPIO::write_time()
{
	time_t start,end;
	setDir("out");
	/* Calculando el tiempo..... */
	cout << "Calculando el tiempo....." << endl;
	float times[PROM], result=0;
		for(int j = 0; j<PROM;j++)
		{
			time(&start);
			for(int i= 0; i<TIME_MESURE;i++)
					{
						setValue(1);
					}
			time(&end);
			times[j] = (difftime(end,start)/TIME_MESURE);
		}
		for (int i = 0; i<PROM; i++)
			result+=times[i];
		return (result/(PROM*1.0));
}
