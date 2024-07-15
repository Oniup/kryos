# Contributing Guidelines

Thank you for considering a contribution to our project! Please follow the guidelines below when
contributing code.

## How to Contribute

We welcome contributions in the form of new features, bug fixes, and improvements. To do so,
please follow these steps:

* Fork the repo.
* Follow the code standard stated below.
* Create a pull request for new feature/fixes.

## Code Standards

To ensure high-quality and maintainable code, we have established the following standards:

* All code must be C++17 compilable with Clang, GCC and MSVC.
* Do not use the standard library, use engine types instead unless required (eg.
  `initalizer_list` and `type_traits`).
* Code should be compilable on both UNIX/POSIX and Windows platforms.
* Code with warnings will not be accepted; please address any warnings.
* Whenever possible, include tests for new features using the test framework.
* Write Internal documentation for new features using the following style:
```cpp
// A templated hash function for a given type `_T`.
//
// Template Args:
// - _T: The type for which to compute the hash value.
//
// Detail:
// Provides a hash function for a given type `_T` using the Fowler–Noll–Vo (FNV) non-cryptographic
// hash algorithm.
template <typename _T>
struct Hash
{
    // Computes a hash value for a given value of type `_T`.
    //
    // - val: The value of type `_T` to compute the hash for.
    // Returns: A hash value computed using the FNV algorithm.
    constexpr usize operator()(const _T& val) const;

    // Computes a hash value for a given value of type `_T` with a specified size.
    //
    // - val: The value of type `_T` to compute the hash for.
    // - size: The number of elements in the value.
    // Returns: A hash value computed using the FNV algorithm.
    constexpr usize operator()(const _T& val, usize size) const;
};
```
* Every source file must contain the following license comment at the top:
```cpp
// This file is part of Kryos Engine (https://github.com/Oniup/kryos-engine)
// Copyright (c) 2024 Oniup (https://github.com/Oniup)
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not
// use this file except in compliance with the License. You may obtain a copy of
// the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS, WITHOUT
// WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. See the
// License for the specific language governing permissions and limitations under
// the License.
```

### Code Style

* Provide Doxygen comments with detailed descriptions for all public functions and types unless
  the name is self explanatory.
* Maintain consistent formatting with the project's style, preferably using clang-format.
* Use spaces for indentation instead of tabs.

### Naming Conventions

* Everything should be implemented within the `ky` namespace.
* Use `KY_` prefix for macros.
* Format header guards as `KRYOS__[MOD]_[DIR]__[FILE]_H`.
* Use `.cpp` and `.h` extensions.
* `snake_case` for variables/fields, functions/methods, `PascalCase` for types, type defines, and
  templates parameters. `UPPER_SNAKE_CASE` for macros, constants, and enum values.
* Avoid abbreviating names, especially for functions/methods and class names.
* Any internal/private OOP code variables/types/functions and template parameters require `_` as a
  prefix. Otherwise put inside the `intl` namespace
* Follow the common patterns and styles as the rest of the codebase to be consistent.
