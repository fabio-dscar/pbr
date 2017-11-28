#ifndef __PBR_PBR_H__
#define __PBR_PBR_H__

#if defined(_WIN32) || defined(_WIN64)
#define PBR_WINDOWS
#elif defined(__linux__)
#define PBR_LINUX
#endif

// Global includes
#include <cstdint>
#include <type_traits>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include <iomanip>

#if defined(_MSC_VER)
#define PBR_MSVC
#include <float.h>
#include <intrin.h>
#pragma warning(disable : 4244)  // int to float warning
#pragma warning(disable : 4843)  // double to float warning
#pragma warning(disable : 4305)  // double const to float
#endif

#if defined(__GNUC__)
#define PBR_GNUC
#endif

#if defined(_MSC_VER) && _MSC_VER == 1800
#define PBR_MSVC2013
#endif

#ifdef PBR_MSVC2013
// MSVC2013 does not support constexpr
#define PBR_CONSTEXPR const
#else
#define PBR_CONSTEXPR constexpr
#endif

// Decide if we are importing or exporting from/to a dll
// Or just static linking
#if defined(PBR_BUILD_SHARED) && defined(PBR_DLL_IMPORT)
#if defined(PBR_MSVC)
#define PBR_SHARED __declspec(dllimport)
#else
#define PBR_SHARED
#endif
#elif defined(PBR_BUILD_SHARED) && defined(PBR_DLL_EXPORT)
#if defined(PBR_MSVC)
#define PBR_SHARED __declspec(dllexport)
#elif defined(PBR_GNUC)
#define PBR_SHARED __attribute__((visibility("default")))
#endif
#else
#define PBR_SHARED  
#endif

#define make_sref std::make_shared

namespace pbr {

    // Integer types
    typedef std::uint8_t  uint8;
    typedef std::uint16_t uint16;
    typedef std::uint32_t uint32;
    typedef std::uint64_t uint64;

    typedef std::int8_t  int8;
    typedef std::int16_t int16;
    typedef std::int32_t int32;
    typedef std::int64_t int64;

    typedef int64 RRID;

    template<typename T> 
    using sref = std::shared_ptr<T>;

}

#endif