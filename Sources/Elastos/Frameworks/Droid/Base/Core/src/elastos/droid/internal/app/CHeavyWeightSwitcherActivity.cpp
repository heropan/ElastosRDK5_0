
#include "elastos/droid/app/CHeavyWeightSwitcherActivity.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/utility/CTypedValue.h"

using Elastos::Core::CString;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::View::EIID_IViewOnClickListener;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

CAR_INTERFACE_IMPL(SwitchOldListener, IViewOnClickListener)

ECode SwitchOldListener::OnClick(
    /* [in] */ IView* v)
{
//    try {
    ActivityManagerNative::GetDefault()->MoveTaskToFront(mHost->mCurTask, 0, NULL);
//    } catch (RemoteException e) {
//    }
    mHost->Finish();
    return NOERROR;
}

CAR_INTERFACE_IMPL(SwitchNewListener, IViewOnClickListener)

ECode SwitchNewListener::OnClick(
    /* [in] */ IView* v)
{
//    try {
    ActivityManagerNative::GetDefault()->FinishHeavyWeightApp();
//    } catch (RemoteException e) {
//    }
//    try {
    if (mHost->mHasResult) {
        mHost->StartIntentSenderForResult(mHost->mStartIntent, -1, NULL,
                IIntent::FLAG_ACTIVITY_FORWARD_RESULT,
                IIntent::FLAG_ACTIVITY_FORWARD_RESULT, 0);
    } else {
        mHost->StartIntentSenderForResult(mHost->mStartIntent, -1, NULL, 0, 0, 0);
    }
//    } catch (IntentSender.SendIntentException ex) {
//        Log.w("HeavyWeightSwitcherActivity", "Failure starting", ex);
//    }
    mHost->Finish();
    return NOERROR;
}

CAR_INTERFACE_IMPL(CancelListener, IViewOnClickListener)

ECode CancelListener::OnClick(
    /* [in] */ IView* v)
{
    mHost->Finish();
    return NOERROR;
}

ECode CHeavyWeightSwitcherActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    Activity::OnCreate(savedInstanceState);

    Boolean supported;
    RequestWindowFeature(IWindow::FEATURE_LEFT_ICON, &supported);

    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
//    GetIntent()->GetParcelableExtra(KEY_INTENT, (IParcelable**)&mStartIntent->Probe(EIID_IParcelable));
    intent->GetBooleanExtra(IHeavyWeightSwitcherActivity::KEY_HAS_RESULT, FALSE, &mHasResult);
    intent->GetStringExtra(IHeavyWeightSwitcherActivity::KEY_CUR_APP, &mCurApp);
    intent->GetInt32Extra(IHeavyWeightSwitcherActivity::KEY_CUR_TASK, 0, &mCurTask);
    intent->GetStringExtra(IHeavyWeightSwitcherActivity::KEY_NEW_APP, &mNewApp);

    SetContentView(0/*com.android.internal.R.layout.heavy_weight_switcher*/);

    SetIconAndText(0/*R.id.old_app_icon*/, 0/*R.id.old_app_action*/, 0/*R.id.old_app_description*/,
            mCurApp, 0/*R.string.old_app_action*/, 0/*R.string.old_app_description*/);
    SetIconAndText(0/*R.id.new_app_icon*/, 0/*R.id.new_app_action*/, 0/*R.id.new_app_description*/,
            mNewApp, 0/*R.string.new_app_action*/, 0/*R.string.new_app_description*/);

    AutoPtr<IView> button;
    FindViewById(0/*R.id.switch_old*/, (IView**)&button);
    button->SetOnClickListener(mSwitchOldListener);
    button = NULL;
    FindViewById(0/*R.id.switch_new*/, (IView**)&button);
    button->SetOnClickListener(mSwitchNewListener);
    button = NULL;
    FindViewById(0/*R.id.cancel*/, (IView**)&button);
    button->SetOnClickListener(mCancelListener);

    AutoPtr<ITypedValue> out;
    CTypedValue::New((ITypedValue**)&out);
    AutoPtr<IResourcesTheme> theme;
    GetTheme((IResourcesTheme**)&theme);
    Boolean founded;
    theme->ResolveAttribute(0/*android.R.attr.alertDialogIcon*/, out, TRUE, &founded);
    Int32 resId;
    out->GetResourceId(&resId);
    GetWindow()->SetFeatureDrawableResource(IWindow::FEATURE_LEFT_ICON,
            resId);
    return NOERROR;
}

void CHeavyWeightSwitcherActivity::SetText(
    /* [in] */ Int32 id,
    /* [in] */ ICharSequence* text)
{
    AutoPtr<ITextView> tView;
    FindViewById(id, (IView**)(ITextView**)&tView);
    tView->SetText(text);
}

void CHeavyWeightSwitcherActivity::SetDrawable(
    /* [in] */ Int32 id,
    /* [in] */ IDrawable* dr)
{
    if (dr != NULL) {
        AutoPtr<IImageView> imageView;
        FindViewById(id, (IView**)(IImageView**)&imageView);
        imageView->SetImageDrawable(dr);
    }
}

void CHeavyWeightSwitcherActivity::SetIconAndText(
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 actionId,
    /* [in] */ Int32 descriptionId,
    /* [in] */ const String& packageName,
    /* [in] */ Int32 actionStr,
    /* [in] */ Int32 descriptionStr)
{
    AutoPtr<ICharSequence> appName;
    CString::New(String(""), (ICharSequence**)&appName);
    AutoPtr<IDrawable> appIcon;
    if (mCurApp != NULL) {
//         try {
        AutoPtr<IPackageManager> pkgManager;
        GetPackageManager((IPackageManager**)&pkgManager);
        AutoPtr<IApplicationInfo> info;
        pkgManager->GetApplicationInfo(packageName, 0, (IApplicationInfo**)&info);
        appName = NULL;
        info->LoadLabel(pkgManager, (ICharSequence**)&appName);
        info->LoadIcon(pkgManager, (IDrawable**)&appIcon);
//         } catch (PackageManager.NameNotFoundException e) {
//         }
    }

    SetDrawable(iconId, appIcon);
    String str;
    AutoPtr<ArrayOf<IInterface*> > appNameArray= ArrayOf<IInterface*>::Alloc(1);
    appNameArray->Set(0, appName);
    GetString(actionStr, appNameArray, &str);
    AutoPtr<ICharSequence> strSequence;
    CString::New(str, (ICharSequence**)&strSequence);
    SetText(actionId, strSequence);
    AutoPtr<ICharSequence> txt;
    GetText(descriptionStr, (ICharSequence**)&txt);
    SetText(descriptionId, txt);
}

} //namespace App
} //namespace Internal
} //namespace Droid
} //namespace Elastos
