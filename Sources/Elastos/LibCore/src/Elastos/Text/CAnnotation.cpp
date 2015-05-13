
#include "CAnnotation.h"

namespace Elastos {
namespace Text {

ECode CAnnotation::constructor()
{
    return NOERROR;
}

ECode CAnnotation::constructor(
    /* [in] */ IInterface * attribute)
{
    return Annotation::Init(attribute);
}

ECode CAnnotation::GetValue(
    /* [out] */ IInterface ** value)
{
    return Annotation::GetValue(value);
}

ECode CAnnotation::ToString(
    /* [out] */ String * str)
{
    return Annotation::ToString(str);
}

} // namespace Text
} // namespace Elastos

