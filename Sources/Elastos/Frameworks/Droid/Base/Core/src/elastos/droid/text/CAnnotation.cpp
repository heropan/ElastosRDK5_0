
#include "elastos/droid/text/CAnnotation.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Text {

IPARCELABLESPAN_METHODS_IMPL(CAnnotation, Annotation, Annotation)

ECode CAnnotation::GetKey(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Annotation::GetKey();
    return NOERROR;
}

ECode CAnnotation::GetValue(
    /* [out] */ String* ret)
{
    VALIDATE_NOT_NULL(ret);
    *ret = Annotation::GetValue();
    return NOERROR;
}

ECode CAnnotation::constructor()
{
    return NOERROR;
}

ECode CAnnotation::constructor(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    Init(key, value);
    return NOERROR;
}

ECode CAnnotation::constructor(
    /* [in] */ IParcel* src)
{
    Init(src);
    return NOERROR;
}

} // namespace Text
} // namepsace Droid
} // namespace Elastos
