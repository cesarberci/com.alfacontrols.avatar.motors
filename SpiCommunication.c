#include "SpiCommunication.h"
#include "enhaced_spi.h"

/*---------------------------------------------------------------------------*
 Interface functions prototypes
 *---------------------------------------------------------------------------*/
static int read(Spi * this, uint8_t * data, uint8_t length);
static int write(Spi * this, uint8_t * data, uint8_t length);
static void tasks(Spi * this);
static void open(Spi * this);
static void close(Spi * this);
static void exchangeDone(Spi * this);
static void setTimeBaseCounter(uint32_t * timeMillis);
static void reset(Spi * this);
bool txDone(Spi * this);
bool rxDone(Spi * this);
static bool healthy(Spi * this);
/*---------------------------------------------------------------------------*
 Local Functions prototypes
 *---------------------------------------------------------------------------*/
static void __run(void (*function)(void));
/*---------------------------------------------------------------------------*
 * Local Variables
 *---------------------------------------------------------------------------*/
static uint32_t * currentTimeMillis;

/*---------------------------------------------------------------------------*
 * Local Functions implementations
 *---------------------------------------------------------------------------*/
static void __run(void (*function)(void)) {
    if (function > 0)
        function();
}

/*---------------------------------------------------------------------------*
 * Interface Functions implementations
 *---------------------------------------------------------------------------*/
Spi NewSpiDriver(bool(*op)(void), void (*cl)(void), uint8_t(*rd)(void), void (*wr1)(uint8_t), void (*wr2)(uint8_t), void (*rst)(void), void (*ie)(bool st)) {
    Spi spi = {
        .readByte = rd,
        .writeByte_1 = wr1,
        .writeByte_2 = wr2,
        .read = read,
        .write = write,
        .hwopen = op,
        .hwreset = rst,
        .open = open,
        .close = close,
        .hwclose = cl,
        .exchangeDone = exchangeDone,
        .tasks = tasks,
        .state.val = 0,
        .txDone = txDone,
        .rxDone = rxDone,
        .setTimeBaseCounter = setTimeBaseCounter,
        .reset = reset,
        .hwInterrupEnable = ie,
        .healthy = healthy,
    };
    spi.timer = *currentTimeMillis;
    return spi;
}

static int read(Spi * this, uint8_t * data, uint8_t length) {
    if (this->state.b.open) {
        if (this->charsToReceive == 0) {
            this->timer = *currentTimeMillis;
            this->charsToReceive = length;
            this->dataToRead = data;
            return 0;
        } else
            return -2;
    } else
        return -1;
}

static int write(Spi * this, uint8_t * data, uint8_t length) {
    if (this->state.b.open) {
        if (this->charsToSend == 0) {
            this->timer = *currentTimeMillis;
            this->charsToSend = length - 1;
            this->dataToWrite = data;
            this->writeByte_2(*this->dataToWrite++);
            return 0;
        } else
            return -2;
    } else
        return -1;
}

static void exchangeDone(Spi * this) {
    if (this->charsToReceive > 0) {
        *this->dataToRead++ = this->readByte();
        this->charsToReceive--;
        if (this->charsToReceive == 0) {
            this->state.b.receiveComplete = 1;
            this->newData = true;
        }
    } else
        this->rChar = this->readByte();
    if (this->charsToSend > 0) {
        this->writeByte_1(*this->dataToWrite++);
        this->charsToSend--;
        if (this->charsToSend == 0)
            this->state.b.sendComplete = 1;
    } else {
        this->writeByte_1(0xFF);
    }
}

static void open(Spi * this) {
    this->state.b.open = this->hwopen();
}

static void close(Spi * this) {
    this->hwclose();
    this->state.b.open = 0;
}

bool txDone(Spi * this) {
    return this->charsToSend == 0;
}

bool rxDone(Spi * this) {
    if (this->newData) {
        this->newData = false;
        return true;
    }
    return false;
}

static bool healthy(Spi * this) {
    if (this->state.b.open) {
        if ((((*currentTimeMillis) - this->timer) < 4000))
            return true;
    }
    return false;
}

static void setTimeBaseCounter(uint32_t * timeMillis) {
    currentTimeMillis = timeMillis;
}

static void reset(Spi * this) {
    this->hwInterrupEnable(false);
    this->hwreset();
    this->state.val = 0x0F;
    this->writeByte_2(0);
    this->charsToReceive = 0;
    this->charsToSend = 0;
    this->hwInterrupEnable(true);
}

static void tasks(Spi * this) {
    if (this->state.b.open) {
        if (this->state.b.receiveComplete)
            __run(this->readCallBack);
        if (this->state.b.sendComplete)
            __run(this->writeCallBack);
        if (this->state.b.byteRead) {
            __run(this->byteReadCallBack);
        }
        if (this->charsToReceive + this->charsToSend > 0) {
            if (((*currentTimeMillis) - this->timer) > 5000) {
                __run(this->errorCallBack);
                this->reset(this);
            }
        }
        this->state.val &= 0x0F;
    }
}
/* EOF */