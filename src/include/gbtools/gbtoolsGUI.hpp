/*  File: gbtoolsGUI.hpp
 *  Author: Gemma Barson, 2015-03-02
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
 * Description: Miscellaneous GUI functions
 *----------------------------------------------------------------------------
 */
#ifndef gbtoolsGUI_h_included
#define gbtoolsGUI_h_included

#include <gbtools/gbtoolsUtils.hpp>
#include <gtk/gtk.h>

namespace gbtools
{

gboolean GUIGetTrueMonitorSize(GtkWidget *widget, int *widthOut, int *heightOut) ;
gboolean GUIGetTrueMonitorSizeFraction(GtkWidget *widget, const double widthFraction, const double heightFraction, int *widthOut, int *heightOut) ;


} /* gbtools namespace */

#endif /* gbtoolsGUI_h_included */
