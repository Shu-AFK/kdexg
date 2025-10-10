KDIR := $(PWD)/build-5.15/ubuntu-headers/usr/src/linux-headers-5.15.0-157-generic
obj-m += kDEXG.o

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
