/*
 * PID2.cpp
 *
 *  Created on: 28/08/2014
 *      Author: andres
 */
#include "PID2.h"

PID2::PID2
				(string gpio_out, int ms_per, int ms_res) :
														GPIO(gpio_out,"out"),
														TIMER(0,0)
{
	ms_resolution = ms_res;
	ms_period = ms_per;

	/* Cuidado que con el join() bloquea el flujo del programa */
	atom_r1.store(0);
	atom_r3.store(0);
	atom_x1.store(0);
	atom_x3.store(0);

	signal_emited2 = 0;
}

void PID2::run(void)
{
	cout << "run" << endl;
	sigemptyset(&signal_set2);
	sigaddset(&signal_set2,get_Signum());
	sigprocmask(SIG_SETMASK, &signal_set2, NULL);
	signal(get_Signum(), pid);
	thread_control = thread(&PID2::control, this);
	thread_inter_bra = thread(&PID2::test_bra, this, "110");
	thread_inter_pen = thread(&PID2::test_pen, this, "7");

	setNewTime(0,ms_resolution*100000000);
}

void PID2::control(void)
{
	float ref1, ref3=0, refx1, refx3;
	int Output, ref_x1[73], i;

	/* Entradas a la planta */
	Soft_PWM pwm1("117", 7650, 30);
	GPIO my_gpio2("113", "out");
	
	cout << "Digite la ref en grados" << endl;
	cin >> ref1;
	atom_r1.store(ref1);
	pwm1.run();
	while(true)
	{
		//thread_inter_bra;
		sigwait(&signal_set2, &signal_emited2);
	}
}


/* Destructor */
PID2::~PID2()
{
	thread_control.~thread();
	thread_inter_bra.~thread();
}

void PID2::test_bra(string gpio)
{
	static int count_bra = 0;
	int d, infb, i;
	int refx1;
		POLL_GPIO my_int_gpio("111");
		GPIO my_gpio(gpio);
		//cout << "Esperando interrupcion" << endl;
		while (true)
		{
			infb=my_int_gpio.wait_until_edge();
			d=my_gpio.getValue();

			if(infb==0)
			{
				count_bra=0;
				atom_x1.store(count_bra);
			}
			if((infb>0)&&(d==0))
			{
				count_bra++;
				atom_x1.store(count_bra);
			}
			else if((infb>0)&&(d==1))
			{
				count_bra--;
				atom_x1.store(count_bra);
			}
			//cout << count << endl;
			//return count_bra;
		}
}

void PID2::test_pen(string gpio2)
{
	static int count_pen = 0;
	int f, infp;
	float refx3;
		POLL_GPIO my_int_gpio2("20");
		GPIO my_gpio2(gpio2);
		//cout << "Esperando interrupcion" << endl;
		while (true)
		{
			infp=my_int_gpio2.wait_until_edge();
			f=my_gpio2.getValue();
			//refx3=atom_r1.load();
			//cout << refx3 << endl;

			if(infp==0)
			{
				count_pen=0;
				atom_x3.store(count_pen);
			}
			if((infp>0)&&(f==0))
			{
				count_pen++;
				atom_x3.store(count_pen);
			}
			else if((infp>0)&&(f==1))
			{
				count_pen--;
				atom_x3.store(count_pen);
			}

			//cout << "Wala!!!   " << "Contador: "<< ++count << endl<< endl<< endl;
			//cout << count << endl;//Agregado octubre 23
			//return count_pen;
		}
}

/* Descripcion en el interior de la funcion */
void PID2::pid(int sig)
{
	static float u1k_1=0, u1k_2=0, e1k_1=0, e1k_2=0;
	float r1_rad, r2_rad, x1_rad, x3_rad;
	float e1k, e2k, u1k, u2k, uk;

	//r1_rad=(atom_r1.load())*(pi/180);
	//r2_rad=0;
	//x1_rad=(atom_x1.load())*(pi/144);
	//x3_rad=(atom_x3.load())*(pi/1000);	

	///CALCULO DEL ERROR
	e1k=(r1_rad-x1_rad)*0.05;
	e2k=(r2_rad-x3_rad)*0.05;

	cout << r1_rad << " " << r2_rad << " " << x1_rad << " " << x3_rad << endl;
	cout << e1k << " " << e2k << endl;

	///COMPONENTES  DE LA LEY  DE CONTROL
	u1k=((Ku1_4*u1k_1)+(Ku1_5*u1k_2)+(Ku1_1*e1k)+(Ku1_2*e1k_1)-(Ku1_3*e1k_2))*0.9;
	u2k=(K*e2k)*1.5;

	///CALCULO DE LA LEY  DE CONTROL
	uk=(u1k+u2k)*(-1);

	///ACTUALIZACION DE LOS ESTADOS ANTERIORES
	e1k_2=e1k_1;
	e1k_1=e1k;
	u1k_2=u1k_1;
	u1k_1=u1k;

	//uk=uk/Fac_PID;

	cout << uk << endl;

	if(uk>5)
	{
		uk=5;///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	else if(uk<(-5))
	{
		uk=-5; ///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
}