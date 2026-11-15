LINK_OPT= \
	-L../libizm -lizm \
	-lcrypto \
	-lm

COMPILE_OPT= \
	-D PROJECTROOT_DPATH=${PROJECTROOT_DPATH} \
	-I. \
	-I../libizm \
	-Wno-pointer-arith \
    -Wno-deprecated-declarations \
	-g \
	-O0

HEADER= \
    izmusr.hpp

all : izmusr izmusr-admin

izmusr : izmusr.o ../libizm/libizm.so
	g++ -o izmusr izmusr.o ${LINK_OPT};

izmusr-admin : izmusr-admin.o ../libizm/libizm.so
	g++ -o izmusr-admin izmusr-admin.o ${LINK_OPT};

izmusr.o : izmusr.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmusr.o izmusr.cpp;

izmusr-admin.o : izmusr-admin.cpp ${HEADER}
	g++ -c ${COMPILE_OPT} -o izmusr-admin.o izmusr-admin.cpp;

.PHONY : clean install uninstall
clean :
	rm -f izmusr izmusr-admin izmusr*.o 2>/dev/null;

install :
	sudo install -m 755 izmusr izmusr-admin ${PROJECTROOT_DPATH}/bin/; \
	sudo chown root:root ${PROJECTROOT_DPATH}/bin/izmusr-admin; \
	sudo chmod 700 ${PROJECTROOT_DPATH}/bin/izmusr-admin; \
	sudo chown root:sys ${PROJECTROOT_DPATH}/bin/izmusr; \
	sudo chmod 711 ${PROJECTROOT_DPATH}/bin/izmusr; \
	sudo chmod u+s ${PROJECTROOT_DPATH}/bin/izmusr;

uninstall :
	rm -f ${PROJECTROOT_DPATH}/bin/izmusr 2>/dev/null; \
	rm -f ${PROJECTROOT_DPATH}/bin/izmusr-admin 2>/dev/null;
