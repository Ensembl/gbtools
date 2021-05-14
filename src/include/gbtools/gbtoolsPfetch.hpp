/*  File: gbtoolsPfetch.hpp
 *  Author: Ed Griffiths (edgrif@sanger.ac.uk)
 *  Copyright (c) 2006-2017: Genome Research Ltd.
 *  Copyright [2018-2021] EMBL-European Bioinformatics Institute
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
 * Description: Object implementing fetching of sequence data via
 *              the pfetch server either through a pipe to the
 *              pfetch command line script or through http requests.
 *
 *-------------------------------------------------------------------
 */
#ifndef GBTOOLS_PFETCH_H
#define GBTOOLS_PFETCH_H

#include <glib.h>

#include <gbtools/gbtoolsCurl.hpp>


namespace gbtools
{
  // Callback functions.
  typedef bool (* ReaderFunc)(char *output, guint *output_size, char **error_msg,
                              void *user_data) ;

  typedef bool (* ErrorFunc)(char *output, guint *output_size, char **error_msg,
                             void *user_data) ;

  typedef void (* ClosedFunc)(void *user_data) ;



  // Abstract base class.
  class Pfetch
  {
  public:

    void setEntryType(bool full_entry) ;

    void setDebug(bool debug_on) ;

    virtual bool fetch(const char *sequence, char **error_msg) = 0 ;

    const char* getLocation() ;

    virtual ~Pfetch() ;

  protected:

    Pfetch(const char *location,
           ReaderFunc reader_func, ErrorFunc error_func, ClosedFunc closed_func,
           void *user_data) ;

    const char *location_{NULL} ;

    struct
    {
      bool full{false} ;	/* full pfetch entry (-F on command line) */
      bool debug{false} ;	/* internal debug */
    } opts_ ;

    // user callbacks
    ReaderFunc reader_func_{NULL} ;
    ErrorFunc error_func_{NULL} ;
    ClosedFunc closed_func_{NULL} ;
    void *user_data_{NULL} ;

  private:


  } ;


  // Pipe interface
  //
  class PfetchPipe: public Pfetch
  {
  public:

    // No default contstructor.
    PfetchPipe() = delete ;

    // No copy operations
    PfetchPipe(const PfetchPipe&) = delete ;
    PfetchPipe& operator=(const PfetchPipe&) = delete ;

    // no move operations
    PfetchPipe(PfetchPipe&&) = delete ;
    PfetchPipe& operator=(PfetchPipe&&) = delete ;

    PfetchPipe(const char *location,
               ReaderFunc reader_func, ErrorFunc error_func, ClosedFunc closed_func,
               void *user_data) ;

    bool fetch(const char *sequence, char **error_msg) ;

    ~PfetchPipe() ;

  private:

  } ;


  // http interface
  //
  class PfetchHttp: public Pfetch
  {
  public:

    // No default contstructor.
    PfetchHttp() = delete ;

    // No copy operations
    PfetchHttp(const PfetchHttp&) = delete ;
    PfetchHttp& operator=(const PfetchHttp&) = delete ;

    // no move operations
    PfetchHttp(PfetchHttp&&) = delete ;
    PfetchHttp& operator=(PfetchHttp&&) = delete ;

    PfetchHttp(const char *location, unsigned int port,
               const char* cookie_jar, long ipresolve, const char *cainfo, char *proxy,
               ReaderFunc reader_func, ErrorFunc error_func, ClosedFunc closed_func,
               void *user_data) ;

    void setProxy(const char *proxy) ;

    char* getError() ;

    bool fetch(const char *sequence, char **error_msg) ;

    ~PfetchHttp() ;

  private:

    unsigned int http_port_{0};
    const char *cookie_jar_location_{NULL};
    long ipresolve_{0};
    const char *cainfo_{NULL};
    const  char *proxy_{NULL} ;

    CURLObject curl_object_{NULL};
    unsigned int request_counter_{0};

  } ;



} /* gbtools namespace */


#endif /* GBTOOLS_PFETCH_H */
