/*
 * Copyright [2018-2021] EMBL-European Bioinformatics Institute
 * 
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 *      http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/* errCatch - help catch errors so that errAborts aren't
 * fatal, and warn's don't necessarily get printed immediately. 
 * Note that error conditions caught this way will tend to
 * leak resources unless there are additional wrappers. 
 *
 * Typical usage is
 * errCatch = errCatchNew();
 * if (errCatchStart(errCatch))
 *     doFlakyStuff();
 * errCatchEnd(errCatch);
 * if (errCatch->gotError)
 *     warn("Flaky stuff failed: %s", errCatch->message->string);
 * errCatchFree(&errCatch); 
 * cleanupFlakyStuff();
 */
#ifndef ERRCATCH_H
#define ERRCATCH_H

#ifndef DYSTRING_H
    #include "dystring.h"
#endif

struct errCatch
/* Something to help catch errors.   */
    {
    struct errCatch *next;	 /* Next in stack. */
    jmp_buf jmpBuf;		 /* Where to jump back to for recovery. */
    struct dyString *message; /* Error message if any */
    boolean gotError;		 /* Some sort of error was caught. */
    boolean gotWarning;		 /* Some sort of error warning was raised. */
    };

struct errCatch *errCatchNew();
/* Return new error catching structure. */

void errCatchFree(struct errCatch **pErrCatch);
/* Free up resources associated with errCatch */

#define errCatchStart(e) (errCatchPushHandlers(e) && setjmp(e->jmpBuf) == 0)
/* Little wrapper around setjmp.  This returns TRUE
 * on the main execution thread, FALSE after abort. */


boolean errCatchPushHandlers(struct errCatch *errCatch);
/* Push error handlers.  Not usually called directly. 
 * but rather through errCatchStart() macro.  Always
 * returns TRUE. */

void errCatchEnd(struct errCatch *errCatch);
/* Restore error handlers and pop self off of catching stack. */

void errCatchReWarn(struct errCatch *errCatch);
/* Re-warn any warnings that happened even though no abort happened 
 * to make them visible. */

boolean errCatchFinish(struct errCatch **pErrCatch);
/* Finish up error catching.  Report error if there is a
 * problem and return FALSE.  If no problem return TRUE.
 * This handles errCatchEnd and errCatchFree. */
#endif /* ERRCATCH_H */

