all : forkexec

PROJECTROOT_DPATH= \
	$(shell dirname ${CURDIR})

forkexec :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	make -f makefile.lib; make -f makefile.bin;

.PHONY : clean install uninstall

clean :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	make -f makefile.bin clean; make -f makefile.lib clean;

install :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	make -f makefile.lib install; make -f makefile.bin install;

uninstall :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	make -f makefile.bin uninstall; make -f makefile.lib uninstall;
