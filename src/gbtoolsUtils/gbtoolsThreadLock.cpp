/*  File: gbtoolsThreadLock.cpp
 *  Author: Ed Griffiths (edgrif@sanger.ac.uk)
 *  Copyright (c) 2006-2017: Genome Research Ltd.
 *-------------------------------------------------------------------
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------
 * This file is part of the ZMap genome database package
 * originally written by:
 * 
 *      Ed Griffiths (Sanger Institute, UK) edgrif@sanger.ac.uk
 *        Roy Storey (Sanger Institute, UK) rds@sanger.ac.uk
 *   Malcolm Hinsley (Sanger Institute, UK) mh17@sanger.ac.uk
 *       Gemma Guest (Sanger Institute, UK) gb10@sanger.ac.uk
 *      Steve Miller (Sanger Institute, UK) sm23@sanger.ac.uk
 *  
 * Description: Provides global locking to ensure that within a
 *              program only one thread at a time is running through
 *              a section of code.
 *
 * Exported functions: See gbtools/gbtoolsUtils.hpp
 *
 *-------------------------------------------------------------------
 */

#include <pthread.h>

#include <gbtools/gbtoolsUtils.hpp>


namespace gbtools 
{


static void once_init_routine(void) ;



//
//                   Globals
//


// Used as a global lock for all threads that call the lock/unlock functions.
//
static bool once_init_status_G = false ;
static pthread_once_t once_control_G = PTHREAD_ONCE_INIT ;
static pthread_mutex_t global_thread_lock_G ;




//
//                   External interface
//




// Two functions to lock/unlock around anything that should not be run in more than one thread at
// a time.

// Lock the thread:
//
// If the attempt to lock is valid the function will either lock and return immediately or block
// until the lock can be made, in either case the function returns 'true' and err_num is left
// untouched.
// If the thread tries to lock when it has already locked then 'false' is returned and err_num
// (if supplied) is set to the system errno.
//
bool UtilsGlobalThreadLock(int *err_num)
{
  bool result = false ;
  int status ;

  if ((status = pthread_once(&once_control_G,  once_init_routine)) != 0)
    {
      // This shouldn't ever really happen but we have to catch it.
      if (err_num)
        *err_num = status ;

      result = false ;
    }

  if (status == 0 && once_init_status_G)
    {
      if ((status = pthread_mutex_lock(&global_thread_lock_G)) != 0)
        {
          if (err_num)
            *err_num = status ;

          result = false ;
        }
      else
        {
          result = true ;
        }
    }

  return result ;
}


// Unlock the thread:
//
// If the attempt to unlock is valid the function will unlock the mutex and return 'true'
// and err_num is left untouched.
//
// If the thread tries to unlock without previously having locked then 'false' is returned
// and err_num (if supplied) is set to the system errno.
//
bool UtilsGlobalThreadUnlock(int *err_num)
{
  bool result = false ;

  if (once_init_status_G)
    {
      int status ;

      if ((status = pthread_mutex_unlock(&global_thread_lock_G)) != 0)
        {
          if (err_num)
            *err_num = status ;

          result = false ;
        }
      else
        {
          result = true ;
        }
    }

  return result ;
}



//
//                   Internal routines
//

// pthreads guarantees that this routine is called just once and it's here we init the global
// mutex.
//
// Not so good is that we don't log or report any errors from here, we don't have a common way to
// do this in the gbtools library so we don't do anything. We do however set a status flag to show
// that initialisation worked.
//
static void once_init_routine(void)
{
  pthread_mutexattr_t mutex_attrs ;
  int status = 0 ;

  // Create attrs here....
  if (status == 0)
    {
      if ((status = pthread_mutexattr_init(&mutex_attrs)) != 0)
        {
          // sadly gbtools has no error report mechanism
        }
    }

  // Set attrs.....
  if (status == 0)
    {
      if ((status = pthread_mutexattr_settype(&mutex_attrs, PTHREAD_MUTEX_ERRORCHECK)) != 0)
        {
          // sadly gbtools has no error report mechanism
        }
    }

  // now init the mutex....
  if (status == 0)
    {
      if ((status = pthread_mutex_init(&global_thread_lock_G, &mutex_attrs)) != 0)
        {
          // sadly gbtools has no error report mechanism
        }
    }

  // Set this flag to signal success in initialising the mutex.
  if (status == 0)
    {
      once_init_status_G = true ;
    }

  return ;
}



} /* gbtools namespace */

