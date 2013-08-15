/**
 * @file Version.h
 *
 * Created on: Aug 14, 2013Jun 20, 2013
 *     Author: Pierre Dubath
 */

#ifndef VERSION_H_
#define VERSION_H_

/**
 * @brief
 *    Function returning a version string extracted from SVN keywords
 * @details
 *    ElementsExample/ElementsProgramExample.cpp shows how to use this method to implement
 *    a getVersion in a prgram
 * @param svnUrl
 *    This is a string that can be filled with the HeadURL svn keywords. Writing
 *       #define SVN_URL "SVN $HeadURL$
 *    in a file.
 * @param svnId
 *     This is a string that can be filled with the Id svn keywords. Writing
 *       #define SVN_ID "SVN $Id$
 *    in a file.
 *
 *    the svn keywords will be expanded upon the next commit if the svn
 *    client is instructed to do this with
 *       svn propset svn:keywords 'Id Revision HeadURL' ElementsProgramExample.cpp
 *
 * @return
 */
std::string getVersionFromSvnKeywords(std::string svnUrl, std::string svnId);


#endif /* VERSION_H_ */
