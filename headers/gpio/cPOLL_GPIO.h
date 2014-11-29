/*
 * cPOLL_GPIO.h
 *
 *  Created on: 15/08/2014
 *      Author: juan
 */

#ifndef CPOLL_GPIO_H_
#define CPOLL_GPIO_H_

#include "cGPIO.h" /* Clase base */
#include <poll.h>	//struct pollfd

class POLL_GPIO : public GPIO {

	public:
		POLL_GPIO(string gpio, string int_edge = "falling");
		~POLL_GPIO();

		int wait_until_edge(void);

	private:
		struct pollfd GPIO_int;
		char Read_Value;
		int fcn_return;
};



#endif /* CPOLL_GPIO_H_ */
