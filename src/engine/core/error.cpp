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

#include "core/error.h"

#include <cstdarg>
#include <cstdio>
#include <cstdlib>

namespace ky {
namespace error_internal {

    static ErrorHandler* error_handler = nullptr;

    void console_error_handler_callback(void*, const char* function, const char* file, int line,
                                        const char* msg, ErrorCode code) {
        FILE* out = stderr;
        if (code == ErrorCode::WARNING) {
            out = stdout;
        }
        fprintf(out, "%s\nFILE: %s:%d:%s:\n%s\n", error::code_to_cstring(code), file, line,
                function, msg);
        fflush(out);
    }

    // TODO: Global mutex is required for multi-threading
    void print_error(const char* function, const char* file, int line, ErrorCode code,
                     const char* fmt, ...) {
        char msg_buf[KY_ERROR_MESSAGE_MAX_SIZE] {};
        va_list args;
        va_start(args, fmt);
        vsprintf(msg_buf, fmt, args);
        va_end(args);

        ErrorHandler* handler = error_handler;
        while (handler != nullptr) {
            handler->callback(handler->user_data, function, file, line, msg_buf, code);
            handler = handler->next;
        }
    }

} // namespace error_internal

namespace error {

    const char* code_to_cstring(ErrorCode code) {
        switch (code) {
            case ErrorCode::ERROR:
                return "ERROR:";
            case ErrorCode::FATAL:
                return "FATAL:";
            case ErrorCode::WARNING:
                return "WARNING:";
            case ErrorCode::SHADER:
                return "SHADER:";
            default:
                return "UNDEFINED:";
        }
    }

    void init() {
        error_internal::error_handler = (ErrorHandler*)std::malloc(sizeof(ErrorHandler));
        error_internal::error_handler->next = nullptr;
        error_internal::error_handler->user_data = nullptr;
        error_internal::error_handler->callback = error_internal::console_error_handler_callback;
        error_internal::error_handler->free_callback = nullptr;
    }

    void shutdown() {
        ErrorHandler* handler = error_internal::error_handler;
        while (handler != nullptr) {
            ErrorHandler* next = handler->next;
            if (handler->user_data) {
                handler->free_callback(handler->user_data);
            }
            std::free(handler);
            handler = next;
        }
    }

    // TODO: Global mutex is required for multi-threading
    void add_error_handler(ErrorHandler* handler) {
        ErrorHandler* last = error_internal::error_handler;
        while (last->next != nullptr) {
            last = last->next;
        }
        last->next = handler;
    }

    // TODO: Global mutex is required for multi-threading
    void remove_error_handler(const ErrorHandler* handler) {
        ErrorHandler* prev = nullptr;
        ErrorHandler* curr = error_internal::error_handler;
        while (curr->next != nullptr) {
            if (curr == handler) {
                break;
            }
            prev = curr;
            curr = curr->next;
        }
        if (prev != nullptr) {
            prev->next = curr->next;
        }
        if (curr->user_data != nullptr) {
            if (curr->free_callback != nullptr) {
                curr->free_callback(curr->user_data);
            }
            std::free(curr->user_data);
        }
        std::free(curr);
    }

} // namespace error
} // namespace ky
