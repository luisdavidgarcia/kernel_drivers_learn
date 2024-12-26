obj-m += hello-sysfs.o

PWD := $(CURDIR)

CFLAGS_hello-sysfs.o := -DDEBUG

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean