// Copyright (c) 2015 Volodymyr Syvochka
#include "Core/Network/Socket.h"

#if RIO_PLATFORM_WINDOWS

namespace Rio
{
	void TcpSocket::open()
	{
		socket = ::socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		RIO_ASSERT(socket >= 0, "socket: WSAGetLastError = %d", WSAGetLastError());
	}

	ConnectResult TcpSocket::connect(const NetAddress& ip, uint16_t port)
	{
		close();
		open();

		ConnectResult cr;
		cr.error = ConnectResult::NO_ERROR;

		sockaddr_in addr_in;
		addr_in.sin_family = AF_INET;
		addr_in.sin_addr.s_addr = ::htonl(ip.getAddress());
		addr_in.sin_port = ::htons(port);

		int err = ::connect(socket, (const sockaddr*)&addr_in, sizeof(sockaddr_in));

		if (err == 0)
			return cr;

		int wsaerr = WSAGetLastError();
		if (wsaerr == WSAECONNREFUSED)
			cr.error = ConnectResult::REFUSED;
		else if (wsaerr == WSAETIMEDOUT)
			cr.error = ConnectResult::TIMEOUT;
		else
			cr.error = ConnectResult::UNKNOWN;

		return cr;
	}

	bool TcpSocket::bind(uint16_t port)
	{
		close();
		open();
		setReuseAddress(true);
		sockaddr_in address;
		address.sin_family = AF_INET;
		address.sin_addr.s_addr = htonl(INADDR_ANY);
		address.sin_port = htons(port);

		int err = ::bind(socket, (const sockaddr*)&address, sizeof(sockaddr_in));
		RIO_ASSERT(err == 0, "bind: WSAGetLastError = %d", WSAGetLastError());
		RIO_UNUSED(err);
		return true;
	}

	void TcpSocket::listen(uint32_t max)
	{
		int err = ::listen(socket, max);
		RIO_ASSERT(err == 0, "listen: WSAGetLastError = %d", WSAGetLastError());
		RIO_UNUSED(err);
	}

	AcceptResult TcpSocket::acceptInternal(TcpSocket& c)
	{
		AcceptResult ar;
		ar.error = AcceptResult::NO_ERROR;

		SOCKET err = ::accept(socket, NULL, NULL);

		if (err != INVALID_SOCKET)
		{
			c.socket = err;
			return ar;
		}

		int wsaerr = WSAGetLastError();
		if (wsaerr == WSAEWOULDBLOCK)
			ar.error = AcceptResult::NO_CONNECTION;
		else
			ar.error = AcceptResult::UNKNOWN;

		return ar;
	}

	void TcpSocket::close()
	{
		if (socket != INVALID_SOCKET)
		{
			::closesocket(socket);
			socket = INVALID_SOCKET;
		}
	}

	ReadResult TcpSocket::readInternal(void* data, size_t size)
	{
		ReadResult rr;
		rr.error = ReadResult::NO_ERROR;
		rr.bytesRead = 0;

		char* buf = (char*)data;
		size_t sizeToRead = size;

		while (sizeToRead > 0)
		{
			int bytesRead = ::recv(socket, buf, (int)sizeToRead, 0);

			if (bytesRead == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK)
				return rr;
			else if (bytesRead == SOCKET_ERROR && WSAGetLastError() == WSAETIMEDOUT)
			{
				rr.error = ReadResult::TIMEOUT;
				return rr;
			}
			else if (bytesRead == 0)
			{
				rr.error = ReadResult::REMOTE_CLOSED;
				return rr;
			}

			buf += bytesRead;
			sizeToRead -= bytesRead;
			rr.bytesRead += bytesRead;
		}

		return rr;
	}

	WriteResult TcpSocket::writeInternal(const void* data, size_t size)
	{
		WriteResult wr;
		wr.error = WriteResult::NO_ERROR;
		wr.bytesWritten = 0;

		const char* buf = (const char*)data;
		size_t sizeToSend = size;

		while (sizeToSend > 0)
		{
			int bytesWritten = ::send(socket, (const char*)buf, (int)sizeToSend, 0);

			if (bytesWritten == SOCKET_ERROR && WSAGetLastError() == WSAEWOULDBLOCK)
			{
				return wr;
			}
			else if (bytesWritten == SOCKET_ERROR && WSAGetLastError() == WSAETIMEDOUT)
			{
				wr.error = WriteResult::TIMEOUT;
				return wr;
			}
			else if (bytesWritten == 0)
			{
				wr.error = WriteResult::REMOTE_CLOSED;
				return wr;
			}
			else
			{
				wr.error = WriteResult::UNKNOWN;
				return wr;
			}

			buf += bytesWritten;
			sizeToSend -= bytesWritten;
			wr.bytesWritten += bytesWritten;
		}

		return wr;
	}

	void TcpSocket::setBlocking(bool blocking)
	{
		//Warning! http://www.sockets.com/winsock.htm#IoctlSocket
		u_long nonBlocking = blocking ? 0 : 1;
		ioctlsocket(socket, FIONBIO, &nonBlocking);
	}

	void TcpSocket::setReuseAddress(bool reuse)
	{
		int optval = (int)reuse;
		int err = setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, (const char*)&optval, sizeof(optval));
		RIO_ASSERT(err == 0, "setsockopt: WSAGetLastError = %d", WSAGetLastError());
		RIO_UNUSED(err);
	}

	void TcpSocket::setTimeout(uint32_t seconds)
	{
		struct timeval timeout;
		timeout.tv_sec = seconds;
		timeout.tv_usec = 0;
		int err = setsockopt(socket, SOL_SOCKET, SO_RCVTIMEO, (char*)&timeout, sizeof(timeout));
		RIO_ASSERT(err == 0, "setsockopt: WSAGetLastError: %d", WSAGetLastError());
		err = setsockopt(socket, SOL_SOCKET, SO_SNDTIMEO, (char*)&timeout, sizeof(timeout));
		RIO_ASSERT(err == 0, "setsockopt: WSAGetLastError: %d", WSAGetLastError());
		RIO_UNUSED(err);
	}

} // namespace Rio

#endif // RIO_PLATFORM_WINDOWS