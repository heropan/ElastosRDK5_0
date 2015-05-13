
#ifndef __CHIDERETURNSTRANSFORMATIONMETHOD_H__
#define __CHIDERETURNSTRANSFORMATIONMETHOD_H__

#include "_CHideReturnsTransformationMethod.h"
#include "text/method/HideReturnsTransformationMethod.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IView;
using Elastos::Droid::Graphics::IRect;

namespace Elastos {
namespace Droid {
namespace Text {
namespace Method {

CarClass(CHideReturnsTransformationMethod), public HideReturnsTransformationMethod
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

public:
    static AutoPtr<IHideReturnsTransformationMethod> GetInstance();

private:
    static AutoPtr<IHideReturnsTransformationMethod> sInstance;
};

} //namespace Method
} //namespace Text
} //namespace Droid
} //namespace Elastos

#endif // __CHIDERETURNSTRANSFORMATIONMETHOD_H__
