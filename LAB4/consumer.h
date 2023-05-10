#pragma once

#include "init.h"

#include <sys/types.h>
#include <sys/msg.h>
#include <sys/ipc.h>

extern int consumer_counter;

void consume(message message);
void createConsumer();
message getMessage();
void showMessage(message message);