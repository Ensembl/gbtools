/*  File: gbtoolsThreadLock.cpp
 *  Author: Ed Griffiths (edgrif@sanger.ac.uk)
 *  Copyright (c) 2016: Genome Research Ltd.
 *-------------------------------------------------------------------
 * ZMap is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * or see the on-line version at http://www.gnu.org/copyleft/gpl.txt
 *-------------------------------------------------------------------
 * This file is part of the ZMap genome database package
 * originally written by:
 *
 * 	Ed Griffiths (Sanger Institute, UK) edgrif@sanger.ac.uk,
 *      Steve Miller (Sanger Institute, UK) sm23@sanger.ac.uk
 *      Gemma Barson (Sanger Institute, UK) gb10@sanger.ac.uk
 *
 * Description: Provides global locking to ensure that within a
 *              program only one thread at a time is running through
 *              a section of code.
 *
 * Exported functions: See gbtools/gbtoolsUtils.hpp
 *
 *-------------------------------------------------------------------
 */

#include <gbtools/gbtoolsUtils.hpp>




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
