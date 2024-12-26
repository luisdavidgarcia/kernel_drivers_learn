obj-m += procfs3.o

PWD := $(CURDIR)

CFLAGS_procfs3.o := -DDEBUG

all:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

clean:
	$(MAKE) -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean