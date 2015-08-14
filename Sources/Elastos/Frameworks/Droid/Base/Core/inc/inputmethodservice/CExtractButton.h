
#ifndef __ELASTOS_DROID_INPUTMETHODSERVICE_ELASTOS_DROID_INPUTMEHTODSERVICE_CEXTRACTBUTTON_H__
#define  __ELASTOS_DROID_INPUTMETHODSERVICE_ELASTOS_DROID_INPUTMEHTODSERVICE_CEXTRACTBUTTON_H__

//#include "ext/frameworkext.h"
#include "_Elastos_Droid_InputMethodService_CExtractButton.h"
#include "widget/Button.h"

using Elastos::Droid::Widget::Button;

namespace Elastos {
namespace Droid {
namespace InputMethodService {

/**
 * Specialization of {@link Button} that ignores the window not being focused.
 */
CarClass(CExtractButton), public Button
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CExtractButton();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnPreDraw(
        /* [out] */ Boolean* result);

    /**
     * Pretend like the window this view is in always has focus, so it will
     * highlight when selected.
     */
    CARAPI_(Boolean) HasWindowFocus();
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_INPUTMETHODSERVICE_ELASTOS_DROID_INPUTMEHTODSERVICE_CEXTRACTBUTTON_H__
