obj-m := mymodule.o

KERNELDIR := /home/student/Desktop/grupp1/TTK4147SANNTIDSYSTEMER/ex8/buildroot-avr32-v3.0.0_TTK4147/output/build/linux-2.6.35.4
			#/home/student/Desktop/grupp1/TTK4147SANNTIDSYSTEMER/ex8/buildroot-avr-32-v3.0.0_TTK4147/output/build/linux-2.6.35.4
CROSS := /home/student/Desktop/grupp1/TTK4147SANNTIDSYSTEMER/ex8/buildroot-avr32-v3.0.0_TTK4147/output/staging/usr/bin/avr32-linux-

all:
	make ARCH=avr32 CROSS_COMPILE=$(CROSS) -C $(KERNELDIR) M=$(shell pwd) modules

clean:
	rm -rf *.0 *~core .depend .*.cmd *.ko *.mod.c .tmp_versions

