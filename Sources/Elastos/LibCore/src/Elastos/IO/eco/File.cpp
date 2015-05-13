
#include "cmdef.h"
#include "File.h"
#include "CFile.h"
#include "IoUtils.h"
#include "ToStringArray_S.h"
#include <elastos/List.h>
#include <elastos/Vector.h>
#include <elastos/StringBuilder.h>
#include <elastos/CSystem.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/param.h>
#include <utime.h>
#include <unistd.h>
#include "CRandom.h"
#include "COsConstants.h"
#include "CLibcore.h"

using Elastos::Utility::List;
using Elastos::Utility::Vector;
using Elastos::Core::CRandom;
using Elastos::Core::StringBuilder;
using Elastos::Core::CSystem;;
using Elastos::Core::ISystem;
using Libcore::IO::IOsConstants;
using Libcore::IO::COsConstants;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Libcore::IO::IOs;
using Libcore::IO::IStructStat;
using Libcore::IO::IStructStatFs;

static Boolean _Readlink(const char* path, String& result)
{
    // We can't know how big a buffer readlink(2) will need, so we need to
    // loop until it says "that fit".
    size_t bufSize = 512;
    while (TRUE) {
        AutoPtr< ArrayOf<Byte> > buf = ArrayOf<Byte>::Alloc(bufSize);
        ssize_t len = readlink(path, (char*)buf->GetPayload(), buf->GetLength());
        if (len == -1) {
            // An error occurred.
            return FALSE;
        }
        if (static_cast<size_t>(len) < buf->GetLength()) {
            (*buf)[len] = '\0';
            // The buffer was big enough.
            result += (char*)buf->GetPayload();
            return TRUE;
        }
        // Try again with a bigger buffer.
        bufSize *= 2;
    }
}

/**
 * This differs from realpath(3) mainly in its behavior when a path element does not exist or can
 * not be searched. realpath(3) treats that as an error and gives up, but we have Java-compatible
 * behavior where we just assume the path element was not a symbolic link. This leads to a textual
 * treatment of ".." from that point in the path, which may actually lead us back to a path we
 * can resolve (as in "/tmp/does-not-exist/../blah.txt" which would be an error for realpath(3)
 * but "/tmp/blah.txt" under the traditional Java interpretation).
 *
 * This implementation also removes all the fixed-length buffers of the C original.
 */
static Boolean _Realpath(const char* path, String& resolved)
{
    // 'path' must be an absolute path.
    if (path[0] != '/') {
        errno = EINVAL;
        return false;
    }

    resolved = String("/");
    if (path[1] == '\0') {
        return true;
    }

    // Iterate over path components in 'left'.
    Int32 symlinkCount = 0;
    String left(path + 1);
    while (!left.IsNullOrEmpty()) {
        // Extract the next path component.
        Int32 nextSlash = left.IndexOf('/');
        String nextPathComponent;
        if (nextSlash != -1) {
            nextPathComponent = left.Substring(0, nextSlash);
            left = left.Substring(nextSlash + 1, left.GetLength());
        }
        else {
            nextPathComponent = left;
            left = NULL;
        }

        if (nextPathComponent.IsNullOrEmpty()) {
            continue;
        }
        else if (!nextPathComponent.Compare(".")) {
            continue;
        }
        else if (!nextPathComponent.Compare("..")) {
            // Strip the last path component except when we have single "/".
            if (resolved.GetLength() > 1) {
                Int32 rIndex = resolved.LastIndexOf('/');
                resolved = resolved.Substring(0, rIndex);
            }
            continue;
        }

        // Append the next path component.
        if (resolved.GetChar(resolved.GetLength() - 1) != '/') {
            resolved += String("/");
        }
        resolved += nextPathComponent;

        // See if we've got a symbolic link, and resolve it if so.
        struct stat sb;
        if (lstat(resolved.string(), &sb) == 0 && S_ISLNK(sb.st_mode)) {
            if (symlinkCount++ > MAXSYMLINKS) {
                errno = ELOOP;
                return false;
            }

            String symlink;
            if (!_Readlink(resolved.string(), symlink)) {
                return false;
            }
            if (symlink.GetChar(0) == '/') {
                // The symbolic link is absolute, so we need to start from scratch.
                resolved = "/";
            }
            else if (resolved.GetLength() > 1) {
                // The symbolic link is relative, so we just lose the last path component (which
                // was the link).
                Int32 rIndex = resolved.LastIndexOf('/');
                resolved = resolved.Substring(0, rIndex);
            }

            if (!left.IsNullOrEmpty()) {
                const char* maybeSlash = (symlink.GetChar(symlink.GetLength() - 1) != '/') ? "/" : "";
                left = symlink + maybeSlash + left;
            }
            else {
                left = symlink;
            }
        }
    }

    Int32 charCount = resolved.GetLength();
    // Remove trailing slash except when the resolved pathname is a single "/".
    if (charCount > 1 && resolved.GetChar(charCount - 1) == '/') {
        resolved = resolved.Substring(0, charCount - 1);
    }

    return TRUE;
}

