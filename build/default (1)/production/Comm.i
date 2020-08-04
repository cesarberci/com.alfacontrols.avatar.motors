# 1 "Comm.c"
# 1 "<built-in>" 1
# 1 "<built-in>" 3
# 288 "<built-in>" 3
# 1 "<command line>" 1
# 1 "<built-in>" 2
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\language_support.h" 1 3
# 2 "<built-in>" 2
# 1 "Comm.c" 2
# 1 "./Comm.h" 1
# 59 "./Comm.h"
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
# 59 "./Comm.h" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\stdbool.h" 1 3
# 60 "./Comm.h" 2


# 1 "./SpiCommunication.h" 1
# 58 "./SpiCommunication.h"
    typedef union {
        uint8_t val;

        struct {
            uint8_t open : 1;
            uint8_t reserved_6 : 1;
            uint8_t reserved_5 : 1;
            uint8_t reserved_4 : 1;
            uint8_t sendComplete : 1;
            uint8_t receiveComplete : 1;
            uint8_t byteRead : 1;
            uint8_t reserved_0 : 1;
        } b;
    } SpiState;
# 91 "./SpiCommunication.h"
    typedef struct SPI {
# 105 "./SpiCommunication.h"
        int (*read)(struct SPI * this, uint8_t * data, uint8_t length);
# 118 "./SpiCommunication.h"
        int (*write)(struct SPI * this, uint8_t * data, uint8_t length);






        void (*open)(struct SPI * this);






        void (*close)(struct SPI * this);






        void (*tasks)(struct SPI * this);
# 148 "./SpiCommunication.h"
        _Bool (*txDone)(struct SPI * this);
# 157 "./SpiCommunication.h"
        _Bool (*rxDone)(struct SPI * this);
# 167 "./SpiCommunication.h"
        void (*setTimeBaseCounter)(uint32_t * timeMillis);
# 178 "./SpiCommunication.h"
        void (*reset)(struct SPI * this);
# 192 "./SpiCommunication.h"
        void (*exchangeDone)(struct SPI * this);
# 202 "./SpiCommunication.h"
        void (*readCallBack)(void);
# 212 "./SpiCommunication.h"
        void (*writeCallBack)(void);
# 223 "./SpiCommunication.h"
        void (*byteReadCallBack)(void);
# 233 "./SpiCommunication.h"
        void (*errorCallBack)(void);
# 245 "./SpiCommunication.h"
        _Bool (*healthy)(struct SPI * this);
        uint8_t rChar;

        _Bool (*hwopen)(void);
        void (*hwclose)(void);
        void (*hwreset)(void);
        uint8_t(*readByte)(void);
        void (*writeByte_1)(uint8_t d);
        void (*writeByte_2)(uint8_t d);
        void (*hwInterrupEnable)(_Bool ie);
        int charsToReceive;
        int charsToSend;
        SpiState state;
        uint32_t timer;
        uint8_t * dataToRead;
        uint8_t * dataToWrite;
        _Bool newData;
    } Spi;
# 279 "./SpiCommunication.h"
    Spi NewSpiDriver(_Bool (*op)(void), void (*cl)(void), uint8_t (*rd)(void), void (*wr1)(uint8_t), void (*wr2)(uint8_t), void (*rst)(void), void (*ie)(_Bool st));
# 62 "./Comm.h" 2
# 72 "./Comm.h"
    typedef enum COMM_ERROR {
        COMM_ERR_OK = 0,
        COMM_ERR_BAD_CHKS = 1,
        COMM_ERR_BAD_SEQUENCE = 2,
        COMM_ERR_INVALID_REG = 4,
        COMM_ERR_READ_TIMOUT = 8,
        COMM_ERR_WRITE_TIMEOUT = 16,
        COMM_ERR_last = 0xFFFF,
    } CommError;
# 90 "./Comm.h"
    typedef union FRAME_HEADER {
        uint8_t val;
        struct {
            uint8_t write : 1;
            uint8_t sequence : 4;
            uint8_t reserved : 3;
        } d;
    } CommFrameHeader;
# 108 "./Comm.h"
    typedef struct COMM_REG {
        void (*action)(void);
        uint16_t * val;
        uint8_t reg;
    } CommRegister;







    typedef union COMM_FRAME {
        uint8_t data[5];
        struct {
            CommFrameHeader header;
            uint8_t reg;
            uint16_t value;
            uint8_t checksum;
        } fields;
    } CommFrame;







    typedef struct COMM_TABLE {
# 155 "./Comm.h"
        void (*setRegistersTable)(CommRegister * tab, uint8_t length);
        CommRegister regs[10];





        CommRegister * (*getRegister)(uint8_t address);
    } RelationalTable;






    typedef struct COMMUNICATION {






        void (*tasks)(void);
# 187 "./Comm.h"
        void (*setTimeBaseCounter)(uint32_t * timeMillis);
        Spi * commDriver;

        RelationalTable * regsTable;
        CommError errorCode;
    } Communication;







    Communication * GetCommunicationInstance(void);
# 1 "Comm.c" 2

# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\string.h" 1 3
# 10 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\string.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\features.h" 1 3
# 10 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\string.h" 2 3
# 25 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\string.h" 3
# 1 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 1 3
# 122 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef unsigned size_t;
# 411 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\bits/alltypes.h" 3
typedef struct __locale_struct * locale_t;
# 25 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\string.h" 2 3


