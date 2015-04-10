// Copyright 2001 Google Inc. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS-IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Character Map Class
//
// Originally written by Daniel Dulitz
// Yanked out from url.cc on February 2003 by Wei-Hwa Huang
//
//
// A fast, bit-vector map for 8-bit unsigned characters.
//
// Internally stores 256 bits in an array of 8 uint32s.
// See changelist history for micro-optimization attempts.
// Does quick bit-flicking to lookup needed characters.
//
// This class is useful for non-character purposes as well.
//

#ifndef UTIL_GTL_CHARMAP_H_
#define UTIL_GTL_CHARMAP_H_

#include <string.h>

#include "base/integral_types.h"
#include "base/type_traits.h"

class Charmap {
 public:
  // Initializes with given uint32 values.  For instance, the first
  // variable contains bits for values 0x1F (US) down to 0x00 (NUL).
  Charmap(uint32 b0, uint32 b1, uint32 b2, uint32 b3,
          uint32 b4, uint32 b5, uint32 b6, uint32 b7) {
    m_[0] = b0;
    m_[1] = b1;
    m_[2] = b2;
    m_[3] = b3;
    m_[4] = b4;
    m_[5] = b5;
    m_[6] = b6;
    m_[7] = b7;
  }

  // Initializes with a given char*.  Note that NUL is not treated as
  // a terminator, but rather a char to be flicked.
  Charmap(const char* str, int len) {
    Init(str, len);
  }

  // Initializes with a given char*.  NUL is treated as a terminator
  // and will not be in the charmap.
  explicit Charmap(const char* str) {
    Init(str, strlen(str));
  }

  bool contains(unsigned char c) const {
    return (m_[c >> 5] >> (c & 0x1f)) & 0x1;
  }

  // Returns true if and only if a character exists in both maps.
  bool IntersectsWith(const Charmap & c) const {
    for (int i = 0; i < 8; ++i) {
      if ((m_[i] & c.m_[i]) != 0)
        return true;
    }
    return false;
  }

  bool IsZero() const {
    for (int i = 0; i < 8; ++i) {
      if (m_[i] != 0)
        return false;
    }
    return true;
  }

 protected:
  uint32 m_[8];

  void Init(const char* str, int len) {
    memset(&m_, 0, sizeof m_);
    for (int i = 0; i < len; ++i) {
      unsigned char value = static_cast<unsigned char>(str[i]);
      m_[value >> 5] |= 1UL << (value & 0x1f);
    }
  }
};
DECLARE_POD(Charmap);

#endif  // UTIL_GTL_CHARMAP_H_