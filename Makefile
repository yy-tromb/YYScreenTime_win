RM = del

CC = gcc
CFLAGS = -mwindows -municode -Wl,-subsystem,windows -static -static-libgcc -static-libstdc++ -Wall -fexec-charset=CP932
RESCC = windres
RESCFLAGS = -O coff

INC = -I .\include
LIB = -L .\lib

SRCS = main.c winutils.c strutil.c guiutils.c procs.c
RESRCS = resource.rc
OBJS = $(SRCS:%.c=%.o)
RESOBJS = $(RESRCS:%.rc=%.res)

LDFLAGS = -lwinapi_highDPI -lcomctl32

NAME = YYScreenTime_win
EXT ?= exe

ifeq ($(findstring -g,$(EXFLAGS)),-g)
	BUILD = $(NAME)_debug.$(EXT)
else
	BUILD =$(NAME).$(EXT)
endif

build: $(BUILD)

$(BUILD): $(OBJS) $(RESOBJS)
	$(CC) $(OBJS) $(RESOBJS) $(CFLAGS) $(EXFLAGS) $(LIB) $(LDFLAGS) -o $(BUILD)

.SUFFIXES: .rc .res

.rc.res:
	$(RESCC) $< $(RESCFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(EXFLAGS) $(INC) -c $< -o $@

clean:
	$(RM) $(OBJS)
	$(RM) $(RESOBJS)

fullclean:
	make clean
	$(RM) $(BUILD)

re:
	make clean
	make fullclean
