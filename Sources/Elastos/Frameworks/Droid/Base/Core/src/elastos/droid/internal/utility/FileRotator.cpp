
#include "util/FileRotator.h"
#include "os/FileUtils.h"
#include <elastos/StringUtils.h>
#include <elastos/Math.h>
#include <elastos/StringBuilder.h>
#include <elastos/Slogger.h>

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::IO::IStreams;
using Elastos::IO::CStreams;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::CFileOutputStream;
using Elastos::IO::IBufferedOutputStream;
using Elastos::IO::CBufferedOutputStream;
using Elastos::IO::IFlushable;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Zip::IZipOutputStream;
using Elastos::Utility::Zip::CZipOutputStream;
using Elastos::Utility::Zip::IZipEntry;
using Elastos::Utility::Zip::CZipEntry;
using Elastos::Droid::Os::FileUtils;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Utility {

FileRotator::FileInfo::FileInfo(
    /* [in] */ const String& prefix)
    : mStartMillis(0)
    , mEndMillis(0)
{
    assert(!prefix.IsNull());
    mPrefix = prefix;
}

Boolean FileRotator::FileInfo::Parse(
    /* [in] */ const String& name)
{
    mStartMillis = mEndMillis = -1;

    Int32 dotIndex = name.LastIndexOf('.');
    Int32 dashIndex = name.LastIndexOf('-');

    // skip when missing time section
    if (dotIndex == -1 || dashIndex == -1) return FALSE;

    // skip when prefix doesn't match
    if (!mPrefix.Equals(name.Substring(0, dotIndex))) return FALSE;

    // try {
    if (FAILED(StringUtils::ParseInt64(name.Substring(dotIndex + 1, dashIndex), &mStartMillis))) {
        return FALSE;
    }

    if (name.GetLength() - dashIndex == 1) {
        mEndMillis = Elastos::Core::Math::INT64_MAX_VALUE;
    }
    else {
        if (FAILED(StringUtils::ParseInt64(name.Substring(dashIndex + 1), &mEndMillis))) {
            return FALSE;
        }
    }

    return TRUE;
    // } catch (NumberFormatException e) {
    //     return false;
    // }
}

String FileRotator::FileInfo::Build()
{
    StringBuilder name(mPrefix);
    name.AppendChar('.');
    name.AppendInt64(mStartMillis);
    name.AppendChar('-');
    if (mEndMillis != Elastos::Core::Math::INT64_MAX_VALUE) {
        name.AppendInt64(mEndMillis);
    }
    return name.ToString();
}

Boolean FileRotator::FileInfo::IsActive()
{
    return mEndMillis == Elastos::Core::Math::INT64_MAX_VALUE;
}


FileRotator::CombineActiveRewriter::CombineActiveRewriter(
    /* [in] */ Reader* reader,
    /* [in] */ Writer* writer)
    : mReader(reader)
    , mWriter(writer)
{}

UInt32 FileRotator::CombineActiveRewriter::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 FileRotator::CombineActiveRewriter::Release()
{
    return ElRefBase::Release();
}

PInterface FileRotator::CombineActiveRewriter::Probe(
    /* [in] */ REIID riid)
{
    return NULL;
}

ECode FileRotator::CombineActiveRewriter::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return NOERROR;
}

ECode FileRotator::CombineActiveRewriter::Reset()
{
    // ignored
    return NOERROR;
}

ECode FileRotator::CombineActiveRewriter::Read(
    /* [in] */ IInputStream* in)
{
    return mReader->Read(in);
}

ECode FileRotator::CombineActiveRewriter::ShouldWrite(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = TRUE;
    return NOERROR;
}

ECode FileRotator::CombineActiveRewriter::Write(
    /* [in] */ IOutputStream* out)
{
    return mWriter->Write(out);
}


const String FileRotator::TAG("FileRotator");
const Boolean FileRotator::LOGD;
const String FileRotator::SUFFIX_BACKUP(".backup");
const String FileRotator::SUFFIX_NO_BACKUP(".no_backup");

