/*  File: libpfetch.h
 *  Author: Roy Storey (rds@sanger.ac.uk)
 *  Copyright (c) 2006-2015: Genome Research Ltd.
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
 * Description: Object implementing fetching of sequence data via
 *              the pfetch server either through a pipe to the
 *              pfetch command line script or through http requests.
 *
 *-------------------------------------------------------------------
 */
#ifndef __LIBPFETCH_H__
#define __LIBPFETCH_H__

#include <glib.h>
#include <glib-object.h>

#include <gbtools/gbtoolsCurl.hpp>


namespace gbtools
{


typedef enum
  {
    PFETCH_STATUS_OK,
    PFETCH_STATUS_FAILED
  } PFetchStatus;



/*
 * Base Pfetch object
 */

typedef struct _pfetchHandleClassStruct  pfetchHandleClass, *PFetchHandleClass ;
typedef struct _pfetchHandleStruct  pfetchHandle, *PFetchHandle ;

GType PFetchHandleGetType(void) ;

#define PFETCH_TYPE_HANDLE            (PFetchHandleGetType())
#define PFETCH_HANDLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PFETCH_TYPE_HANDLE, pfetchHandle))
#define PFETCH_HANDLE_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST((obj), PFETCH_TYPE_HANDLE, pfetchHandle const))
#define PFETCH_HANDLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  PFETCH_TYPE_HANDLE, pfetchHandleClass))
#define PFETCH_IS_HANDLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PFETCH_TYPE_HANDLE))
#define PFETCH_IS_HANDLE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),  PFETCH_TYPE_HANDLE))
#define PFETCH_HANDLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  PFETCH_TYPE_HANDLE, pfetchHandleClass))



/* 
 * Derived Pfetch object using pipes to command line pfetch
 */

typedef struct _pfetchHandlePipeClassStruct  pfetchHandlePipeClass, *PFetchHandlePipeClass ;
typedef struct _pfetchHandlePipeStruct  pfetchHandlePipe, *PFetchHandlePipe ;

GType PFetchHandlePipeGetType(void) ;

#define PFETCH_TYPE_PIPE_HANDLE            (PFetchHandlePipeGetType())
#define PFETCH_PIPE_HANDLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PFETCH_TYPE_PIPE_HANDLE, pfetchHandlePipe))
#define PFETCH_PIPE_HANDLE_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST((obj), PFETCH_TYPE_PIPE_HANDLE, pfetchHandlePipe const))
#define PFETCH_PIPE_HANDLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  PFETCH_TYPE_PIPE_HANDLE, pfetchHandlePipeClass))
#define PFETCH_IS_PIPE_HANDLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PFETCH_TYPE_PIPE_HANDLE))
#define PFETCH_IS_PIPE_HANDLE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),  PFETCH_TYPE_PIPE_HANDLE))
#define PFETCH_PIPE_HANDLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  PFETCH_TYPE_PIPE_HANDLE, pfetchHandlePipeClass))



/* 
 * Derived Pfetch object using direct http requests to pfetch server
 */

typedef struct _pfetchHandleHttpClassStruct  pfetchHandleHttpClass, *PFetchHandleHttpClass ;
typedef struct _pfetchHandleHttpStruct  pfetchHandleHttp, *PFetchHandleHttp ;

GType PFetchHandleHttpGetType(void) ;

#define PFETCH_TYPE_HTTP_HANDLE            (PFetchHandleHttpGetType ())
#define PFETCH_HTTP_HANDLE(obj)            (G_TYPE_CHECK_INSTANCE_CAST((obj), PFETCH_TYPE_HTTP_HANDLE, pfetchHandleHttp))
#define PFETCH_HTTP_HANDLE_CONST(obj)      (G_TYPE_CHECK_INSTANCE_CAST((obj), PFETCH_TYPE_HTTP_HANDLE, pfetchHandleHttp const))
#define PFETCH_HTTP_HANDLE_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST((klass),  PFETCH_TYPE_HTTP_HANDLE, pfetchHandleHttpClass))
#define PFETCH_IS_HTTP_HANDLE(obj)         (G_TYPE_CHECK_INSTANCE_TYPE((obj), PFETCH_TYPE_HTTP_HANDLE))
#define PFETCH_IS_HTTP_HANDLE_CLASS(klass) (G_TYPE_CHECK_CLASS_CAST((klass),  PFETCH_TYPE_HTTP_HANDLE))
#define PFETCH_HTTP_HANDLE_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS((obj),  PFETCH_TYPE_HTTP_HANDLE, pfetchHandleHttpClass))



// Object functions available for all Pfetch object types.

PFetchHandle PFetchHandleNew(GType type);
PFetchStatus PFetchHandleSettings(PFetchHandle  pfetch, const gchar *first_arg_name, ...);
PFetchStatus PFetchHandleSettings_valist(PFetchHandle  pfetch, const gchar *first_arg_name, va_list args);
PFetchStatus PFetchHandleFetch(PFetchHandle  pfetch, char *sequence, GError **error);
char* PFetchHandleHttpGetError(PFetchHandle *pfetch) ;
PFetchHandle PFetchHandleDestroy(PFetchHandle  pfetch);



/*
 * Public utility functions...
 */

#define PFETCH_TYPE_HANDLE_STATUS          (PFetchHandleStatusGetType())

GType PFetchHandleStatusGetType (void);


} /* gbtools namespace */


#endif /* __LIBPFETCH_H__ */
