#include <stdio.h>
#include <iostream>
#include "Gains_LQT_PID2.h"

float PID_controler(float r1_rad, int r2_rad, int x1_rad, int x3_rad)
{
	static int u1k_1=0, u1k_2, e1k_1=0, e1k_2=0;
	int e1k=0, e2k=0,u1k=0, u2k=0, uk=0;
	
	cout << r1_rad << endl;

	/// CALCCULO DEL ERROR
	e1k=r1_rad-x1_rad;
	e2k=r2_rad-x3_rad;

	///COMPONENTES  DE LA LEY  DE CONTROL
	u1k=(Ku1_4*u1k_1)+ (Ku1_5*u1k_2)+(Ku1_1*e1k)+(Ku1_2*e1k_1)-(Ku1_3*e1k_2);
	u2k=K*e2k;

	///CALCULO DE LA LEY  DE CONTROL
	uk=u1k+u2k;

	///ACTUALIZACION DE LOS ESTADOS ANTERIORES
	e1k_2=e1k_1;
	e1k_1=e1k;
	u1k_2=u1k_1;
	u1k_1=u1k;

	uk=uk/Fac_PID;

	if(uk>5)
	{
		uk=-5;///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	else if(uk<(-5))
	{
		uk=5; /// SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	return(int(uk*51)); /// NO SE SI SERA CON UN RETORNO --O -- SOLO LLAMANDO LA FUNCION DE PWM
}
