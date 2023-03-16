CC = gcc
CFLAGS = -mwindows -municode -Wl,-subsystem,windows -static -static-libgcc -static-libstdc++ -Wall -fexec-charset=CP932
RESCC = windres
RESCFLAGS = -O coff

INC = -I .\include
LIB = -L .\lib

SRCS = main.c winutils.c strutil.c
RESRCS = res.rc
OBJS = $(SRCS:%.c=%.o)
REOBJS = $(RESRCS:%.rc=%.res)

LDFLAGS = -lwinapi_highDPI

NAME = YYScreenTime_win
EXT ?= exe

ifeq ($(findstring -g,$(EXFLAGS)),-g)
	BUILD = $(NAME)_debug.$(EXT)
else
	BUILD =$(NAME).$(EXT)
endif

build: $(BUILD)

$(BUILD): $(OBJS) $(REOBJS)
	$(CC) $(OBJS) $(REOBJS) $(CFLAGS) $(EXFLAGS) $(LIB) $(LDFLAGS) -o $(BUILD)

.SUFFIXES: .rc .res

.rc.res:
	$(RESCC) $< $(RESCFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(EXFLAGS) $(INC) -c $< -o $@

clean:
	del $(OBJS)
	del o.txt
	del icon.res

fullclean:
	make clean
	del $(BUILD)

re:
	make clean
	make fullclean
