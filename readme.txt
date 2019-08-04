Universal Cross-Platform Framework(UCPF)

physical organization£º

high level
^
|
|---applications
|   |---applications are placed here.
|
|---beams: 
|   |---application-oriented hardware/software abstraction:
|   |   |---HAL, event driver framework, etc
|   |   |   OS, none OS
|   |   |   ports between pedestals and columns
|   |   |   ports between different components in columns
|   |
|   |---general hardware/software abstraction:
|   |   |---boards
|   |   |   thirdparty middlewares
|
|---columns: 
|   |---multithreading scheduler: xos, Windows, Linux
|   |   protocol stack: TCP/IP, USB, CANOpen, Xmodem, etc
|   |   other middleware: FIFO, pool, list, hash, timer, etc
|   
|---pedestals:
|   |---chip-level peripheral drivers: UART, SPI, USB, I2C, I2S, ADC, RTC, E2PROM, etc
|   |---board-level peripheral drivers: E2PROM, FLASH, RTC, ADC, Codec, Modules, etc
|   |---compiler pragma
|   |   general macro
|
|
low level 


This framework is designed to run on a classical computer based on a Turing Machine model.
Other computer architectures like FPGAs are not concerned.

This framework is intended to be used on MCUs.

Developing principle: Readable, Simple, Scalable.