void *memcpy (void *restrict, const void *restrict, size_t);
void *memmove (void *, const void *, size_t);
void *memset (void *, int, size_t);
int memcmp (const void *, const void *, size_t);
void *memchr (const void *, int, size_t);

char *strcpy (char *restrict, const char *restrict);
char *strncpy (char *restrict, const char *restrict, size_t);

char *strcat (char *restrict, const char *restrict);
char *strncat (char *restrict, const char *restrict, size_t);

int strcmp (const char *, const char *);
int strncmp (const char *, const char *, size_t);

int strcoll (const char *, const char *);
size_t strxfrm (char *restrict, const char *restrict, size_t);

char *strchr (const char *, int);
char *strrchr (const char *, int);

size_t strcspn (const char *, const char *);
size_t strspn (const char *, const char *);
char *strpbrk (const char *, const char *);
char *strstr (const char *, const char *);
char *strtok (char *restrict, const char *restrict);

size_t strlen (const char *);

char *strerror (int);
# 65 "C:\\Program Files\\Microchip\\xc8\\v2.20\\pic\\include\\c99\\string.h" 3
char *strtok_r (char *restrict, const char *restrict, char **restrict);
int strerror_r (int, char *, size_t);
char *stpcpy(char *restrict, const char *restrict);
char *stpncpy(char *restrict, const char *restrict, size_t);
size_t strnlen (const char *, size_t);
char *strdup (const char *);
char *strndup (const char *, size_t);
char *strsignal(int);
char *strerror_l (int, locale_t);
int strcoll_l (const char *, const char *, locale_t);
size_t strxfrm_l (char *restrict, const char *restrict, size_t, locale_t);




void *memccpy (void *restrict, const void *restrict, int, size_t);
# 2 "Comm.c" 2
# 13 "Comm.c"
typedef enum {
    COMM_IDLE,
    COMM_START_SEND,
    COMM_WAINTING_FRAME,
    COMM_TRANSMITING_FRAME,
} CommState;



static void setRegistersTable(CommRegister * tab, uint8_t length);
static CommRegister * getRegister(uint8_t address);
static void tasks(void);
static void sendCommand(uint8_t dest, uint8_t reg, uint8_t value);
static void setTimeBaseCounter(uint32_t * timeMillis);



static uint8_t getSUM(uint8_t * buff, uint8_t n);
static void commandHandler(void);
static void transferFrames(void);
static void creatErrorFrame(void);
static void reset(void);



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




static void commandHandler(void) {
    instance.errorCode = COMM_ERR_OK;
    if (getSUM(inputFrame.data, 5 - 1) != inputFrame.fields.checksum) {
        instance.errorCode |= COMM_ERR_BAD_CHKS;

        reset();
        return;
    } else if (inputFrame.fields.header.d.sequence == sequence) {
        instance.errorCode |= COMM_ERR_BAD_SEQUENCE;
        outFrame.fields.reg = 100;
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
                if (reg->action == 0) {
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
    outFrame.fields.checksum = getSUM(outFrame.data, 5 - 1);
}

static __attribute__((inline)) int inputR(void) {
    timeoutCounter = (*currentTimeMillis);
    return instance.commDriver->read(instance.commDriver, inputFrame.data, 5);
}

static __attribute__((inline)) int outputW(void) {
    timeoutCounter = (*currentTimeMillis);
    return instance.commDriver->write(instance.commDriver, outFrame.data, 5);
}

static uint8_t getSUM(uint8_t * buff, uint8_t n) {
    uint8_t out = 0;
    for (uint8_t i = 0; i < n; i++)
        out += *buff++;
    return out;
}

static void transferFrames(void) {
    memcpy(outFrame.data, inputFrame.data, 5);
}

static void creatErrorFrame(void) {
    outFrame.fields.header = inputFrame.fields.header;
    outFrame.fields.reg = 100;
    outFrame.fields.value = instance.errorCode;
}




static void setRegistersTable(CommRegister * tab, uint8_t length) {
    CommRegister reg0 = {.action = 0, .val = (uint16_t *) & instance.errorCode, .reg = 100};
    memset(table.regs, 0xFF, (sizeof reg0) * 10);
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
            if (instance.commDriver->rxDone(instance.commDriver)) {
                inputR();
                transferFrames();
                if (outputW() == 0)
                    state = COMM_TRANSMITING_FRAME;
                else
                    state = COMM_START_SEND;
                commandHandler();
            } else if (((*currentTimeMillis) - timeoutCounter) > 1000) {
                instance.errorCode |= COMM_ERR_READ_TIMOUT;
                instance.commDriver->reset(instance.commDriver);
                state = COMM_IDLE;
            }
            break;
        case COMM_TRANSMITING_FRAME:
            if (instance.commDriver->txDone(instance.commDriver)) {
                inputR();
                state = COMM_IDLE;
            } else if (((*currentTimeMillis) - timeoutCounter) > 300) {
                instance.errorCode |= COMM_ERR_WRITE_TIMEOUT;
                instance.commDriver->reset(instance.commDriver);
                state = COMM_IDLE;
            }
            break;
    }
}

static CommRegister * getRegister(uint8_t address) {
    if (address) {
        for (uint8_t i = 0; i < 10; i++) {
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
    instance.commDriver->reset(instance.commDriver);
}
