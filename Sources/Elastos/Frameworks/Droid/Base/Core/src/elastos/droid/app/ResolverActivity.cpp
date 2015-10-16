
#include "elastos/droid/app/ResolverActivity.h"
#include "elastos/droid/app/CActivityManagerHelper.h"
#include "elastos/droid/app/ActivityManagerNative.h"
#include "elastos/droid/content/pm/CResolveInfo.h"
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CComponentName.h"
#include "elastos/droid/os/UserHandle.h"
#include "elastos/droid/net/Uri.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Slogger.h>
#include "elastos/droid/ext/frameworkext.h"
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Utility::Etl::HashSet;
using Elastos::Core::CString;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::Widget::EIID_IAdapterViewOnItemLongClickListener;
using Elastos::Droid::Widget::EIID_IBaseAdapter;
using Elastos::Droid::App::CActivityManagerHelper;
using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::IActivityManager;
using Elastos::Droid::App::IIActivityManager;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::CComponentName;
using Elastos::Droid::Content::IIntentFilterAuthorityEntry;
using Elastos::Droid::Content::Pm::CResolveInfo;
using Elastos::Droid::Content::Pm::ILabeledIntent;
using Elastos::Droid::Content::Pm::EIID_ILabeledIntent;
using Elastos::Droid::Content::Pm::IResolveInfoDisplayNameComparator;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Net::Uri;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace App {

const String ResolverActivity::TAG("ResolverActivity");

ECode ResolverActivity::_PackageMonitor::OnSomePackagesChanged()
{
    return mHost->mAdapter->HandlePackagesChanged();
}

ResolverActivity::ResolverActivity()
    : mLaunchedFromUid(0)
    , mAdapter(NULL)
    , mAlwaysUseOption(FALSE)
    , mShowExtended(FALSE)
    , mIconDpi(0)
    , mIconSize(0)
    , mMaxColumns(0)
    , mLastSelected(IGridView::INVALID_POSITION)
    , mRegistered(FALSE)
{
    mPackageMonitor = new _PackageMonitor(this);
}

AutoPtr<IIntent> ResolverActivity::MakeMyIntent()
{
    AutoPtr<IIntent> intent;
    AutoPtr<IIntent> newIntent;
    GetIntent((IIntent**)&newIntent);
    CIntent::New(newIntent, (IIntent**)&intent);
    // The resolver activity is set to be hidden from recent tasks.
    // we don't want this attribute to be propagated to the next activity
    // being launched.  Note that if the original Intent also had this
    // flag set, we are now losing it.  That should be a very rare case
    // and we can live with this.
    Int32 flags;
    intent->GetFlags(&flags);
    intent->SetFlags(flags&~IIntent::FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
    return intent;
}

ECode ResolverActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState)
{
    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    AutoPtr<ICharSequence> text;
    resources->GetText(0/*com.android.internal.R.string.whichApplication*/, (ICharSequence**)&text);
    return OnCreate(savedInstanceState, MakeMyIntent(),
            text, NULL, NULL, TRUE);
}

ECode ResolverActivity::OnCreate(
    /* [in] */ IBundle* savedInstanceState,
    /* [in] */ IIntent* intent,
    /* [in] */ ICharSequence* title,
    /* [in] */ ArrayOf<IIntent*>* initialIntents,
    /* [in] */ List<AutoPtr<IResolveInfo> >* rList,
    /* [in] */ Boolean alwaysUseOption)
{
    SetTheme(0/*R.style.Theme_DeviceDefault_Light_Dialog_Alert*/);
    AlertActivity::OnCreate(savedInstanceState);
//     try {
    AutoPtr<IBinder> binder;
    GetActivityToken((IBinder**)&binder);
    ActivityManagerNative::GetDefault()->GetLaunchedFromUid(binder, &mLaunchedFromUid);
//     } catch (RemoteException e) {
//         mLaunchedFromUid = -1;
//     }
    GetPackageManager((IPackageManager**)&mPm);
    mAlwaysUseOption = alwaysUseOption;
    AutoPtr<IResources> resources;
    GetResources((IResources**)&resources);
    resources->GetInteger(0/*R.integer.config_maxResolverActivityColumns*/, &mMaxColumns);
    intent->SetComponent(NULL);

    AutoPtr<CAlertControllerAlertParams> ap = mAlertParams;

    ap->SetTitle(title);

//     mPackageMonitor.register(this, getMainLooper(), false);
    mRegistered = TRUE;

    AutoPtr<IActivityManager> am;
    GetSystemService(IContext::ACTIVITY_SERVICE, (IInterface**)(IActivityManager**)&am);
    assert(am != NULL);
    am->GetLauncherLargeIconDensity(&mIconDpi);
    am->GetLauncherLargeIconSize(&mIconSize);

//    mAdapter = new ResolveListAdapter(this, intent, initialIntents, rList,
//            mLaunchedFromUid);
    Int32 count;
    mAdapter->GetCount(&count);
    if (mLaunchedFromUid < 0 || UserHandle::IsIsolated(mLaunchedFromUid)) {
        // Gulp!
        Finish();
        return NOERROR;
    } else if (count > 1) {
        AutoPtr<ILayoutInflater> lInflater;
        GetLayoutInflater((ILayoutInflater**)&lInflater);
        AutoPtr<IView> view;
        lInflater->Inflate(0/*R.layout.resolver_grid*/, NULL, (IView**)&view);
        ap->SetView(view);
        view->FindViewById(0/*R.id.resolver_grid*/, (IView**)(IGridView**)&mGrid);
//        mGrid->SetAdapter(mAdapter);
        mGrid->SetOnItemClickListener(this);
        AutoPtr<ItemLongClickListener> tlcl = new ItemLongClickListener(this);
        mGrid->SetOnItemLongClickListener(tlcl);

        if (alwaysUseOption) {
            mGrid->SetChoiceMode(IListView::CHOICE_MODE_SINGLE);
        }

        ResizeGrid();
    } else if (count == 1) {
        AutoPtr<IIntent> intent;
        mAdapter->IntentForPosition(0, (IIntent**)&intent);
        StartActivity(intent);
        mPackageMonitor->Unregister();
        mRegistered = FALSE;
        Finish();
        return NOERROR;
    } else {
        AutoPtr<IResources> resources;
        GetResources((IResources**)&resources);
        AutoPtr<ICharSequence> text;
        resources->GetText(0/*R.string.noApplications*/, (ICharSequence**)&text);
        ap->SetMessage(text);
    }

    SetupAlert();

    if (alwaysUseOption) {
        AutoPtr<IViewGroup> buttonLayout;
        FindViewById(0/*R.id.button_bar*/, (IView**)(IViewGroup**)&buttonLayout);
        if (buttonLayout != NULL) {
            buttonLayout->SetVisibility(IView::VISIBLE);
            buttonLayout->FindViewById(0/*R.id.button_always*/, (IView**)(IButton**)&mAlwaysButton);
            buttonLayout->FindViewById(0/*R.id.button_once*/, (IView**)(IButton**)&mOnceButton);
        } else {
            mAlwaysUseOption = FALSE;
        }
    }
    return NOERROR;
}

ECode ResolverActivity::ResizeGrid()
{
    Int32 itemCount;
    mAdapter->GetCount(&itemCount);
    mGrid->SetNumColumns(Elastos::Core::Math::Min(itemCount, mMaxColumns));
    return NOERROR;
}

ECode ResolverActivity::GetIcon(
    /* [in] */ IResources* res,
    /* [in] */ Int32 resId,
    /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(res);
    VALIDATE_NOT_NULL(drawable);
    AutoPtr<IDrawable> result;
//     try {
    res->GetDrawableForDensity(resId, mIconDpi, (IDrawable**)&result);
//     } catch (Resources.NotFoundException e) {
//         result = null;
//     }

    *drawable = result;
    REFCOUNT_ADD(*drawable);
    return NOERROR;
}

ECode ResolverActivity::LoadIconForResolveInfo(
        /* [in] */ IResolveInfo* ri,
        /* [out] */ IDrawable** drawable)
{
    VALIDATE_NOT_NULL(ri);
    VALIDATE_NOT_NULL(drawable);

    AutoPtr<IDrawable> dr;
//     try {
    String pkgName;
    ri->GetResolvePackageName(&pkgName);
    Int32 icon;
    ri->GetIcon(&icon);
        if (pkgName != String(NULL) && icon != 0) {
            String riPkgName;
            ri->GetResolvePackageName(&riPkgName);
            AutoPtr<IResources> resources;
            mPm->GetResourcesForApplication(riPkgName, (IResources**)&resources);
            Int32 riIcon;
            ri->GetIcon(&riIcon);
            GetIcon(resources, riIcon, (IDrawable**)&dr);
            if (dr != NULL) {
                *drawable = dr;
                REFCOUNT_ADD(*drawable);
                return NOERROR;
            }
        }
        Int32 iconRes;
        ri->GetIconResource(&iconRes);
        if (iconRes != 0) {
            AutoPtr<IActivityInfo> activityInfo;
            ri->GetActivityInfo((IActivityInfo**)&activityInfo);
            String riPkgName;
            activityInfo->GetPackageName(&riPkgName);
            AutoPtr<IResources> resources;
            mPm->GetResourcesForApplication(riPkgName, (IResources**)&resources);
            GetIcon(resources, iconRes, (IDrawable**)&dr);
            if (dr != NULL) {
                *drawable = dr;
                REFCOUNT_ADD(*drawable);
                return NOERROR;
            }
        }
//     } catch (NameNotFoundException e) {
//         Log.e(TAG, "Couldn't find resources for package", e);
//     }
    ri->LoadIcon(mPm, drawable);
    return NOERROR;
}

ECode ResolverActivity::OnRestart()
{
    AlertActivity::OnRestart();
    if (!mRegistered) {
        AutoPtr<ILooper> looper;
        GetMainLooper((ILooper**)&looper);
        mPackageMonitor->Register(this, looper, FALSE);
        mRegistered = TRUE;
    }
    mAdapter->HandlePackagesChanged();
    return NOERROR;
}

ECode ResolverActivity::OnStop()
{
    AlertActivity::OnStop();
    if (mRegistered) {
//         mPackageMonitor.unregister();
//         mRegistered = false;
    }
    AutoPtr<IIntent> intent;
    GetIntent((IIntent**)&intent);
    Int32 flags;
    intent->GetFlags(&flags);
    if ((flags&IIntent::FLAG_ACTIVITY_NEW_TASK) != 0) {
        // This resolver is in the unusual situation where it has been
        // launched at the top of a new task.  We don't let it be added
        // to the recent tasks shown to the user, and we need to make sure
        // that each time we are launched we get the correct launching
        // uid (not re-using the same resolver from an old launching uid),
        // so we will now finish ourself since being no longer visible,
        // the user probably can't get back to us.
        Boolean changed;
        IsChangingConfigurations(&changed);
        if (!changed) {
            Finish();
        }
    }
    return NOERROR;
}

ECode ResolverActivity::OnRestoreInstanceState(
       /* [in] */ IBundle* savedInstanceState)
{
    AlertActivity::OnRestoreInstanceState(savedInstanceState);
    if (mAlwaysUseOption) {
        Int32 checkedPos;
        mGrid->GetCheckedItemPosition(&checkedPos);
        Boolean enabled = checkedPos != IGridView::INVALID_POSITION;
        mLastSelected = checkedPos;
        mAlwaysButton->SetEnabled(enabled);
        mOnceButton->SetEnabled(enabled);
        if (enabled) {
            mGrid->SetSelection(checkedPos);
        }
    }
    return NOERROR;
}

ECode ResolverActivity::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    Int32 checkedPos;
    mGrid->GetCheckedItemPosition(&checkedPos);
    Boolean hasValidSelection = checkedPos != IGridView::INVALID_POSITION;
    if (mAlwaysUseOption && (!hasValidSelection || mLastSelected != checkedPos)) {
        mAlwaysButton->SetEnabled(hasValidSelection);
        mOnceButton->SetEnabled(hasValidSelection);
        if (hasValidSelection) {
            mGrid->SmoothScrollToPosition(checkedPos);
        }
        mLastSelected = checkedPos;
    } else {
        FAIL_RETURN(StartSelected(position, FALSE));
    }
    return NOERROR;
}

