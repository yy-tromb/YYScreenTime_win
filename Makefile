RM = del

CC = gcc
CFLAGS = -mwindows -municode -Wl,-subsystem,windows -static -static-libgcc -static-libstdc++ -Wall -fexec-charset=CP932 -m64
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
	BUILD = $(NAME).$(EXT)
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

msi: $(BUILD)
	candle -arch x64 YYScreenTime_win.wxs
	light -cultures:ja-jp -ext WixUIExtension YYScreenTime_win.wixobj

msiclean:
	$(RM) YYScreenTime_win.wixobj
	$(RM) YYScreenTime_win.wixpdb

fullclean:
	make clean
	make msiclean
	$(RM) $(BUILD)
	$(RM) YYScreenTime_win.msi

re:
	make fullclean
	make msi
