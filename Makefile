#
#  Makefile to build hello.c
#

CC=g++
CFLAGS=-O1 -g -Wno-write_strings
CFLAGS+=-DDUMP_MEM -DPRINT_DETAIL
#CFLAGS+=-DNO_HARDWARE
CFLAGS+=-DTEST_RF86
CFLAGS+=-DTEST_RF44
CFLAGS+=-DTEST_CP35 -DCP35_STRICT_OFF
CFLAGS+=-DTEST_CP25 -DCP25_STRICT_OFF
CFLAGS+=-DTEST_CAM464V
CFLAGS+=-DTEST_CAMBTB
OBJS=str2num.o spi.o mmaplib.o testbed.o
OBJS+=gs_memory_class.o gs_memory_oldPatternTest.o
OBJS+=rf86_function.o rf44_function.o cp35_function.o cp25_function.o
OBJS+=cam464v_function.o btbcam_function.o
OBJS+=oldPattern_test.o
LDFLAGS=-g

all:	${OBJS}
	${CC} ${LDFLAGS} ${OBJS} -o oldPatternTest

.c.o:
	${CC} ${CFLAGS} -c $< -o $@

.cpp.o:
	${CC} ${CFLAGS} -c $< -o $@

clean:
	rm -f *.o oldPatternTest
