all : forkexec

PROJECTROOT_DPATH= \
	$(shell dirname ${CURDIR})

forkexec :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	find */ -type f -name makefile | xargs -r -L1 dirname | xargs -r -ITARGET make -C TARGET;

.PHONY : clean install uninstall

clean :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	find */ -type f -name makefile | xargs -r -L1 dirname | xargs -r -ITARGET make clean -C TARGET;

install :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	find */ -type f -name makefile | xargs -r -L1 dirname | xargs -r -ITARGET make install -C TARGET;

uninstall :
	export PROJECTROOT_DPATH=${PROJECTROOT_DPATH}; \
	find */ -type f -name makefile | xargs -r -L1 dirname | xargs -r -ITARGET make uninstall -C TARGET;
