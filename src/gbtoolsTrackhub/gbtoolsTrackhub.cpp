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

using namespace gbtools::trackhub;

#define TRACKHUB_REGISTRY_HOST "https://www.trackhubregistry.org"

#define API_INFO_PING "/api/info/ping"
#define API_INFO_VERSION "/api/info/version"
#define API_INFO_SPECIES "/api/info/species"
#define API_INFO_ASSEMBLIES "/api/info/assemblies"
#define API_INFO_TRACKHUBS "/api/info/trackhubs"
#define API_SEARCH "/api/search"
#define API_SEARCH_TRACKDB "/api/search/trackdb"
#define API_LOGIN "/api/login"
#define API_LOGOUT "/api/logout"
#define API_TRACKHUB "/api/trackhub"
#define API_TRACKDB "/api/trackdb"



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


// Create a Track class for the track in the given json and add it to the given list.
// Recurse through any child tracks and add them to the new Track class's children.
// Also add its file type (if it has one) to the list of types.
void getTracks(Json::ValueIterator &iter, 
               list<Track> &track_list,
               list<string> &file_types)
{
  Json::Value js_track = *iter;

  Track track(js_track["track"].asString(),
              js_track["shortLabel"].asString(),
              js_track["bigDataUrl"].asString(),
              js_track["type"].asString()
              );

  track_list.push_back(track);

  if (js_track["type"].isString())
    file_types.push_back(js_track["type"].asString());

  // Recurse through any child tracks
  Json::Value js_children = js_track["members"];

  for (Json::ValueIterator child = js_children.begin(); child != js_children.end(); ++child)
    getTracks(child, track.children_, file_types);
}


} // unnamed namespace



