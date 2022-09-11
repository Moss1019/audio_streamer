
#include <iostream>

#include <string>
#include <vector>

#include "streamserver.hpp"
#include "socketaddress.hpp"

#include "wavfile.hpp"
#include "fileinputstream.hpp"
#include "directoryinfo.hpp"

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
    std::vector<std::string> files;
    while(running)
    {
        std::cin >> input;
        if(input == "-q")
        {
            running = false;
        }
        else if (input == "-c")
        {
            DirectoryInfo dInfo("../songs");
            dInfo.getFiles(files);
            int i = 0;
            for(const auto &f : files) 
            {
                std::cout << i++ << " " << f << "\n";
            }
        }
        else if (input == "-r")
        {
            int index;
            std::cin >> index;
            if(index >= files.size()) 
            {
                continue;
            }
            FileInputStream fInStream(files[index]);
            char *buffer = new char[fInStream.size()];
            fInStream.read(buffer);
            WavFile wf(buffer);
            delete[] buffer;
            std::cout << files[index] << "\n\tchannels: " << wf.channels
                << "\n\tbytes per frame: " << wf.bitsPerFrame
                << "\n\tbits per sample: " << wf.bitsPerSample
                << "\n\tclock rate: " << wf.clockRate
                << "\n\tformat type: " << wf.formatType
                << "\n";
        }
    }
    server.stop();
    return 0;
}
