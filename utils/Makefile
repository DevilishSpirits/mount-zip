# Copyright 2021 Google LLC
# Copyright 2019-2021 Alexander Galanin <al@galanin.nnov.ru>
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
OUT ?= $(abspath ../out)
LIB = $(OUT)/libmountzip.a
LIBS = -L$(OUT) -lmountzip $(shell $(PKG_CONFIG) libzip --libs)
CXXFLAGS += $(shell $(PKG_CONFIG) libzip --cflags)
CXXFLAGS += -Wall -Wextra -Wno-nullability-extension -Wno-sign-compare -Wno-missing-field-initializers -pedantic -std=c++23
CXXFLAGS += -D_FILE_OFFSET_BITS=64 
LDFLAGS += -lstdc++

ifeq ($(DEBUG), 1)
CXXFLAGS += -O0 -g
else
CXXFLAGS += -O2 -DNDEBUG
endif

ifeq ($(ASAN), 1)
CXXFLAGS += -fsanitize=address
LDFLAGS += -fsanitize=address
endif

ifeq ($(UBSAN), 1)
CXXFLAGS += -fsanitize=undefined
LDFLAGS += -fsanitize=undefined
endif

DUMP_EXTRAFLD = $(OUT)/utils/dump-extrafld
CREATE_CONTENT_SIZE = $(OUT)/utils/create-content-size

all: $(DUMP_EXTRAFLD) $(CREATE_CONTENT_SIZE)

$(DUMP_EXTRAFLD): dump-extrafld.cpp $(LIB)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -I.. $(LDFLAGS) $(LIBS) -o $@

$(CREATE_CONTENT_SIZE): create-content-size.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $< -I.. $(LDFLAGS) $(LIBS) -o $@

clean:
	rm -rf $(OUT)/utils

$(LIB):
	$(MAKE) -C ../lib ASAN=$(ASAN) UBSAN=$(UBSAN) DEBUG=$(DEBUG) OUT=$(OUT)

.PHONY: all clean $(LIB)