namespace gbtools
{

namespace trackhub
{


Registry::Registry()
{
  host_ = TRACKHUB_REGISTRY_HOST;

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
  
  Json::Value js = getRequest(API_INFO_PING);

  if (js["ping"].isInt())
    result = js["ping"].asInt();

  return result;
}

// Get the Registry version number
string Registry::version()
{
  string result("");

  Json::Value js = getRequest(API_INFO_VERSION);

  if (js["release"].isString())
    result = js["release"].asString();

  return result;
}


// Get the list of species in the Registry
list<string> Registry::species()
{
  list<string> result;

  Json::Value js = getRequest(API_INFO_SPECIES);

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
  Json::Value js = getRequest(API_INFO_ASSEMBLIES);

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
  Json::Value js = getRequest(API_INFO_TRACKHUBS);
  return js;
}


// Search for the given search string and optional filters
list<TrackDb> Registry::search(const string &search_str,
                               const string &species,
                               const string &assembly,
                               const string &hub)
{
  list<TrackDb> result ;

  // Create a json-formatted message
  Json::Value payload_js;
  payload_js["query"] = search_str;
  payload_js["species"] = species;
  payload_js["assembly"] = assembly;
  payload_js["hub"] = hub;
  
  stringstream payload_ss;
  payload_ss << payload_js;
  
  Json::Value js = postRequest(API_SEARCH, payload_ss.str());

  // Loop through all items in the result and extract the trackDb IDs
  Json::Value items_js = js["items"];

  for (Json::ValueIterator iter = items_js.begin(); iter != items_js.end(); ++iter)
    {
      Json::Value item_js = *iter ;

      if (item_js["id"].isString())
        {
          TrackDb trackdb = searchTrackDb(item_js["id"].asString()) ;
          result.push_back(trackdb) ;
        }
    }

  return result ;
}


TrackDb Registry::searchTrackDb(const string &trackdb_id)
{
  stringstream query_ss;
  query_ss << API_SEARCH_TRACKDB << "/" << trackdb_id;

  Json::Value js = getRequest(query_ss.str());

  // Get the lists of track info and file types
  Json::Value tracks_js = js["configuration"];
  list<Track> tracks;
  list<string> file_types;

  for (Json::ValueIterator iter = tracks_js.begin(); iter != tracks_js.end(); ++iter)
    {
      getTracks(iter, tracks, file_types);
    }

  TrackDb trackdb(trackdb_id, 
                  js["hub"]["shortLabel"].asString(),
                  js["hub"]["longLabel"].asString(),
                  js["hub"]["url"].asString(),
                  js["species"]["scientific_name"].asString(),
                  js["assembly"]["name"].asString(),
                  js["type"].asString(),
                  js["status"]["tracks"]["total"].asInt(),
                  js["status"]["tracks"]["with_data"]["total"].asInt(),
                  file_types,
                  tracks
                  );

  return trackdb;
}


// Log in to the Registry
bool Registry::login(const string &user, const string &pwd)
{
  bool ok = false;

  CURLObjectSet(curl_object_get_,
                "username", user.c_str(), 
                "password", pwd.c_str(), 
                NULL);

  Json::Value js = getRequest(API_LOGIN);

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
bool Registry::logout()
{
  bool result = false;

  if (loggedIn())
    {
      // Do the request
      Json::Value js = getRequest(API_LOGOUT, true);

      // Check return value (should be a message saying logged out ok)
      if (js["message"].isString())
        {
          result = true;
          user_.clear();
          auth_token_.clear();
        }
      else
        {
          cout << "Error logging out" << endl;
        }
    }
  else
    {
      cout << "Cannot log out: not logged in" << endl ;
    }

  return result;
}


// Register a track hub as the current user
// assemblies: a map of assembly name in the hub to INSDC accessions
// type: "genomics", "epigenomics", "transcriptomics" or "proteomics"
Json::Value Registry::registerHub(const string &url,
                                  const map<string, string> &assemblies,
                                  const string &type,
                                  const bool is_public)
{
  Json::Value js;

  if (loggedIn())
    {
      // Create a json-formatted message
      Json::Value payload_js;
      payload_js["url"] = url;

      for (auto iter = assemblies.begin(); iter != assemblies.end(); ++iter)
        payload_js["assemblies"][iter->first] = iter->second;

      if (type.length() > 0)
        payload_js["type"] = type;

      if (is_public)
        payload_js["public"] = 1;
      else
        payload_js["public"] = 0;

      string payload = json_writer_.write(payload_js) ;

      // Do the request
      js = postRequest(API_TRACKHUB, payload, true);
    }
  else
    {
      cout << "Cannot register hub: not logged in" << endl ;
    }

  return js;
}


// Retrieve all trackDbs for the current users' registered hubs. Gets the trackhub with the
// given name, or all registered track hubs if no name is given.
list<TrackDb> Registry::retrieveHub(const string &trackhub)
{
  list<TrackDb> result;

  stringstream query_ss;
  query_ss << API_TRACKHUB;
  
  if (trackhub.length() > 0)
    query_ss << "/" << trackhub;

  // Do the request
  Json::Value js = getRequest(query_ss.str(), true);
  
  // Loop through all items in the returned array
  for (Json::ValueIterator hub_iter = js.begin(); hub_iter != js.end(); ++hub_iter)
    {
      // Loop through all trackdbs in this hub
      Json::Value js_trackdbs = (*hub_iter)["trackdbs"];
      
      for (Json::ValueIterator trackdb_iter = js_trackdbs.begin(); trackdb_iter != js_trackdbs.end(); ++trackdb_iter)
        {
          string uri = (*trackdb_iter)["uri"].asString();

          // Chop off the host etc. to get the trackdb id at the end of the uri
          stringstream ss;
          ss << host_ << API_TRACKDB << "/" ;
          string host = ss.str();
          
          if (uri.length() >= host.length() &&
              strncmp(uri.c_str(), host.c_str(), host.length()) == 0)
            {
              const char *trackdb_id = uri.c_str() + host.length();
              TrackDb trackdb = searchTrackDb(trackdb_id);
              result.push_back(trackdb);
            }
        }
    }
  
  return result;
}


string Registry::deleteHub(const string &trackhub)
{
  string result;

  string query(API_TRACKHUB);
  query += trackhub;

  Json::Value js = deleteRequest(query, true);

  if (js["message"].isString())
    result = js["message"].asString();

  return result;
}

// Retrieve a registered trackdb.
Json::Value Registry::retrieveTrackDb(const string &trackdb)
{
  stringstream query_ss;
  query_ss << API_TRACKDB << "/" << trackdb;

  // Do the request
  Json::Value js = getRequest(query_ss.str(), true);

  return js;
}


Json::Value Registry::deleteTrackDb(const string &trackdb)
{
  stringstream query_ss;
  query_ss << API_TRACKDB << "/" <<  trackdb;

  Json::Value js = deleteRequest(query_ss.str(), true);

  return js;
}

bool Registry::loggedIn()
{
  return (!user_.empty() && !auth_token_.empty());
}


} // namespace trackhub

} // namespace gbtools
