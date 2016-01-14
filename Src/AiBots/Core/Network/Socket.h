// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#include "Core/Base/Config.h"
#include "Core/Base/Types.h"
#include "Core/Debug/Error.h"
#include "Core/Network/NetAddress.h"

#if RIO_PLATFORM_POSIX
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#elif RIO_PLATFORM_WINDOWS
//Undefined in Headers_Windows.h, but winsock2 needs it.
#ifndef NEAR
#define NEAR
#endif
#ifndef FAR
#define FAR
#endif
#include <winsock2.h>
#include "Core/Os/Windows/Headers_Windows.h"
// TODO add WinSock lib to linker includes in project properties
#pragma comment(lib, "Ws2_32.lib")
#endif

namespace Rio
{
	struct ConnectResult
	{
		enum
		{
			NO_ERROR, BAD_SOCKET, REFUSED, TIMEOUT, UNKNOWN
		}
		error;
	};

	struct ReadResult
	{
		enum { NO_ERROR, BAD_SOCKET, REMOTE_CLOSED, TIMEOUT, UNKNOWN } error;
		size_t bytesRead;
	};

	struct WriteResult
	{
		enum { NO_ERROR, BAD_SOCKET, REMOTE_CLOSED, TIMEOUT, UNKNOWN } error;
		size_t bytesWritten;
	};

	struct AcceptResult
	{
		enum { NO_ERROR, BAD_SOCKET, NO_CONNECTION, UNKNOWN } error;
	};

	// TCP socket
	struct TcpSocket
	{
		TcpSocket()
		{
		}

		void open();
		ConnectResult connect(const NetAddress& ip, uint16_t port);
		bool bind(uint16_t port);
		void listen(uint32_t max);
		AcceptResult acceptInternal(TcpSocket& c);

		AcceptResult acceptNonBlocking(TcpSocket& c)
		{
			setBlocking(false);
			return acceptInternal(c);
		}

		AcceptResult accept(TcpSocket& c)
		{
			setBlocking(true);
			return acceptInternal(c);
		}

		void close();
		ReadResult readInternal(void* data, size_t size);

		ReadResult readNonBlocking(void* data, size_t size)
		{
			setBlocking(false);
			return readInternal(data, size);
		}

		ReadResult read(void* data, size_t size)
		{
			setBlocking(true);
			return readInternal(data, size);
		}

		WriteResult writeInternal(const void* data, size_t size);

		WriteResult writeNonBlocking(const void* data, size_t size)
		{
			setBlocking(false);
			return writeInternal(data, size);
		}

		WriteResult write(const void* data, size_t size)
		{
			setBlocking(true);
			return writeInternal(data, size);
		}

		void setBlocking(bool blocking);
		void setReuseAddress(bool reuse);
		void setTimeout(uint32_t seconds);
	private:

#if RIO_PLATFORM_POSIX
		int socket = 0;
#elif RIO_PLATFORM_WINDOWS
		SOCKET socket = INVALID_SOCKET;
#endif
	};

} // namespace Rio
