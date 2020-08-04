/* 
 * File:   Spi.h
 * Author: Cesar Daltoe Berci
 *
 * Created on June 8, 2020, 11:01 AM
 * Revision History:
 * 08/06/2020 -> Created
 * 08/06/2020 -> Stable
 * 
 * @brief
 * Implement software based interface for a hardware SPI communication driver
 * 
 * @details
 * This implementation provide asynchronous and non blocking methods to 
 * exchange data using a SPI device.<br>
 * All operation are controlled by a tasks function, that maintain the
 * communication driver state and handle read and write requests.<br>
 * The procedures and callback functions are also based on the asynchronous 
 * tasks and do not run on the interrupt routine.<br>
 * The client application can be design to use both callback functions and
 * state based structure, to read and write data from SPI driver.<br>
 * Due this asynchronous access to hardware drivers, this implementation can
 * run in half and full duplex mode.
 * 
 * @code 
 * //Init section
 * 
 * Spi spi;
 * spi = NewSpiDriver(open_spi, SPI_Close, SPI_ReadByte, SPI_WriteByte);
 * spi.setTimeBaseCounter((uint32_t *) &time->currentTimeMillis);
 * spi.open(&spi);
 * 
 * //Periodic tasks section
 * spi.tasks(&spi);
 * 
 * //Interrupt service routine
 * static void SPI_Isr(void) {
 *    spi.exchangeDone(&spi);
 * }
 * 
 * //read and write calls
 * spi->read(spi, data, FRAME_SIZE)
 * spi->write(spi, data, FRAME_SIZE)
 */

