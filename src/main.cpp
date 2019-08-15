#include <cstdio>
#include <cstdlib>
#include "prepare/notifier.h"
#include "prepare/parser.h"
#include "server/server.h"
#include "server/db/database.h"

int main(int argc, char* const argv[])
{
    // Show information
    prepare::ShowStartMessage();
    // Parse parameters
    prepare::PreparedParams params = { 0 };
    auto parse = prepare::ParsedParamsValid(argc, argv, &params);
    if (!parse)
    {
        prepare::ShowLoadParamsError();
        exit(0);
    }
    else
    {
        //db::Connect(params.dbhost, params.dbport, params.dbuser, params.dbpassword, params.dbname);
        db::initialize(params.dbhost, params.dbuser, params.dbpassword, params.dbname, params.dbport);
        // Start server
        server::Listen(params.port);
        server::Handle(params.serverLuaFile, params.directory);
        db::terminate();
    }
    return 0;
}
