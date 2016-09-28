/*  File: gbtoolsPfetch.cpp
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
 * Description: http interface to pfetch server, retrieves sequence
 *              information by direct http requests to pfetch server
 *              using libcurl to do the requests.
 *
 * Exported functions: See <gbtools/gbtoolsPfetch.hpp>
 *
 *-------------------------------------------------------------------
 */

#include <stdlib.h>
#include <string.h>

#include <gbtools/gbtoolsPfetch.hpp>


namespace gbtools
{

  // control block used from various callbacks.
  struct PfetchHttpData
  {
    // Need access to the pfetch object from the callbacks.
    PfetchHttp *pfetch ;

    CURLObject curl_object ;

    // user callbacks & data....repeated to here to avoid exposing implementation in class definition.
    ReaderFunc reader_func ;
    ErrorFunc error_func ;
    ClosedFunc closed_func ;
    void *user_data ;

    unsigned int request_counter;
  } ;


  static char *build_post_data(PfetchHttpData *pfetch_data,
                               bool full, const char *location, const char *sequence) ;
  static void pfetch_get_argv(const char *location, bool full, const char *request, char **argv) ;

  static size_t http_curl_write_func (void *ptr, size_t size, size_t nmemb, void *stream) ;
  static size_t http_curl_header_func(void *ptr, size_t size, size_t nmemb, void *stream) ;
  static void conn_close_handler(CURLObject curl_object, PfetchHttpData *pfetch_data) ;





  //
  //                    External functions
  //


  PfetchHttp::PfetchHttp(const char *location, unsigned int port,
                         const char* cookie_jar, long ipresolve, const char *cainfo, char *proxy,
                         ReaderFunc reader_func, ErrorFunc error_func, ClosedFunc closed_func,
                         void *user_data)
    : Pfetch{location, reader_func, error_func, closed_func, user_data},
    http_port_{port},
    cookie_jar_location_{strdup(cookie_jar)},
    ipresolve_{ipresolve}, cainfo_{(cainfo ? strdup(cainfo) : NULL)}, proxy_{(proxy ? strdup(proxy) : NULL)},
    curl_object_{NULL}, request_counter_{0}
{

  return ;
}


  /* Returns the error message in the curl object, if there is one. The result is a new string
   * which should be free'd with g_free. Returns null if no error. */
  char *PfetchHttp::getError()
  {
    char *result = NULL ;

    if (curl_object_)
      CURLObjectErrorMessage(curl_object_, &result) ;

    return result ;
  }




  bool PfetchHttp::fetch(const char *sequence, char **error_msg)
  {
    bool result = true ;
    PfetchHttpData *pfetch_data ;
    char *post_data ;

    // Set up out callback data which includes this object.
    pfetch_data = new PfetchHttpData ;
    pfetch_data->pfetch = this ;
    pfetch_data->reader_func = reader_func_ ;
    pfetch_data->error_func = error_func_ ;
    pfetch_data->closed_func = closed_func_ ;
    pfetch_data->user_data = user_data_ ;

    pfetch_data->curl_object = CURLObjectNew();

    g_signal_connect(G_OBJECT(pfetch_data->curl_object), "connection_closed",
                     (GCallback)conn_close_handler, pfetch_data) ;


    if((post_data = build_post_data(pfetch_data, opts_.full, location_, sequence)))
      {
        CURLObjectSet(pfetch_data->curl_object,
                      /* general settings */
                      "debug", opts_.debug,
                      "post",  TRUE,
                      "url",   location_,
                      "port",  http_port_,
                      "ipresolve", ipresolve_,
                      "cainfo", cainfo_,
                      /* request */
                      "postfields", post_data,
                      "cookiefile",  cookie_jar_location_,
                      /* functions */
                      "writefunction",  http_curl_write_func,
                      "writedata",      pfetch_data,
                      "headerfunction", http_curl_header_func,
                      "headerdata",     pfetch_data,
                      /* end of options */
                      NULL) ;

        if (proxy_)
          CURLObjectSet(pfetch_data->curl_object, "proxy",  proxy_, NULL) ;
      
        pfetch_data->request_counter++ ;

        if (CURLObjectPerform(pfetch_data->curl_object, TRUE) == CURL_STATUS_FAILED)
          {
            char *curl_object_error = NULL;
            unsigned int error_size = 0;
            bool result ;
            char *err_msg = NULL ;

            /* first get message */
            CURLObjectErrorMessage(pfetch_data->curl_object, &curl_object_error) ;

            error_size = strlen(curl_object_error);
             
            result = pfetch_data->error_func(curl_object_error, &error_size, &err_msg, pfetch_data->user_data) ;

            /* set our return status */
            *error_msg = err_msg ;

            result = false ;
          }
      }

    return result ;
  }




  
  PfetchHttp::~PfetchHttp()
  {
    free((void *)cookie_jar_location_) ;

    if (cainfo_)
      free((void *)cainfo_) ;

    if (proxy_)
      free((void *)proxy_) ;

    return ;
  }





  //
  //                        Internal functions
  //

  static char *build_post_data(PfetchHttpData *pfetch_data,
                               bool full, const char *location, const char *sequence)
  {
    GString *post_string = NULL;
    char *post = NULL;

    if(sequence)
      {
        char *tmp_str = g_strdup("\0");
        char *argv[256] = { tmp_str };
        char **argv_ptr = &argv[1];

        post_string = g_string_sized_new(128);
      

        //AGH....HACK FOR NOW....BLIXEM SETS UP "request=" IN THE CONFIG FILE, ZMAP DOESN'T....
        if (!(strstr(sequence, "request=")))
          g_string_append_printf(post_string, "request=");

        // this all seems unecessary but let's go with it for now.....      
        pfetch_get_argv(location, full, sequence, &argv[0]) ;

        if(argv_ptr && *argv_ptr)
          {
            while(argv_ptr && *argv_ptr)
              {
                g_string_append_printf(post_string, "%s ", *argv_ptr);
                argv_ptr++;
              }
          }

        post = g_string_free(post_string, FALSE);

        g_free(tmp_str);
      }

    return post;
  }

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


  // libcurl callbacks.
  //

  static size_t http_curl_header_func( void *ptr, size_t size, size_t nmemb, void *stream)
  {
    size_t size_handled = 0;
    
    size_handled = size * nmemb;

    return size_handled;
  }

  static size_t http_curl_write_func( void *ptr, size_t size, size_t nmemb, void *stream)
  {
    size_t size_handled = 0 ;
    PfetchHttpData *pfetch_data = (PfetchHttpData *)(stream) ;
    guint actual_read = 0;
    guint saved_size  = 0;
    bool result ;
    char *err_msg = NULL ;


    saved_size = actual_read = size * nmemb;

    if ((result = pfetch_data->reader_func((char *)ptr, &actual_read, &err_msg, pfetch_data->user_data)))
      {
        size_handled = actual_read ;
      }
  
    if (saved_size != actual_read)
      g_warning("'%d' != '%d'. Are you sure?", saved_size, actual_read);

    return size_handled ;
  }


  // Main task is to destroy the curl object, it's created when we do a fetch and
  // should be destroyed when we have finished.
  static void conn_close_handler(CURLObject curl_object, PfetchHttpData *pfetch_data)
  {
    pfetch_data->closed_func(pfetch_data->user_data) ;

    if (pfetch_data->curl_object)
      {
        pfetch_data->curl_object = CURLObjectDestroy(pfetch_data->curl_object) ;
        pfetch_data->curl_object = NULL ;
      }

    return ;
  }







} // gbtools namespace should always be last
