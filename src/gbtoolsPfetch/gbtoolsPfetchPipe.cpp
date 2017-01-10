/*  File: gbtoolsPfetch.cpp
 *  Author: Ed Griffiths (edgrif@sanger.ac.uk)
 *  Copyright (c) 2006-2017: Genome Research Ltd.
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
 * Description: pipe interface to pfetch server, retrieves sequence
 *              information by a pipe connected to the pfetch client.
 *
 * Exported functions: See <gbtools/gbtoolsPfetch.hpp
 *
 *-------------------------------------------------------------------
 */

#include <string.h>
#include <sys/wait.h>
#include <errno.h>
#include <unistd.h>

#include <gbtools/gbtoolsPfetch.hpp>



namespace gbtools
{

#define PFETCH_READ_SIZE 80


  // I NEED TO FIX THIS STRUCT UP TO PASS TO OTHER STATIC FUNCTIONS.....
  // I WONDER IF I CAN MAKE THE STATIC FUNCTIONS "FRIEND" FUNCTIONS
  // SO THEY CAN ACCESS THE OBJECT DIRECTLY.....
  // THIS IS TEDIOUS BUT I DON'T WANT ALL THIS IN THE CLASS STRUCT...

  struct PfetchPipeData
  {
    // Need access to the pfetch object from the callbacks.
    PfetchPipe *pfetch ;

    guint    watch_id;
    GPid     watch_pid;

    // this needs to go.....
    gpointer watch_data;


    /* callbacks */
    guint stdin_source_id;
    guint stdout_source_id;
    guint stderr_source_id;
    guint timeout_source_id;


    // user callbacks & data....repeated to here to avoid exposing implementation in class definition.
    ReaderFunc reader_func ;
    ErrorFunc error_func ;
    ClosedFunc closed_func ;
    void *user_data ;


  } ;




  static void pfetch_get_argv(const char *location, bool full, const char *request, char **argv) ;

  static bool pfetch_spawn_async_with_pipes(PfetchPipeData *pfetch_data,
                                            char *argv[], 
                                            GIOFunc stdout_reader, GIOFunc stderr_reader, 
                                            GDestroyNotify pipe_data_destroy, 
                                            GError **error,
                                            GChildWatchFunc child_func) ;

  static gboolean pipe_stdout_func(GIOChannel *source, GIOCondition condition, gpointer user_data) ;
  static gboolean pipe_stderr_func(GIOChannel *source, GIOCondition condition, gpointer user_data) ;
  static void pfetch_child_watch_func(GPid pid, gint status, gpointer user_data) ;
  static gboolean timeout_pfetch_process(gpointer user_data) ;
  static void source_remove_and_zero(guint *source_id);
  static void detach_and_kill(PfetchPipeData *pfetch_data) ;
  static void detach_group_for_later_kill(void *unused) ;

  static gboolean fd_to_GIOChannel_with_watch(gint fd, GIOCondition cond, GIOFunc func, 
                                              gpointer data, GDestroyNotify destroy,
                                              guint *source_id_out);


  //
  //              Class interface
  //

  PfetchPipe::PfetchPipe(const char *location,
                         ReaderFunc reader_func, ErrorFunc error_func, ClosedFunc closed_func,
                         void *user_data)
    : Pfetch{location, reader_func, error_func, closed_func, user_data}
{

  return ;
}



  bool PfetchPipe::fetch(const char *sequence, char **error_msg)
  {
    bool result = false ;
    PfetchPipeData *pfetch_data ;
    char *argv[256];
    GError *error = NULL ;

    // WHERE IS THIS FREED ??????? YOU WALLY.....
    // Set up out callback data which includes this object.
    pfetch_data = new PfetchPipeData ;
    pfetch_data->pfetch = this ;
    pfetch_data->reader_func = reader_func_ ;
    pfetch_data->error_func = error_func_ ;
    pfetch_data->closed_func = closed_func_ ;
    pfetch_data->user_data = user_data_ ;


    // Set up the command line for the call to the pfetch script.
    pfetch_get_argv(location_, opts_.full, sequence, &argv[0]) ;

    if ((result = pfetch_spawn_async_with_pipes(pfetch_data,
                                                argv, 
                                                (reader_func_ != 0 ? pipe_stdout_func : NULL),
                                                (error_func_  != 0 ? pipe_stderr_func : NULL),
                                                NULL, &error, 
                                                pfetch_child_watch_func)))
      {
        pfetch_data->timeout_source_id = g_timeout_add(30 * 1000, timeout_pfetch_process, pfetch_data) ;
      }
    else
      {
        *error_msg = strdup(error->message) ;

        g_error_free(error) ;

        // logging ?
      }
    
    return result ;
  }



