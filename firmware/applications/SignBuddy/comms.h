#ifndef COMMS_H
#define COMMS_H

#include "SignBuddy.pb.h"

void comms_task_setup(void);
void comms_task_start(void);

void comms_tx_sample(Sample *);
void comms_tx_status(Status *);

#endif // COMMS_H
