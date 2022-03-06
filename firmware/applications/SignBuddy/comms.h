#ifndef COMMS_H
#define COMMS_H

#include "sign_buddy.pb.h"

void comms_task_setup(void);
void comms_task_start(void);

void comms_tx_sample(SBPSample *);
void comms_tx_status(SBPStatus *);

#endif // COMMS_H
