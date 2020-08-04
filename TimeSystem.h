/* 
 * File:   TimeSystem.h
 * Author: Cesar Daltoe Berci
 *
 * Created on June 3, 2020, 10:16 PM
 * 
 * Revision History:
 * 03/06/2020 -> Created
 * 03/06/2020 -> Stable
 * 
 * @details
 * Time Control System.
 * Contain variables and functions to control temporized process. It maintains
 * a current time milli seconds variable, that can be read direct from the 
 * structure, and also provide callback timers, to handle temporized 
 * callback functions.
 */

#ifndef TIMESYSTEM_H
#define	TIMESYSTEM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define NUMBER_OF_TIMERS 5
    typedef unsigned long long timeMillis;

#include <stdint.h>
#include <stdbool.h>

    /**
     * Timer
     * @brief
     * Structure to define timer objects.
     * @details
     * This objects ares used to control temporized actions and can run 
     * using callback functions or pulling via isDone function.  <br>
     * If no callback functions was defined, the timer will work without 
     * callback, but, once defined a callback, the isDone function will no
     * longer work.
     */
    typedef struct TIMER {
        timeMillis time; /*!< Time to wait */
        bool autoReset; /*!< If true, the timer will continously repeat */
        /**
         * isDone
         * This function are used to verify timer state. If the time delay was
         * reached, it will return true, only once. If autoReset is set, it will
         * also reset the timer.
         * @return done True if timer was reach the delay time or false, 
         * otherwise.
         * @param this Pointer to the caller object.
         */
        bool (*isDone)(struct TIMER * this);
        /**
         * reset
         * Reset the time counter.
         * @param this Pointer to the caller object.
         */
        void (*reset)(struct TIMER * this);
        void (*setCallback)(struct TIMER * this, void (*callback)(void));
        /**
         * Callback function
         * If set, this function will be called at the end of timer job. 
         * If the autoReset is set, this function will be called in every
         * timer finish.
         * @param this Pointer to the caller object.
         * @param callcback Callback function to run after timer finish.
         */
        void (*callBack)(void);
        /* local variables */
        timeMillis start;
        bool active;
        bool done;
    } Timer;

    /**
     * Timer System Structure
     * Controls timer measure system. 
     */
    typedef struct TIMESYSTEM {
        timeMillis currentTimeMillis; /*!< Actual currentTimeMillis */
        /**
         * updateMillis
         * This function are used to maintain the currentTimeMillis updated. 
         * It should be called by a 1 millisecond periodically function, like
         * a interruption procedure.
         */
        void (*updateMillis)(void);
        /**
         * tasks
         * State control function, should be called periodically, using either 
         * synchronous or not synchronous functions.
         */
        void (*tasks)(void);
        /**
         * addTimer 
         * Adds a new timer to the time control system. It is necessary to 
         * control the timer object.
         * @param timer Timer object to be add.
         */
        void (*addTimer)(Timer * timer);
        /**
         * removeTimer
         * Removes a timer object from time control system, after removed
         * the timer object will be no longer updated.
         */
        void (*removeTimer)(Timer * timer);                
    } TimeSystem;

    /**
     * GetTimeSystemInstance
     * @brief
     * Gets the timer system instance
     * @details
     * This function returns the timer system unique instance. If the instance
     * was not initiated, the first call of this function will create the time 
     * control system properties and functions.
     * @return Timer System unique instance.
     */
    TimeSystem *GetTimeSystemInstance(void);
    /**
     * NewTimer
     * @brief
     * Creates a new timer
     * @brief
     * This function creates a new timer object, with the specified timer 
     * period, and returns the object. This object will be maintained by the
     * caller function while its being used.
     * @param time Time period
     * @return timer object
     */
    Timer NewTimer(timeMillis time);

#ifdef	__cplusplus
}
#endif

#endif	/* TIMESYSTEM_H */

