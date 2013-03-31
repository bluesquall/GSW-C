#  $Id: GNUmakefile,v be9b097f6a58 2013/03/31 01:57:25 fdelahoyde $
#  $Version: 3.01.0 $
#  Makefile for libgswteos-10 on Linux/GNU.

.PHONY: MAKEVERSION_ERROR
.PHONY: all clean install uninstall distclean new-rpm new-release
.PHONY: show-release

          STSPackage :=	libgswteos-10
        _makeVersion :=	$(shell $(MAKE) -v -f /dev/null 2>&1 |\
			head -1 | awk '{if ($$3 >= 3.76) printf "%.2f\n",$$3}')
  ifeq (,$(_makeVersion))
MAKEVERSION_ERROR:
	@echo "This Makefile requires GNU make version 3.76 or greater."; \
	echo "Please read the file README in this directory"; \
	echo "for details."; exit 1; \
	fi;
  endif
          STSVersion :=	$(shell if [ -d .hg ]; then \
			hg tip --template "{latesttag}"; \
			else basename `pwd` | sed -e"'s/$(STSPackage)-//"; fi)
	  STSRelease := $(shell echo $(STSVersion)|sed -e 's/[^-]*-\?//')
  ifeq (,$(STSRelease))
          STSRelease := 1
  endif
	  STSVersion :=	$(shell echo $(STSVersion)|sed -e 's/-.*//')
#
                 ARCH:=	$(shell uname -m)
  ifeq (x86_64,$(ARCH))
           libdirname:=	lib64
  else
           libdirname:=	lib
  endif
               CFLAGS:=	-O3
            CINCLUDES:=	-I.
              Library:=	libgswteos-10.so
           LibVersion:=	$(shell echo $(STSVersion) | \
			awk -F. '{printf "%d.%d\n",$1,$2}')
              Program:=	gsw_check_functions
      $(Program)_SRCS:=	gsw_check_functions.c
      $(Program)_LIBS:=	-L. -lgswteos-10 -lm
      $(Library)_SRCS:=	gsw_oceanographic_toolbox.c \
			gsw_saar.c
      $(Library)_OBJS:=	gsw_oceanographic_toolbox.o \
			gsw_saar.o
             INCLUDES:=	gswteos-10.h
              DESTDIR:=	/usr
           DESTBINDIR:=	$(DESTDIR)/bin
           DESTINCDIR:=	$(DESTDIR)/include
           DESTLIBDIR:= $(DESTDIR)/$(libdirname)
             TARFILES:=	README gsw_check_functions.c \
			gsw_data_v3_0.dat.gz gsw_format.c \
			gsw_oceanographic_toolbox.c gsw_saar.c \
			gsw_saar_data.c gswteos-10.h GNUmakefile
             ZIPFILES:= README gsw_check_functions.c \
			gsw_data_v3_0.dat.gz gsw_format.c \
			gsw_oceanographic_toolbox.c gsw_saar.c \
			gsw_saar_data.c gswteos-10.h Makefile
              ZIPLINK:=	gsw_c_v$(shell echo $(STSVersion)|sed -e 's/\.[^.]*$//')

all:	$(Library) $(Program)

$(Program):	$($(Program)_SRCS)
	gcc $(CFLAGS) $(CINCLUDES) -o $(Program) $($(Program)_SRCS) \
		$($(Program)_LIBS)

$(Library):	$($(Library)_SRCS)
	gcc -fPIC -c $(CFLAGS) $(CINCLUDES) $($(Library)_SRCS)
	gcc -shared -o $(Library).$(LibVersion) $($(Library)_OBJS) -lm
	rm -f $(Library)
	ln -s $(Library).$(LibVersion) $(Library)

install:	$(Library) $(Program)
	install $(Library).$(LibVersion) $(INSTALL_ROOT)$(DESTLIBDIR)
	rm -f $(INSTALL_ROOT)$(DESTLIBDIR)/$(Library)
	ln -s $(Library).$(LibVersion) $(INSTALL_ROOT)$(DESTLIBDIR)/$(Library)
	install -s $(Program) $(INSTALL_ROOT)$(DESTBINDIR)
	install $(INCLUDES) $(INSTALL_ROOT)$(DESTINCDIR)
clean:
	rm -f $(Program) $(Library) $(Library).$(LibVersion) $($(Library)_OBJS)


new-release:	$(STSPackage).spec.proto
		@rm -f $(STSPackage).spec; \
		sed -e 's/@VERSION@/$(STSVersion)/' \
		    -e 's/@RELEASE@/$(STSRelease)/' \
			$(STSPackage).spec.proto >$(STSPackage).spec; \
		rm -f gsw_c_v*; \
		ln -s . $(ZIPLINK); \
		zip -r $(ZIPLINK).zip $(addprefix $(ZIPLINK)/,$(ZIPFILES))

new-rpm:	new-release
		@rm -f $(STSPackage)-*; \
		ln -s . $(STSPackage)-$(STSVersion); \
		tar cvzf $(STSPackage)-$(STSVersion).tar.gz \
		    --dereference \
		    $(addprefix $(STSPackage)-$(STSVersion)/,$(TARFILES)); \
		if [ -d /space/RPMBUILD/SOURCES ]; then \
		    cp $(STSPackage)-$(STSVersion).tar.gz \
			/space/RPMBUILD/SOURCES; \
		    cp $(STSPackage)/$(STSPackage).spec /space/RPMBUILD/SPECS; \
		fi

show-release:
		@echo $(STSVersion)-$(STSRelease)
