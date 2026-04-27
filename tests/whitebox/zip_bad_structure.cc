// Copyright 2021 Google LLC
// Copyright 2013-2021 Alexander Galanin <al@galanin.nnov.ru>
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
TEST(ZipBadStructureTest, DuplicateFileNames) {
  initTest();
  g_libzip_mock.Reset();
  g_libzip_mock.z.filename = "same_file.name";
  g_libzip_mock.z.count = 2;
  Tree t("dummy.zip");
}

TEST(ZipBadStructureTest, RelativePaths) {
  initTest();
  g_libzip_mock.Reset();
  g_libzip_mock.z.filename = "../file.name";
  g_libzip_mock.z.count = 1;
  Tree t("dummy.zip");
}

TEST(ZipBadStructureTest, AbsolutePaths) {
  initTest();
  g_libzip_mock.Reset();
  g_libzip_mock.z.filename = "/file.name";
  g_libzip_mock.z.count = 1;
  Tree t("dummy.zip");
}

}  // namespace
