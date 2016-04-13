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

#include <gbtools/gbtoolsTrackhub.hpp>
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




} // unnamed namespace



namespace gbtools
{

namespace trackhub
{


Registry::Registry()
{
  /* Set up curl objects for get requests */
  curl_object_get_ = CURLObjectNew();

  struct curl_slist *get_headers_ = NULL;
  get_headers_ = curl_slist_append(get_headers_, "Accept: application/json");  
  get_headers_ = curl_slist_append(get_headers_, "Content-Type: application/json");
  get_headers_ = curl_slist_append(get_headers_, "charsets: utf-8");
 
  CURLObjectSet(curl_object_get_, 
                "post",           FALSE,
                "httpheader",     get_headers_, 
                "writefunction",  curlWriteDataCB,
                NULL);


  /* Set up curl objects for post requests */
  curl_object_post_ = CURLObjectNew();

  struct curl_slist *post_headers_ = NULL;
  post_headers_ = curl_slist_append(post_headers_, "Accept: application/json");  
  post_headers_ = curl_slist_append(post_headers_, "Content-Type: application/json");
  post_headers_ = curl_slist_append(post_headers_, "charsets: utf-8"); 

#ifdef USE_CHUNKED
  post_headers_ = curl_slist_append(post_headers_, "Transfer-Encoding: chunked:");
#endif

#ifdef DISABLE_EXPECT
  post_headers_ = curl_slist_append(post_headers_, "Expect:");
#endif

  CURLObjectSet(curl_object_post_, 
                "post",           TRUE,
                "httpheader",     post_headers_, 
                "writefunction",  curlWriteDataCB,
                "readfunction",   curlReadDataCB,
                NULL);
}


Registry::~Registry()
{
  /* Clean up */
  if (get_headers_)
    curl_slist_free_all(get_headers_);

  if (post_headers_)
    curl_slist_free_all(post_headers_);
}


/* Does the work to send the given GET request using CURL */
string Registry::getRequest(const string &url)
{   
  string result("");

  CURLObjectSet(curl_object_get_,
                "url",   url.c_str(),
                "writedata",      &result,
                NULL);

  CURLObjectStatus res = CURLObjectPerform(curl_object_get_, FALSE);
  
  if (res == CURL_STATUS_FAILED)
    cout << "CURL perform failed" << endl;

  return result;
}

/* Does the work to send the given POST request using CURL */
string Registry::postRequest(const string &url, const string &postfields)
{   
  string result("");

  CurlReadData read_data(postfields.c_str());

  CURLObjectSet(curl_object_post_,
                "url",   url.c_str(),
                "writedata",      &result,
                "readdata",       &read_data,
                NULL);

#ifndef USE_CHUNKED
  CURLObjectSet(curl_object_post_,
                "postfieldsize", read_data.sizeleft,
                NULL);
#endif

  CURLObjectStatus res = CURLObjectPerform(curl_object_post_, FALSE);
  
  if (res == CURL_STATUS_FAILED)
    cout << "CURL perform failed" << endl;

  return result;
}


string Registry::getVersion()
{
  string result("");

  string buffer = getRequest("https://www.trackhubregistry.org/api/info/version");

  Json::Value js;
  Json::Reader reader;
  bool ok = reader.parse(buffer, js);

  if (js["release"].isString())
    result = js["release"].asString();

  return result;
}




} // namespace trackhub




// Temp function to allow testing of trackhub functions
void testTrackhub()
{
  cout << "Testing" << endl;

  trackhub::Registry registry;
  cout << registry.getVersion() << endl;

  //string species = downloadJSONCurl("https://www.trackhubregistry.org/api/info/species");
  //string assemblies = downloadJSONCurl("https://www.trackhubregistry.org/api/info/assemblies");
  ////string trackhubs = downloadJSONCurl("https://www.trackhubregistry.org/api/info/trackhubs");
  //
  //string search_result = downloadJSONCurl("https://www.trackhubregistry.org/api/search",
  //                                        "{\"query\": \"mouse\"}");
  //
  //string track_search = downloadJSONCurl("https://www.trackhubregistry.org/api/search/trackdb/AVOEP91mYAv0XSJwlEPl");


}

} // namespace gbtools
