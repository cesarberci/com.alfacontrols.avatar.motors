/* 
 * File:   DcMotor.h
 * Author: Cesar Daltoe Berci
 *
 * Created on June 4, 2020, 10:15 AM
 * 
 * @details
 * This file contain algorithms to controls DC Motors, independent from hardware
 * implementation, based on the software interface. <br>
 * The motors structures should be created using two hardware functions, that
 * controls the MCU outputs.
 */

#ifndef DCMOTOR_H
#define	DCMOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <stdbool.h>
#include <stdint.h>

    /**
     * MotorState
     * Motor state definition
     */
    typedef enum MOTORSTATE {
        MOTOR_RUNNING,
        MOTOR_OFF,
        MOTOR_WAITING_OFF,
        MOTOR_WAITING_CHANGE_DIR,
    } MotorState;

    /**
     * DCMotor
     * @breif
     * DC Motor control structure.
     * @details
     * This structure provide methods and properties to control DC Motors using
     * PWM outputs. <br> 
     * The hardware duty are controlled by tow definitions, MIM_HW_SPEED and 
     * MAX_HW_SPEED, that contains the limits for hardware PWM duty. <br>
     * It also implements a security time before change motor direction. This is
     * useful when using mechanical relays to control motor direction.
     * 
     */
    typedef struct DCMOTOR {
        /**
         * setDir
         * @details
         * Set motor direction, using clockwise and counter clockwise as a 
         * binary option.
         * @param this Pointer to this structure.
         * @param dir Boolean direction.
         */
        void (*setDir)(struct DCMOTOR * this, bool dir);
        /**
         * setSpeed
         * @details
         * Set motor speed. The speed should be a percentage value multiplied 
         * by 10, range: 0% (0) -> 100% (1000).
         * @param this Pointer to this structure.
         * @param speed_X10 Speed
         */
        void (*setSpeed)(struct DCMOTOR * this, uint16_t speed_X10);
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
         * stop
         * @details
         * Stop motor, by setting it to its minimum speed. This function do not
         * changes the motor speed variable, only change its state, this way, a 
         * run() function will restart the motor at it last speed.
         * @param this Pointer to this structure.
         */
        void (*stop)(struct DCMOTOR * this);
        /**
         * run
         * @details
         * Run motor, sets its speed to the last configured speed and changes 
         * its internal state. If the speed is set to 0%, the motor will remain
         * stop, but, its state will be on.
         * @param this Pointer to this structure.
         */
        void (*run)(struct DCMOTOR * this);
        /**
         * accelerate
         * @breif
         * Increases motor speed.
         * @param this Pointer to this structure.
         */
        void (*accelerate)(struct DCMOTOR * this);
        /**
         * slowDown
         * @breif
         * Decreases motor speed.
         * @param this Pointer to this structure.
         */
        void (*slowDown)(struct DCMOTOR * this);
        /**
         * changeDir
         * @breif
         * Invert motor direction.
         * @param this Pointer to this structure.
         */
        void (*changeDir)(struct DCMOTOR * this);
        /**
         * Motor control tasks
         * @details
         * This function should be called, asynchronously, in order to maintain
         * the internal state control.
         * @param this Pointer to this structure.
         */
        void (*tasks)(struct DCMOTOR * this);        
        void (*hwSetDir)(bool dir); /*<! Hardware set dir function */
        void (*hwSetSpeed)(uint16_t value); /*<! Hardware set speed function */
        /* Local functions and variables */
        bool dir;
        bool running;
        MotorState state;
        uint16_t speed;
        uint32_t timer;
        bool changed;
    } DCMotor;

    /**
     * NewDCMotor
     * @brief
     * Creates a new DC Motor object.
     * @details
     * This function will return a new DC Motor structure, using the hardware
     * defined by the functions _setDir and _setSpeed.
     * @param _setDir Hardware function to set direction.
     * @param _setSpeed Hardware function to set speed.
     * @return DC Motor structure.
     */
    DCMotor NewDCMotor(void (*_setDir)(bool), void (*_setSpeed)(uint16_t));

#ifdef	__cplusplus
}
#endif

#endif	/* DCMOTOR_H */

