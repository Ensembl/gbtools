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
#include <gbtools/gbtoolsCurl.hpp>



namespace gbtools
{
namespace trackhub
{


/* Class for accessing the Ensembl Track Hub Registry API */
class Registry
{
public:

  Registry();
  ~Registry();

  bool ping();
  std::string version();

private:
  Json::Value sendRequest(const std::string &url, const std::string &postfields = "");
  std::string getRequest(const std::string &url);
  std::string postRequest(const std::string &url, const std::string &postfields);

  std::string host_;

  CURLObject curl_object_get_;
  CURLObject curl_object_post_;
  curl_slist *get_headers_;
  curl_slist *post_headers_;

  Json::Reader json_reader_;
};


} // namespace trackhub


void testTrackhub(); // temp function for testing

} /* namespace gbtools */

#endif	/* GBTOOLS_TRACKHUB_H */
