
VERSION = 1.3

CC = g++

OPTIONS = -Wall
#OPTIONS += -O3
#OPTIONS += -g

# symbol hidden option (for gcc v4.0+)
SOPTIONS = -fvisibility=hidden -fvisibility-inlines-hidden

# SOURCE CODE
CC_SRC = $(wildcard  *.cpp)
C_SRC = $(wildcard  *.c)

# OBJECTS
CC_OBJS = $(patsubst %.cpp,./%.o,$(CC_SRC))
C_OBJS = $(patsubst %.c,./%.o,$(C_SRC))
OBJS = $(CC_OBJS) $(C_OBJS)

# DEPS
DEPS = $(patsubst %.o,%.d,$(OBJS))

# INCLUDE DIR
INC_DIR = -I. -I./include


# LIB_DIR
#LIB_DIR = -L. -L./lib  -L/lib -L/usr/lib -L/usr/local/lib
LIB_DIR = -L. -L./lib

# LIBS
#LIBS = -lwqplaysdk -lsvacdec -lh264dec.so -lintlc
LIBS = -lwqplaysdk

LDFLAGS = $(LIB_DIR) $(LIBS)

CPPFLAGS = $(SOPTIONS) $(INC_DIR) $(DFLAGS)

EXE = wqdemo

# depends on wqdemo.cpp
EXE_OBJ = wqdemo.o


all: $(EXE)

include $(DEPS)

$(EXE):$(OBJS)
	$(CC) -o $@ $^ $(LDFLAGS)

./%.o:%.cpp
	$(CC) $(OPTIONS) -o $@ -c $< $(CPPFLAGS)

./%.o:%.c
	$(CC) $(OPTIONS) -o $@ -c $< $(CPPFLAGS)

./%.d:%.cpp
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$

./%.d:%.c
	@set -e; rm -f $@; \
	$(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
	sed 's,.*\.o[ :]*,$(patsubst %.d,%.o,$@) $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$


clean:
	rm -f $(EXE)
	rm -f $(DEPS)
	rm -f $(OBJS)

