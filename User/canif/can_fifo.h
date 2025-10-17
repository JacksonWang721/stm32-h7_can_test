#ifndef _CANFD_FIFO_H_
#define _CANFD_FIFO_H_

#ifdef  __cplusplus
extern "C"
{
#endif  /* defined(__cplusplus) */

#include "std_type.h"
/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
typedef uint32                             FRAME_ID_TYPE;
typedef uint8                              FRAME_DATA_TYPE;
typedef uint8                              FRAME_LEN_TYPE;
typedef uint8                              RX_DATA_FIFO_PARA_TYPE;

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
extern void canfd_rx_data_fifo_init(void);

extern Std_ReturnType canfd_rx_data_fifo_insert_tail(FRAME_ID_TYPE Id, FRAME_DATA_TYPE const * pData, FRAME_LEN_TYPE uLen);

extern Std_ReturnType canfd_data_read(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pDest, FRAME_LEN_TYPE * pLen);

extern Std_ReturnType canfd_data_read_no_step(FRAME_ID_TYPE * pId, FRAME_DATA_TYPE * pDest, FRAME_LEN_TYPE * pLen);

extern Std_ReturnType canfd_rx_data_fifo_step_head(void);

#ifdef  __cplusplus
}
#endif  /* defined(__cplusplus) */
#endif /* _CANFD_FIFO_H_ */

