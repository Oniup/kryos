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

#include <string_view>

// TODO: Implement when command line arguments are requried

namespace ky {

class CommandLineArgs
{
public:
    struct Command
    {
        std::string_view flag;

        inline bool valid() const { return !flag.empty(); }
    };

    CommandLineArgs(int argc, char** argv);

private:
};

} // namespace ky
