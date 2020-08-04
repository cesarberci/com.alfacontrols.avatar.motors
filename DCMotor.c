#include "DCMotor.h"

#define SPEED_DELTA 10
#define MIM_HW_SPEED 500
#define MAX_HW_SPEED 0
#define SPEED_LOWER_LIMIT 200 //20%
#define SPEED_UPPER_LIMIT 950 //95%

static void setDir(DCMotor * this, bool dir);
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

DCMotor NewDCMotor(void (*_setDir)(bool), void (*_setSpeed)(uint16_t)) {
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
        .changed = true,
        .speed = 0,
        .running = true,
        .state = MOTOR_RUNNING,
        .changeDir = changeDir,
    };
    return m;
}

static void setDir(DCMotor * this, bool dir) {
    if (this->dir != dir) {
        this->hwSetSpeed(MIM_HW_SPEED);
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
        this->changed = true;
    }
}

static void setTimeBaseCounter(uint32_t * timeMillis) {
    timeBase = timeMillis;
}

static void stop(DCMotor * this) {
    this->running = false;
}

static void run(DCMotor * this) {
    this->running = true;
}

static void changeDir(DCMotor * this) {
    this->setDir(this, !this->dir);
}

static void accelerate(DCMotor * this) {
    if (this->speed < 1000) {
        this->speed += SPEED_DELTA;
        if (this->speed > 1000)
            this->speed = 1000;
        this->changed = true;
    }
}

static void slowDown(DCMotor * this) {
    if (this->speed > 0) {
        if (this->speed > SPEED_DELTA)
            this->speed -= SPEED_DELTA;
        else
            this->speed = 0;
        this->changed = true;
    }
}

static void tasks(DCMotor * this) {
    switch (this->state) {
        case MOTOR_RUNNING:
            if (!this->running) {
                this->hwSetSpeed(MIM_HW_SPEED);
                this->state = MOTOR_OFF;
            } else if (this->changed) {
                this->changed = false;
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
    if (speed_X10 > SPEED_UPPER_LIMIT)
        return MAX_HW_SPEED;
    else if (speed_X10 < SPEED_LOWER_LIMIT)
        return MIM_HW_SPEED;
    else {
        int32_t sp;
        sp = (MAX_HW_SPEED - MIM_HW_SPEED);
        sp *= (int16_t) speed_X10;
        sp = sp / 1000;
        sp += MIM_HW_SPEED;
        return (uint16_t) sp;
    }

}