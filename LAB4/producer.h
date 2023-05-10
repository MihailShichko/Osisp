#pragma once

#include "init.h"

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

message produce();
void createProducer();
//void putIntoQueue(message message);
void putIntoQueue(msg_buf message);