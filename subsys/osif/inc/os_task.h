/**
*********************************************************************************************************
*               Copyright(c) 2024, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      os_task.h
* @brief     Header file for os task API.
* @details   This file is used for task create, delete and other operation.
* @author    rui_yu
* @date      2024-12-30
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _OS_TASK_H_
#define _OS_TASK_H_

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \defgroup    OS_TASK Task Management
 *
 * \brief   Define, create, and control task functions.
 * \details The Task Management function group allows creating, deleting, and controlling tasks
 *          in the system.\n
 * Tasks can be in the following states:\n
 * \arg <b>RUNNING:</b> The task that is currently running is in the running state.
 *                      Only one task at a time can be in this state.
 * \arg <b>READY:</b> Tasks that are ready to run are in the ready state. Once the
 *                    running task has terminated or is waiting, the next
 *                    ready task with the highest priority becomes the running task.
 * \arg <b>WAITING:</b> Tasks that are waiting for an event to occur are in the waiting state.
 * \arg <b>INACTIVE:</b> Tasks that are not created or terminated are in the inactive state.
 *                       These tasks typically consume no system resources.
 *
 * \image html OS-task-state-transition.jpg "Task State Transition" width=50px height=50px
 *
 */

/*============================================================================*
 *                              Functions
*============================================================================*/
/** @defgroup OS_TASK_Exported_Functions OS Task Exported Functions
  * \ingroup  OS_TASK
  * @{
  */

/**
  * @brief  Allocate secure stack for task switch when trustzone is enabled.
  * @param  stack_size Secure stack size, unit: byte.
  * @return Result of allocating secure stack. It return false when there is not enough secure heap to malloc.
  */
extern bool (*os_alloc_secure_ctx)(uint32_t stack_size);

/**
 *
 * \brief Create a new task and add it to the list of tasks that are ready to run.
 *
 * \param[out]  pp_handle   Pointer used to pass back a handle by which the created task
 *                         can be referenced.
 *
 * \param[in]   p_name     A descriptive name for the task.
 *
 * \param[in]   p_routine  Pointer to task routine function that must be implemented
 *                         to never return.
 *
 * \param[in]   p_param    Pointer parameter passed to the task routine function.
 *
 * \param[in]   stack_size The size of the task stack that is specified as the number
 *                         of bytes.
 *
 * \param[in]   priority   The priority at which the task should run. Higher priority
 *                         task has a higher priority value.
 *
 * \return           The status of the task creation.
 * \retval true      Task was created successfully and added to the task ready list.
 * \retval false     Task failed to be created. It happens when there is not enough heap for task stack.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *     }
 * }
 *
 * // Task to be created.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t task_param;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        &task_param, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Use the handle to delete the task.
 *     os_task_delete(p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_create)(void **pp_handle, const char *p_name, void (*p_routine)(void *),
                              void *p_param, uint16_t stack_size, uint16_t priority);

/**
 *
 * \brief Remove a task from RTOS's task management. The task being deleted will be removed
 * from RUNNING, READY, or WAITING state.
 *
 * \param[in] p_handle  The handle of the task to be deleted.
 *
 * \return           The status of the task deletion. It will always return true.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *     }
 * }
 *
 * // Task to be created and deleted.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t task_param;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        &task_param, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Use the handle to delete the task.
 *     os_task_delete(p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_delete)(void *p_handle);

/**
 *
 * \brief Suspend the task. The suspended task will not be scheduled and never get
 * any microcontroller processing time.
 *
 * \param[in] p_handle  The handle of the task to be suspended.
 *
 * \return           The status of the task suspension. It will always return true.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *     }
 * }
 *
 * // Task to be created and suspended.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t task_param;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        &task_param, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Use the handle to suspend the created task.
 *     os_task_suspend(p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_suspend)(void *p_handle);

/**
 *
 * \brief Resume the suspended task.
 *
 * \param[in] p_handle  The handle of the task to be resumed.
 *
 * \return           The status of the task resume. It will always return true.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Task code goes here.
 *     }
 * }
 *
 * // Task to be suspended and resumed.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t task_param;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        &task_param, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Use the handle to suspend the created task.
 *     os_task_suspend(p_handle);
 *
 *     // Resume the suspended task by ourselves.
 *     os_task_resume(p_handle);
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_resume)(void *p_handle);

/**
 *
 * \brief Force a context switch and pass control to the next task that is in ready state.
 *
 * \return           The status of the task resume. It will always return true.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Task routine implementation.
 * void task_routine(void *p_param)
 * {
 *     for (;;)
 *     {
 *          // Force a context switch
 *          os_task_yield();
 *     }
 * }
 *
 * // Task to be created.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t task_param;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        &task_param, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_yield)(void);

/**
 *
 * \brief Get the handle of the currently running task.
 *
 * \param[out] pp_handle  Pointer used to pass back a handle by which the current task
 *                        can be referenced.
 *
 * \return           The status of getting the current task handle.
 * \retval true      Task handle was obtained successfully.
 * \retval false     Task handle failed to get. It happens when pp_handle is NULL.
 *
 * <b>Example usage</b>
 * \code{.c}
 * // Get current task handle.
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *
 *     os_task_handle_get(&p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_handle_get)(void **pp_handle);

/**
 *
 * \brief Get the priority of the specified task.
 *
 * \param[in] p_handle    The handle of the task to be queried. Passing a NULL handle
 *                        means querying the priority of the current task.
 *
 * \param[in] p_priority Pointer used to pass back the priority of the task.
 *
 * \return           The status of getting the task priority.
 * \retval true      Task priority was obtained successfully.
 * \retval false     Task priority failed to get. It happens when p_handle is NULL.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint16_t priority;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Get the task priority.
 *     os_task_priority_get(p_handle, &priority);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_priority_get)(void *p_handle, uint16_t *p_priority);

/**
 *
 * \brief Set the priority of the specified task.
 *
 * \param[in] p_handle  The handle of the task for which the priority is being set.
 *                      Passing a NULL handle means setting the priority of the
 *                      current task.
 *
 * \param[in] priority  The priority to which the task will be set.
 *
 * \return           The status of setting the task priority. It will always return true.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint16_t priority;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Use the handle to raise the created task priority.
 *     os_task_priority_set(p_handle, TASK_PRIORITY + 1);
 *
 *     // Use a NULL handle to raise the current task priority.
 *     os_task_priority_set(NULL, TASK_PRIORITY + 1);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_priority_set)(void *p_handle, uint16_t priority);


/** End of group OS_TASK_Exported_Functions
  * @}
  */