namespace Elastos {
namespace IO {

//static {
//    // The default protection domain grants access to these properties.
//    separatorChar = System.getProperty("file.separator", "/").charAt(0);
//    pathSeparatorChar = System.getProperty("path.separator", ":").charAt(0);
//    separator = String.valueOf(separatorChar);
//    pathSeparator = String.valueOf(pathSeparatorChar);
//}

const Char32 File::sSeparatorChar = '/';
const Char32 File::sPathSeparatorChar = ':';
const String File::sSeparator = String("/");
const String File::sPathSeparator = String(":");

AutoPtr<IRandom> File::mTempFileRandom ;

AutoPtr<IRandom> File::GetTempFileRandom()
{
    if (mTempFileRandom == NULL) {
        AutoPtr<CRandom> random;
        CRandom::NewByFriend((CRandom**)&random);
        mTempFileRandom = (IRandom*)random.Get();
    }

    return mTempFileRandom;
}

File::File()
{}

File::~File()
{}

ECode File::Init(
    /* [in] */ IFile* dir,
    /* [in] */ const String& name)
{
    String path;
    if (dir != NULL) {
        dir->GetPath(&path);
    }
    return Init(path, name);
}

ECode File::Init(
    /* [in] */ const String& path)
{
    mPath = FixSlashes(path);
    return NOERROR;
}

ECode File::Init(
    /* [in] */ const String& dirPath,
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        // throw new NullPointerException("name == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    if (dirPath.IsNullOrEmpty()) {
        mPath = FixSlashes(name);
    }
    else if (name.IsEmpty()) {
        mPath = FixSlashes(dirPath);
    }
    else {
        mPath = FixSlashes(Join(dirPath, name));
    }
    return NOERROR;
}

ECode File::Init(
    /* [in] */ IURI* uri)
{
     // check pre-conditions
     FAIL_RETURN(CheckURI(uri));
     String path;
     uri->GetPath(&path);
     mPath = FixSlashes(path);
     return NOERROR;
}

String File::FixSlashes(
    /* [in] */ const String& origPath)
{
    // Remove duplicate adjacent slashes.
    Boolean lastWasSlash = FALSE;
    AutoPtr< ArrayOf<Char32> > newPath = origPath.GetChars();
    Int32 length = newPath->GetLength();
    Int32 newLength = 0;
    Char32 ch;
    for (Int32 i = 0; i < length; ++i) {
        ch = (*newPath)[i];
        if (ch == '/') {
            if (!lastWasSlash) {
                (*newPath)[newLength++] = sSeparatorChar;
                lastWasSlash = TRUE;
            }
        }
        else {
            (*newPath)[newLength++] = ch;
            lastWasSlash = FALSE;
        }
    }
    // Remove any trailing slash (unless this is the root of the file system).
    if (lastWasSlash && newLength > 1) {
        newLength--;
    }
    // Reuse the original string if possible.
    return (newLength != length) ? String(*newPath.Get(), 0, newLength) : origPath;
}

String File::Join(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix)
{
    Int32 prefixLength = prefix.GetLength();
    Boolean haveSlash = (prefixLength > 0 && prefix.GetChar(prefixLength - 1) == sSeparatorChar);
    if (!haveSlash) {
        haveSlash = (suffix.GetLength() > 0 && suffix.GetChar(0) == sSeparatorChar);
    }
    String joinStr = prefix;
    if (!haveSlash) joinStr.AppendFormat("%c", sSeparatorChar);
    joinStr += suffix;
    return joinStr;
}

ECode File::CheckURI(
    /* [in] */ IURI* uri)
{
    Boolean isAbsolute = FALSE;
    uri->IsAbsolute(&isAbsolute);
    String schemeSpecific;
    uri->GetSchemeSpecificPart(&schemeSpecific);
    if (!isAbsolute) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    else if (!schemeSpecific.StartWith("/")) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String scheme;
    uri->GetScheme(&scheme);
    if (!String("file").Equals(scheme)) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String rawPath;
    uri->GetRawPath(&rawPath);
    if (rawPath.IsNull() || rawPath.IsEmpty()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String authority;
    uri->GetRawAuthority(&authority);
    if (!authority.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String query;
    uri->GetRawQuery(&query);
    if (!query.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    String fragment;
    uri->GetRawFragment(&fragment);
    if (!fragment.IsNull()) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    return NOERROR;
}

ECode File::ListRoots(
    /* [out, callee] */ ArrayOf<IFile*>** roots)
{
    assert(roots != NULL);
    AutoPtr<IFile> file;
    CFile::New(String("/"), (IFile**)&file);
    AutoPtr< ArrayOf<IFile*> > files = ArrayOf<IFile*>::Alloc(1);
    files->Set(0, file);
    *roots = files;
    INTERFACE_ADDREF(*roots);
    return NOERROR;
}

ECode File::CanExecute(
    /* [out] */ Boolean* canExecute)
{
    assert(canExecute != NULL);

    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mode;
    osConstans->GetOsConstant(String("X_OK"), &mode);
    *canExecute = DoAccess(mode);
    return NOERROR;
}

ECode File::CanRead(
    /* [out] */ Boolean* canRead)
{
    assert(canRead != NULL);

    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mode;
    osConstans->GetOsConstant(String("R_OK"), &mode);
    *canRead = DoAccess(mode);
    return NOERROR;
}

ECode File::CanWrite(
    /* [out] */ Boolean* canWrite)
{
    assert(canWrite != NULL);

    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mode;
    osConstans->GetOsConstant(String("W_OK"), &mode);
    *canWrite = DoAccess(mode);
    return NOERROR;
}

Boolean File::DoAccess(
    /* [in] */ Int32 mode)
{
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    Boolean result;
    if (SUCCEEDED(IoUtils::Libcore2IoECode(os->Access(mPath, mode, &result)))) {
        return result;
    }
    return FALSE;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode File::CompareTo(
    /* [in] */ IInterface* obj,
    /* [out] */ Int32* result)
{
    assert(result != NULL);
    String anotherPath;
    if (IFile::Probe(obj) == NULL) return NOERROR;

    IFile::Probe(obj)->GetPath(&anotherPath);
    //todo: the result of String.Compare() maybe not correct;
    *result = mPath.Compare(anotherPath);
    return NOERROR;
}

ECode File::CompareToEx(
    /* [in] */ IFile* another,
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result)

    String anotherPath;
    if (IFile::Probe(another) == NULL) return E_INVALID_ARGUMENT;
    IFile::Probe(another)->GetPath(&anotherPath);
    //todo: the result of String.Compare() maybe not correct;
    *result = mPath.Compare(anotherPath);
    return NOERROR;
}

ECode File::Delete(
    /* [out] */ Boolean* succeeded)
{
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    if (SUCCEEDED(IoUtils::Libcore2IoECode(os->Remove(mPath)))) {
        *succeeded = TRUE;
        return NOERROR;
    }
    *succeeded = FALSE;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode File::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* isEqual)
{
    VALIDATE_NOT_NULL(isEqual);
    *isEqual = FALSE;
    VALIDATE_NOT_NULL(obj);

    if (IFile::Probe(obj) == NULL) return NOERROR;

    String path;
    IFile::Probe(obj)->GetPath(&path);
    *isEqual = mPath.Equals(path);
    return NOERROR;
}

ECode File::Exists(
    /* [out] */ Boolean* isExist)
{
    assert(isExist != NULL);

    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mode;
    osConstans->GetOsConstant(String("F_OK"), &mode);
    *isExist = DoAccess(mode);
    return NOERROR;
}

ECode File::GetAbsolutePath(
    /* [out] */ String* path)
{
    VALIDATE_NOT_NULL(path);
    *path = String(NULL);

    Boolean isAbsolute;
    if (IsAbsolute(&isAbsolute), isAbsolute) {
        *path = mPath;
        return NOERROR;
    }

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

    String userDir;
    system->GetProperty(String("user.dir"), &userDir);
    *path = mPath.IsNullOrEmpty() ? userDir : Join(userDir, mPath);
    return NOERROR;
}

ECode File::GetAbsoluteFile(
    /* [out] */ IFile** file)
{
    assert(file != NULL);

    String absolutePath;
    GetAbsolutePath(&absolutePath);
    return CFile::New(absolutePath, file);
}

ECode File::GetCanonicalPath(
    /* [out] */ String* path)
{
    assert(path != NULL);

    String absolutePath;
    GetAbsolutePath(&absolutePath);
    *path = Realpath(absolutePath);
    return NOERROR;
}

String File::Realpath(
    /* [in] */ const String& path)
{
    if (path.IsNull()) {
        return String(NULL);
    }

    String result;
    if (!_Realpath(path.string(), result)) {
        // jniThrowIOException(env, errno);
        return String(NULL);
    }
    return result;
}

String File::Readlink(
    /* [in] */ const String& path)
{
    if (path.IsNull()) {
        return String(NULL);
    }

    String result;
    if (!_Readlink(path.string(), result)) {
        // jniThrowIOException(env, errno);
        return String(NULL);
    }
    return result;
}

ECode File::GetCanonicalFile(
    /* [out] */ IFile** file)
{
    assert(file != NULL);
    String path;
    GetCanonicalPath(&path);
    return CFile::New(path, file);
}

ECode File::GetName(
    /* [out] */ String* name)
{
    assert(name != NULL);
    Int32 separatorIndex = mPath.LastIndexOf(sSeparator);
    *name = (separatorIndex < 0) ?
            mPath : mPath.Substring(separatorIndex + 1, mPath.GetLength());
    return NOERROR;
}

ECode File::GetParent(
    /* [out] */ String* parent)
{
    assert(parent != NULL);
    Int32 length = mPath.GetLength(), firstInPath = 0;
    if (sSeparatorChar == '\\' && length > 2 && mPath.GetChar(1) == ':') {
        firstInPath = 2;
    }
    Int32 index = mPath.LastIndexOf(sSeparatorChar);
    if (index == -1 && firstInPath > 0) {
        index = 2;
    }
    if (index == -1 || mPath.GetChar(length - 1) == sSeparatorChar) {
        *parent = NULL;
        return NOERROR;
    }
    if (mPath.IndexOf(sSeparatorChar) == index
            && mPath.GetChar(firstInPath) == sSeparatorChar) {
        *parent = mPath.Substring(0, index + 1);
        return NOERROR;
    }
    *parent = mPath.Substring(0, index);
    return NOERROR;
}

ECode File::GetParentFile(
    /* [out] */ IFile** file)
{
    assert(file != NULL);
    String tempParent;
    GetParent(&tempParent);
    if (tempParent.IsNull()) {
        *file = NULL;
        return NOERROR;
    }
    return CFile::New(tempParent, file);
}

ECode File::GetPath(
    /* [out] */ String* path)
{
    assert(path != NULL);
    *path = mPath;
    return NOERROR;
}

ECode File::GetHashCode(
    /* [out] */ Int32* code)
{
    assert(code != NULL);
////  return getPath().hashCode() ^ 1234321;
    *code = mPath.GetHashCode();
    return NOERROR;
}

ECode File::IsAbsolute(
    /* [out] */ Boolean* isAbsolute)
{
    assert(isAbsolute != NULL);
    *isAbsolute = mPath.GetLength() > 0 && mPath.GetChar(0) == sSeparatorChar;
    return NOERROR;
}

ECode File::IsDirectory(
    /* [out] */ Boolean* isDirectory)
{
    assert(isDirectory != NULL);

    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStat> stat;
    if (FAILED(IoUtils::Libcore2IoECode(os->Stat(mPath, (IStructStat**)&stat)))) {
        *isDirectory = FALSE;
        return NOERROR;
    }
    Int32 mode;
    stat->GetMode(&mode);
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    return osConstans->_S_ISDIR(mode, isDirectory);
    // } catch (ErrnoException errnoException) {
    //     // The RI returns false on error. (Even for errors like EACCES or ELOOP.)
    //     return false;
    // }
}

ECode File::IsFile(
    /* [out] */ Boolean* isFile)
{
    assert(isFile != NULL);
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStat> stat;
    if (FAILED(IoUtils::Libcore2IoECode(os->Stat(mPath, (IStructStat**)&stat)))) {
        *isFile = FALSE;
        return NOERROR;
    }
    Int32 mode;
    stat->GetMode(&mode);
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    return osConstans->_S_ISREG(mode, isFile);
    // } catch (ErrnoException errnoException) {
    //     // The RI returns false on error. (Even for errors like EACCES or ELOOP.)
    //     return false;
    // }
}

ECode File::IsHidden(
    /* [out] */ Boolean* isHidden)
{
    assert(isHidden != NULL);
    if (mPath.IsEmpty()) {
        *isHidden = FALSE;
        return NOERROR;
    }
    String name;
    GetName(&name);
    *isHidden = name.StartWith(".");
    return NOERROR;
}

ECode File::LastModified(
    /* [out] */ Int64* time)
{
    assert(time != NULL);
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStat> stat;
    if (FAILED(IoUtils::Libcore2IoECode(os->Stat(mPath, (IStructStat**)&stat)))) {
        *time = 0;
        return NOERROR;
    }
    Int64 mtime;
    stat->GetMtime(&mtime);
    *time = mtime * 1000LL;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     // The RI returns 0 on error. (Even for errors like EACCES or ELOOP.)
    //     return 0;
    // }
}

ECode File::SetLastModified(
    /* [in] */ Int64 time,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    if (time < 0) {
//        throw new IllegalArgumentException("time < 0");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    *succeeded = SetLastModifiedImpl(mPath, time);
    return NOERROR;
}

Boolean File::SetLastModifiedImpl(
    /* [in] */ const String& path,
    /* [in] */ Int64 ms)
{
    if (path.IsNull()) {
        return FALSE;
    }

    // We want to preserve the access time.
    struct stat sb;
    if (stat(path.string(), &sb) == -1) {
        return FALSE;
    }

    // TODO: we could get microsecond resolution with utimes(3), "legacy" though it is.
    utimbuf times;
    times.actime = sb.st_atime;
    times.modtime = static_cast<time_t>(ms / 1000);
    return (utime(path.string(), &times) == 0);
}

ECode File::SetReadOnly(
    /* [out] */ Boolean* succeeded)
{
    return SetWritable(FALSE, FALSE, succeeded);
}

ECode File::SetExecutable(
    /* [in] */ Boolean executable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mdS_IXUSR, mdS_IXGRP, mdS_IXOTH;
    osConstans->GetOsConstant(String("S_IXUSR"), &mdS_IXUSR);
    osConstans->GetOsConstant(String("S_IXGRP"), &mdS_IXGRP);
    osConstans->GetOsConstant(String("S_IXOTH"), &mdS_IXOTH);
    *succeeded = DoChmod(ownerOnly ? mdS_IXUSR : (mdS_IXUSR | mdS_IXGRP | mdS_IXOTH), executable);
    return NOERROR;
}

ECode File::SetExecutable(
    /* [in] */ Boolean executable,
    /* [out] */ Boolean* succeeded)
{
    return SetExecutable(executable, TRUE, succeeded);
}

ECode File::SetReadable(
    /* [in] */ Boolean readable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mdS_IRUSR, mdS_IRGRP, mdS_IROTH;
    osConstans->GetOsConstant(String("S_IRUSR"), &mdS_IRUSR);
    osConstans->GetOsConstant(String("S_IRGRP"), &mdS_IRGRP);
    osConstans->GetOsConstant(String("S_IROTH"), &mdS_IROTH);
    *succeeded = DoChmod(ownerOnly ? mdS_IRUSR : (mdS_IRUSR | mdS_IRGRP | mdS_IROTH), readable);
    return NOERROR;
}

ECode File::SetReadable(
    /* [in] */ Boolean readable,
    /* [out] */ Boolean* succeeded)
{
    return SetReadable(readable, TRUE, succeeded);
}

ECode File::SetWritable(
    /* [in] */ Boolean writable,
    /* [in] */ Boolean ownerOnly,
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mdS_IWUSR, mdS_IWGRP, mdS_IWOTH;
    osConstans->GetOsConstant(String("S_IWUSR"), &mdS_IWUSR);
    osConstans->GetOsConstant(String("S_IWGRP"), &mdS_IWGRP);
    osConstans->GetOsConstant(String("S_IWOTH"), &mdS_IWOTH);
    *succeeded = DoChmod(ownerOnly ? mdS_IWUSR : (mdS_IWUSR | mdS_IWGRP | mdS_IWOTH), writable);
    return NOERROR;
}

ECode File::SetWritable(
    /* [in] */ Boolean writable,
    /* [out] */ Boolean* succeeded)
{
    return SetWritable(writable, TRUE, succeeded);
}

Boolean File::DoChmod(
    /* [in] */ Int32 mask,
    /* [in] */ Boolean set)
{
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStat> stat;
    if (FAILED(IoUtils::Libcore2IoECode(os->Stat(mPath, (IStructStat**)&stat)))) {
        return FALSE;
    }
    Int32 mode;
    stat->GetMode(&mode);
    Int32 newMode = set ? (mode | mask) : (mode & ~mask);
    if (FAILED(IoUtils::Libcore2IoECode(os->Chmod(mPath, newMode)))) {
        return FALSE;
    }
    return TRUE;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode File::GetLength(
    /* [out] */ Int64* length)
{
    assert(length != NULL);
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStat> stat;
    if (FAILED(IoUtils::Libcore2IoECode(os->Stat(mPath, (IStructStat**)&stat)))) {
        *length = 0;
        return NOERROR;
    }
    Int64 size;
    stat->GetSize(&size);
    *length = size;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     // The RI returns 0 on error. (Even for errors like EACCES or ELOOP.)
    //     return 0;
    // }
}

ECode File::List(
    /* [out, callee] */ ArrayOf<String>** files)
{
    assert(files != NULL);
    AutoPtr< ArrayOf<String> > arr = ListImpl(mPath);
    *files = arr;
    INTERFACE_ADDREF(*files);
    return NOERROR;
}

// Iterates over the filenames in the given directory.
class ScopedReaddir
{
public:
    ScopedReaddir(const char* path)
    {
        mDirStream = opendir(path);
        mIsBad = (mDirStream == NULL);
    }

    ~ScopedReaddir()
    {
        if (mDirStream != NULL) {
            closedir(mDirStream);
        }
    }

    // Returns the next filename, or NULL.
    const char* next()
    {
        if (mIsBad) {
            return NULL;
        }
        dirent* result = NULL;
        int rc = readdir_r(mDirStream, &mEntry, &result);
        if (rc != 0) {
            mIsBad = true;
            return NULL;
        }
        return (result != NULL) ? result->d_name : NULL;
    }

    // Has an error occurred on this stream?
    Boolean isBad() const
    {
        return mIsBad;
    }

private:
    DIR* mDirStream;
    dirent mEntry;
    Boolean mIsBad;

    // Disallow copy and assignment.
    ScopedReaddir(const ScopedReaddir&);
    void operator=(const ScopedReaddir&);
};

typedef Vector<String> DirEntries;

// Reads the directory referred to by 'pathBytes', adding each directory entry
// to 'entries'.
static Boolean ReadDirectory(const String& path, DirEntries& entries)
{
    if (path.IsNull()) {
        return FALSE;
    }

    ScopedReaddir dir(path.string());
    const char* filename;
    while ((filename = dir.next()) != NULL) {
        if (strcmp(filename, ".") != 0 && strcmp(filename, "..") != 0) {
            // TODO: this hides allocation failures from us. Push directory iteration up into Java?
            entries.PushBack(String(filename));
        }
    }
    return !dir.isBad();
}

AutoPtr< ArrayOf<String> > File::ListImpl(
    /* [in] */ const String& path)
{
    // Read the directory entries into an intermediate form.
    DirEntries entries;
    if (!ReadDirectory(path, entries)) {
        return NULL;
    }
    // Translate the intermediate form into a Java String[].
    return Elastos::IO::ToStringArray(entries);
}

ECode File::List(
    /* [in] */ IFilenameFilter* filter,
    /* [out, callee] */ ArrayOf<String>** files)
{
    AutoPtr< ArrayOf<String> > filenames = ListImpl(mPath);
    if (filter == NULL || filenames == NULL) {
        *files = filenames;
        INTERFACE_ADDREF(*files);
        return NOERROR;
    }
    Elastos::Utility::List<String> result;
    for (Int32 i = 0; i < filenames->GetLength(); ++i) {
        String filename = (*filenames)[i];
        Boolean isAccept;
        if (filter->Accept((IFile*)this->Probe(EIID_IFile),
                filename, &isAccept), isAccept) {
            result.PushBack(filename);
        }
    }
    AutoPtr< ArrayOf<String> > _files = ArrayOf<String>::Alloc(result.GetSize());
    Elastos::Utility::List<String>::Iterator it;
    Int32 index;
    for (it = result.Begin(), index = 0; it != result.End(); ++it, ++index) {
        (*_files)[index] = *it;
    }
    *files = _files;
    INTERFACE_ADDREF(*files);
    return NOERROR;
}

ECode File::ListFiles(
    /* [out, callee] */ ArrayOf<IFile*>** files)
{
    assert(files != NULL);

    AutoPtr< ArrayOf<String> > filenames = ListImpl(mPath);
    AutoPtr< ArrayOf<IFile*> > _files = FilenamesToFiles(filenames);
    *files = _files;
    INTERFACE_ADDREF(*files);
    return NOERROR;
}

ECode File::ListFiles(
    /* [in] */ IFilenameFilter* filter,
    /* [out, callee] */ ArrayOf<IFile*>** files)
{
    assert(files != NULL);

    AutoPtr< ArrayOf<String> > filenames;
    List(filter, (ArrayOf<String>**)&filenames);
    AutoPtr< ArrayOf<IFile*> > _files = FilenamesToFiles(filenames);
    *files = _files;
    INTERFACE_ADDREF(*files);
    return NOERROR;
}

ECode File::ListFiles(
    /* [in] */ IFileFilter* filter,
    /* [out, callee] */ ArrayOf<IFile*>** files)
{
    AutoPtr< ArrayOf<IFile*> > _files;
    ListFiles((ArrayOf<IFile*>**)&_files);
    if (filter == NULL || _files == NULL) {
        *files = _files;
        INTERFACE_ADDREF(*files);
        return NOERROR;
    }
    Elastos::Utility::List< AutoPtr<IFile> > result;
    for (Int32 i = 0; i < _files->GetLength(); ++i) {
        IFile* file = (*_files)[i];
        Boolean isAccept;
        if (filter->Accept(file, &isAccept), isAccept) {
            result.PushBack(file);
        }
    }
    _files = ArrayOf<IFile*>::Alloc(result.GetSize());
    Elastos::Utility::List< AutoPtr<IFile> >::Iterator it;
    Int32 index;
    for (it = result.Begin(), index = 0; it != result.End(); ++it, ++index) {
        _files->Set(index, *it);
    }
    *files = _files;
    INTERFACE_ADDREF(*files);
    return NOERROR;
}

AutoPtr< ArrayOf<IFile*> > File::FilenamesToFiles(
    /* [in] */ ArrayOf<String>* filenames)
{
    if (filenames == NULL) {
        return NULL;
    }
    Int32 count = filenames->GetLength();
    AutoPtr< ArrayOf<IFile*> > result = ArrayOf<IFile*>::Alloc(count);
    for (Int32 i = 0; i < count; ++i) {
        AutoPtr<IFile> file;
        CFile::New((IFile*)this->Probe(EIID_IFile),
                (*filenames)[i], (IFile**)&file);
        result->Set(i, file);
    }
    return result;
}

ECode File::Mkdir(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    // try {
    *succeeded = SUCCEEDED(MkdirErrno()) ? TRUE : FALSE;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode File::MkdirErrno()
{
    // On Android, we don't want default permissions to allow global access.
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mode;
    osConstans->GetOsConstant(String("S_IRWXU"), &mode);
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    return IoUtils::Libcore2IoECode(os->Mkdir(mPath, mode));
}

ECode File::Mkdirs(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    *succeeded = Mkdirs(FALSE);
    return NOERROR;
}

Boolean File::Mkdirs(
    /* [in] */ Boolean resultIfExists)
{
    // Try to create the directory directly.
    if (SUCCEEDED(MkdirErrno())) {
        return TRUE;
    }
    else {
        // If the parent was missing, try to create it and then try again.
        AutoPtr<IFile> parent;
        if (SUCCEEDED(GetParentFile((IFile**)&parent))) {
            Boolean succeeded;
            if (parent && SUCCEEDED(parent->Mkdirs(&succeeded)) && succeeded) {
                Mkdir(&succeeded);
                return succeeded;
            }
        }
        return FALSE;
    }
}

ECode File::CreateNewFile(
    /* [out] */ Boolean* succeeded)
{
    assert(succeeded != NULL);
    // try {
    // On Android, we don't want default permissions to allow global access.
    AutoPtr<IOsConstants> osConstans;
    COsConstants::AcquireSingleton((IOsConstants**)&osConstans);
    Int32 mdO_RDWR, mdO_CREAT, mdO_EXCL;
    osConstans->GetOsConstant(String("O_RDWR"), &mdO_RDWR);
    osConstans->GetOsConstant(String("O_CREAT"), &mdO_CREAT);
    osConstans->GetOsConstant(String("O_EXCL"), &mdO_EXCL);
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    Int32 fd;
    ECode ec = IoUtils::Libcore2IoECode(os->Open(mPath, mdO_RDWR | mdO_CREAT | mdO_EXCL, 0600, &fd));
    if (fd != -1) {
        IoUtils::Libcore2IoECode(os->Close(fd));
    }
    *succeeded = SUCCEEDED(ec) ? TRUE : FALSE;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     if (errnoException.errno == EEXIST) {
    //         // The file already exists.
    //         return false;
    //     }
    //     throw errnoException.rethrowAsIOException();
    // } finally {
    //     IoUtils.close(fd); // TODO: should we suppress IOExceptions thrown here?
    // }
}

ECode File::CreateTempFile(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [out] */ IFile** file)
{
    assert(file != NULL);
    return CreateTempFile(prefix, suffix, NULL, file);
}

ECode File::CreateTempFile(
    /* [in] */ const String& prefix,
    /* [in] */ const String& suffix,
    /* [in] */ IFile* directory,
    /* [out] */ IFile** file)
{
    // Force a prefix null check first
    String _suffix = suffix;
    if (prefix.GetLength() < 3) {
        //throw new IllegalArgumentException("prefix must be at least 3 characters");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (_suffix.IsNull()) {
        _suffix = ".tmp";
    }
    AutoPtr<IFile> tmpDirFile = directory;
    if (tmpDirFile == NULL) {
        AutoPtr<ISystem> system;
        Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);

        String tmpDir;
        system->GetPropertyEx(String("elastos.io.tmpdir"), String("."), &tmpDir);
        CFile::New(tmpDir, (IFile**)&tmpDirFile);
    }
    AutoPtr<IFile> result;
    Boolean succeeded;
    AutoPtr<IRandom> random = GetTempFileRandom();
    do {
        Int32 randomInt32;
        random->NextInt32(&randomInt32);
        StringBuilder sb(prefix);
        sb += randomInt32;
        sb += _suffix;
        result = NULL;
        CFile::New(tmpDirFile, sb.ToString(), (IFile**)&result);
    } while (result->CreateNewFile(&succeeded), !succeeded);
    *file = result;
    INTERFACE_ADDREF(*file);
    return NOERROR;
}

ECode File::RenameTo(
    /* [in] */ IFile* newPath,
    /* [out] */ Boolean* succeeded)
{
    assert(newPath != NULL);
    assert(succeeded != NULL);

    // try {
    String path;
    newPath->GetPath(&path);
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    *succeeded = SUCCEEDED(IoUtils::Libcore2IoECode(os->Rename(mPath, path))) ? TRUE : FALSE;
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return false;
    // }
}

ECode File::ToString(
    /* [out] */ String* path)
{
    assert(path != NULL);
    *path = mPath;
    return NOERROR;
}

// public URI toURI() {
//     String name = getAbsoluteName();
//     try {
//         if (!name.startsWith("/")) {
//             // start with sep.
//             return new URI("file", null, "/" + name, null, null);
//         } else if (name.startsWith("//")) {
//             return new URI("file", "", name, null); // UNC path
//         }
//         return new URI("file", null, name, null, null);
//     } catch (URISyntaxException e) {
//         // this should never happen
//         return null;
//     }
// }

// public URL toURL() throws java.net.MalformedURLException {
//     String name = getAbsoluteName();
//     if (!name.startsWith("/")) {
//         // start with sep.
//         return new URL("file", "", -1, "/" + name, null);
//     } else if (name.startsWith("//")) {
//         return new URL("file:" + name); // UNC path
//     }
//     return new URL("file", "", -1, name, null);
// }

// // TODO: is this really necessary, or can it be replaced with getAbsolutePath?
// private String getAbsoluteName() {
//     File f = getAbsoluteFile();
//     String name = f.getPath();
//     if (f.isDirectory() && name.charAt(name.length() - 1) != separatorChar) {
//         // Directories must end with a slash
//         name = name + "/";
//     }
//     if (separatorChar != '/') { // Must convert slashes.
//         name = name.replace(separatorChar, '/');
//     }
//     return name;
// }

// private void writeObject(ObjectOutputStream stream) throws IOException {
//     stream.defaultWriteObject();
//     stream.writeChar(separatorChar);
// }

// private void readObject(ObjectInputStream stream) throws IOException, ClassNotFoundException {
//     stream.defaultReadObject();
//     char inSeparator = stream.readChar();
//     this.path = fixSlashes(path.replace(inSeparator, separatorChar));
// }

ECode File::GetTotalSpace(
    /* [out] */ Int64* space)
{
    assert(space != NULL);
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStatFs> sb;
    if (FAILED(IoUtils::Libcore2IoECode(os->Statfs(mPath, (IStructStatFs**)&sb)))) {
        *space = 0;
        return NOERROR;
    }
    Int64 blocks, bsize;
    sb->GetBlocks(&blocks);
    sb->GetBsize(&bsize);
    *space = blocks * bsize; // total block count * block size in bytes.
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return 0;
    // }
}

ECode File::GetUsableSpace(
    /* [out] */ Int64* space)
{
    assert(space != NULL);
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStatFs> sb;
    if (FAILED(IoUtils::Libcore2IoECode(os->Statfs(mPath, (IStructStatFs**)&sb)))) {
        *space = 0;
        return NOERROR;
    }
    Int64 bavail, bsize;
    sb->GetBavail(&bavail);
    sb->GetBsize(&bsize);
    *space = bavail * bsize; // non-root free block count * block size in bytes.
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return 0;
    // }
}

ECode File::GetFreeSpace(
    /* [out] */ Int64* space)
{
    assert(space != NULL);
    // try {
    AutoPtr<ILibcore> libcore;
    CLibcore::AcquireSingleton((ILibcore**)&libcore);
    AutoPtr<IOs> os;
    libcore->GetOs((IOs**)&os);
    AutoPtr<IStructStatFs> sb;
    if (FAILED(IoUtils::Libcore2IoECode(os->Statfs(mPath, (IStructStatFs**)&sb)))) {
        *space = 0;
        return NOERROR;
    }
    Int64 bfree, bsize;
    sb->GetBfree(&bfree);
    sb->GetBsize(&bsize);
    *space = bfree * bsize; // free block count * block size in bytes.
    return NOERROR;
    // } catch (ErrnoException errnoException) {
    //     return 0;
    // }
}

} // namespace IO
} // namespace Elastos