  PfetchPipe::~PfetchPipe()
  {

    return ;
  }



  //
  // Internal functions.
  //


  static void pfetch_get_argv(const char *location, bool full, const char *request, char **argv)
  {
    int current = 0 ;

    argv[current++] = (char *)location ;

    if (full)
      argv[current++] = (char *)"-F" ;

    if (request != NULL)
      argv[current++] = (char *)request ;

    argv[current] = NULL ;
  
    return ;
  }



// Interface to pipe connection via GLib GIO objects.


/* Wrapper around g_spawn_async_with_pipes() to aid with the logic
 * contained in making that call. In the process of making the call
 * the function converts requested File descriptors to non-blocking
 * GIOChannels adding the corresponding GIOFunc to the channel with
 * g_io_add_watch.
 *
 * Passing NULL for any of the GIOFunc parameters means the rules
 * g_spawn_async_with_pipes() uses apply as NULL will be passed in
 * place there.
 *
 * N.B. passing a GChildWatchFunc is untested... I waited for 
 * G_IO_HUP instead.
 *
 * @param **argv        Argument vector
 * @param stdout_reader GIOFunc to handle reading from the pipe.
 * @param stderr_reader GIOFunc to handle reading from the pipe.
 * @param pipe_data     user data to pass to the GIOFunc
 * @param pipe_data_destroy GDestroyNotify for pipe_data
 * @param error         GError location to return into 
 *                       (from g_spawn_async_with_pipes)
 * @param child_func    A GChildWatchFunc for the spawned PID 
 * @param child_func_data data for the child_func
 * @return              gboolean from g_spawn_async_with_pipes()
 *
 *  */

static bool pfetch_spawn_async_with_pipes(PfetchPipeData *pfetch_data,
                                          char *argv[],
                                          GIOFunc stdout_reader, GIOFunc stderr_reader, 
                                          GDestroyNotify pipe_data_destroy,
                                          GError **error,
                                          GChildWatchFunc child_func)
{
  bool result = false ;
  char *cwd = NULL, **envp = NULL;
  GSpawnChildSetupFunc child_setup = detach_group_for_later_kill ;
  gpointer child_data = pfetch_data;
  GPid child_pid;
  gint *stdin_ptr = NULL,                                   // stdin is unused.
    *stdout_ptr = NULL,
    *stderr_ptr = NULL,
    stdout_fd, 
    stderr_fd;
  GSpawnFlags flags = G_SPAWN_SEARCH_PATH;
  GError *our_error = NULL ;

  if(stdout_reader)
    stdout_ptr = &stdout_fd;
  if(stderr_reader)
    stderr_ptr = &stderr_fd;

  if(child_func)
    flags = (GSpawnFlags)(flags | G_SPAWN_DO_NOT_REAP_CHILD) ;
  else
    child_setup = NULL ;

  if ((result = g_spawn_async_with_pipes(cwd, argv, envp,
                                         flags, child_setup, child_data, 
                                         &child_pid, stdin_ptr,
                                         stdout_ptr, stderr_ptr, 
                                         &our_error)))
    {
      GIOCondition read_cond  = (GIOCondition)(G_IO_IN  | G_IO_HUP | G_IO_ERR | G_IO_NVAL);

      if(child_func && pfetch_data && (pfetch_data->watch_pid = child_pid))
	pfetch_data->watch_id = g_child_watch_add(child_pid, child_func, pfetch_data) ;

      /* looks good so far... Set up the readers... */
      if(stdout_ptr && !fd_to_GIOChannel_with_watch(*stdout_ptr, read_cond, stdout_reader,
                                                    pfetch_data, pipe_data_destroy, &(pfetch_data->stdout_source_id)))
	g_warning("%s", "Failed to add watch to stdout");
      if(stderr_ptr && !fd_to_GIOChannel_with_watch(*stderr_ptr, read_cond, stderr_reader,
                                                    pfetch_data, pipe_data_destroy, &(pfetch_data->stderr_source_id)))
	g_warning("%s", "Failed to add watch to stderr");
    }
  else
    {
      *error = our_error ;
    }

  return result ;
}



/* GIOFunc - return TRUE to get called again */
static gboolean pipe_stdout_func(GIOChannel *source, GIOCondition condition, gpointer user_data)
{
  gboolean call_again = FALSE ;
  PfetchPipeData *pfetch_data = (PfetchPipeData *)user_data ;

  if(condition & G_IO_IN)
    {
      GIOStatus io_status;
      gchar output[PFETCH_READ_SIZE] = {0};
      gsize actual_read = 0;
      GError *error = NULL;
      
      if((io_status = g_io_channel_read_chars(source, &output[0], PFETCH_READ_SIZE, 
					      &actual_read, &error)) == G_IO_STATUS_NORMAL)
	{
	  guint actual_read_uint = (guint)actual_read;
          char *err_msg = NULL ;

          // Just call the users read routine.....and get the response...
          call_again = pfetch_data->reader_func(&output[0], &actual_read_uint, &err_msg, pfetch_data->user_data) ;
	}
      else
	{
	  guint actual_read_uint = 0;
          char *err_msg = NULL ;

	  switch(error->code)
	    {
	      /* Derived from errno */
	    case G_IO_CHANNEL_ERROR_FBIG:
	    case G_IO_CHANNEL_ERROR_INVAL:
	    case G_IO_CHANNEL_ERROR_IO:
	    case G_IO_CHANNEL_ERROR_ISDIR:
	    case G_IO_CHANNEL_ERROR_NOSPC:
	    case G_IO_CHANNEL_ERROR_NXIO:
	    case G_IO_CHANNEL_ERROR_OVERFLOW:
	    case G_IO_CHANNEL_ERROR_PIPE:
	      break;
	      /* Other */
	    case G_IO_CHANNEL_ERROR_FAILED:
	    default:
	      break;
	    }

	  output[0]   = '\0';

          // just call the users error routine.....
          call_again = pfetch_data->reader_func(&output[0], &actual_read_uint, &err_msg, pfetch_data->user_data) ;

          // Probably should just set this....
	  call_again = FALSE;
	}
    }
  else if(condition & G_IO_HUP)
    {
      
    }
  else if(condition & G_IO_ERR)
    {

    }
  else if(condition & G_IO_NVAL)
    {

    }


  return call_again ;
}



/* GIOFunc - return TRUE to get called again */
static gboolean pipe_stderr_func(GIOChannel *source, GIOCondition condition, gpointer user_data)
{
  gboolean call_again = FALSE ;
  PfetchPipeData *pfetch_data = (PfetchPipeData *)(user_data) ;

  if (condition & G_IO_IN)
    {
      GIOStatus status;
      gchar text[PFETCH_READ_SIZE] = {0};
      gsize actual_read = 0;
      GError *error = NULL;

      if ((status = g_io_channel_read_chars(source, &text[0], PFETCH_READ_SIZE, 
                                            &actual_read, &error)) == G_IO_STATUS_NORMAL)
	{
	  guint actual_read_uint = (guint)actual_read;
          char *err_msg = NULL ;

          call_again = pfetch_data->error_func(&text[0], &actual_read_uint, &err_msg, pfetch_data->user_data) ;
	}
    }

  return call_again ;
}





static void pfetch_child_watch_func(GPid pid, gint status, gpointer user_data)
{
  PfetchPipeData *pfetch_data = (PfetchPipeData *)(user_data) ;

  if (!WIFSIGNALED(status))
    {
      if(pfetch_data->watch_pid == 0)
	{
	  g_warning("%s", "pfetch pid == 0");
	}
      else if(pfetch_data->watch_pid != pid)
        {
          g_warning("pfetch process pid '%d' does not match '%d'", pid, pfetch_data->watch_pid);
        }

      //gosh not sure what to do here.....how do if and then else get called here....
      // watch_data needs to go...
      else if (pfetch_data->watch_data)
	{
	  pfetch_data->watch_pid = 0;
          
          pfetch_data->closed_func(pfetch_data->user_data) ;
	}
      else
	{
	  /* This is actually the default... */
	  source_remove_and_zero(&(pfetch_data->watch_id));

	  pfetch_data->watch_pid = 0;
	}
    }
  else
    {
      g_warning("pfetch process [pid=%d] terminated (kill -%d).", pid, WTERMSIG(status));
    }

  /* always do this. */
  g_spawn_close_pid(pid) ;

  return ;
}


static gboolean timeout_pfetch_process(gpointer user_data)
{
  PfetchPipeData *pfetch_data = (PfetchPipeData *)(user_data) ;

  if (pfetch_data->watch_pid)
    {
      gchar *text = NULL ;
      bool call_again ;

      text = g_strdup_printf("Process '%s' [pid=%d] timed out. Will be killed.",
                             pfetch_data->pfetch->getLocation(), pfetch_data->watch_pid) ;

      call_again = pfetch_data->error_func(text, 0, NULL, pfetch_data->user_data) ;

      g_free(text) ;

      /* we ned to do this before detach and kill I think... */
      /* returning FALSE ensures it's removed. having a
       * g_source_remove() go on, might be bad. */
      pfetch_data->timeout_source_id = 0;
	  
      detach_and_kill(pfetch_data) ;
    }

  /* Always remove it. */
  return FALSE ;
}



static void source_remove_and_zero(guint *source_id)
{
  if(source_id && *source_id)
    {
      g_source_remove(*source_id);
      
      *source_id = 0;
    }

  return ;
}

static void detach_and_kill(PfetchPipeData *pfetch_data)
{
  GPid pid;

  source_remove_and_zero(&(pfetch_data->stdin_source_id));

  source_remove_and_zero(&(pfetch_data->stdout_source_id));

  source_remove_and_zero(&(pfetch_data->stderr_source_id));

  source_remove_and_zero(&(pfetch_data->timeout_source_id));

  if((pid = pfetch_data->watch_pid))
    {
      pid_t gid;
      pfetch_data->watch_pid = 0;

      errno = 0;
      gid = getpgid(pid);

      if(errno == ESRCH)
	{
	  g_warning("pfetch process [pid=%d] not found!", pid);
	  errno = 0;
	}
      else if(gid > 1 && pid == gid)
	{
	  g_warning("pfetch process [pid=%d, gid=%d] being killed", pid, gid);
	  kill(-gid, SIGKILL);
	}
      else
	{
	  g_warning("pfetch process [pid=%d, gid=%d] not killed as pid != gid", pid, gid);
	}
    }

  return ;
}


// I wonder if this is ever called.....


/* WARNING: This is _only_ executed in the child process! In order to
 * debug this with totalview you _must_ use the following in LDFLAGS
 * -L/usr/totalview/linux-x86/lib -ldbfork */
static void detach_group_for_later_kill(void *unused)
{
  int setgrp_rv = 0;

  errno = 0;

  if((setgrp_rv = setpgid(0,0)) != 0)
    {
      /* You'll almost certainly _not_ see the result of these prints */
      switch(errno)
	{
	case EACCES:
	  printf("errno = EACCES\n");
	  break;
	case EINVAL:
	  printf("errno = EINVAL\n");
	  break;
	case EPERM:
	  printf("errno = EPERM\n");
	  break;
	case ESRCH:
	  printf("errno = ESRCH\n");
	  break;
	case 0:
	default:
	  /* should not be reached according to man setpgrp() */
	  break;
	}
      errno = 0;
    }

  return ;
}


static gboolean fd_to_GIOChannel_with_watch(gint fd, GIOCondition cond, GIOFunc func, 
					    gpointer data, GDestroyNotify destroy,
					    guint *source_id_out)
{
  gboolean success = FALSE;
  GIOChannel *io_channel;

  if((io_channel = g_io_channel_unix_new(fd)))
    {
      GError *flags_error = NULL;
      GIOStatus status;

      if((status = g_io_channel_set_flags(io_channel, 
					  (GIOFlags)(G_IO_FLAG_NONBLOCK | g_io_channel_get_flags(io_channel)), 
					  &flags_error)) == G_IO_STATUS_NORMAL)
	{
	  guint source_id;

	  //g_io_channel_set_encoding(io_channel, "ISO8859-1", NULL);

	  source_id = g_io_add_watch_full(io_channel, G_PRIORITY_DEFAULT, cond, func, data, destroy);

	  if(source_id_out)
	    *source_id_out = source_id;

	  success = TRUE;
	}
      else
	g_warning("%s", flags_error->message);

      g_io_channel_unref(io_channel); /* We don't need this anymore */
    }

  return success;
}







} // End of gbtools namespace...should always be last.
