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
	//test();
	test_PWM(); 	
}

void test(void)
{
	PID2 my_pid("117", 0, 10);
	my_pid.run();
	while(true);
}

void test_PWM(void)
{
	int OC, x;
	Soft_PWM pwm0("117", 7650, 30);
	GPIO my_direction("113", "out");
	pwm0.run();
	while(1)
	{
		cout << "Valor PWM 0" << endl;
		cin >> x;
		pwm0.setAsync_OC(x);
	}
}
