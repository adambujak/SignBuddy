#ifndef COMMS_H
#define COMMS_H

#include "SignBuddy.pb.h"

void comms_task_setup(void);
void comms_task_start(void);

void comms_tx_data(Sample *);

#endif // comms_H
