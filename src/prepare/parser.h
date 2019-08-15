#ifndef DASADOS_PREPARE_PARSER_H
#define DASADOS_PREPARE_PARSER_H

#define DEFAULT_PORT 8080
#define DEFAULT_DB_PORT 3306
#define STRING_SIZE 200

namespace prepare
{
typedef struct
{
    unsigned short port;
    char serverLuaFile[STRING_SIZE];
    char directory[STRING_SIZE];
    char dbhost[STRING_SIZE];
    unsigned short dbport;
    char dbuser[STRING_SIZE];
    char dbpassword[STRING_SIZE];
    char dbname[STRING_SIZE];
} PreparedParams;

bool ParsedParamsValid(int argc, char* const argv[], PreparedParams* p);
}

#endif
