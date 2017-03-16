/*  File: gbtoolsCurl_P.h
 *  Author: Roy Storey (rds@sanger.ac.uk)
 *  Copyright (c) 2006-2017: Genome Research Ltd.
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
