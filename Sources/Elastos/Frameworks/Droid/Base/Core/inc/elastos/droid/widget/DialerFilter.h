
#ifndef __ELASTOS_DROID_WIDGET_DIALERFILTER_H__
#define __ELASTOS_DROID_WIDGET_DIALERFILTER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/RelativeLayout.h"
#include "Elastos.Droid.Text.h"

using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Text::IInputFilter;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Widget::RelativeLayout;

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Widget {

class DialerFilter
    : public RelativeLayout
    , public IDialerFilter
{
public:
    CAR_INTERFACE_DECL()

    DialerFilter();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI IsQwertyKeyboard(
        /* [out] */ Boolean* res);

    CARAPI OnKeyDown(
        /* [in] */ Int32 keycode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI OnKeyUp(
        /* [in] */ Int32 keycode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    CARAPI GetMode(
        /* [out] */ Int32* mode);

    CARAPI SetMode(
        /* [in] */ Int32 newMode);

    CARAPI GetLetters(
        /* [out] */ ICharSequence** letters);

    CARAPI GetDigits(
        /* [out] */ ICharSequence** digits);

    CARAPI GetFilterText(
        /* [out] */ ICharSequence** text);

    CARAPI Append(
        /* [in] */ const String& text);

    CARAPI ClearText();

    CARAPI SetLettersWatcher(
        /* [in] */ ITextWatcher* watcher);

    CARAPI SetDigitsWatcher(
        /* [in] */ ITextWatcher* watcher);

    CARAPI SetFilterWatcher(
        /* [in] */ ITextWatcher* watcher);

    CARAPI RemoveFilterWatcher(
        /* [in] */  ITextWatcher* watcher);

protected:
    virtual CARAPI OnFinishInflate();

    virtual CARAPI_(void) OnFocusChanged(
        /* [in] */ Boolean focused,
        /* [in] */ Int32 direction,
        /* [in] */ IRect* previouslyFocusedRect);

    CARAPI OnModeChange(
        /* [in] */ Int32 oldMode,
        /* [in] */ Int32 newMode);

private:
    CARAPI MakeLettersPrimary();

    CARAPI MakeDigitsPrimary();

    CARAPI SwapPrimaryAndHint(
        /* [in] */ Boolean makeLettersPrimary);

private:
    AutoPtr<IEditText> mLetters;
    AutoPtr<IEditText> mDigits;
    AutoPtr<IEditText> mPrimary;
    AutoPtr<IEditText> mHint;

    AutoPtr<ArrayOf<IInputFilter*> > mInputFilters;

    AutoPtr<IImageView> mIcon;
    Int32 mMode;

    Boolean mIsQwerty;
};


} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif
