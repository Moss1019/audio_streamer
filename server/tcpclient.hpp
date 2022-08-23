#pragma once

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#include <sys/socket.h>
#endif

#include <thread>

#include "types.hpp"
#include "tcpsocket.hpp"
#include "memoryinputstream.hpp"
#include "memoryoutputstream.hpp"

class TcpClient
{
private:
	TcpSocket *m_sock;

	std::thread *m_worker;

	onReceiveCallback m_onReceive;

	bool m_running;

	void doWork();

public:
	TcpClient(socktype sock, onReceiveCallback onReceive);

	TcpClient(const TcpClient &other) = delete;

	TcpClient operator=(const TcpClient &other) = delete;

	~TcpClient();

	bool running() const;

	bool inError() const;

	const std::string &errorMsg() const;

	void start();

	void stop();

	int sendData(const MemoryOutputStream &stream);
};

