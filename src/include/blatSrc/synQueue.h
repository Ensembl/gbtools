/*
 * Copyright [2018] EMBL-European Bioinformatics Institute
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

/* synQueue - a sychronized message queue for messages between
 * threads. */

#ifndef SYNQUEUE_H
#define SYNQUEUE_H

struct synQueue *synQueueNew();
/* Make a new, empty, synQueue. */

void synQueueFree(struct synQueue **pSq);
/* Free up synQueue.  Be sure no other threads are using
 * it first though! This will not free any dynamic memory
 * in the messages.  Use synQueueFreeAndVals for that. */

void synQueueFreeAndVals(struct synQueue **pSq);
/* Free up synQueue.  Be sure no other threads are using
 * it first though! This will freeMem all the messages */

void synQueuePut(struct synQueue *sq, void *message);
/* Add message to end of queue. */

void synQueuePutUnprotected(struct synQueue *sq, void *message);
/* Add message to end of queue without protecting against multithreading
 * contention - used before pthreads are launched perhaps. */

void *synQueueGet(struct synQueue *sq);
/* Get message off start of queue.  Wait until there is
 * a message if queue is empty. */

void *synQueueGrab(struct synQueue *sq);
/* Get message off start of queue.  Return NULL immediately 
 * if queue is empty. */

int synQueueSize(struct synQueue *sq);
/* Return number of messages currently on queue. */

#endif /* SYNQUEUE_H */

