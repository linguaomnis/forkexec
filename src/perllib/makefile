all :

.PHONY : clean install uninstall
clean :

install :
	install -m 755 -D Mrb/Lexicon.pm ${PROJECTROOT_DPATH}/lib/perl/Mrb/Lexicon.pm; \
	install -m 755 -D Hkr/Util.pm ${PROJECTROOT_DPATH}/lib/perl/Hkr/Util.pm; \
	install -m 755 -D Izm/Util.pm ${PROJECTROOT_DPATH}/lib/perl/Izm/Util.pm;

uninstall :
	rm -rf ${PROJECTROOT_DPATH}/lib/perl 2>/dev/null;
