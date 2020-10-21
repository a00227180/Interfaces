#ifndef UART_H
#define UART_H

#include "types.h"

// TODO: Complete these defines with the correct addresses
//       Review versatilepb spec doc
#define ARM_VERSATILE_PL011_UART0 0x101F1000 // UART0
#define ARM_VERSATILE_PL011_UART1 0x101F2000 // UART1
#define ARM_VERSATILE_PL011_UART2 0x101F3000 // UART2
#define ARM_VERSATILE_PL011_UART3 0x10009000 // UART3

// TODO: Complete these defines with the correct addresses
//       UART registers - Refer to PL110 spec doc
#define UDR  0x00 // Data Register
#define UDS  0x04 // RX status or error clear
#define UFR  0x18 // Flag register - it reflects the working status of a UART instance
#define CNTL 0x2c // Line control
#define IMSC 0x38 // Interrupt Mask Set/Clear, this enable(set)/disable(clear) a certain interrupt
#define MIS  0x40 // Masked Interrupt Status, this reflect the status of a certain interrupt

// TODO: Complete the defines with the correct addresses
//       These defines correspond to UART FLAG REGISTER
#define TXFE 0x80 // fifo is disabled, bit 7 set when tx holding register is empty
#define TXFF 0x20 // fifo is disabled, bit 6 set when tx holding register is full
#define RXFE 0x10 // fifo is disabled, bit 6 set when rx holding register is empty
#define RXFF 0x40 // fifo is disabled, bit 6 set when rx holding register is full
#define BUSY 0x08 // bit 3 set when UART busy transmitting data

// TODO: Create the correct bit masks in IMSC register and MIS register
#define RX_BIT 0x20 // RX interrupt mask bit in IMSC register, also RX interrupt status bit in MIS register.
#define TX_BIT 0x10 // TX interrupt mask bit in IMSC register, also TX interrupt status bit in MIS register.

// String buffer size
#define SBUFSIZE 128

// TIPS:
//       outdata: how many chars in outbuf to be transmitted
//       outroom: how much room remaining in outbuf
//       outhead: the latest char buffered
//       outtail: the next char to be transmitted

typedef volatile struct uart {
    u8 * base;    // base address of UART
    u32 n;          // uart id 0~3
    u8 inbuf[SBUFSIZE];
    u32 indata, inroom, inhead, intail;
    u8 outbuf[SBUFSIZE];
    u32 outdata, outroom, outhead, outtail;
    volatile u32 txon;  // 1 = TX interrput is on, the UART is in transmitting state
} UART;

void uart_init();


#endif
