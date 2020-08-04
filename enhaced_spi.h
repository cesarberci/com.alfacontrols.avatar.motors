/* 
 * Enhaced SPI Driver
 * 
 * File:   enhaced_spi
 * Author: Cesar Daltoe Berci
 * Comments: Enhanced SPI Wrapper, created to complement CODE CONFIGURATOR 
 * codes to handle spi hardware
 * Revision history: june 2020, fisrt version
 */

// This is a guard condition so that contents of this file are not included
// more than once.  
#ifndef ENHANCED_SPI_H
#define	ENHANCED_SPI_H

#include <xc.h> // include processor files - each processor file is guarded.  

// TODO Insert appropriate #include <>
#include <stdint.h>
#include <stdbool.h>

// TODO Insert C++ class definitions if appropriate

// TODO Insert declarations
/**
 * Spi open
 * @details
 * Opens the spi driver for start communications. It should be called  
 * before using write and read functions.
 * @return True if spi is open and false otherwise.
 */
bool espi_op(void);
/**
 * Spi write thread 1
 * @details
 * Thread safe spi write function.
 * <p> Spi write function to be used in thread 1.</p>
 * <p> This function write data in spi buffer register, and do not wait for
 * transmission, than, the user should handle the interrupt or read flags to 
 * assure the transmission.
 * @param byte Data to be write
 */
inline void espi_wr1(uint8_t byte);
/**
 * Spi write thread 2
 * @details
 * Thread safe spi write function.
 * <p> Spi write function to be used in thread 1.</p>
 * <p> This function write data in spi buffer register, and do not wait for
 * transmission, than, the user should handle the interrupt or read flags to 
 * assure the transmission.
 * @param byte Data to be write
 */
inline void espi_wr2(uint8_t byte);
/**
 * Spi read function
 * @details
 * This function only return the value in spi buffer. It do not block to wait
 * for data, than, if there is no data received, it will return the last data
 * on buffer. The user should use interrupt or flags to assure that there is 
 * new data to read.
 * @return data um buffer
 */
uint8_t espi_rd(void);
/**
 * Spi close
 */
inline void espi_cl(void);
/**
 * Set interrupt function
 * @param fun Function to handle spi interrupt
 */
void inline espi_setInterrupHandler(void (*fun)(void));
/**
 * Reset
 * @details
 * Reset spi driver data and state.
 */
void espi_rst(void);
/**
 * Initilize SPI driver
 * @param ie
 */
void espi_int(bool ie);

#ifdef	__cplusplus
extern "C" {
#endif /* __cplusplus */

    // TODO If C++ is being used, regular C code needs function names to have C 
    // linkage so the functions can be used by the c code. 

#ifdef	__cplusplus
}
#endif /* __cplusplus */

#endif	/* ENHANCED_SPI_H */

