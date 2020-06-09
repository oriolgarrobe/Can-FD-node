# MPLAB IDE generated this makefile for use with GNU make.
# Project: CANFDPIC18.mcp
# Date: Sun Jun 16 18:37:18 2019

AS = MPASMWIN.exe
CC = mcc18.exe
LD = mplink.exe
AR = mplib.exe
RM = rm

CANFDPIC18.cof : main.o drv_spi.o drv_canfdspi_api.o
	$(LD) /l"C:\Program Files (x86)\Microchip\MCC18\lib" "18f4520_g.lkr" "main.o" "drv_spi.o" "drv_canfdspi_api.o" /u_CRUNTIME /u_DEBUG /u_DEBUGCODESTART=0x7dc0 /u_DEBUGCODELEN=0x240 /u_DEBUGDATASTART=0x5f6 /u_DEBUGDATALEN=0xa /z__MPLAB_BUILD=1 /z__MPLAB_DEBUG=1 /z__MPLAB_DEBUGGER_ICD3=1 /z__ICD2RAM=1 /o"CANFDPIC18.cof" /M"CANFDPIC18.map" /W

main.o : main.c ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/stdlib.h drv_canfdspi_api.h drv_spi.h drv_canfdspi_register.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/spi.h main.c ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/p18f4520.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/stddef.h drv_canfdspi_defines.h main.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/pconfig.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/p18cxxx.h
	$(CC) -p=18F4520 "main.c" -fo="main.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

drv_spi.o : drv_spi.c ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/stdlib.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/spi.h drv_spi.h drv_spi.c drv_canfdspi_defines.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/stddef.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/p18f4520.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/pconfig.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/p18cxxx.h
	$(CC) -p=18F4520 "drv_spi.c" -fo="drv_spi.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

drv_canfdspi_api.o : drv_canfdspi_api.c ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/stdlib.h drv_canfdspi_api.h drv_canfdspi_register.h drv_spi.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/spi.h drv_canfdspi_api.c ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/stddef.h drv_canfdspi_defines.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/p18f4520.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/pconfig.h ../../../../Program\ Files\ (x86)/Microchip/MCC18/h/p18cxxx.h
	$(CC) -p=18F4520 "drv_canfdspi_api.c" -fo="drv_canfdspi_api.o" -D__DEBUG -D__MPLAB_DEBUGGER_ICD3=1 -Ou- -Ot- -Ob- -Op- -Or- -Od- -Opa-

clean : 
	$(RM) "main.o" "drv_spi.o" "drv_canfdspi_api.o" "CANFDPIC18.cof" "CANFDPIC18.hex"

