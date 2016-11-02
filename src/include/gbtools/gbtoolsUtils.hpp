/*  File: gbtoolsUtils.hpp
 *  Author: Gemma Barson, 2015-03-24
 *  Copyright (c) 2015 Genome Research Ltd
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
 * written by
 *      Ed Griffiths      (Sanger Institute, UK)  <edgrif@sanger.ac.uk>
 *      Gemma Barson      (Sanger Institute, UK)  <gb10@sanger.ac.uk>
 *      Steve Miller      (Sanger Institute, UK)  <sm23@sanger.ac.uk>
 *
 * Description: Miscellaneous utility functions
 *----------------------------------------------------------------------------
 */

#ifndef _gbtoolsUtils_h_included_
#define _gbtoolsUtils_h_included_

#include <gtk/gtk.h>

namespace gbtools
{

/* Utility macro to check that our gtk version meets the given minimum version. Use this in
 * an #if statement */
#define CHECK_GTK_VERSION(MAJOR, MINOR) GTK_MAJOR_VERSION > (MAJOR) || (GTK_MAJOR_VERSION == (MAJOR) && GTK_MINOR_VERSION >= (MINOR))


  const char *UtilsGetVersionString() ;
  const char *UtilsGetVersionTitle() ;

  bool UtilsGlobalThreadLock(int *errno) ;
  bool UtilsGlobalThreadUnlock(int *errno) ;


} /* gbtools namespace */

#endif /* _gbtoolsUtils_h_included_ */