ECode ResolverActivity::OnButtonClick(
       /* [in] */ IView* v)
{
    Int32 id;
    v->GetId(&id);
    Int32 pos;
    mGrid->GetCheckedItemPosition(&pos);
    FAIL_RETURN(StartSelected(pos, id == 0/*R.id.button_always*/));
    Dismiss();
    return NOERROR;
}

ECode ResolverActivity::StartSelected(
    /* [in] */ Int32 which,
    /* [in] */ Boolean always)
{
    AutoPtr<IResolveInfo> ri;
    mAdapter->ResolveInfoForPosition(which, (IResolveInfo**)&ri);
    AutoPtr<IIntent> intent;
    mAdapter->IntentForPosition(which, (IIntent**)&intent);
    FAIL_RETURN(OnIntentSelected(ri, intent, always));
    Finish();
    return NOERROR;
}

ECode ResolverActivity::OnIntentSelected(
    /* [in] */ IResolveInfo* ri,
    /* [in] */ IIntent* intent,
    /* [in] */ Boolean alwaysCheck)
{
    if (alwaysCheck) {
        // Build a reasonable intent filter, based on what matched.
        AutoPtr<IIntentFilter> filter;
        CIntentFilter::New((IIntentFilter**)&filter);

        String action;
        intent->GetAction(&action);
        if (action != String(NULL)) {
            filter->AddAction(action);
        }
        AutoPtr<ArrayOf<String> > categories;
        intent->GetCategories((ArrayOf<String>**)&categories);
        Int32 size = categories->GetLength();
        if (categories != NULL) {
            for (Int32 i = 0; i < size; ++i) {
                filter->AddCategory((*categories)[i]);
            }
        }
        filter->AddCategory(IIntent::CATEGORY_DEFAULT);

        Int32 match;
        ri->GetMatch(&match);
        Int32 cat = match&IIntentFilter::MATCH_CATEGORY_MASK;
        AutoPtr<IUri> data;
        intent->GetData((IUri**)&data);
        if (cat == IIntentFilter::MATCH_CATEGORY_TYPE) {
            String mimeType;
            intent->ResolveType(this, &mimeType);
            if (mimeType != NULL) {
//                 try {
                filter->AddDataType(mimeType);
//                 } catch (IntentFilter.MalformedMimeTypeException e) {
//                     Log.w("ResolverActivity", e);
//                     filter = null;
//                 }
            }
        }
        String scheme;
        data->GetScheme(&scheme);
        if (data != NULL && scheme != String(NULL)) {
            // We need the data specification if there was no type,
            // OR if the scheme is not one of our magical "file:"
            // or "content:" schemes (see IntentFilter for the reason).
            if (cat != IIntentFilter::MATCH_CATEGORY_TYPE
                    || (!(String("file") == scheme)
                            && !(String("content") == scheme))) {
                filter->AddDataScheme(scheme);

                // Look through the resolved filter to determine which part
                // of it matched the original Intent.
                AutoPtr<IIntentFilter> rfilter;
                ri->GetFilter((IIntentFilter**)&rfilter);
                AutoPtr<ArrayOf<IIntentFilterAuthorityEntry*> > aIt;
                rfilter->GetAuthorities((ArrayOf<IIntentFilterAuthorityEntry*>**)&aIt);
                Int32 size = aIt->GetLength();
                if (aIt != NULL) {
                    for (Int32 i = 0; i < size; ++i) {
                        AutoPtr<IIntentFilterAuthorityEntry> a = (*aIt)[i];
                        Int32 match;
                        a->Match(data, &match);
                        if (match >= 0) {
                            Int32 port;
                            a->GetPort(&port);
                            String host;
                            a->GetHost(&host);
                            filter->AddDataAuthority(host,
                                    port >= 0 ? StringUtils::Int32ToString(port) : String(NULL));
                            break;
                        }
                    }
                }
                AutoPtr<ArrayOf<IPatternMatcher*> > pIt;
                rfilter->GetPaths((ArrayOf<IPatternMatcher*>**)&pIt);
                size = pIt->GetLength();
                if (pIt != NULL) {
                    String path;
                    data->GetPath(&path);
                    if (path != NULL) {
                        for (Int32 i = 0; i < size; ++i) {
                            AutoPtr<IPatternMatcher> p = (*pIt)[i];
                            Boolean match;
                            p->Match(path, &match);
                            if (match) {
                                String path;
                                p->GetPath(&path);
                                Int32 type;
                                p->GetType(&type);
                                filter->AddDataPath(path, type);
                                break;
                            }
                        }
                    }
                }
            }
        }

        if (filter != NULL) {
            Int32 N = mAdapter->mList.GetSize();
            AutoPtr<ArrayOf<IComponentName*> > set = ArrayOf<IComponentName*>::Alloc(N);
            Int32 bestMatch = 0;
            for (Int32 i=0; i<N; i++) {
                AutoPtr<IResolveInfo> r = mAdapter->mList[i]->mRi;
                AutoPtr<IActivityInfo> aInfo;
                r->GetActivityInfo((IActivityInfo**)&aInfo);
                String pkgName, name;
                aInfo->GetPackageName(&pkgName);
                aInfo->GetName(&name);
                CComponentName::New(pkgName, name, (IComponentName**)&(*set)[i]);
                Int32 rmatch;
                r->GetMatch(&rmatch);
                if (rmatch > bestMatch) bestMatch = rmatch;
            }
            AutoPtr<IPackageManager> pkgMgr;
            GetPackageManager((IPackageManager**)&pkgMgr);
            AutoPtr<IComponentName> component;
            intent->GetComponent((IComponentName**)&component);
            FAIL_RETURN(pkgMgr->AddPreferredActivity(filter, bestMatch, set, component));
        }
    }

    if (intent != NULL) {
        StartActivity(intent);
    }
    return NOERROR;
}

