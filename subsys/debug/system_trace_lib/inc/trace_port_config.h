/****************************************************************************************************//**
 * @file     trace_port_config.h
 *
 * @brief
 *
 * @version  v0.1
 * @date     2018-11-05
 *
 * @note
 *******************************************************************************************************/
#ifndef _TRACE_PORT_CONFIG_H_
#define _TRACE_PORT_CONFIG_H_

#include "mem_config.h"
#include "mem_types.h"


/******************* OS CONFIGURATION  *******************/
#define MAX_TASK_COUNT_FOR_DEBUG                ( 12 )

/******************* HEAP CONFIGURATION *******************/

/* Bee2 has 2 types: data and buffer */
#define TRACE_HEAP_TYPE_NUM                       2
#define TRACE_HEAP_MAX_NUMBER_IN_BLOCK_LIST     ( 16 )




/******************* QUEUE CONFIGURATION *******************/

#define MAX_QUEUE_OBJECTS_NUM                   ( 8 )

/******************* DLPS CONFIGURATION *******************/




/******************* CONFIGURATION END*******************/
#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif /*_TRACE_PORT_CONFIG_H_*/

/******************* (C) COPYRIGHT 2015 Realtek Semiconductor Corporation *****END OF FILE****/
