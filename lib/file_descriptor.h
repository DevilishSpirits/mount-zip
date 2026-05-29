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

#ifndef FILE_DESCRIPTOR_H
#define FILE_DESCRIPTOR_H

#include <utility>

#include <sys/types.h>
#include <unistd.h>

#include "log.h"

// A scoped file descriptor.
class FileDescriptor {
 public:
  // Closes the underlying file descriptor if it is currently valid.
  ~FileDescriptor() {
    if (IsValid() && close(fd_) < 0) {
      PLOG(ERROR) << "Cannot close file";
    }
  }

  // Initializes an empty (invalid) FileDescriptor.
  FileDescriptor() noexcept : fd_(-1) {}

  // Takes ownership of the provided raw file descriptor.
  explicit FileDescriptor(int fd) noexcept : fd_(fd) {}

  // Move-constructs a FileDescriptor, taking ownership from the other instance.
  FileDescriptor(FileDescriptor&& other) noexcept
      : fd_(std::exchange(other.fd_, -1)) {}

  // Atomically swaps the underlying file descriptors between two instances.
  void SwapWith(FileDescriptor& other) noexcept { std::swap(fd_, other.fd_); }

  // Explicitly closes the underlying file descriptor and marks this instance as
  // invalid.
  void Close() noexcept {
    FileDescriptor other;
    SwapWith(other);
  }

  // Assigns ownership from another instance.
  FileDescriptor& operator=(FileDescriptor other) noexcept {
    SwapWith(other);
    return *this;
  }

  // Returns true if the instance holds a valid, non-negative file descriptor.
  bool IsValid() const noexcept { return fd_ >= 0; }

  // Gets the underlying raw file descriptor.
  operator int() const noexcept { return fd_; }

 private:
  friend class FileDescriptorTest;

  // Raw file descriptor.
  int fd_;
};

#ifdef CHROME_ICU
// A file mapping to memory.
class FileMapping {
 public:
  // Removes the memory mapping.
  ~FileMapping();

  // Maps a file to memory in read-only mode.
  // Throws a system_error in case of error.
  explicit FileMapping(const char* path);

  // No copy.
  FileMapping(const FileMapping&) = delete;
  FileMapping& operator=(const FileMapping&) = delete;

  // Start of the memory mapping.
  const void* data() const { return data_; }

 private:
  void* data_;
  size_t size_;
};
#endif

#endif  // FILE_DESCRIPTOR_H
