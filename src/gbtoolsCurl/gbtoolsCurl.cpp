/*  File: libcurlobject.c
 *  Author: Roy Storey (rds@sanger.ac.uk)
 *  Copyright (c) 2006-2017: Genome Research Ltd.
 *  Copyright [2018-2021] EMBL-European Bioinformatics Institute
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
 * Description: Object to access curl send/receive http requests.
 *
 *-------------------------------------------------------------------
 */

#include <unistd.h>

#include <gbtoolsCurl_P.hpp>


namespace gbtools
{


#ifdef _WIN32
#define SHORT_SLEEP Sleep(100)
#else
#define SHORT_SLEEP usleep(100000)
#endif

enum
  {
    CURLOBJECT_PROPID = 0,		/* zero is invalid prop id */
    CURLOBJECT_LAST_CURLOPT = CURLOPT_LASTENTRY,
    CURLOBJECT_MANAGE_POSTFIELDS,
    CURLOBJECT_ALLOW_QUEUES,
    CURLOBJECT_ALLOW_REUSE_UNCHANGED,
    CURLOBJECT_RESPONSE_CODE,
    CURLOBJECT_WAIT
  };

typedef struct
{
  CURLoption  option;
  GValue      value;
  GParamSpec *pspec{NULL};
}curl_settings_struct, *curl_settings;

typedef struct
{
  GList   *settings_list{NULL};
  gboolean use_multi{FALSE};
  gboolean perform_called{FALSE};
}curl_settings_perform_struct, *curl_settings_perform;

/* object implementation functions */
static void curl_object_class_init(CURLObjectClass curlobj_class);
#ifdef NEVER_GETS_CALLED_FOR_STATIC_TYPES
static void curl_object_class_finalize(CURLObjectClass curlobj_class);
#endif /* NEVER_GETS_CALLED_FOR_STATIC_TYPES */
static void curl_object_init(CURLObject curl_object);
static void curl_object_dispose(GObject *gobject);
static void curl_object_finalize(GObject *gobject);
static void curl_object_set_property(GObject *gobject, 
				  guint param_id, 
				  const GValue *value, 
				  GParamSpec *pspec);
static void curl_object_get_property(GObject *gobject, 
				  guint param_id, 
				  GValue *value, 
				  GParamSpec *pspec);

/* internal functions */
static gboolean _curl_status_ok(GObject *object);
static gboolean curl_fd_to_watched_GIOChannel(gint         fd, 
					      GIOCondition cond, 
					      GIOFunc      func, 
					      gpointer     data);
static gboolean curl_object_watch_func(GIOChannel  *source, 
				   GIOCondition condition, 
				   gpointer     user_data);
static void run_multi_perform(CURLObject curl_object);
static CURLObjectStatus perform_later(CURLObject curl_object, gboolean use_multi);
static void save_settings(CURLObject    curl_object, 
			  guint         param_id,
			  const GValue *value,
			  GParamSpec   *pspec);
static void invoke_set(gpointer list_data, gpointer user_data);
static void perform_next(CURLObject curl_object);

static void transfer_finished_notify(gpointer user_data);
static void destroy_list_item(gpointer list_data, gpointer unused_data);
static gpointer destroy_settings_perform(curl_settings_perform details);
static void invoke_destroy_settings_perform(gpointer q_data, gpointer unused_data);
static void free_pre717_strings(gpointer str_data, gpointer unused);

#ifdef NEEDS_PROGRESS
static int curl_object_progress_func(void  *clientp,
				 double dltotal,
				 double dlnow,
				 double ultotal,
				 double ulnow);
#endif /* NEEDS_PROGRESS */

/* Thread safety issue:
 *
 * Even I can see that the usual idiom of creating a class isn't thread safe.
 *
 * http://osdir.com/ml/gnome.gtk+.general/2003-07/msg00034.html
 * http://bugzilla.gnome.org/show_bug.cgi?id=69668
 * http://bugzilla.gnome.org/show_bug.cgi?id=64764
 *
 * http://testbit.eu/gitdata?p=glib.git;a=blob;f=gobject/tests/threadtests.c;h=3b955038ca9dc436be9487b33b1efbd92bab6ba8;hb=HEAD
 *
 * What people do currently when using GObject in threaded applications
 * is to call g_type_class_ref (MY_TYPE_WHATEVER) for all their types
 * before creating the first thread. 
 */

/* public functions */

/*!
 * Get GType for the CURLObj object
 */

GType CURLObjectGetType(void)
{
  static GType curl_object_type = 0;

  if(!curl_object_type)
    {
      GTypeInfo curl_info = 
	{
	  sizeof(curlObjectClass),
	  (GBaseInitFunc)     NULL,
	  (GBaseFinalizeFunc) NULL,
	  (GClassInitFunc) curl_object_class_init,
	  (GClassFinalizeFunc)NULL,// curl_object_class_finalize,	/* class_finalize */
	  NULL,			/* class_data */
	  sizeof(curlObject),
	  0,			/* n_preallocs */
	  (GInstanceInitFunc) curl_object_init
	};

      curl_object_type = g_type_register_static(G_TYPE_OBJECT,
						"cURL-wrapper-object", 
						&curl_info, (GTypeFlags)0);
    }

  return curl_object_type;
}

/*!
 * Get a new CURLObject 
 */
CURLObject CURLObjectNew()
{
  CURLObject curl_obj = NULL ;

  curl_obj = (CURLObject)g_object_new(CURL_TYPE_OBJECT, NULL);

  return curl_obj ;
}

/*!
 * Setup the object to do as you'd like.
 */
CURLObjectStatus CURLObjectSet(CURLObject curl_object, const gchar *first_arg_name, ...)
{
  CURLObjectStatus result = CURL_STATUS_FAILED;
  va_list args;

  va_start(args, first_arg_name);
  g_object_set_valist(G_OBJECT(curl_object), first_arg_name, args);
  va_end(args);

  if (curl_object->last_easy_status == CURLE_OK)
    result = CURL_STATUS_OK;
  else
    result = CURL_STATUS_FAILED;

  return result;
}

/*!
 * Do the transfer...
 */
CURLObjectStatus CURLObjectPerform(CURLObject curl_object, gboolean use_multi)
{
  CURLObjectStatus status = CURL_STATUS_FAILED;
  use_multi = 0; // current curl has a bug with multithreading, so we disable it for now;
  if(_curl_status_ok(G_OBJECT(curl_object)))
    {
      if(use_multi)
	{
#ifdef PROGRESS_INTERNAL
	  curl_easy_setopt(curl_object->easy, CURLOPT_PROGRESSFUNCTION, curl_object_progress_func);

	  curl_easy_setopt(curl_object->easy, CURLOPT_PROGRESSDATA, curl_object);
#endif /* PROGRESS_INTERNAL */

	  if(curl_object->transfer_in_progress)
	    {
	      if(curl_object->allow_queue)
		status = perform_later(curl_object, use_multi);
	      else
		status = (CURLObjectStatus)101; 	/* NO QUEUES and TRANSFER IN PROGRESS. */
	    }
	  else if(!g_queue_is_empty(curl_object->perform_queue))
	    {
	      status = perform_later(curl_object, use_multi);
	      perform_next(curl_object);
	    }
	  else
	    {
	      curl_object->last_multi_status = curl_multi_add_handle(curl_object->multi, 
								     curl_object->easy);
	      
	      run_multi_perform(curl_object);
	      
	      if(curl_object->last_easy_status == CURLE_OK)
		status = CURL_STATUS_OK;
	    }
	}
      else
	{
	  if((curl_object->last_easy_status = curl_easy_perform(curl_object->easy)) == CURLE_OK)
	    status = CURL_STATUS_OK;
	}
    }

  return status;
}

CURLObjectStatus CURLObjectErrorMessage(CURLObject curl_object, char **message)
{
  CURLObjectStatus result = CURL_STATUS_FAILED;

  if(message)
    {
      *message = NULL;
      if(curl_object->error_message)
	*message = g_strdup(curl_object->error_message);
    }

  if (curl_object->last_easy_status == CURLE_OK)
    result = CURL_STATUS_OK;
  else
    result = CURL_STATUS_FAILED;

  return result;
}

/*!
 * Cleanup after you're finished with the object.
 */
CURLObject CURLObjectDestroy(CURLObject curl_object)
{
  g_object_unref(G_OBJECT(curl_object));

  return NULL;
}

/* Internal functions. object implementation functions first */

static void curl_object_class_init(CURLObjectClass curl_object_class)
{
  GObjectClass *gobject_class;
  
  gobject_class = (GObjectClass *)curl_object_class;

  gobject_class->set_property = curl_object_set_property;
  gobject_class->get_property = curl_object_get_property;

  /* --- Behaviour options --- */
  g_object_class_install_property(gobject_class, CURLOPT_VERBOSE,
				  g_param_spec_boolean("verbose", "verbose",
						       "The verbose information will be sent to stderr, or the stream set with CURLOPT_STDERR",
						       TRUE, (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_VERBOSE,
				  g_param_spec_boolean("debug", "debug",
						       "The verbose information will be sent to stderr, or the stream set with CURLOPT_STDERR",
						       TRUE, (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_HEADER,
				  g_param_spec_boolean("header", "header",
						       "A non-zero parameter tells the library to include the header in the body output.",
						       TRUE, (GParamFlags)CURL_PARAM_STATIC_WO));
  /* NOPROGRESS & NOSIGNAL not supported */

  /* --- Callback options --- */
  g_object_class_install_property(gobject_class, CURLOPT_WRITEFUNCTION,
				  g_param_spec_pointer("writefunction", "writefunction",
						       "Function called when data has been received",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_WRITEDATA,
				  g_param_spec_pointer("writedata", "writedata",
						       "User data passed to writefunction",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_READFUNCTION,
				  g_param_spec_pointer("readfunction", "readfunction",
						       "Function called when data can be sent",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_READDATA,
				  g_param_spec_pointer("readdata", "readdata",
						       "User data passed to readfunction",
						       (GParamFlags)CURL_PARAM_STATIC_WO));

  /* CURLOPT_IOCTLFUNCTION, CURLOPT_IOCTLDATA, 
   * CURLOPT_SEEKFUNCTION, CURLOPT_SEEKDATA,
   * CURLOPT_SOCKOPTFUNCTION, CURLOPT_SOCKOPTDATA,
   * CURLOPT_OPENSOCKETFUNCTION, CURLOPT_OPENSOCKETDATA
   * not supported
   */
  g_object_class_install_property(gobject_class, CURLOPT_PROGRESSFUNCTION,
				  g_param_spec_pointer("progressfunction", "progressfunction",
						       "Function called when data has been received",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_PROGRESSDATA,
				  g_param_spec_pointer("progressdata", "progressdata",
						       "User data passed to progressfunction",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_HEADERFUNCTION,
				  g_param_spec_pointer("headerfunction", "headerfunction",
						       "Function called when header data has been received",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_HEADERDATA,
				  g_param_spec_pointer("headerdata", "headerdata",
						       "User data passed to headerfunction",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_DEBUGFUNCTION,
				  g_param_spec_pointer("debugfunction", "debugfunction",
						       "Function called when debug data has been received",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_DEBUGDATA,
				  g_param_spec_pointer("debugdata", "debugdata",
						       "User data passed to debugfunction",
						       (GParamFlags)CURL_PARAM_STATIC_WO));

  /* CURLOPT_SSL_CTX_FUNCTION, CURLOPT_SSL_CTX_DATA,
   * CURLOPT_CONV_TO_NETWORK_FUNCTION,
   * CURLOPT_CONV_FROM_NETWORK_FUNCTION,
   * CURLOPT_CONV_FROM_UTF8_FUNCTION not supported
   */

  /* --- Error options --- */
  /* support comes later...
   * CURLOPT_ERRORBUFFER (not for this one. This object sets it for it's own use)
   * CURLOPT_STDERR
   * CURLOPT_FAILONERROR
   */

  /* --- Network options --- */
  g_object_class_install_property(gobject_class, CURLOPT_URL,
				  g_param_spec_string("url", "URL",
						      "Uniform Resource Location",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_PORT,
				  g_param_spec_uint("port", "port",
						    "port",
						    80, 65535, 80, (GParamFlags)CURL_PARAM_STATIC_WO));

  /* A whole load of other network options need adding */

  /* --- Names & Passwords options --- */

  g_object_class_install_property(gobject_class, CURLOPT_USERPWD,
				  g_param_spec_string("userpwd", "userpwd",
						      "user:password",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));

  g_object_class_install_property(gobject_class, CURLOPT_USERNAME,
				  g_param_spec_string("username", "username",
						      "username",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));

  g_object_class_install_property(gobject_class, CURLOPT_PASSWORD,
				  g_param_spec_string("password", "password",
						      "password",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));


  /* HTTP Options */
  g_object_class_install_property(gobject_class, CURLOPT_AUTOREFERER,
				  g_param_spec_boolean("autoreferer", "autoreferer",
						       "Add Referer: to redirect requests",
						       FALSE, (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_ENCODING,
				  g_param_spec_string("encoding", "encoding",
						      "Add Accept-Encoding: [identity|deflate|gzip]",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_FOLLOWLOCATION,
				  g_param_spec_boolean("followlocation", "followlocation",
						       "Follow any Location: headers",
						       FALSE, (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_POST,
				  g_param_spec_boolean("post", "post",
						       "Regular POST requesting",
						       FALSE, (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_POSTFIELDS,
				  g_param_spec_pointer("postfields", "postfields",
						       "POST data to send",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_POSTFIELDSIZE,
				  g_param_spec_uint("postfieldsize", "postfieldsize",
						    "Size of the POSTFIELDS", 
						    0, 65535, 0,
						    (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_HTTPPOST,
				  g_param_spec_pointer("httppost", "httppost",
						       "List of curl_httppost structs",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_REFERER,
				  g_param_spec_string("referer", "referer", 
						      "Referer: string", 
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_USERAGENT,
				  g_param_spec_string("useragent", "useragent",
						      "User-Agent: string",
						      "libcurlobj-0.1",
						      (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_HTTPHEADER,
				  g_param_spec_pointer("httpheader", "httpheader",
						       "curl_slist list of header strings",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_HTTP200ALIASES,
				  g_param_spec_pointer("http200aliases", "http200aliases",
						       "curl_slist list of header 200 alias strings",
						       (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_COOKIE,
				  g_param_spec_string("cookie", "cookie",
						      "NAME=CONTENTS cookie string",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_COOKIEFILE,
				  g_param_spec_string("cookiefile", "cookiefile",
						      "Location of a Netscape/Mozilla format cookiefile.",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_COOKIEJAR,
				  g_param_spec_string("cookiejar", "cookiejar",
						      "Location for curl's cookiejar. Written when curl_easy_cleanup called",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_COOKIESESSION,
				  g_param_spec_long("cookiesession", "cookiesession",
						    "Start a new cookie session",
						    0, 1, 0, (GParamFlags)CURL_PARAM_STATIC_WO));
  g_object_class_install_property(gobject_class, CURLOPT_PROXY,
				  g_param_spec_string("proxy", "proxy",
						      "proxy",
						      "", (GParamFlags)CURL_PARAM_STATIC_WO));
#ifdef COOKIELIST
  g_object_class_install_property(gobject_class, CURLOPT_COOKIELIST,
				  g_param_spec_string("cookielist", "cookielist",
						      "cookie control",
						      "", CURL_PARAM_STATIC_WO));
#endif	/* COOKIELIST */
  g_object_class_install_property(gobject_class, CURLOPT_HTTPGET,
				  g_param_spec_boolean("httpget", "httpget",
						       "Force easy handle to use GET request",
						       TRUE, (GParamFlags)CURL_PARAM_STATIC_WO));
  /* --- FTP options --- */
  
  /* --- Protocol options --- */
  g_object_class_install_property(gobject_class, CURLOPT_IPRESOLVE,
				  g_param_spec_long("ipresolve", "ipresolve",
                                                    "Specify whether libcurl should use IPv4, IPv6, or either",
                                                    0, 2, CURL_IPRESOLVE_WHATEVER,
                                                    (GParamFlags)CURL_PARAM_STATIC_WO));

  g_object_class_install_property(gobject_class, CURLOPT_CAINFO,
				  g_param_spec_string("cainfo", "cainfo",
                                                      "Specify the location of the cainfo file libcurl should use",
                                                      NULL, (GParamFlags)CURL_PARAM_STATIC_WO));

  /* --- Connection options --- */
  g_object_class_install_property(gobject_class, CURLOPT_TIMEOUT,
				  g_param_spec_long("timeout", "timeout",
						    "Timeout (uses SIGALRM unless nosignal set).",
						    0, 65535, 0, (GParamFlags)CURL_PARAM_STATIC_WO));
  /* --- SSL options --- */

  /* --- SSH options --- */

  /* --- Other options --- */
  g_object_class_install_property(gobject_class, CURLOPT_CUSTOMREQUEST,
				  g_param_spec_string("customrequest", "customrequest",
                                                      "Custom request",
                                                      NULL, (GParamFlags)CURL_PARAM_STATIC_WO));

  /* --- Telnet options --- */

  /* --- This object Options --- */

  g_object_class_install_property(gobject_class, CURLOBJECT_MANAGE_POSTFIELDS,
				  g_param_spec_boolean("manage-postfields", "Manage Postfields",
						       "Free the POST data as soon as possible",
						       FALSE, (GParamFlags)CURL_PARAM_STATIC_RW));

  g_object_class_install_property(gobject_class, CURLOBJECT_ALLOW_QUEUES,
				  g_param_spec_boolean("allow-queues", "Allow queues",
						       "Allow reuse of the curl handle, by queuing requests",
						       FALSE, (GParamFlags)CURL_PARAM_STATIC_RW));

  /* CURLINFO data, only some implemented so far. _all_ Read-only */

  g_object_class_install_property(gobject_class, CURLOBJECT_RESPONSE_CODE,
				  g_param_spec_long("response-code", "response-code",
						    "The Response Code of the current request",
						    0, 599, 200, (GParamFlags)CURL_PARAM_STATIC_RO));

  /* Signals */
  curl_object_class->signals[CONNECTION_CLOSED_SIGNAL] =
    g_signal_new("connection_closed", 
		 G_TYPE_FROM_CLASS(curl_object_class),
		 G_SIGNAL_RUN_LAST,
		 G_STRUCT_OFFSET(curlObjectClass, connection_closed), /* class closure */
		 NULL, 		/* accumulator */
		 NULL, 		/* accumulator data */
		 g_cclosure_marshal_VOID__VOID, /* marshal */
		 G_TYPE_NONE,	/* return type */
		 0,	/* number of params */
		 G_TYPE_STRING,
		 G_TYPE_UINT,
		 G_TYPE_POINTER); /* param types */

  /* This function is not thread safe! */
  curl_object_class->global_init = curl_global_init(CURL_GLOBAL_ALL);

  gobject_class->dispose  = curl_object_dispose;
  gobject_class->finalize = curl_object_finalize;

  return ;
}

#ifdef NEVER_GETS_CALLED_FOR_STATIC_TYPES
static void curl_object_class_finalize(CURLObjectClass curl_object_class)
{
  /* Check it'll only get run once! */
  curl_global_cleanup();

  return ;
}
#endif

static void curl_object_init(CURLObject curl_object)
{
  CURLObjectClass obj_class = CURL_OBJECT_GET_CLASS(curl_object);

  curl_object->last_easy_status = obj_class->global_init;

  if(curl_object->last_easy_status == CURLE_OK)
    {
      /* Can carry on here... */
      if((curl_object->easy  = curl_easy_init()))
	curl_object->last_easy_status  = CURLE_OK;
      if((curl_object->multi = curl_multi_init()))
	curl_object->last_multi_status = CURLM_OK;
      /* We don't add the easy handle here, 
       * but do it dynamically later. */

      curl_easy_setopt(curl_object->easy, CURLOPT_NOPROGRESS, TRUE);
      
      //curl_easy_setopt(curl_object->easy, CURLOPT_NOSIGNAL, TRUE);

      curl_easy_setopt(curl_object->easy, CURLOPT_ERRORBUFFER, &(curl_object->error_message[0]));

      curl_object->perform_queue = g_queue_new();

      curl_object->allow_queue   = TRUE;

      curl_object->curl_version  = curl_version_info(CURLVERSION_NOW);
    }

  return ;
}

static void curl_object_dispose(GObject *gobject)
{
  GObjectClass *gobject_class;
  CURLObjectClass curl_class;

  curl_class = CURL_OBJECT_GET_CLASS(gobject);

  gobject_class = (GObjectClass *)g_type_class_peek_parent(curl_class);

  if(gobject_class->dispose)
    (* gobject_class->dispose)(gobject);

  return ;
}

static void curl_object_finalize(GObject *gobject)
{
  CURLObject curl_object = CURL_OBJECT(gobject);
  GObjectClass *gobject_class;
  CURLObjectClass curl_class;

  if(_curl_status_ok(gobject))
    {
      curl_multi_cleanup(curl_object->multi);
      curl_object->multi = NULL;
      curl_easy_cleanup(curl_object->easy);
      curl_object->easy  = NULL;
    }
  
  if((curl_object->curl_version) &&
     (curl_object->curl_version->version_num < 0x071700))
    g_list_foreach(curl_object->pre717strings, free_pre717_strings, NULL);
  
  if((curl_object->perform_queue) &&
     (!g_queue_is_empty(curl_object->perform_queue)))
    {
      g_queue_foreach(curl_object->perform_queue, 
		      invoke_destroy_settings_perform, 
		      NULL);
    }

  if(curl_object->perform_queue)
    g_queue_free(curl_object->perform_queue);
  curl_object->perform_queue = NULL;

  if(curl_object->settings_to_destroy)
    {
      curl_object->settings_to_destroy =
	(GObjectClass*)destroy_settings_perform((curl_settings_perform_struct *)(curl_object->settings_to_destroy));
    }

  if(curl_object->manage_post_data &&
     curl_object->post_data_2_free)
    {
      g_free(curl_object->post_data_2_free);
      curl_object->post_data_2_free = NULL ;
    }

  /* Chain up.... */
  curl_class = CURL_OBJECT_GET_CLASS(gobject);

  gobject_class = (GObjectClass*)g_type_class_peek_parent(curl_class);

  if(gobject_class->finalize)
    (* gobject_class->finalize)(gobject);

  return ;
}

static void curl_object_set_property(GObject      *gobject, 
				     guint         param_id, 
				     const GValue *value, 
				     GParamSpec   *pspec)
{
  CURLObject curl_object = NULL;

  if (!_curl_status_ok(gobject))
    g_warning("Bad curl status\n") ;

  g_return_if_fail(_curl_status_ok(gobject));

  curl_object = CURL_OBJECT(gobject);

  if(curl_object->transfer_in_progress)
    {
      if(curl_object->allow_queue)
	save_settings(curl_object, param_id, value, pspec);

      return ;
    }

  switch(param_id)
    {
    case CURLOPT_URL:
    case CURLOPT_VERBOSE:
    case CURLOPT_PORT:
    case CURLOPT_WRITEFUNCTION:
    case CURLOPT_WRITEDATA:
    case CURLOPT_READFUNCTION:
    case CURLOPT_READDATA:
    case CURLOPT_HEADERFUNCTION:
    case CURLOPT_HEADERDATA:
    case CURLOPT_POST:
    case CURLOPT_POSTFIELDSIZE:
    case CURLOPT_HTTPGET:
    case CURLOPT_HTTPHEADER:
    case CURLOPT_COOKIEFILE:
    case CURLOPT_PROXY:
    case CURLOPT_USERAGENT:
    case CURLOPT_IPRESOLVE:
    case CURLOPT_CAINFO:
    case CURLOPT_USERPWD:
    case CURLOPT_USERNAME:
    case CURLOPT_PASSWORD:
    case CURLOPT_CUSTOMREQUEST:
      if(G_IS_PARAM_SPEC_STRING(pspec))
	{
	  char *str;
	  str = (char *)g_value_get_string(value);

	  if(curl_object->debug == 1 && str)
	    g_message("Setting param '%d' to '%s'\n", param_id, str);

	  /* Need to know whether to copy the string */
	  if(curl_object->curl_version->version_num < 0x071700)
	    {
	      str = g_value_dup_string(value);
	      curl_object->pre717strings = g_list_append(curl_object->pre717strings, str);
	    }

	  curl_object->last_easy_status =
            curl_easy_setopt(curl_object->easy, (CURLoption)param_id, str);

	  if(curl_object->debug == 1)
	    g_message("Setting param '%d' to '%s'\n", param_id, str);
	}
      else if(G_IS_PARAM_SPEC_POINTER(pspec))
        {
          gpointer ptr = g_value_get_pointer(value) ;

	  if(curl_object->debug == 1)
	    g_message("Setting param '%d' to '%p'\n", param_id, ptr);

          curl_object->last_easy_status =
            curl_easy_setopt(curl_object->easy, (CURLoption)param_id, ptr);	
        }
      else if(G_IS_PARAM_SPEC_BOOLEAN(pspec))
	{
          gboolean val = g_value_get_boolean(value) ;

	  if(curl_object->debug == 1)
	    g_message("Setting param '%d' to '%d'\n", param_id, val);

	  curl_object->last_easy_status =
	    curl_easy_setopt(curl_object->easy, (CURLoption)param_id, val);
	  if(param_id == CURLOPT_VERBOSE)
	    curl_object->debug = g_value_get_boolean(value);
	}
      else if(G_IS_PARAM_SPEC_LONG(pspec))
        {
          long val = g_value_get_long(value) ;

	  if(curl_object->debug == 1)
	    g_message("Setting param '%d' to '%f'\n", param_id, (double)val);

          curl_object->last_easy_status =
            curl_easy_setopt(curl_object->easy, (CURLoption)param_id, val);
        }
      else if(G_IS_PARAM_SPEC_UINT(pspec))
        {
          uint val = g_value_get_uint(value) ;

	  if(curl_object->debug == 1)
	    g_message("Setting param '%d' to '%d'\n", param_id, val);

          curl_object->last_easy_status =
            curl_easy_setopt(curl_object->easy, (CURLoption)param_id, val);	
        }
      else
        {
          g_warning("Param id '%d' has unexpected ParamSpec.", param_id);
        }
      break;
    case CURLOPT_POSTFIELDS:
      if(G_IS_PARAM_SPEC_POINTER(pspec))
	{
          gpointer val = g_value_get_pointer(value) ;

	  if(curl_object->debug == 1)
	    g_message("Setting param '%d' to '%p'\n", param_id, val);

	  curl_object->last_easy_status =
	    curl_easy_setopt(curl_object->easy, (CURLoption)param_id, val);
	  curl_object->post_data_2_free = g_value_get_pointer(value);
	}
      else
        {
          g_warning("Param id '%d' has unexpected ParamSpec.\n", param_id);
        }
      break;
    case CURLOBJECT_MANAGE_POSTFIELDS:
      {
        curl_object->manage_post_data = g_value_get_boolean(value);
      }
      break;
    case CURLOBJECT_ALLOW_QUEUES:
      {
        curl_object->allow_queue = g_value_get_boolean(value);
      }
      break;
    default:
      {
        G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, param_id, pspec);
      }
      break;
    }

  if(curl_object->last_easy_status != CURLE_OK)
    g_warning("Setting property '%s' failed. (curl_easy_setopt code %d)\n", 
	      pspec->name, curl_object->last_easy_status);

  return ;
}

static void curl_object_get_property(GObject    *gobject, 
				     guint       param_id, 
				     GValue     *value, 
				     GParamSpec *pspec)
{
  CURLObject curl_object = CURL_OBJECT(gobject);

  g_return_if_fail(_curl_status_ok(gobject));

  switch(param_id)
    {
    case CURLOBJECT_ALLOW_QUEUES:
      g_value_set_boolean(value, curl_object->allow_queue);
      break;
    case CURLOBJECT_MANAGE_POSTFIELDS:
      g_value_set_boolean(value, curl_object->manage_post_data);
      break;
    case CURLOBJECT_RESPONSE_CODE:
      {
	long response_code;
	if(curl_easy_getinfo(curl_object->easy, 
			     CURLINFO_RESPONSE_CODE, 
			     &response_code) == CURLE_OK)
	  g_value_set_long(value, response_code);
      }
      break;
    default:
      G_OBJECT_WARN_INVALID_PROPERTY_ID(gobject, param_id, pspec);
      break;
    }

  return ;
}


static gboolean _curl_status_ok(GObject *gobject)
{
  CURLObject object;

  g_return_val_if_fail(CURL_IS_OBJECT(gobject), FALSE);

  object = CURL_OBJECT(gobject);

  return ((object->last_multi_status == CURLM_OK) &&
	  (object->last_easy_status  == CURLE_OK));
}

static gboolean curl_fd_to_watched_GIOChannel(gint         fd, 
					      GIOCondition cond, 
					      GIOFunc      func, 
					      gpointer     data)
{
  gboolean success = FALSE;
  GIOChannel *io_channel;
  
  if((io_channel = g_io_channel_unix_new(fd)))
    {
      GError *flags_error = NULL;
      GIOStatus status;
      
      if((status = g_io_channel_set_flags(io_channel, 
					  (GIOFlags)(G_IO_FLAG_NONBLOCK | g_io_channel_get_flags(io_channel)), 
					  &flags_error)) == G_IO_STATUS_NORMAL)
	{
#ifdef CURL_SET_ENCODING
	  g_io_channel_set_encoding(io_channel, "ISO8859-1", NULL);
#endif /* CURL_SET_ENCODING */
	  g_io_add_watch_full(io_channel, G_PRIORITY_DEFAULT, 
			      cond, func, data, 
			      transfer_finished_notify);
	  
	  success = TRUE;
	}
      else
	g_warning("%s", flags_error->message);
      
      g_io_channel_unref(io_channel); /* We don't need this anymore */
    }
  
  return success;
}

static gboolean curl_object_watch_func(GIOChannel  *source, 
				       GIOCondition condition, 
				       gpointer     user_data)
{
  CURLObject curl_object = CURL_OBJECT(user_data);
  int running_handles = 0 ;
  gboolean call_again = FALSE;
    
  if((condition & G_IO_OUT) ||
     (condition & G_IO_IN))
    {
      while((curl_object->last_multi_status = curl_multi_perform(curl_object->multi, &running_handles))
            == CURLM_CALL_MULTI_PERFORM)
        {
          if(running_handles < 1)
            {
              g_warning("%s\n", "curl_multi_perform requested to run again, but has no remaining handles");
              break ;
            }
        }

      if (running_handles > 0)
        call_again = TRUE ;
    }
  else if((condition & G_IO_HUP) ||
	  (condition & G_IO_ERR) ||
	  (condition & G_IO_NVAL))
    {
      g_warning("%s\n", "HUP, ERR or NVAL");
      call_again = FALSE;
    }
  else
    {
      g_warning("%s\n", "something else?");
      call_again = FALSE;
    }
    
  return call_again;
}

static void run_multi_perform(CURLObject curl_object)
{
  CURLMsg *easy_msg;
  int still_runnning = 0;
  bool got_handler = FALSE;
  gboolean try_again = FALSE;
  int max_tries = 100;
  int count = 0;

  g_return_if_fail(_curl_status_ok(G_OBJECT(curl_object)));

  do 
    {
      try_again = FALSE;
      ++count;

      do {
        curl_object->last_multi_status = curl_multi_perform(curl_object->multi, &still_runnning);
      } while (curl_object->last_multi_status == CURLM_CALL_MULTI_PERFORM);

      easy_msg = curl_multi_info_read(curl_object->multi, &still_runnning);

      if(easy_msg && easy_msg->easy_handle == curl_object->easy)
        curl_object->last_easy_status = easy_msg->data.result;

      if((curl_object->last_easy_status == CURLE_OK) &&
         (curl_object->last_multi_status != CURLM_CALL_MULTI_PERFORM))
        {
          GIOCondition write_cond = (GIOCondition)(G_IO_OUT | G_IO_HUP | G_IO_ERR | G_IO_NVAL);
          fd_set read_set, write_set, exc_set;
          int fd, fd_max;

          FD_ZERO(&read_set);
          FD_ZERO(&write_set);
          FD_ZERO(&exc_set);

          /* Stupid curl, why can't I get the fd to the current added job? */
          curl_object->last_multi_status = curl_multi_fdset(curl_object->multi, 
                                                            &read_set, &write_set, 
                                                            &exc_set, &fd_max);
          if(fd_max == -1) {
            g_warning("curl_multi_fdset returned fd_max as -1; trying again\n");
            SHORT_SLEEP;
            try_again = TRUE;
          }
          else 
            {
              for(fd = 0; fd <= fd_max; fd++)
                {
                  if (FD_ISSET(fd, &write_set))
                    {
                      curl_fd_to_watched_GIOChannel(fd, write_cond, 
                                                    curl_object_watch_func, 
                                                    curl_object);
                      curl_object->transfer_in_progress = got_handler = TRUE;
                    }
                }
            }
        }
    } while (try_again && count < max_tries);

  if(got_handler == FALSE)
    {
      curl_multi_remove_handle(curl_object->multi, curl_object->easy);
    }

  return ;
}

static CURLObjectStatus perform_later(CURLObject curl_object, gboolean use_multi)
{
  CURLObjectStatus status = CURL_STATUS_OK;
  curl_settings_perform details = NULL;

  if (!(details = (curl_settings_perform)g_queue_peek_tail(curl_object->perform_queue)))
    {
      /* use previous... 
       * This will retry the previous easy handle completely unchanged... */
      details = g_new0(curl_settings_perform_struct, 1);
      g_queue_push_head(curl_object->perform_queue, details);
    }

  details->use_multi = use_multi;
  details->perform_called = TRUE;

  return status;
}

static void save_settings(CURLObject    curl_object, 
			  guint         param_id,
			  const GValue *value,
			  GParamSpec   *pspec)
{
  curl_settings_perform details = NULL;
  curl_settings this_detail = NULL;

  if(g_queue_is_empty(curl_object->perform_queue))
    {
      details = g_new0(curl_settings_perform_struct, 1);
      g_queue_push_head(curl_object->perform_queue, details);
    }
  else
    {
      details = (curl_settings_perform)g_queue_peek_tail(curl_object->perform_queue);
    }

  if((this_detail = g_new0(curl_settings_struct, 1)))
    {
      this_detail->option = (CURLoption)param_id;
      this_detail->pspec  = g_param_spec_ref(pspec);

      g_value_init(&(this_detail->value), 
		   G_PARAM_SPEC_VALUE_TYPE(pspec));
      g_value_copy(value, &(this_detail->value));

      details->settings_list = g_list_append(details->settings_list, 
					     this_detail);
    }
  
  return ;
}

static void invoke_set(gpointer list_data, gpointer user_data)
{
  CURLObject curl_object = CURL_OBJECT(user_data);
  curl_settings setting = (curl_settings)list_data;
  CURLObjectClass curl_class;
  GObjectClass *gobject_class;

  curl_class    = CURL_OBJECT_GET_CLASS(curl_object);
  gobject_class = G_OBJECT_CLASS(curl_class);

  (gobject_class->set_property)(G_OBJECT(curl_object), 
				setting->option,
				&(setting->value),
				setting->pspec);

  return ;
}

static void perform_next(CURLObject curl_object)
{
  curl_settings_perform details = NULL;

  if((curl_object->transfer_in_progress == FALSE) &&
     (details = (curl_settings_perform)g_queue_pop_head(curl_object->perform_queue)) &&
     (details->perform_called == TRUE))
    {
      g_list_foreach(details->settings_list, invoke_set, curl_object);
      CURLObjectPerform(curl_object, details->use_multi);
      curl_object->settings_to_destroy = details;
    }

  return;
}

static void transfer_finished_notify(gpointer user_data)
{
  CURLObject curl_object = NULL;

  curl_object = CURL_OBJECT(user_data);

  if(curl_object->settings_to_destroy)
    {
      curl_object->settings_to_destroy =
	destroy_settings_perform((curl_settings_perform)(curl_object->settings_to_destroy));
    }

  curl_object->transfer_in_progress = FALSE;

  curl_object->last_multi_status = 
    curl_multi_remove_handle(curl_object->multi,
			     curl_object->easy);


  g_signal_emit(G_OBJECT(curl_object),
		CURL_OBJECT_GET_CLASS(curl_object)->signals[CONNECTION_CLOSED_SIGNAL],
		0, NULL);

  if(curl_object->perform_queue)
    perform_next(curl_object);

  return ;
}

/* Clean up functions for the curl_settings_perform alloactions */
static void destroy_list_item(gpointer list_data, gpointer unused_data)
{
  curl_settings destroy_me = (curl_settings)list_data;

  g_param_spec_unref(destroy_me->pspec);

  /* I have a feeling this is going to miss clearing up pointers... */

  g_value_unset(&(destroy_me->value));

  g_free(destroy_me);

  return ;
}

static gpointer destroy_settings_perform(curl_settings_perform details)
{
  details->perform_called = FALSE;

  g_list_foreach(details->settings_list, destroy_list_item, NULL);
  
  g_list_free(details->settings_list);

  g_free(details);

  return NULL;
}

static void invoke_destroy_settings_perform(gpointer q_data, gpointer unused_data)
{
  destroy_settings_perform((curl_settings_perform)q_data);

  return ;
}

static void free_pre717_strings(gpointer str_data, gpointer unused)
{
  g_free(str_data);

  return ;
}

#ifdef NEEDS_PROGRESS
static int curl_object_progress_func(void *clientp,
				     double dltotal,
				     double dlnow,
				     double ultotal,
				     double ulnow)
{
  CURLObject curl_object = NULL;

  curl_object = CURL_OBJECT(clientp);

  g_warning("Downloaded %f of %f", dlnow, dltotal);

  return 0;
}
#endif /* NEEDS_PROGRESS */





} /* gbtools namespace */

