#ifndef DASADOS_SERVER_LUAPLUGINS_LUAPLUGINS_H
#define DASADOS_SERVER_LUAPLUGINS_LUAPLUGINS_H

#include <string>
#include <map>

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif

namespace luaplugins
{
void initialize(const char* path, const char* dir);
void terminate();
void resolveDirectory(const int& conn, const std::string& type, const std::string& dir, const std::map<std::string, std::string>& post);
}

#endif
