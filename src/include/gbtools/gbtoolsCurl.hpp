/*  File: libcurlobject.h
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
 * Description: Object providing an interface to the curl library.
 *
 *-------------------------------------------------------------------
 */
#ifndef GBTOOLS_CURL_H
#define GBTOOLS_CURL_H

#include <glib.h>
#include <glib-object.h>
#include <curl/curl.h>

namespace gbtools
{


#define CURL_TYPE_OBJECT            (CURLObjectGetType())
#define CURL_OBJECT(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), CURL_TYPE_OBJECT, curlObject))
#define CURL_OBJECT_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST((obj), CURL_TYPE_OBJECT, curlObject const))
#define CURL_OBJECT_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  CURL_TYPE_OBJECT, curlObjectClass))
#define CURL_IS_OBJECT(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), CURL_TYPE_OBJECT))
#define CURL_IS_OBJECT_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),  CURL_TYPE_OBJECT))
#define CURL_OBJECT_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  CURL_TYPE_OBJECT, curlObjectClass))


typedef enum
  {
    CURL_STATUS_OK,
    CURL_STATUS_FAILED
  } CURLObjectStatus;


/*  */
typedef struct _curlObjectStruct *CURLObject;

typedef struct _curlObjectStruct  curlObject;

/*  */
typedef struct _curlObjectClassStruct *CURLObjectClass;

typedef struct _curlObjectClassStruct  curlObjectClass;


GType CURLObjectGetType(void);

CURLObject       CURLObjectNew         (void);
CURLObjectStatus CURLObjectSet         (CURLObject curlobject, const gchar *first_arg_name, ...);
CURLObjectStatus CURLObjectPerform     (CURLObject curlobject, gboolean use_multi);
CURLObjectStatus CURLObjectErrorMessage(CURLObject curl_object, char **message);
CURLObject       CURLObjectDestroy     (CURLObject curlobject);


} /* gbtools namespace */

#endif	/* GBTOOLS_CURL_H */
