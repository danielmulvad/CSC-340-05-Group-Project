#include "./Server.h"

int main(int argc, char const *argv[])
{
    Server *server = new Server(8080);
    return server->run();
}
