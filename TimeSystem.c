#include "TimeSystem.h"
#include <stddef.h>

/*
 ------------------------------------------------------------------------------
 Functions prototypes
 */
static void updateMillis(void);
static void tasks(void);
static void resetTimer(Timer * this);
static void addTimer(Timer * timer);
static void removeTimer(Timer * timer);
static bool isDone(Timer * this);
static void setCallback(Timer * this, void (*callback)(void));
static uint32_t * currentTimeMillis;

/*
 * ----------------------------------------------------------------------------
 * Variables
 */
static Timer * timers[NUMBER_OF_TIMERS]; /*!< timers, xc8 does not support
                                         * dinamic memory access. */
static uint8_t nTimers = 0;
static TimeSystem instance = {
    .currentTimeMillis = 0,
    .updateMillis = updateMillis,
    .tasks = 0,
    .addTimer = addTimer,
    .removeTimer = removeTimer,
};

/*
 * ----------------------------------------------------------------------------
 * Interface functions implementations
 */
TimeSystem *GetTimeSystemInstance(void) {
    if (instance.tasks == 0) {
        instance.tasks = tasks;
    }
    return &instance;
}

/*
 * ----------------------------------------------------------------------------
 * Local functions implementations
 */
static void updateMillis(void) {
    instance.currentTimeMillis++;
}

static void tasks(void) {
    for (uint8_t i = 0; i < nTimers; i++) {
        if (!timers[i]->done) {
            if ((instance.currentTimeMillis - timers[i]->start) >= timers[i]->time) {
                timers[i]->done = true;
                if (timers[i]->callBack > 0) {
                    timers[i]->isDone(timers[i]);
                    timers[i]->callBack();
                }
            }
        }

    }
}

static void addTimer(Timer * timer) {
    timers[nTimers] = timer;
    nTimers++;
}

static void removeTimer(Timer * timer) {
    for (size_t i = 0; i < nTimers; i++) {
        if (timers[i] == timer) {
            nTimers--;
            for (size_t j = i; j < nTimers; j++)
                timers[j] = timers[j + 1];
            return;
        }
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////       Timer Implementations       /////////////////////////////
///////////////////////////////////////////////////////////////////////////////

Timer NewTimer(timeMillis time) {
    Timer tmr = {
        .isDone = isDone,
        .reset = resetTimer,
        .time = time,
        .start = instance.currentTimeMillis,
        .autoReset = true,
        .done = false,
        .setCallback = setCallback,
    };
    return tmr;
}

static void resetTimer(Timer * this) {
    this->start = instance.currentTimeMillis;
    this->active = true;
}

static bool isDone(Timer * this) {
    if (this->done) {
        this->active = false;
        if (this->autoReset)
            resetTimer(this);
        this->done = false;
        return true;
    }
    return false;
}

static void setCallback(Timer * this, void (*callback)(void)) {
    this->callBack = callback;
}