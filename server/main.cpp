
#include <string>
#include <iostream>

#include "streamserver.hpp"


onAcceptedCallback onAccepted = []() 
{
    std::cout << "Accepted client\n";
};

int main()
{
    SocketAddress serverAddr("127.0.0.1", 8081);
    StreamServer server(serverAddr, onAccepted);
    server.start();
    bool running = true;
    while(running)
    {
        std::string input;
        std::cin >> input;
        if(input == "-q")
        {
            running = false;
        }
    }
    server.stop();
    return 0;
}
