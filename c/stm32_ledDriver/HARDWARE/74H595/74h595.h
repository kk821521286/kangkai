#ifndef __74H595_H__
#define __74H595_H__
#include"sys.h"

#define led PAout(8)

#define SH PAout(4)
#define DS PAout(5)
#define ST PAout(6)

#define A138 PBout(0)
#define B138 PBout(1)
#define C138 PBout(2)

void H595_Init(void);
void write_smg(u8 temp);



#endif
