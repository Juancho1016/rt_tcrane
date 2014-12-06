/* Main file */
#include <pthread.h>
#include "cTIMER.h"
#include "cSoft_PWM.h"
#include "cPOLL_GPIO.h"
#include "cGPIO.h"
#include "Gains_LQT_PID2.h"
#include "PID2.h"
#include "main.h"

using namespace std;

void test(void);
void test_PWM(void);

sem_t sinc;

int main()
{
	cout << "Hola Chitan y Tamayo" << endl;
	test();
	//hilo2=thread(&test_PWM);
	//while(true); 	
}

void test(void)
{
	PID2 my_pid("115", 0, 10);
	my_pid.run();
	sem_wait(&sinc);
	while(true);
}

void test_PWM(void)
{
	int OC;
	Soft_PWM pwm0("117", 7650, 30);
	GPIO my_direction("113", "out");
	pwm0.run();
	sem_post(&sinc);
	while(1)
	{
		OC=atom_uk.load();
		//cout << OC << endl;
		if(OC<0)
		{
			pwm0.setAsync_OC(OC*(-1));
			my_direction.setValue(1);
		}
		else
		{
			pwm0.setAsync_OC(OC);
			my_direction.setValue(0);
		}
		//cout << "Valor PWM 0" << endl;
		//cin >> x;
	}
}
