# 1 "TimeSystem.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "TimeSystem.c" 2
# 1 "./TimeSystem.h" 1
# 27 "./TimeSystem.h"
    typedef unsigned long long timeMillis;


# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdint.h" 1 3



# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\musl_xc8.h" 1 3
# 4 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdint.h" 2 3
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 127 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long uintptr_t;
# 142 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long intptr_t;
# 158 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef signed char int8_t;




typedef short int16_t;




typedef __int24 int24_t;




typedef long int32_t;





typedef long long int64_t;
# 188 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long long intmax_t;





typedef unsigned char uint8_t;




typedef unsigned short uint16_t;




typedef __uint24 uint24_t;




typedef unsigned long uint32_t;





typedef unsigned long long uint64_t;
# 229 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned long long uintmax_t;
# 22 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdint.h" 2 3


typedef int8_t int_fast8_t;

typedef int64_t int_fast64_t;


typedef int8_t int_least8_t;
typedef int16_t int_least16_t;

typedef int24_t int_least24_t;

typedef int32_t int_least32_t;

typedef int64_t int_least64_t;


typedef uint8_t uint_fast8_t;

typedef uint64_t uint_fast64_t;


typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;

typedef uint24_t uint_least24_t;

typedef uint32_t uint_least32_t;

typedef uint64_t uint_least64_t;
# 139 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdint.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/stdint.h" 1 3
typedef int32_t int_fast16_t;
typedef int32_t int_fast32_t;
typedef uint32_t uint_fast16_t;
typedef uint32_t uint_fast32_t;
# 139 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdint.h" 2 3
# 29 "./TimeSystem.h" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdbool.h" 1 3
# 30 "./TimeSystem.h" 2
# 43 "./TimeSystem.h"
    typedef struct TIMER {
        timeMillis time;
        _Bool autoReset;
# 55 "./TimeSystem.h"
        _Bool (*isDone)(struct TIMER * this);





        void (*reset)(struct TIMER * this);
        void (*setCallback)(struct TIMER * this, void (*callback)(void));
# 71 "./TimeSystem.h"
        void (*callBack)(void);

        timeMillis start;
        _Bool active;
        _Bool done;
    } Timer;





    typedef struct TIMESYSTEM {
        timeMillis currentTimeMillis;






        void (*updateMillis)(void);





        void (*tasks)(void);






        void (*addTimer)(Timer * timer);





        void (*removeTimer)(Timer * timer);
    } TimeSystem;
# 122 "./TimeSystem.h"
    TimeSystem *GetTimeSystemInstance(void);
# 134 "./TimeSystem.h"
    Timer NewTimer(timeMillis time);
# 1 "TimeSystem.c" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stddef.h" 1 3
# 19 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stddef.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 18 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long int wchar_t;
# 122 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned size_t;
# 132 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef long ptrdiff_t;
# 19 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stddef.h" 2 3
# 2 "TimeSystem.c" 2






static void updateMillis(void);
static void tasks(void);
static void resetTimer(Timer * this);
static void addTimer(Timer * timer);
static void removeTimer(Timer * timer);
static _Bool isDone(Timer * this);
static void setCallback(Timer * this, void (*callback)(void));
static uint32_t * currentTimeMillis;





static Timer * timers[5];

static uint8_t nTimers = 0;
static TimeSystem instance = {
    .currentTimeMillis = 0,
    .updateMillis = updateMillis,
    .tasks = 0,
    .addTimer = addTimer,
    .removeTimer = removeTimer,
};





TimeSystem *GetTimeSystemInstance(void) {
    if (instance.tasks == 0) {
        instance.tasks = tasks;
    }
    return &instance;
}





static void updateMillis(void) {
    instance.currentTimeMillis++;
}

static void tasks(void) {
    for (uint8_t i = 0; i < nTimers; i++) {
        if (!timers[i]->done) {
            if ((instance.currentTimeMillis - timers[i]->start) >= timers[i]->time) {
                timers[i]->done = 1;
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





Timer NewTimer(timeMillis time) {
    Timer tmr = {
        .isDone = isDone,
        .reset = resetTimer,
        .time = time,
        .start = instance.currentTimeMillis,
        .autoReset = 1,
        .done = 0,
        .setCallback = setCallback,
    };
    return tmr;
}

static void resetTimer(Timer * this) {
    this->start = instance.currentTimeMillis;
    this->active = 1;
}

static _Bool isDone(Timer * this) {
    if (this->done) {
        this->active = 0;
        if (this->autoReset)
            resetTimer(this);
        this->done = 0;
        return 1;
    }
    return 0;
}

static void setCallback(Timer * this, void (*callback)(void)) {
    this->callBack = callback;
}
