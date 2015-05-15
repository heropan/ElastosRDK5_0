
#ifndef __CANNOTATION_H__
#define __CANNOTATION_H__

#include "_CAnnotation.h"
#include "Annotation.h"

namespace Elastos {
namespace Text {

CarClass(CAnnotation) , public Annotation
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IInterface * attribute);

    CARAPI GetValue(
        /* [out] */ IInterface ** value);

    CARAPI ToString(
        /* [out] */ String * str);
};

} // namespace Text
} // namespace Elastos

#endif // __CANNOTATION_H__
