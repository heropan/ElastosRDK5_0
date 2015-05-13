
#include "cmdef.h"
#include "CZipFile.h"

namespace Elastos {
namespace Utility {
namespace Zip {


ECode CZipFile::Close()
{
    return ZipFile::Close();
}

ECode CZipFile::GetEntries(
    /* [out] */ IObjectContainer** entries)
{
    VALIDATE_NOT_NULL(entries);
    return ZipFile::GetEntries(entries);
}

ECode CZipFile::GetEntry(
    /* [in] */ const String& entryName,
    /* [out] */ IZipEntry** entry)
{
    VALIDATE_NOT_NULL(entry);
    return ZipFile::GetEntry(entryName, entry);
}

ECode CZipFile::GetInputStream(
    /* [in] */ IZipEntry* entry,
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is);
    return ZipFile::GetInputStream(entry, is);
}

ECode CZipFile::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    return ZipFile::GetName(name);
}

ECode CZipFile::GetSize(
    /* [out] */ Int32* size)
{
    VALIDATE_NOT_NULL(size);
    return ZipFile::GetSize(size);
}

ECode CZipFile::constructor(
    /* [in] */ IFile* file)
{
    return ZipFile::Init(file);
}

ECode CZipFile::constructor(
    /* [in] */ IFile* file,
    /* [in] */ Int32 mode)
{
    return ZipFile::Init(file, mode);
}

ECode CZipFile::constructor(
    /* [in] */ const String& name)
{
    return ZipFile::Init(name);
}


} // namespace Zip
} // namespace Utility
} // namespace Elastos
