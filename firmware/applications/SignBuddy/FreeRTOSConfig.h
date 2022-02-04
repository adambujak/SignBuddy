#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H

/* Ensure stdint is only used by the compiler, and not the assembler. */
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
    #include <stdint.h>
extern uint32_t SystemCoreClock;
#endif

#define configUSE_PREEMPTION               1
#define configUSE_IDLE_HOOK                0
#define configUSE_TICK_HOOK                0
#define configCPU_CLOCK_HZ                 (SystemCoreClock)
#define configTICK_RATE_HZ                 (( TickType_t ) 1000)
#define configMAX_PRIORITIES               (5)
#define configMINIMAL_STACK_SIZE           (( unsigned short ) 60)
#define configTOTAL_HEAP_SIZE              (( size_t ) (2048))
#define configMAX_TASK_NAME_LEN            (5)
#define configUSE_TRACE_FACILITY           1
#define configUSE_16_BIT_TICKS             0
#define configIDLE_SHOULD_YIELD            1
#define configUSE_MUTEXES                  1
#define configQUEUE_REGISTRY_SIZE          8
#define configCHECK_FOR_STACK_OVERFLOW     2
#define configUSE_RECURSIVE_MUTEXES        1
#define configUSE_MALLOC_FAILED_HOOK       0
#define configUSE_APPLICATION_TASK_TAG     0
#define configUSE_COUNTING_SEMAPHORES      1
#define configGENERATE_RUN_TIME_STATS      0

/* Co-routine definitions. */
#define configUSE_CO_ROUTINES              0
#define configMAX_CO_ROUTINE_PRIORITIES    (2)

/* Software timer definitions. */
#define configUSE_TIMERS                   1
#define configTIMER_TASK_PRIORITY          (2)
#define configTIMER_QUEUE_LENGTH           5
#define configTIMER_TASK_STACK_DEPTH       (80)

/* Set the following definitions to 1 to include the API function, or zero
 * to exclude the API function. */
#define INCLUDE_vTaskPrioritySet           1
#define INCLUDE_uxTaskPriorityGet          1
#define INCLUDE_vTaskDelete                1
#define INCLUDE_vTaskCleanUpResources      1
#define INCLUDE_vTaskSuspend               1
#define INCLUDE_vTaskDelayUntil            1
#define INCLUDE_vTaskDelay                 1

/* Normal assert() semantics without relying on the provision of an assert.h
 * header file. */
// *INDENT-OFF* // uncrustify ignore comment
#define configASSERT(x)                    if ((x) == 0) { taskDISABLE_INTERRUPTS(); for ( ;;); }
// *INDENT-ON*

/* Definitions that map the FreeRTOS port interrupt handlers to their CMSIS
 * standard names - or at least those used in the unmodified vector table. */
#define vPortSVCHandler        SVC_Handler
#define xPortPendSVHandler     PendSV_Handler
#define xPortSysTickHandler    OSSysTick_Handler

#endif /* FREERTOS_CONFIG_H */
