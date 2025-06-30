obj-m := kexpita.o
KDIR := /lib/modules/$(shell uname -r)/build
PWD := $(shell pwd)

# Don't create .text.unlikely/.text.hot sections.
EXTRA_CFLAGS += -fno-reorder-functions

# Disable stack protector to allow exploiting stack buffer overflows.
EXTRA_CFLAGS += -fno-stack-protector

EXTRA_CFLAGS += -g -DDEBUG
ccflags-y += ${EXTRA_CFLAGS}
CC += ${EXTRA_CFLAGS}

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean