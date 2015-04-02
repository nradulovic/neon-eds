#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

typedef struct __attribute__ ((__packed__)) esEvt {
    uint16_t id;
    uint16_t dynamic;
} esEvt_T;

struct evtTable {
    size_t size;
    const char * name;
    const char * type;
    const char * desc;
};

struct epaTable {
    const char * name;
    const char * desc;
};

typedef uint16_t esEvtId_T;

enum evtSignalId {
    ES_EVT_ID_USR = 15
};

#define EXPAND_EVT_AS_ENUM(a, b, c)     a,
#define EXPAND_EVT_AS_TABLE(a, b, c)    [a] = {sizeof(b), #a, #b, c},
#define EXPAND_EVT_AS_TYPE(a, b, c)     typedef b a##_T;

#define ES_DECLARE_EVT_TABLE(table)                                             \
    enum evtId {                                                                \
        ES_EVT_ID_USR_ = ES_EVT_ID_USR - 1U,                                    \
        table(EXPAND_EVT_AS_ENUM)                                               \
        LAST_EVT_ID};                                                           \
    table(EXPAND_EVT_AS_TYPE)                                                   \
    extern const struct evtTable gEvtTable_[LAST_EVT_ID]                     
    
#define ES_INITIALIZE_EVT_TABLE(table)                                          \
    const struct evtTable gEvtTable_[LAST_EVT_ID] = {                           \
        table(EXPAND_EVT_AS_TABLE)                                              \
    }              
/*----------------------------------------------------------------------------*/
/*
    Format stampanja podataka o radu automata

        Action      Source      Destination     Answer
                                Event         
type    const       arg         arg             arg

Primer: 
        CALL        S3          S               SUPER
        CALL        S2          S               SUPER
        CALL        S1          S               TRAN
        TRAN        S1          P0              0
        CALL        S3          EXIT            SUPER
        CALL        S2          EXIT            HANDLED
        CALL        S1          EXIT            SUPER
        CALL        P0          ENTRY           HANDLED

    - S2 je superstanje S3, S1 je super stanje S2, S1 i P0 su susedna stanja.
    - stanje S1 je osetljivo na dogadjaj S
    - izvrsava se tranzicija S1 -> P0
*/
/*
    Format komunikacije između automata

        Action      Source      Destination     Answer
                                Event         
type    const       arg         arg             arg

Primer: 
        SEND        A1          A2              S
        RECV        A2          A1              S

    - Automat A1 salje automatu A2 događaj S
    - Automat A2 prima od automata A1 događaj S   
*/
/*
    Format prekidanja rada

        Action      Source      Destination     Answer
                                Event         
type    const       arg         arg             arg

Primer: 
        SWITCH      A1          A2

    - Automat A1 se suspenduje, izvrsava se A2

*/
/*
        SEND        A1          A2              S
        SWITCH      A1          A2
        RECV        A2          A1              S
        CALL        S3          S               SUPER
        CALL        S2          S               SUPER
        CALL        S1          S               TRAN
        TRAN        S1          P0              0
        CALL        S3          EXIT            SUPER
        CALL        S2          EXIT            HANDLED
        CALL        S1          EXIT            SUPER
        CALL        P0          ENTRY           HANDLED
        SWITCH      A2          A1
        :
        :
        :
        SWITCH      A1          A0
        :
        :
        :
*/
/*----------------------------------------------------------------------------*/

/*--- EVT Types --------------------------------------------------------------*/

typedef struct basicEvt {
    esEvt_T super;
} basicEvt_T;


typedef struct __attribute__ ((__packed__)) menuEvt  {
    esEvt_T super;
    uint8_t object;
    uint8_t state;
    uint8_t id;
} menuEvt_T;

typedef struct msgPacket {
    esEvt_T super;
    uint8_t id;
    uint8_t * from;
    uint8_t * to;
    void (* callback)(uint8_t);
} msgPacket_T;

/*--- EVT Table: name, type, description -------------------------------------*/
#define EVENTS(entry)                                                           \
    entry(SIG_SWITCH,   basicEvt_T,     "Signal to switch LEDs")                \
    entry(COUNT_TICK,   basicEvt_T,     "Tick event")                           \
    entry(MENU_CONN,    menuEvt_T,      "A user has pressed a button in MENU system")\
    entry(MSG_RECV,     msgPacket_T,    "A message has been received")          \
    entry(MSG_TX,       msgPacket_T,    "A message will be transmitted")
    
/*--- State table: name, supersteate, description ----------------------------*/
#define STATES(entry)                                                           \
    entry(INIT,         TOP,            "Initialize SM")                        \
    entry(START,        TOP,            "Start the device")                     \
    entry(LED_ON,       START,          "Status LED on")                        \
    entry(LED_OFF,      START,          "Status LED off")

/*--- EPA TAble: name, prio, sm levels, queue levels, description ------------*/
/** sizeof(workspace_T) se predaje esEpaCreate funkciji */
#define EPA(entry)                                                              \
    entry(BLINKY,       1U,     10U,    "Blinks a diode")                       \
    entry(MENU,         2U      10U,    "Menu system")

#define EXPAND_STATE_AS_ENUM(a, b, c)   a,

#define EXPAND_STATE_AS_PROTOTYPE(a,b,c)                                        \
    esStatus_T state##a (void * sm, esEvt_T * evt);


