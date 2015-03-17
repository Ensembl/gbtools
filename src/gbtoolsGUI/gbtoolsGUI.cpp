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
#include <gtk/gtk.h>
#include <gdk/gdkx.h>
#include <string.h>


namespace gbtools 
{

/* When we guess max window size we allow this much space for desktop toolbars on the screen. */
#define TOOLBAR_ALLOWANCE 0.90

static gboolean getWorkAreaSize(GdkScreen *screen, gint *width_out, gint *height_out) ;


/* Utility to get the size of the monitor that the given widget is displayed on. Returns true if 
 * successful, false if there was a problem (in which case output values are unchanged).
 *
 * If X windows is configured using RANDR to have several physical screens as one big
 * virtual screen then this routine will still find the size of the current physical
 * screen which is what we want for display rather then spreading zmap across several 
 * screens */
gboolean GUIGetTrueMonitorSize(GtkWidget *widget, int *width_out, int *height_out)
{
  gboolean result = FALSE ;
  GdkScreen *screen = NULL ;
  int monitor_idx = 0 ; /* default to first monitor (idx 0) */
  gint width = 0 ;
  gint height = 0 ;
  GdkRectangle rect ;

  g_return_val_if_fail(widget, result) ;

  /* Note that a screen may consist of multiple monitors, so we actually use the current
   * monitor dimensions as this makes more sense than spreading our windows across multiple
   * monitors */
  screen = gtk_widget_get_screen(widget) ;

  if (widget->window)
    monitor_idx = gdk_screen_get_monitor_at_window(screen, widget->window) ;
#if CHECK_GTK_VERSION(2, 20)
  else
    monitor_idx = gdk_screen_get_primary_monitor(screen) ;
#endif

#if CHECK_GTK_VERSION(3, 4)

  gdk_screen_get_monitor_workarea(screen, monitor_idx, &rect) ;

  width = rect.width ;
  height = rect.height ;

#else

  if (!getWorkAreaSize(screen, &width, &height))
    {
      /* OK, here we just get the raw screen size and return that * some appropriate proportion,
       * note that the window width is kind of irrelevant, we just set it to be a bit less
       * than it will finally be and the widgets will resize it to the correct width.
       * We don't use gtk_window_set_default_size() because it doesn't seem to work. */
      gdk_screen_get_monitor_geometry(screen, monitor_idx, &rect) ;

      width = rect.width ;
      height = rect.height ;

      /* Reduce the height because it's really annoying to have it the full height of the screen
       * initially as nearly everyone has tool bars etc. on their screen.... */
      height = (int)((float)(height) * TOOLBAR_ALLOWANCE) ;
    }

#endif

  if (width_out)
    *width_out = rect.width ;
  
  if (height_out)
    *height_out = rect.height ;

  result = TRUE ;

  return result ;
}


/* Utility to get the size of the monitor multiplied by the given width/height fractions. Returns true
 * if successful, false if there was a problem (in which case output values are unchanged) */
gboolean GUIGetTrueMonitorSizeFraction(GtkWidget *widget, const double width_fraction, const double height_fraction, int *width_out, int *height_out)
{
  gboolean result = gbtools::GUIGetTrueMonitorSize(widget, width_out, height_out) ;

  if (result)
    {
      if (width_out)
        *width_out *= width_fraction ;
        
      if (height_out)
        *height_out *= height_fraction ;
    }

  return result ;
}


/*
 *                      Internal functions
 */

/* Quizzes various Atoms that may be set up in the X server to give hints about
 * screen size and layout (e.g. screen size accounting for toolbars etc) and
 * returns the max work area size or FALSE if the atoms aren't on the server. */
static gboolean getWorkAreaSize(GdkScreen *screen, gint *width_out, gint *height_out)
{
  gboolean result = FALSE ;

  /*! \todo This code causes strange crashing or incorrect window sizes. So far I've only
   * seen the problem in optimised code on ubuntu trusty 64-bit dual monitor machines. */
#ifdef CRASHES_IN_OPTIMISED_CODE
  GdkAtom geometry_atom, workarea_atom, max_atom_vert ;
  int window_width = 0, window_height = 0 ;


  /* Get the atoms for _NET_* properties. */
  geometry_atom = gdk_atom_intern("_NET_DESKTOP_GEOMETRY", FALSE) ;
  workarea_atom = gdk_atom_intern("_NET_WORKAREA", FALSE) ;
  max_atom_vert = gdk_atom_intern("_NET_WM_STATE_MAXIMIZED_VERT", FALSE) ;

  if (gdk_x11_screen_supports_net_wm_hint(screen, geometry_atom)
      && gdk_x11_screen_supports_net_wm_hint(screen, workarea_atom))
    {
      /* We want to get these properties....
       *   _NET_DESKTOP_GEOMETRY(CARDINAL) = 1600, 1200
       *   _NET_WORKAREA(CARDINAL) = 0, 0, 1600, 1154, 0, 0, 1600, 1154,...repeated for all workspaces.
       *
       * In fact we don't use the geometry (i.e. screen size) but its useful
       * to see it.
       *
       * When retrieving 32 bit items, these items will be stored in _longs_, this means
       * that on a 32 bit machine they come back in 32 bits BUT on 64 bit machines they
       * come back in 64 bits.
       *
       *  */
      GdkWindow *root_window ;
      gulong offset, length ;
      gint pdelete = FALSE ;    /* Never delete the property data. */
      GdkAtom actual_property_type ;
      gint actual_format, actual_length, field_size, num_fields ;
      guchar *data, *curr ;
      guint width, height, left, top, right, bottom ;

      field_size = sizeof(glong) ;    /* see comment above re. 32 vs. 64 bits. */

      root_window = gdk_screen_get_root_window(screen) ;


      /* Only get this property because it's good for debugging... */
      offset = 0 ;
      num_fields = 2 ;
      length = num_fields * 4 ;    /* Get two unsigned ints worth of data. */
      actual_format = actual_length = 0 ;
      data = NULL ;
      result = gdk_property_get(root_window,
                                geometry_atom,
                                GDK_NONE,
                                offset,
                                length,
                                pdelete,
                                &actual_property_type,
                                &actual_format,
                                &actual_length,
                                &data) ;

      if (result && num_fields == actual_length/sizeof(glong))
        {
          curr = data ;
          memcpy(&width, curr, field_size) ;
          memcpy(&height, (curr += field_size), field_size) ;
          g_free(data) ;
        }

      offset = 0 ;
      num_fields = 4 ;
      length = num_fields * 4 ;    /* Get four unsigned ints worth of data. */
      actual_format = actual_length = 0 ;
      data = NULL ;
      result = gdk_property_get(root_window,
                                workarea_atom,
                                GDK_NONE,
                                offset,
                                length,
                                pdelete,
                                &actual_property_type,
                                &actual_format,
                                &actual_length,
                                &data) ;

      if (result && num_fields == actual_length/sizeof(glong))
        {
          curr = data ;
          memcpy(&left, curr, field_size) ;
          memcpy(&top, (curr += field_size), field_size) ;
          memcpy(&right, (curr += field_size), field_size) ;
          memcpy(&bottom, (curr += field_size), field_size) ;
          g_free(data) ;

          /* off by one ? */
          window_width = right - left ;
          window_height = bottom - top ;

          *width_out = window_width ;
          *height_out = window_height ;
        }
    }

#endif

  return result ;
}


} /* gbtools namespace */
