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

#ifndef KRYOS_CORE__CONSOLE_H
#define KRYOS_CORE__CONSOLE_H

#ifdef _MSC_VER
#    define KY_INTERNAL_GENERATE_TRAP() __debugbreak()
#else
#    define KY_INTERNAL_GENERATE_TRAP() __builtin_trap()
#endif

#define KY_FUNCTION_STR __FUNCTION__

// Internal
#define KY_INTERNAL_MSG(_context, print_func, ...)                                                \
    ky::ConsoleManager::print_func(KY_FUNCTION_STR, __FILE__, __LINE__, fmt::format(__VA_ARGS__), \
                                   (_context))

#define KY_INTERNAL_MSG_RETURN(_context, _returning, print_func, ...)                             \
    ky::ConsoleManager::print_func(KY_FUNCTION_STR, __FILE__, __LINE__, fmt::format(__VA_ARGS__), \
                                   (_context));                                                   \
    return (_returning)

#define KY_INTERNAL_FATAL_MSG(_context, print_func, ...)                                          \
    ky::ConsoleManager::print_func(KY_FUNCTION_STR, __FILE__, __LINE__, fmt::format(__VA_ARGS__), \
                                   (_context));                                                   \
    KY_INTERNAL_GENERATE_TRAP()

#define KY_INTERNAL_CONDITION(_context, _condition, _print_func, ...)                            \
    if (!(_condition)) {                                                                         \
        ky::ConsoleManager::_print_func(                                                         \
            KY_FUNCTION_STR, __FILE__, __LINE__,                                                 \
            fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), (_context)); \
        return;                                                                                  \
    } else                                                                                       \
        ((void)0)

#define KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, _print_func, ...)         \
    if (!(_condition)) {                                                                         \
        ky::ConsoleManager::_print_func(                                                         \
            KY_FUNCTION_STR, __FILE__, __LINE__,                                                 \
            fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), (_context)); \
        return (_returning);                                                                     \
    } else                                                                                       \
        ((void)0)

#define KY_INTERNAL_FATAL_CONDITION(_context, _condition, _print_func, ...)                      \
    if (!(_condition)) {                                                                         \
        ky::ConsoleManager::_print_func(                                                         \
            KY_FUNCTION_STR, __FILE__, __LINE__,                                                 \
            fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), (_context)); \
        KY_INTERNAL_GENERATE_TRAP();                                                             \
    } else                                                                                       \
        ((void)0)

// Basic
#define KY_INFO(...)    KY_INTERNAL_MSG(nullptr, print_info, __VA_ARGS__)
#define KY_VERBOSE(...) KY_INTERNAL_MSG(nullptr, print_verbose, __VA_ARGS__)
#define KY_WARN(...)    KY_INTERNAL_MSG(nullptr, print_warning, __VA_ARGS__)
#define KY_ERROR(...)   KY_INTERNAL_MSG(nullptr, print_error, __VA_ARGS__)
#define KY_FATAL(...)   KY_INTERNAL_FATAL_MSG(nullptr, print_fatal, __VA_ARGS__)

// Basic Return
#define KY_INFO_RETURN(_returning, ...) \
    KY_INTERNAL_MSG_RETURN(nullptr, _returning, print_info, __VA_ARGS__)
#define KY_VERBOSE_RETURN(_returning, ...) \
    KY_INTERNAL_MSG_RETURN(nullptr, print_verbose, __VA_ARGS__)
#define KY_WARN_RETURN(_returning, ...) \
    KY_INTERNAL_MSG_RETURN(nullptr, _returning, print_warning, __VA_ARGS__)
#define KY_ERROR_RETURN(_returning, ...) \
    KY_INTERNAL_MSG_RETURN(nullptr, _returning, print_error, __VA_ARGS__)

// Context
#define KY_CONTEXT_INFO(_context, ...)    KY_INTERNAL_MSG(_context, print_info, __VA_ARGS__)
#define KY_CONTEXT_VERBOSE(_context, ...) KY_INTERNAL_MSG(_context, print_verbose, __VA_ARGS__)
#define KY_CONTEXT_WARN(_context, ...)    KY_INTERNAL_MSG(_context, print_warning, __VA_ARGS__)
#define KY_CONTEXT_ERROR(_context, ...)   KY_INTERNAL_MSG(_context, print_error, __VA_ARGS__)
#define KY_CONTEXT_FATAL(_context, ...)   KY_INTERNAL_FATAL_MSG(_context, print_fatal, __VA_ARGS__)

// Context Return
#define KY_CONTEXT_INFO_RETURN(_context, _returning, ...) \
    KY_INTERNAL_MSG_RETURN(_context, _returning, print_info, __VA_ARGS__)
