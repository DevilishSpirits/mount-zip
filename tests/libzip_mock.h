// Copyright 2021 Google LLC
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef LIBZIP_MOCK_H
#define LIBZIP_MOCK_H

#include <syslog.h>
#include <zip.h>
#include <string>

inline void initTest() {
  // hide almost all messages
  setlogmask(LOG_MASK(LOG_EMERG));
}

// Global state to control libzip mock behavior.
struct LibZipMock {
  struct zip {
    std::string filename;
    zip_int64_t count = 0;
  };

  zip z;

  // Configuration flags
  bool zip_open_fails = false;
  int zip_open_error = 0;

  // Reset to default state
  void Reset() {
    z.filename.clear();
    z.count = 0;
    zip_open_fails = false;
    zip_open_error = 0;
  }
};

extern LibZipMock g_libzip_mock;

#endif  // LIBZIP_MOCK_H
