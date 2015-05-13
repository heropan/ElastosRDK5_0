
#include "ext/frameworkext.h"
#include "CByteArrayPartSource.h"

using Elastos::IO::CByteArrayInputStream;
using Elastos::IO::IInputStream;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

ECode CByteArrayPartSource::constructor(
    /* [in] */ const String& fileName,
    /* [in] */ ArrayOf<Byte>* bytes)
{
    mFileName = fileName;
    mBytes = bytes;
    return NOERROR;
}

ECode CByteArrayPartSource::GetLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);
    *length = mBytes->GetLength();
    return NOERROR;
}

ECode CByteArrayPartSource::GetFileName(
    /* [out] */ String* fileName)
{
    VALIDATE_NOT_NULL(fileName);
    *fileName = mFileName;
    return NOERROR;
}

ECode CByteArrayPartSource::CreateInputStream(
    /* [out] */ IInputStream** stream)
{
    return CByteArrayInputStream::New(mBytes, stream);
}

}
}
}
}
}
}

