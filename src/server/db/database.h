#ifndef DASADOS_SERVER_DB_DATABASE_H
#define DASADOS_SERVER_DB_DATABASE_H

#include <vector>
#include <string>
#include <map>

namespace db
{
void initialize(const char* host, const char* user, const char* password, const char* database, const unsigned short& port);
std::vector<std::map<std::string, std::string>> selectAll(const char* table);
void terminate();
}

#endif
