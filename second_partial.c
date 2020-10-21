#include "types.h"
#include "versatilepb_pl190_vic.h"
#include "uart.h"
#include "uart.c"           // UART driver
// Declare extern functions (if needed)


void main()
{
    u8 line[128];
    UART *up;
    // TODO: Update VIC_INTENABLE in order to enable UART IRQs
    VIC_INTENABLE |=UART0_IRQ_VIC_BIT;
    VIC_INTENABLE |=UART1_IRQ_VIC_BIT;

    uart_init();
    up= &uart[0];

    while(1)
    {
        uprints(up, "Enter a line from UART:\n\r");
        ugets(up, line);
        uprints(up, "You have entered below line from UART:\n\r");
        uprints(up, line);
    }
}

// INFO: DO NOT MODIFY copy_vectors subroutine
// Copy interrupt vectors to relevant addresses
// Interrupt vector addresses are defined in startup.s
void copy_vectors()
{
    extern u32 vectors_start, vectors_end;
    u32 *vectors_src = &vectors_start;
    u32 *vectors_dst = (u32 *)0;
    while(vectors_src < &vectors_end)
    {
        *vectors_dst++ = *vectors_src++;
    }
}

// IRQ handler
// TODO:
//      1 - Read VIC_STATUS reg in order to identify interrupt source
//      2 - Call UART handler(s)
void IRQ_handler() {
    // read VIC status registers to find out which interrupt
    int vicstatus = VIC_STATUS;
    // VIC status BITs: timer0,1=4,uart0=13,uart1=14
    
    if (vicstatus & (1<<12)) // bit12=1: uart0
    uart_handler(&uart[0]);
    if (vicstatus & (1<<13)) // bit13=1: uart1
    uart_handler(&uart[1]);
   
}
