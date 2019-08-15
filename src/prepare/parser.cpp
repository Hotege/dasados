#include "parser.h"
#include <unistd.h>
#include <getopt.h>
#include <cstdio>
#include <cstring>

namespace prepare
{
bool ParsedParamsValid(int argc, char* const argv[], PreparedParams* ptr)
{
    bool checkp = false, checks = false, checkd = false;
    bool checkH = false, checkP = false, checkU = false, checkW = false, checkD = false;
    int opt;
    int option_index = 0;
    char format[] = "p:s:d:H:P:U:W:D:";
    static struct option long_options[] =
    {
        { "port", required_argument, nullptr, 'p' },
        { "server-lua", required_argument, nullptr, 's' },
        { "files-dir", required_argument, nullptr, 'd' },
        { "db-host", required_argument, nullptr, 'H'},
        { "db-port", required_argument, nullptr, 'P' },
        { "db-user", required_argument, nullptr, 'U' },
        { "db-password", required_argument, nullptr, 'W' },
        { "db-name", required_argument, nullptr, 'D' },
        { nullptr, 0, nullptr, 0 },
    };
    while((opt = getopt_long_only(argc, argv, format, long_options, &option_index)) != -1)
    {
        switch (opt)
        {
        case 'p':
        {
            checkp = true;
            sscanf(optarg, "%hd", &(ptr->port));
        }
            break;
        case 's':
        {
            checks = true;
            strcpy(ptr->serverLuaFile, optarg);
        }
            break;
        case 'd':
        {
            checkd = true;
            strcpy(ptr->directory, optarg);
        }
            break;
        case 'H':
        {
            checkH = true;
            strcpy(ptr->dbhost, optarg);
        }
            break;
        case 'P':
        {
            checkP = true;
            sscanf(optarg, "%hd", &(ptr->dbport));
        }
            break;
        case 'U':
        {
            checkU = true;
            strcpy(ptr->dbuser, optarg);
        }
            break;
        case 'W':
        {
            checkW = true;
            strcpy(ptr->dbpassword, optarg);
        }
            break;
        case 'D':
        {
            checkD = true;
            strcpy(ptr->dbname, optarg);
        }
            break;
        }
    }
    if (!checkp || ptr->port == 0)
        ptr->port = DEFAULT_PORT;
    if (!checkd || strcmp(ptr->directory, "") == 0)
        strcpy(ptr->directory, "./");
    if (!checkP || ptr->dbport == 0)
        ptr->dbport = DEFAULT_DB_PORT;
    if (!checkU || strcmp(ptr->dbuser, "") == 0)
        strcpy(ptr->dbuser, "dasados");
    if (!checkD || strcmp(ptr->dbname, "") == 0)
        strcpy(ptr->dbname, "dasados");
    return checks && checkH && checkW;
}
}
