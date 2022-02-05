#ifndef SENSORS_H
#define SENSORS_H

#include "SignBuddy.pb.h"

void sensors_task_setup(void);
void sensors_task_start(void);
void sensors_data_ready_cb(void);

void get_sensor_data(Sample *);
void comms_data_recvd_cb(void);

#endif // SENSORS_H
