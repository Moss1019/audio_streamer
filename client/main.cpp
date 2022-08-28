#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include <inttypes.h>

#include "tcpsocket.hpp"
#include "tcpmessage.hpp"
#include "socketaddress.hpp"
#include "streamerclient.hpp"

int main()
{
    SocketAddress serverAddress("127.0.0.1", 8081);
    StreamerClient client(serverAddress);
    std::vector<std::string> files;

    client.getSongs(files);

    int i = 0;
    for(const auto &f: files)
    {
        std::cout << i++ << " " << f << "\n";   
    }

    client.pause();

    bool running = true;
    while(running)
    {
        std::string input;
        std::cin >> input;
        if(input == "-q")
        {
            running = false;
        }
        else if (input == "-s")
        {
            int index;
            std::cin >> index;
            std::cout << "Starting song " << files[0] << "\n";
            if(index < files.size())
            {
                client.play(files[index]);
            }
        }
        else if(input == "-p")
        {
            client.pause();
            std::cout << "pausing\n";
        }
        else if(input == "-r")
        {
            client.resume();
            std::cout << "resuming\n";
        }
    }

    return 0;
}