FileRotator::FileRotator(
    /* [in] */ IFile* basePath,
    /* [in] */ const String& prefix,
    /* [in] */ Int64 rotateAgeMillis,
    /* [in] */ Int64 deleteAgeMillis)
    : mRotateAgeMillis(rotateAgeMillis)
    , mDeleteAgeMillis(deleteAgeMillis)
{
    assert(basePath != NULL);
    mBasePath = basePath;
    assert(!prefix.IsNull());
    mPrefix = prefix;

    // ensure that base path exists
    Boolean result;
    mBasePath->Mkdirs(&result);

    // recover any backup files
    AutoPtr< ArrayOf<String> > files;
    ASSERT_SUCCEEDED(mBasePath->List((ArrayOf<String>**)&files))
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[0];
        if (!name.StartWith(mPrefix)) continue;

        if (name.EndWith(SUFFIX_BACKUP)) {
            if (LOGD) Slogger::D(TAG, "recovering %s", name.string());

            AutoPtr<IFile> backupFile, file;
            CFile::New(mBasePath, name, (IFile**)&backupFile);
            CFile::New(mBasePath, name.Substring(0, name.GetLength() - SUFFIX_BACKUP.GetLength()), (IFile**)&file);

            // write failed with backup; recover last file
            Boolean result;
            backupFile->RenameTo(file, &result);
        }
        else if (name.EndWith(SUFFIX_NO_BACKUP)) {
            if (LOGD) Slogger::D(TAG, "recovering %s", name.string());

            AutoPtr<IFile> noBackupFile, file;
            CFile::New(mBasePath, name, (IFile**)&noBackupFile);
            CFile::New(mBasePath, name.Substring(0, name.GetLength() - SUFFIX_NO_BACKUP.GetLength()), (IFile**)&file);

            // write failed without backup; delete both
            Boolean result;
            noBackupFile->Delete(&result);
            file->Delete(&result);
        }
    }
}

void FileRotator::DeleteAll()
{
    AutoPtr<FileInfo> info = new FileInfo(mPrefix);
    AutoPtr< ArrayOf<String> > files;
    ASSERT_SUCCEEDED(mBasePath->List((ArrayOf<String>**)&files))
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[i];
        if (info->Parse(name)) {
            // delete each file that matches parser
            AutoPtr<IFile> file;
            CFile::New(mBasePath, name, (IFile**)&file);
            Boolean result;
            file->Delete(&result);
        }
    }
}

ECode FileRotator::DumpAll(
    /* [in] */ IOutputStream* os)
{
    AutoPtr<IZipOutputStream> zos;
    CZipOutputStream::New(os, (IZipOutputStream**)&zos);
    // try {
    AutoPtr<FileInfo> info = new FileInfo(mPrefix);
    AutoPtr< ArrayOf<String> > files;
    ECode ec = mBasePath->List((ArrayOf<String>**)&files);
    FAIL_GOTO(ec, failed)
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[i];
        if (info->Parse(name)) {
            AutoPtr<IZipEntry> entry;
            CZipEntry::New(name, (IZipEntry**)&entry);
            ec = zos->PutNextEntry(entry);
            FAIL_GOTO(ec, failed);

            AutoPtr<IFile> file;
            CFile::New(mBasePath, name, (IFile**)&file);
            AutoPtr<IFileInputStream> is;
            CFileInputStream::New(file, (IFileInputStream**)&is);
            // try {
            AutoPtr<IStreams> stream;
            CStreams::AcquireSingleton((IStreams**)&stream);
            Int32 result;
            stream->Copy(is, zos, &result);
            AutoPtr<IIoUtils> ioutils;
            CIoUtils::AcquireSingleton((IIoUtils**)&ioutils);
            ioutils->CloseQuietly(is);
            // } finally {
            //     IoUtils.closeQuietly(is);
            // }

            zos->CloseEntry();
        }
    }
    // } finally {
    //     IoUtils.closeQuietly(zos);
    // }
failed:
    AutoPtr<IIoUtils> ioutils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioutils);
    ioutils->CloseQuietly(zos);
    return ec;
}

ECode FileRotator::RewriteActive(
    /* [in] */ Rewriter* rewriter,
    /* [in] */ Int64 currentTimeMillis)
{
    String activeName = GetActiveName(currentTimeMillis);
    return RewriteSingle(rewriter, activeName);
}

