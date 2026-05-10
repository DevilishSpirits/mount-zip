// Copyright 2021 Google LLC
// Copyright 2008-2019 Alexander Galanin <al@galanin.nnov.ru>
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

#include <gtest/gtest.h>
#include <string>

#include "log.h"
#include "path.h"

namespace {

void checkConversion(const std::string_view fname,
                     const std::string_view expected) {
  EXPECT_EQ(Path(fname).Normalized(), expected);
}

TEST(FilenameValidator, Normalized) {
  checkConversion("normal.name", "/normal.name");
  checkConversion("path/to/normal.name", "/path/to/normal.name");
  checkConversion("", "/?");
  checkConversion("./", "/");
  checkConversion(".///", "/");
  checkConversion("./..//.///", "/");
  checkConversion("./..//.///a/b/c", "/a/b/c");
  checkConversion("a/.", "/a/?");
  checkConversion("a/./", "/a/?");
  checkConversion("a/./c", "/a/?/c");
  checkConversion("a/..", "/a/?");
  checkConversion("a/../", "/a/?");
  checkConversion("a/../c", "/a/?/c");
  checkConversion(".", "/?");
  checkConversion("..", "/?");
  checkConversion("/.", "/?");
  checkConversion("/..", "/?");
  checkConversion("/./a", "/a");
  checkConversion("/../a", "/a");
  checkConversion(".hidden", "/.hidden");
  checkConversion("path/to/.hidden", "/path/to/.hidden");
  checkConversion("path/to/.hidden/dir/", "/path/to/.hidden/dir");
  checkConversion("../", "/");
  checkConversion("../../../", "/");
  checkConversion("../abc", "/abc");
  checkConversion("../../../abc", "/abc");
  checkConversion("..///..//..//abc", "/abc");
  checkConversion("/", "/");
  checkConversion("///", "/");
  checkConversion("/rootname", "/rootname");
  checkConversion("///rootname", "/rootname");
  checkConversion("/path/name", "/path/name");
  checkConversion("///path///name", "/path/name");
}

TEST(PathTest, OutputOperator) {
  std::ostringstream oss;
  Path::redact = false;
  oss << Path("foo/bar.txt");
  EXPECT_EQ(oss.str(), "'foo/bar.txt'");

  oss.str("");
  oss << Path("a'b\\c\x01");
  EXPECT_EQ(oss.str(), "'a\\'b\\\\c\\x01'");

  oss.str("");
  Path::redact = true;
  oss << Path("private/data.txt");
  EXPECT_EQ(oss.str(), "(redacted)");
  Path::redact = false;
}

TEST(LogTest, Timer) {
  Timer timer;
  EXPECT_GE(timer.Milliseconds(), 0);
  std::ostringstream oss;
  oss << timer;
  EXPECT_TRUE(oss.str().find("ms") != std::string::npos);
}

TEST(LogTest, Beat) {
  Beat beat;
  // Initial state: not time for next beat yet.
  EXPECT_FALSE(beat);
  EXPECT_EQ(beat.Count(), 0);
}

}  // namespace
