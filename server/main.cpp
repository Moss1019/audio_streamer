
#include <iostream>

#include "streamserver.hpp"
#include "socketaddress.hpp"

#include "wavfile.hpp"
#include "fileinputstream.hpp"

void onReceived()
{
    std::cout << "Client accpted\n";
}

int main()
{
    SocketAddress localAddress("127.0.0.1", 8081);
    StreamServer server(localAddress, onReceived);
    server.start();
    std::string input;
    bool running = true;
    while(running)
    {
        std::cin >> input;
        if(input == "-q")
        {
            running = false;
        }
    }
    server.stop();
    return 0;
}
