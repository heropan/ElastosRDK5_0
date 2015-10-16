
#include "PartBase.h"
#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/ext/frameworkext.h"

using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

ECode PartBase::Init(
    /* [in] */ const String& name,
    /* [in] */ const String& contentType,
    /* [in] */ const String& charSet,
    /* [in] */ const String& transferEncoding)
{

    mName = name;
    mContentType = contentType;
    mCharSet = charSet;
    mTransferEncoding = transferEncoding;

    if (name.IsNullOrEmpty()) {
        Logger::E(String("PartBase"), String("Name must not be null"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    return NOERROR;
}

ECode PartBase::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode PartBase::GetContentType(
    /* [out] */ String* contentType)
{
    VALIDATE_NOT_NULL(contentType);
    *contentType = mContentType;
    return NOERROR;
}

ECode PartBase::GetCharSet(
    /* [out] */ String* charSet)
{
    VALIDATE_NOT_NULL(charSet);
    *charSet = mCharSet;
    return NOERROR;
}

ECode PartBase::GetTransferEncoding(
    /* [out] */ String* transferEncoding)
{
    VALIDATE_NOT_NULL(transferEncoding);
    *transferEncoding = mTransferEncoding;
    return NOERROR;
}

ECode PartBase::SetCharSet(
    /* [in] */ const String& charSet)
{
    mCharSet = charSet;
    return NOERROR;
}

ECode PartBase::SetContentType(
    /* [in] */ const String& contentType)
{
    mContentType = contentType;
    return NOERROR;
}

ECode PartBase::SetName(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode PartBase::SetTransferEncoding(
    /* [in] */ const String& transferEncoding)
{
    mTransferEncoding = transferEncoding;
    return NOERROR;
}

}
}
}
}
}
}
