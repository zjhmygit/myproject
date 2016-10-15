obj-m += keyled.o

 KERNEL = /home/gec/linuxdrv/src/android-kernel-samsung-dev
 CC = /usr/local/arm/arm-2009q3/bin/arm-linux-gcc

all : 
	make modules -C $(KERNEL) M=$(shell pwd)
	$(CC) app.c -o app
clean :
	make modules clean -C $(KERNEL) M=`pwd`
	rm -rf app
