# Quick-start guide

Neon Base is a collection of low-level libraries for Neon Real-Time Framework.  


## TODO list

- test, test, test...


# Using Base system

## Configuration and ports

Configuration is done in two group of files: 
- the port independent settings are located in 'include/nbase_config.h'
- the port dependent settings are located in the port directory


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

Doxygen configuration and full documentation source files are available in `/doc` 
directory. Go to the directory `doc` create a directory named `kernel` and than 
run doxygen:

    # doxygen doxyfile-base

This will generate HTML, LaTex and man documentation in `./doc/base` directory.


## Running

To successfully use and run kernel you will need to study the documentation. The 
documentation is still being written and some examples will be added later.

