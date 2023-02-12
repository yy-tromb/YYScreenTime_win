CC = gcc
CFLAGS = -mwindows -municode -Wl,-subsystem,windows -static -Wall

INC = -I .\include
LIB = -L .\lib

SRCS = main.c #のちのちresource.rc
OBJS = $(SRCS:%.c=%.o)

LDFLAGS = -lwinapi_highDPI

EXE = YYScreenTime_win.exe

build: $(EXE)

$(EXE): $(OBJS)
	$(CC) $(OBJS) $(CFLAGS) $(LIB) $(LDFLAGS) -o $(EXE)

.c.o:
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	del $(OBJS)
	del o.txt

fullclean:
	make clean
	del $(EXE)

re:
	make clean
	make fullclean
