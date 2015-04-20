#ifndef EXTERN_H
#define EXTERN_H

#include "args.h"

/* Function for the parser */

extern void *answerPing(Args);
extern void *managePrivmsg(Args);
extern void *manageJoin(Args);

extern void *managePart(Args);
extern void *manageQuit(Args);
extern void *manageNick(Args);

#endif
