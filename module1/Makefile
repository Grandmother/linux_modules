obj-m += hello-1.o
obj-m += hello-2.o
obj-m += hello-separated.o
hello-separated-objs := hello-8-start.o hello-8-exit.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
