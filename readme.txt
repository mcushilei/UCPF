Universal Cross-Platform Framework(UCPF)

physical organization£º

high level
^
|
|---applications
|   |---applications are placed here.
|
|---beams: 
|   |---protocol stack: TCP/IP, USB, CANOpen, Xmodem, etc
|   |
|   |---board-level peripheral drivers: E2PROM, FLASH, RTC, ADC, Codec, Modules, etc
|   |
|   |---frameworks
|   |   |---boards
|   |   |---general hardware/software abstraction
|   |   |---ports between pedestals and columns
|   |   |   ports between different components in columns
|
|---columns: 
|   |---generic infrastructural middlewares
|   |   data structure: FIFO, pool, list, hash, timer, etc
|   |   algorithm: calendar, cipher, parser, etc
|   |   multithreading scheduler: xos, Windows, Linux
|   
|---pedestals:
|   |---on-chip peripheral drivers: UART, SPI, USB, I2C, I2S, ADC, RTC, E2PROM, etc
|   |---compiler pragma
|   |   general macro
|
|
low level 


This framework is designed to run on a classical computer based on a Turing Machine model.
Other computer architectures like FPGAs are not concerned.

This framework is intended to be used on MCUs.

Developing principle: Readable, Simple, Scalable.


guide for compiling:
1, the path of "beams", "columns" and "pedestals" should be in your "include path". eg:
    for gcc:
    -I <your path>/beams -I <your path>/columns -I <your path>/pedestals 
2, the header files in <your path>\beams\frameworks\<your framework>\toolchain_preinclude should be pre-included. eg:
    for gcc:
    -include <your path>\beams\frameworks\<your framework>\toolchain_preinclude\preinclude.h
