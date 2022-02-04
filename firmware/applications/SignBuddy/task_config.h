#ifndef TASK_CONFIG_H
#define TASK_CONFIG_H

// higher number is higher priority
#define SENSORS_TASK_PRIORITY    (tskIDLE_PRIORITY + 3)
#define TSC_TASK_PRIORITY        (tskIDLE_PRIORITY + 4)

#define SENSORS_STACK_SIZE       512
#define TSC_STACK_SIZE           256

/* IRQ CONFIGS */
// lower number is higher priority - lowest priority is 3 - max is 0

#endif // TASK_CONFIG_H
