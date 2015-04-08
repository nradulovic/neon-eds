# Neon Event Driven System (EDS) Quick-start guide

Neon is a collection of software components for real-time applications.

Main Neon EDS components are:
* **port** - Portable system that offers consistent API to control the
    underlaying microcontroller and OS (if used).
* **base** - Bare minimum headers needed by other Neon components.
* **mm** - Memory management
* **sched** - Contains priority array and the cooperative scheduler
* **timer** - Virtual general purpose timers. 
* **ep** - Event processing provides event generation and handling, event 
    queues, broadcasting services and execution of finite state machines.

## Features


### System

* **Static design**. No heap memory required. All data structures are allocated 
    during the compile time.
* **Static configuration**. The configuration is done in C header files using macro
    defines.
* **Lightweight**. All kernel functions have very small code size and usage of
    stack located and static variables is at minimum.
* **Portable**. The kernel can run on wide range of micro-controllers. It was tested
    on 8-bit Microchip PIC up to 64-bit AMD CPU as Linux user thread.
* **Easy integration**. The kernel can be easily integrated into an existing 
    project. The kernel does not require any custom linker scripts or start-up 
    files. Assembly code is minimally used in port system. 
* **Preemption aware**. The system can be coupled with an existing RTOS to 
    provide preemptive execution if it is needed. Several instances of the 
    scheduler can be started to provide multiple levels of preemption.


### Scheduler

* Unlimited number of threads.
* Up to 64 priority levels on 8-bit micro-controllers and up to 256 priority 
    levels on 32-bit micro-controllers.
* Round-robing, non-preemptive scheduling of threads with same priority.
* O(1) constant time complexity, scheduling time does not increase if new 
    threads are added to scheduler's ready queue.
* Hardware support for efficient schedulers algorithm execution.

### Thread

A thread is a function with the following prototype: 

        void function(void * stack)
    
Each thread **must return** after some defined time. When the thread returns it 
leaves the CPU time for other threads to execute. Ideally, threads are written 
as finite state machines which by definition are always returning.

During the thread execution interrupts are allowed. 

The argument to thread function is always the stack pointer which was given 
during the thread creation process. This gives the ability to write parametrized 
thread functions.

# Using Event Driven System 

## Configuration

Configuration is done in `neon_eds_app_config.h` header file. The file is included
by `base/config.h` header file, which is in included by all Neon components.

## Building

### Include paths

- `include` - include path. Submodules are referenced by their respective 
    subdirectories.
- `port/_port-name_` - Neon core port. The _port-name_ is the directory of the 
    port that is being used.

### Source files

- `source/epa.c` - Event Processing Agent
- `source/equeue.c` - Event Queue
- `source/etimer.c` - Event Timer
- `source/event.c` - Event Object
- `source/heap.c` - Heap memory allocator
- `source/mem.c` - Memory allocator class
- `source/pool.c` - Pool memory allocator
- `source/sched.c` - Scheduler
- `source/smp.c` - State machine processor
- `source/static.c` - Static memory allocator
- `source/timer.c` - Virtual timer
- `port/_port-name_/p_core.c` - main Neon core port source file. Ports may have 
    additional source files located in `port/_port-name_` directory which need 
    to be compiled.
    
## Documentation

Doxygen configuration and full documentation source files are available in root
Neon project in `documentiontion/eds` directory. Documentation files are built
using doxygen tool.

    # doxygen doxyfile-eds

This will generate HTML, LaTex and man documentation in `documentation/eds` 
directory.

