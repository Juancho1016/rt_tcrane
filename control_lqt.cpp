#include<stdio.h>
#include<iostream>
//#libreria "patino.h"

float LQT_control(float rk_rad, int x1_rad, int x3_rad)
{
	static int x1_rad_1=0, x3_rad_1=0, u1k_1=0;   ///INICIALIZACION DE LAS VARIABLES  ANTERIORES DEL CONTROLADOR
	int epk=0, u1k=0, u2k=0, uk=0, u3k=0, u4k=0, u5k=0;  ///VARIABLES DEL CONTROLADOR
	float m=0.0001, Ts=0.01;

	epk=rk_rad-x1_rad;  /// CALCULO DEL ERROR

	///COMPONENTES  DE LA LEY  DE CONTROL
	u1k=(u1k_1*Fac_LQT)+(epk*Ku1);
	u2k=x1_rad*Ku2;
	u3k=((x1_rad-x1_rad_1)/((m*(x1_rad-x1_rad_1))+Ts))*Ku3;
	u4k=x3_rad*Ku4;
	u5k=((x3_rad-x3_rad_1)/((m*(x3_rad-x3_rad_1))+Ts))*Ku5;

	///CALCULO DE LA LEY  DE CONTROL	
	uk=-u1k-u2k-u3k-u4k-u5k;

	///ACTUALIZACION DE LOS ESTADOS ANTERIORES
	x1_rad_1=x1_rad;
	x3_rad_1=x3_rad;
	u1_1=u1k;
	u2k_1=u2k;

	uk=uk/Fac_LQT; ///SE DIVIDE SOBRE EL FACTOR DE APROXIMACION, PARA OBTENER EL VALOR REAL

	if(uk>5)
	{
		uk=-5;/// SIGNO INVERTIDO POR LESTURA DE PLANTA
	}
	else if(uk<(-5))
	{
		uk=5; ///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
        ///COOMOOOOOO SACARRR EL SIGGNOOOOO
	return (int(uk*51)); /// NO SE SI SERA CON UN RETORNO --O -- SOLO LLAMANDO LA FUNCION DE PWM
}
