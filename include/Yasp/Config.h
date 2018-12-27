#ifndef YASP_CONFIG_H
#define YASP_CONFIG_H

#include <cstdint>
typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;

typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;


#ifdef _WIN32
	#define YASP_PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#define NOMINMAX
	#define YASP_API_EXPORT __declspec(dllexport)
	#define YASP_API_IMPORT __declspec(dllimport)
	#define YASP_RENDERER_D3D
#endif

#define NOMOVORCOPY(type) \
type(const type&)= delete; \
type& operator=(const type&) = delete; \
type(type&&) = delete; \
type& operator=( type&&) = delete;


#endif
