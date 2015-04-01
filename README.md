# Neon Event Driven System (EDS) Quick-start guide

Neon is a collection of software components for real-time applications.

# Using Event Driven System 

## Configuration

Configuration is done in `neon_app_config.h` header file. The file is included
by `base/include/shared/config.h` file, which is in included in all other Neon
components.

## Building

### Include paths

- `eds/include` - standard Neon include path

### Source files

- `eds/source/epa.c` - Event Processing Agent
- `eds/source/equeue.c` - Event Queue
- `eds/source/etimer.c` - Event Timer
- `eds/source/event.c` - Event Object
- `eds/source/smp.c` - State machine processor
    
### Project dependencies

Neon Eds does depend on the following components:
- base
- kernel

## Documentation

Doxygen configuration and full documentation source files are available in root
Neon project in `documentiontion/eds` directory. Documentation files are built
using doxygen tool.

    # doxygen doxyfile-eds

This will generate HTML, LaTex and man documentation in `documentation/eds` 
directory.

