#include "database.h"
#include <mysql.h>
#include <cstdio>

namespace db
{
MYSQL conn;

void initialize(const char* host, const char* user, const char* password, const char* database, const unsigned short& port)
{
    mysql_real_connect(&conn, host, user, password, database, (int)port, NULL, 0);
}

std::vector<std::map<std::string, std::string>> selectAll(const char* table)
{
    std::vector<std::map<std::string, std::string>> result;
    int res = mysql_query(&conn, (std::string("select * from ") + table).c_str());
    if (!res)
    {
        MYSQL_RES* res_ptr = mysql_store_result(&conn);
        if (res_ptr)
        {
            int column = mysql_num_fields(res_ptr);
            int row = mysql_num_rows(res_ptr) + 1;
            std::vector<std::string> tags;
            for (int i = 0; MYSQL_FIELD* field = mysql_fetch_field(res_ptr); i++)
                tags.push_back(field->name);
            for (int i = 1; i < row; i++)
            {
                MYSQL_ROW result_row = mysql_fetch_row(res_ptr);
                std::map<std::string, std::string> m;
                for (int j = 0; j < column; j++)
                    m[tags[j]] = result_row[j];
                result.push_back(m);
            }
        }
        mysql_free_result(res_ptr);
    }
    return result;
}

void terminate()
{
    mysql_close(&conn);
    mysql_library_end();
}
}
