/*  File: gbtoolsPfetch.cpp
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
 * Description: Abstract base class for pfetch classes.
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

  Pfetch::Pfetch(const char *location,
                 ReaderFunc reader_func, ErrorFunc error_func, ClosedFunc closed_func,
                 void *user_data)
    : location_{strdup(location)},
    reader_func_{reader_func}, error_func_{error_func}, closed_func_{closed_func}, user_data_{user_data}
  {

    return ;
  }


  void Pfetch::setEntryType(bool full_entry)
  {
    opts_.full = full_entry ;

    return ;
  }


  void Pfetch::setDebug(bool debug_on)
  {
    opts_.debug = debug_on ;

    return ;
  }

  const char* Pfetch::getLocation()
  {
    return location_ ;
  }

  Pfetch::~Pfetch()
  {
    free((void *)location_) ;

    return ;
  }


} /* gbtools namespace */
