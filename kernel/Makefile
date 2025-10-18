obj-m += kdexg.o

SRC_DIR := $(src)/src
SRC_FILES := $(shell find $(SRC_DIR) -type f -name '*.c')
kdexg-y := kdexg_main.o \
           $(patsubst $(src)/%,%,$(SRC_FILES:.c=.o))

ccflags-y += -I$(SRC_DIR)
ccflags-y += $(addprefix -I, $(shell find $(SRC_DIR) -type d))
