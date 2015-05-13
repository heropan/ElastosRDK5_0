
#ifndef __CANNOTATION_H__
#define __CANNOTATION_H__

#include "_CAnnotation.h"
#include "text/Annotation.h"
#include "ParcelableSpanMacro.h"

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

#endif // __CANNOTATION_H__
