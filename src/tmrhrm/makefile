LINK_OPT= \
	-L../libizm -lizm \
	-lm

COMPILE_OPT= \
	-D PROJECTROOT_DPATH=${PROJECTROOT_DPATH} \
	-I. \
	-I../libizm \
	-Wno-pointer-arith \
	-g \
	-O0

OBJ= \
	tmrhrm_osc.o \
	tmrhrm_seq.o \
	tmrhrm_util.o \
	tmrhrm.o

HEADER= \
	tmrhrm_osc.h \
	tmrhrm_seq.h \
	tmrhrm_util.h

all : tmrhrm

tmrhrm : ${OBJ}
	gcc ${LINK_OPT} -o tmrhrm ${OBJ};

tmrhrm.o : tmrhrm.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o tmrhrm.o tmrhrm.c;

tmrhrm_osc.o : tmrhrm_osc.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o tmrhrm_osc.o tmrhrm_osc.c;

tmrhrm_seq.o : tmrhrm_seq.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o tmrhrm_seq.o tmrhrm_seq.c;

tmrhrm_util.o : tmrhrm_util.c ${HEADER}
	gcc -c ${COMPILE_OPT} -o tmrhrm_util.o tmrhrm_util.c;

.PHONY : clean install uninstall
clean :
	rm -f tmrhrm tmrhrm*.o 2>/dev/null;

install :
	install -m 755 tmrhrm ${PROJECTROOT_DPATH}/bin/tmrhrm;

uninstall :
	rm -f ${PROJECTROOT_DPATH}/bin/tmrhrm 2>/dev/null;