ECode ResolverActivity::ShowAppDetails(
    /* [in] */ IResolveInfo* ri)
{
    AutoPtr<IIntent> in;
    CIntent::New((IIntent**)&in);
    in->SetAction(String("elastos.settings.APPLICATION_DETAILS_SETTINGS"));
    AutoPtr<IActivityInfo> aInfo;
    ri->GetActivityInfo((IActivityInfo**)&aInfo);
    String pkgName;
    aInfo->GetPackageName(&pkgName);
    AutoPtr<IUri> uri;
    Uri::FromParts(String("package"), pkgName, String(NULL), (IUri**)&uri);
    in->SetData(uri);
    in->AddFlags(IIntent::FLAG_ACTIVITY_CLEAR_WHEN_TASK_RESET);
    StartActivity(in);
    return NOERROR;
}

ResolverActivity::ResolveListAdapter::ResolveListAdapter(
    /* [in] */ ResolverActivity* host,
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent,
    /* [in] */ ArrayOf<IIntent*>* initialIntents,
    /* [in] */ List<AutoPtr<IResolveInfo> >* rList,
    /* [in] */ Int32 launchedFromUid)
    : mHost(host)
    , mCurrentResolveList(NULL)
{
    CIntent::New(intent, (IIntent**)&mIntent);
    mIntent->SetComponent(NULL);
    mInitialIntents = initialIntents;
    mBaseResolveList = rList;
    mLaunchedFromUid = launchedFromUid;
    context->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE,
            (IInterface**)(ILayoutInflater**)&mInflater);
    RebuildList();
}

