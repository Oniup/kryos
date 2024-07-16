// This file is part of Kryos Engine (https://github.com/Oniup/kryos-engine)
// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef KRYOS_CORE__COMMON_H
#define KRYOS_CORE__COMMON_H

#include <cstddef>
#include <cstdint>

#if defined(_WIN32) || defined(__CYGWIN__)
#    ifdef KY_WIN_EXPORT
#        ifdef __GNUC__
#            define KYAPI __attribute__((dllexport))
#        else
#            define KYAPI __declspec(dllexport)
#        endif
#    else
#        ifdef __GNUC__
#            define KYAPI __attribute__((dllimport))
#        else
#            define KYAPI __declspec(dllimport)
#        endif
#    endif
#    define NKYAPI
#else
#    if __GNUC__ >= 4
#        define KYAPI  __attribute__((visibility("default")))
#        define NKYAPI __attribute__((visibility("hidden")))
#    else
#        define KYAPI
#        define NKYAPI
#    endif
#endif

#ifdef _MSC_VER
#    define KY_FORCE_INLINE __forceinline
#elif __GNUC__
#    define KY_FORCE_INLINE __attribute__((always_inline)) inline
#else
#    define KY_FORCE_INLINE inline
#endif

#define KY_STR(non_null_term_str) (int) non_null_term_str.size(), non_null_term_str.data()

#endif
