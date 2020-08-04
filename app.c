#include "app.h"
#include "mcc_generated_files/tmr4.h"
#include "mcc_generated_files/epwm1.h"
#include "mcc_generated_files/spi.h"
#include "mcc_generated_files/pwm3.h"
#include "mcc_generated_files/pin_manager.h"
#include "TimeSystem.h"
#include "DCMotor.h"
#include "SpiCommunication.h"
#include "SpiCommunication.h"
#include "Comm.h"
#include "enhaced_spi.h"

#include <stdint.h>

typedef enum state {
    APP_NOT_STARTED,
    APP_RUNNING

} AppState;

static AppState state = APP_NOT_STARTED;
static TimeSystem * time;
static Timer tmr1, tmr2, tmr3;
static DCMotor motor1, motor2;
static uint16_t speedm1, speedm2;
static uint16_t _speedm1, _speedm2;
static Spi spi;
static Communication * comm;

static void wdt(void) {
    if (spi.healthy(&spi))
        asm(" clrwdt");
}

static void wdtReset(void) {
    while (1);
}

static void dataReceived(void) {
    spi.exchangeDone(&spi);
}

static void motor1SetDir(bool dir) {
    if (dir)
        M1D_SetHigh();
    else
        M1D_SetLow();
}

static void motor2SetDir(bool dir) {
    if (dir)
        M2D_SetHigh();
    else
        M2D_SetLow();
}

static void setMotor1Clockwise(void) {
    motor1.setDir(&motor1, true);
}

static void setMotor1Counterclockwise(void) {
    motor1.setDir(&motor1, false);
}

static void setMotor2Clockwise(void) {
    motor2.setDir(&motor2, true);
}

static void setMotor2Counterclockwise(void) {
    motor2.setDir(&motor2, false);
}

static void runMotors(void) {
    motor1.run(&motor1);
    motor2.run(&motor2);
}

static void stopMotors(void) {
    motor1.stop(&motor1);
    motor2.stop(&motor2);
}

void appTasks(void) {
    switch (state) {
        case APP_NOT_STARTED:
            time = GetTimeSystemInstance();
            TMR4_SetInterruptHandler(time->updateMillis);
            motor1 = NewDCMotor(motor1SetDir, EPWM1_LoadDutyValue);
            motor2 = NewDCMotor(motor2SetDir, PWM3_LoadDutyValue);
            motor1.setTimeBaseCounter((uint32_t *) & time->currentTimeMillis);
            spi = NewSpiDriver(espi_op, espi_cl, espi_rd, espi_wr1, espi_wr2, espi_rst, espi_int);
            espi_setInterrupHandler(dataReceived);
            spi.setTimeBaseCounter((uint32_t *) & time->currentTimeMillis);
            spi.open(&spi);
            comm = GetCommunicationInstance();
            CommRegister regs[] = {
                { .action = 0, .val = &speedm1, .reg = 10},
                { .action = 0, .val = &speedm2, .reg = 11},
                { .action = setMotor1Clockwise, .val = 0, .reg = 50},
                { .action = setMotor1Counterclockwise, .val = 0, .reg = 51},
                { .action = setMotor2Clockwise, .val = 0, .reg = 52},
                { .action = setMotor2Counterclockwise, .val = 0, .reg = 53},
                { .action = runMotors, .val = 0, .reg = 55},
                { .action = stopMotors, .val = 0, .reg = 54},
                { .action = wdtReset, .val = 0, .reg = 1},
            };
            comm->regsTable->setRegistersTable(regs, sizeof regs);
            comm->commDriver = &spi;
            comm->setTimeBaseCounter((uint32_t *) & time->currentTimeMillis);
            motor1.setSpeed(&motor1, speedm1);
            motor2.setSpeed(&motor2, speedm2);
            runMotors();
            state = APP_RUNNING;
            break;
        case APP_RUNNING:
            //motor1.setSpeed(&motor1, speedm1);
            //motor2.setSpeed(&motor2, speedm2);
            time->tasks();
            spi.tasks(&spi);
            comm->tasks();
            motor1.tasks(&motor1);
            motor2.tasks(&motor2);
            if (speedm1 != _speedm1) {
                _speedm1 = speedm1;
                motor1.setSpeed(&motor1, speedm1);
            }
            if (speedm2 != _speedm2) {
                _speedm2 = speedm2;
                motor2.setSpeed(&motor2, speedm2);
            }
            break;
    }
    wdt();
}

