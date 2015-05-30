
## Process this file with automake to produce Makefile.in.

noinst_PROGRAMS = tutorial \
                  tutorial2

AM_CFLAGS = @PTHREAD_CFLAGS@ \
            @FREETYPE2_CFLAGS@ \
            @FONTCONFIG_CFLAGS@ \
            @GLUT_CFLAGS@ \
            @FRIBIDI_CFLAGS@

LDADD = $(top_builddir)/build/libGLC.la \
        @GLUT_LIBS@ \
        @FONTCONFIG_LIBS@ \
        @FREETYPE2_LIBS@ \
        @PTHREAD_LIBS@ \
        @FRIBIDI_LIBS@

tutorial_SOURCES = tutorial.c

tutorial2_SOURCES = tutorial2.c

clean-generic:
	rm -f *.gcno *.gcda *.gcov
