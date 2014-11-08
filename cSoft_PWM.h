/*
 * cSoft_PWM.h
 *
 *  Created on: 28/08/2014
 *      Author: juan
 */

#ifndef CSOFT_PWM_H_
#define CSOFT_PWM_H_

#ifndef __GXX_EXPERIMENTAL_CXX0X__
#define __GXX_EXPERIMENTAL_CXX0X__
#endif

#include "cGPIO.h" 	/* Clase base para acceso al GPIO*/
#include "cTIMER.h" 	/* Clase base para control de timer*/

/* Project -> Propieties -> Path and Symbols -> GNU C++ -> ADD
 * __GXX_EXPERIMENTAL_CXX0X__
 * VALUE -> NADA */

/* Librerias C++11 */
#include <thread>
#include <atomic>

					/* Herencia Multiple */
class Soft_PWM :
					public GPIO,
					public TIMER{

	friend void Soft_PWM_calculate(Soft_PWM*); /* No importa en que ambito sea declarado, Privado
									   Publico o Protegido */

	public:
		Soft_PWM(string gpio_out,int us_per, int us_res = 10);
		~Soft_PWM();
		void run(void);

		int getAsync_OC(void);
		int getTOP(void);

		void setAsync_OC(int OC);

	private:
		void calculate(void);
		static void dummy(int sig);

		int count;
		int compare_value;
		int us_resolution;
		int us_period;
		int TOP_value;
		int signal_emited;

		bool correct;

		sigset_t signal_set;

		thread thread_calculate;
		atomic<int> atom_compare_value;
};

//void Soft_PWM_calculate(Soft_PWM*);

#endif /* CSOFT_PWM_H_ */
