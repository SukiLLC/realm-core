// Copyright 2008 Google Inc. All Rights Reserved.
//
// Various Google-specific macros.
//
// This code is compiled directly on many platforms, including client
// platforms like Windows, Mac, and embedded systems.  Before making
// any changes here, make sure that you're not breaking any platforms.
//

#ifndef BASE_MACROS_H_
#define BASE_MACROS_H_

#include "s2/base/definer.h"   // For OS_WINDOWS

#include <stddef.h>         // For size_t
#include <type_traits>

// The swigged version of an abstract class must be concrete if any methods
// return objects of the abstract type. We keep it abstract in C++ and
// concrete for swig.
#ifndef SWIG
#define ABSTRACT = 0
#endif

#define COMPILE_ASSERT(expr, msg) \
    static_assert(expr, #msg)

// A macro to disallow the copy constructor and operator= functions
// This should be used in the private: declarations for a class
//
// For disallowing only assign or copy, write the code directly, but declare
// the intend in a comment, for example:
// void operator=(const TypeName&);  // DISALLOW_ASSIGN
// Note, that most uses of DISALLOW_ASSIGN and DISALLOW_COPY are broken
// semantically, one should either use disallow both or neither. Try to
// avoid these in new code.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&);               \
  void operator=(const TypeName&)

// An older, politically incorrect name for the above.
// Prefer DISALLOW_COPY_AND_ASSIGN for new code.
#define DISALLOW_EVIL_CONSTRUCTORS(TypeName) DISALLOW_COPY_AND_ASSIGN(TypeName)

// A macro to disallow all the implicit constructors, namely the
// default constructor, copy constructor and operator= functions.
//
// This should be used in the private: declarations for a class
// that wants to prevent anyone from instantiating it. This is
// especially useful for classes containing only static methods.
#define DISALLOW_IMPLICIT_CONSTRUCTORS(TypeName) \
  TypeName(const TypeName&);                     \
  void operator=(const TypeName&);               \
  TypeName()

// A macro to declare that a class is a POD (plain old data) type.
//
// This macro is used to inform the type traits library that a user class
// is a POD.  It is important that this macro be used correctly, since
// the type traits library uses this information to determine whether
// it is safe to use a class with certain template functions.
//
// A class is a POD type if it has no user-declared constructor, no
// private or protected non-static data members, no base classes, and
// no virtual functions.  In other words, a POD type is a class that
// has the same layout as a C struct.
//
// Note: This macro has been disabled as it specializes std::is_pod which
// is not allowed in modern C++ compilers (Xcode 26.4+). The S2 library
// internally does not rely on this specialization.
#define DECLARE_POD(TypeName)

// DECLARE_NESTED_POD is a synonym for DECLARE_POD.
#define DECLARE_NESTED_POD(TypeName) DECLARE_POD(TypeName)

// PROPAGATE_POD_FROM_TEMPLATE_ARGUMENT has been disabled as it specializes
// std::is_pod which is not allowed in modern C++ compilers (Xcode 26.4+).
#define PROPAGATE_POD_FROM_TEMPLATE_ARGUMENT(TemplateName)

// Macro that does nothing if TypeName is a POD, and gives a compiler
// error if TypeName is a non-POD.  You should put a descriptive
// comment right next to the macro call so that people can tell what
// the compiler error is about.
//
// Implementation note: this works by taking the size of a type that's
// complete when TypeName is a POD and incomplete otherwise.

template <bool IsPod> struct ERROR_TYPE_MUST_BE_POD;
template <> struct ERROR_TYPE_MUST_BE_POD<true> { };
#define ENFORCE_POD(TypeName)                                             \
  enum { dummy_##TypeName                                                 \
           = sizeof(ERROR_TYPE_MUST_BE_POD<                               \
                      std::is_trivial<TypeName>::value>) }

#endif  // BASE_MACROS_H_
