LINK_OPT= \
	-L../libizm -lizm \
	-lcrypto \
	-lm

COMPILE_OPT= \
	-D PROJECTROOT_DPATH=${PROJECTROOT_DPATH} \
	-I. \
	-I../libizm \
	-Wno-pointer-arith \
	-g \
	-O0

OBJ= \
	izmPrintLog.o

HEADER=

all : izmPrintLog

izmPrintLog : ${OBJ}
	g++ -o izmPrintLog ${OBJ} ${LINK_OPT};

izmPrintLog.o : izmPrintLog.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmPrintLog.o izmPrintLog.cpp;

.PHONY : clean install uninstall
clean :
	rm -f izmPrintLog izmPrintLog*.o 2>/dev/null;

install :
	install -m 755 izmPrintLog ${PROJECTROOT_DPATH}/bin/izmPrintLog;

uninstall :
	rm -f ${PROJECTROOT_DPATH}/bin/izmPrintLog 2>/dev/null;
