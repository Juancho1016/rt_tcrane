/*
 * PID.cpp
 *
 *  Created on: 28/08/2014
 *      Author: juan
 */

#include "PID.h"
#include "Gains_LQT_PID2.h"

PID::PID(int seg, int ms_res) : POLL_GPIO("120", "falling"), TIMER(0,0) /*GPIO(gpio_out,"in") Soft_PWM("168",7650,30)*/
{
	ms_resolution=ms_res;
	s_resolution=seg;
	signal_emited2=0;
	atom_refx1.store(0);
	atom_refx3.store(0);
	atom_ref1.store(0);
	atom_ref3.store(0);
}

void PID::run(void)
{
	sigemptyset(&signal_set);
	sigaddset(&signal_set,get_Signum());
	sigprocmask(SIG_SETMASK, &signal_set, NULL); 
	signal(get_Signum(),dummy);
	
	control_thread = thread(&PID::control, this);
	
	setNewTime(0,ms_resolution*1000);
}

void PID::control(void)
{
	static float refx1, refx3, ref3=0, ref1=1.5708;
	int Output;
	//Soft_PWM pwm1("168",7650,30);
	//pwm1.run();
	while(true)
	{
		sigwait(&signal_set, &signal_emited2);
		test_bra("115");
		test_pen("60");
		x1_rad=atom_refx1.load();
		refx1=x1_rad*(pi/144);
		x3_rad=atom_refx3.load();
		refx3=x3_rad*(pi/2000);
		//Output=calc(ref1, ref3, refx1, refx3);
		//pwm1.setAsync_OC(Output);
	}
}

int PID::test_bra(string gpio)
{
	static int count_bra = 0;
	int d, infb;
	float refx1;
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
				atom_refx1.store(refx1);
			}
			if((infb>0)&&(d==0))
			{
				count_bra++;
				atom_refx1.store(refx1);
			}
			else if((infb>0)&&(d==1))
			{				
				count_bra--;
				atom_refx1.store(refx1);
			}			
			//cout << "Wala!!!   " << "Contador: "<< ++count << endl<< endl<< endl;
			//cout << count << endl;//Agregado octubre 23
			return count_bra;
		}
}

int PID::test_pen(string gpio)
{
	static int count_pen = 0;
	int f, infp;
	float refx3;
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
				atom_refx3.store(refx3);
			}
			if((infp>0)&&(f==1))
			{
				count_pen++;
				refx3=count_pen*(pi/2000);
				atom_refx3.store(refx3);
			}
			else if((infp>0)&&(f==0))
			{
				count_pen--;
				refx3=count_pen*(pi/2000);
				atom_refx3.store(refx3);
			}

			//cout << "Wala!!!   " << "Contador: "<< ++count << endl<< endl<< endl;
			//cout << count << endl;//Agregado octubre 23
			return count_pen;
		}
}

int PID::calc(float r1_rad, float r2_rad, float x1_rad, float x3_rad)
{
	static int u1k_1=0, u1k_2=0, e1k_1=0, e1k_2=0;
	int e1k=0, e2k=0,u1k=0, u2k=0, uk=0;

	//cout << r2_rad << endl;	
	//cout << r1_rad << endl;
	//cout << x1_rad << endl;
	//cout << x3_rad << endl;

	///CALCULO DEL ERROR
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

	//uk=uk/Fac_PID;

	if(uk>5)
	{
		uk=-5;///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	else if(uk<(-5))
	{
		uk=5; ///SIGNO INVERTIDO POR LECTURA DE PLANTA
	}
	return(int(uk*51));
}

PID::~PID()
{
	control_thread.~thread();
}

void PID::dummy(int sig)
{
	/* Esta funcion es un dummy al cual se conectan todas las señales,
	 * ya que cuando se declara mas de un objeto de tipo Soft_PWM
	 * las señales emitidas por cada uno causan interferencia entre si, puesto que
	 * un hilo determina que una señal emita no fue tenida en cuenta y aborta el programa
	 */
}
