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

#include "libzip_mock.h"

#include <gtest/gtest.h>
#include <cassert>
#include <cstring>

LibZipMock g_libzip_mock;

// libzip stub structures
struct zip_file {};
struct zip_source {};

// libzip stub functions

void zip_stat_init(struct zip_stat* sb) {
  memset(sb, 0, sizeof(struct zip_stat));
}

struct zip* zip_open(const char*, int, int* errorp) {
  if (g_libzip_mock.zip_open_fails) {
    if (errorp) {
      *errorp = g_libzip_mock.zip_open_error;
    }
    return nullptr;
  }
  if (errorp) {
    *errorp = 0;
  }
  return reinterpret_cast<struct zip*>(&g_libzip_mock.z);
}

int zip_close(struct zip*) {
  return 0;
}

int zip_error_to_str(char* buf, zip_uint64_t len, int, int) {
  if (len == 0) {
    return 0;
  }
  strncpy(buf, "Expected error", len);
  buf[len - 1] = '\0';
  return strlen(buf);
}

void zip_error_init_with_code(zip_error_t*, int) {}

const char* zip_error_strerror(zip_error_t*) {
  return "Expected error";
}

void zip_error_fini(zip_error_t*) {}

zip_int64_t zip_get_num_entries(struct zip* z, zip_flags_t) {
  return reinterpret_cast<LibZipMock::zip*>(z)->count;
}

const char* zip_get_name(struct zip* z, zip_uint64_t, zip_flags_t) {
  return reinterpret_cast<LibZipMock::zip*>(z)->filename.c_str();
}

int zip_stat_index(struct zip* z,
                   zip_uint64_t index,
                   zip_flags_t,
                   struct zip_stat* zs) {
  auto* mz = reinterpret_cast<LibZipMock::zip*>(z);
  memset(zs, 0, sizeof(struct zip_stat));
  zs->valid = ZIP_STAT_NAME | ZIP_STAT_INDEX | ZIP_STAT_SIZE |
              ZIP_STAT_COMP_SIZE | ZIP_STAT_MTIME | ZIP_STAT_CRC |
              ZIP_STAT_COMP_METHOD | ZIP_STAT_ENCRYPTION_METHOD |
              ZIP_STAT_FLAGS;
  zs->name = mz->filename.c_str();
  zs->index = index;
  zs->size = 0;
  zs->comp_size = 0;
  zs->mtime = 0;
  zs->crc = 0;
  zs->comp_method = ZIP_CM_STORE;
  zs->encryption_method = ZIP_EM_NONE;
  zs->flags = 0;
  return 0;
}

struct zip_file* zip_fopen_index(struct zip*, zip_uint64_t, zip_flags_t) {
  static struct zip_file f;
  return &f;
}

zip_int64_t zip_fread(struct zip_file*, void*, zip_uint64_t) {
  return 0;
}

int zip_fclose(struct zip_file*) {
  return 0;
}

int zip_delete(struct zip*, zip_uint64_t) {
  return 0;
}

int zip_file_rename(struct zip*, zip_uint64_t, const char*, zip_flags_t) {
  return 0;
}

int zip_file_replace(struct zip*,
                     zip_uint64_t,
                     struct zip_source*,
                     zip_flags_t) {
  return 0;
}

struct zip_source* zip_source_function(struct zip*,
                                       zip_source_callback,
                                       void*) {
  static struct zip_source s;
  return &s;
}

void zip_error_clear(struct zip*) {}

void zip_error_get(struct zip*, int*, int*) {}

int zip_error_get_sys_type(int) {
  return 0;
}

void zip_file_error_clear(struct zip_file*) {}

void zip_file_error_get(struct zip_file*, int*, int*) {}

struct zip_file* zip_fopen(struct zip*, const char*, int) {
  return NULL;
}

const char* zip_get_archive_comment(struct zip*, int*, int) {
  return NULL;
}

int zip_get_archive_flag(struct zip*, int, int) {
  return 0;
}

const char* zip_get_file_comment(struct zip*, int, int*, int) {
  return NULL;
}

int zip_name_locate(struct zip*, const char*, int) {
  return 0;
}

int zip_set_archive_comment(struct zip*, const char*, int) {
  return 0;
}

int zip_set_archive_flag(struct zip*, int, int) {
  return 0;
}

int zip_set_file_comment(struct zip*, int, const char*, int) {
  return 0;
}

struct zip_source* zip_source_buffer(struct zip*, const void*, off_t, int) {
  return NULL;
}

struct zip_source* zip_source_file(struct zip*, const char*, off_t, off_t) {
  return NULL;
}

struct zip_source* zip_source_filep(struct zip*, FILE*, off_t, off_t) {
  return NULL;
}

struct zip_source* zip_source_zip(struct zip*,
                                  struct zip*,
                                  int,
                                  int,
                                  off_t,
                                  off_t) {
  return NULL;
}

int zip_stat(struct zip*, const char*, int, struct zip_stat*) {
  return 0;
}

int zip_unchange(struct zip*, int) {
  return 0;
}

int zip_unchange_all(struct zip*) {
  return 0;
}

int zip_unchange_archive(struct zip*) {
  return 0;
}

zip_int16_t zip_file_extra_fields_count(struct zip*,
                                        zip_uint64_t,
                                        zip_flags_t) {
  return 0;
}

zip_int16_t zip_file_extra_fields_count_by_id(struct zip*,
                                              zip_uint64_t,
                                              zip_uint16_t,
                                              zip_flags_t) {
  return 0;
}

const zip_uint8_t* zip_file_extra_field_get(struct zip*,
                                            zip_uint64_t,
                                            zip_uint16_t,
                                            zip_uint16_t*,
                                            zip_uint16_t*,
                                            zip_flags_t) {
  return NULL;
}

const zip_uint8_t* zip_file_extra_field_get_by_id(struct zip*,
                                                  zip_uint64_t,
                                                  zip_uint16_t,
                                                  zip_uint16_t,
                                                  zip_uint16_t*,
                                                  zip_flags_t) {
  return NULL;
}

int zip_file_extra_field_delete(struct zip*,
                                zip_uint64_t,
                                zip_uint16_t,
                                zip_flags_t) {
  return 0;
}

int zip_file_extra_field_set(struct zip*,
                             zip_uint64_t,
                             zip_uint16_t,
                             zip_uint16_t,
                             const zip_uint8_t*,
                             zip_uint16_t,
                             zip_flags_t) {
  return 0;
}

int zip_file_get_external_attributes(struct zip*,
                                     zip_uint64_t,
                                     zip_flags_t,
                                     zip_uint8_t* opsysPtr,
                                     zip_uint32_t* attrPtr) {
  *opsysPtr = ZIP_OPSYS_UNIX;
  *attrPtr = 0;
  return 0;
}

const char* zip_file_get_comment(zip_t*,
                                 zip_uint64_t,
                                 zip_uint32_t*,
                                 zip_flags_t) {
  return NULL;
}

int zip_file_set_external_attributes(struct zip*,
                                     zip_uint64_t,
                                     zip_flags_t,
                                     zip_uint8_t,
                                     zip_uint32_t) {
  return 0;
}

void zip_source_free(struct zip_source*) {}
