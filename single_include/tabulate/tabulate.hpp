// Copyright 2016-2018 by Martin Moene
//
// https://github.com/martinmoene/variant-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_VARIANT_LITE_HPP
#define NONSTD_VARIANT_LITE_HPP

#define variant_lite_MAJOR 1
#define variant_lite_MINOR 2
#define variant_lite_PATCH 2

#define variant_lite_VERSION                                                                       \
  variant_STRINGIFY(variant_lite_MAJOR) "." variant_STRINGIFY(                                     \
      variant_lite_MINOR) "." variant_STRINGIFY(variant_lite_PATCH)

#define variant_STRINGIFY(x) variant_STRINGIFY_(x)
#define variant_STRINGIFY_(x) #x

// variant-lite configuration:

#define variant_VARIANT_DEFAULT 0
#define variant_VARIANT_NONSTD 1
#define variant_VARIANT_STD 2

#if !defined(variant_CONFIG_SELECT_VARIANT)
#define variant_CONFIG_SELECT_VARIANT                                                              \
  (variant_HAVE_STD_VARIANT ? variant_VARIANT_STD : variant_VARIANT_NONSTD)
#endif

#ifndef variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
#define variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO 0
#endif

#ifndef variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
#define variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO 0
#endif

// Control presence of exception handling (try and auto discover):

#ifndef variant_CONFIG_NO_EXCEPTIONS
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define variant_CONFIG_NO_EXCEPTIONS 0
#else
#define variant_CONFIG_NO_EXCEPTIONS 1
#endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef variant_CPLUSPLUS
#if defined(_MSVC_LANG) && !defined(__clang__)
#define variant_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
#else
#define variant_CPLUSPLUS __cplusplus
#endif
#endif

#define variant_CPP98_OR_GREATER (variant_CPLUSPLUS >= 199711L)
#define variant_CPP11_OR_GREATER (variant_CPLUSPLUS >= 201103L)
#define variant_CPP11_OR_GREATER_ (variant_CPLUSPLUS >= 201103L)
#define variant_CPP14_OR_GREATER (variant_CPLUSPLUS >= 201402L)
#define variant_CPP17_OR_GREATER (variant_CPLUSPLUS >= 201703L)
#define variant_CPP20_OR_GREATER (variant_CPLUSPLUS >= 202000L)

// Use C++17 std::variant if available and requested:

#if variant_CPP17_OR_GREATER && defined(__has_include)
#if __has_include(<variant> )
#define variant_HAVE_STD_VARIANT 1
#else
#define variant_HAVE_STD_VARIANT 0
#endif
#else
#define variant_HAVE_STD_VARIANT 0
#endif

#define variant_USES_STD_VARIANT                                                                   \
  ((variant_CONFIG_SELECT_VARIANT == variant_VARIANT_STD) ||                                       \
   ((variant_CONFIG_SELECT_VARIANT == variant_VARIANT_DEFAULT) && variant_HAVE_STD_VARIANT))

//
// in_place: code duplicated in any-lite, expected-lite, optional-lite, value-ptr-lite,
// variant-lite:
//

#ifndef nonstd_lite_HAVE_IN_PLACE_TYPES
#define nonstd_lite_HAVE_IN_PLACE_TYPES 1

// C++17 std::in_place in <utility>:

#if variant_CPP17_OR_GREATER

#include <utility>

namespace nonstd {

using std::in_place;
using std::in_place_index;
using std::in_place_index_t;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;

#define nonstd_lite_in_place_t(T) std::in_place_t
#define nonstd_lite_in_place_type_t(T) std::in_place_type_t<T>
#define nonstd_lite_in_place_index_t(K) std::in_place_index_t<K>

#define nonstd_lite_in_place(T)                                                                    \
  std::in_place_t {}
#define nonstd_lite_in_place_type(T)                                                               \
  std::in_place_type_t<T> {}
#define nonstd_lite_in_place_index(K)                                                              \
  std::in_place_index_t<K> {}

} // namespace nonstd

#else // variant_CPP17_OR_GREATER

#include <cstddef>

namespace nonstd {
namespace detail {

template <class T> struct in_place_type_tag {};

template <std::size_t K> struct in_place_index_tag {};

} // namespace detail

struct in_place_t {};

template <class T>
inline in_place_t in_place(detail::in_place_type_tag<T> = detail::in_place_type_tag<T>()) {
  return in_place_t();
}

template <std::size_t K>
inline in_place_t in_place(detail::in_place_index_tag<K> = detail::in_place_index_tag<K>()) {
  return in_place_t();
}

template <class T>
inline in_place_t in_place_type(detail::in_place_type_tag<T> = detail::in_place_type_tag<T>()) {
  return in_place_t();
}

template <std::size_t K>
inline in_place_t in_place_index(detail::in_place_index_tag<K> = detail::in_place_index_tag<K>()) {
  return in_place_t();
}

// mimic templated typedef:

#define nonstd_lite_in_place_t(T) nonstd::in_place_t (&)(nonstd::detail::in_place_type_tag<T>)
#define nonstd_lite_in_place_type_t(T) nonstd::in_place_t (&)(nonstd::detail::in_place_type_tag<T>)
#define nonstd_lite_in_place_index_t(K)                                                            \
  nonstd::in_place_t (&)(nonstd::detail::in_place_index_tag<K>)

#define nonstd_lite_in_place(T) nonstd::in_place_type<T>
#define nonstd_lite_in_place_type(T) nonstd::in_place_type<T>
#define nonstd_lite_in_place_index(K) nonstd::in_place_index<K>

} // namespace nonstd

#endif // variant_CPP17_OR_GREATER
#endif // nonstd_lite_HAVE_IN_PLACE_TYPES

//
// Use C++17 std::variant:
//

#if variant_USES_STD_VARIANT

#include <functional> // std::hash<>
#include <variant>

#if !variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
#define variant_size_V(T) nonstd::variant_size<T>::value
#endif

#if !variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
#define variant_alternative_T(K, T) typename nonstd::variant_alternative<K, T>::type
#endif

namespace nonstd {

using std::bad_variant_access;
using std::hash;
using std::monostate;
using std::variant;
using std::variant_alternative;
using std::variant_alternative_t;
using std::variant_size;
using std::variant_size_v;

using std::get;
using std::get_if;
using std::holds_alternative;
using std::visit;
using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;
using std::swap;

constexpr auto variant_npos = std::variant_npos;
} // namespace nonstd

#else // variant_USES_STD_VARIANT

#include <cstddef>
#include <limits>
#include <new>
#include <utility>

#if variant_CONFIG_NO_EXCEPTIONS
#include <cassert>
#else
#include <stdexcept>
#endif

// variant-lite type and visitor argument count configuration (script/generate_header.py):

#define variant_CONFIG_MAX_TYPE_COUNT 16
#define variant_CONFIG_MAX_VISITOR_ARG_COUNT 5

// variant-lite alignment configuration:

#ifndef variant_CONFIG_MAX_ALIGN_HACK
#define variant_CONFIG_MAX_ALIGN_HACK 0
#endif

#ifndef variant_CONFIG_ALIGN_AS
// no default, used in #if defined()
#endif

#ifndef variant_CONFIG_ALIGN_AS_FALLBACK
#define variant_CONFIG_ALIGN_AS_FALLBACK double
#endif

// half-open range [lo..hi):
#define variant_BETWEEN(v, lo, hi) ((lo) <= (v) && (v) < (hi))

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  variant_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  variant_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  variant_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  variant_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  variant_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  variant_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  variant_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  variant_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  variant_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  variant_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  variant_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if defined(_MSC_VER) && !defined(__clang__)
#define variant_COMPILER_MSVC_VER (_MSC_VER)
#define variant_COMPILER_MSVC_VERSION (_MSC_VER / 10 - 10 * (5 + (_MSC_VER < 1900)))
#else
#define variant_COMPILER_MSVC_VER 0
#define variant_COMPILER_MSVC_VERSION 0
#endif

#define variant_COMPILER_VERSION(major, minor, patch) (10 * (10 * (major) + (minor)) + (patch))

#if defined(__clang__)
#define variant_COMPILER_CLANG_VERSION                                                             \
  variant_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
#define variant_COMPILER_CLANG_VERSION 0
#endif

#if defined(__GNUC__) && !defined(__clang__)
#define variant_COMPILER_GNUC_VERSION                                                              \
  variant_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define variant_COMPILER_GNUC_VERSION 0
#endif

#if variant_BETWEEN(variant_COMPILER_MSVC_VER, 1300, 1900)
#pragma warning(push)
#pragma warning(disable : 4345) // initialization behavior changed
#endif

// Presence of language and library features:

#define variant_HAVE(feature) (variant_HAVE_##feature)

#ifdef _HAS_CPP0X
#define variant_HAS_CPP0X _HAS_CPP0X
#else
#define variant_HAS_CPP0X 0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for variant-lite:

#if variant_COMPILER_MSVC_VER >= 1900
#undef variant_CPP11_OR_GREATER
#define variant_CPP11_OR_GREATER 1
#endif

#define variant_CPP11_90 (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1500)
#define variant_CPP11_100 (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1600)
#define variant_CPP11_110 (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1700)
#define variant_CPP11_120 (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1800)
#define variant_CPP11_140 (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1900)
#define variant_CPP11_141 (variant_CPP11_OR_GREATER_ || variant_COMPILER_MSVC_VER >= 1910)

#define variant_CPP14_000 (variant_CPP14_OR_GREATER)
#define variant_CPP17_000 (variant_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define variant_HAVE_CONSTEXPR_11 variant_CPP11_140
#define variant_HAVE_INITIALIZER_LIST variant_CPP11_120
#define variant_HAVE_NOEXCEPT variant_CPP11_140
#define variant_HAVE_NULLPTR variant_CPP11_100
#define variant_HAVE_OVERRIDE variant_CPP11_140

// Presence of C++14 language features:

#define variant_HAVE_CONSTEXPR_14 variant_CPP14_000

// Presence of C++17 language features:

// no flag

// Presence of C++ library features:

#define variant_HAVE_CONDITIONAL variant_CPP11_120
#define variant_HAVE_REMOVE_CV variant_CPP11_120
#define variant_HAVE_STD_ADD_POINTER variant_CPP11_90
#define variant_HAVE_TYPE_TRAITS variant_CPP11_90

#define variant_HAVE_TR1_TYPE_TRAITS (!!variant_COMPILER_GNUC_VERSION)
#define variant_HAVE_TR1_ADD_POINTER (!!variant_COMPILER_GNUC_VERSION)

// C++ feature usage:

#if variant_HAVE_CONSTEXPR_11
#define variant_constexpr constexpr
#else
#define variant_constexpr /*constexpr*/
#endif

#if variant_HAVE_CONSTEXPR_14
#define variant_constexpr14 constexpr
#else
#define variant_constexpr14 /*constexpr*/
#endif

#if variant_HAVE_NOEXCEPT
#define variant_noexcept noexcept
#else
#define variant_noexcept /*noexcept*/
#endif

#if variant_HAVE_NULLPTR
#define variant_nullptr nullptr
#else
#define variant_nullptr NULL
#endif

#if variant_HAVE_OVERRIDE
#define variant_override override
#else
#define variant_override /*override*/
#endif

// additional includes:

#if variant_CPP11_OR_GREATER
#include <functional> // std::hash
#endif

#if variant_HAVE_INITIALIZER_LIST
#include <initializer_list>
#endif

#if variant_HAVE_TYPE_TRAITS
#include <type_traits>
#elif variant_HAVE_TR1_TYPE_TRAITS
#include <tr1/type_traits>
#endif

// Method enabling

#if variant_CPP11_OR_GREATER

#define variant_REQUIRES_0(...)                                                                    \
  template <bool B = (__VA_ARGS__), typename std::enable_if<B, int>::type = 0>

#define variant_REQUIRES_T(...) , typename std::enable_if<(__VA_ARGS__), int>::type = 0

#define variant_REQUIRES_R(R, ...) typename std::enable_if<(__VA_ARGS__), R>::type

#define variant_REQUIRES_A(...) , typename std::enable_if<(__VA_ARGS__), void *>::type = nullptr

#endif

//
// variant:
//

namespace nonstd {
namespace variants {

// C++11 emulation:

namespace std11 {

#if variant_HAVE_STD_ADD_POINTER

using std::add_pointer;

#elif variant_HAVE_TR1_ADD_POINTER

using std::tr1::add_pointer;

#else

template <class T> struct remove_reference { typedef T type; };
template <class T> struct remove_reference<T &> { typedef T type; };

template <class T> struct add_pointer { typedef typename remove_reference<T>::type *type; };

#endif // variant_HAVE_STD_ADD_POINTER

#if variant_HAVE_REMOVE_CV

using std::remove_cv;

#else

template <class T> struct remove_const { typedef T type; };
template <class T> struct remove_const<const T> { typedef T type; };

template <class T> struct remove_volatile { typedef T type; };
template <class T> struct remove_volatile<volatile T> { typedef T type; };

template <class T> struct remove_cv {
  typedef typename remove_volatile<typename remove_const<T>::type>::type type;
};

#endif // variant_HAVE_REMOVE_CV

#if variant_HAVE_CONDITIONAL

using std::conditional;

#else

template <bool Cond, class Then, class Else> struct conditional;

template <class Then, class Else> struct conditional<true, Then, Else> { typedef Then type; };

template <class Then, class Else> struct conditional<false, Then, Else> { typedef Else type; };

#endif // variant_HAVE_CONDITIONAL

} // namespace std11

/// type traits C++17:

namespace std17 {

#if variant_CPP17_OR_GREATER

using std::is_nothrow_swappable;
using std::is_swappable;

#elif variant_CPP11_OR_GREATER

namespace detail {

using std::swap;

struct is_swappable {
  template <typename T, typename = decltype(swap(std::declval<T &>(), std::declval<T &>()))>
  static std::true_type test(int);

  template <typename> static std::false_type test(...);
};

struct is_nothrow_swappable {
  // wrap noexcept(epr) in separate function as work-around for VC140 (VS2015):

  template <typename T> static constexpr bool test() {
    return noexcept(swap(std::declval<T &>(), std::declval<T &>()));
  }

  template <typename T> static auto test(int) -> std::integral_constant<bool, test<T>()> {}

  template <typename> static std::false_type test(...);
};

} // namespace detail

// is [nothow] swappable:

template <typename T> struct is_swappable : decltype(detail::is_swappable::test<T>(0)) {};

template <typename T>
struct is_nothrow_swappable : decltype(detail::is_nothrow_swappable::test<T>(0)) {};

#endif // variant_CPP17_OR_GREATER

} // namespace std17

// detail:

namespace detail {

// typelist:

#define variant_TL1(T1) detail::typelist<T1, detail::nulltype>
#define variant_TL2(T1, T2) detail::typelist<T1, variant_TL1(T2)>
#define variant_TL3(T1, T2, T3) detail::typelist<T1, variant_TL2(T2, T3)>
#define variant_TL4(T1, T2, T3, T4) detail::typelist<T1, variant_TL3(T2, T3, T4)>
#define variant_TL5(T1, T2, T3, T4, T5) detail::typelist<T1, variant_TL4(T2, T3, T4, T5)>
#define variant_TL6(T1, T2, T3, T4, T5, T6) detail::typelist<T1, variant_TL5(T2, T3, T4, T5, T6)>
#define variant_TL7(T1, T2, T3, T4, T5, T6, T7)                                                    \
  detail::typelist<T1, variant_TL6(T2, T3, T4, T5, T6, T7)>
#define variant_TL8(T1, T2, T3, T4, T5, T6, T7, T8)                                                \
  detail::typelist<T1, variant_TL7(T2, T3, T4, T5, T6, T7, T8)>
#define variant_TL9(T1, T2, T3, T4, T5, T6, T7, T8, T9)                                            \
  detail::typelist<T1, variant_TL8(T2, T3, T4, T5, T6, T7, T8, T9)>
#define variant_TL10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10)                                      \
  detail::typelist<T1, variant_TL9(T2, T3, T4, T5, T6, T7, T8, T9, T10)>
#define variant_TL11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)                                 \
  detail::typelist<T1, variant_TL10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11)>
#define variant_TL12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)                            \
  detail::typelist<T1, variant_TL11(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12)>
#define variant_TL13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)                       \
  detail::typelist<T1, variant_TL12(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13)>
#define variant_TL14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)                  \
  detail::typelist<T1, variant_TL13(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14)>
#define variant_TL15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)             \
  detail::typelist<T1, variant_TL14(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15)>
#define variant_TL16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15, T16)        \
  detail::typelist<T1, variant_TL15(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15,  \
                                    T16)>

// variant parameter unused type tags:

template <class T> struct TX : T {
  inline TX<T> operator+() const { return TX<T>(); }
  inline TX<T> operator-() const { return TX<T>(); }

  inline TX<T> operator!() const { return TX<T>(); }
  inline TX<T> operator~() const { return TX<T>(); }

  inline TX<T> *operator&() const { return variant_nullptr; }

  template <class U> inline TX<T> operator*(U const &)const { return TX<T>(); }
  template <class U> inline TX<T> operator/(U const &) const { return TX<T>(); }

  template <class U> inline TX<T> operator%(U const &) const { return TX<T>(); }
  template <class U> inline TX<T> operator+(U const &) const { return TX<T>(); }
  template <class U> inline TX<T> operator-(U const &) const { return TX<T>(); }

  template <class U> inline TX<T> operator<<(U const &) const { return TX<T>(); }
  template <class U> inline TX<T> operator>>(U const &) const { return TX<T>(); }

  inline bool operator==(T const &) const { return false; }
  inline bool operator<(T const &) const { return false; }

  template <class U> inline TX<T> operator&(U const &)const { return TX<T>(); }
  template <class U> inline TX<T> operator|(U const &) const { return TX<T>(); }
  template <class U> inline TX<T> operator^(U const &) const { return TX<T>(); }

  template <class U> inline TX<T> operator&&(U const &) const { return TX<T>(); }
  template <class U> inline TX<T> operator||(U const &) const { return TX<T>(); }
};

struct S0 {};
typedef TX<S0> T0;
struct S1 {};
typedef TX<S1> T1;
struct S2 {};
typedef TX<S2> T2;
struct S3 {};
typedef TX<S3> T3;
struct S4 {};
typedef TX<S4> T4;
struct S5 {};
typedef TX<S5> T5;
struct S6 {};
typedef TX<S6> T6;
struct S7 {};
typedef TX<S7> T7;
struct S8 {};
typedef TX<S8> T8;
struct S9 {};
typedef TX<S9> T9;
struct S10 {};
typedef TX<S10> T10;
struct S11 {};
typedef TX<S11> T11;
struct S12 {};
typedef TX<S12> T12;
struct S13 {};
typedef TX<S13> T13;
struct S14 {};
typedef TX<S14> T14;
struct S15 {};
typedef TX<S15> T15;

struct nulltype {};

template <class Head, class Tail> struct typelist {
  typedef Head head;
  typedef Tail tail;
};

// typelist max element size:

template <class List> struct typelist_max;

template <> struct typelist_max<nulltype> {
  enum V { value = 0 };
  typedef void type;
};

template <class Head, class Tail> struct typelist_max<typelist<Head, Tail>> {
private:
  enum TV { tail_value = size_t(typelist_max<Tail>::value) };

  typedef typename typelist_max<Tail>::type tail_type;

public:
  enum V { value = (sizeof(Head) > tail_value) ? sizeof(Head) : std::size_t(tail_value) };

  typedef typename std11::conditional<(sizeof(Head) > tail_value), Head, tail_type>::type type;
};

#if variant_CPP11_OR_GREATER

// typelist max alignof element type:

template <class List> struct typelist_max_alignof;

template <> struct typelist_max_alignof<nulltype> {
  enum V { value = 0 };
};

template <class Head, class Tail> struct typelist_max_alignof<typelist<Head, Tail>> {
private:
  enum TV { tail_value = size_t(typelist_max_alignof<Tail>::value) };

public:
  enum V { value = (alignof(Head) > tail_value) ? alignof(Head) : std::size_t(tail_value) };
};

#endif

// typelist size (length):

template <class List> struct typelist_size {
  enum V { value = 1 };
};

template <> struct typelist_size<T0> {
  enum V { value = 0 };
};
template <> struct typelist_size<T1> {
  enum V { value = 0 };
};
template <> struct typelist_size<T2> {
  enum V { value = 0 };
};
template <> struct typelist_size<T3> {
  enum V { value = 0 };
};
template <> struct typelist_size<T4> {
  enum V { value = 0 };
};
template <> struct typelist_size<T5> {
  enum V { value = 0 };
};
template <> struct typelist_size<T6> {
  enum V { value = 0 };
};
template <> struct typelist_size<T7> {
  enum V { value = 0 };
};
template <> struct typelist_size<T8> {
  enum V { value = 0 };
};
template <> struct typelist_size<T9> {
  enum V { value = 0 };
};
template <> struct typelist_size<T10> {
  enum V { value = 0 };
};
template <> struct typelist_size<T11> {
  enum V { value = 0 };
};
template <> struct typelist_size<T12> {
  enum V { value = 0 };
};
template <> struct typelist_size<T13> {
  enum V { value = 0 };
};
template <> struct typelist_size<T14> {
  enum V { value = 0 };
};
template <> struct typelist_size<T15> {
  enum V { value = 0 };
};

template <> struct typelist_size<nulltype> {
  enum V { value = 0 };
};

template <class Head, class Tail> struct typelist_size<typelist<Head, Tail>> {
  enum V { value = typelist_size<Head>::value + typelist_size<Tail>::value };
};

// typelist index of type:

template <class List, class T> struct typelist_index_of;

template <class T> struct typelist_index_of<nulltype, T> {
  enum V { value = -1 };
};

template <class Tail, class T> struct typelist_index_of<typelist<T, Tail>, T> {
  enum V { value = 0 };
};

template <class Head, class Tail, class T> struct typelist_index_of<typelist<Head, Tail>, T> {
private:
  enum TV { nextVal = typelist_index_of<Tail, T>::value };

public:
  enum V { value = nextVal == -1 ? -1 : 1 + nextVal };
};

// typelist type at index:

template <class List, std::size_t i> struct typelist_type_at;

template <class Head, class Tail> struct typelist_type_at<typelist<Head, Tail>, 0> {
  typedef Head type;
};

template <class Head, class Tail, std::size_t i> struct typelist_type_at<typelist<Head, Tail>, i> {
  typedef typename typelist_type_at<Tail, i - 1>::type type;
};

#if variant_CONFIG_MAX_ALIGN_HACK

// Max align, use most restricted type for alignment:

#define variant_UNIQUE(name) variant_UNIQUE2(name, __LINE__)
#define variant_UNIQUE2(name, line) variant_UNIQUE3(name, line)
#define variant_UNIQUE3(name, line) name##line

#define variant_ALIGN_TYPE(type)                                                                   \
  type variant_UNIQUE(_t);                                                                         \
  struct_t<type> variant_UNIQUE(_st)

template <class T> struct struct_t { T _; };

union max_align_t {
  variant_ALIGN_TYPE(char);
  variant_ALIGN_TYPE(short int);
  variant_ALIGN_TYPE(int);
  variant_ALIGN_TYPE(long int);
  variant_ALIGN_TYPE(float);
  variant_ALIGN_TYPE(double);
  variant_ALIGN_TYPE(long double);
  variant_ALIGN_TYPE(char *);
  variant_ALIGN_TYPE(short int *);
  variant_ALIGN_TYPE(int *);
  variant_ALIGN_TYPE(long int *);
  variant_ALIGN_TYPE(float *);
  variant_ALIGN_TYPE(double *);
  variant_ALIGN_TYPE(long double *);
  variant_ALIGN_TYPE(void *);

#ifdef HAVE_LONG_LONG
  variant_ALIGN_TYPE(long long);
#endif

  struct Unknown;

  Unknown (*variant_UNIQUE(_))(Unknown);
  Unknown *Unknown::*variant_UNIQUE(_);
  Unknown (Unknown::*variant_UNIQUE(_))(Unknown);

  struct_t<Unknown (*)(Unknown)> variant_UNIQUE(_);
  struct_t<Unknown * Unknown::*> variant_UNIQUE(_);
  struct_t<Unknown (Unknown::*)(Unknown)> variant_UNIQUE(_);
};

#undef variant_UNIQUE
#undef variant_UNIQUE2
#undef variant_UNIQUE3

#undef variant_ALIGN_TYPE

#elif defined(variant_CONFIG_ALIGN_AS) // variant_CONFIG_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define variant_ALIGN_AS(unused) variant_CONFIG_ALIGN_AS

#else // variant_CONFIG_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define variant_ALIGN_AS(to_align)                                                                 \
  typename detail::type_of_size<detail::alignment_types,                                           \
                                detail::alignment_of<to_align>::value>::type

template <typename T> struct alignment_of;

template <typename T> struct alignment_of_hack {
  char c;
  T t;
  alignment_of_hack();
};

template <size_t A, size_t S> struct alignment_logic {
  enum V { value = A < S ? A : S };
};

template <typename T> struct alignment_of {
  enum V { value = alignment_logic<sizeof(alignment_of_hack<T>) - sizeof(T), sizeof(T)>::value };
};

template <typename List, size_t N> struct type_of_size {
  typedef
      typename std11::conditional<N == sizeof(typename List::head), typename List::head,
                                  typename type_of_size<typename List::tail, N>::type>::type type;
};

template <size_t N> struct type_of_size<nulltype, N> {
  typedef variant_CONFIG_ALIGN_AS_FALLBACK type;
};

template <typename T> struct struct_t { T _; };

#define variant_ALIGN_TYPE(type) typelist < type, typelist < struct_t<type>

struct Unknown;

typedef variant_ALIGN_TYPE(char), variant_ALIGN_TYPE(short), variant_ALIGN_TYPE(int),
    variant_ALIGN_TYPE(long), variant_ALIGN_TYPE(float), variant_ALIGN_TYPE(double),
    variant_ALIGN_TYPE(long double),

    variant_ALIGN_TYPE(char *), variant_ALIGN_TYPE(short *), variant_ALIGN_TYPE(int *),
    variant_ALIGN_TYPE(long *), variant_ALIGN_TYPE(float *), variant_ALIGN_TYPE(double *),
    variant_ALIGN_TYPE(long double *),

    variant_ALIGN_TYPE(Unknown (*)(Unknown)), variant_ALIGN_TYPE(Unknown *Unknown::*),
    variant_ALIGN_TYPE(Unknown (Unknown::*)(Unknown)),

    nulltype >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> alignment_types;

#undef variant_ALIGN_TYPE

#endif // variant_CONFIG_MAX_ALIGN_HACK

#if variant_CPP11_OR_GREATER

template <typename T> inline std::size_t hash(T const &v) { return std::hash<T>()(v); }

inline std::size_t hash(T0 const &) { return 0; }
inline std::size_t hash(T1 const &) { return 0; }
inline std::size_t hash(T2 const &) { return 0; }
inline std::size_t hash(T3 const &) { return 0; }
inline std::size_t hash(T4 const &) { return 0; }
inline std::size_t hash(T5 const &) { return 0; }
inline std::size_t hash(T6 const &) { return 0; }
inline std::size_t hash(T7 const &) { return 0; }
inline std::size_t hash(T8 const &) { return 0; }
inline std::size_t hash(T9 const &) { return 0; }
inline std::size_t hash(T10 const &) { return 0; }
inline std::size_t hash(T11 const &) { return 0; }
inline std::size_t hash(T12 const &) { return 0; }
inline std::size_t hash(T13 const &) { return 0; }
inline std::size_t hash(T14 const &) { return 0; }
inline std::size_t hash(T15 const &) { return 0; }

#endif // variant_CPP11_OR_GREATER

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
struct helper {
  typedef signed char type_index_t;
  typedef variant_TL16(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                       T15) variant_types;

  template <class U> static U *as(void *data) { return reinterpret_cast<U *>(data); }

  template <class U> static U const *as(void const *data) {
    return reinterpret_cast<const U *>(data);
  }

  static type_index_t to_index_t(std::size_t index) { return static_cast<type_index_t>(index); }

  static void destroy(type_index_t index, void *data) {
    switch (index) {
    case 0:
      as<T0>(data)->~T0();
      break;
    case 1:
      as<T1>(data)->~T1();
      break;
    case 2:
      as<T2>(data)->~T2();
      break;
    case 3:
      as<T3>(data)->~T3();
      break;
    case 4:
      as<T4>(data)->~T4();
      break;
    case 5:
      as<T5>(data)->~T5();
      break;
    case 6:
      as<T6>(data)->~T6();
      break;
    case 7:
      as<T7>(data)->~T7();
      break;
    case 8:
      as<T8>(data)->~T8();
      break;
    case 9:
      as<T9>(data)->~T9();
      break;
    case 10:
      as<T10>(data)->~T10();
      break;
    case 11:
      as<T11>(data)->~T11();
      break;
    case 12:
      as<T12>(data)->~T12();
      break;
    case 13:
      as<T13>(data)->~T13();
      break;
    case 14:
      as<T14>(data)->~T14();
      break;
    case 15:
      as<T15>(data)->~T15();
      break;
    }
  }

#if variant_CPP11_OR_GREATER
  template <class T, class... Args> static type_index_t construct_t(void *data, Args &&... args) {
    new (data) T(std::forward<Args>(args)...);

    return to_index_t(detail::typelist_index_of<variant_types, T>::value);
  }

  template <std::size_t K, class... Args>
  static type_index_t construct_i(void *data, Args &&... args) {
    using type = typename detail::typelist_type_at<variant_types, K>::type;

    construct_t<type>(data, std::forward<Args>(args)...);

    return to_index_t(K);
  }

  static type_index_t move_construct(type_index_t const from_index, void *from_value,
                                     void *to_value) {
    switch (from_index) {
    case 0:
      new (to_value) T0(std::move(*as<T0>(from_value)));
      break;
    case 1:
      new (to_value) T1(std::move(*as<T1>(from_value)));
      break;
    case 2:
      new (to_value) T2(std::move(*as<T2>(from_value)));
      break;
    case 3:
      new (to_value) T3(std::move(*as<T3>(from_value)));
      break;
    case 4:
      new (to_value) T4(std::move(*as<T4>(from_value)));
      break;
    case 5:
      new (to_value) T5(std::move(*as<T5>(from_value)));
      break;
    case 6:
      new (to_value) T6(std::move(*as<T6>(from_value)));
      break;
    case 7:
      new (to_value) T7(std::move(*as<T7>(from_value)));
      break;
    case 8:
      new (to_value) T8(std::move(*as<T8>(from_value)));
      break;
    case 9:
      new (to_value) T9(std::move(*as<T9>(from_value)));
      break;
    case 10:
      new (to_value) T10(std::move(*as<T10>(from_value)));
      break;
    case 11:
      new (to_value) T11(std::move(*as<T11>(from_value)));
      break;
    case 12:
      new (to_value) T12(std::move(*as<T12>(from_value)));
      break;
    case 13:
      new (to_value) T13(std::move(*as<T13>(from_value)));
      break;
    case 14:
      new (to_value) T14(std::move(*as<T14>(from_value)));
      break;
    case 15:
      new (to_value) T15(std::move(*as<T15>(from_value)));
      break;
    }
    return from_index;
  }