ResolverActivity::ResolveListAdapter::~ResolveListAdapter()
{
    mList.Clear();
}

ECode ResolverActivity::ResolveListAdapter::HandlePackagesChanged()
{
    Int32 oldItemCount;
    GetCount(&oldItemCount);
    RebuildList();
//    NotifyDataSetChanged();
    if (mList.IsEmpty()) {
        // We no longer have any items...  just finish the activity.
        mHost->Finish();
    }

    Int32 newItemCount;
    GetCount(&newItemCount);
    if (newItemCount != oldItemCount) {
        mHost->ResizeGrid();
    }
    return NOERROR;
}

void ResolverActivity::ResolveListAdapter::RebuildList()
{
    if (mBaseResolveList != NULL) {
        mCurrentResolveList = mBaseResolveList;
    }
    else {
        AutoPtr<IObjectContainer> activities;
        mHost->mPm->QueryIntentActivities(
                mIntent, IPackageManager::MATCH_DEFAULT_ONLY
                | (mHost->mAlwaysUseOption ? IPackageManager::GET_RESOLVED_FILTER : 0),
                (IObjectContainer**)&activities);
        AutoPtr<IObjectEnumerator> enumerator;
        activities->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
        Boolean hasNext;
        Int32 i = 0;
        while (enumerator->MoveNext(&hasNext), hasNext) {
            AutoPtr<IResolveInfo> resInfo;
            enumerator->Current((IInterface**)(IResolveInfo**)&resInfo);
            (*mCurrentResolveList)[i] = resInfo;
            ++i;
        }
        // Filter out any activities that the launched uid does not
        // have permission for.  We don't do this when we have an explicit
        // list of resolved activities, because that only happens when
        // we are being subclassed, so we can safely launch whatever
        // they gave us.
        if (mCurrentResolveList != NULL) {
            AutoPtr<IActivityManagerHelper> amHelper;
            CActivityManagerHelper::AcquireSingleton((IActivityManagerHelper**)&amHelper);
            String permission;
            Int32 uid, granted;
            Boolean exported;
            List<AutoPtr<IResolveInfo> >::ReverseIterator it;
            for (it = mCurrentResolveList->RBegin(); it != mCurrentResolveList->REnd(); ) {
                AutoPtr<IActivityInfo> ai;
                (*it)->GetActivityInfo((IActivityInfo**)&ai);
                ai->GetPermission(&permission);
                AutoPtr<IApplicationInfo> appInfo;
                ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
                appInfo->GetUid(&uid);
                ai->GetExported(&exported);
                amHelper->CheckComponentPermission(
                    permission, mLaunchedFromUid, uid, exported, &granted);
                if (granted != IPackageManager::PERMISSION_GRANTED) {
                    // Access not allowed!
                    it = List<AutoPtr<IResolveInfo> >::ReverseIterator(
                        mCurrentResolveList->Erase((++it).GetBase()));
                }
                else {
                    ++it;
                }
            }
        }
    }

    if (mCurrentResolveList != NULL) {
        return;
    }

    Int32 N = mCurrentResolveList->GetSize();
    if (N > 0) {
        // Only display the first matches that are either of equal
        // priority or have asked to be default options.
        AutoPtr<IResolveInfo> r0 = (*mCurrentResolveList)[0];
        Int32 i = 0;
        Boolean erased = FALSE;
        List<AutoPtr<IResolveInfo> >::Iterator it;
        for (it = mCurrentResolveList->Begin(); it != mCurrentResolveList->End(); ++i) {
            AutoPtr<IResolveInfo> ri = *it;
            Int32 rPriority;
            r0->GetPriority(&rPriority);
            Boolean rIsDefault;
            r0->GetIsDefault(&rIsDefault);
            Int32 riPriority;
            ri->GetPriority(&riPriority);
            Boolean riIsDefault;
            ri->GetIsDefault(&riIsDefault);
            if (FALSE) {
                AutoPtr<IActivityInfo> rActivityInfo;
                r0->GetActivityInfo((IActivityInfo**)&rActivityInfo);
                String rName;
                rActivityInfo->GetName(&rName);
                AutoPtr<IActivityInfo> riActivityInfo;
                ri->GetActivityInfo((IActivityInfo**)&riActivityInfo);
                String riName;
                riActivityInfo->GetName(&riName);

                StringBuilder sb("ResolveListActivity");
                sb += rName;
                sb += "=";
                sb += rPriority;
                sb += "/";
                sb += rIsDefault;
                sb += " vs ";
                sb += riName;
                sb += "=";
                sb += riPriority;
                sb += "/";
                sb += riIsDefault;
                Slogger::V("ResolverActivity", sb.ToString());
            }

            erased = FALSE;
            if (rPriority != riPriority || rIsDefault != riIsDefault) {
                while (i < N) {
                    erased = TRUE;
                    it = mCurrentResolveList->Erase(it);
                    N--;
                }
            }

            if (!erased) ++it;
        }

        N = mCurrentResolveList->GetSize();
        if (N > 1) {
            AutoPtr<IResolveInfoDisplayNameComparator> rComparator;
//            CResolveInfoDisplayNameComparator::New(mPm,
//                    (IResolveInfoDisplayNameComparator**)&rComparator);
//             Collections.sort(mCurrentResolveList, rComparator);
        }

        mList.Clear();

        // First put the initial items at the top.
        if (mInitialIntents != NULL) {
            for (Int32 i = 0; i < mInitialIntents->GetLength(); i++) {
                AutoPtr<IIntent> ii = (*mInitialIntents)[i];
                if (ii == NULL) {
                    continue;
                }
                AutoPtr<IPackageManager> pkgMgr;
                mHost->GetPackageManager((IPackageManager**)&pkgMgr);
                AutoPtr<IActivityInfo> ai;
                ii->ResolveActivityInfo(pkgMgr, 0, (IActivityInfo**)&ai);
                if (ai == NULL) {
                    Slogger::W("ResolverActivity", "No activity found for %p", ii.Get());
                    continue;
                }
                AutoPtr<IResolveInfo> ri;
                CResolveInfo::New((IResolveInfo**)&ri);
                ri->SetActivityInfo(ai);
                if (ii->Probe(EIID_ILabeledIntent)) {
                    AutoPtr<ILabeledIntent> li = (ILabeledIntent*)ii->Probe(EIID_ILabeledIntent);
                    String sourcePkg;
                    li->GetSourcePackage(&sourcePkg);
                    ri->SetResolvePackageName(sourcePkg);
                    Int32 labelRes;
                    li->GetLabelResource(&labelRes);
                    ri->SetLabelRes(labelRes);
                    AutoPtr<ICharSequence> label;
                    li->GetNonLocalizedLabel((ICharSequence**)&label);
                    ri->SetNonLocalizedLabel(label);
                    Int32 iconRes;
                    li->GetIconResource(&iconRes);
                    ri->SetIcon(iconRes);
                }
                AutoPtr<ICharSequence> label;
                ri->LoadLabel(pkgMgr, (ICharSequence**)&label);
                AutoPtr<DisplayResolveInfo> info = new DisplayResolveInfo(ri, label, NULL, ii);
                mList.PushBack(info);
            }
        }

        // Check for applications with same name and use application name or
        // package name if necessary
        r0 = (*mCurrentResolveList)[0];
        Int32 start = 0;
        AutoPtr<ICharSequence> r0Label;
        r0->LoadLabel(mHost->mPm, (ICharSequence**)&r0Label);
        mHost->mShowExtended = FALSE;
        it = mCurrentResolveList->Begin();
        ++it;
        for (; it != mCurrentResolveList->End(); ++it) {
            Int32 i = 1;
            String r0pkgName, ripkgName;
            if (r0Label == NULL) {
                AutoPtr<IActivityInfo> aInfo;
                r0->GetActivityInfo((IActivityInfo**)&aInfo);
                aInfo->GetPackageName(&r0pkgName);
                CString::New(r0pkgName, (ICharSequence**)&r0Label);
            }
            AutoPtr<IResolveInfo> ri = *it;
            AutoPtr<ICharSequence> riLabel;
            ri->LoadLabel(mHost->mPm, (ICharSequence**)&riLabel);
            if (riLabel == NULL) {
                AutoPtr<IActivityInfo> aInfo;
                ri->GetActivityInfo((IActivityInfo**)&aInfo);
                aInfo->GetPackageName(&ripkgName);
                CString::New(ripkgName, (ICharSequence**)&riLabel);
            }
            if (ripkgName.Equals(r0pkgName)) {
                continue;
            }
            ProcessGroup(mCurrentResolveList, start, (i-1), r0, r0Label);
            r0 = ri;
            r0Label = riLabel;
            start = i;
            i++;
        }
        // Process last group
        ProcessGroup(mCurrentResolveList, start, (N-1), r0, r0Label);
    }
}

