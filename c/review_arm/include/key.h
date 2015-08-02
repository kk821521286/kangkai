/*
 * key.h
 *
 *  Created on: 2014-9-20
 *      Author: Administrator
 */

#ifndef KEY_H_
#define KEY_H_

#include "s5pc100.h"


//定义了一个变量，这个变量是一个指针， 指向一个函数

extern void init_key();
extern void  key1_handler(void);
extern void key2_handler(void);
//extern void key3_handler(void);

#endif /* KEY_H_ */