  static type_index_t move_assign(type_index_t const from_index, void *from_value, void *to_value) {
    switch (from_index) {
    case 0:
      *as<T0>(to_value) = std::move(*as<T0>(from_value));
      break;
    case 1:
      *as<T1>(to_value) = std::move(*as<T1>(from_value));
      break;
    case 2:
      *as<T2>(to_value) = std::move(*as<T2>(from_value));
      break;
    case 3:
      *as<T3>(to_value) = std::move(*as<T3>(from_value));
      break;
    case 4:
      *as<T4>(to_value) = std::move(*as<T4>(from_value));
      break;
    case 5:
      *as<T5>(to_value) = std::move(*as<T5>(from_value));
      break;
    case 6:
      *as<T6>(to_value) = std::move(*as<T6>(from_value));
      break;
    case 7:
      *as<T7>(to_value) = std::move(*as<T7>(from_value));
      break;
    case 8:
      *as<T8>(to_value) = std::move(*as<T8>(from_value));
      break;
    case 9:
      *as<T9>(to_value) = std::move(*as<T9>(from_value));
      break;
    case 10:
      *as<T10>(to_value) = std::move(*as<T10>(from_value));
      break;
    case 11:
      *as<T11>(to_value) = std::move(*as<T11>(from_value));
      break;
    case 12:
      *as<T12>(to_value) = std::move(*as<T12>(from_value));
      break;
    case 13:
      *as<T13>(to_value) = std::move(*as<T13>(from_value));
      break;
    case 14:
      *as<T14>(to_value) = std::move(*as<T14>(from_value));
      break;
    case 15:
      *as<T15>(to_value) = std::move(*as<T15>(from_value));
      break;
    }
    return from_index;
  }
#endif

  static type_index_t copy_construct(type_index_t const from_index, const void *from_value,
                                     void *to_value) {
    switch (from_index) {
    case 0:
      new (to_value) T0(*as<T0>(from_value));
      break;
    case 1:
      new (to_value) T1(*as<T1>(from_value));
      break;
    case 2:
      new (to_value) T2(*as<T2>(from_value));
      break;
    case 3:
      new (to_value) T3(*as<T3>(from_value));
      break;
    case 4:
      new (to_value) T4(*as<T4>(from_value));
      break;
    case 5:
      new (to_value) T5(*as<T5>(from_value));
      break;
    case 6:
      new (to_value) T6(*as<T6>(from_value));
      break;
    case 7:
      new (to_value) T7(*as<T7>(from_value));
      break;
    case 8:
      new (to_value) T8(*as<T8>(from_value));
      break;
    case 9:
      new (to_value) T9(*as<T9>(from_value));
      break;
    case 10:
      new (to_value) T10(*as<T10>(from_value));
      break;
    case 11:
      new (to_value) T11(*as<T11>(from_value));
      break;
    case 12:
      new (to_value) T12(*as<T12>(from_value));
      break;
    case 13:
      new (to_value) T13(*as<T13>(from_value));
      break;
    case 14:
      new (to_value) T14(*as<T14>(from_value));
      break;
    case 15:
      new (to_value) T15(*as<T15>(from_value));
      break;
    }
    return from_index;
  }

  static type_index_t copy_assign(type_index_t const from_index, const void *from_value,
                                  void *to_value) {
    switch (from_index) {
    case 0:
      *as<T0>(to_value) = *as<T0>(from_value);
      break;
    case 1:
      *as<T1>(to_value) = *as<T1>(from_value);
      break;
    case 2:
      *as<T2>(to_value) = *as<T2>(from_value);
      break;
    case 3:
      *as<T3>(to_value) = *as<T3>(from_value);
      break;
    case 4:
      *as<T4>(to_value) = *as<T4>(from_value);
      break;
    case 5:
      *as<T5>(to_value) = *as<T5>(from_value);
      break;
    case 6:
      *as<T6>(to_value) = *as<T6>(from_value);
      break;
    case 7:
      *as<T7>(to_value) = *as<T7>(from_value);
      break;
    case 8:
      *as<T8>(to_value) = *as<T8>(from_value);
      break;
    case 9:
      *as<T9>(to_value) = *as<T9>(from_value);
      break;
    case 10:
      *as<T10>(to_value) = *as<T10>(from_value);
      break;
    case 11:
      *as<T11>(to_value) = *as<T11>(from_value);
      break;
    case 12:
      *as<T12>(to_value) = *as<T12>(from_value);
      break;
    case 13:
      *as<T13>(to_value) = *as<T13>(from_value);
      break;
    case 14:
      *as<T14>(to_value) = *as<T14>(from_value);
      break;
    case 15:
      *as<T15>(to_value) = *as<T15>(from_value);
      break;
    }
    return from_index;
  }
};

} // namespace detail

//
// Variant:
//

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
class variant;

// 19.7.8 Class monostate

class monostate {};

// 19.7.9 monostate relational operators

inline variant_constexpr bool operator<(monostate, monostate) variant_noexcept { return false; }
inline variant_constexpr bool operator>(monostate, monostate) variant_noexcept { return false; }
inline variant_constexpr bool operator<=(monostate, monostate) variant_noexcept { return true; }
inline variant_constexpr bool operator>=(monostate, monostate) variant_noexcept { return true; }
inline variant_constexpr bool operator==(monostate, monostate) variant_noexcept { return true; }
inline variant_constexpr bool operator!=(monostate, monostate) variant_noexcept { return false; }

// 19.7.4 variant helper classes

// obtain the size of the variant's list of alternatives at compile time

template <class T> struct variant_size; /* undefined */

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
struct variant_size<variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>> {
  enum _ {
    value = detail::typelist_size<variant_TL16(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11,
                                               T12, T13, T14, T15)>::value
  };
};

#if variant_CPP14_OR_GREATER
template <class T> constexpr std::size_t variant_size_v = variant_size<T>::value;
#endif

#if !variant_CONFIG_OMIT_VARIANT_SIZE_V_MACRO
#define variant_size_V(T) nonstd::variant_size<T>::value
#endif

// obtain the type of the alternative specified by its index, at compile time:

template <std::size_t K, class T> struct variant_alternative; /* undefined */

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
struct variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>> {
  typedef typename detail::typelist_type_at<variant_TL16(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9,
                                                         T10, T11, T12, T13, T14, T15),
                                            K>::type type;
};

#if variant_CPP11_OR_GREATER
template <std::size_t K, class T>
using variant_alternative_t = typename variant_alternative<K, T>::type;
#endif

#if !variant_CONFIG_OMIT_VARIANT_ALTERNATIVE_T_MACRO
#define variant_alternative_T(K, T) typename nonstd::variant_alternative<K, T>::type
#endif

// NTS:implement specializes the std::uses_allocator type trait
// std::uses_allocator<nonstd::variant>

// index of the variant in the invalid state (constant)

#if variant_CPP11_OR_GREATER
variant_constexpr std::size_t variant_npos = static_cast<std::size_t>(-1);
#else
static const std::size_t variant_npos = static_cast<std::size_t>(-1);
#endif

#if !variant_CONFIG_NO_EXCEPTIONS

// 19.7.11 Class bad_variant_access

class bad_variant_access : public std::exception {
public:
#if variant_CPP11_OR_GREATER
  virtual const char *what() const variant_noexcept variant_override
#else
  virtual const char *what() const throw()
#endif
  {
    return "bad variant access";
  }
};

#endif // variant_CONFIG_NO_EXCEPTIONS

// 19.7.3 Class template variant

template <class T0, class T1 = detail::T1, class T2 = detail::T2, class T3 = detail::T3,
          class T4 = detail::T4, class T5 = detail::T5, class T6 = detail::T6,
          class T7 = detail::T7, class T8 = detail::T8, class T9 = detail::T9,
          class T10 = detail::T10, class T11 = detail::T11, class T12 = detail::T12,
          class T13 = detail::T13, class T14 = detail::T14, class T15 = detail::T15>
class variant {
  typedef detail::helper<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>
      helper_type;
  typedef variant_TL16(T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                       T15) variant_types;

public:
  // 19.7.3.1 Constructors

  variant() : type_index(0) { new (ptr()) T0(); }

  variant(T0 const &t0) : type_index(0) { new (ptr()) T0(t0); }
  variant(T1 const &t1) : type_index(1) { new (ptr()) T1(t1); }
  variant(T2 const &t2) : type_index(2) { new (ptr()) T2(t2); }
  variant(T3 const &t3) : type_index(3) { new (ptr()) T3(t3); }
  variant(T4 const &t4) : type_index(4) { new (ptr()) T4(t4); }
  variant(T5 const &t5) : type_index(5) { new (ptr()) T5(t5); }
  variant(T6 const &t6) : type_index(6) { new (ptr()) T6(t6); }
  variant(T7 const &t7) : type_index(7) { new (ptr()) T7(t7); }
  variant(T8 const &t8) : type_index(8) { new (ptr()) T8(t8); }
  variant(T9 const &t9) : type_index(9) { new (ptr()) T9(t9); }
  variant(T10 const &t10) : type_index(10) { new (ptr()) T10(t10); }
  variant(T11 const &t11) : type_index(11) { new (ptr()) T11(t11); }
  variant(T12 const &t12) : type_index(12) { new (ptr()) T12(t12); }
  variant(T13 const &t13) : type_index(13) { new (ptr()) T13(t13); }
  variant(T14 const &t14) : type_index(14) { new (ptr()) T14(t14); }
  variant(T15 const &t15) : type_index(15) { new (ptr()) T15(t15); }

#if variant_CPP11_OR_GREATER
  variant(T0 &&t0) : type_index(0) { new (ptr()) T0(std::move(t0)); }
  variant(T1 &&t1) : type_index(1) { new (ptr()) T1(std::move(t1)); }
  variant(T2 &&t2) : type_index(2) { new (ptr()) T2(std::move(t2)); }
  variant(T3 &&t3) : type_index(3) { new (ptr()) T3(std::move(t3)); }
  variant(T4 &&t4) : type_index(4) { new (ptr()) T4(std::move(t4)); }
  variant(T5 &&t5) : type_index(5) { new (ptr()) T5(std::move(t5)); }
  variant(T6 &&t6) : type_index(6) { new (ptr()) T6(std::move(t6)); }
  variant(T7 &&t7) : type_index(7) { new (ptr()) T7(std::move(t7)); }
  variant(T8 &&t8) : type_index(8) { new (ptr()) T8(std::move(t8)); }
  variant(T9 &&t9) : type_index(9) { new (ptr()) T9(std::move(t9)); }
  variant(T10 &&t10) : type_index(10) { new (ptr()) T10(std::move(t10)); }
  variant(T11 &&t11) : type_index(11) { new (ptr()) T11(std::move(t11)); }
  variant(T12 &&t12) : type_index(12) { new (ptr()) T12(std::move(t12)); }
  variant(T13 &&t13) : type_index(13) { new (ptr()) T13(std::move(t13)); }
  variant(T14 &&t14) : type_index(14) { new (ptr()) T14(std::move(t14)); }
  variant(T15 &&t15) : type_index(15) { new (ptr()) T15(std::move(t15)); }

#endif

  variant(variant const &other) : type_index(other.type_index) {
    (void)helper_type::copy_construct(other.type_index, other.ptr(), ptr());
  }

#if variant_CPP11_OR_GREATER

  variant(variant &&other) noexcept(
      std::is_nothrow_move_constructible<T0>::value &&std::is_nothrow_move_constructible<T1>::value
          &&std::is_nothrow_move_constructible<T2>::value &&std::is_nothrow_move_constructible<
              T3>::value &&std::is_nothrow_move_constructible<T4>::value
              &&std::is_nothrow_move_constructible<T5>::value &&std::is_nothrow_move_constructible<
                  T6>::value &&std::is_nothrow_move_constructible<T7>::value
                  &&std::is_nothrow_move_constructible<T8>::value
                      &&std::is_nothrow_move_constructible<T9>::value
                          &&std::is_nothrow_move_constructible<T10>::value
                              &&std::is_nothrow_move_constructible<T11>::value
                                  &&std::is_nothrow_move_constructible<T12>::value
                                      &&std::is_nothrow_move_constructible<T13>::value
                                          &&std::is_nothrow_move_constructible<T14>::value
                                              &&std::is_nothrow_move_constructible<T15>::value)
      : type_index(other.type_index) {
    (void)helper_type::move_construct(other.type_index, other.ptr(), ptr());
  }

  template <std::size_t K>
  using type_at_t = typename detail::typelist_type_at<variant_types, K>::type;

  template <class T, class... Args variant_REQUIRES_T(std::is_constructible<T, Args...>::value)>
  explicit variant(nonstd_lite_in_place_type_t(T), Args &&... args) {
    type_index = variant_npos_internal();
    type_index = helper_type::template construct_t<T>(ptr(), std::forward<Args>(args)...);
  }

  template <class T, class U,
            class... Args variant_REQUIRES_T(
                std::is_constructible<T, std::initializer_list<U> &, Args...>::value)>
  explicit variant(nonstd_lite_in_place_type_t(T), std::initializer_list<U> il, Args &&... args) {
    type_index = variant_npos_internal();
    type_index = helper_type::template construct_t<T>(ptr(), il, std::forward<Args>(args)...);
  }

  template <std::size_t K,
            class... Args variant_REQUIRES_T(std::is_constructible<type_at_t<K>, Args...>::value)>
  explicit variant(nonstd_lite_in_place_index_t(K), Args &&... args) {
    type_index = variant_npos_internal();
    type_index = helper_type::template construct_i<K>(ptr(), std::forward<Args>(args)...);
  }

  template <size_t K, class U,
            class... Args variant_REQUIRES_T(
                std::is_constructible<type_at_t<K>, std::initializer_list<U> &, Args...>::value)>
  explicit variant(nonstd_lite_in_place_index_t(K), std::initializer_list<U> il, Args &&... args) {
    type_index = variant_npos_internal();
    type_index = helper_type::template construct_i<K>(ptr(), il, std::forward<Args>(args)...);
  }

#endif // variant_CPP11_OR_GREATER

  // 19.7.3.2 Destructor

  ~variant() {
    if (!valueless_by_exception()) {
      helper_type::destroy(type_index, ptr());
    }
  }

  // 19.7.3.3 Assignment

  variant &operator=(variant const &other) { return copy_assign(other); }

#if variant_CPP11_OR_GREATER

  variant &operator=(variant &&other) noexcept(
      std::is_nothrow_move_assignable<T0>::value &&std::is_nothrow_move_assignable<T1>::value
          &&std::is_nothrow_move_assignable<T2>::value &&std::is_nothrow_move_assignable<T3>::value
              &&std::is_nothrow_move_assignable<T4>::value &&std::is_nothrow_move_assignable<
                  T5>::value &&std::is_nothrow_move_assignable<T6>::value
                  &&std::is_nothrow_move_assignable<T7>::value &&std::is_nothrow_move_assignable<
                      T8>::value &&std::is_nothrow_move_assignable<T9>::value &&
                      std::is_nothrow_move_assignable<T10>::value &&std::is_nothrow_move_assignable<
                          T11>::value &&std::is_nothrow_move_assignable<T12>::value
                          &&std::is_nothrow_move_assignable<T13>::value
                              &&std::is_nothrow_move_assignable<T14>::value
                                  &&std::is_nothrow_move_assignable<T15>::value) {
    return move_assign(std::move(other));
  }

  variant &operator=(T0 &&t0) { return assign_value<0>(std::move(t0)); }
  variant &operator=(T1 &&t1) { return assign_value<1>(std::move(t1)); }
  variant &operator=(T2 &&t2) { return assign_value<2>(std::move(t2)); }
  variant &operator=(T3 &&t3) { return assign_value<3>(std::move(t3)); }
  variant &operator=(T4 &&t4) { return assign_value<4>(std::move(t4)); }
  variant &operator=(T5 &&t5) { return assign_value<5>(std::move(t5)); }
  variant &operator=(T6 &&t6) { return assign_value<6>(std::move(t6)); }
  variant &operator=(T7 &&t7) { return assign_value<7>(std::move(t7)); }
  variant &operator=(T8 &&t8) { return assign_value<8>(std::move(t8)); }
  variant &operator=(T9 &&t9) { return assign_value<9>(std::move(t9)); }
  variant &operator=(T10 &&t10) { return assign_value<10>(std::move(t10)); }
  variant &operator=(T11 &&t11) { return assign_value<11>(std::move(t11)); }
  variant &operator=(T12 &&t12) { return assign_value<12>(std::move(t12)); }
  variant &operator=(T13 &&t13) { return assign_value<13>(std::move(t13)); }
  variant &operator=(T14 &&t14) { return assign_value<14>(std::move(t14)); }
  variant &operator=(T15 &&t15) { return assign_value<15>(std::move(t15)); }

#endif

  variant &operator=(T0 const &t0) { return assign_value<0>(t0); }
  variant &operator=(T1 const &t1) { return assign_value<1>(t1); }
  variant &operator=(T2 const &t2) { return assign_value<2>(t2); }
  variant &operator=(T3 const &t3) { return assign_value<3>(t3); }
  variant &operator=(T4 const &t4) { return assign_value<4>(t4); }
  variant &operator=(T5 const &t5) { return assign_value<5>(t5); }
  variant &operator=(T6 const &t6) { return assign_value<6>(t6); }
  variant &operator=(T7 const &t7) { return assign_value<7>(t7); }
  variant &operator=(T8 const &t8) { return assign_value<8>(t8); }
  variant &operator=(T9 const &t9) { return assign_value<9>(t9); }
  variant &operator=(T10 const &t10) { return assign_value<10>(t10); }
  variant &operator=(T11 const &t11) { return assign_value<11>(t11); }
  variant &operator=(T12 const &t12) { return assign_value<12>(t12); }
  variant &operator=(T13 const &t13) { return assign_value<13>(t13); }
  variant &operator=(T14 const &t14) { return assign_value<14>(t14); }
  variant &operator=(T15 const &t15) { return assign_value<15>(t15); }

  std::size_t index() const {
    return variant_npos_internal() == type_index ? variant_npos
                                                 : static_cast<std::size_t>(type_index);
  }

  // 19.7.3.4 Modifiers

#if variant_CPP11_OR_GREATER
  template <class T, class... Args variant_REQUIRES_T(std::is_constructible<T, Args...>::value)>
  T &emplace(Args &&... args) {
    helper_type::destroy(type_index, ptr());
    type_index = variant_npos_internal();
    type_index = helper_type::template construct_t<T>(ptr(), std::forward<Args>(args)...);

    return *as<T>();
  }

  template <class T, class U,
            class... Args variant_REQUIRES_T(
                std::is_constructible<T, std::initializer_list<U> &, Args...>::value)>
  T &emplace(std::initializer_list<U> il, Args &&... args) {
    helper_type::destroy(type_index, ptr());
    type_index = variant_npos_internal();
    type_index = helper_type::template construct_t<T>(ptr(), il, std::forward<Args>(args)...);

    return *as<T>();
  }

  template <size_t K,
            class... Args variant_REQUIRES_T(std::is_constructible<type_at_t<K>, Args...>::value)>
  variant_alternative_t<K, variant> &emplace(Args &&... args) {
    return this->template emplace<type_at_t<K>>(std::forward<Args>(args)...);
  }

  template <size_t K, class U,
            class... Args variant_REQUIRES_T(
                std::is_constructible<type_at_t<K>, std::initializer_list<U> &, Args...>::value)>
  variant_alternative_t<K, variant> &emplace(std::initializer_list<U> il, Args &&... args) {
    return this->template emplace<type_at_t<K>>(il, std::forward<Args>(args)...);
  }

#endif // variant_CPP11_OR_GREATER

  // 19.7.3.5 Value status

  bool valueless_by_exception() const { return type_index == variant_npos_internal(); }

  // 19.7.3.6 Swap

  void swap(variant &other)
#if variant_CPP11_OR_GREATER
      noexcept(
          std::is_nothrow_move_constructible<T0>::value &&std17::is_nothrow_swappable<
              T0>::value &&std::is_nothrow_move_constructible<T1>::value
              &&std17::is_nothrow_swappable<T1>::value &&std::is_nothrow_move_constructible<
                  T2>::value &&std17::is_nothrow_swappable<T2>::value
                  &&std::is_nothrow_move_constructible<T3>::value &&std17::is_nothrow_swappable<
                      T3>::value &&std::is_nothrow_move_constructible<T4>::value
                      &&std17::is_nothrow_swappable<T4>::value &&std::is_nothrow_move_constructible<
                          T5>::value &&std17::is_nothrow_swappable<T5>::value &&std::
                          is_nothrow_move_constructible<T6>::value &&std17::is_nothrow_swappable<
                              T6>::value &&std::is_nothrow_move_constructible<T7>::value &&std17::
                              is_nothrow_swappable<T7>::value &&std::is_nothrow_move_constructible<
                                  T8>::value &&std17::is_nothrow_swappable<T8>::value
                                  &&std::is_nothrow_move_constructible<
                                      T9>::value &&std17::is_nothrow_swappable<T9>::value
                                      &&std::is_nothrow_move_constructible<
                                          T10>::value &&std17::is_nothrow_swappable<T10>::value
                                          &&std::is_nothrow_move_constructible<
                                              T11>::value &&std17::is_nothrow_swappable<T11>::value
                                              &&std::is_nothrow_move_constructible<T12>::value
                                                  &&std17::is_nothrow_swappable<T12>::value &&
                                                      std::is_nothrow_move_constructible<T13>::value
                                                          &&std17::is_nothrow_swappable<T13>::value
                                                              &&std::is_nothrow_move_constructible<
                                                                  T14>::value
                                                                  &&std17::is_nothrow_swappable<
                                                                      T14>::value &&std::
                                                                      is_nothrow_move_constructible<
                                                                          T15>::value &&std17::
                                                                          is_nothrow_swappable<
                                                                              T15>::value

      )
#endif
  {
    if (valueless_by_exception() && other.valueless_by_exception()) {
      // no effect
    } else if (type_index == other.type_index) {
      this->swap_value(type_index, other);
    } else {
#if variant_CPP11_OR_GREATER
      variant tmp(std::move(*this));
      *this = std::move(other);
      other = std::move(tmp);
#else
      variant tmp(*this);
      *this = other;
      other = tmp;
#endif
    }
  }

  //
  // non-standard:
  //

  template <class T> static variant_constexpr std::size_t index_of() variant_noexcept {
    return to_size_t(
        detail::typelist_index_of<variant_types, typename std11::remove_cv<T>::type>::value);
  }

  template <class T> T &get() {
#if variant_CONFIG_NO_EXCEPTIONS
    assert(index_of<T>() == index());
#else
    if (index_of<T>() != index()) {
      throw bad_variant_access();
    }
#endif
    return *as<T>();
  }

  template <class T> T const &get() const {
#if variant_CONFIG_NO_EXCEPTIONS
    assert(index_of<T>() == index());
#else
    if (index_of<T>() != index()) {
      throw bad_variant_access();
    }
#endif
    return *as<const T>();
  }

  template <std::size_t K> typename variant_alternative<K, variant>::type &get() {
    return this->template get<typename detail::typelist_type_at<variant_types, K>::type>();
  }

  template <std::size_t K> typename variant_alternative<K, variant>::type const &get() const {
    return this->template get<typename detail::typelist_type_at<variant_types, K>::type>();
  }

private:
  typedef typename helper_type::type_index_t type_index_t;

  void *ptr() variant_noexcept { return &data; }

  void const *ptr() const variant_noexcept { return &data; }

  template <class U> U *as() { return reinterpret_cast<U *>(ptr()); }

  template <class U> U const *as() const { return reinterpret_cast<U const *>(ptr()); }

  template <class U> static variant_constexpr std::size_t to_size_t(U index) {
    return static_cast<std::size_t>(index);
  }

  variant_constexpr type_index_t variant_npos_internal() const variant_noexcept {
    return static_cast<type_index_t>(-1);
  }

  variant &copy_assign(variant const &other) {
    if (valueless_by_exception() && other.valueless_by_exception()) {
      // no effect
    } else if (!valueless_by_exception() && other.valueless_by_exception()) {
      helper_type::destroy(type_index, ptr());
      type_index = variant_npos_internal();
    } else if (index() == other.index()) {
      type_index = helper_type::copy_assign(other.type_index, other.ptr(), ptr());
    } else {
      helper_type::destroy(type_index, ptr());
      type_index = variant_npos_internal();
      type_index = helper_type::copy_construct(other.type_index, other.ptr(), ptr());
    }
    return *this;
  }

#if variant_CPP11_OR_GREATER

  variant &move_assign(variant &&other) {
    if (valueless_by_exception() && other.valueless_by_exception()) {
      // no effect
    } else if (!valueless_by_exception() && other.valueless_by_exception()) {
      helper_type::destroy(type_index, ptr());
      type_index = variant_npos_internal();
    } else if (index() == other.index()) {
      type_index = helper_type::move_assign(other.type_index, other.ptr(), ptr());
    } else {
      helper_type::destroy(type_index, ptr());
      type_index = variant_npos_internal();
      type_index = helper_type::move_construct(other.type_index, other.ptr(), ptr());
    }
    return *this;
  }

  template <std::size_t K, class T> variant &assign_value(T &&value) {
    if (index() == K) {
      *as<T>() = std::forward<T>(value);
    } else {
      helper_type::destroy(type_index, ptr());
      type_index = variant_npos_internal();
      new (ptr()) T(std::forward<T>(value));
      type_index = K;
    }
    return *this;
  }

#endif // variant_CPP11_OR_GREATER

  template <std::size_t K, class T> variant &assign_value(T const &value) {
    if (index() == K) {
      *as<T>() = value;
    } else {
      helper_type::destroy(type_index, ptr());
      type_index = variant_npos_internal();
      new (ptr()) T(value);
      type_index = K;
    }
    return *this;
  }

  void swap_value(type_index_t index, variant &other) {
    using std::swap;
    switch (index) {
    case 0:
      swap(this->get<0>(), other.get<0>());
      break;
    case 1:
      swap(this->get<1>(), other.get<1>());
      break;
    case 2:
      swap(this->get<2>(), other.get<2>());
      break;
    case 3:
      swap(this->get<3>(), other.get<3>());
      break;
    case 4:
      swap(this->get<4>(), other.get<4>());
      break;
    case 5:
      swap(this->get<5>(), other.get<5>());
      break;
    case 6:
      swap(this->get<6>(), other.get<6>());
      break;
    case 7:
      swap(this->get<7>(), other.get<7>());
      break;
    case 8:
      swap(this->get<8>(), other.get<8>());
      break;
    case 9:
      swap(this->get<9>(), other.get<9>());
      break;
    case 10:
      swap(this->get<10>(), other.get<10>());
      break;
    case 11:
      swap(this->get<11>(), other.get<11>());
      break;
    case 12:
      swap(this->get<12>(), other.get<12>());
      break;
    case 13:
      swap(this->get<13>(), other.get<13>());
      break;
    case 14:
      swap(this->get<14>(), other.get<14>());
      break;
    case 15:
      swap(this->get<15>(), other.get<15>());
      break;
    }
  }

private:
  enum { data_size = detail::typelist_max<variant_types>::value };

#if variant_CPP11_OR_GREATER

  enum { data_align = detail::typelist_max_alignof<variant_types>::value };

  using aligned_storage_t = typename std::aligned_storage<data_size, data_align>::type;
  aligned_storage_t data;

#elif variant_CONFIG_MAX_ALIGN_HACK

  typedef union {
    unsigned char data[data_size];
  } aligned_storage_t;

  detail::max_align_t hack;
  aligned_storage_t data;

#else
  typedef typename detail::typelist_max<variant_types>::type max_type;

  typedef variant_ALIGN_AS(max_type) align_as_type;

  typedef union {
    align_as_type data[1 + (data_size - 1) / sizeof(align_as_type)];
  } aligned_storage_t;
  aligned_storage_t data;

  // #   undef variant_ALIGN_AS

#endif // variant_CONFIG_MAX_ALIGN_HACK

  type_index_t type_index;
};

// 19.7.5 Value access

template <class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool holds_alternative(
    variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v)
    variant_noexcept {
  return v.index() == variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                              T15>::template index_of<T>();
}

template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline R &get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &v,
              nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
  return v.template get<R>();
}

template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline R const &
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
    nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
  return v.template get<R>();
}

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
inline typename variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::type &
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &v,
    nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
  assert(K == v.index());
#else
  if (K != v.index()) {
    throw bad_variant_access();
  }
#endif
  return v.template get<K>();
}

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
inline typename variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::type const &
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
    nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
  assert(K == v.index());
#else
  if (K != v.index()) {
    throw bad_variant_access();
  }
#endif
  return v.template get<K>();
}

#if variant_CPP11_OR_GREATER

template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline R &&get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &&v,
               nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
  return std::move(v.template get<R>());
}

template <class R, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline R const &&
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &&v,
    nonstd_lite_in_place_type_t(R) = nonstd_lite_in_place_type(R)) {
  return std::move(v.template get<R>());
}

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
inline typename variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::type &&
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &&v,
    nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
  assert(K == v.index());
#else
  if (K != v.index()) {
    throw bad_variant_access();
  }
#endif
  return std::move(v.template get<K>());
}

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
inline typename variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::type const &&
get(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &&v,
    nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
#if variant_CONFIG_NO_EXCEPTIONS
  assert(K == v.index());
#else
  if (K != v.index()) {
    throw bad_variant_access();
  }
#endif
  return std::move(v.template get<K>());
}

#endif // variant_CPP11_OR_GREATER

template <class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline typename std11::add_pointer<T>::type
get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *pv,
       nonstd_lite_in_place_type_t(T) = nonstd_lite_in_place_type(T)) {
  return (pv->index() == variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                                 T15>::template index_of<T>())
             ? &get<T>(*pv)
             : variant_nullptr;
}

template <class T, class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7,
          class T8, class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline typename std11::add_pointer<const T>::type
get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const *pv,
       nonstd_lite_in_place_type_t(T) = nonstd_lite_in_place_type(T)) {
  return (pv->index() == variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14,
                                 T15>::template index_of<T>())
             ? &get<T>(*pv)
             : variant_nullptr;
}

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
inline typename std11::add_pointer<typename variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::type>::type
get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> *pv,
       nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
  return (pv->index() == K) ? &get<K>(*pv) : variant_nullptr;
}

template <std::size_t K, class T0, class T1, class T2, class T3, class T4, class T5, class T6,
          class T7, class T8, class T9, class T10, class T11, class T12, class T13, class T14,
          class T15>
inline typename std11::add_pointer<const typename variant_alternative<
    K, variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::type>::type
get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const *pv,
       nonstd_lite_in_place_index_t(K) = nonstd_lite_in_place_index(K)) {
  return (pv->index() == K) ? &get<K>(*pv) : variant_nullptr;
}

// 19.7.10 Specialized algorithms

template <
    class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
    class T9, class T10, class T11, class T12, class T13, class T14,
    class T15
#if variant_CPP11_OR_GREATER
        variant_REQUIRES_T(
            std::is_move_constructible<T0>::value &&std17::is_swappable<
                T0>::value &&std::is_move_constructible<T1>::value &&std17::is_swappable<T1>::value
                &&std::is_move_constructible<T2>::value &&std17::is_swappable<
                    T2>::value &&std::is_move_constructible<T3>::value &&std17::is_swappable<T3>::
                    value &&std::is_move_constructible<T4>::value &&std17::is_swappable<T4>::value
                        &&std::is_move_constructible<T5>::value &&std17::is_swappable<
                            T5>::value &&std::is_move_constructible<T6>::value
                            &&std17::is_swappable<T6>::value &&std::is_move_constructible<
                                T7>::value &&std17::is_swappable<T7>::value
                                &&std::is_move_constructible<T8>::value &&std17::is_swappable<
                                    T8>::value &&std::is_move_constructible<T9>::value
                                    &&std17::is_swappable<T9>::value &&std::is_move_constructible<
                                        T10>::value &&std17::is_swappable<T10>::value &&std::
                                        is_move_constructible<T11>::value &&std17::is_swappable<
                                            T11>::value &&std::is_move_constructible<T12>::value
                                            &&std17::is_swappable<
                                                T12>::value &&std::is_move_constructible<T13>::value
                                                &&std17::is_swappable<T13>::value
                                                    &&std::is_move_constructible<T14>::value
                                                        &&std17::is_swappable<T14>::value
                                                            &&std::is_move_constructible<T15>::value
                                                                &&std17::is_swappable<T15>::value)
