#ifndef COMMS_H
#define COMMS_H

#include "SignBuddy.pb.h"

void comms_task_setup(void);
void comms_task_start(void);

void comms_tx_data(Sample *);
void comms_rx_data_ready_cb(void);
void comms_tx_empty_cb(void);
void comms_tx_data_clear_cb(void);

#endif // COMMS_H
