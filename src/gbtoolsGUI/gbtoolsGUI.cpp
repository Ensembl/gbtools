/*  File: gbtoolsGUI.cpp
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
 * Description: See gbtoolsGUI.hpp
 *----------------------------------------------------------------------------
 */

#include <gbtools/gbtoolsGUI.hpp>

namespace gbtools
{

  /* Utility to get the size of the monitor multiplied by the given width/height fractions. Returns true
   * if successful, false if there was a problem (in which case output values are unchanged) */
  gboolean getMonitorSizeFraction(GtkWidget *widget, const double widthFraction, const double heightFraction, int *widthOut, int *heightOut)
  {
    gboolean result = getMonitorSize(widget, widthOut, heightOut);

    if (result)
      {
        if (widthOut)
          *widthOut *= widthFraction;
        
        if (heightOut)
          *heightOut *= heightFraction;
      }

    return result;
  }


  /* Utility to get the size of the monitor that the given widget is displayed on. Returns true if 
   * successful, false if there was a problem (in which case output values are unchanged) */
  gboolean getMonitorSize(GtkWidget *widget, int *widthOut, int *heightOut)
  {
    gboolean result = FALSE;
  
    if (!widthOut && !heightOut)
      {
        result = TRUE; /* didn't ask for anything so no error */
      }
    else if (widget)
      {
        /* Note that a screen may consist of multiple monitors, so we actually use the current
         * monitor dimensions as this makes more sense than spreading our windows across multiple
         * monitors */
        GdkScreen *screen = gtk_widget_get_screen(widget);
        int monitor_idx = 0; /* default to first monitor (idx 0) */

        if (widget->window)
          monitor_idx = gdk_screen_get_monitor_at_window(screen, widget->window);
#if CHECK_GTK_VERSION(2, 20)
        else
          monitor_idx = gdk_screen_get_primary_monitor(screen);
#endif

        GdkRectangle rect;
#if CHECK_GTK_VERSION(3, 4)
        gdk_screen_get_monitor_workarea(screen, monitor_idx, &rect);
#else
        gdk_screen_get_monitor_geometry(screen, monitor_idx, &rect);
#endif
      
        if (widthOut)
          *widthOut = rect.width;
  
        if (heightOut)
          *heightOut = rect.height;

        result = TRUE;
      }

    return result;
  }


}
