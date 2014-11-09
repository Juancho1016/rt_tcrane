/*
 * PID.h
 *
 *  Created on: 26/08/2014
 *      Author: juan
 */

#ifndef PID_H_
#define PID_H_

#ifndef __GXX_EXPERIMENTAL_CXX0X__
#define __GXX_EXPERIMENTAL_CXX0X__
#endif

#include <atomic>
#include <thread>
#include "cGPIO.h"
#include "cPOLL_GPIO.h"
#include "cSoft_PWM.h"
#include "cTIMER.h"

#define pi 3.14159265

class PID : 
		public TIMER, 
		public POLL_GPIO
{
		//public Soft_PWM

	public:
		PID(int seg, int ms_res);
		~PID();
		void run(void);

		void control(void);
		static void dummy(int sig);
		int test_bra(string gpio);
		int test_pen(string gpio2);
		int calc(float r1_rad, float r2_rad, float x1_rad, float x3_rad);

		float x1_rad;
		float x3_rad;
		float ref1_rad;
		float ref3_rad;
		int ms_resolution;
		int s_resolution;
		int signal_emited2;


		thread control_thread;
		atomic<int> atom_refx1;
		atomic<int> atom_refx3;
		atomic<int> atom_ref1;
		atomic<int> atom_ref3;

		sigset_t signal_set;
};
		
#endif /* PID_H_ */
