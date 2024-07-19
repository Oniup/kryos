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
// ------------------------------------------------------------------------------------------------
#define KY_INTERNAL_MSG(_context, _severity, ...)                                      \
    ky::ConsoleManager::print_to_outputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__, \
                                         KY_FUNCTION_STR, (_context),                  \
                                         ky::ConsoleMessage::_severity)

#define KY_INTERNAL_MSG_RETURN(_context, _returning, _severity, ...)                   \
    ky::ConsoleManager::print_to_outputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__, \
                                         KY_FUNCTION_STR, (_context),                  \
                                         ky::ConsoleMessage::_severity);               \
    return (_returning)

#ifndef NDEBUG
#    define KY_INTERNAL_FATAL_MSG(_context, _severity, ...)                                \
        ky::ConsoleManager::print_to_outputs(__LINE__, fmt::format(__VA_ARGS__), __FILE__, \
                                             KY_FUNCTION_STR, (_context),                  \
                                             ky::ConsoleMessage::_severity);               \
        KY_INTERNAL_GENERATE_TRAP()
#else
#    define KY_INTERNAL_FATAL_MSG(_context, _severity, ...)
#endif

#define KY_INTERNAL_CONDITION(_context, _condition, _severity, ...)                           \
    if (!(_condition)) {                                                                      \
        ky::ConsoleManager::print_to_outputs(                                                 \
            __LINE__, fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), \
            __FILE__, KY_FUNCTION_STR, (_context), ky::ConsoleMessage::_severity);            \
        return;                                                                               \
    } else                                                                                    \
        ((void)0)

#define KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, _severity, ...)        \
    if (!(_condition)) {                                                                      \
        ky::ConsoleManager::print_to_outputs(                                                 \
            __LINE__, fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), \
            __FILE__, KY_FUNCTION_STR, (_context), ky::ConsoleMessage::_severity);            \
        return (_returning);                                                                  \
    } else                                                                                    \
        ((void)0)

