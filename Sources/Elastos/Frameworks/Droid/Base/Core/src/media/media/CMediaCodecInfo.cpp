
#include "ext/frameworkext.h"
#include "media/CMediaCodecInfo.h"
#include <elastos/Logger.h>
#include "media/CMediaCodecList.h"

using namespace Elastos::Core;
//using namespace Elastos::Utility::Logging;

namespace Elastos {
namespace Droid {
namespace Media {


ECode CMediaCodecInfo::constructor(
    /* [in] */ Int32 index)
{
    mIndex = index;
    return NOERROR;
}

ECode CMediaCodecInfo::GetName(
    /* [out] */ String* result)
{
    return CMediaCodecList::GetCodecName(mIndex, result);
}

ECode CMediaCodecInfo::IsEncoder(
    /* [out] */ Boolean* result)
{
    return CMediaCodecList::IsEncoder(mIndex,result);
}

ECode CMediaCodecInfo::GetSupportedTypes(
    /* [out, callee] */ ArrayOf<String>** result)
{
    return CMediaCodecList::GetSupportedTypes(mIndex, result);
}

ECode CMediaCodecInfo::GetCapabilitiesForType(
    /* [in] */ const String& type,
    /* [out] */ ICodecCapabilities** result)
{
    return CMediaCodecList::GetCodecCapabilities(mIndex, type, result);
}


} // namespace Media
} // namepsace Droid
} // namespace Elastos
