// Copyright (c) 2015 Volodymyr Syvochka
#pragma once

#define RIO_VERSION_MAJOR "0"
#define RIO_VERSION_MINOR "1"
#define RIO_VERSION_MICRO "11"

#define RIO_COMPILER_CLANG 0
#define RIO_COMPILER_GCC 0
#define RIO_COMPILER_MSVC 0

#define RIO_PLATFORM_ANDROID 0
#define RIO_PLATFORM_IOS 0
#define RIO_PLATFORM_LINUX 0
#define RIO_PLATFORM_OSX 0
#define RIO_PLATFORM_WINDOWS 0

#define RIO_CPU_ARM  0
#define RIO_CPU_JIT  0
#define RIO_CPU_MIPS 0
#define RIO_CPU_PPC  0
#define RIO_CPU_X86  0

#define RIO_ARCH_32BIT 0
#define RIO_ARCH_64BIT 0

#define RIO_CPU_ENDIAN_BIG 0
#define RIO_CPU_ENDIAN_LITTLE 0

//////////////
// Compiler //
//////////////
#if defined(_MSC_VER)
	#undef RIO_COMPILER_MSVC
	#define RIO_COMPILER_MSVC 1
#elif defined(__clang__)
	// clang defines __GNUC__
	#undef RIO_COMPILER_CLANG
	#define RIO_COMPILER_CLANG 1
#elif defined(__GNUC__) || defined(__GNUG__) && !(defined(__clang__) || defined(__INTEL_COMPILER))
	// GNU GCC/G++ Compiler
	#undef RIO_COMPILER_GCC
	#define RIO_COMPILER_GCC 1
#elif defined(__INTEL_COMPILER)
	// Intel C++ Compiler
	#define RIO_COMPILER_INTEL
#else
	#error "RIO_COMPILER_* is not defined!"
#endif

///////////////
// System OS //
///////////////

// Identify the operating system used
#if defined(_WIN32) || defined(_WIN64)
	// Windows
	#undef RIO_PLATFORM_WINDOWS
	#if !defined(WINVER) && !defined(_WIN32_WINNT)
		// Windows Server 2003 with SP1, Windows XP with SP2 and above
		#define WINVER 0x0502
		#define _WIN32_WINNT 0x0502
	#endif // !defined(WINVER) && !defined(_WIN32_WINNT)
	#define RIO_PLATFORM_WINDOWS 1
	// To ensure
	#ifndef NOMINMAX
		#define NOMINMAX
	#endif
#elif defined(__ANDROID__)
// Android compiler defines __linux__
	#undef RIO_PLATFORM_ANDROID
	#define RIO_PLATFORM_ANDROID 1
#elif defined(__linux__)
	#undef RIO_PLATFORM_LINUX
	#define RIO_PLATFORM_LINUX 1
#elif defined(__ENVIRONMENT_IPHONE_OS_VERSION_MIN_REQUIRED__)
	#undef RIO_PLATFORM_IOS
	#define RIO_PLATFORM_IOS 1
#elif defined(__ENVIRONMENT_MAC_OS_X_VERSION_MIN_REQUIRED__)
	#undef RIO_PLATFORM_OSX
	#define RIO_PLATFORM_OSX 1
#else
	#error "OS is not supported RIO_PLATFORM_* is not defined!"
#endif

#define RIO_PLATFORM_POSIX (RIO_PLATFORM_ANDROID \
						|| RIO_PLATFORM_IOS \
						|| RIO_PLATFORM_LINUX \
						|| RIO_PLATFORM_OSX)

#if defined(__arm__)
	#undef RIO_CPU_ARM
	#define RIO_CPU_ARM 1
	#define RIO_CACHE_LINE_SIZE 64
#elif defined(__MIPSEL__) || defined(__mips_isa_rev)
	#undef RIO_CPU_MIPS
	#define RIO_CPU_MIPS 1
	#define RIO_CACHE_LINE_SIZE 64
#elif defined(_M_PPC) || defined(__powerpc__) || defined(__powerpc64__)
	#undef RIO_CPU_PPC
	#define RIO_CPU_PPC 1
	#define RIO_CACHE_LINE_SIZE 128
#elif defined(_M_IX86) || defined(_M_X64) || defined(__i386__) || defined(__x86_64__)
	#undef RIO_CPU_X86
	#define RIO_CPU_X86 1
	#define RIO_CACHE_LINE_SIZE 64
#else // PNaCl doesn't have CPU defined.
	#undef RIO_CPU_JIT
	#define RIO_CPU_JIT 1
	#define RIO_CACHE_LINE_SIZE 64
#endif //

//////////////////////////
// Environment Bit Size //
//////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__64BIT__) || defined(__powerpc64__) || defined(__ppc64__)
	#undef RIO_ARCH_64BIT
	#define RIO_ARCH_64BIT 64
#else
	#undef RIO_ARCH_32BIT
	#define RIO_ARCH_32BIT 32
#endif //

#if RIO_CPU_PPC
	#undef RIO_CPU_ENDIAN_BIG
	#define RIO_CPU_ENDIAN_BIG 1
#else
	#undef RIO_CPU_ENDIAN_LITTLE
	#define RIO_CPU_ENDIAN_LITTLE 1
#endif

#if RIO_COMPILER_GCC
	#define RIO_COMPILER_NAME "GCC"
#elif RIO_COMPILER_MSVC
	#define RIO_COMPILER_NAME "MSVC"
#endif

#if RIO_PLATFORM_ANDROID
	#define RIO_PLATFORM_NAME "Android"
#elif RIO_PLATFORM_IOS
	#define RIO_PLATFORM_NAME "iOS"
#elif RIO_PLATFORM_LINUX
	#define RIO_PLATFORM_NAME "Linux"
#elif RIO_PLATFORM_OSX
	#define RIO_PLATFORM_NAME "OSX"
#elif RIO_PLATFORM_WINDOWS
	#define RIO_PLATFORM_NAME "Windows"
#endif // RIO_PLATFORM_

#if RIO_CPU_ARM
	#define RIO_CPU_NAME "ARM"
#elif RIO_CPU_MIPS
	#define RIO_CPU_NAME "MIPS"
#elif RIO_CPU_PPC
	#define RIO_CPU_NAME "PowerPC"
#elif RIO_CPU_JIT
	#define RIO_CPU_NAME "JIT-VM"
#elif RIO_CPU_X86
	#define RIO_CPU_NAME "x86"
#endif // RIO_CPU_

#if RIO_ARCH_32BIT
	#define RIO_ARCH_NAME "32-bit"
#elif RIO_ARCH_64BIT
	#define RIO_ARCH_NAME "64-bit"
#endif // RIO_ARCH_
