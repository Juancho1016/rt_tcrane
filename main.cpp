/* Main file */
#include <iostream>
#include "main.h"

using namespace std;

void test(void);
void test_PWM(void);

int main()
{
	cout << "Hola Chitan y Tamayo" << endl;
	test();
	//test_PWM();
	while(true); /* (jjescof) Chitan usted crea todo y el programa inmediatamente
														muere, matando todos los hilos y por eso lo
														solucionaste con el join, pero es un error, 
														puesto que va bloquear el flujo del
														programa, el join deja la secuencia en espera hasta
														que el hilo muere, y el hilo nunca muere */
	return 0;	
}

void test(void)
{
	PID2 my_pid("115", 0, 10);
	my_pid.run();
}

void test_PWM(void)
{
	Soft_PWM pwm0("168",7650,30);
	//cout << out << endl;
	//Soft_PWM pwm1("20",40);
	pwm0.run();
	//pwm1.run();
	int x;
	cout << pwm0.getTOP() << endl;
	while(1)
	{
		cout << "Valor PWM 0" << endl;
		cin >> x;
		pwm0.setAsync_OC(x);
		//cout << "Valor PWM 1" << endl;
		//cin >> x;
		//pwm1.setAsync_OC(x);
	}
}
