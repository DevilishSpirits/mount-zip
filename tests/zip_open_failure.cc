// Copyright 2021 Google LLC
// Copyright 2010-2021 Alexander Galanin <al@galanin.nnov.ru>
// http://galanin.nnov.ru/~al
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

#include "libzip_mock.h"

#include <gtest/gtest.h>
#include <stdlib.h>
#include <zip.h>
#include <string>

#include "tree.h"

namespace {

// test functions

TEST(ZipOpenFailureTest, Basic) {
  initTest();
  g_libzip_mock.Reset();
  g_libzip_mock.zip_open_fails = true;

  try {
    Tree t("test.zip");
    FAIL() << "Should have thrown an exception";
  } catch (const std::exception& e) {
    EXPECT_EQ(std::string(e.what()),
              "Cannot open ZIP archive 'test.zip': Expected error");
  }
}

}  // namespace