ECode FileRotator::CombineActive(
    /* [in] */ Reader* reader,
    /* [in] */ Writer* writer,
    /* [in] */ Int64 currentTimeMillis)
{
    AutoPtr<CombineActiveRewriter> rewriter = new CombineActiveRewriter(reader, writer);
    return RewriteActive(rewriter, currentTimeMillis);
}

ECode FileRotator::RewriteAll(
    /* [in] */ Rewriter* rewriter)
{
    AutoPtr<FileInfo> info = new FileInfo(mPrefix);
    AutoPtr< ArrayOf<String> > files;
    FAIL_RETURN(mBasePath->List((ArrayOf<String>**)&files))
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[i];
        if (!info->Parse(name)) continue;

        // process each file that matches parser
        FAIL_RETURN(RewriteSingle(rewriter, name))
    }
    return NOERROR;
}

ECode FileRotator::RewriteSingle(
    /* [in] */ Rewriter* rewriter,
    /* [in] */ const String& name)
{
    if (LOGD) Slogger::D(TAG, "rewriting %s", name.string());

    AutoPtr<IFile> file;
    CFile::New(mBasePath, name, (IFile**)&file);
    AutoPtr<IFile> backupFile;

    rewriter->Reset();

    Boolean isExist;
    if (file->Exists(&isExist), isExist) {
        // read existing data
        FAIL_RETURN(ReadFile(file, rewriter))

        // skip when rewriter has nothing to write
        Boolean shouldWrite;
        if (rewriter->ShouldWrite(&shouldWrite), !shouldWrite) return NOERROR;

        // backup existing data during write
        CFile::New(mBasePath, name + SUFFIX_BACKUP, (IFile**)&backupFile);
        Boolean result;
        file->RenameTo(backupFile, &result);

        // try {
        ECode ec = WriteFile(file, rewriter);
        if (FAILED(ec)) {
            // write failed, delete file and restore backup
            file->Delete(&result);
            backupFile->RenameTo(file, &result);
            return E_IO_EXCEPTION;
        }

        // write success, delete backup
        backupFile->Delete(&result);
        // } catch (Throwable t) {
        //     // write failed, delete file and restore backup
        //     file.delete();
        //     backupFile.renameTo(file);
        //     throw rethrowAsIoException(t);
        // }
    }
    else {
        // create empty backup during write
        CFile::New(mBasePath, name + SUFFIX_NO_BACKUP, (IFile**)&backupFile);
        Boolean result;
        backupFile->CreateNewFile(&result);

        // try {
        ECode ec = WriteFile(file, rewriter);
        if (FAILED(ec)) {
            // write failed, delete file and restore backup
            file->Delete(&result);
            backupFile->Delete(&result);
            return E_IO_EXCEPTION;
        }

        // write success, delete empty backup
        backupFile->Delete(&result);
        // } catch (Throwable t) {
        //     // write failed, delete file and empty backup
        //     file.delete();
        //     backupFile.delete();
        //     throw rethrowAsIoException(t);
        // }
    }
    return NOERROR;
}

ECode FileRotator::ReadMatching(
    /* [in] */ Reader* reader,
    /* [in] */ Int64 matchStartMillis,
    /* [in] */ Int64 matchEndMillis)
{
    AutoPtr<FileInfo> info = new FileInfo(mPrefix);
    AutoPtr< ArrayOf<String> > files;
    FAIL_RETURN(mBasePath->List((ArrayOf<String>**)&files))
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[i];
        if (!info->Parse(name)) continue;

        // read file when it overlaps
        if (info->mStartMillis <= matchEndMillis && matchStartMillis <= info->mEndMillis) {
            if (LOGD) Slogger::D(TAG, "reading matching %s", name.string());

            AutoPtr<IFile> file;
            CFile::New(mBasePath, name, (IFile**)&file);
            FAIL_RETURN(ReadFile(file, reader))
        }
    }
    return NOERROR;
}

