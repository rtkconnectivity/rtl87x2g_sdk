/*
 * FreeRTOS Kernel V10.4.4
 * Copyright (C) 2021 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * https://www.FreeRTOS.org
 * https://github.com/FreeRTOS
 *
 */

/*-----------------------------------------------------------
* Portable layer API.  Each function must be defined for each port.
*----------------------------------------------------------*/

#ifndef PORTABLE_H
#define PORTABLE_H

/* Each FreeRTOS port has a unique portmacro.h header file.  Originally a
 * pre-processor definition was used to ensure the pre-processor found the correct
 * portmacro.h file for the port being used.  That scheme was deprecated in favour
 * of setting the compiler's include path such that it found the correct
 * portmacro.h file - removing the need for the constant and allowing the
 * portmacro.h file to be located anywhere in relation to the port being used.
 * Purely for reasons of backward compatibility the old method is still valid, but
 * to make it clear that new projects should not use it, support for the port
 * specific constants has been moved into the deprecated_definitions.h header
 * file. */
#include "deprecated_definitions.h"

/* If portENTER_CRITICAL is not defined then including deprecated_definitions.h
 * did not result in a portmacro.h header file being included - and it should be
 * included here.  In this case the path to the correct portmacro.h header file
 * must be set in the compiler's include path. */
#ifndef portENTER_CRITICAL
#include "portmacro.h"
#endif

#if portBYTE_ALIGNMENT == 32
#define portBYTE_ALIGNMENT_MASK    ( 0x001f )
#elif portBYTE_ALIGNMENT == 16
#define portBYTE_ALIGNMENT_MASK    ( 0x000f )
#elif portBYTE_ALIGNMENT == 8
#define portBYTE_ALIGNMENT_MASK    ( 0x0007 )
#elif portBYTE_ALIGNMENT == 4
#define portBYTE_ALIGNMENT_MASK    ( 0x0003 )
#elif portBYTE_ALIGNMENT == 2
#define portBYTE_ALIGNMENT_MASK    ( 0x0001 )
#elif portBYTE_ALIGNMENT == 1
#define portBYTE_ALIGNMENT_MASK    ( 0x0000 )
#else
#error "Invalid portBYTE_ALIGNMENT definition"
#endif

#ifndef portUSING_MPU_WRAPPERS
#define portUSING_MPU_WRAPPERS 0
#endif

#ifndef portNUM_CONFIGURABLE_REGIONS
#define portNUM_CONFIGURABLE_REGIONS    1
#endif

#ifndef portHAS_STACK_OVERFLOW_CHECKING
#define portHAS_STACK_OVERFLOW_CHECKING    0
#endif

#ifndef portARCH_NAME
#define portARCH_NAME    NULL
#endif

#ifndef configSTACK_ALLOCATION_FROM_SEPARATE_HEAP
/* Defaults to 0 for backward compatibility. */
#define configSTACK_ALLOCATION_FROM_SEPARATE_HEAP 0
#endif

/* *INDENT-OFF* */
#ifdef __cplusplus
    extern "C" {
#endif
/* *INDENT-ON* */

#include "mpu_wrappers.h"
#include "mem_types.h"

    /*
     * Setup the stack of a new task so it is ready to be placed under the
     * scheduler control.  The registers have to be placed on the stack in
     * the order that the port expects to find them.
     *
     */
#if ( portUSING_MPU_WRAPPERS == 1 )
#if ( portHAS_STACK_OVERFLOW_CHECKING == 1 )
    StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                       StackType_t *pxEndOfStack,
                                       TaskFunction_t pxCode,
                                       void *pvParameters,
                                       BaseType_t xRunPrivileged) PRIVILEGED_FUNCTION;
#else
    StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                       TaskFunction_t pxCode,
                                       void *pvParameters,
                                       BaseType_t xRunPrivileged) PRIVILEGED_FUNCTION;
#endif
#else /* if ( portUSING_MPU_WRAPPERS == 1 ) */
#if ( portHAS_STACK_OVERFLOW_CHECKING == 1 )
    StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                       StackType_t *pxEndOfStack,
                                       TaskFunction_t pxCode,
                                       void *pvParameters) PRIVILEGED_FUNCTION;
#if (ENABLE_TRUSTZONE == TRUSTZONE_ROM_DISABLE)
    StackType_t *pxPortInitialiseStack_ns(StackType_t *pxTopOfStack,
                                          StackType_t *pxEndOfStack,
                                          TaskFunction_t pxCode,
                                          void *pvParameters) PRIVILEGED_FUNCTION;
#endif /* if (ENABLE_TRUSTZONE == TRUSTZONE_ROM_DISABLE) */
#else
    StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                       TaskFunction_t pxCode,
                                       void *pvParameters) PRIVILEGED_FUNCTION;