void ResolverActivity::ResolveListAdapter::ProcessGroup(
    /* [in] */ List<AutoPtr<IResolveInfo> >* rList,
    /* [in] */ Int32 start,
    /* [in] */ Int32 end,
    /* [in] */ IResolveInfo* ro,
    /* [in] */ ICharSequence* roLabel)
{
    // Process labels from start to i
    Int32 num = end - start+1;
    if (num == 1) {
        // No duplicate labels. Use label for entry at start
        AutoPtr<DisplayResolveInfo> info = new DisplayResolveInfo(ro, roLabel, NULL, NULL);
        mList.PushBack(info);
    } else {
        mHost->mShowExtended = TRUE;
        Boolean usePkg = FALSE;
        AutoPtr<IActivityInfo> aInfo;
        ro->GetActivityInfo((IActivityInfo**)&aInfo);
        AutoPtr<IApplicationInfo> appInfo;
        aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
        AutoPtr<ICharSequence> startApp;
        appInfo->LoadLabel(mHost->mPm, (ICharSequence**)&startApp);
        if (startApp == NULL) {
            usePkg = TRUE;
        }
        if (!usePkg) {
            // Use HashSet to track duplicates
            AutoPtr<HashSet<AutoPtr<ICharSequence> > > duplicates = new HashSet<AutoPtr<ICharSequence> >();
            duplicates->Insert(startApp);
            List<AutoPtr<IResolveInfo> >::Iterator it = rList->Begin();
            Int32 j = 0;
            while (j < start + 1) {
                ++it;
                ++j;
            }
            for (; it != rList->End(); ++it) {
                AutoPtr<IResolveInfo> jRi = *it;
                AutoPtr<IActivityInfo> aInfo;
                jRi->GetActivityInfo((IActivityInfo**)&aInfo);
                AutoPtr<IApplicationInfo> appInfo;
                aInfo->GetApplicationInfo((IApplicationInfo**)&appInfo);
                AutoPtr<ICharSequence> jApp;
                appInfo->LoadLabel(mHost->mPm, (ICharSequence**)&jApp);
                HashSet<AutoPtr<ICharSequence> >::Iterator hit = duplicates->Find(jApp);
                Boolean finded = hit != duplicates->End() ? TRUE : FALSE;
                if ( (jApp == NULL) || finded) {
                    usePkg = TRUE;
                    break;
                } else {
                    duplicates->Insert(jApp);
                }
            }
            // Clear HashSet for later use
            duplicates = NULL;
        }
        List<AutoPtr<IResolveInfo> >::Iterator it = rList->Begin();
        Int32 k = 0;
        while (k < start) {
            ++it;
            ++k;
        }
        for (; it != rList->End(); ++it) {
            AutoPtr<IResolveInfo> add = *it;
            if (usePkg) {
                // Use application name for all entries from start to end-1
                AutoPtr<IActivityInfo> aaInfo;
                add->GetActivityInfo((IActivityInfo**)&aaInfo);
                String aname;
                aaInfo->GetPackageName(&aname);
                AutoPtr<ICharSequence> cname;
                CString::New(aname, (ICharSequence**)&cname);
                AutoPtr<DisplayResolveInfo> info =
                        new DisplayResolveInfo(add, roLabel, cname, NULL);
                mList.PushBack(info);
            } else {
                // Use package name for all entries from start to end-1
                AutoPtr<IActivityInfo> aaInfo;
                add->GetActivityInfo((IActivityInfo**)&aaInfo);
                AutoPtr<IApplicationInfo> aappInfo;
                aaInfo->GetApplicationInfo((IApplicationInfo**)&aappInfo);
                AutoPtr<ICharSequence> label;
                aappInfo->LoadLabel(mHost->mPm, (ICharSequence**)&label);
                AutoPtr<DisplayResolveInfo> info =
                        new DisplayResolveInfo(add, roLabel, label, NULL);
                mList.PushBack(info);
            }
        }
    }
}

