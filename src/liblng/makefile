LINK_OPT= \
	-L../libizm -lizm

COMPILE_OPT= \
	-D PROJECTROOT_DPATH=${PROJECTROOT_DPATH} \
	-I. \
	-I../libizm \
	-Wno-pointer-arith \
	-std=c++23 \
	-g \
	-O0

all : testapp

testapp : testapp.cpp lng.hpp 
	g++ ${COMPILE_OPT} -o testapp testapp.cpp;

.PHONY : clean install uninstall
clean :
	rm -f testapp 2>/dev/null;

install :

uninstall :

