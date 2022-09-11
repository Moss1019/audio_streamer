#include <cmath>
#include <vector>
#include <string>
#include <cstring>
#include <iostream>

#include <inttypes.h>

#include "tcpsocket.hpp"
#include "tcpmessage.hpp"
#include "socketaddress.hpp"
#include "streamerclient.hpp"

void audioMain() 
{
    SocketAddress serverAddress("127.0.0.1", 8081);
    StreamerClient client(serverAddress);
    std::vector<std::string> files;

    if(client.inError()) 
    {
        std::cout << client.errorMsg() << "\n";
    }

    client.getSongs(files);
    int i = 0;
    std::cout << "Got " << files.size() << " files\n";
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
}

void audioTestMain()
{
    unsigned seconds = 5;
    unsigned sampleRate = 44100;
    unsigned channels = 2;
    double volume = 0.4;
    double tone = 330;
    WavPlayer player(sampleRate, channels);
    unsigned bufferLength = sampleRate * channels * seconds;
    uint16_t *frames = new uint16_t[sampleRate * channels * seconds];
    for(unsigned i = 0; i < bufferLength; i += 2) 
    {
        double sampleFreq = sampleRate / tone;
        frames[i] = (uint16_t)(((sin(i / sampleFreq * (3.14 * 2)) + 1) / 2 * volume) * 65535);
        frames[i + 1] = frames[i];
    }
    player.play(frames, bufferLength);
}

int main()
{
    audioMain();
    return 0;
}
