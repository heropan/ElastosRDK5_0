
#include "media/CMediaPlayerSubInfo.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Media {


ECode CMediaPlayerSubInfo::constructor(
    /* [in] */ ArrayOf<Byte>* name,
    /* [in] */ const String& charset,
    /* [in] */ Int32 type)
{
    mName = name;
    mCharset = charset;
    mType = type;
    return NOERROR;
}

ECode CMediaPlayerSubInfo::SetName(
    /* [in] */ ArrayOf<Byte>* name)
{
    mName = name;
    return NOERROR;
}

ECode CMediaPlayerSubInfo::GetName(
    /* [out, callee] */ ArrayOf<Byte>** name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    REFCOUNT_ADD(*name);
    return NOERROR;
}

ECode CMediaPlayerSubInfo::SetCharset(
    /* [in] */ const String& charset)
{
    mCharset = charset;
    return NOERROR;
}

ECode CMediaPlayerSubInfo::GetCharset(
    /* [out] */ String* charset)
{
    VALIDATE_NOT_NULL(charset);
    *charset = mCharset;
    return NOERROR;
}

ECode CMediaPlayerSubInfo::SetType(
    /* [in] */ Int32 type)
{
    mType = type;
    return NOERROR;
}

ECode CMediaPlayerSubInfo::GetType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mType;
    return NOERROR;
}


} // namespace media
} // namepsace Droid
} // namespace Elastos
