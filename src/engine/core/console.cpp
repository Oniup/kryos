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

#include "core/console.h"
#include <fmt/color.h>

namespace ky {

ConsoleManager* ConsoleManager::_instance_ptr = nullptr;

const char* ConsoleMessage::code_to_string(ConsoleMessage::Code code)
{
    switch (code) {
        case ConsoleMessage::VERBOSE:
            return "VERBOSE";
        case ConsoleMessage::INFO:
            return "INFO";
        case ConsoleMessage::WARNING:
            return "WARNING";
        case ConsoleMessage::ERROR:
            return "ERROR";
        case ConsoleMessage::FATAL:
            return "FATAL";
        default:
            return "INVALID";
    }
}

ConsoleOutput::ConsoleOutput(uint32_t flags)
      : _flags(flags)
{
}

std::string ConsoleOutput::_format_head(const ConsoleMessage& error)
{
    fmt::text_style style;
    if (_flags & CONSOLE_COLOR_BIT) {
        switch (error.code) {
            case ConsoleMessage::VERBOSE:
                break;
            case ConsoleMessage::INFO:
                style = fmt::fg(fmt::color::sky_blue);
                break;
            case ConsoleMessage::WARNING:
                style = fmt::emphasis::italic | fmt::fg(fmt::color::yellow);
                break;
            case ConsoleMessage::ERROR:
                style =
                    fmt::emphasis::italic | fmt::emphasis::bold | fmt::fg(fmt::color::orange_red);
                break;
            case ConsoleMessage::FATAL:
                style = fmt::emphasis::italic | fmt::fg(fmt::color::white) |
                        fmt::bg(fmt::color::dark_red);
                break;
            default:
                break;
        }
    }

    if (error.context != nullptr) {
        return fmt::format(style, "{} {}", error.context,
                           ConsoleMessage::code_to_string(error.code));
    } else {
        return fmt::format(style, "{}", ConsoleMessage::code_to_string(error.code));
    }
}

std::string ConsoleOutput::_format_body(const ConsoleMessage& error)
{
    std::string body;
    if (_flags & CONSOLE_BREAK_AFTER_HEADER) {
        body.append("\n");
    }
    if (error.code > ConsoleMessage::INFO) {
        if (_flags & ~CONSOLE_FILTER_FILE_BIT) {
            body.append(fmt::format(" file={}", error.file));
        }
        if (_flags & ~CONSOLE_FILTER_LINE_BIT) {
            body.append(fmt::format(" line={}", error.line));
        }
        if (_flags & ~CONSOLE_FILTER_FUNCTION_BIT) {
            body.append(fmt::format(" func={}", error.function));
        }
    }
    body.append(
        fmt::format(":{}{}", (_flags & CONSOLE_BREAK_AFTER_INFO) ? "\n" : " ", error.message));
    return body;
}

void ConsoleManager::print_info(const char* function, const char* file, int line,
                                const std::string& msg, const char* context)
{
    _instance_ptr->_print_output_impl(ConsoleMessage {
        .line = line,
        .message = msg,
        .file = file,
        .function = function,
        .context = context,
        .code = ConsoleMessage::INFO,
    });
}

void ConsoleManager::print_verbose(const char* function, const char* file, int line,
                                   const std::string& msg, const char* context)
{
    _instance_ptr->_print_output_impl(ConsoleMessage {
        .line = line,
        .message = msg,
        .file = file,
        .function = function,
        .context = context,
        .code = ConsoleMessage::VERBOSE,
    });
}

void ConsoleManager::print_warning(const char* function, const char* file, int line,
                                   const std::string& msg, const char* context)
{
    _instance_ptr->_print_output_impl(ConsoleMessage {
        .line = line,
        .message = msg,
        .file = file,
        .function = function,
        .context = context,
        .code = ConsoleMessage::WARNING,
    });
}

void ConsoleManager::print_error(const char* function, const char* file, int line,
                                 const std::string& msg, const char* context)
{
    _instance_ptr->_print_output_impl(ConsoleMessage {
        .line = line,
        .message = msg,
        .file = file,
        .function = function,
        .context = context,
        .code = ConsoleMessage::ERROR,
    });
}

void ConsoleManager::print_fatal(const char* function, const char* file, int line,
                                 const std::string& msg, const char* context)
{
    _instance_ptr->_print_output_impl(ConsoleMessage {
        .line = line,
        .message = msg,
        .file = file,
        .function = function,
        .context = context,
        .code = ConsoleMessage::FATAL,
    });
}

ConsoleManager::ConsoleManager()
{
    _instance_ptr = this;
}

ConsoleManager::ConsoleManager(std::vector<ConsoleOutput*>&& outputs)
      : _outputs(std::move(outputs))
{
    _instance_ptr = this;
}

void ConsoleManager::shutdown()
{
    for (ConsoleOutput* output : _outputs) {
        delete output;
    }
    _instance_ptr = nullptr;
}

void ConsoleManager::add_error_output(ConsoleOutput* output)
{
    _outputs.push_back(output);
}

void ConsoleManager::remove_error_output(const std::string_view& name)
{
    for (size_t i = 0; i < _outputs.size(); i++) {
        if (_outputs[i]->name() == name) {
            _outputs.erase(_outputs.begin() + i);
            return;
        }
    }
}

void ConsoleManager::_print_output_impl(const ConsoleMessage& error)
{
    for (ConsoleOutput* output : _outputs) {
        output->print_output(error);
    }
}

ConsoleTerminalOutput::ConsoleTerminalOutput(uint32_t flags)
      : ConsoleOutput(flags)
{
}

void ConsoleTerminalOutput::print_output(const ConsoleMessage& error)
{
    FILE* out = stdout;
    if (error.code > ConsoleMessage::WARNING) {
        out = stderr;
    }
    fmt::println(out, "{}{}", _format_head(error), _format_body(error));
    if (_flags & CONSOLE_FLUSH_PER_MSG_BIT) {
        std::fflush(out);
    }
}

} // namespace ky
