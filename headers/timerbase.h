#ifndef LIBNIX_TIMERBASE_H
#define LIBNIX_TIMERBASE_H

#include <exec/errors.h>
#include <devices/timer.h>
#include <proto/exec.h>
#include <proto/alib.h>
#include <proto/timer.h>
#include <string.h>

extern struct timerequest *__timereq;
extern struct MsgPort *__timermp;

GLOBAL LONG DoTimer(struct timeval *tv,LONG unit,LONG command);

#endif /* LIBNIX_TIMERBASE_H */
