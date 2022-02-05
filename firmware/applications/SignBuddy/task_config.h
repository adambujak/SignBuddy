#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

// higher number is higher priority
#define COMMS_TASK_PRIORITY      (tskIDLE_PRIORITY + 1)
#define SENSORS_TASK_PRIORITY    (tskIDLE_PRIORITY + 2)
#define TSC_TASK_PRIORITY        (tskIDLE_PRIORITY + 3)
#define FLEX_TASK_PRIORITY       (tskIDLE_PRIORITY + 3)

#define SENSORS_STACK_SIZE       256
#define COMMS_STACK_SIZE         256
#define TSC_STACK_SIZE           256
#define FLEX_STACK_SIZE          256

/* IRQ CONFIGS */
// lower number is higher priority - lowest priority is 3 - max is 0

#endif // TASK_CONFIG_H