ECode ResolverActivity::ResolveListAdapter::ResolveInfoForPosition(
    /* [in] */ Int32 position,
            /* [out] */ IResolveInfo** resInfo)
{
    VALIDATE_NOT_NULL(position);
    VALIDATE_NOT_NULL(resInfo);

    if (mList.IsEmpty()) {
        *resInfo = NULL;
        return NOERROR;
    }

    *resInfo = mList[position]->mRi;
    REFCOUNT_ADD(*resInfo);
    return NOERROR;
}

ECode ResolverActivity::ResolveListAdapter::IntentForPosition(
    /* [in] */ Int32 position,
            /* [out] */ IIntent** bkIntent)
{
    VALIDATE_NOT_NULL(position);
    VALIDATE_NOT_NULL(bkIntent);

    if (mList.IsEmpty()) {
        *bkIntent = NULL;
        return NOERROR;
    }

    AutoPtr<DisplayResolveInfo> dri = mList[position];

    AutoPtr<IIntent> intent;
    CIntent::New(dri->mOrigIntent != NULL
            ? dri->mOrigIntent : mIntent, (IIntent**)&intent);
    intent->AddFlags(IIntent::FLAG_ACTIVITY_FORWARD_RESULT
            |IIntent::FLAG_ACTIVITY_PREVIOUS_IS_TOP);
    AutoPtr<IActivityInfo> ai;
    dri->mRi->GetActivityInfo((IActivityInfo**)&ai);
    AutoPtr<IApplicationInfo> appInfo;
    ai->GetApplicationInfo((IApplicationInfo**)&appInfo);
    String pkgName;
    appInfo->GetPackageName(&pkgName);
    String name;
    ai->GetName(&name);
    AutoPtr<IComponentName> component;
    CComponentName::New(pkgName, name, (IComponentName**)&component);
    intent->SetComponent(component);
    *bkIntent = intent;
    REFCOUNT_ADD(*bkIntent);
    return NOERROR;
}

