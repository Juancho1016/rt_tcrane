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
	signal(get_Signum(),dummy);

	thread_control = thread(&PID2::control, this);
	thread_control.join(); //Asi solucione el error

	setNewTime(0,ms_resolution*100000000);
}

void PID2::control(void)
{
	float ref1, ref3=0, refx1, refx3, Output;
	Soft_PWM pwm1("168", 7650, 30);
	cout << "Digite la ref en grados" << endl;
	cin >> ref1;
	ref1=(ref1*pi)/180;
	pwm1.run();
	//cout << pwm1.getTOP() << endl;
	while(true)
	{
		//cout << ref1 << endl;
		//sigwait(&signal_set2, &signal_emited2);
		test_bra("30");
		refx1=(atom_x1.load())*(pi/144);
		test_pen("60");
		refx3=(atom_x3.load())*(pi/1000);
		cout << ref1 << " " << ref3 << " " << refx1 << " " << refx3 << endl;
		Output=law_control(ref1, ref3, refx1, refx3);
		cout << Output << endl;
		pwm1.setAsync_OC(Output);
	}
}


/* Destructor */
PID2::~PID2()
{
	thread_control.~thread();
}

int PID2::test_bra(string gpio)
{
	static int count_bra = 0;
	int d, infb;
	float refx1=30;
		POLL_GPIO my_int_gpio("20");
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
			//cout << "Wala!!!   " << "Contador: "<< ++count << endl<< endl<< endl;
			//cout << count << endl;//Agregado octubre 23
			return count_bra;
		}
}

int PID2::test_pen(string gpio)
{
	static int count_pen = 0;
	int f, infp;
	float refx3=30;
		POLL_GPIO my_int_gpio("30");
		GPIO my_gpio(gpio);
		//cout << "Esperando interrupcion" << endl;
		while (true)
		{
			infp=my_int_gpio.wait_until_edge();
			f=my_gpio.getValue();

			if(infp==0)
			{
				count_pen=0;
				atom_x3.store(count_pen);
			}
			if((infp>0)&&(f==1))
			{
				count_pen++;
				refx3=count_pen*(pi/2000);
				atom_x3.store(count_pen);
			}
			else if((infp>0)&&(f==0))
			{
				count_pen--;
				refx3=count_pen*(pi/2000);
				atom_x3.store(count_pen);
			}

			//cout << "Wala!!!   " << "Contador: "<< ++count << endl<< endl<< endl;
			//cout << count << endl;//Agregado octubre 23
			return count_pen;
		}
}

float PID2::law_control(float r1_rad, float r2_rad, float x1_rad, float x3_rad)
{
	static float u1k_1=0, u1k_2=0, e1k_1=0, e1k_2=0;
	float e1k, e2k, u1k, u2k, uk;	

	///CALCULO DEL ERROR
	e1k=r1_rad-x1_rad;
	e2k=r2_rad-x3_rad;

	//cout << r1_rad << " " << r2_rad << " " << x1_rad << " " << x3_rad << endl;

	///COMPONENTES  DE LA LEY  DE CONTROL
	u1k=(Ku1_4*u1k_1)+(Ku1_5*u1k_2)+(Ku1_1*e1k)+(Ku1_2*e1k_1)-(Ku1_3*e1k_2);
	u2k=K*e2k;

	///CALCULO DE LA LEY  DE CONTROL
	uk=u1k+u2k;

	///ACTUALIZACION DE LOS ESTADOS ANTERIORES
	e1k_2=e1k_1;
	e1k_1=e1k;
	u1k_2=u1k_1;
	u1k_1=u1k;

	//uk=uk/Fac_PID;

	if(uk>5)
	{
		uk=-5;///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	else if(uk<(-5))
	{
		uk=5; ///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	return(float(uk*51));
}

/* Descripcion en el interior de la funcion */
void PID2::dummy(int sig)
{
	/* Esta funcion es un dummy al cual se conectan todas las señales,
	 * ya que cuando se declara mas de un objeto de tipo PID
	 * las señales emitidas por cada uno causan interferencia entre si, puesto que
	 * un hilo determina que una señal emita no fue tenida en cuenta y aborta el programa
	 */
}