#define KY_CONTEXT_VERBOSE_RETURN(_context, _returning, ...) \
    KY_INTERNAL_MSG_RETURN(_context, _returning, print_verbose, __VA_ARGS__)
#define KY_CONTEXT_WARN_RETURN(_context, _returning, ...) \
    KY_INTERNAL_MSG_RETURN(_context, _returning, print_warning, __VA_ARGS__)
#define KY_CONTEXT_ERROR_RETURN(_context, _returning, ...) \
    KY_INTERNAL_MSG_RETURN(_context, _returning, print_error, __VA_ARGS__)

// Condition
#define KY_CONDITION_VERBOSE(_condition, ...) \
    KY_INTERNAL_CONDITION(nullptr, _condition, print_verbose, __VA_ARGS__);
#define KY_CONDITION_INFO(_condition, ...) \
    KY_INTERNAL_CONDITION(nullptr, _condition, print_info, __VA_ARGS__);
#define KY_CONDITION_WARN(_condition, ...) \
    KY_INTERNAL_CONDITION(nullptr, _condition, print_warning, __VA_ARGS__);
#define KY_CONDITION_ERROR(_condition, ...) \
    KY_INTERNAL_CONDITION(nullptr, _condition, print_error, __VA_ARGS__);
#define KY_CONDITION_FATAL(_condition, ...) \
    KY_INTERNAL_FATAL_CONDITION(nullptr, _condition, print_fatal, __VA_ARGS__);

// Condition Return
#define KY_CONDITION_VERBOSE_RETURN(_condition, _returning, ...)                             \
    KY_INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, _returning, print_verbose, \
                                 __VA_ARGS__);
#define KY_CONDITION_INFO_RETURN(_condition, _returning, ...) \
    KY_INTERNAL_CONDITION(nullptr, _returning, _condition, print_info, __VA_ARGS__);
#define KY_CONDITION_WARN_RETURN(_condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, print_warning, __VA_ARGS__);
#define KY_CONDITION_ERROR_RETURN(_condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, print_error, __VA_ARGS__);

// Context Condition
#define KY_CONTEXT_CONDITION_VERBOSE(_context, _condition, ...) \
    KY_INTERNAL_CONDITION(_context, _condition, print_verbose, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_INFO(_context, _condition, ...) \
    KY_INTERNAL_CONDITION(_context, _condition, print_info, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_WARN(_context, _condition, ...) \
    KY_INTERNAL_CONDITION(_context, _condition, print_warning, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_ERROR(_context, _condition, ...) \
    KY_INTERNAL_CONDITION(_context, _condition, print_error, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_FATAL(_context, _condition, ...) \
    KY_INTERNAL_FATAL_CONDITION(_context, _condition, print_fatal, __VA_ARGS__);

// Context Condition Return
#define KY_CONTEXT_CONDITION_VERBOSE_RETURN(_context, _condition, _returning, ...)            \
    KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, _returning, print_verbose, \
                                 __VA_ARGS__);
#define KY_CONTEXT_CONDITION_INFO_RETURN(_context, _condition, _returning, ...) \
    KY_INTERNAL_CONDITION(_context, _returning, _condition, print_info, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_WARN_RETURN(_context, _condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, print_warning, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_ERROR_RETURN(_context, _condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, print_error, __VA_ARGS__);

// Vulkan
#define KY_VULKAN_VERBOSE(...) KY_CONTEXT_VERBOSE("VULKAN", __VA_ARGS__)
#define KY_VULKAN_INFO(...)    KY_CONTEXT_INFO("VULKAN", __VA_ARGS__)
#define KY_VULKAN_WARN(...)    KY_CONTEXT_WARN("VULKAN", __VA_ARGS__)
#define KY_VULKAN_ERROR(...)   KY_CONTEXT_ERROR("VULKAN", __VA_ARGS__)
#define KY_VULKAN_FATAL(...)   KY_CONTEXT_ERROR("VULKAN", __VA_ARGS__)

// Vulkan
#define KY_VULKAN_VERBOSE_RETURN(_returning, ...) \
    KY_CONTEXT_VERBOSE_RETURN("VULKAN", _returning, __VA_ARGS__)
#define KY_VULKAN_INFO_RETURN(_returning, ...) \
    KY_CONTEXT_INFO_RETURN("VULKAN", _returning, __VA_ARGS__)
#define KY_VULKAN_WARN_RETURN(_returning, ...) \
    KY_CONTEXT_WARN_RETURN("VULKAN", _returning, __VA_ARGS__)
#define KY_VULKAN_ERROR_RETURN(_returning, ...) \
    KY_CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)
#define KY_VULKAN_FATAL_RETURN(_returning, ...) \
    KY_CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)

#define KY_VULKAN_CONDITION_INFO(_condition, ...) \
    KY_CONTEXT_CONDITION_INFO("VULKAN", _condition, __VA_ARGS__)
#define KY_VULKAN_CONDITION_VERBOSE(_condition, ...) \
    KY_CONTEXT_CONDITION_VERBOSE("VULKAN", _condition, __VA_ARGS__)
#define KY_VULKAN_CONDITION_WARN(_condition, ...) \
    KY_CONTEXT_CONDITION_WARN("VULKAN", _condition, __VA_ARGS__)
#define KY_VULKAN_CONDITION_ERROR(_condition, ...) \
    KY_CONTEXT_CONDITION_ERROR("VULKAN", _condition, __VA_ARGS__)
#define KY_VULKAN_CONDITION_FATAL(_condition, ...) \
    KY_CONTEXT_CONDITION_FATAL("VULKAN", _condition, __VA_ARGS__)

#define KY_VULKAN_CONDITION_INFO_RETURN(_condition, _returning, ...) \
    KY_CONTEXT_CONDITION_INFO_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)
#define KY_VULKAN_CONDITION_VERBOSE_RETURN(_condition, _returning, ...) \
    KY_CONTEXT_CONDITION_VERBOSE_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)
#define KY_VULKAN_CONDITION_WARN_RETURN(_condition, _returning, ...) \
    KY_CONTEXT_CONDITION_WARN_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)
