/**
 * @file memory.cpp
 *
 * This file is part of the Kryos Engine (See AUTHORS.md)
 * GitHub Repository: https://github.com/Oniup/kryos
 *
 * @copyright
 * Copyright (c) 2023-present Oniup (https://github.com/Oniup/)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 *
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#include "core/memory.hpp"
#include "core/log.hpp"

#include <cstdio>
#include <cstdlib>
#include <new>

namespace kryos {

void* memory::malloc(size_t size)
{
  void* ptr = std::malloc(size);
  KASSERT(ptr, "Failed to malloc system memory for size %zu", size);
  return ptr;
}

void* memory::realloc(void* ptr, size_t size)
{
  if (ptr != nullptr) {
    void* temp = nullptr;
    temp = std::realloc(ptr, size);
    KASSERT(temp, "Failed to realloc system memory for size %zu at address %p",
            size, ptr);
  }
  else {
    ptr = memory::malloc(size);
  }
  return ptr;
}

void memory::free(void* ptr)
{
  KASSERT(
      ptr,
      "Attempted to free pointer that is null or has already been released");
  std::free(ptr);
}

} // namespace kryos
