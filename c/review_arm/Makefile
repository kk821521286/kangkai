CROSS_COMPILE=arm-cortex_a8-linux-gnueabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump

INCLUDE_PATH="./include"
CFLAG=-mfpu=vfpv3 -mfloat-abi=softfp -mabi=apcs-gnu -fno-builtin  -fno-builtin-function \
-g -O0 -c -I $(INCLUDE_PATH) -o
   
NAME=rtc_time

OBJS=./arch/start.o
#OBJS+=./arch/_udivsi3.o
#OBJS+=./arch/_umodsi3.o

OBJS+=./drivers/led.o
OBJS+=./drivers/key.o
OBJS+=./drivers/uart.o
OBJS+=./drivers/irq.o
OBJS+=./drivers/rtc.o

OBJS+=./lib/string.o
OBJS+=./lib/printf.o
OBJS+=main.o

all:$(OBJS)	
	$(LD) $(OBJS) -T map.lds -o $(NAME).elf 
	$(OBJCOPY) -O binary $(NAME).elf $(NAME).bin
	$(OBJDUMP) -D $(NAME).elf > $(NAME).dis 

%.o: %.s
	$(CC) $(CFLAG) $@ $<
%.o: %.c
	$(CC) $(CFLAG) $@ $<
	
clean:
	rm *.bin *elf *.o *.dis $(OBJS)
	
	
