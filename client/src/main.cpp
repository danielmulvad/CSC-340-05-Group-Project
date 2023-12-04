#include "Client.h"

int main(int argc, char const *argv[])
{
    Client *client = new Client(8080);
    return client->start();
}
