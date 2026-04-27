# Copyright 2021 Google LLC
# Copyright 2010-2021 Alexander Galanin <al@galanin.nnov.ru>
# http://galanin.nnov.ru/~al
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

PKG_CONFIG ?= pkg-config
PC_DEPS = libzip icu-uc icu-i18n gtest gtest_main

PROJECT_CXXFLAGS = -std=c++20 -Wall -Wextra -Wno-sign-compare \
                   -Wno-missing-field-initializers -g -D_FILE_OFFSET_BITS=64 \
                   -D_TIME_BITS=64

PROJECT_CXXFLAGS += $(shell $(PKG_CONFIG) --cflags $(PC_DEPS))

ifeq ($(DEBUG), 1)
PROJECT_CXXFLAGS += -O0
else
PROJECT_CXXFLAGS += -O2
endif

PROJECT_LDFLAGS = -L$(OUT) -lmountzip $(shell $(PKG_CONFIG) --libs $(PC_DEPS))

ifeq ($(ASAN), 1)
PROJECT_CXXFLAGS += -fsanitize=address
PROJECT_LDFLAGS += -fsanitize=address
endif

ifeq ($(UBSAN), 1)
PROJECT_CXXFLAGS += -fsanitize=undefined
PROJECT_LDFLAGS += -fsanitize=undefined
endif

VALGRIND = valgrind -q --leak-check=full --track-origins=yes --error-exitcode=33
OUT ?= $(abspath ../../out)
LIB = $(OUT)/libmountzip.a
EXECUTABLE = $(OUT)/tests/whitebox/unit_tests

SOURCES = $(wildcard *.cc)
OBJECTS = $(addprefix $(OUT)/tests/whitebox/,$(SOURCES:.cc=.o))

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) $(LIB)
	$(CXX) $(LDFLAGS) $(OBJECTS) $(PROJECT_LDFLAGS) -o $@

$(OUT)/tests/whitebox/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) -I../../lib -c $(CPPFLAGS) $(PROJECT_CXXFLAGS) $(CXXFLAGS) $< -o $@ -MMD -MP -MF $(@:.o=.d)

ifneq ($(filter clean,$(MAKECMDGOALS)),)
else
-include $(OBJECTS:.o=.d)
endif

$(LIB):
	$(MAKE) -C ../../lib ASAN=$(ASAN) UBSAN=$(UBSAN) DEBUG=$(DEBUG) OUT=$(OUT)

clean:
	rm -rf $(OUT)/tests/whitebox

test: $(EXECUTABLE)
	$(EXECUTABLE)

valgrind: $(EXECUTABLE)
	$(VALGRIND) $(EXECUTABLE)

.PHONY: all clean test valgrind $(LIB)
