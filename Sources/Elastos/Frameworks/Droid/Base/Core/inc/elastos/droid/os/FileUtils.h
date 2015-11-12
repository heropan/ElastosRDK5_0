#ifndef __ELASTOS_DROID_OS_FILEUTILS_H__
#define __ELASTOS_DROID_OS_FILEUTILS_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/core/Object.h"

using Elastos::Core::IComparator;
using Elastos::Utility::Regex::IPattern;
using Elastos::IO::IFile;
using Elastos::IO::IInputStream;
using Elastos::IO::IFileOutputStream;
using Elastos::IO::IFileDescriptor;

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
    /**
     * Set owner and mode of of given {@link File}.
     *
     * @param mode to apply through {@code chmod}
     * @param uid to apply through {@code chown}, or -1 to leave unchanged
     * @param gid to apply through {@code chown}, or -1 to leave unchanged
     * @return 0 on success, otherwise errno.
     */
    static CARAPI_(Int32) SetPermissions(
        /* [in] */ IFile* file,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    /**
     * Set owner and mode of of given path.
     *
     * @param mode to apply through {@code chmod}
     * @param uid to apply through {@code chown}, or -1 to leave unchanged
     * @param gid to apply through {@code chown}, or -1 to leave unchanged
     * @return 0 on success, otherwise errno.
     */
    static CARAPI_(Int32) SetPermissions(
        /* [in] */ const String& path,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    /**
     * Set owner and mode of of given {@link FileDescriptor}.
     *
     * @param mode to apply through {@code chmod}
     * @param uid to apply through {@code chown}, or -1 to leave unchanged
     * @param gid to apply through {@code chown}, or -1 to leave unchanged
     * @return 0 on success, otherwise errno.
     */
    static CARAPI_(Int32) SetPermissions(
        /* [in] */ IFileDescriptor* file,
        /* [in] */ Int32 mode,
        /* [in] */ Int32 uid,
        /* [in] */ Int32 gid);

    /**
     * Return owning UID of given path, otherwise -1.
     */
    static CARAPI_(Int32) GetUid(
        /* [in] */ const String& path);

    /**
     * Perform an fsync on the given FileOutputStream.  The stream at this
     * point must be flushed but not yet closed.
     */
    static CARAPI_(Boolean) Sync(
        /* [in] */ IFileOutputStream* stream);

    // copy a file from srcFile to destFile, return true if succeed, return
    // false if fail
    static CARAPI_(Boolean) CopyFile(
        /* [in] */ IFile* srcFile,
        /* [in] */ IFile* destFile);

    /**
     * Copy data from a source stream to destFile.
     * Return true if succeed, return false if failed.
     */
    static CARAPI_(Boolean) CopyToFile(
        /* [in] */ IInputStream* inputStream,
        /* [in] */ IFile* destFile);

    /**
     * Check if a filename is "safe" (no metacharacters or spaces).
     * @param file  The file to check
     */
    static CARAPI_(Boolean) IsFilenameSafe(
        /* [in] */ IFile* srcFile);

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

    /**
     * Delete older files in a directory until only those matching the given
     * constraints remain.
     *
     * @param minCount Always keep at least this many files.
     * @param minAge Always keep files younger than this age.
     * @return if any files were deleted.
     */
    static CARAPI_(Boolean) DeleteOlderFiles(
        /* [in] */ IFile* dir,
        /* [in] */ Int32 minCount,
        /* [in] */ Int64 minAge);

    /**
     * Test if a file lives under the given directory, either as a direct child
     * or a distant grandchild.
     * <p>
     * Both files <em>must</em> have been resolved using
     * {@link File#getCanonicalFile()} to avoid symlink or path traversal
     * attacks.
     */
    static CARAPI_(Boolean) Contains(
        /* [in] */ IFile* dir,
        /* [in] */ IFile* file);

    static CARAPI_(Boolean) DeleteContents(
        /* [in] */ IFile* dir);

    /**
     * Assert that given filename is valid on ext4.
     */
    static CARAPI_(Boolean) IsValidExtFilename(
        /* [in] */ const String& name);

    static CARAPI_(String) RewriteAfterRename(
        /* [in] */ IFile* beforeDir,
        /* [in] */ IFile* afterDir,
        /* [in] */ const String& path);

    static CARAPI_(AutoPtr<ArrayOf<String> >) RewriteAfterRename(
        /* [in] */ IFile* beforeDir,
        /* [in] */ IFile* afterDir,
        /* [in] */ ArrayOf<String>* path);

    /**
     * Given a path under the "before" directory, rewrite it to live under the
     * "after" directory. For example, {@code /before/foo/bar.txt} would become
     * {@code /after/foo/bar.txt}.
     */
    static AutoPtr<IFile> RewriteAfterRename(
        /* [in] */ IFile* beforeDir,
        /* [in] */ IFile* afterDir,
        /* [in] */ IFile* file);

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

private:
    class FileComparator
        : public Object
        , public IComparator
    {
    public:
        CAR_INTERFACE_DECL()

        CARAPI Compare(
            /* [in] */ IInterface* lhs,
            /* [in] */ IInterface* rhs,
            /* [out] */ Int32* result);
    };
};

} // namespace Os
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_OS_FILEUTILS_H__