#ifndef SPICOMM_H
#define	SPICOMM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>
    /**
     * SPI communication state
     */
    typedef union {
        uint8_t val;

        struct {
            uint8_t open : 1; /*!< driver open */
            uint8_t reserved_6 : 1; /*!<reserved */
            uint8_t reserved_5 : 1; /*!<reserved */
            uint8_t reserved_4 : 1; /*!<reserved */
            uint8_t sendComplete : 1; /*!< send operantion completed */
            uint8_t receiveComplete : 1; /*!< receive operation completed */
            uint8_t byteRead : 1; /*!< one byte read withou request */
            uint8_t reserved_0 : 1; /*!< reserved */
        } b;
    } SpiState;

    /**
     * SPI Communication structure
     * @brief
     * Functions and properties of a SPI communication driver
     * @details
     * This structure define high level functions to control data input and
     * output using a SPI hardware driver. <br>
     * An application can have many SPI drivers, but, every one needs a
     * dedicated hardware.<br>
     * @code 
     * //Init section
     * Spi spi;
     * spi = NewSpiDriver(open_spi, SPI_Close, SPI_ReadByte, SPI_WriteByte);
     * spi.setTimeBaseCounter((uint32_t *) &time->currentTimeMillis);
     * spi.open(&spi);
     * //Periodic tasks section
     * spi.tasks(&spi);
     */
    typedef struct SPI {
        /**
         * read
         * @brief
         * Asynchronous reads data
         * @details
         * This function read length bytes of data from SPI driver and store in 
         * the provided pointer.<br>
         * Once finished, the function rxDone will return true.
         * @param this Pointer to this object.
         * @param data Pointer to data array where data will be store
         * @param length Number of bytes to read
         * @return 0 if operation success, or, a negative number if fail 
         */        
        int (*read)(struct SPI * this, uint8_t * data, uint8_t length);
        /**
         * write
         * @brife
         * Asynchronous write data to SPI driver
         * @details
         * Writes length bytes of data in the output driver.<br>
         * Once finished, the function txDone will return true.
         * @param this Pointer to this object.
         * @param data Pointer to data array to be write in the output
         * @param length Number of bytes to write
         * @return 0 if operation success, or, a negative number if fail 
         */
        int (*write)(struct SPI * this, uint8_t * data, uint8_t length);
        /**
         * open
         * @brief
         * Open communication hardware driver.
         * @param this Pointer to this object.
         */
        void (*open)(struct SPI * this);
        /**
         * close
         * @brief
         * Close communication hardware driver.
         * @param this Pointer to this object.
         */
        void (*close)(struct SPI * this);
        /**
         * tasks
         * @brief
         * Asynchronous periodic tasks
         * @param this Pointer to this object.
         */
        void (*tasks)(struct SPI * this);
        /**
         * txDone
         * @brief
         * Verify if there is a transmit operation in progress.
         * @param this Pointer to this object.
         * @return True if there is no write operation in progress and false 
         * otherwise
         */
        bool (*txDone)(struct SPI * this);
        /**
         * txDone
         * @brief
         * Verify if there is a receive operation in progress.
         * @param this Pointer to this object.
         * @return True if there is a receive operation in progress or false
         * otherwise
         */
        bool (*rxDone)(struct SPI * this);
        /**
         * setTimeBaseCounter
         * @details
         * The motor state control requires a time base counter, to count
         * milliseconds in order to control the timing between states changes.
         * This function can be called only once, and it is not needed to be
         * called for all motors.
         * @param timeMillis Pointer to milliseconds counter.
         */
        void (*setTimeBaseCounter)(uint32_t * timeMillis);
        /**
         * reset
         * @brief
         * Reset hardware driver
         * @details
         * This function can be called externally when a inconsistent 
         * communication state is detected. Its also called by the internal 
         * algorithm when the transmit or receive timeout was detected.
         * @param this Pointer to this object.
         */
        void (*reset)(struct SPI * this);     
        /**
         * exchangeDone
         * @brief
         * Inform the end of a SPI transaction
         * @details
         * This function should be called by the hardware interrupt service, in
         * order to execute the exchange operations.
         * @param this Pointer to this object.
         * @code
         * static void SPI_Isr(void) {
         *    spi.exchangeDone(&spi);
         * }
         */
        void (*exchangeDone)(struct SPI * this);
        /**
         * readCallBack
         * @brief
         * Callback function
         * @details
         * This function will run when a read operation was done.
         * <p>If this function was not defined, no callback will be generated
         * for this parameter </p>
         */
        void (*readCallBack)(void);
        /**
         * writeCallBack
         * @brief
         * Callback function
         * @details
         * This function will run when the write operation was done.
         * <p>If this function was not defined, no callback will be generated
         * for this parameter </p>
         */
        void (*writeCallBack)(void);
        /**
         * byteReadCallBack
         * @brief
         * Callback function
         * @details
         * This function will run when a byte was received and neither read or
         * write operation are in progress.
         * <p>If this function was not defined, no callback will be generated
         * for this parameter </p>
         */
        void (*byteReadCallBack)(void);
        /**
         * errorCallBack
         * @brief
         * Callback function
         * @details
         * This function will be called if there is a error in the SPI driver.
         * <p>If this function was not defined, no callback will be generated
         * for this parameter </p>
         */
        void (*errorCallBack)(void);    
        /**
         * healthy
         * @brief 
         * State of the object 
         * @details
         * If the object are running normally, this function returns true, 
         * otherwise, if there is an unrecoverable error, or inconsistent state,
         * this function will return false.
         * @param this Pointer to this object.
         * @return healthy
         */
        bool (*healthy)(struct SPI * this);
        uint8_t rChar; /*!< received data */        
        /* Algorithm local functions and variables */               
        bool (*hwopen)(void);        
        void (*hwclose)(void);
        void (*hwreset)(void);
        uint8_t(*readByte)(void);
        void (*writeByte_1)(uint8_t d); /*<! Thread safe write functions */
        void (*writeByte_2)(uint8_t d); /*<! Thread safe write functions */
        void (*hwInterrupEnable)(bool ie);
        int charsToReceive;
        int charsToSend;
        SpiState state;
        uint32_t timer;
        uint8_t * dataToRead;
        uint8_t * dataToWrite;        
        bool newData;
    } Spi;

    /**
     * NewSpiDriver
     * @brief
     * Creates a SPI communication driver
     * @details
     * This function creates a new object to handle a hardware SPI driver, 
     * based on the provided hardware access functions.
     * @param _op Hardware open function
     * @param _cl Hardware close function
     * @param _rd Hardware read single byte function
     * @param _wr1 Hardware write single byte function thread 1
     * @param _wr2 Hardware write single byte function thread 2
     * @param _rst Hardware reset function
     * @return New SPI communication object.
     */
    Spi NewSpiDriver(bool (*op)(void), void (*cl)(void), uint8_t (*rd)(void), void (*wr1)(uint8_t), void (*wr2)(uint8_t), void (*rst)(void), void (*ie)(bool st));

#ifdef	__cplusplus
}
#endif

#endif	/* SPI_H */

