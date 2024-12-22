obj-m += ldd.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules

user-app:
	./user-app.py

clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean