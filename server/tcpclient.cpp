#include "tcpclient.hpp"

#include "tcpmessage.hpp"

void TcpClient::doWork()
{
	unsigned bufferSize = 1024 * 512;
	void *buffer = std::malloc(bufferSize);
	while (m_running)
	{
		MemoryOutputStream out;
		int received = m_sock->receiveData(out);
		if (received <= 0)
		{
			m_running = false;
		}
		else
		{
			MemoryInputStream in(out.getBufferPtr(), out.getLength());
			m_onReceive(this, in);
		}
	}
	std::free(buffer);
}

TcpClient::TcpClient(socktype sock, onReceiveCallback onReceive)
	:m_sock{nullptr}, m_worker{nullptr}, m_onReceive{onReceive}, m_running{false}
{
	m_sock = new TcpSocket(sock);
}

TcpClient::~TcpClient()
{
	stop();
	if (m_worker != nullptr)
	{
		if (m_worker->joinable())
		{
			m_worker->join();
		}
		delete m_worker;
		m_worker = nullptr;
	}
	if (m_sock != nullptr)
	{
		delete m_sock;
		m_sock = nullptr;
	}
}

bool TcpClient::running() const
{
	return m_running;
}

bool TcpClient::inError() const
{
	return m_sock->inError();
}

const std::string &TcpClient::errorMsg() const
{
	return m_sock->errorMsg();
}

void TcpClient::start()
{
	m_running = true;
	m_worker = new std::thread(&TcpClient::doWork, this);
}

void TcpClient::stop()
{
	m_running = false;
}

int TcpClient::sendData(const MemoryOutputStream &stream)
{
	return m_sock->sendData(stream);
}
