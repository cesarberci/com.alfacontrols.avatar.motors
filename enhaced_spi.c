#include "enhaced_spi.h"
#include "mcc_generated_files/spi.h"

static void (*SPI_InterruptHandler)(void);

static uint8_t buff;

static void interrupHandler(void) {
    if (PIR2bits.BCL1IF) { // bus colision detected
        PIR2bits.BCL1IF = 0;
        PIE2bits.BCL1IE = 0;
        PIE1bits.SSP1IE = 0;        
        asm(" nop");
        PIE1bits.SSP1IE = 1;
        PIE2bits.BCL1IE = 1;
        PIE1bits.SSP1IE = 1;
    } else
        SPI_InterruptHandler();

}

bool espi_op(void) {
    return SPI_Open(SPI_DEFAULT);
}

inline void espi_wr1(uint8_t byte) {
    SSP1BUF = byte;
}

inline void espi_wr2(uint8_t byte) {
    SSP1BUF = byte;
}

void inline espi_setInterrupHandler(void (*fun)(void)) {
    SPI_InterruptHandler = fun;
    SPI_SetInterruptHandler(interrupHandler);
}

uint8_t espi_rd(void) {
    return SSP1BUF;
}

inline void espi_cl(void) {
    SSP1CON1bits.SSPEN = 0;
}

void espi_rst(void) {
    SSP1CON1bits.SSPEN = 0;
    asm(" nop");
    asm(" nop");
    asm(" nop");
    espi_op();
    SSP1CON1bits.WCOL = 0;
    while (SSP1STATbits.BF)
        buff = SSP1BUF;
}

void espi_int(bool ie) {
    if (ie) {
        PIE2bits.BCL1IE = 1;
        PIE1bits.SSP1IE = 1;
    } else {
        PIE2bits.BCL1IE = 0;
        PIE1bits.SSP1IE = 0;
    }
}