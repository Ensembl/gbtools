/*  File: gbtoolsCurl_P.h
 *  Author: Roy Storey (rds@sanger.ac.uk)
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
 * 	Ed Griffiths (Sanger Institute, UK) edgrif@sanger.ac.uk,
 *      Roy Storey (Sanger Institute, UK) rds@sanger.ac.uk
 *
 * Description: Internal structs etc for the object interface to
 *              the curl library.
 *
 *-------------------------------------------------------------------
 */
#ifndef GBTOOLS_CURL_P_H
#define GBTOOLS_CURL_P_H

#include <gbtools/gbtoolsCurl.hpp>


namespace gbtools
{

#define CURL_PARAM_STATIC    (G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB)
#define CURL_PARAM_STATIC_RO (CURL_PARAM_STATIC | G_PARAM_READABLE)
#define CURL_PARAM_STATIC_RW (CURL_PARAM_STATIC | G_PARAM_READWRITE)
#define CURL_PARAM_STATIC_WO (CURL_PARAM_STATIC | G_PARAM_WRITABLE)


enum
  {
    CONNECTION_CLOSED_SIGNAL,
    LAST_SIGNAL
  };

typedef struct _curlObjectStruct
{
  GObject __parent__;

  CURL  *easy{NULL};
  CURLM *multi{NULL};
  
  CURLcode  last_easy_status{CURLE_OK};
  CURLMcode last_multi_status{CURLM_OK};

  GQueue *perform_queue{NULL};

  gpointer settings_to_destroy{NULL};
  gpointer post_data_2_free{NULL};

  char error_message[CURL_ERROR_SIZE]{};

  curl_version_info_data *curl_version{NULL};

  GList *pre717strings{NULL};

  bool allow_queue{false};
  bool transfer_in_progress{false};
  bool manage_post_data{false};
  bool debug{false};
} curlObjectStruct;

typedef struct _curlObjectClassStruct
{
  GObjectClass parent_class;

  CURLcode  global_init{CURLE_OK};

  void (* connection_closed)(CURLObject object){NULL};

  guint signals[LAST_SIGNAL]{};

} curlObjectClassStruct;



} /* gbtools namespace */


#endif /* GBTOOLS_CURL_P_H */
