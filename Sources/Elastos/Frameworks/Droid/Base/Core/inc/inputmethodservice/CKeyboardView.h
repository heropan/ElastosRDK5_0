
#ifndef  __ELASTOS_DROID_INPUTMEHTODSERVICE_CKEYBOARDVIEW_H__
#define  __ELASTOS_DROID_INPUTMEHTODSERVICE_CKEYBOARDVIEW_H__

#include "_Elastos_Droid_InputMethodService_CKeyboardView.h"
#include "inputmethodservice/KeyboardView.h"

namespace Elastos {
namespace Droid {
namespace InputMethodService {

/**
 * A view that renders a virtual {@link Keyboard}. It handles rendering of keys and
 * detecting key presses and touch movements.
 *
 * @attr ref android.R.styleable#KeyboardView_keyBackground
 * @attr ref android.R.styleable#KeyboardView_keyPreviewLayout
 * @attr ref android.R.styleable#KeyboardView_keyPreviewOffset
 * @attr ref android.R.styleable#KeyboardView_labelTextSize
 * @attr ref android.R.styleable#KeyboardView_keyTextSize
 * @attr ref android.R.styleable#KeyboardView_keyTextColor
 * @attr ref android.R.styleable#KeyboardView_verticalCorrection
 * @attr ref android.R.styleable#KeyboardView_popupLayout
 */
CarClass(CKeyboardView), public KeyboardView
{
public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);
};

} // namespace InputMethodService
} // namespace Droid
} // namespace Elastos

#endif  // __ELASTOS_DROID_INPUTMEHTODSERVICE_CKEYBOARDVIEW_H__
