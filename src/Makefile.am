CFLAGS += -Wall -I$(srcdir)/include -I$(srcdir)/..

NAGBOT_SRC = ../config.h cmd.h include/ircraw.h message.h parser.h parser.cpp irc.h irc.cpp socket.cpp socket.h ConfigFile.cpp Makefile.am ConfigFile.h flog.cpp ircraw.h nagbot.cpp flog.h nagbot.h main.cpp

#BUILT_SOURCES = svnversion.h
#VERSION_SRC = version.h release.h svnversion.h

bin_PROGRAMS = nagbot
nagbot_SOURCES = $(NAGBOT_SRC)
nagbot_CFLAGS = 
nagbot_LDFLAGS = @LIBS@ -lboost_thread
nagbot_LDADD = 


#.PHONY: svnversion.h.tmp
#svnversion.h.tmp: 
#	if [ -d .svn ]; then \
#		ver=`svnversion -c . | sed -e 's/.*://' -e 's/[A-Za-z]//'`; \
#		echo "#define POM_SVN_VERSION \"svn-r$$ver\"" > $@; \
#	else \
#		ver=`date +%F`; \
#		echo "#define POM_SVN_VERSION \"built-$$ver\"" > $@; \
#	fi
#svnversion.h: svnversion.h.tmp
#	diff $@ $@.tmp >/dev/null 2>&1 || cp -f $@.tmp $@; \
#	rm -f $@.tmp
#
#mostlyclean-local:
#	rm -f svnversion.h svnversion.h.tmp

