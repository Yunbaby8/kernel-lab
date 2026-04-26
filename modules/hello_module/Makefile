obj-m += hello_module.o

KDIR := ~/allcode/linux-6.6.87-build

all:
	make -C $(KDIR) M=$(PWD) modules

clean:
	make -C $(KDIR) M=$(PWD) clean
