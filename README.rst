
.. contents:: Table of contents
   :backlinks: top
   :local:

Neon Event Driven System (EDS)
==============================

Neon Event Driven System (EDS) is a collection of software components for
real-time applications. The Neon EDS components are:

* **Base** (``base``) :  Base headers needed by other Neon components.
* **Event Processor** (``ep``) : Event processor provides generation and
  handling of events, event queues, event broadcasting services and execution
  of finite state machines.
* **Memory Management** (``mm``) : Dynamic memory management. This module
  provides several algorithms for managing heap like memory.
* **Port** (``port``) : Portable system that offers consistent API to control
  the underlaying microcontroller and/or OS (if used).
* **Scheduler** (``sched``) : Contains priority array and the cooperative
  scheduler.
* **Virtual Timer** (``timer``) : Virtual general purpose timers. 

System
======

* **Static design**: No heap memory required. All data structures are allocated 
  during the compile time.
* **Static configuration**: The configuration is done in C header files using
  macro defines.
* **Lightweight**: All kernel functions have very small code size and usage of
  stack and static variables is at minimum.
* **Portable**: The kernel can run on wide range of micro-controllers. It was
  tested on 8-bit Microchip PIC up to 64-bit AMD CPU as Linux user thread.
* **Easy integration**: The kernel can be easily integrated into an existing 
  project. The kernel does not require any custom linker scripts or start-up 
  files. Assembly code is minimally used in port system. 
* **Preemption aware**: The system can be coupled with an existing RTOS to 
  provide preemptive execution if it is needed.

Scheduler
=========

* Unlimited number of threads.
* Up to 64 priority levels on 8-bit micro-controllers and up to 256 priority 
  levels on 32-bit micro-controllers.
* Round-robing, non-preemptive scheduling of threads with same priority.
* O(1) constant time complexity, scheduling time does not increase if new 
  threads are added to scheduler's ready queue.
* Hardware support for efficient schedulers algorithm execution.

Task
====

A task is a function with the following prototype: 

.. code::

    void function(struct ntask * ctx, void * arg)
    
Each task **must return** after some defined time. When the task returns it 
leaves the CPU time for other tasks to execute. Ideally, tasks are written 
as finite state machines which by definition are always returning.

During the task execution interrupts are allowed. 

The arguments to task function is always the task context structure and an
argument pointer which was given during the task creation process. This gives 
the ability to write parametrized task functions.

Using Event Driven System
=========================

Configuration
-------------

Configuration is done in ``neon_eds_app_config.h`` header file. The file is
included by ``base/config.h`` header file, which is in included by all Neon
components.

Building
--------

**Include paths**

- ``include`` - include path. Submodules are referenced by their respective 
  subdirectories.
- ``port/_port-name_`` - Neon core port. The _port-name_ is the directory of
  the port that is being used. For example, for application using a STM32F MCU
  with Cortex M4 core and GCC compiler the include path would be 
  ``port/stm32f4xx-none-gcc``.

**Source files**

- ``source/deferred.c`` - Deferred job execution
- ``source/epa.c`` - Event Processing Agent
- ``source/equeue.c`` - Event Queue
- ``source/etimer.c`` - Event Timer
- ``source/event.c`` - Event Object
- ``source/heap.c`` - Heap memory allocator
- ``source/mem.c`` - Memory allocator class
- ``source/pool.c`` - Pool memory allocator
- ``source/sched.c`` - Scheduler
- ``source/smp.c`` - State machine processor
- ``source/static.c`` - Static memory allocator
- ``source/stdheap.c`` - wrapper around C library malloc/free
- ``source/timer.c`` - Virtual timer
- ``port/_port-name_/p_core.c`` - main Neon core port source file. Ports may
  have additional source files located in `port/_port-name_` directory which
  need to be compiled.
   
Documentation
-------------

Doxygen configuration and full documentation source files are available in root
Neon project in ``doc`` directory. Documentation files are built using doxygen
tool:

.. code:: bash

    doxygen doxyfile-eds

This will generate HTML, LaTex and man documentation in ``doc/build``
directory.

Support
=======

If you've found an error, please `file an issue
<https://github.com/nradulovic/neon-eds/issues/new/>`_.

Patches are encouraged, and may be submitted by `forking this project 
<https://github.com/nradulovic/neon-eds/fork/>`_ and submitting
a pull request through GitHub. Please see `CONTRIBUTING.rst
<https://github.com/nradulovic/neon-eds/blob/master/CONTRIBUTING.rst/>`_ for
more details.
