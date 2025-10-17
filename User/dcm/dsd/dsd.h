#ifndef _DSD_H_
#define _DSD_H_

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define SIDNR												0x7F
/*Negative response codes of UDS--------------------------------------------*/
#define NRC_SERVICE_NOT_SUPPORTED								0x11
#define NRC_SERVICE_NOT_SUPPORTED_IN_ACTIVE_SESSION				0x7F
#define NRC_SECURITY_ACCESS_DENIED								0x33
#define NRC_RESPONSE_PENDING									0x78
#define NRC_BUSY_REPEAT_REQUEST									0x21
#define NRC_INCORRECT_MSG_LEN_OR_INVALID_FORMAT					0x13
#define NRC_CONDITION_NOT_CORRECT								0x22
#define NRC_SUB_FUNCTION_NOT_SUPPORTED							0x12
#define NRC_REQUEST_OUT_OF_RANGE								0x31
#define NRC_REQUEST_SEQUENCE_ERROR								0x24
#define NRC_INVALID_KEY											0x35
#define NRC_GENERALPROGRAMMINGFAILURE							0x72
#define NRC_WRONGBLOCKSEQUENCECOUNTER							0x73
#define NRC_EXCEED_NUMBER_OF_ATTEMPTS							0x36
#define NRC_REQUIRED_TIME_DELAY_NOT_EXPIRED						0x37
#define NRC_GENERAL_PROGRAMMING_FAILURE							0x72
#define NRC_SUBFUN_NOT_SUPPORTED_IN_ACTIVESESSION               0x7E
#define NRC_VOLTAGETOOHIGH                                      0x92
#define NRC_VOLTAGETOOLOW                                       0x93
#define NRC_GENERAL_REJECT                                      0x10
#define NRC_UPLOADDOWNLOADNOTACCEPTED                           0x70
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* VARIABLE DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* FUNCTION DECLARE                                                         */
/*                                                                          */
/*--------------------------------------------------------------------------*/
extern void dsd_init(void);

extern Std_ReturnType dsd_query_addressing_supportness(uint8 uSupposeSid, PROTOCAL_FLAG_TYPE IsPhyAddr);

extern void dsd_data_indication_functionality(LP_MSG_CONTEXT pMsgContext);

extern void dsd_set_negative_response(LP_MSG_CONTEXT pMsgContext, uint8 uNRC);

extern void dsd_process_done(void);

extern void dsd_process_cancel_reponse(void);

extern void dsd_process_pending(void);

extern void dsd_resp_edit_lock(void);

extern void dsd_resp_edit_unlock(void);

extern void dsd_final(void);

#endif /* _DSD_H_ */

