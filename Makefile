CC = gcc
CFLAGS = -mwindows -municode -Wl,-subsystem,windows -static -Wall

RESCC = windres
RESCFLAGS = -O coff

INC = -I .\include
LIB = -L .\lib

SRCS = main.c
RESRCS = icon.rc
OBJS = $(SRCS:%.c=%.o)
REOBJS = $(RESRCS:%.rc=%.res)

LDFLAGS = -lwinapi_highDPI

EXE = YYScreenTime_win.exe

build: $(EXE)

$(EXE): $(OBJS) $(REOBJS)
	$(CC) $(OBJS) $(REOBJS) $(CFLAGS) $(LIB) $(LDFLAGS) -o $(EXE)

.SUFFIXES: .rc .res

.rc.res:
	$(RESCC) $< $(RESCFLAGS) -o $@

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	del $(OBJS)
	del o.txt
	del icon.res

fullclean:
	make clean
	del $(EXE)

re:
	make clean
	make fullclean
