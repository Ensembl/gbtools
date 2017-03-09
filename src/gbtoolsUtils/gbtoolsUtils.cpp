/*  File: gbtoolsUtils.cpp
 *  Author: Gemma Barson, 2015-03-24
 *  Copyright (c) 2006-2017 Genome Research Ltd
 * ---------------------------------------------------------------------------
 * gbtools is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 3
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
