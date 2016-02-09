/*  File: libpfetch_I.h
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
 * Description: Internal header for structs etc for the pfetch object.
 *
 *-------------------------------------------------------------------
 */
#ifndef __LIBPFETCH_I_H__
#define __LIBPFETCH_I_H__

#include <gbtools/gbtoolsPfetch.hpp>



#define PFETCH_PARAM_STATIC (G_PARAM_STATIC_NAME | G_PARAM_STATIC_NICK | G_PARAM_STATIC_BLURB)
#define PFETCH_PARAM_STATIC_RW (PFETCH_PARAM_STATIC | G_PARAM_READWRITE)
#define PFETCH_PARAM_STATIC_RO (PFETCH_PARAM_STATIC | G_PARAM_READABLE)

#define PFETCH_READ_SIZE 80


typedef struct
{
  guint    watch_id;
  GPid     watch_pid;
  gpointer watch_data;
}ChildWatchDataStruct, *ChildWatchData;



typedef enum
  {
    PFETCH_PIPE,
    PFETCH_HTTP
  }PFetchMethod;

enum
  {
    PROP_0,			/* Zero is invalid property id! */
    PFETCH_FULL,
    PFETCH_ARCHIVE,
    PFETCH_DEBUG,
    PFETCH_LOCATION,
    PFETCH_PORT,
    PFETCH_UNIPROT_ISOFORM_SEQ,
    PFETCH_ONE_SEQ_PER_LINE,
    PFETCH_DNA_LOWER_AA_UPPER,
    PFETCH_BLIXEM_STYLE,
    /* http specific stuff */
    PFETCH_COOKIE_JAR,
    PFETCH_PROXY,
    PFETCH_IPRESOLVE,
    PFETCH_CAINFO,
    PFETCH_VERBOSE,
    PFETCH_URL,			/* same as location */
    PFETCH_POST,
    PFETCH_WRITE_FUNC,
    PFETCH_WRITE_DATA,
    PFETCH_HEADER_FUNC,
    PFETCH_HEADER_DATA,
    PFETCH_READ_FUNC,
    PFETCH_READ_DATA,
    PFETCH_TRANSFER_CHUNKED
  };


enum
  {
    HANDLE_READER_SIGNAL,
    HANDLE_ERROR_SIGNAL,
    HANDLE_WRITER_SIGNAL,
    HANDLE_CLOSED_SIGNAL,
    HANDLE_LAST_SIGNAL
  };


/* 
 * BASE
 */

typedef struct _pfetchHandleClassStruct
{
  GObjectClass parent_class;

  /* methods */
  PFetchStatus (* fetch)(PFetchHandle handle, char *sequence, GError **error);

  /* signals */
  PFetchStatus (* reader)(PFetchHandle handle, char *output, guint *output_size, GError *error);

  PFetchStatus (* error) (PFetchHandle handle, char *output, guint *output_size, GError *error);

  PFetchStatus (* writer)(PFetchHandle handle, char *input,  guint *input_size,  GError *error);

  PFetchStatus (* closed)(void);

  /* signal ids... should they be here? */
  guint handle_signals[HANDLE_LAST_SIGNAL];

} pfetchHandleClassStruct;


typedef struct _pfetchHandleStruct
{
  GObject __parent__;

  char *location;

  struct
  {
    unsigned int full    : 1;	/* -F */
    unsigned int archive : 1;	/* -A */
    unsigned int debug   : 1;	/* internal debug */
    unsigned int isoform_seq : 1; /* internal option */
    unsigned int one_per_line : 1; /* -q */
    unsigned int dna_PROTEIN : 1; /* -C */
  }opts;

} pfetchHandleStruct;




/* 
 * PIPE
 */

typedef struct _pfetchHandlePipeClassStruct
{
  pfetchHandleClass parent_class;

} pfetchHandlePipeClassStruct;


typedef struct _pfetchHandlePipeStruct
{
  pfetchHandle __parent__;

  guint stdin_source_id;
  guint stdout_source_id;
  guint stderr_source_id;
  guint timeout_source_id;

  ChildWatchDataStruct watch_data;
} pfetchHandlePipeStruct;



/* 
 * HTTP
 */

typedef struct _pfetchHandleHttpClassStruct
{
  pfetchHandleClass parent_class;

} pfetchHandleHttpClassStruct;

typedef struct _pfetchHandleHttpStruct
{
  pfetchHandle __parent__;

  CURLObject curl_object;

  char *post_data;
  char *cookie_jar_location;
  char *proxy;
  char *cainfo;
  unsigned int http_port;
  gboolean debug;
  long ipresolve;

  unsigned int request_counter;
} pfetchHandleHttpStruct;





PFetchStatus emit_signal(PFetchHandle handle,
			 guint        signal_id,
			 GQuark       detail,
			 char        *text, 
			 guint       *text_size, 
			 GError      *error);


#endif /* __LIBPFETCH_I_H__ */
