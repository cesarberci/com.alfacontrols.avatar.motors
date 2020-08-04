#include "Comm.h"
#include <string.h>
/*---------------------------------------------------------------------------*
 Makro definitions
 *---------------------------------------------------------------------------*/
#define rxDone() instance.commDriver->rxDone(instance.commDriver)
#define txDone() instance.commDriver->txDone(instance.commDriver)
#define drvReset() instance.commDriver->reset(instance.commDriver)

/*---------------------------------------------------------------------------*
 Local types
 *---------------------------------------------------------------------------*/
typedef enum {
    COMM_IDLE,
    COMM_START_SEND,
    COMM_WAINTING_FRAME,
    COMM_TRANSMITING_FRAME,
} CommState;
/*---------------------------------------------------------------------------*
 Interface Functions prototypes
 *---------------------------------------------------------------------------*/
static void setRegistersTable(CommRegister * tab, uint8_t length);
static CommRegister * getRegister(uint8_t address);
static void tasks(void);
static void sendCommand(uint8_t dest, uint8_t reg, uint8_t value);
static void setTimeBaseCounter(uint32_t * timeMillis);
/*---------------------------------------------------------------------------*
 Local Functions prototypes
 *---------------------------------------------------------------------------*/
static uint8_t getSUM(uint8_t * buff, uint8_t n);
static void commandHandler(void);
static void transferFrames(void);
static void creatErrorFrame(void);
static void reset(void);
/*---------------------------------------------------------------------------*
 * Local Variables
 *---------------------------------------------------------------------------*/
static RelationalTable table = {
    .setRegistersTable = setRegistersTable,
    .getRegister = getRegister
};
static Communication instance = {
    .tasks = tasks,
    .setTimeBaseCounter = setTimeBaseCounter,
    .regsTable = &table,
    .errorCode = COMM_ERR_OK,
};
static CommState state = 0;
static uint32_t * currentTimeMillis;
static uint32_t timeoutCounter;
static CommFrame inputFrame;
static CommFrame outFrame;
static uint8_t sequence = 16;

/*---------------------------------------------------------------------------*
 * Local Functions implementations
 *---------------------------------------------------------------------------*/
static void commandHandler(void) {
    instance.errorCode = COMM_ERR_OK;
    if (getSUM(inputFrame.data, FRAME_SIZE - 1) != inputFrame.fields.checksum) {
        instance.errorCode |= COMM_ERR_BAD_CHKS;
        //reset driver
        reset();
        return;
    } else if (inputFrame.fields.header.d.sequence == sequence) {
        instance.errorCode |= COMM_ERR_BAD_SEQUENCE;
        outFrame.fields.reg = ERROR_CODE_REG;
        outFrame.fields.value = COMM_ERR_BAD_SEQUENCE;
        sequence = 16;        
    } else {
        sequence = inputFrame.fields.header.d.sequence;
        CommRegister * reg = table.getRegister(inputFrame.fields.reg);
        if (reg != 0) {
            if (inputFrame.fields.header.d.write) {
                if (reg->action)
                    reg->action();
                else
                    *reg->val = inputFrame.fields.value;
            } else {
                if (reg->action == 0) { //do not read commands
                    outFrame.fields.header.d.write = 1;
                    outFrame.fields.value = *reg->val;
                } else
                    instance.errorCode |= COMM_ERR_INVALID_REG;
            }
        } else
            instance.errorCode |= COMM_ERR_INVALID_REG;
    }
    if (instance.errorCode != COMM_ERR_OK)
        creatErrorFrame();
    outFrame.fields.checksum = getSUM(outFrame.data, FRAME_SIZE - 1);
}

static inline int inputR(void) {
    timeoutCounter = (*currentTimeMillis);
    return instance.commDriver->read(instance.commDriver, inputFrame.data, FRAME_SIZE);
}

static inline int outputW(void) {
    timeoutCounter = (*currentTimeMillis);
    return instance.commDriver->write(instance.commDriver, outFrame.data, FRAME_SIZE);
}

static uint8_t getSUM(uint8_t * buff, uint8_t n) {
    uint8_t out = 0;
    for (uint8_t i = 0; i < n; i++)
        out += *buff++;
    return out;
}

static void transferFrames(void) {
    memcpy(outFrame.data, inputFrame.data, FRAME_SIZE);
}

static void creatErrorFrame(void) {
    outFrame.fields.header = inputFrame.fields.header;
    outFrame.fields.reg = ERROR_CODE_REG;
    outFrame.fields.value = instance.errorCode;
}

/*---------------------------------------------------------------------------*
 * Interface Functions implementations
 *---------------------------------------------------------------------------*/
static void setRegistersTable(CommRegister * tab, uint8_t length) {
    CommRegister reg0 = {.action = 0, .val = (uint16_t *) & instance.errorCode, .reg = 100};
    memset(table.regs, 0xFF, (sizeof reg0) * NUMBER_OF_REGISTERS);
    memcpy(table.regs, tab, length);
    table.regs[length / (sizeof reg0)] = reg0;
}

static void tasks(void) {
    switch (state) {
        case COMM_IDLE:
            inputR();
            state = COMM_WAINTING_FRAME;
            break;
        case COMM_START_SEND:
            if (outputW() == 0)
                state = COMM_TRANSMITING_FRAME;
        case COMM_WAINTING_FRAME:
            if (rxDone()) {
                inputR();
                transferFrames();
                if (outputW() == 0)
                    state = COMM_TRANSMITING_FRAME;
                else
                    state = COMM_START_SEND;
                commandHandler();
            } else if (((*currentTimeMillis) - timeoutCounter) > 1000) {
                instance.errorCode |= COMM_ERR_READ_TIMOUT;
                drvReset();
                state = COMM_IDLE;
            }
            break;
        case COMM_TRANSMITING_FRAME:
            if (txDone()) {
                inputR();
                state = COMM_IDLE;
            } else if (((*currentTimeMillis) - timeoutCounter) > 300) {
                instance.errorCode |= COMM_ERR_WRITE_TIMEOUT;
                drvReset();
                state = COMM_IDLE;
            }
            break;
    }
}

static CommRegister * getRegister(uint8_t address) {
    if (address) {
        for (uint8_t i = 0; i < NUMBER_OF_REGISTERS; i++) {
            if (table.regs[i].reg == address)
                return &table.regs[i];
        }
    }
    return 0;
}

static void setTimeBaseCounter(uint32_t * timeMillis) {
    currentTimeMillis = timeMillis;
}

Communication * GetCommunicationInstance(void) {
    return &instance;
}

static void reset(void){
    state = COMM_IDLE;
    drvReset();
}
/* EOF */