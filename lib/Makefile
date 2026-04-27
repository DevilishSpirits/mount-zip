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

OUT ?= $(abspath ../out)
DEST = $(OUT)/libmountzip.a
PKG_CONFIG ?= pkg-config
DEPS = libzip icu-uc icu-i18n

PROJECT_CXXFLAGS = -std=c++20 -Wall -Wextra -Wno-nullability-extension \
                   -Wno-sign-compare -Wno-missing-field-initializers \
                   -pedantic -D_FILE_OFFSET_BITS=64 -D_TIME_BITS=64

PROJECT_CXXFLAGS += $(shell $(PKG_CONFIG) --cflags $(DEPS))

ifeq ($(DEBUG), 1)
PROJECT_CXXFLAGS += -O0 -g
else
PROJECT_CXXFLAGS += -O2 -DNDEBUG
endif

ifeq ($(ASAN), 1)
PROJECT_CXXFLAGS += -fsanitize=address
endif

ifeq ($(UBSAN), 1)
PROJECT_CXXFLAGS += -fsanitize=undefined
endif

SOURCES = $(sort $(wildcard *.cc))
OBJECTS = $(addprefix $(OUT)/lib/,$(SOURCES:.cc=.o))

all: $(DEST)

$(DEST): $(OBJECTS)
	$(AR) $(ARFLAGS) $@ $(OBJECTS)

$(OUT)/lib/%.o: %.cc
	@mkdir -p $(dir $@)
	$(CXX) -c $(CPPFLAGS) $(PROJECT_CXXFLAGS) $(CXXFLAGS) $< -o $@ -MMD -MP -MF $(@:.o=.d)

ifneq ($(filter clean,$(MAKECMDGOALS)),)
else
-include $(OBJECTS:.o=.d)
endif

clean:
	rm -rf $(OUT)/lib
	rm -f $(DEST)

.PHONY: all clean
