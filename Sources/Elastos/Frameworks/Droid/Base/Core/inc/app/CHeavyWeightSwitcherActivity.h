
#ifndef __ELASTOS_DROID_INTERNAL_APP_CHEAVYWEIGHTSWITCHERACTIVITY_H__
#define __ELASTOS_DROID_INTERNAL_APP_CHEAVYWEIGHTSWITCHERACTIVITY_H__

#include "_Elastos_Droid_Internal_App_CHeavyWeightSwitcherActivity.h"

#include "app/Activity.h"

using Elastos::Core::ICharSequence;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::App::Activity;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Droid::View::IViewOnClickListener;
using Elastos::Droid::Graphics::Drawable::IDrawable;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

class CHeavyWeightSwitcherActivity;

class SwitchOldListener
    : public ElRefBase
    , public IViewOnClickListener
{
public:
    SwitchOldListener(
        /* [in] */ CHeavyWeightSwitcherActivity* host)
        : mHost(host)
    {}

    CAR_INTERFACE_DECL()

    CARAPI OnClick(
        /* [in] */ IView* v);

private:
    AutoPtr<CHeavyWeightSwitcherActivity> mHost;
};

class SwitchNewListener
    : public ElRefBase
    , public IViewOnClickListener
{
public:
    SwitchNewListener(
        /* [in] */ CHeavyWeightSwitcherActivity* host)
        : mHost(host)
    {}

    CAR_INTERFACE_DECL()

    CARAPI OnClick(
        /* [in] */ IView* v);

private:
    AutoPtr<CHeavyWeightSwitcherActivity> mHost;
};

class CancelListener
    : public ElRefBase
    , public IViewOnClickListener
{
public:
    CancelListener(
        CHeavyWeightSwitcherActivity* host)
        : mHost(host)
    {}

    CAR_INTERFACE_DECL()

    CARAPI OnClick(
        /* [in] */ IView* v);

private:
    AutoPtr<CHeavyWeightSwitcherActivity> mHost;
};

class CHeavyWeightSwitcherActivity
    : public Activity
{
public:
    CHeavyWeightSwitcherActivity()
        : mStartIntent(NULL)
        , mHasResult(FALSE)
        , mCurApp(String(NULL))
        , mCurTask(0)
        , mNewApp(String(NULL))
        , mSwitchOldListener(new SwitchOldListener(this))
        , mSwitchNewListener(new SwitchNewListener(this))
        , mCancelListener(new CancelListener(this))
    {}

    CARAPI OnCreate(
        /* [in] */ IBundle* savedInstanceState);

    CARAPI_(void) SetText(
        /* [in] */ Int32 id,
        /* [in] */ ICharSequence* text);

    CARAPI_(void) SetDrawable(
        /* [in] */ Int32 id,
        /* [in] */ IDrawable* dr);

    CARAPI_(void) SetIconAndText(
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 actionId,
        /* [in] */ Int32 descriptionId,
        /* [in] */ const String& packageName,
        /* [in] */ Int32 actionStr,
        /* [in] */ Int32 descriptionStr);

public:
    AutoPtr<IIntentSender> mStartIntent;
    Boolean mHasResult;
    String mCurApp;
    Int32 mCurTask;
    String mNewApp;
    AutoPtr<SwitchOldListener> mSwitchOldListener;
    AutoPtr<SwitchNewListener> mSwitchNewListener;
    AutoPtr<CancelListener> mCancelListener;
};

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos

#endif //__ELASTOS_DROID_INTERNAL_APP_CHEAVYWEIGHTSWITCHERACTIVITY_H__
