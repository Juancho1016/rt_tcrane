/*
 * PID2.h
 *
 *  Created on: 28/08/2014
 *      Author: andres
 */

#ifndef PID2_H_
#define PID2_H_

#ifndef __GXX_EXPERIMENTAL_CXX0X__
#define __GXX_EXPERIMENTAL_CXX0X__
#endif

#include <thread>
#include <atomic>
#include "cGPIO.h"
#include "cPOLL_GPIO.h"
#include "cSoft_PWM.h"
#include "cTIMER.h"
#include "Gains_LQT_PID2.h"
#include "main.h"

#define pi 3.14159265

class PID2 : public GPIO, public TIMER{

	public:
		PID2(string gpio_out, int ms_per, int ms_res = 10);
		~PID2();
		void run(void);

		// Atributos(las constantes estan en un .h)
		static constexpr float e1k_1=0, e1k_2=0, u1k_1=0, u1k_2=0;
		int count_bra, count_pen;
		float r1_rad, r2_rad;
		float e1k, e2k, u1k, u2k;
		float x1_rad, x3_rad;
		int b, p, infb, infp, ref1;
		atomic<int> atom_r1;//digitada por el usuario
		atomic<int> atom_x1;//conteo de pulsos brazo
		atomic<int> atom_x3;//conteo de pulsos pendulo
		atomic<int> atom_uk;//modificado

		// Atributos tipo clase
		Soft_PWM *pwm1;
		GPIO *my_direction;
		GPIO *arm;
		GPIO *pendulum;
		POLL_GPIO *my_arm;
		POLL_GPIO *my_pendulum;
 		
		// Metodos
		static void dummy(int sig);
		void test_bra(void);
		void test_pen(void);
		void control(void);
		void pwm(void);
		void law_control(void);

		int ms_resolution;
		int ms_period;
		int signal_emited2;

		sigset_t signal_set2;

		thread thread_bra;
		thread thread_pen;
		thread thread_control;
};

//void Soft_PWM_calculate(Soft_PWM*);

#endif /* PID2_H_ */