/**
  * \cond     private
  * \defgroup OS_TASK_Private_Functions
  * \{
  */

/**
 *
 * \brief Send a notification signal to the specified task.
 *
 * The notification signal sent to a task will remain pending until it is
 * cleared by the task calling os_task_signal_recv(). If the task was already
 * in the WAITING state to wait for the signal, then the task will be removed
 * from WAITING state and the signal cleared.
 *
 * \param[in] p_handle  The handle of the task to which the signal is sent.
 *
 * \param[in] signal    The signal to be sent.
 *
 * \return          The status of sending the signal.
 * \retval true     Task signal was sent successfully.
 * \retval false    Task signal failed to send.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t signal;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Send signal to the created task.
 *     signal = 1;
 *     os_task_signal_send(p_handle, signal);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_signal_send)(void *p_handle, uint32_t signal);

/**
 *
 * \brief Wait for a notification signal.
 *
 * \param[out] p_signal Used to pass back the received signal.
 *
 * \param[in] wait_ms   The timeout in milliseconds to wait for the signal.
 * \arg \c 0            No blocking and return immediately.
 * \arg \c 0xFFFFFFFF   Block infinitely until the signal received.
 * \arg \c others       The timeout value in milliseconds.
 *
 * \return          The status of receiving the signal.
 * \retval true     Task signal was received successfully.
 * \retval false    Task signal failed to receive.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     uint32_t signal;
 *
 *     // Block to wait for the signal sent from other tasks.
 *     os_task_signal_recv(&signal, 0xFFFFFFFF);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_signal_recv)(uint32_t *p_signal, uint32_t wait_ms);

/**
 *
 * \brief Clear the signal of the specified task.
 *
 * \param[in] p_handle  The handle of the task to which the signal is cleared.
 *
 * \return          The status of clearing the signal.
 * \retval true     Task signal was cleared successfully.
 * \retval false    Task signal failed to clear.
 *
 * <b>Example usage</b>
 * \code{.c}
 * int test(void)
 * {
 *     void *p_handle = NULL;
 *     uint32_t signal;
 *
 *     if (os_task_create(&p_handle, "task", task_routine,
 *                        NULL, STACK_SIZE, TASK_PRIORITY) == true)
 *     {
 *         // Task created successfully.
 *     }
 *     else
 *     {
 *         // Task failed to create.
 *         return -1;
 *     }
 *
 *     // Send signal to the created task.
 *     signal = 1;
 *     os_task_signal_send(p_handle, signal);
 *
 *     // Clear signal of the created task.
 *     os_task_signal_clear(p_handle);
 *
 *     return 0;
 * }
 * \endcode
 *
 */
extern bool (*os_task_signal_clear)(void *p_handle);

extern bool (*os_task_signal_create)(void *p_handle, uint32_t count);

extern bool (*os_task_notify_take)(long xClearCountOnExit, uint32_t xTicksToWait,
                                   uint32_t *p_notify);
extern bool (*os_task_notify_give)(void *p_handle);

extern void (*os_task_status_dump)(void);
/**
  *  End of OS_TASK_Private_Functions
  * \}
  * \endcond
  */

/** End of OS_TASK
  * @}
  */
#ifdef __cplusplus
}
#endif

#endif /* _OS_TASK_H_ */