#define KY_VULKAN_CONDITION_ERROR_RETURN(_condition, _returning, ...) \
    KY_CONTEXT_CONDITION_ERROR_RETURN("VULKAN", _condition, _returning, __VA_ARGS__)

#include <fmt/format.h>
#include <string>
#include <string_view>
#include <vector>

namespace ky {

struct ConsoleMessage
{
    enum Code
    {
        INVALID = 0,
        VERBOSE = 1 << 0,
        INFO = 1 << 1,
        WARNING = 1 << 2,
        ERROR = 1 << 3,
        FATAL = 1 << 4,
    };

    static const char* code_to_string(ConsoleMessage::Code code);

    int line = -1;
    std::string message;
    const char* file = nullptr;
    const char* function = nullptr;
    const char* context = nullptr;
    Code code = ConsoleMessage::INVALID;
};

enum ConsoleOutputFlags : uint32_t
{
    CONSLE_CONSOLE_NONE_BIT = 0,
    CONSOLE_FLUSH_PER_MSG_BIT = 1 << 0,
    CONSOLE_COLOR_BIT = 1 << 1,

    CONSOLE_BREAK_AFTER_HEADER = 1 << 2,
    CONSOLE_BREAK_AFTER_INFO = 1 << 3,

    CONSOLE_FILTER_FILE_BIT = 1 << 4,
    CONSOLE_FILTER_LINE_BIT = 1 << 5,
    CONSOLE_FILTER_FUNCTION_BIT = 1 << 6,
};

class ConsoleOutput
{
public:
    ConsoleOutput(uint32_t flags);
    virtual ~ConsoleOutput() = default;
    virtual std::string_view name() const = 0;
    virtual void print_output(const ConsoleMessage& error) = 0;

    inline uint32_t flags() const { return _flags; }

protected:
    uint32_t _flags;

    virtual std::string _format_head(const ConsoleMessage& error);
    virtual std::string _format_body(const ConsoleMessage& error);
};

class ConsoleManager
{
public:
    static int filtered_severities() { return _instance_ptr->_filtered_severities; }
    static size_t registered_outputs_count() { return _instance_ptr->_outputs.size(); }

    static void print_info(const char* function, const char* file, int line,
                           const std::string& msg, const char* context = nullptr);
    static void print_verbose(const char* function, const char* file, int line,
                              const std::string& msg, const char* context = nullptr);
    static void print_warning(const char* function, const char* file, int line,
                              const std::string& msg, const char* context = nullptr);
    static void print_error(const char* function, const char* file, int line,
                            const std::string& msg, const char* context = nullptr);
    static void print_fatal(const char* function, const char* file, int line,
                            const std::string& msg, const char* context = nullptr);

public:
    ConsoleManager();
    ConsoleManager(std::vector<ConsoleOutput*>&& outputs);
    void shutdown();

    void add_error_output(ConsoleOutput* output);
    void remove_error_output(const std::string_view& name);

private:
    static ConsoleManager* _instance_ptr;
    std::vector<ConsoleOutput*> _outputs;
    int _filtered_severities;

    void _print_output_impl(const ConsoleMessage& error);
};

class ConsoleTerminalOutput : public ConsoleOutput
{
public:
    ConsoleTerminalOutput(uint32_t flags = CONSLE_CONSOLE_NONE_BIT);

    inline std::string_view name() const override { return "Error Terminal Output"; }
    void print_output(const ConsoleMessage& error) override;
};

} // namespace ky

#endif