ECode ResolverActivity::ResolveListAdapter::GetCount(
            /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    *count = mList.GetSize();
    return NOERROR;
}

ECode ResolverActivity::ResolveListAdapter::GetItem(
    /* [in] */ Int32 position,
    /* [out] */ IInterface** item)
{
    VALIDATE_NOT_NULL(item);
//    return position;
    return E_NOT_IMPLEMENTED;
}

ECode ResolverActivity::ResolveListAdapter::GetItemId(
    /* [in] */ Int32 position,
    /* [out] */ Int64* id)
{
    VALIDATE_NOT_NULL(id);

    *id = position;
    return NOERROR;
}

ECode ResolverActivity::ResolveListAdapter::GetView(
    /* [in] */ Int32 position,
    /* [in] */ IView* convertView,
    /* [in] */ IViewGroup* parent,
    /* [out] */ IView** bkView)
{
    VALIDATE_NOT_NULL(bkView);
    AutoPtr<IView> view;
    if (convertView == NULL) {
        mInflater->Inflate(R::layout::resolve_list_item, parent, FALSE, (IView**)&view);

        // Fix the icon size even if we have different sized resources
        AutoPtr<IImageView> icon;
        view->FindViewById(0x01020006/*R.id.icon*/, (IView**)(IImageView**)&icon);
        AutoPtr<IViewGroupLayoutParams> lp;
        icon->GetLayoutParams((IViewGroupLayoutParams**)&lp);
        lp->SetHeight(mHost->mIconSize);
        lp->SetWidth(mHost->mIconSize);
    } else {
        view = convertView;
    }
    BindView(view, mList[position]);
    *bkView = view;
    REFCOUNT_ADD(*bkView);
    return NOERROR;
}

