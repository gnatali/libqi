/*
 * Copyright (c) 2012 Aldebaran Robotics. All rights reserved.
 * Use of this source code is governed by a BSD-style license that can be
 * found in the COPYING file.
 */

/** @file
 *  @brief dll import/export and compiler message
 */

#pragma once
#ifndef _LIBQI_QI_MACRO_HPP_
#define _LIBQI_QI_MACRO_HPP_

#include <qi/preproc.hpp>

// Deprecated
#if defined(__GNUC__) && defined(WITH_DEPRECATED) && !defined(QI_NO_API_DEPRECATED)
#  define QI_API_DEPRECATED __attribute__((deprecated))
#elif defined(_MSC_VER) && defined(WITH_DEPRECATED) && !defined(QI_NO_API_DEPRECATED)
#  define QI_API_DEPRECATED __declspec(deprecated)
#else
#  define QI_API_DEPRECATED
#endif

// For shared library




/** @return the proper type specification for import/export
 * @param libname the name of your library.
 * This macro will use two preprocessor defines:
 * libname_EXPORTS (cmake convention) and libname_STATIC_BUILD.
 * Those macro can be unset or set to 0 to mean false, or set to empty or 1 to
 * mean true.
 * The first one must be true if the current compilation unit is within the library.
 * The second must be true if the library was built as a static archive.
 * The proper way to use this macro is to:
 * - Have your buildsystem set mylib_EXPORTS when building MYLIB
 * - Have your buildsystem produce a config.h file that
     #define mylib_STATIC_BUILD to 1 or empty if it is a static build, and
     not define mylib_STATIC_BUILD or define it to 0 otherwise
 * In one header, write
 *   #include <mylib/config.h>
 *   #define MYLIB_API QI_LIB_API(mylib)
 */
#define QI_LIB_API(libname) _QI_LIB_API(BOOST_PP_CAT(libname, _EXPORTS), BOOST_PP_CAT(libname, _STATIC_BUILD))

#define _QI_LIB_API(IS_BUILDING_LIB, IS_LIB_STATIC_BUILD) \
  QI_LIB_API_NORMALIZED(_QI_IS_ONE_OR_EMPTY(BOOST_PP_CAT(_ , IS_BUILDING_LIB)), _QI_IS_ONE_OR_EMPTY(BOOST_PP_CAT(_, IS_LIB_STATIC_BUILD)))

// Each platform must provide a QI_LIB_API_NORMALIZED(isBuilding, isStatic)
#if defined _WIN32 || defined __CYGWIN__
#  define QI_EXPORT_API __declspec(dllexport)
#  define QI_IMPORT_API __declspec(dllimport)
#  define QI_LIB_API_NORMALIZED(exporting, isstatic) BOOST_PP_CAT(BOOST_PP_CAT(_QI_LIB_API_NORMALIZED_, exporting), isstatic)
#  define _QI_LIB_API_NORMALIZED_00 QI_IMPORT_API
#  define _QI_LIB_API_NORMALIZED_10 QI_EXPORT_API
#  define _QI_LIB_API_NORMALIZED_11
#  define _QI_LIB_API_NORMALIZED_01
#elif __GNUC__ >= 4
#  define QI_EXPORT_API __attribute__ ((visibility("default")))
#  define QI_IMPORT_API QI_EXPORT_API
#  define QI_LIB_API_NORMALIZED(a, b) QI_EXPORT_API
#else
#  define QI_IMPORT_API
#  define QI_EXPORT_API
#  define QI_LIB_API_NORMALIZED(a, b)
#endif


//! \cond internal
// Macros adapted from opencv2.2
#if defined(_MSC_VER)
  #define QI_DO_PRAGMA(x) __pragma(x)
  #define __ALSTR2__(x) #x
  #define __ALSTR1__(x) __ALSTR2__(x)
  #define _ALMSVCLOC_ __FILE__ "("__ALSTR1__(__LINE__)") : "
  #define QI_MSG_PRAGMA(_msg) QI_DO_PRAGMA(message (_ALMSVCLOC_ _msg))
#elif defined(__GNUC__)
  #define QI_DO_PRAGMA(x) _Pragma (#x)
  #define QI_MSG_PRAGMA(_msg) QI_DO_PRAGMA(message (_msg))
#else
  #define QI_DO_PRAGMA(x)
  #define QI_MSG_PRAGMA(_msg)
#endif
//! \endcond


// Use this macro to generate compiler warnings.
#if !defined(WITH_DEPRECATED) || defined(QI_NO_COMPILER_WARNING)
# define QI_COMPILER_WARNING(x)
#else
# define QI_COMPILER_WARNING(x) QI_MSG_PRAGMA("Warning: " #x)
#endif

// Deprecate a header, add a message to explain what user should do
#if !defined(WITH_DEPRECATED) || defined(QI_NO_DEPRECATED_HEADER)
# define QI_DEPRECATED_HEADER(x)
#else
# define QI_DEPRECATED_HEADER(x) QI_MSG_PRAGMA("\
This file includes at least one deprecated or antiquated ALDEBARAN header \
which may be removed without further notice in the next version. \
Please consult the changelog for details. " #x)
#endif


// A macro to disallow copy constructor and operator=
#define QI_DISALLOW_COPY_AND_ASSIGN(type)       \
  type(type const &);                           \
  void operator=(type const &)

#if defined(__GNUC__)
# define QI_WARN_UNUSED_RESULT __attribute__((warn_unused_result))
#else
# define QI_WARN_UNUSED_RESULT
#endif

#define QI_UNUSED(x)

#endif  // _LIBQI_QI_API_HPP_

