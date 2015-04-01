# Neon Base Quick-start guide

Neon is a collection of software components for real-time applications.

# Using Base

## Configuration

Configuration is done in `neon_app_config.h` header file. The file is included
by `base/include/shared/config.h` file, which is in included in all other Neon
components.

## Building

### Include paths

- `base/include` - standard Neon include path
- `port/_port-name_` - where the _port-name_ is the directory of port that is 
used.

### Source files

- `port/_port-name_/p_core.c` - main port source file. Ports may have additional
    source files located in `port/_port-name_` directory which need to be 
    compiled.
    
### Project dependencies

Neon Base does not depend on additional Neon components.

## Documentation

Doxygen configuration and full documentation source files are available in root
Neon project in `documentiontion/base` directory. Documentation files are built
using doxygen tool.

    # doxygen doxyfile-base

This will generate HTML, LaTex and man documentation in `documentation/base` 
directory.

