#ifndef SEM_H
#define SEM_H
#include "head.h"
extern int set_semvalue(void);
extern void del_semvalue(void);
extern int semaphore_p(void);
extern int semaphore_v(void);

int sem_id;

#endif 