#endif
#endif /* if ( portUSING_MPU_WRAPPERS == 1 ) */

    /* Used by heap_5.c to define the start address and size of each memory region
     * that together comprise the total FreeRTOS heap space. */
    typedef struct HeapRegion
    {
        uint8_t *pucStartAddress;
        size_t xSizeInBytes;
    } HeapRegion_t;

    /* Used to pass information about the heap out of vPortGetHeapStats(). */
    typedef struct xHeapStats
    {
        size_t xAvailableHeapSpaceInBytes[RAM_TYPE_NUM];          /* The total heap size currently available - this is the sum of all the free blocks, not the largest block that can be allocated. */
        size_t xSizeOfLargestFreeBlockInBytes[RAM_TYPE_NUM];      /* The maximum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
        size_t xSizeOfSmallestFreeBlockInBytes[RAM_TYPE_NUM];     /* The minimum size, in bytes, of all the free blocks within the heap at the time vPortGetHeapStats() is called. */
        size_t xNumberOfFreeBlocks[RAM_TYPE_NUM];                 /* The number of free memory blocks within the heap at the time vPortGetHeapStats() is called. */
        size_t xMinimumEverFreeBytesRemaining[RAM_TYPE_NUM];      /* The minimum amount of total free memory (sum of all free blocks) there has been in the heap since the system booted. */
        size_t xNumberOfSuccessfulAllocations[RAM_TYPE_NUM];      /* The number of calls to pvPortMalloc() that have returned a valid memory block. */
        size_t xNumberOfSuccessfulFrees[RAM_TYPE_NUM];            /* The number of calls to vPortFree() that has successfully freed a block of memory. */
    } HeapStats_t;

    /*
     * Used to define multiple heap regions for use by heap_5.c.  This function
     * must be called before any calls to pvPortMalloc() - not creating a task,
     * queue, semaphore, mutex, software timer, event group, etc. will result in
     * pvPortMalloc being called.
     *
     * pxHeapRegions passes in an array of HeapRegion_t structures - each of which
     * defines a region of memory that can be used as the heap.  The array is
     * terminated by a HeapRegions_t structure that has a size of 0.  The region
     * with the lowest start address must appear first in the array.
     */
    void vPortDefineHeapRegions(RAM_TYPE ramType,
                                const HeapRegion_t *const pxHeapRegions) PRIVILEGED_FUNCTION;

    /*
     * Returns a HeapStats_t structure filled with information about the current
     * heap state.
     */
    void vPortGetHeapStats(RAM_TYPE ramType, HeapStats_t *pxHeapStats);

    /*
     * Map to the memory management routines required for the port.
     */
    extern void *(*pvPortMalloc)(RAM_TYPE ramType, size_t xSize) PRIVILEGED_FUNCTION;
    void *pvPortMalloc_auto_type(size_t xSize) PRIVILEGED_FUNCTION;
    extern void (*vPortFree)(void *pv) PRIVILEGED_FUNCTION;
    void vPortInitialiseBlocks(void) PRIVILEGED_FUNCTION;
    extern size_t (* xPortGetFreeHeapSize)(RAM_TYPE ramType) PRIVILEGED_FUNCTION;
    size_t xPortGetMinimumEverFreeHeapSize(RAM_TYPE ramType) PRIVILEGED_FUNCTION;

#if( configSTACK_ALLOCATION_FROM_SEPARATE_HEAP == 1 )
    void *pvPortMallocStack(size_t xSize) PRIVILEGED_FUNCTION;
    void vPortFreeStack(void *pv) PRIVILEGED_FUNCTION;
#else
//#define pvPortMallocStack pvPortMalloc
#define pvPortMallocStack pvPortMalloc_auto_type
#define vPortFreeStack vPortFree
#endif

    /*
     * Setup the hardware ready for the scheduler to take control.  This generally
     * sets up a tick interrupt and sets timers for the correct tick frequency.
     */
    BaseType_t xPortStartScheduler(void) PRIVILEGED_FUNCTION;

    /*
     * Undo any hardware/ISR setup that was performed by xPortStartScheduler() so
     * the hardware is left in its original condition after the scheduler stops
     * executing.
     */
    void vPortEndScheduler(void) PRIVILEGED_FUNCTION;

    /*
     * The structures and methods of manipulating the MPU are contained within the
     * port layer.
     *
     * Fills the xMPUSettings structure with the memory region information
     * contained in xRegions.
     */
#if ( portUSING_MPU_WRAPPERS == 1 )
    struct xMEMORY_REGION;
    void vPortStoreTaskMPUSettings(xMPU_SETTINGS *xMPUSettings,
                                   const struct xMEMORY_REGION *const xRegions,
                                   StackType_t *pxBottomOfStack,
                                   uint32_t ulStackDepth) PRIVILEGED_FUNCTION;
#endif

    /*
     * For internal use only.  Start Scheduler from restore flow
     */
    void vPortStartSchedulerInRestore(void) PRIVILEGED_FUNCTION;

/* *INDENT-OFF* */
#ifdef __cplusplus
    }
#endif
/* *INDENT-ON* */

#endif /* PORTABLE_H */
