#ifndef IMU_H
#define IMU_H

#include <stdint.h>

#include "sign_buddy.pb.h"

void imu_task_setup(void);
void imu_task_start(void);
void imu_start_read(void);

void imu_callback_register(void (*callback)(void));

void imu_data_get(SBPSample_IMUData *data);

#endif // IMU_H
