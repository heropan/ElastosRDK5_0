
#include "ext/frameworkext.h"
#include "media/CCodecCapabilities.h"

namespace Elastos {
namespace Droid {
namespace Media {

CCodecCapabilities::CCodecCapabilities()
{
}

ECode CCodecCapabilities::GetProfileLevels(
    /* [out, callee] */ ArrayOf<ICodecProfileLevel*>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = mProfileLevels;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CCodecCapabilities::SetProfileLevels(
    /* [in] */ ArrayOf<ICodecProfileLevel*>* value)
{
    mProfileLevels = value;
    return NOERROR;
}

ECode CCodecCapabilities::GetColorFormats(
    /* [out, callee] */ ArrayOf<Int32>** value)
{
    VALIDATE_NOT_NULL(value);
    *value = mColorFormats;
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CCodecCapabilities::SetColorFormats(
   /* [in] */ ArrayOf<Int32>* value)
{
    mColorFormats = value;
    return NOERROR;
}


} // namespace Media
} // namepsace Droid
} // namespace Elastos
