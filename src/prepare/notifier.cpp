#include "notifier.h"
#include <cstdio>

namespace prepare
{
void ShowStartMessage()
{
    const char message[] =
"/******************************************************************************/\n"
"/* Welcome to DL's Articles Sharing And Discussing Operating Server(DASADOS). */\n"
"/* Last created at 2019/08/06 by DL.                                          */\n"
"/*          _______   ____   ______   ____  _______   ____   ______           */\n"
"/*          | ___  \\ / __ \\ /  ____) / __ \\ | ___  \\ / __ \\ /  ____)          */\n"
"/*          | |  | || |  | || (____ | |  | || |  | || |  | || (____           */\n"
"/*          | |  | || |__| |\\____  \\| |__| || |  | || |  | |\\____  \\          */\n"
"/*          | |__/ ||  __  | ____) ||  __  || |__/ || |__| | ____) |          */\n"
"/*          |_____/ |_|  |_|(______/|_|  |_||_____/  \\____/ (______/          */\n"
"/*                                                                            */\n"
"/* Usage: dasados [OPTIONS]                                                   */\n"
"/* Options:                                                                   */\n"
"/*   --port=port/-p port(default: 8080):       Port of dasados.               */\n"
"/*   --server-lua=file/-s file(required):      DASADOS Lua File path.         */\n"
"/*   --files-dir=dir/-d dir(default: ./):      Directory of (static) files.   */\n"
"/*   --db-host=host/-H host(required):         Host of database.              */\n"
"/*   --db-port=port/-P port(default: 3306):    Port of database.              */\n"
"/*   --db-user=user/-U user(default: dasados): User of database.              */\n"
"/*   --db-password=pwd/-W pwd(required):       Password of database.          */\n"
"/*   --db-name=name/-D name(default: dasados): Database name of database.     */\n"
"/******************************************************************************/\n"
;
    printf(message);
}

void ShowLoadParamsError()
{
    const char message[] =
">> Failed to load parameters.\n"
;
    printf(message);
}
}
