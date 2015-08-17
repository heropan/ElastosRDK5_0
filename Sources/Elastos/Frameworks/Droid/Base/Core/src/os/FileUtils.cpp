#include "ext/frameworkext.h"
#include "os/FileUtils.h"
#include <sys/errno.h>
#include <sys/stat.h>
#include <fcntl.h>

using Elastos::Core::CString;
using Elastos::Core::ICharSequence;
using Elastos::IO::IFlushable;
using Elastos::IO::ICloseable;
using Elastos::IO::IFileDescriptor;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CByteArrayOutputStream;
using Elastos::IO::IFileWriter;
using Elastos::IO::CFileWriter;
using Elastos::Utility::Regex::IPatternHelper;
using Elastos::Utility::Regex::CPatternHelper;
using Elastos::Utility::Regex::IMatcher;
using Elastos::Utility::Zip::ICRC32;
using Elastos::Utility::Zip::CCRC32;
using Elastos::Utility::Zip::ICheckedInputStream;
using Elastos::Utility::Zip::CCheckedInputStream;

namespace Elastos {
namespace Droid {
namespace Os {

static AutoPtr<IPattern> InitSAFE_FILENAME_PATTERN()
{
    const String str("[\\w%+,./=_-]+");
    AutoPtr<IPatternHelper> helper;
    CPatternHelper::AcquireSingleton((IPatternHelper**)&helper);
    AutoPtr<IPattern> pattern;
    helper->Compile(str, (IPattern**)&pattern);
    return pattern;
}

AutoPtr<IPattern> FileUtils::SAFE_FILENAME_PATTERN = InitSAFE_FILENAME_PATTERN();

const Int32 FileUtils::sS_IRWXU;
const Int32 FileUtils::sS_IRUSR;
const Int32 FileUtils::sS_IWUSR;
const Int32 FileUtils::sS_IXUSR;
const Int32 FileUtils::sS_IRWXG;
const Int32 FileUtils::sS_IRGRP;
const Int32 FileUtils::sS_IWGRP;
const Int32 FileUtils::sS_IXGRP;
const Int32 FileUtils::sS_IRWXO;
const Int32 FileUtils::sS_IROTH;
const Int32 FileUtils::sS_IWOTH;
const Int32 FileUtils::sS_IXOTH;

Int32 FileUtils::SetPermissions(
    /* [in] */ const String& filename,
    /* [in] */ Int32 mode,
    /* [in] */ Int32 uid,
    /* [in] */ Int32 gid)
{
    if (filename.IsNullOrEmpty())
        return ENOENT;

    if (uid >= 0 || gid >= 0) {
        Int32 res = chown(filename, uid, gid);
        if (res != 0) {
            return errno;
        }
    }
    return chmod(filename, mode) == 0 ? 0 : errno;
}

/** returns the FAT file system volume ID for the volume mounted
 * at the given mount point, or -1 for failure
 * @param mountPoint point for FAT volume
 * @return volume ID or -1
 */
Int32 FileUtils::GetFatVolumeId(
    /* [in] */ const String& mountPoint)
{
    Int32 result = -1;
    // only if our system supports this ioctl
    #ifdef VFAT_IOCTL_GET_VOLUME_ID
    Int32 fd = open(mountPoint, O_RDONLY);
    if (fd >= 0) {
        result = ioctl(fd, VFAT_IOCTL_GET_VOLUME_ID);
        close(fd);
    }
    #endif
    return result;
}

/**
 * Perform an fsync on the given FileOutputStream.  The stream at this
 * point must be flushed but not yet closed.
 */
ECode FileUtils::Sync(
    /* [in] */ IFileOutputStream* stream,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    if (stream != NULL) {
        AutoPtr<IFileDescriptor> fd;
        FAIL_RETURN(stream->GetFD((IFileDescriptor**)&fd));
        FAIL_RETURN(fd->Sync());
        *result = TRUE;
    }

    return NOERROR;
}

// copy a file from srcFile to destFile, return true if succeed, return
// FALSE if fail
ECode FileUtils::CopyFile(
    /* [in] */ IFile* srcFile,
    /* [in] */ IFile* destFile,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    VALIDATE_NOT_NULL(srcFile);
    VALIDATE_NOT_NULL(destFile);
    *result = FALSE;

    AutoPtr<IInputStream> in;
    ECode ec = CFileInputStream::New(srcFile, (IFileInputStream**)&in);
    FAIL_GOTO(ec, _EXIT_);
    ec = CopyToFile(in, destFile, result);

_EXIT_:
    if (in)
        in->Close();
    return ec;
}

/**
 * Copy data from a source stream to destFile.
 * Return true if succeed, return FALSE if failed.
 */
ECode FileUtils::CopyToFile(
    /* [in] */ IInputStream* inputStream,
    /* [in] */ IFile* destFile,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(inputStream);
    VALIDATE_NOT_NULL(destFile);
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    Boolean temp;
    destFile->Exists(&temp);
    if (temp) {
        destFile->Delete(&temp);
    }

    AutoPtr<IFileDescriptor> fd;
    AutoPtr<ArrayOf<Byte> > buffer;
    Int32 bytesRead;

    AutoPtr<IFileOutputStream> out;
    ECode ec = CFileOutputStream::New(destFile, (IFileOutputStream**)&out);
    FAIL_GOTO(ec, _EXIT_);

    buffer = ArrayOf<Byte>::Alloc(4096);
    while (inputStream->ReadBytes(buffer, &bytesRead), bytesRead >= 0) {
        out->WriteBytes(*buffer, 0, bytesRead);
    }

    IFlushable::Probe(out)->Flush();
    out->GetFD((IFileDescriptor**)&fd);
    fd->Sync();

    *result = TRUE;
_EXIT_:
    if (out) {
        out->Close();
    }

    return ec;
}

/**
 * Check if a filename is "safe" (no metacharacters or spaces).
 * @param file  The file to check
 */
ECode FileUtils::IsFilenameSafe(
    /* [in] */ IFile* file,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(file);

    // Note, we check whether it matches what's known to be safe,
    // rather than what's known to be unsafe.  Non-ASCII, control
    // characters, etc. are all unsafe by default.
    String filepath;
    file->GetPath(&filepath);
    AutoPtr<IMatcher> matcher;
    SAFE_FILENAME_PATTERN->Matcher(filepath, (IMatcher**)&matcher);
    return matcher->Matches(result);
}

/**
 * Read a text file into a String, optionally limiting the length.
 * @param file to read (will not seek, so things like /proc files are OK)
 * @param max length (positive for head, negative of tail, 0 for no limit)
 * @param ellipsis to add of the file was truncated (can be NULL)
 * @return the contents of the file, possibly truncated
 * @throws IOException if something goes wrong reading the file
 */
ECode FileUtils::ReadTextFile(
    /* [in] */ IFile* file,
    /* [in] */ Int32 max,
    /* [in] */ const String& ellipsis,
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(file);
    VALIDATE_NOT_NULL(result);
    *result = String("");

    AutoPtr<IInputStream> input;
    AutoPtr<IBufferedInputStream> bis;
    Int64 size;
    Int32 length;
    AutoPtr<ArrayOf<Byte> > data, last, tmp;
    Boolean rolled = FALSE;
    AutoPtr<IByteArrayOutputStream> contents;

    ECode ec = CFileInputStream::New(file, (IFileInputStream**)&input);
    FAIL_GOTO(ec, _EXIT_);

    // wrapping a BufferedInputStream around it because when reading /proc with unbuffered
    // input stream, bytes read not equal to buffer size is not necessarily the correct
    // indication for EOF; but it is true for BufferedInputStream due to its implementation.
    ec = CBufferedInputStream::New(input, (IBufferedInputStream**)&bis);
    FAIL_GOTO(ec, _EXIT_);

    file->GetLength(&size);
    if (max > 0 || (size > 0 && max == 0)) {  // "head" mode: read the first N bytes
        if (size > 0 && (max == 0 || size < max)) max = (Int32) size;
        data = ArrayOf<Byte>::Alloc(max + 1);
        bis->ReadBytes(data, &length);
        if (length <= 0) {
            *result = String("");
        }
        else if (length <= max) {
            *result = String((const char *)(data->GetPayload()), length);
        }
        else if (ellipsis.IsNull()) {
            *result = String((const char *)(data->GetPayload()), max);
        }
        else {
            *result = String((const char *)(data->GetPayload()), max);
            *result += ellipsis;
        }
    }
    else if (max < 0) {  // "tail" mode: keep the last N
        do {
            if (last != NULL) rolled = true;
            tmp = last; last = data; data = tmp;
            if (data == NULL) data = ArrayOf<Byte>::Alloc(-max);
            bis->ReadBytes(data, &length);
        } while (length == data->GetLength());

        if (last == NULL && length <= 0) {
            *result = String("");
            goto _EXIT_;
        }
        if (last == NULL) {
            *result = String((const char *)(data->GetPayload()), length);
            goto _EXIT_;
        }

        if (length > 0) {
            rolled = true;
            last->Copy(length, last, 0, last->GetLength() - length);
            last->Copy(last->GetLength() - length, data, 0, length);
//            System.arraycopy(last, length, last, 0, last.length - length);
//            System.arraycopy(data, 0, last, last.length - len, length);
        }

        if (ellipsis == NULL || !rolled) {
            *result = String((const char *)(last->GetPayload()), last->GetLength());
        }
        else {
            *result =  ellipsis;
            *result += String((const char *)(last->GetPayload()), last->GetLength());
        }
    }
    else {  // "cat" mode: size unknown, read it all in streaming fashion
        CByteArrayOutputStream::New((IByteArrayOutputStream**)&contents);
        data = ArrayOf<Byte>::Alloc(1024);
        do {
            bis->ReadBytes(data, &length);
            if (length > 0) contents->WriteBytes(*data, 0, length);
        } while (length == data->GetLength());

        contents->ToString(result);
    }

_EXIT_:
    if (bis) bis->Close();
    if (input) input->Close();

    return ec;
}

/**
  * Writes string to file. Basically same as "echo -n $string > $filename"
  *
  * @param filename
  * @param string
  * @throws IOException
  */
ECode FileUtils::StringToFile(
    /* [in] */ const String& filename,
    /* [in] */ const String& string)
{
    AutoPtr<IFileWriter> out;
    ECode ec = CFileWriter::New(filename, (IFileWriter**)&out);
    FAIL_GOTO(ec, _EXIT_);

    ec = out->WriteString(string);

_EXIT_:
    if (out && ICloseable::Probe(out))
        ICloseable::Probe(out)->Close();

    return ec;
}

/**
 * Computes the checksum of a file using the CRC32 checksum routine.
 * The value of the checksum is returned.
 *
 * @param file  the file to checksum, must not be NULL
 * @return the checksum value or an exception is thrown.
 */
ECode FileUtils::ChecksumCrc32(
    /* [in] */ IFile* file,
    /* [out] */ Int64* summer)
{
    VALIDATE_NOT_NULL(file);
    VALIDATE_NOT_NULL(summer);
    *summer = 0;

    AutoPtr<ArrayOf<Byte> > buf;
    Int32 length;

    AutoPtr<ICRC32> checkSummer;
    CCRC32::New((ICRC32**)&checkSummer);

    AutoPtr<IFileInputStream> input;
    CFileInputStream::New(file, (IFileInputStream**)&input);

    AutoPtr<ICheckedInputStream> cis;
    ECode ec = CCheckedInputStream::New(input, checkSummer, (ICheckedInputStream**)&cis);
    FAIL_GOTO(ec, _EXIT_);

    buf = ArrayOf<Byte>::Alloc(128);

    while(cis->ReadBytes(buf, &length), length >= 0) {
        // Just read for checksum to get calculated.
    }

    checkSummer->GetValue(summer);

_EXIT_:
    if (cis != NULL) {
        cis->Close();
    }

    return ec;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos
