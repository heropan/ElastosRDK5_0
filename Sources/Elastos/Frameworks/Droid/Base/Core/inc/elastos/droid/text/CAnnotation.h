
#ifndef __ELASTOS_DROID_TEXT_CANNOTATION_H__
#define __ELASTOS_DROID_TEXT_CANNOTATION_H__

#include "_Elastos_Droid_Text_CAnnotation.h"
#include "elastos/droid/text/Annotation.h"

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CAnnotation), public Annotation
{
public:
    IPARCELABLESPAN_METHODS_DECL()

    CARAPI GetKey(
        /* [out] */ String* ret);

    CARAPI GetValue(
        /* [out] */ String* ret);

    CARAPI constructor(
        /* [in] */ const String& key,
        /* [in] */ const String& value);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IParcel* src);

};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_CANNOTATION_H__