typedef esStatus_T (* state_T) (void * sm, void * evt);
struct stateElem {
    state_T     state;
    state_T     superState;
    const char * desc;
}

#define EXPAND_STATE_AS_TABLE(a, b, c)                                          \
    [a] = {state##a, state##b, c},

enum stateId {
    STATES(EXPAND_STATE_AS_ENUM)
    STATE_NUM
};

STATES(EXPAND_STATE_AS_PROTOTYPE)

struct stateElem stateTable[STATE_NUM] = {
    STATES(EXPAND_STATE_AS_TABLE)
};

esStatus INIT (void * sm, esEvt_T * evt) {
    switch (evt->id) {
        case SIG_ENTRY : {
            :
            :
            :
            return (RETN_HANDLED());
        }

        case SIG_INIT : {
            :
            :
            :
            return (RETN_TRAN(START));
        }
    }
}

esStatus stateSTART (void * sm, esEvt_T * evt) {
    switch (evt->id) {
        case SIG_ENTRY : {
            :
            :
            :
            return (RETN_HANDLED());
        }

        case SIG_INIT : {
            :
            :
            :
            return (RETN_TRAN(START));
        }
    }
}

esStatus LED_ON (void * sm, esEvt_T * evt) {
    switch (evt->id) {
        case SIG_ENTRY : {
            :
            :
            :
            return (RETN_HANDLED());
        }

        case SIG_INIT : {
            :
            :
            :
            return (RETN_TRAN(LED_OFF));
        }
    }
}

/*--- EPA Table: name, id, desciption                                         */
#define EPA(entry)                                                              \
    entry(BLINKY,       gEpaBlinky,     "Simple blink finite state machine")                    

enum evtId {

    EVENTS(EXPAND_EVT_AS_ENUM)
    LAST_EVT_ID
};

EVENTS(EXPAND_EVT_AS_TYPE)

static const struct evtTable gEvtTable_[LAST_EVT_ID] = {
    EVENTS(EXPAND_EVT_AS_TABLE)
};

void printEvtContent(esEvt_T * evt);
void printEvtTable(const struct evtTable * table);
void printEpaTable(const struct epaTable * table);
    
int main (void) {
    msgPacket_T msg;
    menuEvt_T menu;
    esEvt_T * evt;
    
    printEvtTable(&gEvtTable_[0]);
    /* printEpaTable(&gEpaTable_[0]); */
    
    printf(
        "\nEvent Table usage: %d event records\n size per record [B]: %d\n total table size[B]: %d\n",
        LAST_EVT_ID,
        (int)sizeof(gEvtTable_) / LAST_EVT_ID,
        (int)sizeof(gEvtTable_));
    printf("\n");

    evt = (esEvt_T *)&msg;
    evt->id = MSG_TX;
    evt->dynamic = 0U;
    ((MSG_TX_T *)evt)->id = 10;
    ((MSG_TX_T *)evt)->from = NULL;
    ((MSG_TX_T *)evt)->to   = NULL;
    ((MSG_TX_T *)evt)->callback = NULL;
    
    printEvtContent(evt);        
    
    evt = (esEvt_T *)&menu;
    evt->id = MENU_CONN;
    evt->dynamic = 0U;
    ((MENU_CONN_T *)evt)->object= 6;
    ((MENU_CONN_T *)evt)->state = 2;
    ((MENU_CONN_T *)evt)->id    = 1;

    printEvtContent(evt);        
    
    return 0;
}

void printEvtContent(esEvt_T * evt) {
    size_t evtDataSize;
    
    evtDataSize = (size_t)(gEvtTable_[evt->id].size - sizeof(esEvt_T));
    printf(
        "Event header information:\n id  : %d\n size: %d\n name: %s\n type: %s\n desc: %s\n", 
        evt->id, 
        (int)gEvtTable_[evt->id].size, 
        gEvtTable_[evt->id].name,
        gEvtTable_[evt->id].type,
        gEvtTable_[evt->id].desc);
        
    printf(
        "Event data information:\n data size: %d\n", 
        (int)evtDataSize);
        
    if (0UL != evtDataSize) {
        uint32_t cntr;
        uint8_t * data;
        
        data = (uint8_t *)&(evt->dynamic);
        data++;
        printf(" pos : ");
        for (cntr = 0UL; cntr < evtDataSize; cntr++) {
            printf("%3x", cntr);
        }
        printf("\n data: ");
        for (cntr = 0UL; cntr < evtDataSize; cntr++) {
            data++;
            printf("%3x", *data);
        }
    } else {
        printf(" no data");
    }
    printf("\n");
}

void printEvtTable(const struct evtTable * table) {
    uint32_t cntr;
    
    for (cntr = ES_EVT_ID_USR; cntr < LAST_EVT_ID; cntr++) {
        printf(
            "\nEvent:\n id  : %d\n size: %d\n name: %s\n type: %s\n desc: %s\n data size: %d\n", 
            cntr, 
            (int)table[cntr].size, 
            table[cntr].name,
            table[cntr].type,
            table[cntr].desc,
            (int)(table[cntr].size - sizeof(esEvt_T)));
     }
}

void printEpaTable(const struct epaTable * table) {
   (void)table;
}

