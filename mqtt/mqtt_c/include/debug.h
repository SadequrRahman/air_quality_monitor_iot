/*
 * debug.h
 *
 *  Created on: Dec 4, 2014
 *      Author: Minh
 */

#ifndef USER_DEBUG_H_
#define USER_DEBUG_H_

#ifdef DEBUG_ON
#ifndef INFO
#define INFO os_printf
#endif
#else
#define INFO
#endif

#endif /* USER_DEBUG_H_ */
