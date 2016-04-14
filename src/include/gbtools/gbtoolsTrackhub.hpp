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

#include <gbtools/gbtoolsCurl.hpp>



namespace gbtools
{
namespace trackhub
{


/* Class for accessing the Ensembl Track Hub Registry API */
class Registry
{
public:

  // Construct/destruct
  Registry();
  ~Registry();

  // Info API
  bool ping();
  std::string version();
  std::list<std::string> species();
  std::map<std::string, std::list<std::string>> assemblies();
  Json::Value trackhubs();

  // Search API
  Json::Value search(const std::string &query, const std::string &species = "",
                     const std::string &assembly = "", const std::string &hub = "");
  Json::Value searchTrackDb(const std::string &trackdb);

  // Registration API
  bool login(const std::string &user, const std::string &pwd);
  std::string logout();

  Json::Value registerHub(const std::string &url, 
                          const std::map<std::string, std::string> &assemblies,
                          const std::string &type = "");
  Json::Value retrieveHub(const std::string &trackhub = "");
  std::string deleteHub(const std::string &trackhub);

  Json::Value retrieveTrackDb(const std::string &trackdb);
  Json::Value deleteTrackDb(const std::string &trackdb);
  

private:
  curl_slist* getHeaders(const bool authorise);
  curl_slist* postHeaders(const bool authorise);

  std::string doGetRequest(const std::string &url, const bool authorise);
  std::string doPostRequest(const std::string &url, const std::string &postfields, const bool authorise);
  std::string doDeleteRequest(const std::string &url, const bool authorise);

  Json::Value getRequest(const std::string &request, const bool authorise = false);
  Json::Value postRequest(const std::string &request, const std::string &postfields, const bool authorise = false);
  Json::Value deleteRequest(const std::string &request, const bool authorise = false);

  std::string host_;
  std::string user_;
  std::string auth_token_;

  CURLObject curl_object_get_;
  CURLObject curl_object_post_;
  CURLObject curl_object_delete_;

  Json::Reader json_reader_;
};


} // namespace trackhub


void testTrackhub(); // temp function for testing

} /* namespace gbtools */

#endif	/* GBTOOLS_TRACKHUB_H */
