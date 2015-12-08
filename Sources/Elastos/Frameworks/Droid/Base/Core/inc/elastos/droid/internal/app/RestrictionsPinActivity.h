
#ifndef __ELASTOS_DROID_INTERNAL_APP_RESTRICTIONSPINACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_RESTRICTIONSPINACTIVITY_H__

#include "elastos/droid/internal/app/AlertActivity.h"
#include "elastos/droid/os/Runnable.h"

using Elastos::Droid::Os::IUserManager;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::Text::IEditable;
using Elastos::Droid::Text::INoCopySpan;
using Elastos::Droid::Text::ITextWatcher;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Widget::IButton;
using Elastos::Droid::Widget::IEditText;
using Elastos::Droid::Widget::IOnEditorActionListener;
using Elastos::Droid::Widget::ITextView;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class RestrictionsPinActivity
    : public AlertActivity
    , public IRestrictionsPinActivity
    , public IViewOnClickListener
    , public ITextWatcher
    , public INoCopySpan
    , public IOnEditorActionListener
{
private:
    class LocalListener
        : public Object
        , public IViewOnClickListener
        , public ITextWatcher
        , public INoCopySpan
        , public IOnEditorActionListener
    {
    public:
        LocalListener(
            /* [in] */ RestrictionsPinActivity* host);

        CAR_INTERFACE_DECL()

        // @Override
        CARAPI BeforeTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 start,
            /* [in] */ Int32 count,
            /* [in] */ Int32 after);

        // @Override
        CARAPI OnTextChanged(
            /* [in] */ ICharSequence* s,
            /* [in] */ Int32 start,
            /* [in] */ Int32 before,
            /* [in] */ Int32 count);

        // @Override
        CARAPI AfterTextChanged(
            /* [in] */ IEditable* s);

        // @Override
        CARAPI OnEditorAction(
            /* [in] */ ITextView* v,
            /* [in] */ Int32 actionId,
            /* [in] */ IKeyEvent* event,
            /* [out] */ Boolean* result);

        // @Override
        CARAPI OnClick(
            /* [in] */ IView* v);

    private:
        RestrictionsPinActivity* mHost;
    };

    class MyRunnable : public Runnable
    {
    public:
        MyRunnable(
            /* [in] */ RestrictionsPinActivity* host);

        CARAPI Run();

    private:
        RestrictionsPinActivity* mHost;
    };


public:
    RestrictionsPinActivity();

    CAR_INTERFACE_DECL()

    // @Override
    CARAPI OnCreate(
        /* [in] */ IBundle* icicle);

    CARAPI OnResume();

    // @Override
    CARAPI BeforeTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 count,
        /* [in] */ Int32 after);

    // @Override
    CARAPI OnTextChanged(
        /* [in] */ ICharSequence* s,
        /* [in] */ Int32 start,
        /* [in] */ Int32 before,
        /* [in] */ Int32 count);

    // @Override
    CARAPI AfterTextChanged(
        /* [in] */ IEditable* s);

    // @Override
    CARAPI OnEditorAction(
        /* [in] */ ITextView* v,
        /* [in] */ Int32 actionId,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    // @Override
    CARAPI OnClick(
        /* [in] */ IView* v);

protected:
    CARAPI_(void) InitUi();

    CARAPI_(Boolean) VerifyingPin();

    CARAPI_(void) SetPositiveButtonState(
        /* [in] */ Boolean enabled);

    CARAPI_(void) PerformPositiveButtonAction();

private:
    CARAPI_(Boolean) UpdatePinTimer(
        /* [in] */ Int32 pinTimerMs);

protected:
    AutoPtr<IUserManager> mUserManager;
    Boolean mHasRestrictionsPin;

    AutoPtr<IEditText> mPinText;
    AutoPtr<ITextView> mPinErrorMessage;

private:
    AutoPtr<IButton> mOkButton;
    AutoPtr<IButton> mCancelButton;
    AutoPtr<IRunnable> mCountdownRunnable;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_RESTRICTIONSPINACTIVITY_H__
