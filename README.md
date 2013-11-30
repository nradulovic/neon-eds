# Quick-start guide

eSolid - Base system is a library for embedded devices.  

eSolid is a collection of resources for embedded system design and this
Base system is only a piece of that collection.


## TODO list

- Integrate a profiling system (memory usage, fragmentation...)
- test, test, test...


# Using eSolid - Base system

## Configuration and ports

Configuration is done in two group of files: the ones located under /inc/base
directory (like `dbg_cfg.h`...), which are port independent settings, and in 
port directory (like `intr_cfg.h`, `sysctrl_cfg`), which are port depended 
settings.
Currently, eSolid is ported only to ARMv7-M architecture range of 
microcontrollers. It was tested on STM32F100 series of microcontrollers, but it
should work, with minimal modifications, on any ARMv7-M CPU. Some other ports 
like AVR-GCC are planned, too.


## Building

The base system was built using arm-none-eabi GCC v4.7.3 compiler toolchain 
(from https://launchpad.net/gcc-arm-embedded/+download) and binary was 
downloaded to the MCU using _texane_ gdb-server. There are no makefiles, it is 
assumed that IDE will generate them for you.

#### Example for STM32F10x family port

There are two groups of source files which need to be compiled for ARMv7-M 
architecture: 
- `mem.c`, `dbg.c` in `./src` source directory and 
- `cpu.c`, `intr.c` in `./port/arm-none-eabi-gcc/v7-m` port directory.

The following include paths are needed:
- `./inc`
- `./port/arm-none-eabi-gcc/v7-m`
- `./port/arm-none-eabi-gcc/stm32f10x`

## Documentation

Some documentation is available under Wiki 
https://github.com/nradulovic/esolid-base/wiki. 
Doxygen configuration and full documentation source files are available in `/doc` 
directory. Go to the directory `doc` create a directory named `kernel` and than 
run doxygen:

    # doxygen doxyfile-base

This will generate HTML, LaTex and man documentation in `./doc/base` directory.


## Running
To successfully use and run kernel you will need to study the documentation. The 
documentation is still being written and some examples will be added later.

