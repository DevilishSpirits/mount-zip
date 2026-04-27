// Copyright 2021 Google LLC
// Copyright 2008-2021 Alexander Galanin <al@galanin.nnov.ru>
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

#ifndef NODE_H
#define NODE_H

#include <cassert>
#include <functional>
#include <iterator>
#include <memory>
#include <ostream>
#include <string>
#include <string_view>
#include <vector>

#include <sys/stat.h>
#include <unistd.h>
#include <zip.h>

#include <boost/intrusive/slist.hpp>
#include <boost/intrusive/unordered_set.hpp>

#include "path.h"
#include "reader.h"

namespace bi = boost::intrusive;

enum class FileType : mode_t {
  Unknown = 0,            // Unknown
  BlockDevice = S_IFBLK,  // Block-oriented device
  CharDevice = S_IFCHR,   // Character-oriented device
  Directory = S_IFDIR,    // Directory
  Fifo = S_IFIFO,         // FIFO or pipe
  File = S_IFREG,         // Regular file
  Socket = S_IFSOCK,      // Socket
  Symlink = S_IFLNK,      // Symbolic link
};

inline FileType GetFileType(const mode_t mode) {
  return FileType(mode & S_IFMT);
}

inline void SetFileType(mode_t* const mode, const FileType type) {
  assert(mode);
  *mode &= ~static_cast<mode_t>(S_IFMT);
  *mode |= static_cast<mode_t>(type);
}

std::ostream& operator<<(std::ostream& out, FileType t);

// Represents a named file or directory entry in the filesystem tree.
struct Node {
  // Nodes are dynamically allocated and passed around by unique_ptr when
  // the ownership is transferred.
  using Ptr = std::unique_ptr<Node>;

  // Constants and settings shared by all nodes.
  static const timespec g_now;
  static const uid_t g_uid;
  static const gid_t g_gid;
  static mode_t fmask;
  static mode_t dmask;
  static bool original_permissions;
  static ino_t ino_count;

  // Reference to the ZIP archive.
  zip_t* zip = nullptr;

  // Index of the entry represented by this node in the ZIP archive, or -1 if it
  // is not directly represented in the ZIP archive (like the root directory, or
  // any intermediate directory).
  i64 id = -1;

  // Inode-specific data.
  ino_t const ino = ++ino_count;
  mutable nlink_t nlink = 1;
  mode_t mode = 0;
  uid_t uid = g_uid;
  gid_t gid = g_gid;
  dev_t dev = 0;
  zip_uint64_t size = 0;
  timespec mtime = g_now;
  timespec atime = g_now;
  timespec ctime = g_now;

  // Link target (e.g. for symlinks or hardlinks)
  std::string target;
  mutable Reader::Ptr cached_reader;
  static const blksize_t block_size = 512;

  // If this Node is a hardlink, this points to the target node.
  Node* hardlink_target = nullptr;

  // Pointer to the parent node. Should be non null. The only exception is the
  // root directory which has a null parent pointer.
  Node* parent = nullptr;

  // Name of this node in the context of its parent. This name should be a valid
  // and non-empty filename, and it shouldn't contain any '/' separator. The
  // only exception is the root directory, which is just named "/".
  std::string name;

  // Original path as recorded in the ZIP archive. This is used to find hardlink
  // targets.
  std::string_view original_path;

  // Number of entries whose name have initially collided with this node.
  int collision_count = 0;

#ifdef NDEBUG
  using LinkMode = bi::link_mode<bi::normal_link>;
#else
  using LinkMode = bi::link_mode<bi::safe_link>;
#endif

  // Hook used to index Nodes by parent.
  using ByParent = bi::slist_member_hook<LinkMode>;
  ByParent by_parent;

  // Children of this node. The children are not sorted and their order is not
  // relevant. This collection doesn't own the children nodes. The |parent|
  // pointer of every child in |children| should point back to this node.
  using Children = bi::slist<Node,
                             bi::member_hook<Node, ByParent, &Node::by_parent>,
                             bi::constant_time_size<false>,
                             bi::linear<true>,
                             bi::cache_last<true>>;
  Children children;

  // Hooks used to index Nodes by full path and by original path.
  using ByPath = bi::unordered_set_member_hook<LinkMode, bi::store_hash<true>>;
  ByPath by_path, by_original_path;

  // Methods.
  const Node& GetTarget() const {
    return hardlink_target ? *hardlink_target : *this;
  }
  Node& GetTarget() { return hardlink_target ? *hardlink_target : *this; }

  using Stat = struct stat;
  operator Stat() const;

  FileType GetType() const { return GetFileType(GetTarget().mode); }
  bool IsDir() const { return GetType() == FileType::Directory; }

  // Gets the full absolute path of this node.
  std::string GetPath() const;

  void AddChild(Node* child);
  Node* GetUniqueChildDirectory();

  bool CacheAll(std::function<void(ssize_t)> progress = {});

  // Gets a Reader to read file contents.
  Reader::Ptr GetReader() const;

  static void Init(Node& node, zip_t* zip, i64 id, mode_t mode);
};

std::ostream& operator<<(std::ostream& out, const Node& node);

#endif  // NODE_H
