
#ifndef __ELASTOS_DROID_TEXT_METHOD_CALLCAPSTRANSFORMATIONMETHOD_H__
#define __ELASTOS_DROID_TEXT_METHOD_CALLCAPSTRANSFORMATIONMETHOD_H__

#include "_Elastos_Droid_Text_Method_CAllCapsTransformationMethod.h"
#include "text/method/AllCapsTransformationMethod.h"

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CAllCapsTransformationMethod)
    , public AllCapsTransformationMethod
{
public:
    CARAPI GetTransformation(
        /* [in] */ ICharSequence* source,
        /* [in] */ IView* view,
        /* [out] */ ICharSequence** csq);

    CARAPI OnFocusChanged(
        /* [in] */ IView* view,
        /* [in] */ ICharSequence* sourceText,
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI SetLengthChangesAllowed(
        /* [in] */ Boolean allowLengthChanges);

    CARAPI constructor(
        /* [in] */ IContext* context);
};

} // namespace Method
} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_TEXT_METHOD_CALLCAPSTRANSFORMATIONMETHOD_H__
