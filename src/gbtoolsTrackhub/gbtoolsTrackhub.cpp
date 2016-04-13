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


//void printList(list<string> lst)
//{
//  for (auto &it : lst)
//    cout << it << ", ";
//}
//
//void printMap(map<string, list<string>> m)
//{
//  for (auto &iter : m)
//    {
//      cout << iter.first << ":  ";
//      printList(iter.second);
//      cout << endl;
//    }
//}



} // unnamed namespace



namespace gbtools
{

namespace trackhub
{


Registry::Registry()
{
  host_ = "https://www.trackhubregistry.org";

  // Set up curl objects for GET/POST requests. Set the properties here that will be 
  // the same for all requests.  */
  curl_object_get_ = CURLObjectNew();
  curl_object_post_ = CURLObjectNew();
  curl_object_delete_ = CURLObjectNew();
 
  CURLObjectSet(curl_object_get_, 
                "post",           FALSE,
                "writefunction",  curlWriteDataCB,
                NULL);

  CURLObjectSet(curl_object_post_, 
                "post",           TRUE,
                "writefunction",  curlWriteDataCB,
                "readfunction",   curlReadDataCB,
                NULL);

  CURLObjectSet(curl_object_delete_, 
                "post",           FALSE,
                "writefunction",  curlWriteDataCB,
                "customrequest", "DELETE",
                NULL);
}


Registry::~Registry()
{
}


// Return the headers for GET requests
curl_slist* Registry::getHeaders(const bool authorise)
{
  curl_slist *headers = NULL;

  headers = curl_slist_append(headers, "Accept: application/json");  
  headers = curl_slist_append(headers, "Content-Type: application/json");
  headers = curl_slist_append(headers, "charsets: utf-8");

  if (authorise)
    {
      if (user_.empty() || auth_token_.empty())
        {
          cout << "Not logged in" << endl;
        }
      else
        {
          // Set user and auth token in the headers
          string user_header = "User: " + user_;
          string auth_header = "Auth-Token: " + auth_token_;

          headers = curl_slist_append(headers, user_header.c_str());
          headers = curl_slist_append(headers, auth_header.c_str());
        }
    }

  return headers;
}


// Return the headers for POST requests
curl_slist* Registry::postHeaders(const bool authorise)
{
  curl_slist *headers = getHeaders(authorise);

#ifdef USE_CHUNKED
  headers = curl_slist_append(headers, "Transfer-Encoding: chunked:");
#endif

#ifdef DISABLE_EXPECT
  headers = curl_slist_append(headers, "Expect:");
#endif

  return headers;
}


// Does the work to send the given GET request using CURL
string Registry::doGetRequest(const string &url,
                              const bool authorise)
{   
  string result("");

  curl_slist *headers = getHeaders(authorise);

  CURLObjectSet(curl_object_get_,
                "url",            url.c_str(),
                "httpheader",     headers, 
                "writedata",      &result,
                NULL);

  CURLObjectStatus res = CURLObjectPerform(curl_object_get_, FALSE);
  
  if (res == CURL_STATUS_FAILED)
    cout << "CURL perform failed" << endl;

  // Clean up
  if (headers)
    curl_slist_free_all(headers);

  return result;
}

// Does the work to send the given DELETE request using CURL
string Registry::doDeleteRequest(const string &url,
                                 const bool authorise)
{   
  string result("");

  curl_slist *headers = getHeaders(authorise);

  CURLObjectSet(curl_object_delete_,
                "url",            url.c_str(),
                "httpheader",     headers, 
                "writedata",      &result,
                NULL);

  CURLObjectStatus res = CURLObjectPerform(curl_object_delete_, FALSE);
  
  if (res == CURL_STATUS_FAILED)
    cout << "CURL perform failed" << endl;

  // Clean up
  if (headers)
    curl_slist_free_all(headers);

  return result;
}


// Does the work to send the given POST request using CURL
string Registry::doPostRequest(const string &url, 
                               const string &postfields,
                               const bool authorise)
{   
  string result("");

  CurlReadData read_data(postfields.c_str());
  curl_slist *headers = postHeaders(authorise);

  CURLObjectSet(curl_object_post_,
                "url",            url.c_str(),
                "httpheader",     headers, 
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

  // Clean up
  if (headers)
    curl_slist_free_all(headers);

  return result;
}


Json::Value Registry::getRequest(const string &request,
                                 const bool authorise)
{
  Json::Value js;
  
  string url = host_ + request;
  string buffer = doGetRequest(url, authorise);
  json_reader_.parse(buffer, js);

  return js;
}

Json::Value Registry::postRequest(const string &request,
                                  const string &postfields,
                                  const bool authorise)
{
  Json::Value js;
  
  string url = host_ + request;
  string buffer = doPostRequest(url, postfields, authorise);
  json_reader_.parse(buffer, js);

  return js;
}

Json::Value Registry::deleteRequest(const string &request,
                                    const bool authorise)
{
  Json::Value js;
  
  string url = host_ + request;
  string buffer = doDeleteRequest(url, authorise);
  json_reader_.parse(buffer, js);

  return js;
}


// Ping the Registry server. Returns true if ok.
bool Registry::ping()
{
  bool result = false;
  
  Json::Value js = getRequest("/api/info/ping");

  if (js["ping"].isInt())
    result = js["ping"].asInt();

  return result;
}

// Get the Registry version number
string Registry::version()
{
  string result("");

  Json::Value js = getRequest("/api/info/version");

  if (js["release"].isString())
    result = js["release"].asString();

  return result;
}


// Get the list of species in the Registry
list<string> Registry::species()
{
  list<string> result;

  Json::Value js = getRequest("/api/info/species");

  if (js.isArray())
    {
      for (const Json::Value &iter : js)
        {
          if (iter.isString())
            {
              result.push_back(iter.asString());
            }
          else
            {
              result.clear();
              break;
            }
        }
    }

  return result;
}

// Get the list of assemblies in the Registry per species name
map<string, list<string>> Registry::assemblies()
{
  map<string, list<string>> result;

  bool ok = true;
  Json::Value js = getRequest("/api/info/assemblies");

  for (Json::ValueIterator species_iter = js.begin(); species_iter != js.end(); ++species_iter)
    {
      Json::Value species_key = species_iter.key();
      Json::Value species_val = *species_iter;

      if (species_key.isString() && species_val.isArray())
        {
          list<string> assembly_list;

          for (const Json::Value &assembly_iter : species_val)
            {
              if (assembly_iter.isString())
                {
                  assembly_list.push_back(assembly_iter.asString());
                }
              else
                {
                  assembly_list.clear();
                  ok = false;
                  break;
                }
            }

          result[species_key.asString()] = assembly_list;
        }
      else
        {
          result.clear();
          ok = false;
          break;
        }
    }

  return result;
}


Json::Value Registry::trackhubs()
{
  Json::Value js = getRequest("/api/info/trackhubs");
  return js;
}


// Search for the given search string and optional filters
Json::Value Registry::search(const string &search_str,
                             const string &species,
                             const string &assembly,
                             const string &hub)
{
  // Create a json-formatted message
  Json::Value payload_js;
  payload_js["query"] = search_str;
  payload_js["species"] = species;
  payload_js["assembly"] = assembly;
  payload_js["hub"] = hub;
  
  stringstream payload_ss;
  payload_ss << payload_js;
  
  Json::Value js = postRequest("/api/search", payload_ss.str());

  return js;
}


Json::Value Registry::searchTrackDb(const string &trackdb)
{
  string query("/api/search/trackdb/");
  query += trackdb;

  Json::Value js = getRequest(query);
  return js;
}


// Log in to the Registry
bool Registry::login(const string &user, const string &pwd)
{
  bool ok = false;

  CURLObjectSet(curl_object_get_,
                "username", user.c_str(), 
                "password", pwd.c_str(), 
                NULL);

  Json::Value js = getRequest("/api/login");

  CURLObjectSet(curl_object_get_, 
                "username", NULL,
                "password", NULL,
                NULL);

  if (js["auth_token"].isString())
    {
      ok = true;
      user_ = user;
      auth_token_ = js["auth_token"].asString();
    }

  return ok;
}

// Log out of the Registry
string Registry::logout()
{
  string result;

  // Do the request
  Json::Value js = getRequest("/api/logout", true);

  // Check return value
  if (js["message"].isString())
    {
      result = js["message"].asString();
      user_.clear();
      auth_token_.clear();
    }
  else
    {
      cout << "Error logging out" << endl;
    }

  return result;
}


// Retrieve current users' registered track hubs
Json::Value Registry::retrieve()
{
  // Do the request
  Json::Value js = getRequest("/api/trackhub", true);

  return js;
}


// Register a track hub as the current user
// assemblies: a map of assembly name in the hub to INSDC accessions
// type: "genomics", "epigenomics", "transcriptomics" or "proteomics"
Json::Value Registry::registerHub(const string &url,
                                  const map<string, string> &assemblies,
                                  const string &type)
{
  // Create a json-formatted message
  Json::Value payload_js;
  payload_js["url"] = url;

  for (auto iter = assemblies.begin(); iter != assemblies.end(); ++iter)
    payload_js["assemblies"][iter->first] = iter->second;

  if (type.length() > 0)
    payload_js["type"] = type;

  stringstream payload_ss;
  payload_ss << payload_js;

  // Do the request
  Json::Value js = postRequest("/api/trackhub", payload_ss.str(), true);

  return js;
}


string Registry::deleteHub(const string &trackhub)
{
  string result;

  string query("/api/trackdb/");
  query += trackhub;

  Json::Value js = deleteRequest(query, true);

  if (js["message"].isString())
    result = js["message"].asString();

  return result;
}


} // namespace trackhub



void processTrack(Json::ValueIterator &iter, const int indent = 0)
{
  Json::Value js_track = *iter;

  stringstream ss;
  for (int i = 0; i < indent; ++i)
    ss << " ";

  cout << ss.str() << iter.key() << endl;

  if (js_track["bigDataUrl"] != Json::nullValue)
    cout << ss.str() << js_track["bigDataUrl"] << endl;

  Json::Value js_children = js_track["members"];

  for (Json::ValueIterator child = js_children.begin(); child != js_children.end(); ++child)
    processTrack(child, indent + 2);

  cout << endl;
}

// Temp function to allow testing of trackhub functions
void testTrackhub()
{
  cout << "Testing" << endl;

  trackhub::Registry registry;

  cout << "Ping: " << registry.ping() << endl;
  cout << "Version: " << registry.version() << endl;

  list<string> species = registry.species();
  //printList(species);

  map<string, list<string>> assemblies = registry.assemblies();
  //printMap(assemblies);

  Json::Value trackhubs = registry.trackhubs();
  //cout << trackhubs << endl;

  Json::Value mouse = registry.search("mouse");
  //cout << mouse << endl;

  Json::Value trackdb = registry.searchTrackDb("AVOEP91mYAv0XSJwlEPl");
  //cout << trackdb << endl;
  //Json::Value tracks = trackdb["configuration"];
  //stringstream indent;
  //for (Json::ValueIterator iter = tracks.begin(); iter != tracks.end(); ++iter)
  //  {
  //    processTrack(iter);
  //  }

//map<string, string> mymap;
//mymap["araTha1"] = "GCA_000001735.1";
//mymap["ricCom1"] = "GCA_000151685.2";
//mymap["braRap1"] = "GCA_000309985.1";
//registry.registerHub("http://genome-test.cse.ucsc.edu/~hiram/hubs/Plants/hub.txt", mymap);

}

} // namespace gbtools
