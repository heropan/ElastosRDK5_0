
#include "widget/ShareActionProvider.h"
#include "utility/CTypedValue.h"
#include <R.h>
#include <elastos/core/Math.h>

using Elastos::Core::CStringWrapper;
using Elastos::Droid::View::EIID_IOnMenuItemClickListener;
using Elastos::Droid::View::IMenu;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Utility::CTypedValue;
using Elastos::Droid::Content::Res::IResourcesTheme;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::View::EIID_IActionProvider;

namespace Elastos {
namespace Droid {
namespace Widget {

const Int32 ShareActionProvider::DEFAULT_INITIAL_ACTIVITY_COUNT;
const String ShareActionProvider::DEFAULT_SHARE_HISTORY_FILE_NAME = String("share_history.xml");

ShareActionProvider::ShareActionProvider()
    : mMaxShownActivityCount(DEFAULT_INITIAL_ACTIVITY_COUNT)
    , mShareHistoryFileName(DEFAULT_SHARE_HISTORY_FILE_NAME)
{
    mOnMenuItemClickListener = new ShareMenuItemOnMenuItemClickListener(this);
}

ShareActionProvider::ShareActionProvider(
    /* [in] */ IContext* context)
    : mMaxShownActivityCount(DEFAULT_INITIAL_ACTIVITY_COUNT)
    , mShareHistoryFileName(DEFAULT_SHARE_HISTORY_FILE_NAME)
{
    mOnMenuItemClickListener = new ShareMenuItemOnMenuItemClickListener(this);
    mContext = context;
}

ECode ShareActionProvider::Init(
    /* [in] */ IContext* context)
{
    mContext = context;
    return NOERROR;
}

ECode ShareActionProvider::SetOnShareTargetSelectedListener(
    /* [in] */ IOnShareTargetSelectedListener* listener)
{
    mOnShareTargetSelectedListener = listener;
    SetActivityChooserPolicyIfNeeded();
    return NOERROR;
}

AutoPtr<IView> ShareActionProvider::OnCreateActionView()
{
    AutoPtr<IActivityChooserModelHelper> helper;
    assert(0 && "TODO");
    //CActivityChooserModelHelper::AcquireSingleton((IActivityChooserModelHelper**)&helper);
    AutoPtr<IActivityChooserModel> dataModel;
    helper->Get(mContext, mShareHistoryFileName, (IActivityChooserModel**)&dataModel);
    AutoPtr<IActivityChooserView> activityChooserView;
    assert(0 && "TODO");
    //CActivityChooserView::New(mContext, (IActivityChooserView**)&activityChooserView);
    activityChooserView->SetActivityChooserModel(dataModel);

    AutoPtr<ITypedValue> outTypedValue;
    CTypedValue::New((ITypedValue**)&outTypedValue);
    AutoPtr<IResourcesTheme> theme;
    mContext->GetTheme((IResourcesTheme**)&theme);
    Boolean res = FALSE;
    theme->ResolveAttribute(R::attr::actionModeShareDrawable, outTypedValue, true, &res);

    Int32 id = 0;
    outTypedValue->GetResourceId(&id);
    AutoPtr<IResources> resources;
    mContext->GetResources((IResources**)&resources);
    AutoPtr<IDrawable> drawable;
    resources->GetDrawable(id, (IDrawable**)&drawable);
    activityChooserView->SetExpandActivityOverflowButtonDrawable(drawable);
    activityChooserView->SetProvider((IActionProvider*)this->Probe(EIID_IActionProvider));

    activityChooserView->SetDefaultActionButtonContentDescription(R::string::shareactionprovider_share_with_application);
    activityChooserView->SetExpandActivityOverflowButtonContentDescription(R::string::shareactionprovider_share_with);

    return activityChooserView;
}

Boolean ShareActionProvider::HasSubMenu()
{
    return TRUE;
}

ECode ShareActionProvider::OnPrepareSubMenu(
    /* [in] */ ISubMenu* subMenu)
{
    subMenu->Clear();
    AutoPtr<IActivityChooserModelHelper> helper;
    assert(0 && "TODO");
    //CActivityChooserModelHelper::AcquireSingleton((IActivityChooserModelHelper**)&helper);
    AutoPtr<IActivityChooserModel> dataModel;
    helper->Get(mContext, mShareHistoryFileName, (IActivityChooserModel**)&dataModel);

    AutoPtr<IPackageManager> packageManager;
    mContext->GetPackageManager((IPackageManager**)&packageManager);
    Int32 expandedActivityCount = 0;
    dataModel->GetActivityCount(&expandedActivityCount);
    Int32 collapsedActivityCount = Elastos::Core::Math::Min(expandedActivityCount, mMaxShownActivityCount);

    for (Int32 i = 0; i < collapsedActivityCount; i++) {
        AutoPtr<IResolveInfo> activity;
        dataModel->GetActivity(i, (IResolveInfo**)&activity);
        AutoPtr<ICharSequence> csq;
        activity->LoadLabel(packageManager, (ICharSequence**)&csq);
        AutoPtr<IMenuItem> item;
        subMenu->Add(0, i, i, csq,(IMenuItem**)&item);
        AutoPtr<IDrawable> icon;
        activity->LoadIcon(packageManager, (IDrawable**)&icon);
        item->SetIcon(icon);
        item->SetOnMenuItemClickListener(mOnMenuItemClickListener);
    }

    if (collapsedActivityCount < expandedActivityCount) {
        String str;
        mContext->GetString(R::string::activity_chooser_view_see_all, &str);
        AutoPtr<ICharSequence> strcsq;
        CStringWrapper::New(str, (ICharSequence**)&strcsq);
        AutoPtr<ISubMenu> expandedSubMenu;
        subMenu->AddSubMenu(IMenu::NONE, collapsedActivityCount, collapsedActivityCount, strcsq, (ISubMenu**)&expandedSubMenu);

        for (Int32 i = 0; i < expandedActivityCount; i++) {
            AutoPtr<IResolveInfo> activity;
            dataModel->GetActivity(i, (IResolveInfo**)&activity);
            AutoPtr<ICharSequence> lebel;
            activity->LoadLabel(packageManager, (ICharSequence**)&lebel);
            AutoPtr<IMenuItem> itemMenu;
            expandedSubMenu->Add(0, i, i, lebel,(IMenuItem**)&itemMenu);
            AutoPtr<IDrawable> drawable;
            activity->LoadIcon(packageManager, (IDrawable**)&drawable);
            itemMenu->SetIcon(drawable);
            itemMenu->SetOnMenuItemClickListener(mOnMenuItemClickListener);
        }
    }
    return NOERROR;
}

ECode ShareActionProvider::SetShareHistoryFileName(
    /* [in] */ const String& shareHistoryFile)
{
    mShareHistoryFileName = shareHistoryFile;
    SetActivityChooserPolicyIfNeeded();
    return NOERROR;
}

ECode ShareActionProvider::SetShareIntent(
    /* [in] */ IIntent* shareIntent)
{
    AutoPtr<IActivityChooserModelHelper> helper;
    assert(0 && "TODO");
    //CActivityChooserModelHelper::AcquireSingleton((IActivityChooserModelHelper**)&helper);
    AutoPtr<IActivityChooserModel> dataModel;
    helper->Get(mContext, mShareHistoryFileName, (IActivityChooserModel**)&dataModel);
    dataModel->SetIntent(shareIntent);
    return NOERROR;
}

ECode ShareActionProvider::SetActivityChooserPolicyIfNeeded()
{
    if (!mOnShareTargetSelectedListener) {
        return NOERROR;
    }
    if (!mOnChooseActivityListener) {
        mOnChooseActivityListener = new ShareAcitivityChooserModelPolicy(this);
    }
    AutoPtr<IActivityChooserModelHelper> helper;
    assert(0 && "TODO");
    //CActivityChooserModelHelper::AcquireSingleton((IActivityChooserModelHelper**)&helper);
    AutoPtr<IActivityChooserModel> dataModel;
    helper->Get(mContext, mShareHistoryFileName, (IActivityChooserModel**)&dataModel);
    dataModel->SetOnChooseActivityListener(mOnChooseActivityListener);
    return NOERROR;
}

//============================================================================
//        ShareActionProvider::ShareMenuItemOnMenuItemClickListener
//============================================================================
CAR_INTERFACE_IMPL(ShareActionProvider::ShareMenuItemOnMenuItemClickListener, IOnMenuItemClickListener)

ShareActionProvider::ShareMenuItemOnMenuItemClickListener::ShareMenuItemOnMenuItemClickListener(
    /* [in] */ ShareActionProvider* host) :
    mHost(host)
{

}

ECode ShareActionProvider::ShareMenuItemOnMenuItemClickListener::OnMenuItemClick(
    /* [in] */ IMenuItem* item,
    /* [out] */ Boolean* isConsumed)
{
    AutoPtr<IActivityChooserModelHelper> helper;
    assert(0 && "TODO");
    //CActivityChooserModelHelper::AcquireSingleton((IActivityChooserModelHelper**)&helper);
    AutoPtr<IActivityChooserModel> dataModel;
    helper->Get(mHost->mContext, mHost->mShareHistoryFileName, (IActivityChooserModel**)&dataModel);
    Int32 itemId;
    item->GetItemId(&itemId);
    AutoPtr<IIntent> launchIntent;
    dataModel->ChooseActivity(itemId, (IIntent**)&launchIntent);
    if (launchIntent) {
        launchIntent->AddFlags(IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
        mHost->mContext->StartActivity(launchIntent);
    }
    *isConsumed = TRUE;
    return NOERROR;
}

//============================================================================
//        ShareActionProvider::ShareAcitivityChooserModelPolicy
//============================================================================
CAR_INTERFACE_IMPL(ShareActionProvider::ShareAcitivityChooserModelPolicy, IOnChooseActivityListener)

ShareActionProvider::ShareAcitivityChooserModelPolicy::ShareAcitivityChooserModelPolicy(
    /* [in] */ ShareActionProvider* host) :
    mHost(host)
{

}

ECode ShareActionProvider::ShareAcitivityChooserModelPolicy::OnChooseActivity(
    /* [in] */  IActivityChooserModel* host,
    /* [in] */  IIntent* intent,
    /* [out] */  Boolean* res)
{
    if (mHost->mOnShareTargetSelectedListener) {
        mHost->mOnShareTargetSelectedListener->OnShareTargetSelected(
            (IShareActionProvider*)mHost->Probe(EIID_IShareActionProvider), intent, res);
    }
    *res = FALSE;
    return NOERROR;
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos

