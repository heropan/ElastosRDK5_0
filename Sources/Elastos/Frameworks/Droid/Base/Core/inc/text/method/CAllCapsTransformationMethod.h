
#ifndef __CALLCAPSTRANSFORMATIONMETHOD_H__
#define __CALLCAPSTRANSFORMATIONMETHOD_H__

#include "_CAllCapsTransformationMethod.h"
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

#endif // __CALLCAPSTRANSFORMATIONMETHOD_H__
