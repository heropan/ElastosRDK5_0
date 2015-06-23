#ifndef __FILEUTILS_H__
#define __FILEUTILS_H__

#ifdef DROID_CORE
#include "Elastos.Droid.Core_server.h"
#else
#include "Elastos.Droid.Core.h"
#endif
#include <Elastos.CoreLibrary.h>

using Elastos::Utility::Regex::IPattern;
using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IFileOutputStream;

namespace Elastos {
namespace Droid {
namespace Os {

/**
 * Tools for managing files.  Not for public consumption.
 * @hide
 */
class FileUtils
{
public:
    static CARAPI_(Int32) SetPermissions(
        /* [in] */ const String& file,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    /** returns the FAT file system volume ID for the volume mounted
     * at the given mount point, or -1 for failure
     * @param mountPoint point for FAT volume
     * @return volume ID or -1
     */
    static CARAPI_(Int32) GetFatVolumeId(
        /* [in] */ const String& mountPoint);

    /**
     * Perform an fsync on the given FileOutputStream.  The stream at this
     * point must be flushed but not yet closed.
     */
    static CARAPI Sync(
        /* [in] */ IFileOutputStream* stream,
        /* [out] */ Boolean* result);

    // copy a file from srcFile to destFile, return true if succeed, return
    // false if fail
    static CARAPI CopyFile(
        /* [in] */ IFile* srcFile,
        /* [in] */ IFile* destFile,
        /* [out] */ Boolean* result);

    /**
     * Copy data from a source stream to destFile.
     * Return true if succeed, return false if failed.
     */
    static CARAPI CopyToFile(
        /* [in] */ IInputStream* inputStream,
        /* [in] */ IFile* destFile,
        /* [out] */ Boolean* result);

    /**
     * Check if a filename is "safe" (no metacharacters or spaces).
     * @param file  The file to check
     */
    static CARAPI IsFilenameSafe(
        /* [in] */ IFile* srcFile,
        /* [out] */ Boolean* result);

    /**
     * Read a text file into a String, optionally limiting the length.
     * @param file to read (will not seek, so things like /proc files are OK)
     * @param max length (positive for head, negative of tail, 0 for no limit)
     * @param ellipsis to add of the file was truncated (can be null)
     * @return the contents of the file, possibly truncated
     * @throws IOException if something goes wrong reading the file
     */
    static CARAPI ReadTextFile(
        /* [in] */ IFile* file,
        /* [in] */ Int32 max,
        /* [in] */ const String& ellipsis,
        /* [out] */ String* result);

    /**
      * Writes string to file. Basically same as "echo -n $string > $filename"
      *
      * @param filename
      * @param string
      * @throws IOException
      */
    static CARAPI StringToFile(
        /* [in] */ const String& filename,
        /* [in] */ const String& string);

    /**
     * Computes the checksum of a file using the CRC32 checksum routine.
     * The value of the checksum is returned.
     *
     * @param file  the file to checksum, must not be null
     * @return the checksum value or an exception is thrown.
     */
    static CARAPI ChecksumCrc32(
        /* [in] */ IFile* file,
        /* [out] */ Int64* summer);

public:
    static const Int32 sS_IRWXU = 00700;
    static const Int32 sS_IRUSR = 00400;
    static const Int32 sS_IWUSR = 00200;
    static const Int32 sS_IXUSR = 00100;

    static const Int32 sS_IRWXG = 00070;
    static const Int32 sS_IRGRP = 00040;
    static const Int32 sS_IWGRP = 00020;
    static const Int32 sS_IXGRP = 00010;

    static const Int32 sS_IRWXO = 00007;
    static const Int32 sS_IROTH = 00004;
    static const Int32 sS_IWOTH = 00002;
    static const Int32 sS_IXOTH = 00001;

private:
    /** Regular expression for safe filenames: no spaces or metacharacters */
    static AutoPtr<IPattern> SAFE_FILENAME_PATTERN;
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__FILEUTILS_H__
