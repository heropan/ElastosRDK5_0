
#include "CZipFile.h"

namespace Elastos {
namespace Utility {
namespace Zip {

CAR_OBJECT_IMPL(CZipFile)

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
