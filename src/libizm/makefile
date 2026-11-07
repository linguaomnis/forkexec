LINK_OPT= \
	-shared

COMPILE_OPT= \
	-D PROJECTROOT_DPATH=${PROJECTROOT_DPATH} \
	-I. \
	-Wno-pointer-arith \
	-fPIC \
	-g \
	-O0

OBJ= \
	izmmath.o \
	izmstr.o \
	izmvector.o \
	izmwav.o

HEADER= \
	izm.h

all : libizm.so

libizm.so : ${OBJ}
	gcc ${LINK_OPT} -o libizm.so ${OBJ};

izmmath.o : izmmath.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o izmmath.o izmmath.c;

izmstr.o : izmstr.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o izmstr.o izmstr.c;

izmvector.o : izmvector.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o izmvector.o izmvector.c;

izmwav.o : izmwav.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o izmwav.o izmwav.c;

.PHONY : clean install uninstall
clean :
	rm -f libizm.so izm*.o 2>/dev/null;

install :
	install -m 755 libizm.so ${PROJECTROOT_DPATH}/lib/libizm.so;

uninstall :
	rm -f ${PROJECTROOT_DPATH}/lib/libizm.so 2>/dev/null;