#ifndef NDEBUG
#    define KY_INTERNAL_FATAL_CONDITION(_context, _condition, _severity, ...)                     \
        if (!(_condition)) {                                                                      \
            ky::ConsoleManager::print_to_outputs(                                                 \
                __LINE__, fmt::format(fmt::format("`{}` == FALSE: ", #_condition) + __VA_ARGS__), \
                __FILE__, KY_FUNCTION_STR, (_context), ky::ConsoleMessage::_severity);            \
            KY_INTERNAL_GENERATE_TRAP();                                                          \
        } else                                                                                    \
            ((void)0)
#else
#    define KY_INTERNAL_FATAL_CONDITION(_context, _condition, _severity, ...) (_condition)
#endif

// Basic
// ------------------------------------------------------------------------------------------------
#define KY_VERBOSE(...) KY_INTERNAL_MSG(nullptr, VERBOSE, __VA_ARGS__)
#define KY_TRACE(...)   KY_INTERNAL_MSG(nullptr, TRACE, __VA_ARGS__)
#define KY_INFO(...)    KY_INTERNAL_MSG(nullptr, INFO, __VA_ARGS__)
#define KY_WARN(...)    KY_INTERNAL_MSG(nullptr, WARNING, __VA_ARGS__)
#define KY_ERROR(...)   KY_INTERNAL_MSG(nullptr, ERROR, __VA_ARGS__)
#define KY_FATAL(...)   KY_INTERNAL_FATAL_MSG(nullptr, FATAL, __VA_ARGS__)

#define KY_CONTEXT_VERBOSE(_context, ...) KY_INTERNAL_MSG(_context, VERBOSE, __VA_ARGS__)
#define KY_CONTEXT_TRACE(_context, ...)   KY_INTERNAL_MSG(_context, TRACE, __VA_ARGS__)
#define KY_CONTEXT_INFO(_context, ...)    KY_INTERNAL_MSG(_context, INFO, __VA_ARGS__)
#define KY_CONTEXT_WARN(_context, ...)    KY_INTERNAL_MSG(_context, WARNING, __VA_ARGS__)
#define KY_CONTEXT_ERROR(_context, ...)   KY_INTERNAL_MSG(_context, ERROR, __VA_ARGS__)
#define KY_CONTEXT_FATAL(_context, ...)   KY_INTERNAL_FATAL_MSG(_context, FATAL, __VA_ARGS__)

// Return Value
// ------------------------------------------------------------------------------------------------
#define KY_WARN_RETURN(_returning, ...) \
    KY_INTERNAL_MSG_RETURN(nullptr, _returning, WARNING, __VA_ARGS__)
#define KY_ERROR_RETURN(_returning, ...) \
    KY_INTERNAL_MSG_RETURN(nullptr, _returning, ERROR, __VA_ARGS__)

#define KY_CONTEXT_WARN_RETURN(_context, _returning, ...) \
    KY_INTERNAL_MSG_RETURN(_context, _returning, WARNING, __VA_ARGS__)
#define KY_CONTEXT_ERROR_RETURN(_context, _returning, ...) \
    KY_INTERNAL_MSG_RETURN(_context, _returning, ERROR, __VA_ARGS__)

// Conditions
// ------------------------------------------------------------------------------------------------
#define KY_CONDITION_WARN(_condition, ...) \
    KY_INTERNAL_CONDITION(nullptr, _condition, WARNING, __VA_ARGS__);
#define KY_CONDITION_ERROR(_condition, ...) \
    KY_INTERNAL_CONDITION(nullptr, _condition, ERROR, __VA_ARGS__);
#define KY_CONDITION_FATAL(_condition, ...) \
    KY_INTERNAL_FATAL_CONDITION(nullptr, _condition, FATAL, __VA_ARGS__);

#define KY_CONDITION_WARN_RETURN(_condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, WARNING, __VA_ARGS__);
#define KY_CONDITION_ERROR_RETURN(_condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(nullptr, _returning, _condition, ERROR, __VA_ARGS__);

// Context
// ------------------------------------------------------------------------------------------------
#define KY_CONTEXT_CONDITION_WARN(_context, _condition, ...) \
    KY_INTERNAL_CONDITION(_context, _condition, WARNING, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_ERROR(_context, _condition, ...) \
    KY_INTERNAL_CONDITION(_context, _condition, ERROR, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_FATAL(_context, _condition, ...) \
    KY_INTERNAL_FATAL_CONDITION(_context, _condition, FATAL, __VA_ARGS__);

#define KY_CONTEXT_CONDITION_WARN_RETURN(_context, _condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, WARNING, __VA_ARGS__);
#define KY_CONTEXT_CONDITION_ERROR_RETURN(_context, _condition, _returning, ...) \
    KY_INTERNAL_CONDITION_RETURN(_context, _returning, _condition, ERROR, __VA_ARGS__);

// Vulkan
// ------------------------------------------------------------------------------------------------
#define KY_VULKAN_VERBOSE(...) KY_CONTEXT_VERBOSE("VULKAN", __VA_ARGS__)
#define KY_VULKAN_TRACE(...)   KY_CONTEXT_TRACE("VULKAN", __VA_ARGS__)
#define KY_VULKAN_INFO(...)    KY_CONTEXT_INFO("VULKAN", __VA_ARGS__)
#define KY_VULKAN_WARN(...)    KY_CONTEXT_WARN("VULKAN", __VA_ARGS__)
#define KY_VULKAN_ERROR(...)   KY_CONTEXT_ERROR("VULKAN", __VA_ARGS__)
#define KY_VULKAN_FATAL(...)   KY_CONTEXT_ERROR("VULKAN", __VA_ARGS__)

#define KY_VULKAN_WARN_RETURN(_returning, ...) \
    KY_CONTEXT_WARN_RETURN("VULKAN", _returning, __VA_ARGS__)
#define KY_VULKAN_ERROR_RETURN(_returning, ...) \
    KY_CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)
#define KY_VULKAN_FATAL_RETURN(_returning, ...) \
    KY_CONTEXT_ERROR_RETURN("VULKAN", _returning, __VA_ARGS__)

#define KY_VULKAN_CONDITION_WARN(_condition, ...) \
    KY_CONTEXT_CONDITION_WARN("VULKAN", _condition, __VA_ARGS__)
#define KY_VULKAN_CONDITION_ERROR(_condition, ...) \
    KY_CONTEXT_CONDITION_ERROR("VULKAN", _condition, __VA_ARGS__)
#define KY_VULKAN_CONDITION_FATAL(_condition, ...) \
    KY_CONTEXT_CONDITION_FATAL("VULKAN", _condition, __VA_ARGS__)

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
    enum Severity
    {
        INVALID = 0,
        VERBOSE = 1 << 0,
        TRACE = 1 << 1,
        INFO = 1 << 2,
        WARNING = 1 << 3,
        ERROR = 1 << 4,
        FATAL = 1 << 5,
    };

    static const char* severity_to_string(ConsoleMessage::Severity code);

    int line = -1;
    std::string message;
    const char* file = nullptr;
    const char* function = nullptr;
    const char* context = nullptr;
    Severity severity = ConsoleMessage::INVALID;
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
    virtual void print_output(const ConsoleMessage& msg) = 0;

    inline uint32_t flags() const { return _flags; }

protected:
    uint32_t _flags;

    virtual std::string _format_head(const ConsoleMessage& msg);
    virtual std::string _format_body(const ConsoleMessage& msg);
};

class ConsoleManager
{
public:
    static constexpr int DEFAULT_MESSAGING = ky::ConsoleMessage::INFO | ky::ConsoleMessage::ERROR |
                                             ky::ConsoleMessage::WARNING |
                                             ky::ConsoleMessage::FATAL;
    static constexpr int VERBOSE_MESSAGING =
        ky::ConsoleMessage::VERBOSE | ky::ConsoleMessage::TRACE | ky::ConsoleMessage::INFO |
        ky::ConsoleMessage::ERROR | ky::ConsoleMessage::WARNING | ky::ConsoleMessage::FATAL;

    static int filtered_severities() { return _instance_ptr->_severity_flags; }
    static size_t registered_outputs_count() { return _instance_ptr->_outputs.size(); }

    static void print_to_outputs(int line, const std::string& msg, const char* file,
                                 const char* function, const char* context,
                                 ConsoleMessage::Severity severity);

public:
    ConsoleManager();
    ConsoleManager(std::vector<ConsoleOutput*>&& outputs, int severity_flags = DEFAULT_MESSAGING);
    void shutdown();

    void add_error_output(ConsoleOutput* output);
    void remove_error_output(const std::string_view& name);

private:
    static ConsoleManager* _instance_ptr;
    std::vector<ConsoleOutput*> _outputs;
    int _severity_flags;
};

class ConsoleTerminalOutput : public ConsoleOutput
{
public:
    ConsoleTerminalOutput(uint32_t flags = CONSLE_CONSOLE_NONE_BIT);

    inline std::string_view name() const override { return "Error Terminal Output"; }
    void print_output(const ConsoleMessage& msg) override;
};

} // namespace ky

#endif
