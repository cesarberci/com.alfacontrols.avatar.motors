# 1 "DCMotor.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "DCMotor.c" 2
# 1 "./DCMotor.h" 1
# 20 "./DCMotor.h"
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdbool.h" 1 3
# 20 "./DCMotor.h" 2

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
# 21 "./DCMotor.h" 2






    typedef enum MOTORSTATE {
        MOTOR_RUNNING,
        MOTOR_OFF,
        MOTOR_WAITING_OFF,
        MOTOR_WAITING_CHANGE_DIR,
    } MotorState;
# 47 "./DCMotor.h"
    typedef struct DCMOTOR {
# 56 "./DCMotor.h"
        void (*setDir)(struct DCMOTOR * this, _Bool dir);
# 65 "./DCMotor.h"
        void (*setSpeed)(struct DCMOTOR * this, uint16_t speed_X10);
# 75 "./DCMotor.h"
        void (*setTimeBaseCounter)(uint32_t * timeMillis);
# 84 "./DCMotor.h"
        void (*stop)(struct DCMOTOR * this);
# 93 "./DCMotor.h"
        void (*run)(struct DCMOTOR * this);






        void (*accelerate)(struct DCMOTOR * this);






        void (*slowDown)(struct DCMOTOR * this);






        void (*changeDir)(struct DCMOTOR * this);







        void (*tasks)(struct DCMOTOR * this);
        void (*hwSetDir)(_Bool dir);
        void (*hwSetSpeed)(uint16_t value);

        _Bool dir;
        _Bool running;
        MotorState state;
        uint16_t speed;
        uint32_t timer;
        _Bool changed;
    } DCMotor;
# 145 "./DCMotor.h"
    DCMotor NewDCMotor(void (*_setDir)(_Bool), void (*_setSpeed)(uint16_t));
# 1 "DCMotor.c" 2








static void setDir(DCMotor * this, _Bool dir);
static void setSpeed(DCMotor * this, uint16_t speed_X10);
static void setTimeBaseCounter(uint32_t * timeMillis);
static void stop(DCMotor * this);
static void run(DCMotor * this);
static void changeDir(DCMotor * this);
static void accelerate(DCMotor * this);
static void slowDown(DCMotor * this);
static void tasks(DCMotor * this);
static uint16_t getHWSpeed(uint16_t speed_X10);

static uint32_t * timeBase;

DCMotor NewDCMotor(void (*_setDir)(_Bool), void (*_setSpeed)(uint16_t)) {
    DCMotor m = {
        .hwSetDir = _setDir,
        .hwSetSpeed = _setSpeed,
        .setDir = setDir,
        .setSpeed = setSpeed,
        .tasks = tasks,
        .setTimeBaseCounter = setTimeBaseCounter,
        .accelerate = accelerate,
        .slowDown = slowDown,
        .stop = stop,
        .run = run,
        .changed = 1,
        .speed = 0,
        .running = 1,
        .state = MOTOR_RUNNING,
        .changeDir = changeDir,
    };
    return m;
}

static void setDir(DCMotor * this, _Bool dir) {
    if (this->dir != dir) {
        this->hwSetSpeed(500);
        this->state = MOTOR_WAITING_OFF;
        this->dir = dir;
        this->timer = *timeBase;
    }

}

static void setSpeed(DCMotor * this, uint16_t speed_X10) {
    if (this->speed != speed_X10) {
        if (speed_X10 > 1000)
            this->speed = 1000;
        else
            this->speed = speed_X10;
        this->changed = 1;
    }
}

static void setTimeBaseCounter(uint32_t * timeMillis) {
    timeBase = timeMillis;
}

static void stop(DCMotor * this) {
    this->running = 0;
}

static void run(DCMotor * this) {
    this->running = 1;
}

static void changeDir(DCMotor * this) {
    this->setDir(this, !this->dir);
}

static void accelerate(DCMotor * this) {
    if (this->speed < 1000) {
        this->speed += 10;
        if (this->speed > 1000)
            this->speed = 1000;
        this->changed = 1;
    }
}

static void slowDown(DCMotor * this) {
    if (this->speed > 0) {
        if (this->speed > 10)
            this->speed -= 10;
        else
            this->speed = 0;
        this->changed = 1;
    }
}

static void tasks(DCMotor * this) {
    switch (this->state) {
        case MOTOR_RUNNING:
            if (!this->running) {
                this->hwSetSpeed(500);
                this->state = MOTOR_OFF;
            } else if (this->changed) {
                this->changed = 0;
                this->hwSetSpeed(getHWSpeed(this->speed));
            }
            break;
        case MOTOR_OFF:
            if (this->running) {
                this->hwSetSpeed(getHWSpeed(this->speed));
                this->state = MOTOR_RUNNING;
            }
            break;
        case MOTOR_WAITING_OFF:
            if ((*timeBase - this->timer) > 2) {
                this->hwSetDir(this->dir);
                this->timer = *timeBase;
                this->state = MOTOR_WAITING_CHANGE_DIR;
            }
            break;
        case MOTOR_WAITING_CHANGE_DIR:
            if ((*timeBase - this->timer) > 2)
                this->state = MOTOR_OFF;
            break;
    }
}

static uint16_t getHWSpeed(uint16_t speed_X10) {
    if (speed_X10 > 950)
        return 0;
    else if (speed_X10 < 200)
        return 500;
    else {
        int32_t sp;
        sp = (0 - 500);
        sp *= (int16_t) speed_X10;
        sp = sp / 1000;
        sp += 500;
        return (uint16_t) sp;
    }

}
