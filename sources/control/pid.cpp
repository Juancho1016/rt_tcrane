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
	atom_r1.store(90);
	atom_x1.store(0);
	atom_x3.store(0);
	atom_uk.store(50);

	signal_emited2 = 0;

	pwm1 = new Soft_PWM(gpio_out, 7650, 30);
	my_direction = new GPIO("113", "out");
	arm = new GPIO("110", "in");
	pendulum = new GPIO("7", "in");
	my_arm = new POLL_GPIO("111");
	my_pendulum = new POLL_GPIO("20");  

	thread_bra = thread(&PID2::test_bra, this);
	thread_pen = thread(&PID2::test_pen, this);
	
	pwm1 -> run();
	pwm1 -> setAsync_OC(atom_uk.load());
}

void PID2::run(void)
{
	cout << "run" << endl;

	sigemptyset(&signal_set2);
	sigaddset(&signal_set2,get_Signum());
	sigprocmask(SIG_SETMASK, &signal_set2, NULL);
	signal(get_Signum(), dummy);
	
	thread_control = thread(&PID2::control, this);

	setNewTime(0,ms_resolution*1000000);
}

void PID2::control(void)
{
	//cout << "Digite la ref en grados" << endl;
	//cin >> ref1;
	//atom_r1.store(ref1);
	while(true)
	{
		sigwait(&signal_set2, &signal_emited2);
		//law_control();
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

void PID2::law_control(void)
{	
	//VARIABLES ATOMICAS
	r1_rad=(atom_r1.load())*(pi/180);
	r2_rad=0;
	x1_rad=(atom_x1.load())*(pi/144);
	x3_rad=(atom_x3.load())*(pi/1000);

	///CALCULO DEL ERROR
	e1k=(r1_rad-x1_rad)*0.05;
	e2k=(r2_rad-x3_rad)*0.05;

	//cout << "Hola ley de control" << endl;

	//cout << r1_rad << " " << r2_rad << " " << x1_rad << " " << x3_rad << endl;
	//cout << e1k << " " << e2k << endl;

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

	//cout << uk << endl;

	if(uk>5)
	{
		uk=5;///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	else if(uk<(-5))
	{
		uk=-5; ///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	uk=uk*51;
	/*if(uk<0)
	{
		pwm1 -> setAsync_OC(uk*(-1));
	}
	else
	{
		pwm1 -> setAsync_OC(uk);
	}*/
}

void PID2::dummy(int sig)
{
	/* Esta funcion es un dummy al cual se conectan todas las señales,
	 * ya que cuando se declara mas de un objeto de tipo PID
	 * las señales emitidas por cada uno causan interferencia entre si, puesto que
	 * un hilo determina que una señal emita no fue tenida en cuenta y aborta el programa
	 */
}
