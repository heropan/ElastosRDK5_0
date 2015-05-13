#ifndef __CFILE_H__
#define __CFILE_H__

#include "_CFile.h"
#include "File.h"

namespace Elastos {
namespace IO {

CarClass(CFile)
    , public File
{
public:
    CARAPI constructor(
        /* [in] */ IFile* dir,
        /* [in] */ const String& name);

    CARAPI constructor(
        /* [in] */ const String& path);

    CARAPI constructor(
        /* [in] */ const String& dirPath,
        /* [in] */ const String& name);

    CARAPI constructor(
       /* [in] */ IURI* uri);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI CanExecute(
        /* [out] */ Boolean* canExecute);

    CARAPI CanRead(
        /* [out] */ Boolean* canRead);

    CARAPI CanWrite(
        /* [out] */ Boolean* canWrite);

    CARAPI CompareTo(
        /* [in] */ IInterface* obj,
        /* [out] */ Int32* result);

    CARAPI CompareToEx(
        /* [in] */ IFile* another,
        /* [out] */ Int32* result);

    CARAPI CreateNewFile(
        /* [out] */ Boolean* succeeded);

    CARAPI Delete(
        /* [out] */ Boolean* isDeleted);

    CARAPI Equals(
        /* [in] */ IInterface* obj,
        /* [out] */ Boolean* isEqual);

    CARAPI Exists(
        /* [out] */ Boolean* isExist);

    CARAPI GetAbsoluteFile(
        /* [out] */ IFile** file);

    CARAPI GetAbsolutePath(
        /* [out] */ String* path);

    CARAPI GetCanonicalFile(
        /* [out] */ IFile** file);

    CARAPI GetCanonicalPath(
        /* [out] */ String* path);

    CARAPI GetFreeSpace(
        /* [out] */ Int64* space);

    CARAPI GetLength(
        /* [out] */ Int64* length);

    CARAPI GetName(
        /* [out] */ String* name);

    CARAPI GetParent(
        /* [out] */ String* parent);

    CARAPI GetParentFile(
        /* [out] */ IFile** file);

    CARAPI GetPath(
        /* [out] */ String* path);

    CARAPI GetTotalSpace(
        /* [out] */ Int64* space);

    CARAPI GetUsableSpace(
        /* [out] */ Int64* space);

    CARAPI GetHashCode(
        /* [out] */ Int32* code);

    CARAPI IsAbsolute(
        /* [out] */ Boolean* isAbsolute);

    CARAPI IsDirectory(
        /* [out] */ Boolean* isDirectory);

    CARAPI IsFile(
        /* [out] */ Boolean* isFile);

    CARAPI IsHidden(
        /* [out] */ Boolean* isHidden);

    CARAPI LastModified(
        /* [out] */ Int64* time);

    CARAPI List(
        /* [out, callee] */ ArrayOf<String>** files);

    CARAPI ListEx(
        /* [in] */ IFilenameFilter* filter,
        /* [out, callee] */ ArrayOf<String>** files);

    CARAPI ListFiles(
        /* [out, callee] */ ArrayOf<IFile*>** files);

    CARAPI ListFilesEx(
        /* [in] */ IFilenameFilter* filter,
        /* [out, callee] */ ArrayOf<IFile*>** files);

    CARAPI ListFilesEx2(
        /* [in] */ IFileFilter* filter,
        /* [out, callee] */ ArrayOf<IFile*>** files);

    CARAPI Mkdir(
        /* [out] */ Boolean* succeeded);

    CARAPI Mkdirs(
        /* [out] */ Boolean* succeeded);

    CARAPI RenameTo(
        /* [in] */ IFile* newPath,
        /* [out] */ Boolean* succeeded);

    CARAPI SetLastModified(
        /* [in] */ Int64 time,
        /* [out] */ Boolean* succeeded);

    CARAPI SetReadOnly(
        /* [out] */ Boolean* succeeded);

    CARAPI SetExecutable(
        /* [in] */ Boolean executable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    CARAPI SetExecutableEx(
        /* [in] */ Boolean executable,
        /* [out] */ Boolean* succeeded);

    CARAPI SetReadable(
        /* [in] */ Boolean readable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    CARAPI SetReadableEx(
        /* [in] */ Boolean readable,
        /* [out] */ Boolean* succeeded);

    CARAPI SetWritable(
        /* [in] */ Boolean writable,
        /* [in] */ Boolean ownerOnly,
        /* [out] */ Boolean* succeeded);

    CARAPI SetWritableEx(
        /* [in] */ Boolean writable,
        /* [out] */ Boolean* succeeded);

    CARAPI ToString(
        /* [out] */ String* str);

//    CARAPI ToURI(
//        /* [out] */ IURI** uri);

//    CARAPI ToURL(
//        /* [out] */ IURL** uri);
};

} // namespace IO
} // namespace Elastos

#endif // __CFILE_H__
