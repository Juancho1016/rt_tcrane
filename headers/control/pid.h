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

#define pi 3.14159265

class PID2 :
					public GPIO,
					public TIMER{

	public:
		PID2(string gpio_out, int ms_per, int ms_res = 10);
		~PID2();
		void run(void);

		void control(void);
		static void dummy(int sig);
		void test_bra(string gpio);
		void test_pen(string gpio2);
		int law_control(void);

		int ms_resolution;
		int ms_period;
		int signal_emited2;

		sigset_t signal_set2;

		thread thread_control;
		thread thread_inter_bra;
		thread thread_inter_pen;
		atomic<int> atom_r1;
		atomic<int> atom_r3;
		atomic<int> atom_x1;
		atomic<int> atom_x3;
};

//void Soft_PWM_calculate(Soft_PWM*);

#endif /* PID2_H_ */
