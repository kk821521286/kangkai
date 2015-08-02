CROSS_COMPILE=arm-none-eabi-
CC=$(CROSS_COMPILE)gcc
LD=$(CROSS_COMPILE)ld
OBJCOPY=$(CROSS_COMPILE)objcopy
OBJDUMP=$(CROSS_COMPILE)objdump

INCLUDE_PATH="./include"

CFLAG=-mfpu=vfpv3 -mfloat-abi=softfp -mabi=apcs-gnu -fno-builtin  -fno-builtin-function \
-g -O0 -c -I $(INCLUDE_PATH) -o

NAME=adc_poll

OBJS=./arch/start.o 
OBJS+=./arch/_udivsi3.o
OBJS+=./arch/_umodsi3.o

OBJS+=./drivers/led.o
OBJS+=./drivers/beep.o
OBJS+=./drivers/uart0.o
OBJS+=./lib/string.o
OBJS+=./lib/printf.o
OBJS+=main.o

all: $(OBJS) 
	$(LD) $(OBJS) -Tmap.lds -o $(NAME).elf 	#连接 elf
	$(OBJCOPY) -O binary $(NAME).elf $(NAME).bin		#2进制
	$(OBJDUMP) -D $(NAME).elf > $(NAME).dis				#反编译

%.o: %.s
	$(CC) $(CFLAG) $@ $<					#只编译没连接
	
%.o: %.c
	$(CC) $(CFLAG) $@ $<					#只编译没连接

clean:
	rm *.bin *.elf *.o *.dis $(OBJS)