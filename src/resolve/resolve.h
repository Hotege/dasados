#ifndef DASADOS_RESOLVE_RESOLVE_H
#define DASADOS_RESOLVE_RESOLVE_H

#include <string>
#include <map>

namespace resolve
{
std::string GetRequestType(const std::string& source);
std::string GetHTTPRequestType(const std::string& source);
std::string GetHTTPRequestDir(const std::string& source);
std::map<std::string, std::string> GetHTTPRequestPost(const std::string& source);
}

#endif