#endif
    >
inline void swap(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &a,
                 variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> &b)
#if variant_CPP11_OR_GREATER
    noexcept(noexcept(a.swap(b)))
#endif
{
  a.swap(b);
}

// 19.7.7 Visitation

// Variant 'visitor' implementation

namespace detail {

template <typename R, typename VT> struct VisitorApplicatorImpl {
  template <typename Visitor, typename T> static R apply(Visitor const &v, T const &arg) {
    return v(arg);
  }
};

template <typename R, typename VT> struct VisitorApplicatorImpl<R, TX<VT>> {
  template <typename Visitor, typename T> static R apply(Visitor const &, T) {
    // prevent default construction of a const reference, see issue #39:
    std::terminate();
  }
};

template <typename R> struct VisitorApplicator;

template <typename R, typename Visitor, typename V1> struct VisitorUnwrapper;

#if variant_CPP11_OR_GREATER
template <size_t NumVars, typename R, typename Visitor, typename... T>
#else
template <size_t NumVars, typename R, typename Visitor, typename T1, typename T2 = S0,
          typename T3 = S0, typename T4 = S0, typename T5 = S0>
#endif
struct TypedVisitorUnwrapper;

template <typename R, typename Visitor, typename T2>
struct TypedVisitorUnwrapper<2, R, Visitor, T2> {
  const Visitor &visitor;
  T2 const &val2;

  TypedVisitorUnwrapper(const Visitor &visitor_, T2 const &val2_)
      : visitor(visitor_), val2(val2_)

  {}

  template <typename T> R operator()(const T &val1) const { return visitor(val1, val2); }
};

template <typename R, typename Visitor, typename T2, typename T3>
struct TypedVisitorUnwrapper<3, R, Visitor, T2, T3> {
  const Visitor &visitor;
  T2 const &val2;
  T3 const &val3;

  TypedVisitorUnwrapper(const Visitor &visitor_, T2 const &val2_, T3 const &val3_)
      : visitor(visitor_), val2(val2_), val3(val3_)

  {}

  template <typename T> R operator()(const T &val1) const { return visitor(val1, val2, val3); }
};

template <typename R, typename Visitor, typename T2, typename T3, typename T4>
struct TypedVisitorUnwrapper<4, R, Visitor, T2, T3, T4> {
  const Visitor &visitor;
  T2 const &val2;
  T3 const &val3;
  T4 const &val4;

  TypedVisitorUnwrapper(const Visitor &visitor_, T2 const &val2_, T3 const &val3_, T4 const &val4_)
      : visitor(visitor_), val2(val2_), val3(val3_), val4(val4_)

  {}

  template <typename T> R operator()(const T &val1) const {
    return visitor(val1, val2, val3, val4);
  }
};

template <typename R, typename Visitor, typename T2, typename T3, typename T4, typename T5>
struct TypedVisitorUnwrapper<5, R, Visitor, T2, T3, T4, T5> {
  const Visitor &visitor;
  T2 const &val2;
  T3 const &val3;
  T4 const &val4;
  T5 const &val5;

  TypedVisitorUnwrapper(const Visitor &visitor_, T2 const &val2_, T3 const &val3_, T4 const &val4_,
                        T5 const &val5_)
      : visitor(visitor_), val2(val2_), val3(val3_), val4(val4_), val5(val5_)

  {}

  template <typename T> R operator()(const T &val1) const {
    return visitor(val1, val2, val3, val4, val5);
  }
};

template <typename R, typename Visitor, typename V2> struct VisitorUnwrapper {
  const Visitor &visitor;
  const V2 &r;

  VisitorUnwrapper(const Visitor &visitor_, const V2 &r_) : visitor(visitor_), r(r_) {}

  template <typename T1> R operator()(T1 const &val1) const {
    typedef TypedVisitorUnwrapper<2, R, Visitor, T1> visitor_type;
    return VisitorApplicator<R>::apply(visitor_type(visitor, val1), r);
  }

  template <typename T1, typename T2> R operator()(T1 const &val1, T2 const &val2) const {
    typedef TypedVisitorUnwrapper<3, R, Visitor, T1, T2> visitor_type;
    return VisitorApplicator<R>::apply(visitor_type(visitor, val1, val2), r);
  }

  template <typename T1, typename T2, typename T3>
  R operator()(T1 const &val1, T2 const &val2, T3 const &val3) const {
    typedef TypedVisitorUnwrapper<4, R, Visitor, T1, T2, T3> visitor_type;
    return VisitorApplicator<R>::apply(visitor_type(visitor, val1, val2, val3), r);
  }

  template <typename T1, typename T2, typename T3, typename T4>
  R operator()(T1 const &val1, T2 const &val2, T3 const &val3, T4 const &val4) const {
    typedef TypedVisitorUnwrapper<5, R, Visitor, T1, T2, T3, T4> visitor_type;
    return VisitorApplicator<R>::apply(visitor_type(visitor, val1, val2, val3, val4), r);
  }

  template <typename T1, typename T2, typename T3, typename T4, typename T5>
  R operator()(T1 const &val1, T2 const &val2, T3 const &val3, T4 const &val4,
               T5 const &val5) const {
    typedef TypedVisitorUnwrapper<6, R, Visitor, T1, T2, T3, T4, T5> visitor_type;
    return VisitorApplicator<R>::apply(visitor_type(visitor, val1, val2, val3, val4, val5), r);
  }
};

template <typename R> struct VisitorApplicator {
  template <typename Visitor, typename V1> static R apply(const Visitor &v, const V1 &arg) {
    switch (arg.index()) {
    case 0:
      return apply_visitor<0>(v, arg);
    case 1:
      return apply_visitor<1>(v, arg);
    case 2:
      return apply_visitor<2>(v, arg);
    case 3:
      return apply_visitor<3>(v, arg);
    case 4:
      return apply_visitor<4>(v, arg);
    case 5:
      return apply_visitor<5>(v, arg);
    case 6:
      return apply_visitor<6>(v, arg);
    case 7:
      return apply_visitor<7>(v, arg);
    case 8:
      return apply_visitor<8>(v, arg);
    case 9:
      return apply_visitor<9>(v, arg);
    case 10:
      return apply_visitor<10>(v, arg);
    case 11:
      return apply_visitor<11>(v, arg);
    case 12:
      return apply_visitor<12>(v, arg);
    case 13:
      return apply_visitor<13>(v, arg);
    case 14:
      return apply_visitor<14>(v, arg);
    case 15:
      return apply_visitor<15>(v, arg);

    // prevent default construction of a const reference, see issue #39:
    default:
      std::terminate();
    }
  }

  template <size_t Idx, typename Visitor, typename V1>
  static R apply_visitor(const Visitor &v, const V1 &arg) {

#if variant_CPP11_OR_GREATER
    typedef typename variant_alternative<Idx, typename std::decay<V1>::type>::type value_type;
#else
    typedef typename variant_alternative<Idx, V1>::type value_type;
#endif
    return VisitorApplicatorImpl<R, value_type>::apply(v, get<Idx>(arg));
  }

#if variant_CPP11_OR_GREATER
  template <typename Visitor, typename V1, typename V2, typename... V>
  static R apply(const Visitor &v, const V1 &arg1, const V2 &arg2, const V... args) {
    typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
    Unwrapper unwrapper(v, arg1);
    return apply(unwrapper, arg2, args...);
  }
#else

  template <typename Visitor, typename V1, typename V2>
  static R apply(const Visitor &v, V1 const &arg1, V2 const &arg2) {
    typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
    Unwrapper unwrapper(v, arg1);
    return apply(unwrapper, arg2);
  }

  template <typename Visitor, typename V1, typename V2, typename V3>
  static R apply(const Visitor &v, V1 const &arg1, V2 const &arg2, V3 const &arg3) {
    typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
    Unwrapper unwrapper(v, arg1);
    return apply(unwrapper, arg2, arg3);
  }

  template <typename Visitor, typename V1, typename V2, typename V3, typename V4>
  static R apply(const Visitor &v, V1 const &arg1, V2 const &arg2, V3 const &arg3, V4 const &arg4) {
    typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
    Unwrapper unwrapper(v, arg1);
    return apply(unwrapper, arg2, arg3, arg4);
  }

  template <typename Visitor, typename V1, typename V2, typename V3, typename V4, typename V5>
  static R apply(const Visitor &v, V1 const &arg1, V2 const &arg2, V3 const &arg3, V4 const &arg4,
                 V5 const &arg5) {
    typedef VisitorUnwrapper<R, Visitor, V1> Unwrapper;
    Unwrapper unwrapper(v, arg1);
    return apply(unwrapper, arg2, arg3, arg4, arg5);
  }

#endif
};

#if variant_CPP11_OR_GREATER
template <size_t NumVars, typename Visitor, typename... V> struct VisitorImpl {
  typedef decltype(
      std::declval<Visitor>()(get<0>(static_cast<const V &>(std::declval<V>()))...)) result_type;
  typedef VisitorApplicator<result_type> applicator_type;
};
#endif
} // namespace detail

#if variant_CPP11_OR_GREATER
// No perfect forwarding here in order to simplify code
template <typename Visitor, typename... V>
inline auto visit(Visitor const &v, V const &... vars) ->
    typename detail::VisitorImpl<sizeof...(V), Visitor, V...>::result_type {
  typedef detail::VisitorImpl<sizeof...(V), Visitor, V...> impl_type;
  return impl_type::applicator_type::apply(v, vars...);
}
#else

template <typename R, typename Visitor, typename V1>
inline R visit(const Visitor &v, V1 const &arg1) {
  return detail::VisitorApplicator<R>::apply(v, arg1);
}

template <typename R, typename Visitor, typename V1, typename V2>
inline R visit(const Visitor &v, V1 const &arg1, V2 const &arg2) {
  return detail::VisitorApplicator<R>::apply(v, arg1, arg2);
}

template <typename R, typename Visitor, typename V1, typename V2, typename V3>
inline R visit(const Visitor &v, V1 const &arg1, V2 const &arg2, V3 const &arg3) {
  return detail::VisitorApplicator<R>::apply(v, arg1, arg2, arg3);
}

template <typename R, typename Visitor, typename V1, typename V2, typename V3, typename V4>
inline R visit(const Visitor &v, V1 const &arg1, V2 const &arg2, V3 const &arg3, V4 const &arg4) {
  return detail::VisitorApplicator<R>::apply(v, arg1, arg2, arg3, arg4);
}

template <typename R, typename Visitor, typename V1, typename V2, typename V3, typename V4,
          typename V5>
inline R visit(const Visitor &v, V1 const &arg1, V2 const &arg2, V3 const &arg3, V4 const &arg4,
               V5 const &arg5) {
  return detail::VisitorApplicator<R>::apply(v, arg1, arg2, arg3, arg4, arg5);
}

#endif

// 19.7.6 Relational operators

