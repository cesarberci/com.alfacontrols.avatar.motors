/* 
 * File:   Comm.h
 * Author: Cesar Daltoe Berci
 * 
 * Created on June 8, 2020, 3:41 PM
 * 
 * Revision History:
 * 08/06/2020 -> Created
 * 09/06/2020 -> Stable
 * 
 * @details
 * Implementation of a simple communication protocol based on SPI driver.
 * Due SPI hardware addressing capability, it will no longer verify source and
 * destination address.<br>
 * The implementation is based on a singleton patterns, than, only one instance
 * will be allowed in a program.<br>
 * The protocol implements a register based memory access, including function 
 * space. It can handle read and write operations on its registers. <br>
 * Errors are avoided by the checksum in every frame, and a sequence code, that
 * need to be updated in every new request.
 * 
 * Main definitions
 *     -> Registers memory based
 *     -> Fixed frame format
 *     -> Only one register affected per transaction
 *     -> Sequence code should be updated in every new request
 *     -> Requests should be read or write operations
 *     -> A fail operation will return a error code
 *     -> A success operation will return data of the 
 *     -> A sequence error or checksum error will have no response
 *     -> Maximum SPI speed: 30000
 * 
 * Especial registers
 *     -> 100 Error code register
 * 
 * @code
 * //System init section
 * comm = GetCommunicationInstance();
 * CommRegister regs[] = {
 *   { .action = 0, .val = &speedm1, .reg = 10},
 *   { .action = 0, .val = &speedm2, .reg = 20},
 *   { .action = updateSpeeds, .val = 0, .reg = 30},
 * };
 * comm->regsTable->setRegistersTable(regs, sizeof regs);
 * comm->commDriver = &spi;
 * comm->setTimeBaseCounter((uint32_t *) &time->currentTimeMillis);
 * 
 * //Periodic tasks section
 * comm->tasks();
 */

#ifndef COMM_H
#define	COMM_H

#ifdef	__cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stdbool.h>

#include "SpiCommunication.h"

#define NUMBER_OF_REGISTERS 10
#define NULL_REGISTER 0xFF
#define ERROR_CODE_REG 100
#define FRAME_SIZE 5

    /**
     * Communication errors definition.
     */
    typedef enum COMM_ERROR {
        COMM_ERR_OK = 0,
        COMM_ERR_BAD_CHKS = 1,
        COMM_ERR_BAD_SEQUENCE = 2,
        COMM_ERR_INVALID_REG = 4,
        COMM_ERR_READ_TIMOUT = 8,
        COMM_ERR_WRITE_TIMEOUT = 16,
        COMM_ERR_last = 0xFFFF,        
    } CommError;

    /**
     * Communication Frame Header
     * @details
     * It defines the header space on a communication frame, and contain only
     * two variables.
     * The memory is addressed by a unsigned 8 bits variable, that can be 
     * accessed by .val field.
     */
    typedef union FRAME_HEADER {
        uint8_t val;
        struct {
            uint8_t write : 1; /*!< Write/Read, 1 for write operations */
            uint8_t sequence : 4; /*!< 1 Nibble sequence code */
            uint8_t reserved : 3; /*!< Reserved space */
        } d;
    } CommFrameHeader;

    /**
     * Memory register
     * @details
     * It defines the communication memory space, and include memory variables
     * and functions. <br>
     * If action is defined, the register will be treated as a function, 
     * otherwise, it will be recognized as a memory variable, and will
     * have its value updated and read.
     */
    typedef struct COMM_REG {
        void (*action)(void); /*!< Action to be performed in a write operation*/
        uint16_t * val; /*!< Pointer to local memory variable */
        uint8_t reg; /*!< Register number (protocol number) */
    } CommRegister;

    /**
     * Communication Frame
     * @details
     * Contains all fields definitions, include headers, values and error
     * checker.
     */
    typedef union COMM_FRAME {
        uint8_t data[FRAME_SIZE]; /*!< Array is all frame data */
        struct {
            CommFrameHeader header; /*!< Frame header */
            uint8_t reg; /*!< Register numner */
            uint16_t value; /*!< Register value */
            uint8_t checksum; /*!< Error verify */
        } fields;
    } CommFrame;

    /**
     * Communication relational table.
     * @details
     * It defines the relations between communication protocol and local data,
     * holding a table with all application registers.
     */
    typedef struct COMM_TABLE {
        /**
         * setRegistersTable
         * @brief
         * Sets the internal registers table.
         * @details
         * This function copies the data provided to the internal storage 
         * variables, that will maintain the relational parameters during all
         * application lifetime.
         * @param tab Registers array
         * @param length Size of the registers array
         * @code
         * CommRegister regs[] = {
         *   { .action = 0, .val = &var1, .reg = 10}, //var1 to register 10
         *   { .action = 0, .val = &var2, .reg = 20}, //var2 to register 20
         *   { .action = l_function, .val = 0, .reg = 30}, //register 30 func.
         * };
         * comm->regsTable->setRegistersTable(regs, sizeof regs);
         */
        void (*setRegistersTable)(CommRegister * tab, uint8_t length);
        CommRegister regs[NUMBER_OF_REGISTERS]; /*!< Registers table */
        /**
         * getRegister
         * @brief
         * Private function used by the internal algorithm.
         */
        CommRegister * (*getRegister)(uint8_t address);
    } RelationalTable;

    /**
     * Communication System
     * @brief
     * Structure of the communication object.
     */
    typedef struct COMMUNICATION {
        /**
         * tasks
         * @brief
         * Communication state tasks, should be called periodically and 
         * asynchronously
         */
        void (*tasks)(void);
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
        Spi * commDriver; /*!< SPI hardware driver, should be defined by the 
                           * user application on system startup.*/
        RelationalTable * regsTable; /*!< Private internal variable. */
        CommError errorCode; /*< Error state variable. */
    } Communication;

    /**
     * GetCommunicationInstance
     * @brief
     * Getter for the unique instance of the communication object.
     * @return instance Pointer to the communication object.
     */
    Communication * GetCommunicationInstance(void);

#ifdef	__cplusplus
}
#endif

#endif	/* COMM_H */