String FileRotator::GetActiveName(
    /* [in] */ Int64 currentTimeMillis)
{
    String oldestActiveName;
    Int64 oldestActiveStart = Elastos::Core::Math::INT64_MAX_VALUE;

    AutoPtr<FileInfo> info = new FileInfo(mPrefix);
    AutoPtr< ArrayOf<String> > files;
    ASSERT_SUCCEEDED(mBasePath->List((ArrayOf<String>**)&files))
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[i];
        if (!info->Parse(name)) continue;

        // pick the oldest active file which covers current time
        if (info->IsActive() && info->mStartMillis < currentTimeMillis
                && info->mStartMillis < oldestActiveStart) {
            oldestActiveName = name;
            oldestActiveStart = info->mStartMillis;
        }
    }

    if (!oldestActiveName.IsNull()) {
        return oldestActiveName;
    }
    else {
        // no active file found above; create one starting now
        info->mStartMillis = currentTimeMillis;
        info->mEndMillis = Elastos::Core::Math::INT64_MAX_VALUE;
        return info->Build();
    }
}

void FileRotator::MaybeRotate(
    /* [in] */ Int64 currentTimeMillis)
{
    Int64 rotateBefore = currentTimeMillis - mRotateAgeMillis;
    Int64 deleteBefore = currentTimeMillis - mDeleteAgeMillis;

    AutoPtr<FileInfo> info = new FileInfo(mPrefix);
    AutoPtr< ArrayOf<String> > files;
    ASSERT_SUCCEEDED(mBasePath->List((ArrayOf<String>**)&files))
    for (Int32 i = 0; i < files->GetLength(); ++i) {
        String name = (*files)[i];
        if (!info->Parse(name)) continue;

        if (info->IsActive()) {
            if (info->mStartMillis <= rotateBefore) {
                // found active file; rotate if old enough
                if (LOGD) Slogger::D(TAG, "rotating %s", name.string());

                info->mEndMillis = currentTimeMillis;

                AutoPtr<IFile> file;
                CFile::New(mBasePath, name, (IFile**)&file);
                AutoPtr<IFile> destFile;
                CFile::New(mBasePath, info->Build(), (IFile**)&destFile);
                Boolean result;
                file->RenameTo(destFile, &result);
            }
        }
        else if (info->mEndMillis <= deleteBefore) {
            // found rotated file; delete if old enough
            if (LOGD) Slogger::D(TAG, "deleting %s", name.string());

            AutoPtr<IFile> file;
            CFile::New(mBasePath, name, (IFile**)&file);
            Boolean result;
            file->Delete(&result);
        }
    }
}

ECode FileRotator::ReadFile(
    /* [in] */ IFile* file,
    /* [in] */ Reader* reader)
{
    AutoPtr<IFileInputStream> fis;
    CFileInputStream::New(file, (IFileInputStream**)&fis);
    AutoPtr<IBufferedInputStream> bis;
    CBufferedInputStream::New(fis, (IBufferedInputStream**)&bis);
    // try {
    ECode ec = reader->Read(IInputStream::Probe(bis));
    // } finally {
    AutoPtr<IIoUtils> ioutils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioutils);
    ioutils->CloseQuietly(bis);
    // }
    return ec;
}

ECode FileRotator::WriteFile(
    /* [in] */ IFile* file,
    /* [in] */ Writer* writer)
{
    AutoPtr<IFileOutputStream> fos;
    CFileOutputStream::New(file, (IFileOutputStream**)&fos);
    AutoPtr<IBufferedOutputStream> bos;
    CBufferedOutputStream::New(fos, (IBufferedOutputStream**)&bos);
    // try {
    ECode ec = writer->Write(bos);
    FAIL_GOTO(ec, failed)
    IFlushable::Probe(bos)->Flush();
    // } finally {
    //     FileUtils.sync(fos);
    //     IoUtils.closeQuietly(bos);
    // }
failed:
    Boolean result;
    FileUtils::Sync(fos, &result);
    AutoPtr<IIoUtils> ioutils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioutils);
    ioutils->CloseQuietly(bos);
    return ec;
}

} // namespace Utility
} // namespace Internal
} // namespace Droid
} // namespace Elastos
