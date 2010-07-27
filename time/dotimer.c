#include "timerbase.h"

LONG TimeDelay(LONG unit,ULONG seconds,ULONG micros)
{
	struct timeval tv;
	LONG error;

	tv.tv_secs	= seconds;
	tv.tv_micro	= micros;

	error = DoTimer(&tv,unit,TR_ADDREQUEST);

	return(error);
}

LONG DoTimer(struct timeval *tv,LONG unit,LONG command)
{
	ULONG sigs, breakf;
	LONG error;

	if(__timermp->mp_SigTask != FindTask(NULL))
		return IOERR_OPENFAIL;

	__timereq->tr_node.io_Command	= command;
	__timereq->tr_time.tv_secs	= tv->tv_secs;
	__timereq->tr_time.tv_micro	= tv->tv_micro;

	SetSignal(0,(1UL << __timermp->mp_SigBit));

	breakf = (SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_D | SIGBREAKF_CTRL_E);

	BeginIO((struct IORequest *)__timereq);

	sigs = Wait((1UL << __timermp->mp_SigBit) | breakf );

	if(sigs & breakf) {

		error = IOERR_ABORTED;
	}
	else {
		error = __timereq->tr_node.io_Error;
	}

	if(!CheckIO((APTR)__timereq))
		AbortIO((APTR)__timereq);

	WaitIO((APTR)__timereq);

	tv->tv_secs	= __timereq->tr_time.tv_secs;
	tv->tv_micro	= __timereq->tr_time.tv_micro;

	return(error);
}

#ifdef TEST
int main()
{
	struct timeval tv;
	LONG error;

	tv.tv_secs	= 10;
	tv.tv_micro	= 0;

	error = DoTimer(&tv,0,TR_ADDREQUEST);

	Printf("DoTimer = %ld\n", error );
	Printf("%ld %ld\n", tv.tv_secs, tv.tv_micro );

	return error;
}
#endif
