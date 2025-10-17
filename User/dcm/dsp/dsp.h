#ifndef _DSP_H_
#define _DSP_H_

/*--------------------------------------------------------------------------*/
/*                                                                          */
/* MACRO DEFINE                                                             */
/*                                                                          */
/*--------------------------------------------------------------------------*/
#define POSITIVE_RESP_POS       0U

#define SUB_SERVICE_POS         1U

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
extern void dsp_init(void);

extern PROTOCAL_FLAG_TYPE dsp_is_service_idle(void);

extern Std_ReturnType dsp_load_service_ptr(PTR_SRV_FUNC ptr_srv_func);

extern void dsp_set_service_busy(void);

extern void dsp_service_proc(void);

extern void dsp_service_monitor_proc(void);

extern void dsp_direct_process_pending(void);

extern void dsp_final(void);


#endif /* _DSP_H_ */

