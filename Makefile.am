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

neonincdir = $(includedir)/neon
neonbaseincdir = $(neonincdir)/base
neonepincdir = $(neonincdir)/ep
neonmmincdir = $(neonincdir)/mm
neonportincdir = $(neonincdir)/port
neonschedincdir = $(neonincdir)/sched
neontimerincdir = $(neonincdir)/timer
neoninc_HEADERS = include/neon_eds.h
neonbaseinc_HEADERS = \
    include/base/bias_list.h \
    include/base/bitop.h \
    include/base/component.h \
    include/base/config.h \
    include/base/debug.h \
    include/base/error.h \
    include/base/list.h \
    include/base/queue.h
neonepinc_HEADERS = \
    include/ep/epa.h \
    include/ep/equeue.h \
    include/ep/etimer.h \
    include/ep/event.h \
    include/ep/smp.h
neonmminc_HEADERS = \
    include/mm/heap.h \
    include/mm/mem.h \
    include/mm/pool.h \
    include/mm/static.h \
    include/mm/stdheap.h
neonportinc_HEADERS = \
    include/port/compiler.h \
    include/port/core.h \
    include/port/os.h
neonschedinc_HEADERS = \
    include/sched/deferred.h \
    include/sched/sched.h
neontimerinc_HEADERS = \
    include/timer/timer.h
