/*  File: gbtoolsUtils.cpp
 *  Author: Gemma Barson, 2015-03-24
 *  Copyright (c) 2006-2017 Genome Research Ltd
 * ---------------------------------------------------------------------------
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
 * ---------------------------------------------------------------------------
 * This file is part of the gbtools genome browser tools library, 
 * originally written by:
 * 
 *      Ed Griffiths (Sanger Institute, UK) edgrif@sanger.ac.uk
 *        Roy Storey (Sanger Institute, UK) rds@sanger.ac.uk
 *   Malcolm Hinsley (Sanger Institute, UK) mh17@sanger.ac.uk
 *       Gemma Guest (Sanger Institute, UK) gb10@sanger.ac.uk
 *      Steve Miller (Sanger Institute, UK) sm23@sanger.ac.uk
 *  
 * Description: See gbtoolsUtils.hpp
 *----------------------------------------------------------------------------
 */

#include <gbtools/gbtoolsUtils.hpp>
#include <config.h>
#include <gtk/gtk.h>
#include <string.h>


namespace gbtools 
{

/* Returns a string representing the Version.Release.Update of the gbtools code. */
const char *UtilsGetVersionString()
{
  return GBTOOLS_VERSION ;
}

/* Returns a string containing the library name and version */
const char *UtilsGetVersionTitle()
{
  return "gbtools - " GBTOOLS_VERSION ;
}


} /* gbtools namespace */
