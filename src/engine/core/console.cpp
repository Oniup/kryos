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

const char* ConsoleMessage::severity_to_string(ConsoleMessage::Severity severity)
{
    switch (severity) {
        case ConsoleMessage::TRACE:
            return "TRACE";
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

std::string ConsoleOutput::_format_head(const ConsoleMessage& msg)
{
    fmt::text_style style;
    if (_flags & CONSOLE_COLOR_BIT) {
        switch (msg.severity) {
            case ConsoleMessage::TRACE:
                style = fmt::fg(fmt::color::dim_gray);
                break;
            case ConsoleMessage::VERBOSE:
                style = fmt::fg(fmt::color::dim_gray);
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

    if (msg.context != nullptr) {
        return fmt::format(style, "{} [{}]", ConsoleMessage::severity_to_string(msg.severity),
                           msg.context);
    } else {
        return fmt::format(style, "{}", ConsoleMessage::severity_to_string(msg.severity));
    }
}

std::string ConsoleOutput::_format_body(const ConsoleMessage& msg)
{
    std::string body;
    if (_flags & CONSOLE_BREAK_AFTER_HEADER) {
        body.append("\n");
    }
    if (msg.severity > ConsoleMessage::INFO) {
        if (_flags & ~CONSOLE_FILTER_FILE_BIT) {
            body.append(fmt::format(" file={}", msg.file));
        }
        if (_flags & ~CONSOLE_FILTER_LINE_BIT) {
            body.append(fmt::format(" line={}", msg.line));
        }
        if (_flags & ~CONSOLE_FILTER_FUNCTION_BIT) {
            body.append(fmt::format(" func={}", msg.function));
        }
    }
    body.append(
        fmt::format(":{}{}", (_flags & CONSOLE_BREAK_AFTER_INFO) ? "\n" : " ", msg.message));
    return body;
}

ConsoleManager::ConsoleManager()
{
    _instance_ptr = this;
}

ConsoleManager::ConsoleManager(std::vector<ConsoleOutput*>&& outputs, int severity_flags)
      : _outputs(std::move(outputs)), _severity_flags(severity_flags)
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

void ConsoleManager::print_to_outputs(int line, const std::string& msg, const char* file,
                                      const char* function, const char* context,
                                      ConsoleMessage::Severity severity)
{
    ConsoleMessage message {
        .line = line,
        .message = msg,
        .file = file,
        .function = function,
        .context = context,
        .severity = severity,
    };

    for (ConsoleOutput* output : _instance_ptr->_outputs) {
        if (_instance_ptr->_severity_flags & severity) {
            output->print_output(message);
        }
    }
}

ConsoleTerminalOutput::ConsoleTerminalOutput(uint32_t flags)
      : ConsoleOutput(flags)
{
}

void ConsoleTerminalOutput::print_output(const ConsoleMessage& msg)
{
    FILE* out = stdout;
    if (msg.severity > ConsoleMessage::WARNING) {
        out = stderr;
    }
    fmt::println(out, "{}{}", _format_head(msg), _format_body(msg));
    if (_flags & CONSOLE_FLUSH_PER_MSG_BIT) {
        std::fflush(out);
    }
}

} // namespace ky
