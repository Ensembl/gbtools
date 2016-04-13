/*  File: gbtoolsTrackhub.cpp
 *  Author: Gemma Barson (gb10@sanger.ac.uk)
 *  Copyright (c) 2016: Genome Research Ltd.
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
 * Description: Object to access the Ensembl Track Hub Registry
 *
 *-------------------------------------------------------------------
 */

#include <iostream>
#include <string>
#include <curl/curl.h>

#include <json/json.h>

#include <gbtools/gbtoolsCurl.hpp>


using namespace std;
using namespace gbtools;


namespace // unnamed namespace
{

class CurlReadData
{
public:
  CurlReadData(): readptr(NULL), sizeleft(0.0) 
  {
  };

  CurlReadData(const char *data) 
  {
    readptr = data; 
    sizeleft = strlen(data);
  };

  const char *readptr;
  long sizeleft;
};


int curlWriteDataCB(char *data, size_t size, size_t nmemb, string *buffer)
{
  int size_written = size * nmemb;

  if (buffer && data)  
    {
      buffer->append(data, size * nmemb);
    }

  return size_written;
}   

size_t curlReadDataCB(char *data, size_t size, size_t nmemb, CurlReadData *read_data)
{
  int size_written = 0;

  if (size * nmemb > 0 && read_data && read_data->sizeleft > 0 && data)
    {
      *data = read_data->readptr[0]; /* copy one single byte */
      read_data->readptr++;          /* advance pointer */
      read_data->sizeleft--;         /* we return 1 byte at a time! */
      size_written = 1;
    }

  return size_written;
}


string downloadJSONCurl(const string &url, const string &postfields = "")
{   
  string result("");

  CURLObject curl_object = CURLObjectNew();

  if (curl_object) 
    {
      bool post = (postfields.length() > 0);
      CurlReadData read_data(postfields.c_str());

      struct curl_slist *headers = NULL;
      headers = curl_slist_append(headers, "Accept: application/json");  
      headers = curl_slist_append(headers, "Content-Type: application/json");
      headers = curl_slist_append(headers, "charsets: utf-8"); 

      if (post)
        {
#ifdef USE_CHUNKED
          headers = curl_slist_append(headers, "Transfer-Encoding: chunked:");
#endif

#ifdef DISABLE_EXPECT
          headers = curl_slist_append(headers, "Expect:");
#endif
        }

      CURLObjectSet(curl_object,
                    /* general settings */
                    "post",  post,
                    "url",   url.c_str(),
                    //"proxy", "localhost:3128",
                    /* request */
                    /* functions */
                    "writefunction",  curlWriteDataCB,
                    "writedata",      &result,
                    "httpheader",     headers,
                    /* end of options */
                    NULL);

      if (post)
        {
          CURLObjectSet(curl_object,
                        /* general settings */
                        "post",  TRUE,
                        /* functions */
                        "readfunction",  curlReadDataCB,
                        "readdata",      &read_data,
                        /* end of options */
                        NULL);

#ifndef USE_CHUNKED
          CURLObjectSet(curl_object,
                        "postfieldsize", read_data.sizeleft,
                        NULL);
#endif
        }

      CURLObjectStatus res = CURLObjectPerform(curl_object, FALSE);

      if (res == CURL_STATUS_FAILED)
        cout << "CURL perform failed" << endl;

      /* Clean up */
      if (headers)
        curl_slist_free_all(headers);
    }

  return result;
}


} // unnamed namespace


namespace gbtools
{

// Temp function to allow testing of trackhub functions
void testTrackhub()
{
  cout << "Testing" << endl;

  string version = downloadJSONCurl("https://www.trackhubregistry.org/api/info/version");
  string species = downloadJSONCurl("https://www.trackhubregistry.org/api/info/species");
  string assemblies = downloadJSONCurl("https://www.trackhubregistry.org/api/info/assemblies");
  //string trackhubs = downloadJSONCurl("https://www.trackhubregistry.org/api/info/trackhubs");

  string search_result = downloadJSONCurl("https://www.trackhubregistry.org/api/search",
                                          "{\"query\": \"mouse\"}");

  string track_search = downloadJSONCurl("https://www.trackhubregistry.org/api/search/trackdb/AVOEP91mYAv0XSJwlEPl");


}

} // gbtools namespace
