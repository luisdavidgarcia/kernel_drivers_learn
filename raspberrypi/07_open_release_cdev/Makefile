obj-m += hello_cdev.o

all: 
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean 

devices:
	sudo mknod /dev/hello0 c 236 0
	sudo mknod /dev/hello10 c 236 10

clean_dev:
	sudo rm /dev/hello0
	sudo rm /dev/hello10

