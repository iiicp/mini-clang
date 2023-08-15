#ifndef LLVM_CLANG_FILEMANAGER_H
#define LLVM_CLANG_FILEMANAGER_H

#include "llvm/ADT/StringMap.h"
#include "llvm/Support/Allocator.h"
#include <map>
#include <set>
#include <string>
#include <sys/stat.h>
#include <sys/types.h>

namespace clang {
  class FileManager;
  /// DirectoryEntry - Cached information about one directory on the disk.
  ///
  class DirectoryEntry {
    const char *Name;// Name of the directory.
    friend class FileManager;

public:
    DirectoryEntry() : Name(nullptr) {}
    const char *getName() const { return Name; }
  };

  /// FileEntry - Cached information about one file on the disk.
  ///
  class FileEntry {
    const char *Name;         // Name of the file.
    off_t Size;               // File size in bytes.
    time_t ModTime;           // Modification time of file.
    const DirectoryEntry *Dir;// Directory file lives in.
    unsigned UID;             // A unique (small) ID for the file.
    dev_t Device;             // ID for the device containing the file.
    ino_t Inode;              // Inode number for the file.
    mode_t FileMode;          // The file mode as returned by 'stat'.
    friend class FileManager;

public:
    FileEntry(dev_t device, ino_t inode, mode_t m)
        : Name(nullptr), Device(device), Inode(inode), FileMode(m) {}
    // Add a default constructor for use with llvm::StringMap
    FileEntry() : Name(nullptr), Device(0), Inode(0), FileMode(0) {}

    const char *getName() const { return Name; }
    off_t getSize() const { return Size; }
    unsigned getUID() const { return UID; }
    ino_t getInode() const { return Inode; }
    dev_t getDevice() const { return Device; }
    time_t getModificationTime() const { return ModTime; }
    mode_t getFileMode() const { return FileMode; }

    /// getDir - Return the directory the file lives in.
    ///
    const DirectoryEntry *getDir() const { return Dir; }

    bool operator<(const FileEntry &RHS) const {
      return Device < RHS.Device || (Device == RHS.Device && Inode < RHS.Inode);
    }
  };
  /// FileManager - Implements support for file system lookup, file system
  /// caching, and directory search management.  This also handles more advanced
  /// properties, such as uniquing files based on "inode", so that a file with two
  /// names (e.g. symlinked) will be treated as a single file.
  ///
  class FileManager {

    class UniqueDirContainer;
    class UniqueFileContainer;

    /// UniqueDirs/UniqueFiles - Cache for existing directories/files.
    ///
    UniqueDirContainer &UniqueDirs;
    UniqueFileContainer &UniqueFiles;

    /// DirEntries/FileEntries - This is a cache of directory/file entries we have
    /// looked up.  The actual Entry is owned by UniqueFiles/UniqueDirs above.
    ///
    llvm::StringMap<DirectoryEntry *, llvm::BumpPtrAllocator> DirEntries;
    llvm::StringMap<FileEntry *, llvm::BumpPtrAllocator> FileEntries;

    /// NextFileUID - Each FileEntry we create is assigned a unique ID #.
    ///
    unsigned NextFileUID;

    // Statistics.
    unsigned NumDirLookups, NumFileLookups;
    unsigned NumDirCacheMisses, NumFileCacheMisses;

public:
    FileManager();
    ~FileManager();
    /// getDirectory - Lookup, cache, and verify the specified directory.  This
    /// returns null if the directory doesn't exist.
    ///
    const DirectoryEntry *getDirectory(const std::string &Filename) {
      return getDirectory(&Filename[0], &Filename[0] + Filename.size());
    }

    const DirectoryEntry *getDirectory(const char *FileStart,
                                       const char *FileEnd);
    /// getFile - Lookup, cache, and verify the specified file.  This returns null
    /// if the file doesn't exist.
    ///
    const FileEntry *getFile(const std::string &Filename) {
      return getFile(&Filename[0], &Filename[0] + Filename.size());
    }
    const FileEntry *getFile(const char *FilenameStart, const char *FilenameEnd);

    void PrintStats() const;
  };

}// namespace clang

#endif