
obj-m += intr.o 


KERNEL_DIR := /home/115/mk_kernel/src/android-kernel-samsung-dev

all:
	make modules -C $(KERNEL_DIR) M=$(shell pwd)

clean:
	make modules clean -C $(KERNEL_DIR) M=$(shell pwd)


