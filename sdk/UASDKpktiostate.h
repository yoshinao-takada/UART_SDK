#ifndef UASDKPKTIOSTATE_H_
#define UASDKPKTIOSTATE_H_
#ifdef __cplusplus
extern "C" {
#endif
typedef enum {
    StateID_empty,
    StateID_payload_incomplete,
    StateID_Null
} StateID_t;

typedef enum {
    Substate_nodate,
    Substate_continue
} Substate_t;

typedef struct {
    StateID_t state;
    Substate_t substate;
} State_t;

struct UASDKpktio_state;

typedef State_t (*UASDKpktio_statehandler_t)
    (pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
typedef void (*UASDKpktio_actionhandler_t)
    (pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);

typedef struct UASDKpktio_state {
    UASDKpktio_statehandler_t state_handler;
    UASDKpktio_actionhandler_t enter_handler;
    UASDKpktio_actionhandler_t exit_handler;
} UASDKpktio_state_t, *pUASDKpktio_state_t;

State_t SH_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
State_t SH_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
State_t SH_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);

void Enter_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Enter_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Enter_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);

void Exit_empty(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Exit_payload_incomplete(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);
void Exit_Null(pUASDKpktio_rxbuf_t rxbuf, pUASDKpktio_reader_t reader);

#define STATES_DEF  { \
    { SH_empty, Enter_empty, Exit_empty }, \
    { SH_payload_incomplete, Enter_payload_incomplete, Exit_payload_incomplete }, \
    { SH_Null, Enter_Null, Exit_Null } \
}
#ifdef __cplusplus
}
#endif
#endif /* UASDKPKTIOSTATE_H_ */