void ResolverActivity::ResolveListAdapter::BindView(
    /* [in] */ IView* view,
    /* [in] */ DisplayResolveInfo* info)
{
    AutoPtr<ITextView> text;
    view->FindViewById(0x01020014/*com.android.internal.R.id.text1*/, (IView**)(ITextView**)&text);
    AutoPtr<ITextView> text2;
    view->FindViewById(0x01020015/*com.android.internal.R.id.text2*/, (IView**)(ITextView**)&text2);
    AutoPtr<IImageView> icon;
    view->FindViewById(0x01020006/*R.id.icon*/, (IView**)(IImageView**)&icon);
    text->SetText(info->mDisplayLabel);
    if (mHost->mShowExtended) {
        text2->SetVisibility(IView::VISIBLE);
        text2->SetText(info->mExtendedInfo);
    } else {
        text2->SetVisibility(IView::GONE);
    }
    if (info->mDisplayIcon == NULL) {
         mHost->LoadIconForResolveInfo(info->mRi, (IDrawable**)&info->mDisplayIcon);
    }
    icon->SetImageDrawable(info->mDisplayIcon);
}

CAR_INTERFACE_IMPL(ResolverActivity::ItemLongClickListener, IAdapterViewOnItemLongClickListener)

ResolverActivity::ItemLongClickListener::OnItemLongClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id,
    /* [out] */ Boolean* clicked)
{
    VALIDATE_NOT_NULL(clicked);
    AutoPtr<IResolveInfo> ri;
    mHost->mAdapter->ResolveInfoForPosition(position, (IResolveInfo**)&ri);
    mHost->ShowAppDetails(ri);
    *clicked = TRUE;
    return NOERROR;
}

} // namespace App
} // namespace Internal
} // namespace Droid
} // namespace Elastos
