SRCS := $(wildcard src/*.c)
SRCS += $(wildcard src/JTAG/*.c)
SRCS += $(wildcard src/G3Algo/*.c)
SRCS += $(wildcard src/G4Algo/*.c)
OBJS := $(patsubst src/%.c, obj/%.o, $(SRCS))
TARG_SRCS := $(wildcard src/*.cc)
TARGS := $(patsubst src/%.cc, build/%,$(TARG_SRCS))

INCS += -I./src -I./src/JTAG -I./src/G3Algo -I./src/G4Algo -I$(BOOST_INC_DIR) 

LIBS += -L$(BOOST_LIB_DIR_64) -lboost_program_options $(BCM_LIB_DIR)/libbcm2835.a

#DEBUG_LEVEL = -g
FLAGS = -DENABLE_FILE_SYSTEM -DENABLE_G4_SUPPORT -DRPI_SUPPORT
CC = gcc
CPP = g++
CFLAGS = -Wall $(FLAGS) $(DEBUG_LEVEL) $(INCS)
CPPFLAGS = $(CFLAGS) 
CPPFLAGS += -fpermissive

all: $(OBJS) $(TARGS) 

obj/%.o: src/%.c
	$(CC) $(CFLAGS) $(INCS) -c $< -o $@

$(TARGS): $(TARG_SRCS)
	$(CPP) $(CPPFLAGS) $(INCS) $(OBJS) $< -o $@ $(LIBS)

clean: 
	rm -f $(OBJS) $(TARGS)
