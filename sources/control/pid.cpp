/*
 * PID2.cpp
 *
 *  Created on: 28/08/2014
 *      Author: andres
 */
#include "PID2.h"
#include "main.h"

PID2::PID2(string gpio_out, int ms_per, int ms_res) : GPIO(gpio_out,"out"), TIMER(0,0)
{
	ms_resolution = ms_res;
	ms_period = ms_per;

	/* Cuidado que con el join() bloquea el flujo del programa */
	atom_r1.store(0);
	atom_x1.store(0);
	atom_x3.store(0);
	atom_uk.store(0);

	signal_emited2 = 0;

	pwm1 = new Soft_PWM(gpio_out, 7650, 30);
	my_direction = new GPIO("113", "out");
	arm = new GPIO("110", "in");
	pendulum = new GPIO("7", "in");
	my_arm = new POLL_GPIO("111");
	my_pendulum = new POLL_GPIO("20");  

	thread_bra = thread(&PID2::test_bra, this);
	thread_pen = thread(&PID2::test_pen, this);
}

void PID2::run(void)
{
	cout << "run" << endl;

	sigemptyset(&signal_set2);
	sigaddset(&signal_set2,get_Signum());
	sigprocmask(SIG_SETMASK, &signal_set2, NULL);
	signal(get_Signum(), dummy);
	
	thread_control = thread(&PID2::control, this);
	//thread_pwm = thread(&PID2::pwm, this);
	//pwm();

	setNewTime(0,ms_resolution*1000000);
}

void PID2::control(void)
{
	cout << "Digite la ref en grados" << endl;
	cin >> ref1;
	atom_r1.store(ref1);
	while(true)
	{
		sigwait(&signal_set2, &signal_emited2);
		//law_control();
		//pwm();
	}
}


/* Destructor */
PID2::~PID2()
{
	delete pwm1;
	delete my_direction;
	delete arm;
	delete pendulum;
	delete my_arm;
	delete my_pendulum;
	thread_control.~thread();
	thread_bra.~thread();
	thread_pen.~thread();
	//thread_pwm.~thread();
}

void PID2::test_bra(void)
{
	while(true)
	{
		infb=my_arm -> wait_until_edge();
		b=arm -> getValue();
		//cout << "Hola contador brazo" << endl;

		if(infb==0)
		{
			count_bra=0;
			atom_x1.store(count_bra);
		}
		if((infb>0)&&(b==0))
		{
			count_bra++;
			atom_x1.store(count_bra);
		}
		else if((infb>0)&&(b==1))
		{
			count_bra--;
			atom_x1.store(count_bra);
		}
	}
}

void PID2::test_pen(void)
{
	while (true)
	{
		infp=my_pendulum -> wait_until_edge();
		p=pendulum -> getValue();
		//cout << "Hola contador pendulo" << endl;

		if(infp==0)
		{
			count_pen=0;
			atom_x3.store(count_pen);
		}
		if((infp>0)&&(p==0))
		{
			count_pen++;
			atom_x3.store(count_pen);
		}
		else if((infp>0)&&(p==1))
		{
			count_pen--;
			atom_x3.store(count_pen);
		}
	}
}

void PID2::dummy(int sig)
{
	/* Esta funcion es un dummy al cual se conectan todas las señales,
	 * ya que cuando se declara mas de un objeto de tipo PID
	 * las señales emitidas por cada uno causan interferencia entre si, puesto que
	 * un hilo determina que una señal emita no fue tenida en cuenta y aborta el programa
	 */
}
