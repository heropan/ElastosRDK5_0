#ifndef __CSPINNER_H__
#define __CSPINNER_H__

#include "_CSpinner.h"
#include "widget/Spinner.h"
#include "widget/AdapterViewMacro.h"

namespace Elastos {
namespace Droid {
namespace Widget {

CarClass(CSpinner) , public Spinner
{
public:

    IVIEW_METHODS_DECL()
    IVIEWGROUP_METHODS_DECL()
    IADAPTERVIEW_METHODS_DECL()

    CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* ctx);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ Int32 mode);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* ctx,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnClick(
        /* [in] */ IDialogInterface* dialog,
        /* [in] */ Int32 which);

    CARAPI SetSelectionEx(
        /* [in] */ Int32 position,
        /* [in] */ Boolean animate);

    CARAPI PointToPosition(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [out] */ Int32* pos);

    CARAPI SetPopupBackgroundDrawable(
        /* [in] */ IDrawable* background);

    CARAPI SetPopupBackgroundResource(
        /* [in] */ Int32 resId);

    CARAPI GetPopupBackground(
        /* [out] */ IDrawable** d);

    CARAPI SetDropDownVerticalOffset(
        /* [in] */ Int32 pixels);

    CARAPI GetDropDownVerticalOffset(
        /* [out] */ Int32* pixels);

    CARAPI SetDropDownHorizontalOffset(
        /* [in] */ Int32 pixels);

    CARAPI GetDropDownHorizontalOffset(
        /* [out] */ Int32* pixels);

    CARAPI SetDropDownWidth(
        /* [in] */ Int32 pixels);

    CARAPI GetDropDownWidth(
        /* [out] */ Int32* width);

    CARAPI SetGravity(
        /* [in] */ Int32 gravity);

    CARAPI GetGravity(
        /* [out] */ Int32* gravity);

    CARAPI SetPrompt(
        /* [in] */ ICharSequence* prompt);

    CARAPI SetPromptId(
        /* [in] */ Int32 promptId);

    CARAPI GetPrompt(
        /* [out] */ ICharSequence** prompt);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos
#endif
