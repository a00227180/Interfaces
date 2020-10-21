#ifndef VERSATILEPB_PL190_VIC_H
#define VERSATILEPB_PL190_VIC_H


#include "types.h"
// TODO: Complete the following defines according to
//       ARM's versatilepb PL190 VIC specs
#define PICIRQSTATUS_ADDR 0x000 // IRQ status register
#define PICINTENABLE_ADDR 0x010 // Interrupt enable register

// DO NOT MODIFY
#define VIC_STATUS (*(u32*)PICIRQSTATUS_ADDR)
#define VIC_INTENABLE (*(u32*)PICINTENABLE_ADDR)


//check the versatilepb spec for interrupt source bits arrangement
// TODO: Create the correct masks for UART0 and UART1 IRQ VIC bits
#define UART0_IRQ_VIC_BIT 0x800
#define UART1_IRQ_VIC_BIT 0x1000


#endif
