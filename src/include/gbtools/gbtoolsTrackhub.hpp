/*  File: gbtoolsTrackhub.hpp
 *  Author: Gemma Barson (gb10@sanger.ac.uk)
 *  Copyright (c) 2016: Genome Research Ltd.
 *-------------------------------------------------------------------
 * gbtools is free software; you can redistribute it and/or
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
 * This file is part of the gbtools genome browser tools library, 
 * written by
 *      Ed Griffiths      (Sanger Institute, UK)  <edgrif@sanger.ac.uk>
 *      Gemma Barson      (Sanger Institute, UK)  <gb10@sanger.ac.uk>
 *      Steve Miller      (Sanger Institute, UK)  <sm23@sanger.ac.uk>
 *
 * Description: Ensembl Track Hub Registry functions
 *----------------------------------------------------------------------------
 */

#ifndef GBTOOLS_TRACKHUB_H
#define GBTOOLS_TRACKHUB_H


#include <string>
#include <list>
#include <map>

#include <json/json.h>

#include <gbtools/gbtoolsCurl.hpp>



namespace gbtools
{
namespace trackhub
{

// Class to hold info about a track
class Track
{
public:
  Track(std::string name,
        std::string shortLabel,
        std::string url,
        std::string file_type,
        std::string visibility) 
    : name_(name),
      shortLabel_(shortLabel),
      url_(url),
      file_type_(file_type),
      visibility_(visibility)
  {};

  std::string name() const { return name_; };
  std::string description() const { return shortLabel_; };
  std::string url() const { return url_; };
  std::list<Track> children() const { return children_; };
  std::string visibility() const { return visibility_; };
  bool visible() const { return visibility_ != "hide"; };
  std::string fileType() const { return file_type_; };

  std::list<Track> children_;

private:
  std::string name_;
  std::string shortLabel_;
  std::string url_;
  std::string file_type_;
  std::string visibility_;
};


// Class to hold info about a trackDb (i.e. a collection of tracks from a particular hub)
class TrackDb
{
public:
  TrackDb()
    : id_(""), 
      hub_shortLabel_(""),
      hub_longLabel_(""),
      hub_url_(""),
      species_scientific_name_(""),
      assembly_name_(""),
      type_(""),
      num_tracks_(0),
      num_with_data_(0)
  {} ;

  TrackDb(const std::string &id, 
          const std::string &name,
          const std::string &shortLabel,
          const std::string &longLabel,
          const std::string &url,
          const std::string &scientific_name,
          const std::string &assembly_name,
          const std::string &type,
          const int num_tracks,
          const int num_with_data,
          const std::list<std::string> &file_types,
          const std::list<Track> &tracks
          ) 
    : id_(id), 
      hub_name_(name),
      hub_shortLabel_(shortLabel),
      hub_longLabel_(longLabel),
      hub_url_(url),
      species_scientific_name_(scientific_name),
      assembly_name_(assembly_name),
      type_(type),
      num_tracks_(num_tracks),
      num_with_data_(num_with_data),
      file_types_(file_types),
      tracks_(tracks)
  {} ;

  std::string id() const { return id_; } ;
  std::string name() const { return hub_name_; } ;
  std::string label() const { return hub_shortLabel_; } ;
  std::string description() const { return hub_longLabel_; } ;
  std::string url() const { return hub_url_; } ;
  std::string species() const { return species_scientific_name_; } ;
  std::string assembly() const { return assembly_name_; } ;
  int num_tracks() const { return num_tracks_; } ;
  int num_with_data() const { return num_with_data_; } ;
  std::list<std::string> file_types() const { return file_types_; } ;
  std::list<Track> tracks() const { return tracks_; } ;

private:
  std::string id_ ;
  std::string hub_name_ ;
  std::string hub_shortLabel_ ;
  std::string hub_longLabel_ ;
  std::string hub_url_ ;
  std::string species_scientific_name_ ;
  std::string assembly_name_ ;
  std::string type_ ;
  int num_tracks_;
  int num_with_data_;
  std::list<std::string> file_types_;
  std::list<Track> tracks_;
};


// Class to hold info about a track hub
class Trackhub
{
public:
  Trackhub() 
    : name_(""), 
      shortLabel_(""),
      longLabel_(""),
      url_("")
  {} ;

  Trackhub(std::string name,
           std::string shortLabel,
           std::string longLabel,
           std::string url) 
    : name_(""), 
      shortLabel_(""),
      longLabel_(""),
      url_("")
  {} ;

  std::string name() { return name_; } ;
  std::string label() { return shortLabel_; } ;
  std::string description() { return longLabel_; } ;
  std::string url() { return url_; } ;

private:
  std::string name_ ;
  std::string shortLabel_ ;
  std::string longLabel_ ;
  std::string url_ ;
} ;



/* Class for accessing the Ensembl Track Hub Registry API */
class Registry
{
public:

  //
  // Construct/destruct
  //
  Registry();
  ~Registry();

  void initCurl() ;

  //
  // API functions
  //
  bool ping(std::string &err_msg);
  std::string version(std::string &err_msg);
  std::list<std::string> species(std::string &err_msg);
  std::map<std::string, std::list<std::string>> assemblies(std::string &err_msg);
  std::list<Trackhub> trackhubs(std::string &err_msg);

  bool getSearchPage(std::stringstream &payload_ss,
                     const int page_no, std::list<TrackDb> &result,
                     std::string &err_msg);
  std::list<TrackDb> search(const std::string &query, const std::string &species,
                            const std::string &assembly, const std::string &hub,
                            std::string &err_msg);
  TrackDb searchTrackDb(const std::string &trackdb, std::string &err_msg);

  bool login(const std::string &user, const std::string &pwd, std::string &err_msg);
  bool logout(std::string &err_msg);

  Json::Value registerHub(const std::string &url, 
                          const std::map<std::string, std::string> &assemblies,
                          const std::string &type,
                          const bool is_public,
                          std::string &err_msg);
  std::list<TrackDb> retrieveHub(const std::string &trackhub, std::string &err_msg);
  std::string deleteHub(const std::string &trackhub, std::string &err_msg);

  Json::Value retrieveTrackDb(const std::string &trackdb, std::string &err_msg);
  Json::Value deleteTrackDb(const std::string &trackdb, std::string &err_msg);

  //
  // Query
  //
  bool loggedIn();

  //
  // Set properties
  //
  void setDebug(const bool debug);
  void setProxy(const std::string &proxy);
  void setUserAgent(const std::string &useragent);

private:
  curl_slist* getHeaders(const bool authorise);
  curl_slist* postHeaders(const bool authorise);

  std::string doGetRequest(const std::string &url, const bool authorise, long *response_code = NULL);
  std::string doPostRequest(const std::string &url, const std::string &postfields, const bool authorise, long *response_code = NULL);
  std::string doDeleteRequest(const std::string &url, const bool authorise, long *response_code = NULL);

  Json::Value getRequest(const std::string &request, const bool authorise, std::string &err_msg);
  Json::Value postRequest(const std::string &request, const std::string &postfields, const bool authorise, std::string &err_msg);
  Json::Value deleteRequest(const std::string &request, const bool authorise, std::string &err_msg);

  std::string host_;
  std::string user_;
  std::string auth_token_;

  CURLObject curl_object_get_;
  CURLObject curl_object_post_;
  CURLObject curl_object_delete_;

  Json::Reader json_reader_;
  Json::FastWriter json_writer_;

  bool debug_ ;
  std::string proxy_ ;
  std::string useragent_ ;
};



} // namespace trackhub



} /* namespace gbtools */

#endif	/* GBTOOLS_TRACKHUB_H */