namespace detail {

template <class Variant> struct Comparator {
  static inline bool equal(Variant const &v, Variant const &w) {
    switch (v.index()) {
    case 0:
      return get<0>(v) == get<0>(w);
    case 1:
      return get<1>(v) == get<1>(w);
    case 2:
      return get<2>(v) == get<2>(w);
    case 3:
      return get<3>(v) == get<3>(w);
    case 4:
      return get<4>(v) == get<4>(w);
    case 5:
      return get<5>(v) == get<5>(w);
    case 6:
      return get<6>(v) == get<6>(w);
    case 7:
      return get<7>(v) == get<7>(w);
    case 8:
      return get<8>(v) == get<8>(w);
    case 9:
      return get<9>(v) == get<9>(w);
    case 10:
      return get<10>(v) == get<10>(w);
    case 11:
      return get<11>(v) == get<11>(w);
    case 12:
      return get<12>(v) == get<12>(w);
    case 13:
      return get<13>(v) == get<13>(w);
    case 14:
      return get<14>(v) == get<14>(w);
    case 15:
      return get<15>(v) == get<15>(w);

    default:
      return false;
    }
  }

  static inline bool less_than(Variant const &v, Variant const &w) {
    switch (v.index()) {
    case 0:
      return get<0>(v) < get<0>(w);
    case 1:
      return get<1>(v) < get<1>(w);
    case 2:
      return get<2>(v) < get<2>(w);
    case 3:
      return get<3>(v) < get<3>(w);
    case 4:
      return get<4>(v) < get<4>(w);
    case 5:
      return get<5>(v) < get<5>(w);
    case 6:
      return get<6>(v) < get<6>(w);
    case 7:
      return get<7>(v) < get<7>(w);
    case 8:
      return get<8>(v) < get<8>(w);
    case 9:
      return get<9>(v) < get<9>(w);
    case 10:
      return get<10>(v) < get<10>(w);
    case 11:
      return get<11>(v) < get<11>(w);
    case 12:
      return get<12>(v) < get<12>(w);
    case 13:
      return get<13>(v) < get<13>(w);
    case 14:
      return get<14>(v) < get<14>(w);
    case 15:
      return get<15>(v) < get<15>(w);

    default:
      return false;
    }
  }
};

} // namespace detail

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool
operator==(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
           variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &w) {
  if (v.index() != w.index())
    return false;
  else if (v.valueless_by_exception())
    return true;
  else
    return detail::Comparator<
        variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>>::equal(v, w);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool
operator!=(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
           variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &w) {
  return !(v == w);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool
operator<(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
          variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &w) {
  if (w.valueless_by_exception())
    return false;
  else if (v.valueless_by_exception())
    return true;
  else if (v.index() < w.index())
    return true;
  else if (v.index() > w.index())
    return false;
  else
    return detail::Comparator<variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
                                      T14, T15>>::less_than(v, w);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool
operator>(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
          variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &w) {
  return w < v;
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool
operator<=(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
           variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &w) {
  return !(v > w);
}

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
inline bool
operator>=(variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &v,
           variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15> const &w) {
  return !(v < w);
}

} // namespace variants

using namespace variants;

} // namespace nonstd

#if variant_CPP11_OR_GREATER

// 19.7.12 Hash support

namespace std {

template <> struct hash<nonstd::monostate> {
  std::size_t operator()(nonstd::monostate) const variant_noexcept { return 42; }
};

template <class T0, class T1, class T2, class T3, class T4, class T5, class T6, class T7, class T8,
          class T9, class T10, class T11, class T12, class T13, class T14, class T15>
struct hash<nonstd::variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13, T14, T15>> {
  std::size_t operator()(nonstd::variant<T0, T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13,
                                         T14, T15> const &v) const variant_noexcept {
    namespace nvd = nonstd::variants::detail;

    switch (v.index()) {
    case 0:
      return nvd::hash(0) ^ nvd::hash(get<0>(v));
    case 1:
      return nvd::hash(1) ^ nvd::hash(get<1>(v));
    case 2:
      return nvd::hash(2) ^ nvd::hash(get<2>(v));
    case 3:
      return nvd::hash(3) ^ nvd::hash(get<3>(v));
    case 4:
      return nvd::hash(4) ^ nvd::hash(get<4>(v));
    case 5:
      return nvd::hash(5) ^ nvd::hash(get<5>(v));
    case 6:
      return nvd::hash(6) ^ nvd::hash(get<6>(v));
    case 7:
      return nvd::hash(7) ^ nvd::hash(get<7>(v));
    case 8:
      return nvd::hash(8) ^ nvd::hash(get<8>(v));
    case 9:
      return nvd::hash(9) ^ nvd::hash(get<9>(v));
    case 10:
      return nvd::hash(10) ^ nvd::hash(get<10>(v));
    case 11:
      return nvd::hash(11) ^ nvd::hash(get<11>(v));
    case 12:
      return nvd::hash(12) ^ nvd::hash(get<12>(v));
    case 13:
      return nvd::hash(13) ^ nvd::hash(get<13>(v));
    case 14:
      return nvd::hash(14) ^ nvd::hash(get<14>(v));
    case 15:
      return nvd::hash(15) ^ nvd::hash(get<15>(v));

    default:
      return 0;
    }
  }
};

} // namespace std

#endif // variant_CPP11_OR_GREATER

#if variant_BETWEEN(variant_COMPILER_MSVC_VER, 1300, 1900)
#pragma warning(pop)
#endif

#endif // variant_USES_STD_VARIANT

#endif // NONSTD_VARIANT_LITE_HPP
//
// Copyright (c) 2014-2018 Martin Moene
//
// https://github.com/martinmoene/optional-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_OPTIONAL_LITE_HPP
#define NONSTD_OPTIONAL_LITE_HPP

#define optional_lite_MAJOR 3
#define optional_lite_MINOR 2
#define optional_lite_PATCH 0

#define optional_lite_VERSION                                                                      \
  optional_STRINGIFY(optional_lite_MAJOR) "." optional_STRINGIFY(                                  \
      optional_lite_MINOR) "." optional_STRINGIFY(optional_lite_PATCH)

#define optional_STRINGIFY(x) optional_STRINGIFY_(x)
#define optional_STRINGIFY_(x) #x

// optional-lite configuration:

#define optional_OPTIONAL_DEFAULT 0
#define optional_OPTIONAL_NONSTD 1
#define optional_OPTIONAL_STD 2

#if !defined(optional_CONFIG_SELECT_OPTIONAL)
#define optional_CONFIG_SELECT_OPTIONAL                                                            \
  (optional_HAVE_STD_OPTIONAL ? optional_OPTIONAL_STD : optional_OPTIONAL_NONSTD)
#endif

// Control presence of exception handling (try and auto discover):

#ifndef optional_CONFIG_NO_EXCEPTIONS
#if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || defined(_CPPUNWIND)
#define optional_CONFIG_NO_EXCEPTIONS 0
#else
#define optional_CONFIG_NO_EXCEPTIONS 1
#endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef optional_CPLUSPLUS
#if defined(_MSVC_LANG) && !defined(__clang__)
#define optional_CPLUSPLUS (_MSC_VER == 1900 ? 201103L : _MSVC_LANG)
#else
#define optional_CPLUSPLUS __cplusplus
#endif
#endif

#define optional_CPP98_OR_GREATER (optional_CPLUSPLUS >= 199711L)
#define optional_CPP11_OR_GREATER (optional_CPLUSPLUS >= 201103L)
#define optional_CPP11_OR_GREATER_ (optional_CPLUSPLUS >= 201103L)
#define optional_CPP14_OR_GREATER (optional_CPLUSPLUS >= 201402L)
#define optional_CPP17_OR_GREATER (optional_CPLUSPLUS >= 201703L)
#define optional_CPP20_OR_GREATER (optional_CPLUSPLUS >= 202000L)

// C++ language version (represent 98 as 3):

#define optional_CPLUSPLUS_V                                                                       \
  (optional_CPLUSPLUS / 100 - (optional_CPLUSPLUS > 200000 ? 2000 : 1994))

// Use C++17 std::optional if available and requested:

#if optional_CPP17_OR_GREATER && defined(__has_include)
#if __has_include(<optional> )
#define optional_HAVE_STD_OPTIONAL 1
#else
#define optional_HAVE_STD_OPTIONAL 0
#endif
#else
#define optional_HAVE_STD_OPTIONAL 0
#endif

#define optional_USES_STD_OPTIONAL                                                                 \
  ((optional_CONFIG_SELECT_OPTIONAL == optional_OPTIONAL_STD) ||                                   \
   ((optional_CONFIG_SELECT_OPTIONAL == optional_OPTIONAL_DEFAULT) && optional_HAVE_STD_OPTIONAL))

//
// in_place: code duplicated in any-lite, expected-lite, optional-lite, value-ptr-lite,
// variant-lite:
//

#ifndef nonstd_lite_HAVE_IN_PLACE_TYPES
#define nonstd_lite_HAVE_IN_PLACE_TYPES 1

// C++17 std::in_place in <utility>:

#if optional_CPP17_OR_GREATER

#include <utility>

namespace nonstd {

using std::in_place;
using std::in_place_index;
using std::in_place_index_t;
using std::in_place_t;
using std::in_place_type;
using std::in_place_type_t;

#define nonstd_lite_in_place_t(T) std::in_place_t
#define nonstd_lite_in_place_type_t(T) std::in_place_type_t<T>
#define nonstd_lite_in_place_index_t(K) std::in_place_index_t<K>

#define nonstd_lite_in_place(T)                                                                    \
  std::in_place_t {}
#define nonstd_lite_in_place_type(T)                                                               \
  std::in_place_type_t<T> {}
#define nonstd_lite_in_place_index(K)                                                              \
  std::in_place_index_t<K> {}

} // namespace nonstd

#else // optional_CPP17_OR_GREATER

#include <cstddef>

namespace nonstd {
namespace detail {

template <class T> struct in_place_type_tag {};

template <std::size_t K> struct in_place_index_tag {};

} // namespace detail

struct in_place_t {};

template <class T>
inline in_place_t
in_place(detail::in_place_type_tag<T> /*unused*/ = detail::in_place_type_tag<T>()) {
  return in_place_t();
}

template <std::size_t K>
inline in_place_t
in_place(detail::in_place_index_tag<K> /*unused*/ = detail::in_place_index_tag<K>()) {
  return in_place_t();
}

template <class T>
inline in_place_t
in_place_type(detail::in_place_type_tag<T> /*unused*/ = detail::in_place_type_tag<T>()) {
  return in_place_t();
}

template <std::size_t K>
inline in_place_t
in_place_index(detail::in_place_index_tag<K> /*unused*/ = detail::in_place_index_tag<K>()) {
  return in_place_t();
}

// mimic templated typedef:

#define nonstd_lite_in_place_t(T) nonstd::in_place_t (&)(nonstd::detail::in_place_type_tag<T>)
#define nonstd_lite_in_place_type_t(T) nonstd::in_place_t (&)(nonstd::detail::in_place_type_tag<T>)
#define nonstd_lite_in_place_index_t(K)                                                            \
  nonstd::in_place_t (&)(nonstd::detail::in_place_index_tag<K>)

#define nonstd_lite_in_place(T) nonstd::in_place_type<T>
#define nonstd_lite_in_place_type(T) nonstd::in_place_type<T>
#define nonstd_lite_in_place_index(K) nonstd::in_place_index<K>

} // namespace nonstd

#endif // optional_CPP17_OR_GREATER
#endif // nonstd_lite_HAVE_IN_PLACE_TYPES

//
// Using std::optional:
//

#if optional_USES_STD_OPTIONAL

#include <optional>

namespace nonstd {

using std::bad_optional_access;
using std::hash;
using std::optional;

using std::nullopt;
using std::nullopt_t;

using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;
using std::make_optional;
using std::swap;
} // namespace nonstd

#else // optional_USES_STD_OPTIONAL

#include <cassert>
#include <utility>

// optional-lite alignment configuration:

#ifndef optional_CONFIG_MAX_ALIGN_HACK
#define optional_CONFIG_MAX_ALIGN_HACK 0
#endif

#ifndef optional_CONFIG_ALIGN_AS
// no default, used in #if defined()
#endif

#ifndef optional_CONFIG_ALIGN_AS_FALLBACK
#define optional_CONFIG_ALIGN_AS_FALLBACK double
#endif

// Compiler warning suppression:

#if defined(__clang__)
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wundef"
#elif defined(__GNUC__)
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wundef"
#elif defined(_MSC_VER)
#pragma warning(push)
#endif

// half-open range [lo..hi):
#define optional_BETWEEN(v, lo, hi) ((lo) <= (v) && (v) < (hi))

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  optional_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  optional_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  optional_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  optional_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  optional_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  optional_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  optional_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  optional_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  optional_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  optional_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  optional_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if defined(_MSC_VER) && !defined(__clang__)
#define optional_COMPILER_MSVC_VER (_MSC_VER)
#define optional_COMPILER_MSVC_VERSION (_MSC_VER / 10 - 10 * (5 + (_MSC_VER < 1900)))
#else
#define optional_COMPILER_MSVC_VER 0
#define optional_COMPILER_MSVC_VERSION 0
#endif

#define optional_COMPILER_VERSION(major, minor, patch) (10 * (10 * (major) + (minor)) + (patch))

#if defined(__GNUC__) && !defined(__clang__)
#define optional_COMPILER_GNUC_VERSION                                                             \
  optional_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
#define optional_COMPILER_GNUC_VERSION 0
#endif

#if defined(__clang__)
#define optional_COMPILER_CLANG_VERSION                                                            \
  optional_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
#define optional_COMPILER_CLANG_VERSION 0
#endif

#if optional_BETWEEN(optional_COMPILER_MSVC_VERSION, 70, 140)
#pragma warning(disable : 4345) // initialization behavior changed
#endif

#if optional_BETWEEN(optional_COMPILER_MSVC_VERSION, 70, 150)
#pragma warning(disable : 4814) // in C++14 'constexpr' will not imply 'const'
#endif

// Presence of language and library features:

#define optional_HAVE(FEATURE) (optional_HAVE_##FEATURE)

#ifdef _HAS_CPP0X
#define optional_HAS_CPP0X _HAS_CPP0X
#else
#define optional_HAS_CPP0X 0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for optional-lite:

#if optional_COMPILER_MSVC_VER >= 1900
#undef optional_CPP11_OR_GREATER
#define optional_CPP11_OR_GREATER 1
#endif

#define optional_CPP11_90 (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1500)
#define optional_CPP11_100 (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1600)
#define optional_CPP11_110 (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1700)
#define optional_CPP11_120 (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1800)
#define optional_CPP11_140 (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1900)
#define optional_CPP11_141 (optional_CPP11_OR_GREATER_ || optional_COMPILER_MSVC_VER >= 1910)

#define optional_CPP11_140_490                                                                     \
  ((optional_CPP11_OR_GREATER_ && optional_COMPILER_GNUC_VERSION >= 490) ||                        \
   (optional_COMPILER_MSVC_VER >= 1910))

#define optional_CPP14_000 (optional_CPP14_OR_GREATER)
#define optional_CPP17_000 (optional_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define optional_HAVE_CONSTEXPR_11 optional_CPP11_140
#define optional_HAVE_IS_DEFAULT optional_CPP11_140
#define optional_HAVE_NOEXCEPT optional_CPP11_140
#define optional_HAVE_NULLPTR optional_CPP11_100
#define optional_HAVE_REF_QUALIFIER optional_CPP11_140_490
#define optional_HAVE_INITIALIZER_LIST optional_CPP11_140

// Presence of C++14 language features:

#define optional_HAVE_CONSTEXPR_14 optional_CPP14_000

// Presence of C++17 language features:

#define optional_HAVE_NODISCARD optional_CPP17_000

// Presence of C++ library features:

#define optional_HAVE_CONDITIONAL optional_CPP11_120
#define optional_HAVE_REMOVE_CV optional_CPP11_120
#define optional_HAVE_TYPE_TRAITS optional_CPP11_90

#define optional_HAVE_TR1_TYPE_TRAITS (!!optional_COMPILER_GNUC_VERSION)
#define optional_HAVE_TR1_ADD_POINTER (!!optional_COMPILER_GNUC_VERSION)

// C++ feature usage:

#if optional_HAVE(CONSTEXPR_11)
#define optional_constexpr constexpr
#else
#define optional_constexpr /*constexpr*/
#endif

#if optional_HAVE(IS_DEFAULT)
#define optional_is_default = default;
#else
#define optional_is_default                                                                        \
  {}
#endif

#if optional_HAVE(CONSTEXPR_14)
#define optional_constexpr14 constexpr
#else
#define optional_constexpr14 /*constexpr*/
#endif

#if optional_HAVE(NODISCARD)
#define optional_nodiscard [[nodiscard]]
#else
#define optional_nodiscard /*[[nodiscard]]*/
#endif

#if optional_HAVE(NOEXCEPT)
#define optional_noexcept noexcept
#else
#define optional_noexcept /*noexcept*/
#endif

#if optional_HAVE(NULLPTR)
#define optional_nullptr nullptr
#else
#define optional_nullptr NULL
#endif

#if optional_HAVE(REF_QUALIFIER)
// NOLINTNEXTLINE( bugprone-macro-parentheses )
#define optional_ref_qual &
#define optional_refref_qual &&
#else
#define optional_ref_qual    /*&*/
#define optional_refref_qual /*&&*/
#endif

// additional includes:

#if optional_CONFIG_NO_EXCEPTIONS
// already included: <cassert>
#else
#include <stdexcept>
#endif

#if optional_CPP11_OR_GREATER
#include <functional>
#endif

#if optional_HAVE(INITIALIZER_LIST)
#include <initializer_list>
#endif

#if optional_HAVE(TYPE_TRAITS)
#include <type_traits>
#elif optional_HAVE(TR1_TYPE_TRAITS)
#include <tr1/type_traits>
#endif

// Method enabling

#if optional_CPP11_OR_GREATER

#define optional_REQUIRES_0(...)                                                                   \
  template <bool B = (__VA_ARGS__), typename std::enable_if<B, int>::type = 0>

#define optional_REQUIRES_T(...) , typename std::enable_if<(__VA_ARGS__), int>::type = 0

#define optional_REQUIRES_R(R, ...) typename std::enable_if<(__VA_ARGS__), R>::type

#define optional_REQUIRES_A(...) , typename std::enable_if<(__VA_ARGS__), void *>::type = nullptr

#endif

//
// optional:
//

namespace nonstd {
namespace optional_lite {

namespace std11 {

#if optional_CPP11_OR_GREATER
using std::move;
#else
template <typename T> T &move(T &t) { return t; }
#endif

#if optional_HAVE(CONDITIONAL)
using std::conditional;
#else
template <bool B, typename T, typename F> struct conditional { typedef T type; };
template <typename T, typename F> struct conditional<false, T, F> { typedef F type; };
#endif // optional_HAVE_CONDITIONAL

// gcc < 5:
#if optional_CPP11_OR_GREATER
#if optional_BETWEEN(optional_COMPILER_GNUC_VERSION, 1, 500)
template <typename T> struct is_trivially_copy_constructible : std::true_type {};
template <typename T> struct is_trivially_move_constructible : std::true_type {};
#else
using std::is_trivially_copy_constructible;
using std::is_trivially_move_constructible;
#endif
#endif
} // namespace std11

#if optional_CPP11_OR_GREATER

/// type traits C++17:

namespace std17 {

#if optional_CPP17_OR_GREATER

using std::is_nothrow_swappable;
using std::is_swappable;

#elif optional_CPP11_OR_GREATER

namespace detail {

using std::swap;

struct is_swappable {
  template <typename T, typename = decltype(swap(std::declval<T &>(), std::declval<T &>()))>
  static std::true_type test(int /*unused*/);

  template <typename> static std::false_type test(...);
};

struct is_nothrow_swappable {
  // wrap noexcept(expr) in separate function as work-around for VC140 (VS2015):

  template <typename T> static constexpr bool satisfies() {
    return noexcept(swap(std::declval<T &>(), std::declval<T &>()));
  }

  template <typename T>
  static auto test(int /*unused*/) -> std::integral_constant<bool, satisfies<T>()> {}

  template <typename> static auto test(...) -> std::false_type;
};

} // namespace detail

// is [nothow] swappable:

template <typename T> struct is_swappable : decltype(detail::is_swappable::test<T>(0)) {};

template <typename T>
struct is_nothrow_swappable : decltype(detail::is_nothrow_swappable::test<T>(0)) {};

#endif // optional_CPP17_OR_GREATER

} // namespace std17

/// type traits C++20:

namespace std20 {

template <typename T> struct remove_cvref {
  typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
};

} // namespace std20

#endif // optional_CPP11_OR_GREATER

/// class optional

template <typename T> class optional;

namespace detail {

// C++11 emulation:

struct nulltype {};

template <typename Head, typename Tail> struct typelist {
  typedef Head head;
  typedef Tail tail;
};

#if optional_CONFIG_MAX_ALIGN_HACK

// Max align, use most restricted type for alignment:

#define optional_UNIQUE(name) optional_UNIQUE2(name, __LINE__)
#define optional_UNIQUE2(name, line) optional_UNIQUE3(name, line)
#define optional_UNIQUE3(name, line) name##line

#define optional_ALIGN_TYPE(type)                                                                  \
  type optional_UNIQUE(_t);                                                                        \
  struct_t<type> optional_UNIQUE(_st)

template <typename T> struct struct_t { T _; };

union max_align_t {
  optional_ALIGN_TYPE(char);
  optional_ALIGN_TYPE(short int);
  optional_ALIGN_TYPE(int);
  optional_ALIGN_TYPE(long int);
  optional_ALIGN_TYPE(float);
  optional_ALIGN_TYPE(double);
  optional_ALIGN_TYPE(long double);
  optional_ALIGN_TYPE(char *);
  optional_ALIGN_TYPE(short int *);
  optional_ALIGN_TYPE(int *);
  optional_ALIGN_TYPE(long int *);
  optional_ALIGN_TYPE(float *);
  optional_ALIGN_TYPE(double *);
  optional_ALIGN_TYPE(long double *);
  optional_ALIGN_TYPE(void *);

#ifdef HAVE_LONG_LONG
  optional_ALIGN_TYPE(long long);
#endif

  struct Unknown;

  Unknown (*optional_UNIQUE(_))(Unknown);
  Unknown *Unknown::*optional_UNIQUE(_);
  Unknown (Unknown::*optional_UNIQUE(_))(Unknown);

  struct_t<Unknown (*)(Unknown)> optional_UNIQUE(_);
  struct_t<Unknown * Unknown::*> optional_UNIQUE(_);
  struct_t<Unknown (Unknown::*)(Unknown)> optional_UNIQUE(_);
};

#undef optional_UNIQUE
#undef optional_UNIQUE2
#undef optional_UNIQUE3

#undef optional_ALIGN_TYPE

#elif defined(optional_CONFIG_ALIGN_AS) // optional_CONFIG_MAX_ALIGN_HACK

// Use user-specified type for alignment:

#define optional_ALIGN_AS(unused) optional_CONFIG_ALIGN_AS

#else // optional_CONFIG_MAX_ALIGN_HACK

// Determine POD type to use for alignment:

#define optional_ALIGN_AS(to_align)                                                                \
  typename type_of_size<alignment_types, alignment_of<to_align>::value>::type

template <typename T> struct alignment_of;

template <typename T> struct alignment_of_hack {
  char c;
  T t;
  alignment_of_hack();
};

template <size_t A, size_t S> struct alignment_logic {
  enum { value = A < S ? A : S };
};

template <typename T> struct alignment_of {
  enum { value = alignment_logic<sizeof(alignment_of_hack<T>) - sizeof(T), sizeof(T)>::value };
};

template <typename List, size_t N> struct type_of_size {
  typedef
      typename std11::conditional<N == sizeof(typename List::head), typename List::head,
                                  typename type_of_size<typename List::tail, N>::type>::type type;
};

template <size_t N> struct type_of_size<nulltype, N> {
  typedef optional_CONFIG_ALIGN_AS_FALLBACK type;
};

template <typename T> struct struct_t { T _; };

#define optional_ALIGN_TYPE(type) typelist < type, typelist < struct_t<type>

struct Unknown;

typedef optional_ALIGN_TYPE(char), optional_ALIGN_TYPE(short), optional_ALIGN_TYPE(int),
    optional_ALIGN_TYPE(long), optional_ALIGN_TYPE(float), optional_ALIGN_TYPE(double),
    optional_ALIGN_TYPE(long double),

    optional_ALIGN_TYPE(char *), optional_ALIGN_TYPE(short *), optional_ALIGN_TYPE(int *),
    optional_ALIGN_TYPE(long *), optional_ALIGN_TYPE(float *), optional_ALIGN_TYPE(double *),
    optional_ALIGN_TYPE(long double *),

    optional_ALIGN_TYPE(Unknown (*)(Unknown)), optional_ALIGN_TYPE(Unknown *Unknown::*),
    optional_ALIGN_TYPE(Unknown (Unknown::*)(Unknown)),

    nulltype >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> alignment_types;

#undef optional_ALIGN_TYPE

#endif // optional_CONFIG_MAX_ALIGN_HACK

/// C++03 constructed union to hold value.

template <typename T> union storage_t {
  // private:
  //    template< typename > friend class optional;

  typedef T value_type;

  storage_t() optional_is_default

      explicit storage_t(value_type const &v) {
    construct_value(v);
  }

  void construct_value(value_type const &v) { ::new (value_ptr()) value_type(v); }

#if optional_CPP11_OR_GREATER

  explicit storage_t(value_type &&v) { construct_value(std::move(v)); }

  void construct_value(value_type &&v) { ::new (value_ptr()) value_type(std::move(v)); }

  template <class... Args> void emplace(Args &&... args) {
    ::new (value_ptr()) value_type(std::forward<Args>(args)...);
  }

  template <class U, class... Args> void emplace(std::initializer_list<U> il, Args &&... args) {
    ::new (value_ptr()) value_type(il, std::forward<Args>(args)...);
  }

#endif

  void destruct_value() { value_ptr()->~T(); }

  optional_nodiscard value_type const *value_ptr() const { return as<value_type>(); }

  value_type *value_ptr() { return as<value_type>(); }

  optional_nodiscard value_type const &value() const optional_ref_qual { return *value_ptr(); }

  value_type &value() optional_ref_qual { return *value_ptr(); }

#if optional_HAVE(REF_QUALIFIER)

  optional_nodiscard value_type const &&value() const optional_refref_qual {
    return std::move(value());
  }

  value_type &&value() optional_refref_qual { return std::move(value()); }

#endif

#if optional_CPP11_OR_GREATER

  using aligned_storage_t =
      typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;
  aligned_storage_t data;

#elif optional_CONFIG_MAX_ALIGN_HACK

  typedef struct {
    unsigned char data[sizeof(value_type)];
  } aligned_storage_t;

  max_align_t hack;
  aligned_storage_t data;

#else
  typedef optional_ALIGN_AS(value_type) align_as_type;

  typedef struct {
    align_as_type data[1 + (sizeof(value_type) - 1) / sizeof(align_as_type)];
  } aligned_storage_t;
  aligned_storage_t data;

#undef optional_ALIGN_AS

#endif // optional_CONFIG_MAX_ALIGN_HACK

  optional_nodiscard void *ptr() optional_noexcept { return &data; }

  optional_nodiscard void const *ptr() const optional_noexcept { return &data; }

  template <typename U> optional_nodiscard U *as() { return reinterpret_cast<U *>(ptr()); }

  template <typename U> optional_nodiscard U const *as() const {
    return reinterpret_cast<U const *>(ptr());
  }
};

} // namespace detail

/// disengaged state tag

struct nullopt_t {
  struct init {};
  explicit optional_constexpr nullopt_t(init /*unused*/) optional_noexcept {}
};

#if optional_HAVE(CONSTEXPR_11)
constexpr nullopt_t nullopt{nullopt_t::init{}};
#else
// extra parenthesis to prevent the most vexing parse:
const nullopt_t nullopt((nullopt_t::init()));
#endif

/// optional access error

#if !optional_CONFIG_NO_EXCEPTIONS

class bad_optional_access : public std::logic_error {
public:
  explicit bad_optional_access() : logic_error("bad optional access") {}
};

#endif // optional_CONFIG_NO_EXCEPTIONS

/// optional

template <typename T> class optional {
private:
  template <typename> friend class optional;

  typedef void (optional::*safe_bool)() const;

public:
  typedef T value_type;

  // x.x.3.1, constructors

  // 1a - default construct
  optional_constexpr optional() optional_noexcept : has_value_(false), contained() {}

  // 1b - construct explicitly empty
  // NOLINTNEXTLINE( google-explicit-constructor, hicpp-explicit-conversions )
  optional_constexpr optional(nullopt_t /*unused*/) optional_noexcept : has_value_(false),
                                                                        contained() {}

  // 2 - copy-construct
#if optional_CPP11_OR_GREATER
  // template< typename U = T
  //     optional_REQUIRES_T(
  //         std::is_copy_constructible<U>::value
  //         || std11::is_trivially_copy_constructible<U>::value
  //     )
  // >
#endif
  optional_constexpr14 optional(optional const &other) : has_value_(other.has_value()) {
    if (other.has_value()) {
      contained.construct_value(other.contained.value());
    }
  }

#if optional_CPP11_OR_GREATER

  // 3 (C++11) - move-construct from optional
  template <typename U = T optional_REQUIRES_T(std::is_move_constructible<U>::value ||
                                               std11::is_trivially_move_constructible<U>::value)>
  optional_constexpr14 optional(optional &&other)
      // NOLINTNEXTLINE( performance-noexcept-move-constructor )
      noexcept(std::is_nothrow_move_constructible<T>::value)
      : has_value_(other.has_value()) {
    if (other.has_value()) {
      contained.construct_value(std::move(other.contained.value()));
    }
  }

  // 4a (C++11) - explicit converting copy-construct from optional
  template <typename U optional_REQUIRES_T(std::is_constructible<T, U const &>::value &&
                                           !std::is_constructible<T, optional<U> &>::value &&
                                           !std::is_constructible<T, optional<U> &&>::value &&
                                           !std::is_constructible<T, optional<U> const &>::value &&
                                           !std::is_constructible<T, optional<U> const &&>::value &&
                                           !std::is_convertible<optional<U> &, T>::value &&
                                           !std::is_convertible<optional<U> &&, T>::value &&
                                           !std::is_convertible<optional<U> const &, T>::value &&
                                           !std::is_convertible<optional<U> const &&, T>::value &&
                                           !std::is_convertible<U const &, T>::value /*=> explicit
                                                                                      */
                                           )>
  explicit optional(optional<U> const &other) : has_value_(other.has_value()) {
    if (other.has_value()) {
      contained.construct_value(T{other.contained.value()});
    }
  }
#endif // optional_CPP11_OR_GREATER

  // 4b (C++98 and later) - non-explicit converting copy-construct from optional
  template <typename U
#if optional_CPP11_OR_GREATER
                optional_REQUIRES_T(std::is_constructible<T, U const &>::value &&
                                    !std::is_constructible<T, optional<U> &>::value &&
                                    !std::is_constructible<T, optional<U> &&>::value &&
                                    !std::is_constructible<T, optional<U> const &>::value &&
                                    !std::is_constructible<T, optional<U> const &&>::value &&
                                    !std::is_convertible<optional<U> &, T>::value &&
                                    !std::is_convertible<optional<U> &&, T>::value &&
                                    !std::is_convertible<optional<U> const &, T>::value &&
                                    !std::is_convertible<optional<U> const &&, T>::value &&
                                    std::is_convertible<U const &, T>::value /*=> non-explicit */
                                    )
#endif // optional_CPP11_OR_GREATER
            >
  // NOLINTNEXTLINE( google-explicit-constructor, hicpp-explicit-conversions )
  /*non-explicit*/ optional(optional<U> const &other) : has_value_(other.has_value()) {
    if (other.has_value()) {
      contained.construct_value(other.contained.value());
    }
  }

#if optional_CPP11_OR_GREATER

  // 5a (C++11) - explicit converting move-construct from optional
  template <typename U optional_REQUIRES_T(std::is_constructible<T, U &&>::value &&
                                           !std::is_constructible<T, optional<U> &>::value &&
                                           !std::is_constructible<T, optional<U> &&>::value &&
                                           !std::is_constructible<T, optional<U> const &>::value &&
                                           !std::is_constructible<T, optional<U> const &&>::value &&
                                           !std::is_convertible<optional<U> &, T>::value &&
                                           !std::is_convertible<optional<U> &&, T>::value &&
                                           !std::is_convertible<optional<U> const &, T>::value &&
                                           !std::is_convertible<optional<U> const &&, T>::value &&
                                           !std::is_convertible<U &&, T>::value /*=> explicit */
                                           )>
  explicit optional(optional<U> &&other) : has_value_(other.has_value()) {
    if (other.has_value()) {
      contained.construct_value(T{std::move(other.contained.value())});
    }
  }

  // 5a (C++11) - non-explicit converting move-construct from optional
  template <typename U optional_REQUIRES_T(std::is_constructible<T, U &&>::value &&
                                           !std::is_constructible<T, optional<U> &>::value &&
                                           !std::is_constructible<T, optional<U> &&>::value &&
                                           !std::is_constructible<T, optional<U> const &>::value &&
                                           !std::is_constructible<T, optional<U> const &&>::value &&
                                           !std::is_convertible<optional<U> &, T>::value &&
                                           !std::is_convertible<optional<U> &&, T>::value &&
                                           !std::is_convertible<optional<U> const &, T>::value &&
                                           !std::is_convertible<optional<U> const &&, T>::value &&
                                           std::is_convertible<U &&, T>::value /*=> non-explicit */
                                           )>
  // NOLINTNEXTLINE( google-explicit-constructor, hicpp-explicit-conversions )
  /*non-explicit*/ optional(optional<U> &&other) : has_value_(other.has_value()) {
    if (other.has_value()) {
      contained.construct_value(std::move(other.contained.value()));
    }
  }

  // 6 (C++11) - in-place construct
  template <typename... Args optional_REQUIRES_T(std::is_constructible<T, Args &&...>::value)>
  optional_constexpr explicit optional(nonstd_lite_in_place_t(T), Args &&... args)
      : has_value_(true), contained(T(std::forward<Args>(args)...)) {}

  // 7 (C++11) - in-place construct,  initializer-list
  template <typename U,
            typename... Args optional_REQUIRES_T(
                std::is_constructible<T, std::initializer_list<U> &, Args &&...>::value)>
  optional_constexpr explicit optional(nonstd_lite_in_place_t(T), std::initializer_list<U> il,
                                       Args &&... args)
      : has_value_(true), contained(T(il, std::forward<Args>(args)...)) {}

  // 8a (C++11) - explicit move construct from value
  template <
      typename U = T optional_REQUIRES_T(
          std::is_constructible<T, U &&>::value &&
          !std::is_same<typename std20::remove_cvref<U>::type, nonstd_lite_in_place_t(U)>::value &&
          !std::is_same<typename std20::remove_cvref<U>::type, optional<T>>::value &&
          !std::is_convertible<U &&, T>::value /*=> explicit */
          )>
  optional_constexpr explicit optional(U &&value)
      : has_value_(true), contained(T{std::forward<U>(value)}) {}

  // 8b (C++11) - non-explicit move construct from value
  template <
      typename U = T optional_REQUIRES_T(
          std::is_constructible<T, U &&>::value &&
          !std::is_same<typename std20::remove_cvref<U>::type, nonstd_lite_in_place_t(U)>::value &&
          !std::is_same<typename std20::remove_cvref<U>::type, optional<T>>::value &&
          std::is_convertible<U &&, T>::value /*=> non-explicit */
          )>
  // NOLINTNEXTLINE( google-explicit-constructor, hicpp-explicit-conversions )
  optional_constexpr /*non-explicit*/ optional(U &&value)
      : has_value_(true), contained(std::forward<U>(value)) {}

#else // optional_CPP11_OR_GREATER

  // 8 (C++98)
  optional(value_type const &value) : has_value_(true), contained(value) {}

#endif // optional_CPP11_OR_GREATER

  // x.x.3.2, destructor

  ~optional() {
    if (has_value()) {
      contained.destruct_value();
    }
  }

  // x.x.3.3, assignment

  // 1 (C++98and later) -  assign explicitly empty
  optional &operator=(nullopt_t /*unused*/) optional_noexcept {
    reset();
    return *this;
  }

  // 2 (C++98and later) - copy-assign from optional
#if optional_CPP11_OR_GREATER
  // NOLINTNEXTLINE( cppcoreguidelines-c-copy-assignment-signature,
  // misc-unconventional-assign-operator )
  optional_REQUIRES_R(optional &, true
                      //      std::is_copy_constructible<T>::value
                      //      && std::is_copy_assignable<T>::value
                      )
  operator=(optional const &other) noexcept(
      std::is_nothrow_move_assignable<T>::value &&std::is_nothrow_move_constructible<T>::value)
#else
  optional &operator=(optional const &other)
#endif
  {
    if ((has_value() == true) && (other.has_value() == false)) {
      reset();
    } else if ((has_value() == false) && (other.has_value() == true)) {
      initialize(*other);
    } else if ((has_value() == true) && (other.has_value() == true)) {
      contained.value() = *other;
    }
    return *this;
  }

#if optional_CPP11_OR_GREATER

  // 3 (C++11) - move-assign from optional
  // NOLINTNEXTLINE( cppcoreguidelines-c-copy-assignment-signature,
  // misc-unconventional-assign-operator )
  optional_REQUIRES_R(optional &, true
                      //      std::is_move_constructible<T>::value
                      //      && std::is_move_assignable<T>::value
                      )
  operator=(optional &&other) noexcept {
    if ((has_value() == true) && (other.has_value() == false)) {
      reset();
    } else if ((has_value() == false) && (other.has_value() == true)) {
      initialize(std::move(*other));
    } else if ((has_value() == true) && (other.has_value() == true)) {
      contained.value() = std::move(*other);
    }
    return *this;
  }

  // 4 (C++11) - move-assign from value
  template <typename U = T>
  // NOLINTNEXTLINE( cppcoreguidelines-c-copy-assignment-signature,
  // misc-unconventional-assign-operator )
  optional_REQUIRES_R(
      optional &,
      std::is_constructible<T, U>::value &&std::is_assignable<T &, U>::value &&
          !std::is_same<typename std20::remove_cvref<U>::type, nonstd_lite_in_place_t(U)>::value &&
          !std::is_same<typename std20::remove_cvref<U>::type, optional<T>>::value &&
          !(std::is_scalar<T>::value && std::is_same<T, typename std::decay<U>::type>::value))
  operator=(U &&value) {
    if (has_value()) {
      contained.value() = std::forward<U>(value);
    } else {
      initialize(T(std::forward<U>(value)));
    }
    return *this;
  }

#else // optional_CPP11_OR_GREATER

  // 4 (C++98) - copy-assign from value
  template <typename U /*= T*/> optional &operator=(U const &value) {
    if (has_value())
      contained.value() = value;
    else
      initialize(T(value));
    return *this;
  }

#endif // optional_CPP11_OR_GREATER

  // 5 (C++98 and later) - converting copy-assign from optional
  template <typename U>
#if optional_CPP11_OR_GREATER
  // NOLINTNEXTLINE( cppcoreguidelines-c-copy-assignment-signature,
  // misc-unconventional-assign-operator )
  optional_REQUIRES_R(
      optional &,
      std::is_constructible<T, U const &>::value &&std::is_assignable<T &, U const &>::value &&
          !std::is_constructible<T, optional<U> &>::value &&
          !std::is_constructible<T, optional<U> &&>::value &&
          !std::is_constructible<T, optional<U> const &>::value &&
          !std::is_constructible<T, optional<U> const &&>::value &&
          !std::is_convertible<optional<U> &, T>::value &&
          !std::is_convertible<optional<U> &&, T>::value &&
          !std::is_convertible<optional<U> const &, T>::value &&
          !std::is_convertible<optional<U> const &&, T>::value &&
          !std::is_assignable<T &, optional<U> &>::value &&
          !std::is_assignable<T &, optional<U> &&>::value &&
          !std::is_assignable<T &, optional<U> const &>::value &&
          !std::is_assignable<T &, optional<U> const &&>::value)
#else
  optional &
#endif // optional_CPP11_OR_GREATER
  operator=(optional<U> const &other) {
    return *this = optional(other);
  }

#if optional_CPP11_OR_GREATER

  // 6 (C++11) -  converting move-assign from optional
  template <typename U>
  // NOLINTNEXTLINE( cppcoreguidelines-c-copy-assignment-signature,
  // misc-unconventional-assign-operator )
  optional_REQUIRES_R(optional &,
                      std::is_constructible<T, U>::value &&std::is_assignable<T &, U>::value &&
                          !std::is_constructible<T, optional<U> &>::value &&
                          !std::is_constructible<T, optional<U> &&>::value &&
                          !std::is_constructible<T, optional<U> const &>::value &&
                          !std::is_constructible<T, optional<U> const &&>::value &&
                          !std::is_convertible<optional<U> &, T>::value &&
                          !std::is_convertible<optional<U> &&, T>::value &&
                          !std::is_convertible<optional<U> const &, T>::value &&
                          !std::is_convertible<optional<U> const &&, T>::value &&
                          !std::is_assignable<T &, optional<U> &>::value &&
                          !std::is_assignable<T &, optional<U> &&>::value &&
                          !std::is_assignable<T &, optional<U> const &>::value &&
                          !std::is_assignable<T &, optional<U> const &&>::value)
  operator=(optional<U> &&other) {
    return *this = optional(std::move(other));
  }

  // 7 (C++11) - emplace
  template <typename... Args optional_REQUIRES_T(std::is_constructible<T, Args &&...>::value)>
  T &emplace(Args &&... args) {
    *this = nullopt;
    contained.emplace(std::forward<Args>(args)...);
    has_value_ = true;
    return contained.value();
  }

  // 8 (C++11) - emplace, initializer-list
  template <typename U,
            typename... Args optional_REQUIRES_T(
                std::is_constructible<T, std::initializer_list<U> &, Args &&...>::value)>
  T &emplace(std::initializer_list<U> il, Args &&... args) {
    *this = nullopt;
    contained.emplace(il, std::forward<Args>(args)...);
    has_value_ = true;
    return contained.value();
  }

#endif // optional_CPP11_OR_GREATER

  // x.x.3.4, swap

  void swap(optional &other)
#if optional_CPP11_OR_GREATER
      noexcept(std::is_nothrow_move_constructible<T>::value &&std17::is_nothrow_swappable<T>::value)
#endif
  {
    using std::swap;
    if ((has_value() == true) && (other.has_value() == true)) {
      swap(**this, *other);
    } else if ((has_value() == false) && (other.has_value() == true)) {
      initialize(std11::move(*other));
      other.reset();
    } else if ((has_value() == true) && (other.has_value() == false)) {
      other.initialize(std11::move(**this));
      reset();
    }
  }

  // x.x.3.5, observers

  optional_constexpr value_type const *operator->() const {
    return assert(has_value()), contained.value_ptr();
  }

  optional_constexpr14 value_type *operator->() {
    return assert(has_value()), contained.value_ptr();
  }

  optional_constexpr value_type const &operator*() const optional_ref_qual {
    return assert(has_value()), contained.value();
  }

  optional_constexpr14 value_type &operator*() optional_ref_qual {
    return assert(has_value()), contained.value();
  }

#if optional_HAVE(REF_QUALIFIER)

  optional_constexpr value_type const &&operator*() const optional_refref_qual {
    return std::move(**this);
  }

  optional_constexpr14 value_type &&operator*() optional_refref_qual { return std::move(**this); }

#endif

#if optional_CPP11_OR_GREATER
  optional_constexpr explicit operator bool() const optional_noexcept { return has_value(); }
#else
  optional_constexpr operator safe_bool() const optional_noexcept {
    return has_value() ? &optional::this_type_does_not_support_comparisons : 0;
  }
#endif

  // NOLINTNEXTLINE( modernize-use-nodiscard )
  /*optional_nodiscard*/ optional_constexpr bool has_value() const optional_noexcept {
    return has_value_;
  }

  // NOLINTNEXTLINE( modernize-use-nodiscard )
  /*optional_nodiscard*/ optional_constexpr14 value_type const &value() const optional_ref_qual {
#if optional_CONFIG_NO_EXCEPTIONS
    assert(has_value());
#else
    if (!has_value()) {
      throw bad_optional_access();
    }
#endif
    return contained.value();
  }

  optional_constexpr14 value_type &value() optional_ref_qual {
#if optional_CONFIG_NO_EXCEPTIONS
    assert(has_value());
#else
    if (!has_value()) {
      throw bad_optional_access();
    }
#endif
    return contained.value();
  }

#if optional_HAVE(REF_QUALIFIER) &&                                                                \
    (!optional_COMPILER_GNUC_VERSION || optional_COMPILER_GNUC_VERSION >= 490)

  // NOLINTNEXTLINE( modernize-use-nodiscard )
  /*optional_nodiscard*/ optional_constexpr value_type const &&value() const optional_refref_qual {
    return std::move(value());
  }

  optional_constexpr14 value_type &&value() optional_refref_qual { return std::move(value()); }

#endif

#if optional_CPP11_OR_GREATER

  template <typename U> optional_constexpr value_type value_or(U &&v) const optional_ref_qual {
    return has_value() ? contained.value() : static_cast<T>(std::forward<U>(v));
  }

  template <typename U> optional_constexpr14 value_type value_or(U &&v) optional_refref_qual {
    return has_value() ? std::move(contained.value()) : static_cast<T>(std::forward<U>(v));
  }

#else

  template <typename U> optional_constexpr value_type value_or(U const &v) const {
    return has_value() ? contained.value() : static_cast<value_type>(v);
  }

#endif // optional_CPP11_OR_GREATER

  // x.x.3.6, modifiers

  void reset() optional_noexcept {
    if (has_value()) {
      contained.destruct_value();
    }

    has_value_ = false;
  }

private:
  void this_type_does_not_support_comparisons() const {}

  template <typename V> void initialize(V const &value) {
    assert(!has_value());
    contained.construct_value(value);
    has_value_ = true;
  }

#if optional_CPP11_OR_GREATER
  template <typename V> void initialize(V &&value) {
    assert(!has_value());
    contained.construct_value(std::move(value));
    has_value_ = true;
  }

#endif

private:
  bool has_value_;
  detail::storage_t<value_type> contained;
};

// Relational operators

template <typename T, typename U>
inline optional_constexpr bool operator==(optional<T> const &x, optional<U> const &y) {
  return bool(x) != bool(y) ? false : !bool(x) ? true : *x == *y;
}

template <typename T, typename U>
inline optional_constexpr bool operator!=(optional<T> const &x, optional<U> const &y) {
  return !(x == y);
}

template <typename T, typename U>
inline optional_constexpr bool operator<(optional<T> const &x, optional<U> const &y) {
  return (!y) ? false : (!x) ? true : *x < *y;
}

template <typename T, typename U>
inline optional_constexpr bool operator>(optional<T> const &x, optional<U> const &y) {
  return (y < x);
}

template <typename T, typename U>
inline optional_constexpr bool operator<=(optional<T> const &x, optional<U> const &y) {
  return !(y < x);
}

template <typename T, typename U>
inline optional_constexpr bool operator>=(optional<T> const &x, optional<U> const &y) {
  return !(x < y);
}

// Comparison with nullopt

template <typename T>
inline optional_constexpr bool operator==(optional<T> const &x,
                                          nullopt_t /*unused*/) optional_noexcept {
  return (!x);
}

template <typename T>
inline optional_constexpr bool operator==(nullopt_t /*unused*/,
                                          optional<T> const &x) optional_noexcept {
  return (!x);
}

template <typename T>
inline optional_constexpr bool operator!=(optional<T> const &x,
                                          nullopt_t /*unused*/) optional_noexcept {
  return bool(x);
}

template <typename T>
inline optional_constexpr bool operator!=(nullopt_t /*unused*/,
                                          optional<T> const &x) optional_noexcept {
  return bool(x);
}

template <typename T>
inline optional_constexpr bool operator<(optional<T> const & /*unused*/,
                                         nullopt_t /*unused*/) optional_noexcept {
  return false;
}

template <typename T>
inline optional_constexpr bool operator<(nullopt_t /*unused*/,
                                         optional<T> const &x) optional_noexcept {
  return bool(x);
}

template <typename T>
inline optional_constexpr bool operator<=(optional<T> const &x,
                                          nullopt_t /*unused*/) optional_noexcept {
  return (!x);
}

template <typename T>
inline optional_constexpr bool operator<=(nullopt_t /*unused*/,
                                          optional<T> const & /*unused*/) optional_noexcept {
  return true;
}

template <typename T>
inline optional_constexpr bool operator>(optional<T> const &x,
                                         nullopt_t /*unused*/) optional_noexcept {
  return bool(x);
}

template <typename T>
inline optional_constexpr bool operator>(nullopt_t /*unused*/,
                                         optional<T> const & /*unused*/) optional_noexcept {
  return false;
}

template <typename T>
inline optional_constexpr bool operator>=(optional<T> const & /*unused*/,
                                          nullopt_t /*unused*/) optional_noexcept {
  return true;
}

template <typename T>
inline optional_constexpr bool operator>=(nullopt_t /*unused*/,
                                          optional<T> const &x) optional_noexcept {
  return (!x);
}

// Comparison with T

template <typename T, typename U>
inline optional_constexpr bool operator==(optional<T> const &x, U const &v) {
  return bool(x) ? *x == v : false;
}

template <typename T, typename U>
inline optional_constexpr bool operator==(U const &v, optional<T> const &x) {
  return bool(x) ? v == *x : false;
}

template <typename T, typename U>
inline optional_constexpr bool operator!=(optional<T> const &x, U const &v) {
  return bool(x) ? *x != v : true;
}

template <typename T, typename U>
inline optional_constexpr bool operator!=(U const &v, optional<T> const &x) {
  return bool(x) ? v != *x : true;
}

template <typename T, typename U>
inline optional_constexpr bool operator<(optional<T> const &x, U const &v) {
  return bool(x) ? *x < v : true;
}

template <typename T, typename U>
inline optional_constexpr bool operator<(U const &v, optional<T> const &x) {
  return bool(x) ? v < *x : false;
}

template <typename T, typename U>
inline optional_constexpr bool operator<=(optional<T> const &x, U const &v) {
  return bool(x) ? *x <= v : true;
}

template <typename T, typename U>
inline optional_constexpr bool operator<=(U const &v, optional<T> const &x) {
  return bool(x) ? v <= *x : false;
}

template <typename T, typename U>
inline optional_constexpr bool operator>(optional<T> const &x, U const &v) {
  return bool(x) ? *x > v : false;
}

template <typename T, typename U>
inline optional_constexpr bool operator>(U const &v, optional<T> const &x) {
  return bool(x) ? v > *x : true;
}

template <typename T, typename U>
inline optional_constexpr bool operator>=(optional<T> const &x, U const &v) {
  return bool(x) ? *x >= v : false;
}

template <typename T, typename U>
inline optional_constexpr bool operator>=(U const &v, optional<T> const &x) {
  return bool(x) ? v >= *x : true;
}

// Specialized algorithms

template <
    typename T
#if optional_CPP11_OR_GREATER
        optional_REQUIRES_T(std::is_move_constructible<T>::value &&std17::is_swappable<T>::value)
#endif
    >
void swap(optional<T> &x, optional<T> &y)
#if optional_CPP11_OR_GREATER
    noexcept(noexcept(x.swap(y)))
#endif
{
  x.swap(y);
}

#if optional_CPP11_OR_GREATER

template <typename T>
optional_constexpr optional<typename std::decay<T>::type> make_optional(T &&value) {
  return optional<typename std::decay<T>::type>(std::forward<T>(value));
}

template <typename T, typename... Args>
optional_constexpr optional<T> make_optional(Args &&... args) {
  return optional<T>(nonstd_lite_in_place(T), std::forward<Args>(args)...);
}

template <typename T, typename U, typename... Args>
optional_constexpr optional<T> make_optional(std::initializer_list<U> il, Args &&... args) {
  return optional<T>(nonstd_lite_in_place(T), il, std::forward<Args>(args)...);
}

#else

template <typename T> optional<T> make_optional(T const &value) { return optional<T>(value); }

#endif // optional_CPP11_OR_GREATER

} // namespace optional_lite

using optional_lite::nullopt;
using optional_lite::nullopt_t;
using optional_lite::optional;

#if !optional_CONFIG_NO_EXCEPTIONS
using optional_lite::bad_optional_access;
#endif

using optional_lite::make_optional;

} // namespace nonstd

#if optional_CPP11_OR_GREATER

// specialize the std::hash algorithm:

namespace std {

template <class T> struct hash<nonstd::optional<T>> {
public:
  std::size_t operator()(nonstd::optional<T> const &v) const optional_noexcept {
    return bool(v) ? std::hash<T>{}(*v) : 0;
  }
};

} // namespace std

#endif // optional_CPP11_OR_GREATER

#if defined(__clang__)
#pragma clang diagnostic pop
#elif defined(__GNUC__)
#pragma GCC diagnostic pop
#elif defined(_MSC_VER)
#pragma warning(pop)
#endif

#endif // optional_USES_STD_OPTIONAL

#endif // NONSTD_OPTIONAL_LITE_HPP
// Copyright 2017-2020 by Martin Moene
//
// string-view lite, a C++17-like string_view for C++98 and later.
// For more information see https://github.com/martinmoene/string-view-lite
//
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file LICENSE.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#pragma once

#ifndef NONSTD_SV_LITE_H_INCLUDED
#define NONSTD_SV_LITE_H_INCLUDED

#define string_view_lite_MAJOR  1
#define string_view_lite_MINOR  6
#define string_view_lite_PATCH  0

#define string_view_lite_VERSION  nssv_STRINGIFY(string_view_lite_MAJOR) "." nssv_STRINGIFY(string_view_lite_MINOR) "." nssv_STRINGIFY(string_view_lite_PATCH)

#define nssv_STRINGIFY(  x )  nssv_STRINGIFY_( x )
#define nssv_STRINGIFY_( x )  #x

// string-view lite configuration:

#define nssv_STRING_VIEW_DEFAULT  0
#define nssv_STRING_VIEW_NONSTD   1
#define nssv_STRING_VIEW_STD      2

// tweak header support:

#ifdef __has_include
# if __has_include(<nonstd/string_view.tweak.hpp>)
#  include <nonstd/string_view.tweak.hpp>
# endif
#define nssv_HAVE_TWEAK_HEADER  1
#else
#define nssv_HAVE_TWEAK_HEADER  0
//# pragma message("string_view.hpp: Note: Tweak header not supported.")
#endif

// string_view selection and configuration:

#if !defined( nssv_CONFIG_SELECT_STRING_VIEW )
# define nssv_CONFIG_SELECT_STRING_VIEW  ( nssv_HAVE_STD_STRING_VIEW ? nssv_STRING_VIEW_STD : nssv_STRING_VIEW_NONSTD )
#endif

#ifndef  nssv_CONFIG_STD_SV_OPERATOR
# define nssv_CONFIG_STD_SV_OPERATOR  0
#endif

#ifndef  nssv_CONFIG_USR_SV_OPERATOR
# define nssv_CONFIG_USR_SV_OPERATOR  1
#endif

#ifdef   nssv_CONFIG_CONVERSION_STD_STRING
# define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS   nssv_CONFIG_CONVERSION_STD_STRING
# define nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS  nssv_CONFIG_CONVERSION_STD_STRING
#endif

#ifndef  nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS
# define nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS  1
#endif

#ifndef  nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
# define nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS  1
#endif

#ifndef  nssv_CONFIG_NO_STREAM_INSERTION
# define nssv_CONFIG_NO_STREAM_INSERTION  0
#endif

// Control presence of exception handling (try and auto discover):

#ifndef nssv_CONFIG_NO_EXCEPTIONS
# if defined(_MSC_VER)
#  include <cstddef>    // for _HAS_EXCEPTIONS
# endif
# if defined(__cpp_exceptions) || defined(__EXCEPTIONS) || (_HAS_EXCEPTIONS)
#  define nssv_CONFIG_NO_EXCEPTIONS  0
# else
#  define nssv_CONFIG_NO_EXCEPTIONS  1
# endif
#endif

// C++ language version detection (C++20 is speculative):
// Note: VC14.0/1900 (VS2015) lacks too much from C++14.

#ifndef   nssv_CPLUSPLUS
# if defined(_MSVC_LANG ) && !defined(__clang__)
#  define nssv_CPLUSPLUS  (_MSC_VER == 1900 ? 201103L : _MSVC_LANG )
# else
#  define nssv_CPLUSPLUS  __cplusplus
# endif
#endif

#define nssv_CPP98_OR_GREATER  ( nssv_CPLUSPLUS >= 199711L )
#define nssv_CPP11_OR_GREATER  ( nssv_CPLUSPLUS >= 201103L )
#define nssv_CPP11_OR_GREATER_ ( nssv_CPLUSPLUS >= 201103L )
#define nssv_CPP14_OR_GREATER  ( nssv_CPLUSPLUS >= 201402L )
#define nssv_CPP17_OR_GREATER  ( nssv_CPLUSPLUS >= 201703L )
#define nssv_CPP20_OR_GREATER  ( nssv_CPLUSPLUS >= 202000L )

// use C++17 std::string_view if available and requested:

#if nssv_CPP17_OR_GREATER && defined(__has_include )
# if __has_include( <string_view> )
#  define nssv_HAVE_STD_STRING_VIEW  1
# else
#  define nssv_HAVE_STD_STRING_VIEW  0
# endif
#else
# define  nssv_HAVE_STD_STRING_VIEW  0
#endif

#define  nssv_USES_STD_STRING_VIEW  ( (nssv_CONFIG_SELECT_STRING_VIEW == nssv_STRING_VIEW_STD) || ((nssv_CONFIG_SELECT_STRING_VIEW == nssv_STRING_VIEW_DEFAULT) && nssv_HAVE_STD_STRING_VIEW) )

#define nssv_HAVE_STARTS_WITH ( nssv_CPP20_OR_GREATER || !nssv_USES_STD_STRING_VIEW )
#define nssv_HAVE_ENDS_WITH     nssv_HAVE_STARTS_WITH

//
// Use C++17 std::string_view:
//

#if nssv_USES_STD_STRING_VIEW

#include <string_view>

// Extensions for std::string:

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {

template< class CharT, class Traits, class Allocator = std::allocator<CharT> >
std::basic_string<CharT, Traits, Allocator>
to_string( std::basic_string_view<CharT, Traits> v, Allocator const & a = Allocator() )
{
    return std::basic_string<CharT,Traits, Allocator>( v.begin(), v.end(), a );
}

template< class CharT, class Traits, class Allocator >
std::basic_string_view<CharT, Traits>
to_string_view( std::basic_string<CharT, Traits, Allocator> const & s )
{
    return std::basic_string_view<CharT, Traits>( s.data(), s.size() );
}

// Literal operators sv and _sv:

#if nssv_CONFIG_STD_SV_OPERATOR

using namespace std::literals::string_view_literals;

#endif

#if nssv_CONFIG_USR_SV_OPERATOR

inline namespace literals {
inline namespace string_view_literals {


constexpr std::string_view operator "" _sv( const char* str, size_t len ) noexcept  // (1)
{
    return std::string_view{ str, len };
}

constexpr std::u16string_view operator "" _sv( const char16_t* str, size_t len ) noexcept  // (2)
{
    return std::u16string_view{ str, len };
}

constexpr std::u32string_view operator "" _sv( const char32_t* str, size_t len ) noexcept  // (3)
{
    return std::u32string_view{ str, len };
}

constexpr std::wstring_view operator "" _sv( const wchar_t* str, size_t len ) noexcept  // (4)
{
    return std::wstring_view{ str, len };
}

}} // namespace literals::string_view_literals

#endif // nssv_CONFIG_USR_SV_OPERATOR

} // namespace nonstd

#endif // nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {

using std::string_view;
using std::wstring_view;
using std::u16string_view;
using std::u32string_view;
using std::basic_string_view;

// literal "sv" and "_sv", see above

using std::operator==;
using std::operator!=;
using std::operator<;
using std::operator<=;
using std::operator>;
using std::operator>=;

using std::operator<<;

} // namespace nonstd

#else // nssv_HAVE_STD_STRING_VIEW

//
// Before C++17: use string_view lite:
//

// Compiler versions:
//
// MSVC++  6.0  _MSC_VER == 1200  nssv_COMPILER_MSVC_VERSION ==  60  (Visual Studio 6.0)
// MSVC++  7.0  _MSC_VER == 1300  nssv_COMPILER_MSVC_VERSION ==  70  (Visual Studio .NET 2002)
// MSVC++  7.1  _MSC_VER == 1310  nssv_COMPILER_MSVC_VERSION ==  71  (Visual Studio .NET 2003)
// MSVC++  8.0  _MSC_VER == 1400  nssv_COMPILER_MSVC_VERSION ==  80  (Visual Studio 2005)
// MSVC++  9.0  _MSC_VER == 1500  nssv_COMPILER_MSVC_VERSION ==  90  (Visual Studio 2008)
// MSVC++ 10.0  _MSC_VER == 1600  nssv_COMPILER_MSVC_VERSION == 100  (Visual Studio 2010)
// MSVC++ 11.0  _MSC_VER == 1700  nssv_COMPILER_MSVC_VERSION == 110  (Visual Studio 2012)
// MSVC++ 12.0  _MSC_VER == 1800  nssv_COMPILER_MSVC_VERSION == 120  (Visual Studio 2013)
// MSVC++ 14.0  _MSC_VER == 1900  nssv_COMPILER_MSVC_VERSION == 140  (Visual Studio 2015)
// MSVC++ 14.1  _MSC_VER >= 1910  nssv_COMPILER_MSVC_VERSION == 141  (Visual Studio 2017)
// MSVC++ 14.2  _MSC_VER >= 1920  nssv_COMPILER_MSVC_VERSION == 142  (Visual Studio 2019)

#if defined(_MSC_VER ) && !defined(__clang__)
# define nssv_COMPILER_MSVC_VER      (_MSC_VER )
# define nssv_COMPILER_MSVC_VERSION  (_MSC_VER / 10 - 10 * ( 5 + (_MSC_VER < 1900 ) ) )
#else
# define nssv_COMPILER_MSVC_VER      0
# define nssv_COMPILER_MSVC_VERSION  0
#endif

#define nssv_COMPILER_VERSION( major, minor, patch )  ( 10 * ( 10 * (major) + (minor) ) + (patch) )

#if defined( __apple_build_version__ )
# define nssv_COMPILER_APPLECLANG_VERSION  nssv_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
# define nssv_COMPILER_CLANG_VERSION       0
#elif defined( __clang__ )
# define nssv_COMPILER_APPLECLANG_VERSION  0
# define nssv_COMPILER_CLANG_VERSION       nssv_COMPILER_VERSION(__clang_major__, __clang_minor__, __clang_patchlevel__)
#else
# define nssv_COMPILER_APPLECLANG_VERSION  0
# define nssv_COMPILER_CLANG_VERSION       0
#endif

#if defined(__GNUC__) && !defined(__clang__)
# define nssv_COMPILER_GNUC_VERSION  nssv_COMPILER_VERSION(__GNUC__, __GNUC_MINOR__, __GNUC_PATCHLEVEL__)
#else
# define nssv_COMPILER_GNUC_VERSION  0
#endif

// half-open range [lo..hi):
#define nssv_BETWEEN( v, lo, hi ) ( (lo) <= (v) && (v) < (hi) )

// Presence of language and library features:

#ifdef _HAS_CPP0X
# define nssv_HAS_CPP0X  _HAS_CPP0X
#else
# define nssv_HAS_CPP0X  0
#endif

// Unless defined otherwise below, consider VC14 as C++11 for variant-lite:

#if nssv_COMPILER_MSVC_VER >= 1900
# undef  nssv_CPP11_OR_GREATER
# define nssv_CPP11_OR_GREATER  1
#endif

#define nssv_CPP11_90   (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1500)
#define nssv_CPP11_100  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1600)
#define nssv_CPP11_110  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1700)
#define nssv_CPP11_120  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1800)
#define nssv_CPP11_140  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1900)
#define nssv_CPP11_141  (nssv_CPP11_OR_GREATER_ || nssv_COMPILER_MSVC_VER >= 1910)

#define nssv_CPP14_000  (nssv_CPP14_OR_GREATER)
#define nssv_CPP17_000  (nssv_CPP17_OR_GREATER)

// Presence of C++11 language features:

#define nssv_HAVE_CONSTEXPR_11          nssv_CPP11_140
#define nssv_HAVE_EXPLICIT_CONVERSION   nssv_CPP11_140
#define nssv_HAVE_INLINE_NAMESPACE      nssv_CPP11_140
#define nssv_HAVE_NOEXCEPT              nssv_CPP11_140
#define nssv_HAVE_NULLPTR               nssv_CPP11_100
#define nssv_HAVE_REF_QUALIFIER         nssv_CPP11_140
#define nssv_HAVE_UNICODE_LITERALS      nssv_CPP11_140
#define nssv_HAVE_USER_DEFINED_LITERALS nssv_CPP11_140
#define nssv_HAVE_WCHAR16_T             nssv_CPP11_100
#define nssv_HAVE_WCHAR32_T             nssv_CPP11_100

#if ! ( ( nssv_CPP11_OR_GREATER && nssv_COMPILER_CLANG_VERSION ) || nssv_BETWEEN( nssv_COMPILER_CLANG_VERSION, 300, 400 ) )
# define nssv_HAVE_STD_DEFINED_LITERALS  nssv_CPP11_140
#else
# define nssv_HAVE_STD_DEFINED_LITERALS  0
#endif

// Presence of C++14 language features:

#define nssv_HAVE_CONSTEXPR_14          nssv_CPP14_000

// Presence of C++17 language features:

#define nssv_HAVE_NODISCARD             nssv_CPP17_000

// Presence of C++ library features:

#define nssv_HAVE_STD_HASH              nssv_CPP11_120

// Presence of compiler intrinsics:

// Providing char-type specializations for compare() and length() that
// use compiler intrinsics can improve compile- and run-time performance.
//
// The challenge is in using the right combinations of builtin availability
// and its constexpr-ness.
//
// | compiler | __builtin_memcmp (constexpr) | memcmp  (constexpr) |
// |----------|------------------------------|---------------------|
// | clang    | 4.0              (>= 4.0   ) | any     (?        ) |
// | clang-a  | 9.0              (>= 9.0   ) | any     (?        ) |
// | gcc      | any              (constexpr) | any     (?        ) |
// | msvc     | >= 14.2 C++17    (>= 14.2  ) | any     (?        ) |

#define nssv_HAVE_BUILTIN_VER     ( (nssv_CPP17_000 && nssv_COMPILER_MSVC_VERSION >= 142) || nssv_COMPILER_GNUC_VERSION > 0 || nssv_COMPILER_CLANG_VERSION >= 400 || nssv_COMPILER_APPLECLANG_VERSION >= 900 )
#define nssv_HAVE_BUILTIN_CE      (  nssv_HAVE_BUILTIN_VER )

#define nssv_HAVE_BUILTIN_MEMCMP  ( (nssv_HAVE_CONSTEXPR_14 && nssv_HAVE_BUILTIN_CE) || !nssv_HAVE_CONSTEXPR_14 )
#define nssv_HAVE_BUILTIN_STRLEN  ( (nssv_HAVE_CONSTEXPR_11 && nssv_HAVE_BUILTIN_CE) || !nssv_HAVE_CONSTEXPR_11 )

#ifdef __has_builtin
# define nssv_HAVE_BUILTIN( x )  __has_builtin( x )
#else
# define nssv_HAVE_BUILTIN( x )  0
#endif

#if nssv_HAVE_BUILTIN(__builtin_memcmp) || nssv_HAVE_BUILTIN_VER
# define nssv_BUILTIN_MEMCMP  __builtin_memcmp
#else
# define nssv_BUILTIN_MEMCMP  memcmp
#endif

#if nssv_HAVE_BUILTIN(__builtin_strlen) || nssv_HAVE_BUILTIN_VER
# define nssv_BUILTIN_STRLEN  __builtin_strlen
#else
# define nssv_BUILTIN_STRLEN  strlen
#endif

// C++ feature usage:

#if nssv_HAVE_CONSTEXPR_11
# define nssv_constexpr  constexpr
#else
# define nssv_constexpr  /*constexpr*/
#endif

#if  nssv_HAVE_CONSTEXPR_14
# define nssv_constexpr14  constexpr
#else
# define nssv_constexpr14  /*constexpr*/
#endif

#if nssv_HAVE_EXPLICIT_CONVERSION
# define nssv_explicit  explicit
#else
# define nssv_explicit  /*explicit*/
#endif

#if nssv_HAVE_INLINE_NAMESPACE
# define nssv_inline_ns  inline
#else
# define nssv_inline_ns  /*inline*/
#endif

#if nssv_HAVE_NOEXCEPT
# define nssv_noexcept  noexcept
#else
# define nssv_noexcept  /*noexcept*/
#endif

//#if nssv_HAVE_REF_QUALIFIER
//# define nssv_ref_qual  &
//# define nssv_refref_qual  &&
//#else
//# define nssv_ref_qual  /*&*/
//# define nssv_refref_qual  /*&&*/
//#endif

#if nssv_HAVE_NULLPTR
# define nssv_nullptr  nullptr
#else
# define nssv_nullptr  NULL
#endif

#if nssv_HAVE_NODISCARD
# define nssv_nodiscard  [[nodiscard]]
#else
# define nssv_nodiscard  /*[[nodiscard]]*/
#endif

// Additional includes:

#include <algorithm>
#include <cassert>
#include <iterator>
#include <limits>
#include <string>   // std::char_traits<>

#if ! nssv_CONFIG_NO_STREAM_INSERTION
# include <ostream>
#endif

#if ! nssv_CONFIG_NO_EXCEPTIONS
# include <stdexcept>
#endif

#if nssv_CPP11_OR_GREATER
# include <type_traits>
#endif

// Clang, GNUC, MSVC warning suppression macros:

#if defined(__clang__)
# pragma clang diagnostic ignored "-Wreserved-user-defined-literal"
# pragma clang diagnostic push
# pragma clang diagnostic ignored "-Wuser-defined-literals"
#elif defined(__GNUC__)
# pragma  GCC  diagnostic push
# pragma  GCC  diagnostic ignored "-Wliteral-suffix"
#endif // __clang__

#if nssv_COMPILER_MSVC_VERSION >= 140
# define nssv_SUPPRESS_MSGSL_WARNING(expr)        [[gsl::suppress(expr)]]
# define nssv_SUPPRESS_MSVC_WARNING(code, descr)  __pragma(warning(suppress: code) )
# define nssv_DISABLE_MSVC_WARNINGS(codes)        __pragma(warning(push))  __pragma(warning(disable: codes))
#else
# define nssv_SUPPRESS_MSGSL_WARNING(expr)
# define nssv_SUPPRESS_MSVC_WARNING(code, descr)
# define nssv_DISABLE_MSVC_WARNINGS(codes)
#endif

#if defined(__clang__)
# define nssv_RESTORE_WARNINGS()  _Pragma("clang diagnostic pop")
#elif defined(__GNUC__)
# define nssv_RESTORE_WARNINGS()  _Pragma("GCC diagnostic pop")
#elif nssv_COMPILER_MSVC_VERSION >= 140
# define nssv_RESTORE_WARNINGS()  __pragma(warning(pop ))
#else
# define nssv_RESTORE_WARNINGS()
#endif

// Suppress the following MSVC (GSL) warnings:
// - C4455, non-gsl   : 'operator ""sv': literal suffix identifiers that do not
//                      start with an underscore are reserved
// - C26472, gsl::t.1 : don't use a static_cast for arithmetic conversions;
//                      use brace initialization, gsl::narrow_cast or gsl::narow
// - C26481: gsl::b.1 : don't use pointer arithmetic. Use span instead

nssv_DISABLE_MSVC_WARNINGS( 4455 26481 26472 )
//nssv_DISABLE_CLANG_WARNINGS( "-Wuser-defined-literals" )
//nssv_DISABLE_GNUC_WARNINGS( -Wliteral-suffix )

namespace nonstd { namespace sv_lite {

namespace detail {

// support constexpr comparison in C++14;
// for C++17 and later, use provided traits:

template< typename CharT >
inline nssv_constexpr14 int compare( CharT const * s1, CharT const * s2, std::size_t count )
{
    while ( count-- != 0 )
    {
        if ( *s1 < *s2 ) return -1;
        if ( *s1 > *s2 ) return +1;
        ++s1; ++s2;
    }
    return 0;
}

#if nssv_HAVE_BUILTIN_MEMCMP

// specialization of compare() for char, see also generic compare() above:

inline nssv_constexpr14 int compare( char const * s1, char const * s2, std::size_t count )
{
    return nssv_BUILTIN_MEMCMP( s1, s2, count );
}

#endif

#if nssv_HAVE_BUILTIN_STRLEN

// specialization of length() for char, see also generic length() further below:

inline nssv_constexpr std::size_t length( char const * s )
{
    return nssv_BUILTIN_STRLEN( s );
}

#endif

#if defined(__OPTIMIZE__)

// gcc, clang provide __OPTIMIZE__
// Expect tail call optimization to make length() non-recursive:

template< typename CharT >
inline nssv_constexpr std::size_t length( CharT * s, std::size_t result = 0 )
{
    return *s == '\0' ? result : length( s + 1, result + 1 );
}

#else // OPTIMIZE

// non-recursive:

template< typename CharT >
inline nssv_constexpr14 std::size_t length( CharT * s )
{
    std::size_t result = 0;
    while ( *s++ != '\0' )
    {
       ++result;
    }
    return result;
}

#endif // OPTIMIZE

} // namespace detail

template
<
    class CharT,
    class Traits = std::char_traits<CharT>
>
class basic_string_view;

//
// basic_string_view:
//

template
<
    class CharT,
    class Traits /* = std::char_traits<CharT> */
>
class basic_string_view
{
public:
    // Member types:

    typedef Traits traits_type;
    typedef CharT  value_type;

    typedef CharT       * pointer;
    typedef CharT const * const_pointer;
    typedef CharT       & reference;
    typedef CharT const & const_reference;

    typedef const_pointer iterator;
    typedef const_pointer const_iterator;
    typedef std::reverse_iterator< const_iterator > reverse_iterator;
    typedef	std::reverse_iterator< const_iterator > const_reverse_iterator;

    typedef std::size_t     size_type;
    typedef std::ptrdiff_t  difference_type;

    // 24.4.2.1 Construction and assignment:

    nssv_constexpr basic_string_view() nssv_noexcept
        : data_( nssv_nullptr )
        , size_( 0 )
    {}

#if nssv_CPP11_OR_GREATER
    nssv_constexpr basic_string_view( basic_string_view const & other ) nssv_noexcept = default;
#else
    nssv_constexpr basic_string_view( basic_string_view const & other ) nssv_noexcept
        : data_( other.data_)
        , size_( other.size_)
    {}
#endif

    nssv_constexpr basic_string_view( CharT const * s, size_type count ) nssv_noexcept // non-standard noexcept
        : data_( s )
        , size_( count )
    {}

    nssv_constexpr basic_string_view( CharT const * s) nssv_noexcept // non-standard noexcept
        : data_( s )
#if nssv_CPP17_OR_GREATER
        , size_( Traits::length(s) )
#elif nssv_CPP11_OR_GREATER
        , size_( detail::length(s) )
#else
        , size_( Traits::length(s) )
#endif
    {}

    // Assignment:

#if nssv_CPP11_OR_GREATER
    nssv_constexpr14 basic_string_view & operator=( basic_string_view const & other ) nssv_noexcept = default;
#else
    nssv_constexpr14 basic_string_view & operator=( basic_string_view const & other ) nssv_noexcept
    {
        data_ = other.data_;
        size_ = other.size_;
        return *this;
    }
#endif

    // 24.4.2.2 Iterator support:

    nssv_constexpr const_iterator begin()  const nssv_noexcept { return data_;         }
    nssv_constexpr const_iterator end()    const nssv_noexcept { return data_ + size_; }

    nssv_constexpr const_iterator cbegin() const nssv_noexcept { return begin(); }
    nssv_constexpr const_iterator cend()   const nssv_noexcept { return end();   }

    nssv_constexpr const_reverse_iterator rbegin()  const nssv_noexcept { return const_reverse_iterator( end() );   }
    nssv_constexpr const_reverse_iterator rend()    const nssv_noexcept { return const_reverse_iterator( begin() ); }

    nssv_constexpr const_reverse_iterator crbegin() const nssv_noexcept { return rbegin(); }
    nssv_constexpr const_reverse_iterator crend()   const nssv_noexcept { return rend();   }

    // 24.4.2.3 Capacity:

    nssv_constexpr size_type size()     const nssv_noexcept { return size_; }
    nssv_constexpr size_type length()   const nssv_noexcept { return size_; }
    nssv_constexpr size_type max_size() const nssv_noexcept { return (std::numeric_limits< size_type >::max)(); }

    // since C++20
    nssv_nodiscard nssv_constexpr bool empty() const nssv_noexcept
    {
        return 0 == size_;
    }

    // 24.4.2.4 Element access:

    nssv_constexpr const_reference operator[]( size_type pos ) const
    {
        return data_at( pos );
    }

    nssv_constexpr14 const_reference at( size_type pos ) const
    {
#if nssv_CONFIG_NO_EXCEPTIONS
        assert( pos < size() );
#else
        if ( pos >= size() )
        {
            throw std::out_of_range("nonstd::string_view::at()");
        }
#endif
        return data_at( pos );
    }

    nssv_constexpr const_reference front() const { return data_at( 0 );          }
    nssv_constexpr const_reference back()  const { return data_at( size() - 1 ); }

    nssv_constexpr const_pointer   data()  const nssv_noexcept { return data_; }

    // 24.4.2.5 Modifiers:

    nssv_constexpr14 void remove_prefix( size_type n )
    {
        assert( n <= size() );
        data_ += n;
        size_ -= n;
    }

    nssv_constexpr14 void remove_suffix( size_type n )
    {
        assert( n <= size() );
        size_ -= n;
    }

    nssv_constexpr14 void swap( basic_string_view & other ) nssv_noexcept
    {
        const basic_string_view tmp(other);
        other = *this;
        *this = tmp;
    }

    // 24.4.2.6 String operations:

    size_type copy( CharT * dest, size_type n, size_type pos = 0 ) const
    {
#if nssv_CONFIG_NO_EXCEPTIONS
        assert( pos <= size() );
#else
        if ( pos > size() )
        {
            throw std::out_of_range("nonstd::string_view::copy()");
        }
#endif
        const size_type rlen = (std::min)( n, size() - pos );

        (void) Traits::copy( dest, data() + pos, rlen );

        return rlen;
    }

    nssv_constexpr14 basic_string_view substr( size_type pos = 0, size_type n = npos ) const
    {
#if nssv_CONFIG_NO_EXCEPTIONS
        assert( pos <= size() );
#else
        if ( pos > size() )
        {
            throw std::out_of_range("nonstd::string_view::substr()");
        }
#endif
        return basic_string_view( data() + pos, (std::min)( n, size() - pos ) );
    }

    // compare(), 6x:

    nssv_constexpr14 int compare( basic_string_view other ) const nssv_noexcept // (1)
    {
#if nssv_CPP17_OR_GREATER
        if ( const int result = Traits::compare( data(), other.data(), (std::min)( size(), other.size() ) ) )
#else
        if ( const int result = detail::compare( data(), other.data(), (std::min)( size(), other.size() ) ) )
#endif
        {
            return result;
        }

        return size() == other.size() ? 0 : size() < other.size() ? -1 : 1;
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, basic_string_view other ) const // (2)
    {
        return substr( pos1, n1 ).compare( other );
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, basic_string_view other, size_type pos2, size_type n2 ) const // (3)
    {
        return substr( pos1, n1 ).compare( other.substr( pos2, n2 ) );
    }

    nssv_constexpr int compare( CharT const * s ) const // (4)
    {
        return compare( basic_string_view( s ) );
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, CharT const * s ) const // (5)
    {
        return substr( pos1, n1 ).compare( basic_string_view( s ) );
    }

    nssv_constexpr int compare( size_type pos1, size_type n1, CharT const * s, size_type n2 ) const // (6)
    {
        return substr( pos1, n1 ).compare( basic_string_view( s, n2 ) );
    }

    // 24.4.2.7 Searching:

    // starts_with(), 3x, since C++20:

    nssv_constexpr bool starts_with( basic_string_view v ) const nssv_noexcept  // (1)
    {
        return size() >= v.size() && compare( 0, v.size(), v ) == 0;
    }

    nssv_constexpr bool starts_with( CharT c ) const nssv_noexcept  // (2)
    {
        return starts_with( basic_string_view( &c, 1 ) );
    }

    nssv_constexpr bool starts_with( CharT const * s ) const  // (3)
    {
        return starts_with( basic_string_view( s ) );
    }

    // ends_with(), 3x, since C++20:

    nssv_constexpr bool ends_with( basic_string_view v ) const nssv_noexcept  // (1)
    {
        return size() >= v.size() && compare( size() - v.size(), npos, v ) == 0;
    }

    nssv_constexpr bool ends_with( CharT c ) const nssv_noexcept  // (2)
    {
        return ends_with( basic_string_view( &c, 1 ) );
    }

    nssv_constexpr bool ends_with( CharT const * s ) const  // (3)
    {
        return ends_with( basic_string_view( s ) );
    }

    // find(), 4x:

    nssv_constexpr14 size_type find( basic_string_view v, size_type pos = 0 ) const nssv_noexcept  // (1)
    {
        return assert( v.size() == 0 || v.data() != nssv_nullptr )
            , pos >= size()
            ? npos
            : to_pos( std::search( cbegin() + pos, cend(), v.cbegin(), v.cend(), Traits::eq ) );
    }

    nssv_constexpr14 size_type find( CharT c, size_type pos = 0 ) const nssv_noexcept  // (2)
    {
        return find( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr14 size_type find( CharT const * s, size_type pos, size_type n ) const  // (3)
    {
        return find( basic_string_view( s, n ), pos );
    }

    nssv_constexpr14 size_type find( CharT const * s, size_type pos = 0 ) const  // (4)
    {
        return find( basic_string_view( s ), pos );
    }

    // rfind(), 4x:

    nssv_constexpr14 size_type rfind( basic_string_view v, size_type pos = npos ) const nssv_noexcept  // (1)
    {
        if ( size() < v.size() )
        {
            return npos;
        }

        if ( v.empty() )
        {
            return (std::min)( size(), pos );
        }

        const_iterator last   = cbegin() + (std::min)( size() - v.size(), pos ) + v.size();
        const_iterator result = std::find_end( cbegin(), last, v.cbegin(), v.cend(), Traits::eq );

        return result != last ? size_type( result - cbegin() ) : npos;
    }

    nssv_constexpr14 size_type rfind( CharT c, size_type pos = npos ) const nssv_noexcept  // (2)
    {
        return rfind( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr14 size_type rfind( CharT const * s, size_type pos, size_type n ) const  // (3)
    {
        return rfind( basic_string_view( s, n ), pos );
    }

    nssv_constexpr14 size_type rfind( CharT const * s, size_type pos = npos ) const  // (4)
    {
        return rfind( basic_string_view( s ), pos );
    }

    // find_first_of(), 4x:

    nssv_constexpr size_type find_first_of( basic_string_view v, size_type pos = 0 ) const nssv_noexcept  // (1)
    {
        return pos >= size()
            ? npos
            : to_pos( std::find_first_of( cbegin() + pos, cend(), v.cbegin(), v.cend(), Traits::eq ) );
    }

    nssv_constexpr size_type find_first_of( CharT c, size_type pos = 0 ) const nssv_noexcept  // (2)
    {
        return find_first_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_first_of( CharT const * s, size_type pos, size_type n ) const  // (3)
    {
        return find_first_of( basic_string_view( s, n ), pos );
    }

    nssv_constexpr size_type find_first_of(  CharT const * s, size_type pos = 0 ) const  // (4)
    {
        return find_first_of( basic_string_view( s ), pos );
    }

    // find_last_of(), 4x:

    nssv_constexpr size_type find_last_of( basic_string_view v, size_type pos = npos ) const nssv_noexcept  // (1)
    {
        return empty()
            ? npos
            : pos >= size()
            ? find_last_of( v, size() - 1 )
            : to_pos( std::find_first_of( const_reverse_iterator( cbegin() + pos + 1 ), crend(), v.cbegin(), v.cend(), Traits::eq ) );
    }

    nssv_constexpr size_type find_last_of( CharT c, size_type pos = npos ) const nssv_noexcept  // (2)
    {
        return find_last_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_last_of( CharT const * s, size_type pos, size_type count ) const  // (3)
    {
        return find_last_of( basic_string_view( s, count ), pos );
    }

    nssv_constexpr size_type find_last_of( CharT const * s, size_type pos = npos ) const  // (4)
    {
        return find_last_of( basic_string_view( s ), pos );
    }

    // find_first_not_of(), 4x:

    nssv_constexpr size_type find_first_not_of( basic_string_view v, size_type pos = 0 ) const nssv_noexcept  // (1)
    {
        return pos >= size()
            ? npos
            : to_pos( std::find_if( cbegin() + pos, cend(), not_in_view( v ) ) );
    }

    nssv_constexpr size_type find_first_not_of( CharT c, size_type pos = 0 ) const nssv_noexcept  // (2)
    {
        return find_first_not_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_first_not_of( CharT const * s, size_type pos, size_type count ) const  // (3)
    {
        return find_first_not_of( basic_string_view( s, count ), pos );
    }

    nssv_constexpr size_type find_first_not_of( CharT const * s, size_type pos = 0 ) const  // (4)
    {
        return find_first_not_of( basic_string_view( s ), pos );
    }

    // find_last_not_of(), 4x:

    nssv_constexpr size_type find_last_not_of( basic_string_view v, size_type pos = npos ) const nssv_noexcept  // (1)
    {
        return empty()
            ? npos
            : pos >= size()
            ? find_last_not_of( v, size() - 1 )
            : to_pos( std::find_if( const_reverse_iterator( cbegin() + pos + 1 ), crend(), not_in_view( v ) ) );
    }

    nssv_constexpr size_type find_last_not_of( CharT c, size_type pos = npos ) const nssv_noexcept  // (2)
    {
        return find_last_not_of( basic_string_view( &c, 1 ), pos );
    }

    nssv_constexpr size_type find_last_not_of( CharT const * s, size_type pos, size_type count ) const  // (3)
    {
        return find_last_not_of( basic_string_view( s, count ), pos );
    }

    nssv_constexpr size_type find_last_not_of( CharT const * s, size_type pos = npos ) const  // (4)
    {
        return find_last_not_of( basic_string_view( s ), pos );
    }

    // Constants:

#if nssv_CPP17_OR_GREATER
    static nssv_constexpr size_type npos = size_type(-1);
#elif nssv_CPP11_OR_GREATER
    enum : size_type { npos = size_type(-1) };
#else
    enum { npos = size_type(-1) };
#endif

private:
    struct not_in_view
    {
        const basic_string_view v;

        nssv_constexpr explicit not_in_view( basic_string_view v_ ) : v( v_ ) {}

        nssv_constexpr bool operator()( CharT c ) const
        {
            return npos == v.find_first_of( c );
        }
    };

    nssv_constexpr size_type to_pos( const_iterator it ) const
    {
        return it == cend() ? npos : size_type( it - cbegin() );
    }

    nssv_constexpr size_type to_pos( const_reverse_iterator it ) const
    {
        return it == crend() ? npos : size_type( crend() - it - 1 );
    }

    nssv_constexpr const_reference data_at( size_type pos ) const
    {
#if nssv_BETWEEN( nssv_COMPILER_GNUC_VERSION, 1, 500 )
        return data_[pos];
#else
        return assert( pos < size() ), data_[pos];
#endif
    }

private:
    const_pointer data_;
    size_type     size_;

public:
#if nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS

    template< class Allocator >
    basic_string_view( std::basic_string<CharT, Traits, Allocator> const & s ) nssv_noexcept
        : data_( s.data() )
        , size_( s.size() )
    {}

#if nssv_HAVE_EXPLICIT_CONVERSION

    template< class Allocator >
    explicit operator std::basic_string<CharT, Traits, Allocator>() const
    {
        return to_string( Allocator() );
    }

#endif // nssv_HAVE_EXPLICIT_CONVERSION

#if nssv_CPP11_OR_GREATER

    template< class Allocator = std::allocator<CharT> >
    std::basic_string<CharT, Traits, Allocator>
    to_string( Allocator const & a = Allocator() ) const
    {
        return std::basic_string<CharT, Traits, Allocator>( begin(), end(), a );
    }

#else

    std::basic_string<CharT, Traits>
    to_string() const
    {
        return std::basic_string<CharT, Traits>( begin(), end() );
    }

    template< class Allocator >
    std::basic_string<CharT, Traits, Allocator>
    to_string( Allocator const & a ) const
    {
        return std::basic_string<CharT, Traits, Allocator>( begin(), end(), a );
    }

#endif // nssv_CPP11_OR_GREATER

#endif // nssv_CONFIG_CONVERSION_STD_STRING_CLASS_METHODS
};

//
// Non-member functions:
//

// 24.4.3 Non-member comparison functions:
// lexicographically compare two string views (function template):

template< class CharT, class Traits >
nssv_constexpr bool operator== (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class Traits >
nssv_constexpr bool operator!= (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return !( lhs == rhs ); }

template< class CharT, class Traits >
nssv_constexpr bool operator< (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class Traits >
nssv_constexpr bool operator<= (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class Traits >
nssv_constexpr bool operator> (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class Traits >
nssv_constexpr bool operator>= (
    basic_string_view <CharT, Traits> lhs,
    basic_string_view <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0; }

// Let S be basic_string_view<CharT, Traits>, and sv be an instance of S.
// Implementations shall provide sufficient additional overloads marked
// constexpr and noexcept so that an object t with an implicit conversion
// to S can be compared according to Table 67.

#if ! nssv_CPP11_OR_GREATER || nssv_BETWEEN( nssv_COMPILER_MSVC_VERSION, 100, 141 )

// accommodate for older compilers:

// ==

template< class CharT, class Traits>
nssv_constexpr bool operator==(
    basic_string_view<CharT, Traits> lhs,
    CharT const * rhs ) nssv_noexcept
{ return lhs.size() == detail::length( rhs ) && lhs.compare( rhs ) == 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator==(
    CharT const * lhs,
    basic_string_view<CharT, Traits> rhs ) nssv_noexcept
{ return detail::length( lhs ) == rhs.size() && rhs.compare( lhs ) == 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator==(
    basic_string_view<CharT, Traits> lhs,
    std::basic_string<CharT, Traits> rhs ) nssv_noexcept
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator==(
    std::basic_string<CharT, Traits> rhs,
    basic_string_view<CharT, Traits> lhs ) nssv_noexcept
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

// !=

template< class CharT, class Traits>
nssv_constexpr bool operator!=(
    basic_string_view<CharT, Traits> lhs,
    CharT const * rhs ) nssv_noexcept
{ return !( lhs == rhs ); }

template< class CharT, class Traits>
nssv_constexpr bool operator!=(
    CharT const * lhs,
    basic_string_view<CharT, Traits> rhs ) nssv_noexcept
{ return !( lhs == rhs ); }

template< class CharT, class Traits>
nssv_constexpr bool operator!=(
    basic_string_view<CharT, Traits> lhs,
    std::basic_string<CharT, Traits> rhs ) nssv_noexcept
{ return !( lhs == rhs ); }

template< class CharT, class Traits>
nssv_constexpr bool operator!=(
    std::basic_string<CharT, Traits> rhs,
    basic_string_view<CharT, Traits> lhs ) nssv_noexcept
{ return !( lhs == rhs ); }

// <

template< class CharT, class Traits>
nssv_constexpr bool operator<(
    basic_string_view<CharT, Traits> lhs,
    CharT const * rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator<(
    CharT const * lhs,
    basic_string_view<CharT, Traits> rhs ) nssv_noexcept
{ return rhs.compare( lhs ) > 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator<(
    basic_string_view<CharT, Traits> lhs,
    std::basic_string<CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator<(
    std::basic_string<CharT, Traits> rhs,
    basic_string_view<CharT, Traits> lhs ) nssv_noexcept
{ return rhs.compare( lhs ) > 0; }

// <=

template< class CharT, class Traits>
nssv_constexpr bool operator<=(
    basic_string_view<CharT, Traits> lhs,
    CharT const * rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator<=(
    CharT const * lhs,
    basic_string_view<CharT, Traits> rhs ) nssv_noexcept
{ return rhs.compare( lhs ) >= 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator<=(
    basic_string_view<CharT, Traits> lhs,
    std::basic_string<CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator<=(
    std::basic_string<CharT, Traits> rhs,
    basic_string_view<CharT, Traits> lhs ) nssv_noexcept
{ return rhs.compare( lhs ) >= 0; }

// >

template< class CharT, class Traits>
nssv_constexpr bool operator>(
    basic_string_view<CharT, Traits> lhs,
    CharT const * rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator>(
    CharT const * lhs,
    basic_string_view<CharT, Traits> rhs ) nssv_noexcept
{ return rhs.compare( lhs ) < 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator>(
    basic_string_view<CharT, Traits> lhs,
    std::basic_string<CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator>(
    std::basic_string<CharT, Traits> rhs,
    basic_string_view<CharT, Traits> lhs ) nssv_noexcept
{ return rhs.compare( lhs ) < 0; }

// >=

template< class CharT, class Traits>
nssv_constexpr bool operator>=(
    basic_string_view<CharT, Traits> lhs,
    CharT const * rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator>=(
    CharT const * lhs,
    basic_string_view<CharT, Traits> rhs ) nssv_noexcept
{ return rhs.compare( lhs ) <= 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator>=(
    basic_string_view<CharT, Traits> lhs,
    std::basic_string<CharT, Traits> rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0; }

template< class CharT, class Traits>
nssv_constexpr bool operator>=(
    std::basic_string<CharT, Traits> rhs,
    basic_string_view<CharT, Traits> lhs ) nssv_noexcept
{ return rhs.compare( lhs ) <= 0; }

#else // newer compilers:

#define nssv_BASIC_STRING_VIEW_I(T,U)  typename std::decay< basic_string_view<T,U> >::type

#if defined(_MSC_VER)       // issue 40
# define nssv_MSVC_ORDER(x)  , int=x
#else
# define nssv_MSVC_ORDER(x)  /*, int=x*/
#endif

// ==

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator==(
         basic_string_view  <CharT, Traits> lhs,
    nssv_BASIC_STRING_VIEW_I(CharT, Traits) rhs ) nssv_noexcept
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator==(
    nssv_BASIC_STRING_VIEW_I(CharT, Traits) lhs,
         basic_string_view  <CharT, Traits> rhs ) nssv_noexcept
{ return lhs.size() == rhs.size() && lhs.compare( rhs ) == 0; }

// !=

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator!= (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return !( lhs == rhs ); }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator!= (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return !( lhs == rhs ); }

// <

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator< (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator< (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) < 0; }

// <=

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator<= (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator<= (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) <= 0; }

// >

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator> (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator> (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) > 0; }

// >=

template< class CharT, class Traits  nssv_MSVC_ORDER(1) >
nssv_constexpr bool operator>= (
         basic_string_view  < CharT, Traits > lhs,
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0; }

template< class CharT, class Traits  nssv_MSVC_ORDER(2) >
nssv_constexpr bool operator>= (
    nssv_BASIC_STRING_VIEW_I( CharT, Traits ) lhs,
         basic_string_view  < CharT, Traits > rhs ) nssv_noexcept
{ return lhs.compare( rhs ) >= 0; }

#undef nssv_MSVC_ORDER
#undef nssv_BASIC_STRING_VIEW_I

#endif // compiler-dependent approach to comparisons

// 24.4.4 Inserters and extractors:

#if ! nssv_CONFIG_NO_STREAM_INSERTION

namespace detail {

template< class Stream >
void write_padding( Stream & os, std::streamsize n )
{
    for ( std::streamsize i = 0; i < n; ++i )
        os.rdbuf()->sputc( os.fill() );
}

template< class Stream, class View >
Stream & write_to_stream( Stream & os, View const & sv )
{
    typename Stream::sentry sentry( os );

    if ( !os )
        return os;

    const std::streamsize length = static_cast<std::streamsize>( sv.length() );

    // Whether, and how, to pad:
    const bool      pad = ( length < os.width() );
    const bool left_pad = pad && ( os.flags() & std::ios_base::adjustfield ) == std::ios_base::right;

    if ( left_pad )
        write_padding( os, os.width() - length );

    // Write span characters:
    os.rdbuf()->sputn( sv.begin(), length );

    if ( pad && !left_pad )
        write_padding( os, os.width() - length );

    // Reset output stream width:
    os.width( 0 );

    return os;
}

} // namespace detail

template< class CharT, class Traits >
std::basic_ostream<CharT, Traits> &
operator<<(
    std::basic_ostream<CharT, Traits>& os,
    basic_string_view <CharT, Traits> sv )
{
    return detail::write_to_stream( os, sv );
}

#endif // nssv_CONFIG_NO_STREAM_INSERTION

// Several typedefs for common character types are provided:

typedef basic_string_view<char>      string_view;
typedef basic_string_view<wchar_t>   wstring_view;
#if nssv_HAVE_WCHAR16_T
typedef basic_string_view<char16_t>  u16string_view;
typedef basic_string_view<char32_t>  u32string_view;
#endif

}} // namespace nonstd::sv_lite

//
// 24.4.6 Suffix for basic_string_view literals:
//

#if nssv_HAVE_USER_DEFINED_LITERALS

namespace nonstd {
nssv_inline_ns namespace literals {
nssv_inline_ns namespace string_view_literals {

#if nssv_CONFIG_STD_SV_OPERATOR && nssv_HAVE_STD_DEFINED_LITERALS

nssv_constexpr nonstd::sv_lite::string_view operator "" sv( const char* str, size_t len ) nssv_noexcept  // (1)
{
    return nonstd::sv_lite::string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u16string_view operator "" sv( const char16_t* str, size_t len ) nssv_noexcept  // (2)
{
    return nonstd::sv_lite::u16string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u32string_view operator "" sv( const char32_t* str, size_t len ) nssv_noexcept  // (3)
{
    return nonstd::sv_lite::u32string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::wstring_view operator "" sv( const wchar_t* str, size_t len ) nssv_noexcept  // (4)
{
    return nonstd::sv_lite::wstring_view{ str, len };
}

#endif // nssv_CONFIG_STD_SV_OPERATOR && nssv_HAVE_STD_DEFINED_LITERALS

#if nssv_CONFIG_USR_SV_OPERATOR

nssv_constexpr nonstd::sv_lite::string_view operator "" _sv( const char* str, size_t len ) nssv_noexcept  // (1)
{
    return nonstd::sv_lite::string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u16string_view operator "" _sv( const char16_t* str, size_t len ) nssv_noexcept  // (2)
{
    return nonstd::sv_lite::u16string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::u32string_view operator "" _sv( const char32_t* str, size_t len ) nssv_noexcept  // (3)
{
    return nonstd::sv_lite::u32string_view{ str, len };
}

nssv_constexpr nonstd::sv_lite::wstring_view operator "" _sv( const wchar_t* str, size_t len ) nssv_noexcept  // (4)
{
    return nonstd::sv_lite::wstring_view{ str, len };
}

#endif // nssv_CONFIG_USR_SV_OPERATOR

}}} // namespace nonstd::literals::string_view_literals

#endif

//
// Extensions for std::string:
//

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

namespace nonstd {
namespace sv_lite {

// Exclude MSVC 14 (19.00): it yields ambiguous to_string():

#if nssv_CPP11_OR_GREATER && nssv_COMPILER_MSVC_VERSION != 140

template< class CharT, class Traits, class Allocator = std::allocator<CharT> >
std::basic_string<CharT, Traits, Allocator>
to_string( basic_string_view<CharT, Traits> v, Allocator const & a = Allocator() )
{
    return std::basic_string<CharT,Traits, Allocator>( v.begin(), v.end(), a );
}

#else

template< class CharT, class Traits >
std::basic_string<CharT, Traits>
to_string( basic_string_view<CharT, Traits> v )
{
    return std::basic_string<CharT, Traits>( v.begin(), v.end() );
}

template< class CharT, class Traits, class Allocator >
std::basic_string<CharT, Traits, Allocator>
to_string( basic_string_view<CharT, Traits> v, Allocator const & a )
{
    return std::basic_string<CharT, Traits, Allocator>( v.begin(), v.end(), a );
}

#endif // nssv_CPP11_OR_GREATER

template< class CharT, class Traits, class Allocator >
basic_string_view<CharT, Traits>
to_string_view( std::basic_string<CharT, Traits, Allocator> const & s )
{
    return basic_string_view<CharT, Traits>( s.data(), s.size() );
}

}} // namespace nonstd::sv_lite

#endif // nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS

//
// make types and algorithms available in namespace nonstd:
//

namespace nonstd {

using sv_lite::basic_string_view;
using sv_lite::string_view;
using sv_lite::wstring_view;

#if nssv_HAVE_WCHAR16_T
using sv_lite::u16string_view;
#endif
#if nssv_HAVE_WCHAR32_T
using sv_lite::u32string_view;
#endif

// literal "sv"

using sv_lite::operator==;
using sv_lite::operator!=;
using sv_lite::operator<;
using sv_lite::operator<=;
using sv_lite::operator>;
using sv_lite::operator>=;

#if ! nssv_CONFIG_NO_STREAM_INSERTION
using sv_lite::operator<<;
#endif

#if nssv_CONFIG_CONVERSION_STD_STRING_FREE_FUNCTIONS
using sv_lite::to_string;
using sv_lite::to_string_view;
#endif

} // namespace nonstd

// 24.4.5 Hash support (C++11):

// Note: The hash value of a string view object is equal to the hash value of
// the corresponding string object.

#if nssv_HAVE_STD_HASH

#include <functional>

namespace std {

template<>
struct hash< nonstd::string_view >
{
public:
    std::size_t operator()( nonstd::string_view v ) const nssv_noexcept
    {
        return std::hash<std::string>()( std::string( v.data(), v.size() ) );
    }
};

template<>
struct hash< nonstd::wstring_view >
{
public:
    std::size_t operator()( nonstd::wstring_view v ) const nssv_noexcept
    {
        return std::hash<std::wstring>()( std::wstring( v.data(), v.size() ) );
    }
};

template<>
struct hash< nonstd::u16string_view >
{
public:
    std::size_t operator()( nonstd::u16string_view v ) const nssv_noexcept
    {
        return std::hash<std::u16string>()( std::u16string( v.data(), v.size() ) );
    }
};

template<>
struct hash< nonstd::u32string_view >
{
public:
    std::size_t operator()( nonstd::u32string_view v ) const nssv_noexcept
    {
        return std::hash<std::u32string>()( std::u32string( v.data(), v.size() ) );
    }
};

} // namespace std

#endif // nssv_HAVE_STD_HASH

nssv_RESTORE_WARNINGS()

#endif // nssv_HAVE_STD_STRING_VIEW
#endif // NONSTD_SV_LITE_H_INCLUDED
//!
//! termcolor
//! ~~~~~~~~~
//!
//! termcolor is a header-only c++ library for printing colored messages
//! to the terminal. Written just for fun with a help of the Force.
//!
//! :copyright: (c) 2013 by Ihor Kalnytskyi
//! :license: BSD, see LICENSE for details
//!

#ifndef TERMCOLOR_HPP_
#define TERMCOLOR_HPP_

// the following snippet of code detects the current OS and
// defines the appropriate macro that is used to wrap some
// platform specific things
#if defined(_WIN32) || defined(_WIN64)
#define TERMCOLOR_OS_WINDOWS
#elif defined(__APPLE__)
#define TERMCOLOR_OS_MACOS
#elif defined(__unix__) || defined(__unix)
#define TERMCOLOR_OS_LINUX
#else
#error unsupported platform
#endif

// This headers provides the `isatty()`/`fileno()` functions,
// which are used for testing whether a standart stream refers
// to the terminal. As for Windows, we also need WinApi funcs
// for changing colors attributes of the terminal.
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
#include <unistd.h>
#elif defined(TERMCOLOR_OS_WINDOWS)
#include <io.h>
#include <windows.h>
#endif

#include <cstdio>
#include <iostream>

namespace termcolor {
// Forward declaration of the `_internal` namespace.
// All comments are below.
namespace _internal {
// An index to be used to access a private storage of I/O streams. See
// colorize / nocolorize I/O manipulators for details.
static int colorize_index = std::ios_base::xalloc();

inline FILE *get_standard_stream(const std::ostream &stream);
inline bool is_colorized(std::ostream &stream);
inline bool is_atty(const std::ostream &stream);

#if defined(TERMCOLOR_OS_WINDOWS)
inline void win_change_attributes(std::ostream &stream, int foreground, int background = -1);
#endif
} // namespace _internal

inline std::ostream &colorize(std::ostream &stream) {
  stream.iword(_internal::colorize_index) = 1L;
  return stream;
}

inline std::ostream &nocolorize(std::ostream &stream) {
  stream.iword(_internal::colorize_index) = 0L;
  return stream;
}

inline std::ostream &reset(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[00m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, -1);
#endif
  }
  return stream;
}

inline std::ostream &bold(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[1m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &dark(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[2m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &italic(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[3m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &underline(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[4m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &blink(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[5m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &reverse(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[7m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &concealed(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[8m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &crossed(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[9m";
#elif defined(TERMCOLOR_OS_WINDOWS)
#endif
  }
  return stream;
}

inline std::ostream &grey(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[30m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream,
                                     0 // grey (black)
    );
#endif
  }
  return stream;
}

inline std::ostream &red(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[31m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &green(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[32m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_GREEN);
#endif
  }
  return stream;
}

inline std::ostream &yellow(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[33m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_GREEN | FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &blue(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[34m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE);
#endif
  }
  return stream;
}

inline std::ostream &magenta(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[35m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &cyan(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[36m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN);
#endif
  }
  return stream;
}

inline std::ostream &white(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[37m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_grey(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[40m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1,
                                     0 // grey (black)
    );
#endif
  }
  return stream;
}

inline std::ostream &on_red(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[41m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_green(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[42m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN);
#endif
  }
  return stream;
}

inline std::ostream &on_yellow(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[43m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN | BACKGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_blue(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[44m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_BLUE);
#endif
  }
  return stream;
}

inline std::ostream &on_magenta(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[45m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_BLUE | BACKGROUND_RED);
#endif
  }
  return stream;
}

inline std::ostream &on_cyan(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[46m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1, BACKGROUND_GREEN | BACKGROUND_BLUE);
#endif
  }
  return stream;
}

inline std::ostream &on_white(std::ostream &stream) {
  if (_internal::is_colorized(stream)) {
#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
    stream << "\033[47m";
#elif defined(TERMCOLOR_OS_WINDOWS)
    _internal::win_change_attributes(stream, -1,
                                     BACKGROUND_GREEN | BACKGROUND_BLUE | BACKGROUND_RED);
#endif
  }

  return stream;
}

//! Since C++ hasn't a way to hide something in the header from
//! the outer access, I have to introduce this namespace which
//! is used for internal purpose and should't be access from
//! the user code.
namespace _internal {
//! Since C++ hasn't a true way to extract stream handler
//! from the a given `std::ostream` object, I have to write
//! this kind of hack.
inline FILE *get_standard_stream(const std::ostream &stream) {
  if (&stream == &std::cout)
    return stdout;
  else if ((&stream == &std::cerr) || (&stream == &std::clog))
    return stderr;

  return 0;
}

// Say whether a given stream should be colorized or not. It's always
// true for ATTY streams and may be true for streams marked with
// colorize flag.
inline bool is_colorized(std::ostream &stream) {
  return is_atty(stream) || static_cast<bool>(stream.iword(colorize_index));
}

//! Test whether a given `std::ostream` object refers to
//! a terminal.
inline bool is_atty(const std::ostream &stream) {
  FILE *std_stream = get_standard_stream(stream);

  // Unfortunately, fileno() ends with segmentation fault
  // if invalid file descriptor is passed. So we need to
  // handle this case gracefully and assume it's not a tty
  // if standard stream is not detected, and 0 is returned.
  if (!std_stream)
    return false;

#if defined(TERMCOLOR_OS_MACOS) || defined(TERMCOLOR_OS_LINUX)
  return ::isatty(fileno(std_stream));
#elif defined(TERMCOLOR_OS_WINDOWS)
  return ::_isatty(_fileno(std_stream));
#endif
}

#if defined(TERMCOLOR_OS_WINDOWS)
//! Change Windows Terminal colors attribute. If some
//! parameter is `-1` then attribute won't changed.
inline void win_change_attributes(std::ostream &stream, int foreground, int background) {
  // yeah, i know.. it's ugly, it's windows.
  static WORD defaultAttributes = 0;

  // Windows doesn't have ANSI escape sequences and so we use special
  // API to change Terminal output color. That means we can't
  // manipulate colors by means of "std::stringstream" and hence
  // should do nothing in this case.
  if (!_internal::is_atty(stream))
    return;

  // get terminal handle
  HANDLE hTerminal = INVALID_HANDLE_VALUE;
  if (&stream == &std::cout)
    hTerminal = GetStdHandle(STD_OUTPUT_HANDLE);
  else if (&stream == &std::cerr)
    hTerminal = GetStdHandle(STD_ERROR_HANDLE);

  // save default terminal attributes if it unsaved
  if (!defaultAttributes) {
    CONSOLE_SCREEN_BUFFER_INFO info;
    if (!GetConsoleScreenBufferInfo(hTerminal, &info))
      return;
    defaultAttributes = info.wAttributes;
  }

  // restore all default settings
  if (foreground == -1 && background == -1) {
    SetConsoleTextAttribute(hTerminal, defaultAttributes);
    return;
  }

  // get current settings
  CONSOLE_SCREEN_BUFFER_INFO info;
  if (!GetConsoleScreenBufferInfo(hTerminal, &info))
    return;

  if (foreground != -1) {
    info.wAttributes &= ~(info.wAttributes & 0x0F);
    info.wAttributes |= static_cast<WORD>(foreground);
  }

  if (background != -1) {
    info.wAttributes &= ~(info.wAttributes & 0xF0);
    info.wAttributes |= static_cast<WORD>(background);
  }

  SetConsoleTextAttribute(hTerminal, info.wAttributes);
}
#endif // TERMCOLOR_OS_WINDOWS

} // namespace _internal

} // namespace termcolor

#undef TERMCOLOR_OS_WINDOWS
#undef TERMCOLOR_OS_MACOS
#undef TERMCOLOR_OS_LINUX

#endif // TERMCOLOR_HPP_

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <algorithm>
#include <cstdint>
#include <string>

#include <clocale>
#include <locale>

#include <cstdlib>
// #include <tabulate/termcolor.hpp>
#include <wchar.h>

namespace tabulate {

#if defined(__unix__) || defined(__unix) || defined(__APPLE__)
inline int get_wcswidth(const std::string &string, const std::string &locale,
                        size_t max_column_width) {
  if (string.size() == 0)
    return 0;

  // The behavior of wcswidth() depends on the LC_CTYPE category of the current
  // locale. Set the current locale based on cell properties before computing
  // width
  auto old_locale = std::locale::global(std::locale(locale));

  // Convert from narrow std::string to wide string
  wchar_t *wide_string = new wchar_t[string.size()];
  std::mbstowcs(wide_string, string.c_str(), string.size());

  // Compute display width of wide string
  int result = wcswidth(wide_string, max_column_width);
  delete[] wide_string;

  // Restore old locale
  std::locale::global(old_locale);

  return result;
}
#endif

inline size_t get_sequence_length(const std::string &text, const std::string &locale,
                                  bool is_multi_byte_character_support_enabled) {
  if (!is_multi_byte_character_support_enabled)
    return text.length();

#if defined(_WIN32) || defined(_WIN64)
  (void)locale; // unused parameter
  return (text.length() - std::count_if(text.begin(), text.end(),
                                        [](char c) -> bool { return (c & 0xC0) == 0x80; }));
#elif defined(__unix__) || defined(__unix) || defined(__APPLE__)
  auto result = get_wcswidth(text, locale, text.size());
  if (result >= 0)
    return result;
  else
    return (text.length() - std::count_if(text.begin(), text.end(),
                                          [](char c) -> bool { return (c & 0xC0) == 0x80; }));
#endif
}

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
// #include <tabulate/termcolor.hpp>

namespace tabulate {

enum class Color { none, grey, red, green, yellow, blue, magenta, cyan, white };
}

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

namespace tabulate {

enum class FontAlign { left, right, center };
}

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

namespace tabulate {

enum class FontStyle { bold, dark, italic, underline, blink, reverse, concealed, crossed };
}

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <iostream>
#include <memory>
#include <string>
// #include <tabulate/format.hpp>
/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <algorithm>
#include <cctype>
#include <cstddef>
#include <sstream>
#include <string>
// #include <tabulate/color.hpp>
// #include <tabulate/font_align.hpp>
// #include <tabulate/font_style.hpp>
// #include <tabulate/utf8.hpp>

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
// #include <tabulate/optional_lite.hpp>
using nonstd::optional;
#endif

#include <vector>

namespace tabulate {

class Format {
public:
  Format &width(size_t value) {
    width_ = value;
    return *this;
  }

  Format &height(size_t value) {
    height_ = value;
    return *this;
  }

  Format &padding(size_t value) {
    padding_left_ = value;
    padding_right_ = value;
    padding_top_ = value;
    padding_bottom_ = value;
    return *this;
  }

  Format &padding_left(size_t value) {
    padding_left_ = value;
    return *this;
  }

  Format &padding_right(size_t value) {
    padding_right_ = value;
    return *this;
  }

  Format &padding_top(size_t value) {
    padding_top_ = value;
    return *this;
  }

  Format &padding_bottom(size_t value) {
    padding_bottom_ = value;
    return *this;
  }

  Format &border(const std::string &value) {
    border_left_ = value;
    border_right_ = value;
    border_top_ = value;
    border_bottom_ = value;
    return *this;
  }

  Format &border_color(Color value) {
    border_left_color_ = value;
    border_right_color_ = value;
    border_top_color_ = value;
    border_bottom_color_ = value;
    return *this;
  }

  Format &border_background_color(Color value) {
    border_left_background_color_ = value;
    border_right_background_color_ = value;
    border_top_background_color_ = value;
    border_bottom_background_color_ = value;
    return *this;
  }

  Format &border_left(const std::string &value) {
    border_left_ = value;
    return *this;
  }

  Format &border_left_color(Color value) {
    border_left_color_ = value;
    return *this;
  }

  Format &border_left_background_color(Color value) {
    border_left_background_color_ = value;
    return *this;
  }

  Format &border_right(const std::string &value) {
    border_right_ = value;
    return *this;
  }

  Format &border_right_color(Color value) {
    border_right_color_ = value;
    return *this;
  }

  Format &border_right_background_color(Color value) {
    border_right_background_color_ = value;
    return *this;
  }

  Format &border_top(const std::string &value) {
    border_top_ = value;
    return *this;
  }

  Format &border_top_color(Color value) {
    border_top_color_ = value;
    return *this;
  }

  Format &border_top_background_color(Color value) {
    border_top_background_color_ = value;
    return *this;
  }

  Format &border_bottom(const std::string &value) {
    border_bottom_ = value;
    return *this;
  }

  Format &border_bottom_color(Color value) {
    border_bottom_color_ = value;
    return *this;
  }

  Format &border_bottom_background_color(Color value) {
    border_bottom_background_color_ = value;
    return *this;
  }

  Format &show_border() {
    show_border_top_ = true;
    show_border_bottom_ = true;
    show_border_left_ = true;
    show_border_right_ = true;
    return *this;
  }

  Format &hide_border() {
    show_border_top_ = false;
    show_border_bottom_ = false;
    show_border_left_ = false;
    show_border_right_ = false;
    return *this;
  }

  Format &show_border_top() {
    show_border_top_ = true;
    return *this;
  }

  Format &hide_border_top() {
    show_border_top_ = false;
    return *this;
  }

  Format &show_border_bottom() {
    show_border_bottom_ = true;
    return *this;
  }

  Format &hide_border_bottom() {
    show_border_bottom_ = false;
    return *this;
  }

  Format &show_border_left() {
    show_border_left_ = true;
    return *this;
  }

  Format &hide_border_left() {
    show_border_left_ = false;
    return *this;
  }

  Format &show_border_right() {
    show_border_right_ = true;
    return *this;
  }

  Format &hide_border_right() {
    show_border_right_ = false;
    return *this;
  }

  Format &corner(const std::string &value) {
    corner_top_left_ = value;
    corner_top_right_ = value;
    corner_bottom_left_ = value;
    corner_bottom_right_ = value;
    return *this;
  }

  Format &corner_color(Color value) {
    corner_top_left_color_ = value;
    corner_top_right_color_ = value;
    corner_bottom_left_color_ = value;
    corner_bottom_right_color_ = value;
    return *this;
  }

  Format &corner_background_color(Color value) {
    corner_top_left_background_color_ = value;
    corner_top_right_background_color_ = value;
    corner_bottom_left_background_color_ = value;
    corner_bottom_right_background_color_ = value;
    return *this;
  }

  Format &corner_top_left(const std::string &value) {
    corner_top_left_ = value;
    return *this;
  }

  Format &corner_top_left_color(Color value) {
    corner_top_left_color_ = value;
    return *this;
  }

  Format &corner_top_left_background_color(Color value) {
    corner_top_left_background_color_ = value;
    return *this;
  }

  Format &corner_top_right(const std::string &value) {
    corner_top_right_ = value;
    return *this;
  }

  Format &corner_top_right_color(Color value) {
    corner_top_right_color_ = value;
    return *this;
  }

  Format &corner_top_right_background_color(Color value) {
    corner_top_right_background_color_ = value;
    return *this;
  }

  Format &corner_bottom_left(const std::string &value) {
    corner_bottom_left_ = value;
    return *this;
  }

  Format &corner_bottom_left_color(Color value) {
    corner_bottom_left_color_ = value;
    return *this;
  }

  Format &corner_bottom_left_background_color(Color value) {
    corner_bottom_left_background_color_ = value;
    return *this;
  }

  Format &corner_bottom_right(const std::string &value) {
    corner_bottom_right_ = value;
    return *this;
  }

  Format &corner_bottom_right_color(Color value) {
    corner_bottom_right_color_ = value;
    return *this;
  }

  Format &corner_bottom_right_background_color(Color value) {
    corner_bottom_right_background_color_ = value;
    return *this;
  }

  Format &column_separator(const std::string &value) {
    column_separator_ = value;
    return *this;
  }

  Format &column_separator_color(Color value) {
    column_separator_color_ = value;
    return *this;
  }

  Format &column_separator_background_color(Color value) {
    column_separator_background_color_ = value;
    return *this;
  }

  Format &font_align(FontAlign value) {
    font_align_ = value;
    return *this;
  }

  Format &font_style(const std::vector<FontStyle> &style) {
    if (font_style_.has_value()) {
      for (auto &s : style)
        font_style_->push_back(s);
    } else {
      font_style_ = style;
    }
    return *this;
  }

  Format &font_color(Color value) {
    font_color_ = value;
    return *this;
  }

  Format &font_background_color(Color value) {
    font_background_color_ = value;
    return *this;
  }

  Format &color(Color value) {
    font_color(value);
    border_color(value);
    corner_color(value);
    return *this;
  }

  Format &background_color(Color value) {
    font_background_color(value);
    border_background_color(value);
    corner_background_color(value);
    return *this;
  }

  Format &multi_byte_characters(bool value) {
    multi_byte_characters_ = value;
    return *this;
  }

  Format &locale(const std::string &value) {
    locale_ = value;
    return *this;
  }

  // Apply word wrap
  // Given an input string and a line length, this will insert \n
  // in strategic places in input string and apply word wrapping
  static std::string word_wrap(const std::string &str, size_t width, const std::string &locale,
                               bool is_multi_byte_character_support_enabled) {
    std::vector<std::string> words = explode_string(str, {" ", "-", "\t"});
    size_t current_line_length = 0;
    std::string result;

    for (size_t i = 0; i < words.size(); ++i) {
      std::string word = words[i];
      // If adding the new word to the current line would be too long,
      // then put it on a new line (and split it up if it's too long).
      if (current_line_length +
              get_sequence_length(word, locale, is_multi_byte_character_support_enabled) >
          width) {
        // Only move down to a new line if we have text on the current line.
        // Avoids situation where wrapped whitespace causes emptylines in text.
        if (current_line_length > 0) {
          result += '\n';
          current_line_length = 0;
        }

        // If the current word is too long to fit on a line even on it's own
        // then split the word up.
        while (get_sequence_length(word, locale, is_multi_byte_character_support_enabled) > width) {
          result += word.substr(0, width - 1) + "-";
          word = word.substr(width - 1);
          result += '\n';
        }

        // Remove leading whitespace from the word so the new line starts flush
        // to the left.
        word = trim_left(word);
      }
      result += word;
      current_line_length +=
          get_sequence_length(word, locale, is_multi_byte_character_support_enabled);
    }
    return result;
  }

  static std::vector<std::string> split_lines(const std::string &text, const std::string &delimiter,
                                              const std::string &locale,
                                              bool is_multi_byte_character_support_enabled) {
    std::vector<std::string> result{};
    std::string input = text;
    size_t pos = 0;
    std::string token;
    while ((pos = input.find(delimiter)) != std::string::npos) {
      token = input.substr(0, pos);
      result.push_back(token);
      input.erase(0, pos + delimiter.length());
    }
    if (get_sequence_length(input, locale, is_multi_byte_character_support_enabled))
      result.push_back(input);
    return result;
  };

  // Merge two formats
  // first has higher precedence
  // e.g., first = cell-level formatting and
  // second = row-level formatting
  // Result has attributes of both with cell-level
  // formatting taking precedence
  static Format merge(Format first, Format second) {
    Format result;

    // Width and height
    if (first.width_.has_value())
      result.width_ = first.width_;
    else
      result.width_ = second.width_;

    if (first.height_.has_value())
      result.height_ = first.height_;
    else
      result.height_ = second.height_;

    // Font styling
    if (first.font_align_.has_value())
      result.font_align_ = first.font_align_;
    else
      result.font_align_ = second.font_align_;

    if (first.font_style_.has_value()) {
      // Merge font styles using std::set_union
      std::vector<FontStyle> merged_font_style(first.font_style_->size() +
                                               second.font_style_->size());
#if defined(_WIN32) || defined(_WIN64)
      // Fixes error in Windows - Sequence not ordered
      std::sort(first.font_style_->begin(), first.font_style_->end());
      std::sort(second.font_style_->begin(), second.font_style_->end());
#endif
      std::set_union(first.font_style_->begin(), first.font_style_->end(),
                     second.font_style_->begin(), second.font_style_->end(),
                     merged_font_style.begin());
      result.font_style_ = merged_font_style;
    } else
      result.font_style_ = second.font_style_;

    if (first.font_color_.has_value())
      result.font_color_ = first.font_color_;
    else
      result.font_color_ = second.font_color_;

    if (first.font_background_color_.has_value())
      result.font_background_color_ = first.font_background_color_;
    else
      result.font_background_color_ = second.font_background_color_;

    // Padding
    if (first.padding_left_.has_value())
      result.padding_left_ = first.padding_left_;
    else
      result.padding_left_ = second.padding_left_;

    if (first.padding_top_.has_value())
      result.padding_top_ = first.padding_top_;
    else
      result.padding_top_ = second.padding_top_;

    if (first.padding_right_.has_value())
      result.padding_right_ = first.padding_right_;
    else
      result.padding_right_ = second.padding_right_;

    if (first.padding_bottom_.has_value())
      result.padding_bottom_ = first.padding_bottom_;
    else
      result.padding_bottom_ = second.padding_bottom_;

    // Border
    if (first.border_left_.has_value())
      result.border_left_ = first.border_left_;
    else
      result.border_left_ = second.border_left_;

    if (first.border_left_color_.has_value())
      result.border_left_color_ = first.border_left_color_;
    else
      result.border_left_color_ = second.border_left_color_;

    if (first.border_left_background_color_.has_value())
      result.border_left_background_color_ = first.border_left_background_color_;
    else
      result.border_left_background_color_ = second.border_left_background_color_;

    if (first.border_top_.has_value())
      result.border_top_ = first.border_top_;
    else
      result.border_top_ = second.border_top_;

    if (first.border_top_color_.has_value())
      result.border_top_color_ = first.border_top_color_;
    else
      result.border_top_color_ = second.border_top_color_;

    if (first.border_top_background_color_.has_value())
      result.border_top_background_color_ = first.border_top_background_color_;
    else
      result.border_top_background_color_ = second.border_top_background_color_;

    if (first.border_bottom_.has_value())
      result.border_bottom_ = first.border_bottom_;
    else
      result.border_bottom_ = second.border_bottom_;

    if (first.border_bottom_color_.has_value())
      result.border_bottom_color_ = first.border_bottom_color_;
    else
      result.border_bottom_color_ = second.border_bottom_color_;

    if (first.border_bottom_background_color_.has_value())
      result.border_bottom_background_color_ = first.border_bottom_background_color_;
    else
      result.border_bottom_background_color_ = second.border_bottom_background_color_;

    if (first.border_right_.has_value())
      result.border_right_ = first.border_right_;
    else
      result.border_right_ = second.border_right_;

    if (first.border_right_color_.has_value())
      result.border_right_color_ = first.border_right_color_;
    else
      result.border_right_color_ = second.border_right_color_;

    if (first.border_right_background_color_.has_value())
      result.border_right_background_color_ = first.border_right_background_color_;
    else
      result.border_right_background_color_ = second.border_right_background_color_;

    if (first.show_border_top_.has_value())
      result.show_border_top_ = first.show_border_top_;
    else
      result.show_border_top_ = second.show_border_top_;

    if (first.show_border_bottom_.has_value())
      result.show_border_bottom_ = first.show_border_bottom_;
    else
      result.show_border_bottom_ = second.show_border_bottom_;

    if (first.show_border_left_.has_value())
      result.show_border_left_ = first.show_border_left_;
    else
      result.show_border_left_ = second.show_border_left_;

    if (first.show_border_right_.has_value())
      result.show_border_right_ = first.show_border_right_;
    else
      result.show_border_right_ = second.show_border_right_;

    // Corner
    if (first.corner_top_left_.has_value())
      result.corner_top_left_ = first.corner_top_left_;
    else
      result.corner_top_left_ = second.corner_top_left_;

    if (first.corner_top_left_color_.has_value())
      result.corner_top_left_color_ = first.corner_top_left_color_;
    else
      result.corner_top_left_color_ = second.corner_top_left_color_;

    if (first.corner_top_left_background_color_.has_value())
      result.corner_top_left_background_color_ = first.corner_top_left_background_color_;
    else
      result.corner_top_left_background_color_ = second.corner_top_left_background_color_;

    if (first.corner_top_right_.has_value())
      result.corner_top_right_ = first.corner_top_right_;
    else
      result.corner_top_right_ = second.corner_top_right_;

    if (first.corner_top_right_color_.has_value())
      result.corner_top_right_color_ = first.corner_top_right_color_;
    else
      result.corner_top_right_color_ = second.corner_top_right_color_;

    if (first.corner_top_right_background_color_.has_value())
      result.corner_top_right_background_color_ = first.corner_top_right_background_color_;
    else
      result.corner_top_right_background_color_ = second.corner_top_right_background_color_;

    if (first.corner_bottom_left_.has_value())
      result.corner_bottom_left_ = first.corner_bottom_left_;
    else
      result.corner_bottom_left_ = second.corner_bottom_left_;

    if (first.corner_bottom_left_color_.has_value())
      result.corner_bottom_left_color_ = first.corner_bottom_left_color_;
    else
      result.corner_bottom_left_color_ = second.corner_bottom_left_color_;

    if (first.corner_bottom_left_background_color_.has_value())
      result.corner_bottom_left_background_color_ = first.corner_bottom_left_background_color_;
    else
      result.corner_bottom_left_background_color_ = second.corner_bottom_left_background_color_;

    if (first.corner_bottom_right_.has_value())
      result.corner_bottom_right_ = first.corner_bottom_right_;
    else
      result.corner_bottom_right_ = second.corner_bottom_right_;

    if (first.corner_bottom_right_color_.has_value())
      result.corner_bottom_right_color_ = first.corner_bottom_right_color_;
    else
      result.corner_bottom_right_color_ = second.corner_bottom_right_color_;

    if (first.corner_bottom_right_background_color_.has_value())
      result.corner_bottom_right_background_color_ = first.corner_bottom_right_background_color_;
    else
      result.corner_bottom_right_background_color_ = second.corner_bottom_right_background_color_;

    // Column separator
    if (first.column_separator_.has_value())
      result.column_separator_ = first.column_separator_;
    else
      result.column_separator_ = second.column_separator_;

    if (first.column_separator_color_.has_value())
      result.column_separator_color_ = first.column_separator_color_;
    else
      result.column_separator_color_ = second.column_separator_color_;

    if (first.column_separator_background_color_.has_value())
      result.column_separator_background_color_ = first.column_separator_background_color_;
    else
      result.column_separator_background_color_ = second.column_separator_background_color_;

    // Internationlization
    if (first.multi_byte_characters_.has_value())
      result.multi_byte_characters_ = first.multi_byte_characters_;
    else
      result.multi_byte_characters_ = second.multi_byte_characters_;

    if (first.locale_.has_value())
      result.locale_ = first.locale_;
    else
      result.locale_ = second.locale_;

    return result;
  }

private:
  friend class Cell;
  friend class Row;
  friend class Column;
  friend class TableInternal;
  friend class Printer;
  friend class MarkdownExporter;
  friend class LatexExporter;
  friend class AsciiDocExporter;

  void set_defaults() {
    // NOTE: width and height are not set here
    font_align_ = FontAlign::left;
    font_style_ = std::vector<FontStyle>{};
    font_color_ = font_background_color_ = Color::none;
    padding_left_ = padding_right_ = 1;
    padding_top_ = padding_bottom_ = 0;
    border_top_ = border_bottom_ = "-";
    border_left_ = border_right_ = "|";
    show_border_left_ = show_border_right_ = show_border_top_ = show_border_bottom_ = true;
    border_top_color_ = border_top_background_color_ = border_bottom_color_ =
        border_bottom_background_color_ = border_left_color_ = border_left_background_color_ =
            border_right_color_ = border_right_background_color_ = Color::none;
    corner_top_left_ = corner_top_right_ = corner_bottom_left_ = corner_bottom_right_ = "+";
    corner_top_left_color_ = corner_top_left_background_color_ = corner_top_right_color_ =
        corner_top_right_background_color_ = corner_bottom_left_color_ =
            corner_bottom_left_background_color_ = corner_bottom_right_color_ =
                corner_bottom_right_background_color_ = Color::none;
    column_separator_ = "|";
    column_separator_color_ = column_separator_background_color_ = Color::none;
    multi_byte_characters_ = false;
    locale_ = "";
  }

  // Helper methods for word wrapping:

  // trim white spaces from the left end of an input string
  static std::string trim_left(const std::string &input_string) {
    std::string result = input_string;
    result.erase(result.begin(), std::find_if(result.begin(), result.end(),
                                              [](int ch) { return !std::isspace(ch); }));
    return result;
  }

  // trim white spaces from right end of an input string
  static std::string trim_right(const std::string &input_string) {
    std::string result = input_string;
    result.erase(
        std::find_if(result.rbegin(), result.rend(), [](int ch) { return !std::isspace(ch); })
            .base(),
        result.end());
    return result;
  }

  // trim white spaces from either end of an input string
  static std::string trim(const std::string &input_string) {
    return trim_left(trim_right(input_string));
  }

  static size_t index_of_any(const std::string &input, size_t start_index,
                             const std::vector<std::string> &split_characters) {
    std::vector<size_t> indices{};
    for (auto &c : split_characters) {
      auto index = input.find(c, start_index);
      if (index != std::string::npos)
        indices.push_back(index);
    }
    if (indices.size() > 0)
      return *std::min_element(indices.begin(), indices.end());
    else
      return std::string::npos;
  }

  static std::vector<std::string> explode_string(const std::string &input,
                                                 const std::vector<std::string> &split_characters) {
    std::vector<std::string> result{};
    size_t start_index{0};
    while (true) {
      auto index = index_of_any(input, start_index, split_characters);

      if (index == std::string::npos) {
        result.push_back(input.substr(start_index));
        return result;
      }

      std::string word = input.substr(start_index, index - start_index);
      char next_character = input.substr(index, 1)[0];
      // Unlike whitespace, dashes and the like should stick to the word
      // occurring before it.
      if (isspace(next_character)) {
        result.push_back(word);
        result.push_back(std::string(1, next_character));
      } else {
        result.push_back(word + next_character);
      }
      start_index = index + 1;
    }

    return result;
  }

  // Element width and height
  optional<size_t> width_{};
  optional<size_t> height_{};

  // Font styling
  optional<FontAlign> font_align_{};
  optional<std::vector<FontStyle>> font_style_{};
  optional<Color> font_color_{};
  optional<Color> font_background_color_{};

  // Element padding
  optional<size_t> padding_left_{};
  optional<size_t> padding_top_{};
  optional<size_t> padding_right_{};
  optional<size_t> padding_bottom_{};

  // Element border
  optional<bool> show_border_top_{};
  optional<std::string> border_top_{};
  optional<Color> border_top_color_{};
  optional<Color> border_top_background_color_{};

  optional<bool> show_border_bottom_{};
  optional<std::string> border_bottom_{};
  optional<Color> border_bottom_color_{};
  optional<Color> border_bottom_background_color_{};

  optional<bool> show_border_left_{};
  optional<std::string> border_left_{};
  optional<Color> border_left_color_{};
  optional<Color> border_left_background_color_{};

  optional<bool> show_border_right_{};
  optional<std::string> border_right_{};
  optional<Color> border_right_color_{};
  optional<Color> border_right_background_color_{};

  // Element corner
  optional<std::string> corner_top_left_{};
  optional<Color> corner_top_left_color_{};
  optional<Color> corner_top_left_background_color_{};

  optional<std::string> corner_top_right_{};
  optional<Color> corner_top_right_color_{};
  optional<Color> corner_top_right_background_color_{};

  optional<std::string> corner_bottom_left_{};
  optional<Color> corner_bottom_left_color_{};
  optional<Color> corner_bottom_left_background_color_{};

  optional<std::string> corner_bottom_right_{};
  optional<Color> corner_bottom_right_color_{};
  optional<Color> corner_bottom_right_background_color_{};

  // Element column separator
  optional<std::string> column_separator_{};
  optional<Color> column_separator_color_{};
  optional<Color> column_separator_background_color_{};

  // Internationalization
  optional<bool> multi_byte_characters_{};
  optional<std::string> locale_{};
};

} // namespace tabulate

// #include <tabulate/utf8.hpp>

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
// #include <tabulate/optional_lite.hpp>
using nonstd::optional;
#endif

#include <vector>

namespace tabulate {

class Cell {
public:
  explicit Cell(std::shared_ptr<class Row> parent) : parent_(parent) {}

  void set_text(const std::string &text) { data_ = text; }

  const std::string &get_text() { return data_; }

  size_t size() {
    return get_sequence_length(data_, locale(), is_multi_byte_character_support_enabled());
  }

  std::string locale() { return *format().locale_; }

  Format &format();

  bool is_multi_byte_character_support_enabled();

private:
  std::string data_;
  std::weak_ptr<class Row> parent_;
  optional<Format> format_;
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <iostream>
#include <memory>
#include <string>
// #include <tabulate/cell.hpp>

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
// #include <tabulate/optional_lite.hpp>
using nonstd::optional;
#endif

#include <vector>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace tabulate {

class Row {
public:
  explicit Row(std::shared_ptr<class TableInternal> parent) : parent_(parent) {}

  void add_cell(std::shared_ptr<Cell> cell) { cells_.push_back(cell); }

  Cell &operator[](size_t index) { return cell(index); }

  Cell &cell(size_t index) { return *(cells_[index]); }

  std::vector<std::shared_ptr<Cell>> cells() const { return cells_; }

  size_t size() const { return cells_.size(); }

  Format &format();

  class CellIterator {
  public:
    explicit CellIterator(std::vector<std::shared_ptr<Cell>>::iterator ptr) : ptr(ptr) {}

    CellIterator operator++() {
      ++ptr;
      return *this;
    }
    bool operator!=(const CellIterator &other) const { return ptr != other.ptr; }
    Cell &operator*() { return **ptr; }

  private:
    std::vector<std::shared_ptr<Cell>>::iterator ptr;
  };

  auto begin() -> CellIterator { return CellIterator(cells_.begin()); }
  auto end() -> CellIterator { return CellIterator(cells_.end()); }

private:
  friend class Printer;

  // Returns the row height as configured
  // For each cell in the row, check the cell.format.height
  // property and return the largest configured row height
  // This is used to ensure that all cells in a row are
  // aligned when printing the column
  size_t get_configured_height() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      auto cell = cells_[i];
      auto format = cell->format();
      if (format.height_.has_value())
        result = std::max(result, *format.height_);
    }
    return result;
  }

  // Computes the height of the row based on cell contents
  // and configured cell padding
  // For each cell, compute:
  //   padding_top + (cell_contents / column height) + padding_bottom
  // and return the largest value
  //
  // This is useful when no cell.format.height is configured
  // Call get_configured_height()
  // - If this returns 0, then use get_computed_height()
  size_t get_computed_height(const std::vector<size_t> &column_widths) {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      result = std::max(result, get_cell_height(i, column_widths[i]));
    }
    return result;
  }

  // Returns padding_top + cell_contents / column_height + padding_bottom
  // for a given cell in the column
  // e.g.,
  // column width = 5
  // cell_contents = "I love tabulate" (size/length = 15)
  // padding top and padding bottom are 1
  // then, cell height = 1 + (15 / 5) + 1 = 1 + 3 + 1 = 5
  // The cell will look like this:
  //
  // .....
  // I lov
  // e tab
  // ulate
  // .....
  size_t get_cell_height(size_t cell_index, size_t column_width) {
    size_t result{0};
    Cell &cell = *(cells_[cell_index]);
    auto format = cell.format();
    auto text = cell.get_text();

    auto padding_left = *format.padding_left_;
    auto padding_right = *format.padding_right_;

    result += *format.padding_top_;

    if (column_width > (padding_left + padding_right)) {
      column_width -= (padding_left + padding_right);
    }

    // Check if input text has embedded newline characters
    auto newlines_in_text = std::count(text.begin(), text.end(), '\n');
    std::string word_wrapped_text;
    if (newlines_in_text == 0) {
      // No new lines in input
      // Apply automatic word wrapping and compute row height
      word_wrapped_text = Format::word_wrap(text, column_width, cell.locale(),
                                            cell.is_multi_byte_character_support_enabled());
    } else {
      // There are embedded '\n' characters
      // Respect these characters
      word_wrapped_text = text;
    }

    auto newlines_in_wrapped_text =
        std::count(word_wrapped_text.begin(), word_wrapped_text.end(), '\n');
    auto estimated_row_height = newlines_in_wrapped_text;

    if (!word_wrapped_text.empty() &&
        word_wrapped_text[word_wrapped_text.size() - 1] != '\n') // text doesn't end with a newline
      estimated_row_height += 1;

    result += estimated_row_height;

    result += *format.padding_bottom_;

    return result;
  }

  std::vector<std::shared_ptr<Cell>> cells_;
  std::weak_ptr<class TableInternal> parent_;
  optional<Format> format_;
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once

namespace tabulate {

class ColumnFormat : public Format {
public:
  explicit ColumnFormat(class Column &column) : column_(column) {}

  ColumnFormat &width(size_t value);
  ColumnFormat &height(size_t value);

  // Padding
  ColumnFormat &padding(size_t value);
  ColumnFormat &padding_left(size_t value);
  ColumnFormat &padding_right(size_t value);
  ColumnFormat &padding_top(size_t value);
  ColumnFormat &padding_bottom(size_t value);

  // Border
  ColumnFormat &border(const std::string &value);
  ColumnFormat &border_color(Color value);
  ColumnFormat &border_background_color(Color value);
  ColumnFormat &border_left(const std::string &value);
  ColumnFormat &border_left_color(Color value);
  ColumnFormat &border_left_background_color(Color value);
  ColumnFormat &border_right(const std::string &value);
  ColumnFormat &border_right_color(Color value);
  ColumnFormat &border_right_background_color(Color value);
  ColumnFormat &border_top(const std::string &value);
  ColumnFormat &border_top_color(Color value);
  ColumnFormat &border_top_background_color(Color value);
  ColumnFormat &border_bottom(const std::string &value);
  ColumnFormat &border_bottom_color(Color value);
  ColumnFormat &border_bottom_background_color(Color value);

  // Corner
  ColumnFormat &corner(const std::string &value);
  ColumnFormat &corner_color(Color value);
  ColumnFormat &corner_background_color(Color value);

  // Column separator
  ColumnFormat &column_separator(const std::string &value);
  ColumnFormat &column_separator_color(Color value);
  ColumnFormat &column_separator_background_color(Color value);

  // Font styling
  ColumnFormat &font_align(FontAlign value);
  ColumnFormat &font_style(const std::vector<FontStyle> &style);
  ColumnFormat &font_color(Color value);
  ColumnFormat &font_background_color(Color value);
  ColumnFormat &color(Color value);
  ColumnFormat &background_color(Color value);

  // Locale
  ColumnFormat &multi_byte_characters(bool value);
  ColumnFormat &locale(const std::string &value);

private:
  std::reference_wrapper<class Column> column_;
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <algorithm>
#include <functional>
#include <iostream>
#include <memory>

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
// #include <tabulate/optional_lite.hpp>
using nonstd::optional;
#endif

#include <string>
// #include <tabulate/cell.hpp>
// #include <tabulate/column_format.hpp>
#include <vector>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace tabulate {

class Column {
public:
  explicit Column(std::shared_ptr<class TableInternal> parent) : parent_(parent) {}

  void add_cell(Cell &cell) { cells_.push_back(cell); }

  Cell &operator[](size_t index) { return cells_[index]; }

  std::vector<std::reference_wrapper<Cell>> cells() const { return cells_; }

  size_t size() const { return cells_.size(); }

  ColumnFormat format() { return ColumnFormat(*this); }

  class CellIterator {
  public:
    explicit CellIterator(std::vector<std::reference_wrapper<Cell>>::iterator ptr) : ptr(ptr) {}

    CellIterator operator++() {
      ++ptr;
      return *this;
    }
    bool operator!=(const CellIterator &other) const { return ptr != other.ptr; }
    Cell &operator*() { return *ptr; }

  private:
    std::vector<std::reference_wrapper<Cell>>::iterator ptr;
  };

  auto begin() -> CellIterator { return CellIterator(cells_.begin()); }
  auto end() -> CellIterator { return CellIterator(cells_.end()); }

private:
  friend class ColumnFormat;
  friend class Printer;

  // Returns the column width as configured
  // For each cell in the column, check the cell.format.width
  // property and return the largest configured column width
  // This is used to ensure that all cells in a column are
  // aligned when printing the column
  size_t get_configured_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      auto cell = cells_[i];
      auto format = cell.get().format();
      if (format.width_.has_value())
        result = std::max(result, *format.width_);
    }
    return result;
  }

  // Computes the width of the column based on cell contents
  // and configured cell padding
  // For each cell, compute padding_left + cell_contents + padding_right
  // and return the largest value
  //
  // This is useful when no cell.format.width is configured
  // Call get_configured_width()
  // - If this returns 0, then use get_computed_width()
  size_t get_computed_width() {
    size_t result{0};
    for (size_t i = 0; i < size(); ++i) {
      result = std::max(result, get_cell_width(i));
    }
    return result;
  }

  // Returns padding_left + cell_contents.size() + padding_right
  // for a given cell in the column
  size_t get_cell_width(size_t cell_index) {
    size_t result{0};
    Cell &cell = cells_[cell_index].get();
    auto format = cell.format();
    if (format.padding_left_.has_value())
      result += *format.padding_left_;

    // Check if input text has newlines
    auto text = cell.get_text();
    auto split_lines = Format::split_lines(text, "\n", cell.locale(),
                                           cell.is_multi_byte_character_support_enabled());

    // If there are no newlines in input, set column_width = text.size()
    if (split_lines.size() == 1) {
      result += cell.size();
    } else {
      // There are newlines in input
      // Find widest substring in input and use this as column_width
      size_t widest_sub_string_size{0};
      for (auto &line : split_lines)
        if (get_sequence_length(line, cell.locale(),
                                cell.is_multi_byte_character_support_enabled()) >
            widest_sub_string_size)
          widest_sub_string_size = get_sequence_length(
              line, cell.locale(), cell.is_multi_byte_character_support_enabled());
      result += widest_sub_string_size;
    }

    if (format.padding_right_.has_value())
      result += *format.padding_right_;

    return result;
  }

  std::vector<std::reference_wrapper<Cell>> cells_;
  std::weak_ptr<class TableInternal> parent_;
};

inline ColumnFormat &ColumnFormat::width(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().width(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::height(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().height(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::padding(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::padding_left(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_left(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::padding_right(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_right(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::padding_top(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_top(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::padding_bottom(size_t value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().padding_bottom(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_left(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_left(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_left_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_left_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_left_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_left_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_right(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_right(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_right_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_right_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_right_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_right_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_top(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_top(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_top_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_top_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_top_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_top_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_bottom(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_bottom(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_bottom_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_bottom_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::border_bottom_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().border_bottom_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::corner(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().corner(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::corner_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().corner_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::corner_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().corner_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::column_separator(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().column_separator(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::column_separator_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().column_separator_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::column_separator_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().column_separator_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::font_align(FontAlign value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_align(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::font_style(const std::vector<FontStyle> &style) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_style(style);
  return *this;
}

inline ColumnFormat &ColumnFormat::font_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::font_background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().font_background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::background_color(Color value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().background_color(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::multi_byte_characters(bool value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().multi_byte_characters(value);
  return *this;
}

inline ColumnFormat &ColumnFormat::locale(const std::string &value) {
  for (auto &cell : column_.get().cells_)
    cell.get().format().locale(value);
  return *this;
}

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
// #include <tabulate/color.hpp>
// #include <tabulate/font_style.hpp>
#include <utility>
#include <vector>

namespace tabulate {

class Printer {
public:
  static std::pair<std::vector<size_t>, std::vector<size_t>>
  compute_cell_dimensions(TableInternal &table);

  static void print_table(std::ostream &stream, TableInternal &table);

  static void print_row_in_cell(std::ostream &stream, TableInternal &table,
                                const std::pair<size_t, size_t> &index,
                                const std::pair<size_t, size_t> &dimension, size_t num_columns,
                                size_t row_index,
                                const std::vector<std::string> &splitted_cell_text);

  static bool print_cell_border_top(std::ostream &stream, TableInternal &table,
                                    const std::pair<size_t, size_t> &index,
                                    const std::pair<size_t, size_t> &dimension, size_t num_columns);
  static bool print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension,
                                       size_t num_columns);

  static void apply_element_style(std::ostream &stream, Color foreground_color,
                                  Color background_color,
                                  const std::vector<FontStyle> &font_style) {
    apply_foreground_color(stream, foreground_color);
    apply_background_color(stream, background_color);
    for (auto &style : font_style)
      apply_font_style(stream, style);
  }

  static void reset_element_style(std::ostream &stream) { stream << termcolor::reset; }

private:
  static void print_content_left_aligned(std::ostream &stream, const std::string &cell_content,
                                         const Format &format, size_t text_with_padding_size,
                                         size_t column_width) {

    // Apply font style
    apply_element_style(stream, *format.font_color_, *format.font_background_color_,
                        *format.font_style_);
    stream << cell_content;
    // Only apply font_style to the font
    // Not the padding. So calling apply_element_style with font_style = {}
    reset_element_style(stream);
    apply_element_style(stream, *format.font_color_, *format.font_background_color_, {});

    if (text_with_padding_size < column_width) {
      for (size_t j = 0; j < (column_width - text_with_padding_size); ++j) {
        stream << " ";
      }
    }
  }

  static void print_content_center_aligned(std::ostream &stream, const std::string &cell_content,
                                           const Format &format, size_t text_with_padding_size,
                                           size_t column_width) {
    auto num_spaces = column_width - text_with_padding_size;
    if (num_spaces % 2 == 0) {
      // Even spacing on either side
      for (size_t j = 0; j < num_spaces / 2; ++j)
        stream << " ";

      // Apply font style
      apply_element_style(stream, *format.font_color_, *format.font_background_color_,
                          *format.font_style_);
      stream << cell_content;
      // Only apply font_style to the font
      // Not the padding. So calling apply_element_style with font_style = {}
      reset_element_style(stream);
      apply_element_style(stream, *format.font_color_, *format.font_background_color_, {});

      for (size_t j = 0; j < num_spaces / 2; ++j)
        stream << " ";
    } else {
      auto num_spaces_before = num_spaces / 2 + 1;
      for (size_t j = 0; j < num_spaces_before; ++j)
        stream << " ";

      // Apply font style
      apply_element_style(stream, *format.font_color_, *format.font_background_color_,
                          *format.font_style_);
      stream << cell_content;
      // Only apply font_style to the font
      // Not the padding. So calling apply_element_style with font_style = {}
      reset_element_style(stream);
      apply_element_style(stream, *format.font_color_, *format.font_background_color_, {});

      for (size_t j = 0; j < num_spaces - num_spaces_before; ++j)
        stream << " ";
    }
  }

  static void print_content_right_aligned(std::ostream &stream, const std::string &cell_content,
                                          const Format &format, size_t text_with_padding_size,
                                          size_t column_width) {
    if (text_with_padding_size < column_width) {
      for (size_t j = 0; j < (column_width - text_with_padding_size); ++j) {
        stream << " ";
      }
    }

    // Apply font style
    apply_element_style(stream, *format.font_color_, *format.font_background_color_,
                        *format.font_style_);
    stream << cell_content;
    // Only apply font_style to the font
    // Not the padding. So calling apply_element_style with font_style = {}
    reset_element_style(stream);
    apply_element_style(stream, *format.font_color_, *format.font_background_color_, {});
  }

  static void apply_font_style(std::ostream &stream, FontStyle style) {
    switch (style) {
    case FontStyle::bold:
      stream << termcolor::bold;
      break;
    case FontStyle::dark:
      stream << termcolor::dark;
      break;
    case FontStyle::italic:
      stream << termcolor::italic;
      break;
    case FontStyle::underline:
      stream << termcolor::underline;
      break;
    case FontStyle::blink:
      stream << termcolor::blink;
      break;
    case FontStyle::reverse:
      stream << termcolor::reverse;
      break;
    case FontStyle::concealed:
      stream << termcolor::concealed;
      break;
    case FontStyle::crossed:
      stream << termcolor::crossed;
      break;
    default:
      break;
    }
  }

  static void apply_foreground_color(std::ostream &stream, Color foreground_color) {
    switch (foreground_color) {
    case Color::grey:
      stream << termcolor::grey;
      break;
    case Color::red:
      stream << termcolor::red;
      break;
    case Color::green:
      stream << termcolor::green;
      break;
    case Color::yellow:
      stream << termcolor::yellow;
      break;
    case Color::blue:
      stream << termcolor::blue;
      break;
    case Color::magenta:
      stream << termcolor::magenta;
      break;
    case Color::cyan:
      stream << termcolor::cyan;
      break;
    case Color::white:
      stream << termcolor::white;
      break;
    case Color::none:
    default:
      break;
    }
  }

  static void apply_background_color(std::ostream &stream, Color background_color) {
    switch (background_color) {
    case Color::grey:
      stream << termcolor::on_grey;
      break;
    case Color::red:
      stream << termcolor::on_red;
      break;
    case Color::green:
      stream << termcolor::on_green;
      break;
    case Color::yellow:
      stream << termcolor::on_yellow;
      break;
    case Color::blue:
      stream << termcolor::on_blue;
      break;
    case Color::magenta:
      stream << termcolor::on_magenta;
      break;
    case Color::cyan:
      stream << termcolor::on_cyan;
      break;
    case Color::white:
      stream << termcolor::on_white;
      break;
    case Color::none:
    default:
      break;
    }
  }
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <algorithm>
#include <iostream>
#include <string>
// #include <tabulate/column.hpp>
// #include <tabulate/font_style.hpp>
// #include <tabulate/printer.hpp>
// #include <tabulate/row.hpp>
// #include <tabulate/termcolor.hpp>
#include <vector>
#ifdef max
#undef max
#endif
#ifdef min
#undef min
#endif

namespace tabulate {

class TableInternal : public std::enable_shared_from_this<TableInternal> {
public:
  static std::shared_ptr<TableInternal> create() {
    auto result = std::shared_ptr<TableInternal>(new TableInternal());
    result->format_.set_defaults();
    return result;
  }

  void add_row(const std::vector<std::string> &cells) {
    auto row = std::make_shared<Row>(shared_from_this());
    for (auto &c : cells) {
      auto cell = std::make_shared<Cell>(row);
      cell->set_text(c);
      row->add_cell(cell);
    }
    rows_.push_back(row);
  }

  Row &operator[](size_t index) { return *(rows_[index]); }

  const Row &operator[](size_t index) const { return *(rows_[index]); }

  Column column(size_t index) {
    Column column(shared_from_this());
    for (size_t i = 0; i < rows_.size(); ++i) {
      auto row = rows_[i];
      auto &cell = row->cell(index);
      column.add_cell(cell);
    }
    return column;
  }

  size_t size() const { return rows_.size(); }

  std::pair<size_t, size_t> shape() {
    std::pair<size_t, size_t> result{0, 0};
    std::stringstream stream;
    print(stream);
    auto buffer = stream.str();
    auto lines = Format::split_lines(buffer, "\n", "", true);
    if (lines.size()) {
      result = {get_sequence_length(lines[0], "", true), lines.size()};
    }
    return result;
  }

  Format &format() { return format_; }

  void print(std::ostream &stream) { Printer::print_table(stream, *this); }

  size_t estimate_num_columns() const {
    size_t result{0};
    if (size()) {
      auto first_row = operator[](size_t(0));
      result = first_row.size();
    }
    return result;
  }

private:
  friend class Table;
  friend class MarkdownExporter;

  TableInternal() {}
  TableInternal &operator=(const TableInternal &);
  TableInternal(const TableInternal &);

  std::vector<std::shared_ptr<Row>> rows_;
  Format format_;
};

inline Format &Cell::format() {
  std::shared_ptr<Row> parent = parent_.lock();
  if (!format_.has_value()) {   // no cell format
    format_ = parent->format(); // Use parent row format
  } else {
    // Cell has formatting
    // Merge cell formatting with parent row formatting
    format_ = Format::merge(*format_, parent->format());
  }
  return *format_;
}

inline bool Cell::is_multi_byte_character_support_enabled() {
  return (*format().multi_byte_characters_);
}

inline Format &Row::format() {
  std::shared_ptr<TableInternal> parent = parent_.lock();
  if (!format_.has_value()) {   // no row format
    format_ = parent->format(); // Use parent table format
  } else {
    // Row has formatting rules
    // Merge with parent table format
    format_ = Format::merge(*format_, parent->format());
  }
  return *format_;
}

inline std::pair<std::vector<size_t>, std::vector<size_t>>
Printer::compute_cell_dimensions(TableInternal &table) {
  std::pair<std::vector<size_t>, std::vector<size_t>> result;
  size_t num_rows = table.size();
  size_t num_columns = table.estimate_num_columns();

  std::vector<size_t> row_heights, column_widths{};

  for (size_t i = 0; i < num_columns; ++i) {
    Column column = table.column(i);
    size_t configured_width = column.get_configured_width();
    size_t computed_width = column.get_computed_width();
    if (configured_width != 0)
      column_widths.push_back(configured_width);
    else
      column_widths.push_back(computed_width);
  }

  for (size_t i = 0; i < num_rows; ++i) {
    Row row = table[i];
    size_t configured_height = row.get_configured_height();
    size_t computed_height = row.get_computed_height(column_widths);

    // NOTE: Unlike column width, row height is calculated as the max
    // b/w configured height and computed height
    // which means that .width() has higher precedence than .height()
    // when both are configured by the user
    //
    // TODO: Maybe this can be configured?
    // If such a configuration is exposed, i.e., prefer height over width
    // then the logic will be reversed, i.e.,
    // column_widths.push_back(std::max(configured_width, computed_width))
    // and
    // row_height = configured_height if != 0 else computed_height

    row_heights.push_back(std::max(configured_height, computed_height));
  }

  result.first = row_heights;
  result.second = column_widths;

  return result;
}

inline void Printer::print_table(std::ostream &stream, TableInternal &table) {
  size_t num_rows = table.size();
  size_t num_columns = table.estimate_num_columns();
  auto dimensions = compute_cell_dimensions(table);
  auto row_heights = dimensions.first;
  auto column_widths = dimensions.second;
  auto splitted_cells_text = std::vector<std::vector<std::vector<std::string>>>(
      num_rows, std::vector<std::vector<std::string>>(num_columns, std::vector<std::string>{}));

  // Pre-compute the cells' content and split them into lines before actually
  // iterating the cells.
  for (size_t i = 0; i < num_rows; ++i) {
    Row row = table[i];
    for (size_t j = 0; j < num_columns; ++j) {
      Cell cell = row.cell(j);
      const std::string &text = cell.get_text();
      auto padding_left = *cell.format().padding_left_;
      auto padding_right = *cell.format().padding_right_;

      // Check if input text has embedded \n that are to be respected
      bool has_new_line = text.find_first_of('\n') != std::string::npos;

      if (has_new_line) {
        // Respect to the embedded '\n' characters
        splitted_cells_text[i][j] = Format::split_lines(
            text, "\n", cell.locale(), cell.is_multi_byte_character_support_enabled());
      } else {
        // If there are no embedded \n characters, then apply word wrap.
        //
        // Configured column width cannot be lower than (padding_left +
        // padding_right) This is a bad configuration E.g., the user is trying
        // to force the column width to be 5 when padding_left and padding_right
        // are each configured to 3 (padding_left + padding_right) = 6 >
        // column_width
        auto content_width = column_widths[j] > padding_left + padding_right
                                 ? column_widths[j] - padding_left - padding_right
                                 : column_widths[j];
        auto word_wrapped_text = Format::word_wrap(text, content_width, cell.locale(),
                                                   cell.is_multi_byte_character_support_enabled());
        splitted_cells_text[i][j] = Format::split_lines(
            word_wrapped_text, "\n", cell.locale(), cell.is_multi_byte_character_support_enabled());
      }
    }
  }

  // For each row,
  for (size_t i = 0; i < num_rows; ++i) {

    // Print top border
    bool border_top_printed{true};
    for (size_t j = 0; j < num_columns; ++j) {
      border_top_printed &= print_cell_border_top(stream, table, {i, j},
                                                  {row_heights[i], column_widths[j]}, num_columns);
    }
    if (border_top_printed)
      stream << termcolor::reset << "\n";

    // Print row contents with word wrapping
    for (size_t k = 0; k < row_heights[i]; ++k) {
      for (size_t j = 0; j < num_columns; ++j) {
        print_row_in_cell(stream, table, {i, j}, {row_heights[i], column_widths[j]}, num_columns, k,
                          splitted_cells_text[i][j]);
      }
      if (k + 1 < row_heights[i])
        stream << termcolor::reset << "\n";
    }

    if (i + 1 == num_rows) {

      // Check if there is bottom border to print:
      auto bottom_border_needed{true};
      for (size_t j = 0; j < num_columns; ++j) {
        auto cell = table[i][j];
        auto format = cell.format();
        auto corner = *format.corner_bottom_left_;
        auto border_bottom = *format.border_bottom_;
        if (corner == "" && border_bottom == "") {
          bottom_border_needed = false;
          break;
        }
      }

      if (bottom_border_needed)
        stream << termcolor::reset << "\n";
      // Print bottom border for table
      for (size_t j = 0; j < num_columns; ++j) {
        print_cell_border_bottom(stream, table, {i, j}, {row_heights[i], column_widths[j]},
                                 num_columns);
      }
    }
    if (i + 1 < num_rows)
      stream << termcolor::reset << "\n"; // Don't add newline after last row
  }
}

inline void Printer::print_row_in_cell(std::ostream &stream, TableInternal &table,
                                       const std::pair<size_t, size_t> &index,
                                       const std::pair<size_t, size_t> &dimension,
                                       size_t num_columns, size_t row_index,
                                       const std::vector<std::string> &splitted_cell_text) {
  auto column_width = dimension.second;
  auto cell = table[index.first][index.second];
  auto locale = cell.locale();
  auto is_multi_byte_character_support_enabled = cell.is_multi_byte_character_support_enabled();
  auto old_locale = std::locale::global(std::locale(locale));
  auto format = cell.format();
  auto text_height = splitted_cell_text.size();
  auto padding_top = *format.padding_top_;

  if (*format.show_border_left_) {
    apply_element_style(stream, *format.border_left_color_, *format.border_left_background_color_,
                        {});
    stream << *format.border_left_;
    reset_element_style(stream);
  }

  apply_element_style(stream, *format.font_color_, *format.font_background_color_, {});
  if (row_index < padding_top) {
    // Padding top
    stream << std::string(column_width, ' ');
  } else if (row_index >= padding_top && (row_index <= (padding_top + text_height))) {
    // Retrieve padding left and right
    // (column_width - padding_left - padding_right) is the amount of space
    // available for cell text - Use this to word wrap cell contents
    auto padding_left = *format.padding_left_;
    auto padding_right = *format.padding_right_;

    if (row_index - padding_top < text_height) {
      auto line = splitted_cell_text[row_index - padding_top];

      // Print left padding characters
      stream << std::string(padding_left, ' ');

      // Print word-wrapped line
      line = Format::trim(line);
      auto line_with_padding_size =
          get_sequence_length(line, cell.locale(), is_multi_byte_character_support_enabled) +
          padding_left + padding_right;
      switch (*format.font_align_) {
      case FontAlign::left:
        print_content_left_aligned(stream, line, format, line_with_padding_size, column_width);
        break;
      case FontAlign::center:
        print_content_center_aligned(stream, line, format, line_with_padding_size, column_width);
        break;
      case FontAlign::right:
        print_content_right_aligned(stream, line, format, line_with_padding_size, column_width);
        break;
      }

      // Print right padding characters
      stream << std::string(padding_right, ' ');
    } else
      stream << std::string(column_width, ' ');

  } else {
    // Padding bottom
    stream << std::string(column_width, ' ');
  }

  reset_element_style(stream);

  if (index.second + 1 == num_columns) {
    // Print right border after last column
    if (*format.show_border_right_) {
      apply_element_style(stream, *format.border_right_color_,
                          *format.border_right_background_color_, {});
      stream << *format.border_right_;
      reset_element_style(stream);
    }
  }
  std::locale::global(old_locale);
}

inline bool Printer::print_cell_border_top(std::ostream &stream, TableInternal &table,
                                           const std::pair<size_t, size_t> &index,
                                           const std::pair<size_t, size_t> &dimension,
                                           size_t num_columns) {
  auto cell = table[index.first][index.second];
  auto locale = cell.locale();
  auto old_locale = std::locale::global(std::locale(locale));
  auto format = cell.format();
  auto column_width = dimension.second;

  auto corner = *format.corner_top_left_;
  auto corner_color = *format.corner_top_left_color_;
  auto corner_background_color = *format.corner_top_left_background_color_;
  auto border_top = *format.border_top_;

  if ((corner == "" && border_top == "") || !*format.show_border_top_)
    return false;

  apply_element_style(stream, corner_color, corner_background_color, {});
  stream << corner;
  reset_element_style(stream);

  for (size_t i = 0; i < column_width; ++i) {
    apply_element_style(stream, *format.border_top_color_, *format.border_top_background_color_,
                        {});
    stream << border_top;
    reset_element_style(stream);
  }

  if (index.second + 1 == num_columns) {
    // Print corner after last column
    corner = *format.corner_top_right_;
    corner_color = *format.corner_top_right_color_;
    corner_background_color = *format.corner_top_right_background_color_;

    apply_element_style(stream, corner_color, corner_background_color, {});
    stream << corner;
    reset_element_style(stream);
  }
  std::locale::global(old_locale);
  return true;
}

inline bool Printer::print_cell_border_bottom(std::ostream &stream, TableInternal &table,
                                              const std::pair<size_t, size_t> &index,
                                              const std::pair<size_t, size_t> &dimension,
                                              size_t num_columns) {
  auto cell = table[index.first][index.second];
  auto locale = cell.locale();
  auto old_locale = std::locale::global(std::locale(locale));
  auto format = cell.format();
  auto column_width = dimension.second;

  auto corner = *format.corner_bottom_left_;
  auto corner_color = *format.corner_bottom_left_color_;
  auto corner_background_color = *format.corner_bottom_left_background_color_;
  auto border_bottom = *format.border_bottom_;

  if ((corner == "" && border_bottom == "") || !*format.show_border_bottom_)
    return false;

  apply_element_style(stream, corner_color, corner_background_color, {});
  stream << corner;
  reset_element_style(stream);

  for (size_t i = 0; i < column_width; ++i) {
    apply_element_style(stream, *format.border_bottom_color_,
                        *format.border_bottom_background_color_, {});
    stream << border_bottom;
    reset_element_style(stream);
  }

  if (index.second + 1 == num_columns) {
    // Print corner after last column
    corner = *format.corner_bottom_right_;
    corner_color = *format.corner_bottom_right_color_;
    corner_background_color = *format.corner_bottom_right_background_color_;

    apply_element_style(stream, corner_color, corner_background_color, {});
    stream << corner;
    reset_element_style(stream);
  }
  std::locale::global(old_locale);
  return true;
}

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
// #include <tabulate/table_internal.hpp>

#if __cplusplus >= 201703L
#include <string_view>
#include <variant>
using std::get_if;
using std::holds_alternative;
using std::string_view;
using std::variant;
using std::visit;
#else
// #include <tabulate/string_view_lite.hpp>
// #include <tabulate/variant_lite.hpp>
using nonstd::get_if;
using nonstd::holds_alternative;
using nonstd::string_view;
using nonstd::variant;
using nonstd::visit;
#endif

#include <utility>

namespace tabulate {

class Table {
public:
  Table() : table_(TableInternal::create()) {}

  using Row_t = std::vector<variant<std::string, const char *, string_view, Table>>;

  Table &add_row(const Row_t &cells) {

    if (rows_ == 0) {
      // This is the first row added
      // cells.size() is the number of columns
      cols_ = cells.size();
    }

    std::vector<std::string> cell_strings;
    if (cells.size() < cols_) {
      cell_strings.resize(cols_);
      std::fill(cell_strings.begin(), cell_strings.end(), "");
    } else {
      cell_strings.resize(cells.size());
      std::fill(cell_strings.begin(), cell_strings.end(), "");
    }

    for (size_t i = 0; i < cells.size(); ++i) {
      auto cell = cells[i];
      if (holds_alternative<std::string>(cell)) {
        cell_strings[i] = *get_if<std::string>(&cell);
      } else if (holds_alternative<const char *>(cell)) {
        cell_strings[i] = *get_if<const char *>(&cell);
      } else if (holds_alternative<string_view>(cell)) {
        cell_strings[i] = std::string{*get_if<string_view>(&cell)};
      } else {
        auto table = *get_if<Table>(&cell);
        std::stringstream stream;
        table.print(stream);
        cell_strings[i] = stream.str();
      }
    }

    table_->add_row(cell_strings);
    rows_ += 1;
    return *this;
  }

  Row &operator[](size_t index) { return row(index); }

  Row &row(size_t index) { return (*table_)[index]; }

  Column column(size_t index) { return table_->column(index); }

  Format &format() { return table_->format(); }

  void print(std::ostream &stream) { table_->print(stream); }

  std::string str() {
    std::stringstream stream;
    print(stream);
    return stream.str();
  }

  size_t size() const { return table_->size(); }

  std::pair<size_t, size_t> shape() { return table_->shape(); }

  class RowIterator {
  public:
    explicit RowIterator(std::vector<std::shared_ptr<Row>>::iterator ptr) : ptr(ptr) {}

    RowIterator operator++() {
      ++ptr;
      return *this;
    }
    bool operator!=(const RowIterator &other) const { return ptr != other.ptr; }
    Row &operator*() { return **ptr; }

  private:
    std::vector<std::shared_ptr<Row>>::iterator ptr;
  };

  auto begin() -> RowIterator { return RowIterator(table_->rows_.begin()); }
  auto end() -> RowIterator { return RowIterator(table_->rows_.end()); }

private:
  friend class MarkdownExporter;
  friend class LatexExporter;
  friend class AsciiDocExporter;

  friend std::ostream &operator<<(std::ostream &stream, const Table &table);
  size_t rows_{0};
  size_t cols_{0};
  std::shared_ptr<TableInternal> table_;
};

inline std::ostream &operator<<(std::ostream &stream, const Table &table) {
  const_cast<Table &>(table).print(stream);
  return stream;
}

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <string>
// #include <tabulate/table.hpp>

namespace tabulate {

class Exporter {
public:
  virtual std::string dump(Table &table) = 0;
  virtual ~Exporter() {}
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
// #include <tabulate/exporter.hpp>

namespace tabulate {

class MarkdownExporter : public Exporter {
public:
  std::string dump(Table &table) override {
    std::string result{""};
    apply_markdown_format(table);
    result = table.str();
    restore_table_format(table);
    return result;
  }

  virtual ~MarkdownExporter() {}

private:
  void add_alignment_header_row(Table &table) {
    auto &rows = table.table_->rows_;

    if (rows.size() >= 1) {
      auto alignment_row = std::make_shared<Row>(table.table_->shared_from_this());

      // Create alignment header cells
      std::vector<std::string> alignment_cells{};
      for (auto &cell : table[0]) {
        auto format = cell.format();
        if (format.font_align_.value() == FontAlign::left) {
          alignment_cells.push_back(":----");
        } else if (format.font_align_.value() == FontAlign::center) {
          alignment_cells.push_back(":---:");
        } else if (format.font_align_.value() == FontAlign::right) {
          alignment_cells.push_back("----:");
        }
      }

      // Add alignment header cells to alignment row
      for (auto &c : alignment_cells) {
        auto cell = std::make_shared<Cell>(alignment_row);
        cell->format()
            .hide_border_top()
            .hide_border_bottom()
            .border_left("|")
            .border_right("|")
            .column_separator("|")
            .corner("|");
        cell->set_text(c);
        if (c == ":---:")
          cell->format().font_align(FontAlign::center);
        else if (c == "----:")
          cell->format().font_align(FontAlign::right);
        alignment_row->add_cell(cell);
      }

      // Insert alignment header row
      if (rows.size() > 1)
        rows.insert(rows.begin() + 1, alignment_row);
      else
        rows.push_back(alignment_row);
    }
  }

  void remove_alignment_header_row(Table &table) {
    auto &rows = table.table_->rows_;
    table.table_->rows_.erase(rows.begin() + 1);
  }

  void apply_markdown_format(Table &table) {
    // Apply markdown format to cells in each row
    for (auto row : table) {
      for (auto &cell : row) {
        auto format = cell.format();
        formats_.push_back(format);
        cell.format()
            .hide_border_top()
            .hide_border_bottom()
            .border_left("|")
            .border_right("|")
            .column_separator("|")
            .corner("|");
      }
    }
    // Add alignment header row at position 1
    add_alignment_header_row(table);
  }

  void restore_table_format(Table &table) {
    // Remove alignment header row at position 1
    remove_alignment_header_row(table);

    // Restore original formatting for each cell
    size_t format_index{0};
    for (auto row : table) {
      for (auto &cell : row) {
        cell.format() = formats_[format_index];
        format_index += 1;
      }
    }
  }

  std::vector<Format> formats_;
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
// #include <tabulate/exporter.hpp>

#if __cplusplus >= 201703L
#include <optional>
using std::optional;
#else
// #include <tabulate/optional_lite.hpp>
using nonstd::optional;
#endif

namespace tabulate {

class LatexExporter : public Exporter {

  static const char new_line = '\n';

public:
  class ExportOptions {
  public:
    ExportOptions &indentation(std::size_t value) {
      indentation_ = value;
      return *this;
    }

  private:
    friend class LatexExporter;
    optional<size_t> indentation_;
  };

  ExportOptions &configure() { return options_; }

  std::string dump(Table &table) override {
    std::string result{"\\begin{tabular}"};
    result += new_line;

    result += add_alignment_header(table);
    result += new_line;
    const auto rows = table.rows_;
    // iterate content and put text into the table.
    for (size_t i = 0; i < rows; i++) {
      auto &row = table[i];
      // apply row content indentation
      if (options_.indentation_.has_value()) {
        result += std::string(options_.indentation_.value(), ' ');
      }

      for (size_t j = 0; j < row.size(); j++) {

        result += row[j].get_text();

        // check column position, need "\\" at the end of each row
        if (j < row.size() - 1) {
          result += " & ";
        } else {
          result += " \\\\";
        }
      }
      result += new_line;
    }

    result += "\\end{tabular}";
    return result;
  }

  virtual ~LatexExporter() {}

private:
  std::string add_alignment_header(Table &table) {
    std::string result{"{"};

    for (auto &cell : table[0]) {
      auto format = cell.format();
      if (format.font_align_.value() == FontAlign::left) {
        result += 'l';
      } else if (format.font_align_.value() == FontAlign::center) {
        result += 'c';
      } else if (format.font_align_.value() == FontAlign::right) {
        result += 'r';
      }
    }

    result += "}";
    return result;
  }
  ExportOptions options_;
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
#pragma once
#include <algorithm>
#include <optional>
#include <sstream>
#include <string>
// #include <tabulate/exporter.hpp>

namespace tabulate {

class AsciiDocExporter : public Exporter {

  static const char new_line = '\n';

public:
  std::string dump(Table &table) override {
    std::stringstream ss;
    ss << add_alignment_header(table);
    ss << new_line;

    const auto rows = table.rows_;
    // iterate content and put text into the table.
    for (size_t row_index = 0; row_index < rows; row_index++) {
      auto &row = table[row_index];

      for (size_t cell_index = 0; cell_index < row.size(); cell_index++) {
        ss << "|";
        ss << add_formatted_cell(row[cell_index]);
      }
      ss << new_line;
      if (row_index == 0) {
        ss << new_line;
      }
    }

    ss << "|===";
    return ss.str();
  }

  virtual ~AsciiDocExporter() {}

private:
  std::string add_formatted_cell(Cell &cell) const {
    std::stringstream ss;
    auto format = cell.format();
    std::string cell_string = cell.get_text();

    auto font_style = format.font_style_.value();

    bool format_bold = false;
    bool format_italic = false;
    std::for_each(font_style.begin(), font_style.end(), [&](FontStyle &style) {
      if (style == FontStyle::bold) {
        format_bold = true;
      } else if (style == FontStyle::italic) {
        format_italic = true;
      }
    });

    if (format_bold) {
      ss << '*';
    }
    if (format_italic) {
      ss << '_';
    }

    ss << cell_string;
    if (format_italic) {
      ss << '_';
    }
    if (format_bold) {
      ss << '*';
    }
    return ss.str();
  }

  std::string add_alignment_header(Table &table) {
    std::stringstream ss;
    ss << (R"([cols=")");

    size_t column_count = table[0].size();
    size_t column_index = 0;
    for (auto &cell : table[0]) {
      auto format = cell.format();

      if (format.font_align_.value() == FontAlign::left) {
        ss << '<';
      } else if (format.font_align_.value() == FontAlign::center) {
        ss << '^';
      } else if (format.font_align_.value() == FontAlign::right) {
        ss << '>';
      }

      ++column_index;
      if (column_index != column_count) {
        ss << ",";
      }
    }

    ss << R"("])";
    ss << new_line;
    ss << "|===";

    return ss.str();
  }
};

} // namespace tabulate

/*
  __        ___.         .__          __
_/  |______ \_ |__  __ __|  | _____ _/  |_  ____
\   __\__  \ | __ \|  |  \  | \__  \\   __\/ __ \
 |  |  / __ \| \_\ \  |  /  |__/ __ \|  | \  ___/
 |__| (____  /___  /____/|____(____  /__|  \___  >
           \/    \/                \/          \/
Table Maker for Modern C++
https://github.com/p-ranav/tabulate

Licensed under the MIT License <http://opensource.org/licenses/MIT>.
SPDX-License-Identifier: MIT
Copyright (c) 2019 Pranav Srinivas Kumar <pranav.srinivas.kumar@gmail.com>.

Permission is hereby  granted, free of charge, to any  person obtaining a copy
of this software and associated  documentation files (the "Software"), to deal
in the Software  without restriction, including without  limitation the rights
to  use, copy,  modify, merge,  publish, distribute,  sublicense, and/or  sell
copies  of  the Software,  and  to  permit persons  to  whom  the Software  is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE  IS PROVIDED "AS  IS", WITHOUT WARRANTY  OF ANY KIND,  EXPRESS OR
IMPLIED,  INCLUDING BUT  NOT  LIMITED TO  THE  WARRANTIES OF  MERCHANTABILITY,
FITNESS FOR  A PARTICULAR PURPOSE AND  NONINFRINGEMENT. IN NO EVENT  SHALL THE
AUTHORS  OR COPYRIGHT  HOLDERS  BE  LIABLE FOR  ANY  CLAIM,  DAMAGES OR  OTHER
LIABILITY, WHETHER IN AN ACTION OF  CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE  OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef TABULATE_EXPORT_HPP
#define TABULATE_EXPORT_HPP

// #ifdef _WIN32
//     #ifdef TABULATE_STATIC_LIB
//         #define TABULATE_API
//     #else
//         #ifdef TABULATE_EXPORTS
//             #define TABULATE_API __declspec(dllexport)
//         #else
//             #define TABULATE_API __declspec(dllimport)
//         #endif
//     #endif
// #else
//     #define TABULATE_API
// #endif

// Project version
#define TABULATE_VERSION_MAJOR 1
#define TABULATE_VERSION_MINOR 5
#define TABULATE_VERSION_PATCH 0

// Composing the protocol version string from major, and minor
#define TABULATE_CONCATENATE(A, B) TABULATE_CONCATENATE_IMPL(A, B)
#define TABULATE_CONCATENATE_IMPL(A, B) A##B
#define TABULATE_STRINGIFY(a) TABULATE_STRINGIFY_IMPL(a)
#define TABULATE_STRINGIFY_IMPL(a) #a

#endif
