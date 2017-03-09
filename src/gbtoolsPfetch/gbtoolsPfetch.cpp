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

