/*  File: gbtoolsPfetch.hpp
 *  Author: Ed Griffiths (edgrif@sanger.ac.uk)
 *  Copyright (c) 2006-2015: Genome Research Ltd.
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
 *      Roy Storey (Sanger Institute, UK) rds@sanger.ac.uk
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
