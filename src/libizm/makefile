LINK_OPT= \
	-shared

COMPILE_OPT= \
	-I. \
	-Wno-pointer-arith \
	-fPIC \
	-g \
	-O0

OBJ= \
	izmlog.o \
	izmmath.o \
	izmstr.o \
	izmvector.o \
	izmwav.o

HEADER= \
	izm.hpp \
	izm.h

all : libizm.so

libizm.so : ${OBJ}
	g++ ${LINK_OPT} -o libizm.so ${OBJ};

izmlog.o : izmlog.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmlog.o izmlog.cpp;

izmmath.o : izmmath.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmmath.o izmmath.cpp;

izmstr.o : izmstr.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmstr.o izmstr.cpp;

izmvector.o : izmvector.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmvector.o izmvector.cpp;

izmwav.o : izmwav.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmwav.o izmwav.cpp;

.PHONY : clean install uninstall
clean :
	rm -f libizm.so izm*.o 2>/dev/null;

install :
	install -m 755 libizm.so ${FORKEXEC}/lib/libizm.so;

uninstall :
	rm -f ${FORKEXEC}/lib/libizm.so 2>/dev/null;
