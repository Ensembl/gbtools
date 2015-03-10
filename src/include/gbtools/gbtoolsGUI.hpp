/*  File: gbtoolsGUI.hpp
 *  Author: Gemma Barson, 2015-03-02
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
 *      Gemma Barson      (Sanger Institute, UK)  <gb10@sanger.ac.uk>
 * 
 * and utilizing code taken from the AceDB, ZMap and SeqTools packages, written by
 *      Richard Durbin    (Sanger Institute, UK)  <rd@sanger.ac.uk>
 *      Erik Sonnhammer   (SBC, Sweden)           <Erik.Sonnhammer@sbc.su.se>
 *      Jean Thierry-Mieg (CRBM du CNRS, France)  <mieg@kaa.crbm.cnrs-mop.fr>
 *      Ed Griffiths      (Sanger Institute, UK)  <edgrif@sanger.ac.uk>
 *      Roy Storey        (Sanger Institute, UK)  <rds@sanger.ac.uk>
 *      Malcolm Hinsley   (Sanger Institute, UK)  <mh17@sanger.ac.uk>
 *      Gemma Barson      (Sanger Institute, UK)  <gb10@sanger.ac.uk>
 *      Steve Miller      (Sanger Institute, UK)  <sm23@sanger.ac.uk>
 *
 * Description: Miscellaneous GUI functions
 *----------------------------------------------------------------------------
 */

#ifndef _gbtoolsGUI_h_included_
#define _gbtoolsGUI_h_included_

#include <gtk/gtk.h>


namespace gbtools
{

/* Utility macro to check that our gtk version meets the given minimum version. Use this in
 * an #if statement */
#define CHECK_GTK_VERSION(MAJOR, MINOR) GTK_MAJOR_VERSION > (MAJOR) || (GTK_MAJOR_VERSION == (MAJOR) && GTK_MINOR_VERSION >= (MINOR))

  gboolean getMonitorSize(GtkWidget *widget, int *widthOut, int *heightOut) ;
  gboolean getMonitorSizeFraction(GtkWidget *widget, const double widthFraction, const double heightFraction, int *widthOut, int *heightOut) ;

} /* gbtools */

#endif /* _gbtoolsGUI_h_included_ */