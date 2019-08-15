#ifndef DASADOS_SERVER_SERVER_H
#define DASADOS_SERVER_SERVER_H

#ifndef BUFFER_SIZE
#define BUFFER_SIZE 4096
#endif
#define QUEUE 20

namespace server
{
bool Listen(unsigned short port);
void Handle(const char* path, const char* dir);
}

#endif
