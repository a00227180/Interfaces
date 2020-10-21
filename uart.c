#include "uart.h"
#include "types.h"

// Create 4 UART structs
UART uart[4];                       // 4 UART structures

// Functions defined in startup.s
// TODO:
//      1 - Explain why we need lock and unlock functions
extern void lock();
extern void unlock();

// Function for initializing UART
// TODO:
//      1 - Load UART structs with UART correct addresses
void uart_init(){
    int i; UART *up;
    for (i=0; i<4; i++){ // uart0 to uart2 are adjacent
    up = &uart[i];
    up->base = (char *)(0x101F1000 + i*0x1000);
    *(up->base+CNTL) &= ~0x10; // disable UART FIFO
    *(up->base+IMSC) |= 0x30;
    up->n = i;          ///UART ID number
    up->indata = up->inhead = up->intail = 0;
    up->inroom = SBUFSIZE;
    up->outdata = up->outhead = up->outtail = 0;
    up->outroom = SBUFSIZE;
    up->txon = 0;
    }
uart[3].base = (char *)(ARM_VERSATILE_PL011_UART3); // uart3 at 0x10009000
}

// UART RX interrupt handler
// TODO:
//      1 - Collect incoming chars and store them into inbuf
void do_rx(UART *up) {
    char c;
    c = *(up->base+UDR);
    if (up->outdata <= 0){ // if outbuf[ ] is empty
    *(up->base+TX_BIT) = 0x10; // disable TX interrupt
    up->txon = 0; // turn off txon flag
    return;
    c = up->outbuf[up->outtail++];
    up->outtail %= SBUFSIZE;
    *(up->base+UDR) = (int)c; // write c to DR
    up->outdata--; up->outroom++;
    }
}

// UART TX interrupt handler
// TODO:
//      1 - Check whether outdata is 0
//          This means
//      2 - Clear the MIS[TX] by disable IMSC[TX] interrupt.
//          Otherwise, the MIS[TX] will never disappear and the execution
//          will dead loop in the IRQ handling.
//      3 - Write c to output data register.
//          This will clear TX IRQ
void do_tx(UART *up){
    char c;
    c = *(up->base+UDR);
    if (c==0xD)
    up->inbuf[up->inhead++] = c;
    up->inhead %= SBUFSIZE;
    up->indata++; up->inroom--;
}

// UART IRQ_handler
// TODO:
//      1 - Read MIS register
//      2 - Call RX or TX depending on MIS register value
void uart_handler(UART *up) {
    u8 mis = *(up->base + MIS); // read MIS register
    if (mis & (1<<4)) // MIS.bit4=RX interrupt
    do_rx(up);
    if (mis & (1<<5)) // MIS.bit5=TX interrupt
    do_tx(up);
}

// UGETC consumes chars from inbuf[]
// TODO:
//      1 - Wait until there's something in indata
//      2 - Read inbuf and
u8 ugetc(UART *up){
    char c;
    while(up->indata <= 0); // loop until up->data > 0 READONLY
    c = up->inbuf[up->intail++];
    up->intail %= SBUFSIZE;
    // updating variables: must disable interrupts
    lock();
    up->indata--; up->inroom++;
    unlock();
    return c;
}

// TODO:
//      1 - Place char in outbut when txon is set to 1
//      2 - Read inbuf
//      3 - Set txon
//      4 - Enable TX RX interrupt by setting IMSC[TX][RX] bits
//      5 - Write char to data register
void uputc(UART *up, u8 c){
    if (up->txon){ //if TX is on, enter c into outbuf[]
    up->outbuf[up->outhead++] = c;
    up->outhead %= 128;
    lock();
    up->outdata++; up->outroom--;
    unlock();
    return;
    }
    // txon==0 means TX is off => output c & enable TX interrupt
    // PL011 TX is riggered only if write char, else no TX interrupt
    int i = *(up->base+UFR); // read FR
    while( *(up->base+UFR) & 0x20 ); // loop while FR=TXF
    *(up->base+UDR) = (int)c; // write c to DR
    *(up->base+IMSC) |= 0x30; // 0000 0000: bit5=TX mask bit4=RX mask
    up->txon = 1;
}

// UGETS gets a whole line from UART
// TODO:
//      1 - When end of line is detected, echo back user characters
void ugets(UART *up, char *s){
    while ((*s = (char)ugetc(up)) != '\r'){
    uputc(up, *s++);
    }
    *s = 0;
}

// UPRINTS prints a whole line into UART
void uprints(UART *up, u8 *s){
    while(*s)
    uputc(up, *s++);
}
