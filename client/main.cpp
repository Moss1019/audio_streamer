
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include "songinfo.hpp"
#include "tcpsocket.hpp"
#include "wavplayer.hpp"
#include "tcpmessage.hpp"
#include "songrequest.hpp"
#include "streamerclient.hpp"

#include "fileoutputstream.hpp"

int main()
{
    SocketAddress serverAddr("127.0.0.1", 8081);
    bool running = true;
    std::vector<std::string> files;
    StreamerClient client(serverAddr);
    client.start();
    while(running)
    {
        std::string input;
        std::cin >> input;
        if(input == "-q")
        {
            running = false;
        }
        else if(input == "-l")
        {
            client.getSongs(files);
            unsigned i = 0;
            for(const auto &f : files)
            {
                std::cout << i << " " << f << "\n";
            }
        }
        else if(input == "-p")
        {
            int index;
            std::cin >> index;
            if(index >= files.size())
            {
                continue;
            }
            std::string fileName = files[index];
            client.play(fileName);
        }
        else if (input == "-s")
        {
            client.pause();
        }
        else if(input == "-r")
        {
            client.resume();
        }
    }
    client.stop();
    return 0;
}

