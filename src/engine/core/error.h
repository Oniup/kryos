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

#ifndef KRYOS_CORE__ERROR_H
#define KRYOS_CORE__ERROR_H

#include "core/common.h"

// Error messages:
// WARNING: These macros work in the opposite way to assert.
//
// Unlike exceptions and assert, these macros try to maintain consistency and stability. In most
// cases, the bugs or individual data are not fatal and should never allow a perfectly running and
// stable program to crash because of it. Use the macros ending in `_MSG` to leave a meaningful
// message.
//
// NOTE: There are still macros provided that crash the program. These macros should be used
// only when absolutely necessary and only for debugging purposes.

#ifdef _MSC_VER
#    define _KY_GENERATE_TRAP() __debugbreak()
#else
#    define _KY_GENERATE_TRAP() __builtin_trap()
#endif

#define KY_FUNCTION_STR __FUNCTION__

#define KY_ERROR_MESSAGE_MAX_SIZE 4096

// FIXME: Rework error handler, structure should be similar to WindowManager. The error handler
// data should be stored within the engine context and just have a static pointer for static access

namespace ky {

enum class ErrorCode {
    ERROR,
    FATAL,
    WARNING,
    SHADER,
};

struct ErrorHandler {
    using Callback = void (*)(void* user_data, const char* function, const char* file, int32 line,
                              const char* msg, ErrorCode code);
    using FreeUserDataCallback = void (*)(void* user_data);

    enum Type {
        CONSOLE,
        EDITOR_CONSOLE,
    };

    ErrorHandler* next {};
    void* user_data {};
    Callback callback {};
    FreeUserDataCallback free_callback {};
};

namespace error {

    const char* code_to_cstring(ErrorCode code);

    // Initializes the error handlers and sets the console handler as default. Use
    // `add_error_handler` to add additional error handlers and `remove_error_handler` to remove
    // existing ones.
    void init();

    // Shuts down the error handlers by deallocating all error handlers.
    void shutdown();

    // Adds an error handler to the list of error handlers.
    void add_error_handler(ErrorHandler* handler);

    // Removes an error handler from the list of error handlers.
    void remove_error_handler(const ErrorHandler* handler);

} // namespace error

namespace error_internal {

    void print_error(const char* function, const char* file, int32 line, ErrorCode code,
                     const char* fmt, ...);

} // namespace error_internal
} // namespace ky

#define KY_ERROR_MSG(...)                                                                  \
    ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                    __VA_ARGS__);

// Ensures an integer index `_index` is less than `_size`. Otherwise, invalid index and the
// function returns.
#define KY_ERROR_FAIL_INDEX(_index, _size)                                                     \
    if ((_index) >= (_size)) {                                                                 \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                        "Index %zu out of bounds %zu", (_index), (_size));     \
        return;                                                                                \
    } else                                                                                     \
        ((void)0)

// Ensures an integer index `_index` is less than `_size`. Otherwise, invalid index and the
// function returns while providing a custom error message.
#define KY_ERROR_FAIL_INDEX_MSG(_index, _size, _msg)                                           \
    if ((_index) >= (_size)) {                                                                 \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                        "Index %zu out of bounds %zu: %s", (_index), (_size),  \
                                        (_msg));                                               \
        return;                                                                                \
    } else                                                                                     \
        ((void)0)

// Ensures an integer index `_index` is less than `_size`. Otherwise, invalid index and the program
// crashes.
#define KY_FATAL_FAIL_INDEX(_index, _size)                                                     \
    if ((_index) >= (_size)) {                                                                 \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::FATAL, \
                                        "Index %zu out of bounds %zu", (_index), (_size));     \
        _KY_GENERATE_TRAP();                                                                   \
    } else                                                                                     \
        ((void)0)

// Ensures an integer index `_index` is less than `_size`. Otherwise, invalid index and the program
// crashes while providing a custom error message.
#define KY_FATAL_FAIL_INDEX_MSG(_index, _size, _msg)                                           \
    if ((_index) >= (_size)) {                                                                 \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::FATAL, \
                                        "Index %zu out of bounds %zu: %s", (_index), (_size),  \
                                        (_msg));                                               \
        _KY_GENERATE_TRAP();                                                                   \
    } else                                                                                     \
        ((void)0)

// Ensures a condition `_condition` is true. Otherwise, invalid condition and the function returns.
#define KY_ERROR_CONDITION_RETURN(_condition, _returning)                                      \
    if (!(_condition)) {                                                                       \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                        "Condition %s is false", #_condition);                 \
        return (_returning);                                                                   \
    } else                                                                                     \
        ((void)0)

#define KY_ERROR_CONDITION(_condition)                                                         \
    if (!(_condition)) {                                                                       \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                        "Condition %s is false", #_condition);                 \
        return;                                                                                \
    } else                                                                                     \
        ((void)0)

// Ensures a condition `_condition` is true. Otherwise, invalid condition and the function returns
// while providing a custom error message.
#define KY_ERROR_CONDITION_MSG_RETURN(_condition, _returning, _msg)                            \
    if (!(_condition)) {                                                                       \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                        "Condition %s is false: %s", #_condition, _msg);       \
        return (_returning);                                                                   \
    } else                                                                                     \
        ((void)0)

// Ensures a condition `_condition` is true. Otherwise, invalid condition and the function returns
// while providing a custom error message.
#define KY_ERROR_CONDITION_MSG(_condition, _msg)                                               \
    if (!(_condition)) {                                                                       \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::ERROR, \
                                        "Condition %s is false: %s", #_condition, _msg);       \
        return;                                                                                \
    } else                                                                                     \
        ((void)0)

// Ensures a condition `_condition` is true. Otherwise, invalid condition and the program crashes.
#define KY_FATAL_CONDITION(_condition)                                                         \
    if (!(_condition)) {                                                                       \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::FATAL, \
                                        "Condition %s is false", #_condition);                 \
        _KY_GENERATE_TRAP();                                                                   \
    } else                                                                                     \
        ((void)0)

// Ensures a condition `_condition` is true. Otherwise, invalid condition and the program crashes
// while providing a custom error message.
#define KY_FATAL_CONDITION_MSG(_condition, _msg)                                               \
    if (!(_condition)) {                                                                       \
        ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::FATAL, \
                                        "Condition %s is false: %s", #_condition, _msg);       \
        _KY_GENERATE_TRAP();                                                                   \
    } else                                                                                     \
        ((void)0)

// Crashes the program with a custom error message.
#define KY_FATAL_CRASH_PROGRAM(_msg)                                                              \
    ky::error_internal::print_error(KY_FUNCTION_STR, __FILE__, __LINE__, ErrorCode::FATAL, _msg); \
    _KY_GENERATE_TRAP();

#endif
