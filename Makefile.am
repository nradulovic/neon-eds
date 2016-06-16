AUTOMAKE_OPTIONS = foreign
ACLOCAL_AMFLAGS = -I m4

AM_CFLAGS = -I$(top_srcdir)/include -I$(top_srcdir)/port/x86-64-linux-gcc


noinst_LTLIBRARIES = libneoneds.la
libneoneds_la_SOURCES = \
	source/epa.c \
	source/equeue.c \
	source/etimer.c \
	source/event.c \
	source/heap.c \
	source/mem.c \
	source/pool.c \
	source/sched.c \
	source/smp.c \
	source/static.c \
	source/stdheap.c \
	source/timer.c \
	source/deferred.c \
	port/x86-64-linux-gcc/p_core.c

