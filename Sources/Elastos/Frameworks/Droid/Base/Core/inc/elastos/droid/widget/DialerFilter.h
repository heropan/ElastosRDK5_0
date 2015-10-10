
#ifndef __ELASTOS_DROID_WIDGET_DIALERFILTER_H__
#define __ELASTOS_DROID_WIDGET_DIALERFILTER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "widget/RelativeLayout.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::View::IKeyEvent;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Text::IInputFilter;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
using Elastos::Droid::Widget::RelativeLayout;

namespace Elastos {
namespace Droid {
namespace Widget {


class DialerFilter : public Elastos::Droid::Widget::RelativeLayout
{
public:
    DialerFilter();

    DialerFilter(
        /* [in] */ IContext* context);

    DialerFilter(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(Boolean) IsQwertyKeyboard();

    virtual CARAPI_(Boolean) OnKeyDown(
        /* [in] */ Int32 keycode,
        /* [in] */ IKeyEvent* event);

    virtual CARAPI_(Boolean) OnKeyUp(
        /* [in] */ Int32 keycode,
        /* [in] */ IKeyEvent* event);

    CARAPI_(Int32) GetMode();

    CARAPI SetMode(
        /* [in] */ Int32 newMode);

    CARAPI_(AutoPtr<ICharSequence>) GetLetters();

    CARAPI_(AutoPtr<ICharSequence>) GetDigits();

    CARAPI_(AutoPtr<ICharSequence>) GetFilterText();

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

public:
    const static Int32 DIGITS_AND_LETTERS = 1;
    const static Int32 DIGITS_AND_LETTERS_NO_DIGITS = 2;
    const static Int32 DIGITS_AND_LETTERS_NO_LETTERS = 3;
    const static Int32 DIGITS_ONLY = 4;
    const static Int32 LETTERS_ONLY = 5;

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
