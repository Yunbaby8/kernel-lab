obj-m += mydev.o

KDIR := /home/yunhe/allcode/linux-6.6.87-build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
