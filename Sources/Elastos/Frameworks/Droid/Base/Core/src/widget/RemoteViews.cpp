
#include "widget/RemoteViews.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/core/StringBuilder.h>
#include "text/TextUtils.h"
#include "graphics/CRect.h"
#include "graphics/CBitmap.h"
#include "content/CIntent.h"
#include <elastos/utility/etl/HashMap.h>
#include "os/Build.h"
#include "os/Process.h"
#include "os/CBundle.h"
#include "app/CActivityOptionsHelper.h"
#include "widget/CRemoteViews.h"
#include "widget/CRemoteViewsBitmapCache.h"
#include <R.h>
#include <elastos/core/StringUtils.h>

using Elastos::Core::StringUtils;
using Elastos::Core::IBoolean;
using Elastos::Core::CBoolean;
using Elastos::Core::IByte;
using Elastos::Core::CByte;
using Elastos::Core::IInteger16;
using Elastos::Core::CInteger16;
using Elastos::Core::IInteger32;
using Elastos::Core::CInteger32;
using Elastos::Core::IInteger64;
using Elastos::Core::CInteger64;
using Elastos::Core::IFloat;
using Elastos::Core::CFloat;
using Elastos::Core::IDouble;
using Elastos::Core::CDouble;
using Elastos::Core::IChar32;
using Elastos::Core::CChar32;
using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Utility::Etl::HashMap;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Content::Res::IConfiguration;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::PorterDuffMode_CLEAR;
using Elastos::Droid::Graphics::BitmapConfig;
using Elastos::Droid::Graphics::BitmapConfig_ALPHA_8;
using Elastos::Droid::Graphics::BitmapConfig_RGB_565;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_4444;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::IViewParent;
using Elastos::Droid::View::ILayoutInflater;
using Elastos::Droid::View::ILayoutInflaterFilter;
using Elastos::Droid::View::EIID_ILayoutInflaterFilter;
using Elastos::Droid::Widget::IImageView;
using Elastos::Droid::Widget::ITextView;
using Elastos::Droid::Widget::IAdapterView;
using Elastos::Droid::Widget::EIID_IRemoteViews;
using Elastos::Droid::Widget::EIID_RemoteViews;
using Elastos::Droid::Widget::CRemoteViews;
using Elastos::Droid::App::IActivityOptionsHelper;
using Elastos::Droid::App::CActivityOptionsHelper;
using Elastos::Droid::App::IActivityOptions;
using Elastos::Droid::Content::IIntentSender;
using Elastos::Utility::Logging::Logger;
using Elastos::Utility::Logging::Slogger;
// using Elastos::Droid::Appwidget::IAppWidgetHostView;


namespace Elastos {
namespace Droid {
namespace Widget {

//0c9ce335-e934-4473-aef9-a9379eebee53
extern "C" const InterfaceID EIID_RemoteViews =
        { 0x0c9ce335, 0xe934, 0x4473, { 0xae, 0xf9, 0xa9, 0x37, 0x9e, 0xeb, 0xee, 0x53 } };

static AutoPtr<IRemoteViewsOnClickHandler> InitHandler()
{
    AutoPtr<IRemoteViewsOnClickHandler> temp = new RemoteViewsOnClickHandler();
    return temp;
}

const String RemoteViews::EXTRA_REMOTEADAPTER_APPWIDGET_ID("remoteAdapterAppWidgetId");
const Int32 RemoteViews::MODE_NORMAL;
const Int32 RemoteViews::MODE_HAS_LANDSCAPE_AND_PORTRAIT;
const AutoPtr<IRemoteViewsOnClickHandler> RemoteViews::DEFAULT_ON_CLICK_HANDLER = InitHandler();

static String TAG("RemoteViews");

//==============================================================================
//                  RemoteViewsOnClickHandler
//==============================================================================
CAR_INTERFACE_IMPL(RemoteViewsOnClickHandler, IRemoteViewsOnClickHandler)

ECode RemoteViewsOnClickHandler::OnClickHandler(
    /* [in] */ IView* view,
    /* [in] */ IPendingIntent* pendingIntent,
    /* [in] */ IIntent* fillInIntent,
    /* [out] */ Boolean* succeed)
{
    VALIDATE_NOT_NULL(succeed);
    assert(pendingIntent != NULL);

    // try {
        // TODO: Unregister this handler if PendingIntent.FLAG_ONE_SHOT?
    AutoPtr<IContext> context;
    view->GetContext((IContext**)&context);
    Int32 w, h;
    view->GetMeasuredWidth(&w);
    view->GetMeasuredHeight(&h);
    AutoPtr<IActivityOptionsHelper> helper;
    CActivityOptionsHelper::AcquireSingleton((IActivityOptionsHelper**)&helper);
    AutoPtr<IActivityOptions> opts;
    helper->MakeScaleUpAnimation(view, 0, 0, w, h, (IActivityOptions**)&opts);
    AutoPtr<IBundle> bundle;
    opts->ToBundle((IBundle**)&bundle);
    AutoPtr<IIntentSender> sender;
    pendingIntent->GetIntentSender((IIntentSender**)&sender);
    ECode ec = context->StartIntentSender(sender, fillInIntent,
        IIntent::FLAG_ACTIVITY_NEW_TASK,
        IIntent::FLAG_ACTIVITY_NEW_TASK, 0, bundle);
    if (FAILED(ec)) {
        *succeed = FALSE;
        return NOERROR;
    }
    // } catch (IntentSender.SendIntentException e) {
    //     android.util.Log.e(LOG_TAG, "Cannot send pending intent: ", e);
    //     return false;
    // } catch (Exception e) {
    //     android.util.Log.e(LOG_TAG, "Cannot send pending intent due to " +
    //             "unknown exception: ", e);
    //     return false;
    // }
    *succeed = TRUE;
    return NOERROR;
}

//==============================================================================
//                  MemoryUsageCounter
//==============================================================================

MemoryUsageCounter::MemoryUsageCounter()
    : mMemoryUsage(0)
{
}

void MemoryUsageCounter::Clear()
{
    mMemoryUsage = 0;
}

void MemoryUsageCounter::Increment(
    /* [in */ Int32 numBytes)
{
    mMemoryUsage += numBytes;
}

Int32 MemoryUsageCounter::GetMemoryUsage()
{
    return mMemoryUsage;
}

void MemoryUsageCounter::AddBitmapMemory(
    /* [in */ IBitmap* b)
{
    BitmapConfig c;
    b->GetConfig(&c);
    // If we don't know, be pessimistic and assume 4
    int bpp = 4;
    switch (c) {
    case BitmapConfig_ALPHA_8:
        bpp = 1;
        break;
    case BitmapConfig_RGB_565:
    case BitmapConfig_ARGB_4444:
        bpp = 2;
        break;
    case BitmapConfig_ARGB_8888:
        bpp = 4;
        break;
    }

    Int32 w, h;
    b->GetWidth(&w);
    b->GetHeight(&h);
    Increment(w * h * bpp);
}

//==============================================================================
//                  RemoteViews::Action
//==============================================================================
const Int32 RemoteViews::Action::MERGE_REPLACE;
const Int32 RemoteViews::Action::MERGE_APPEND;
const Int32 RemoteViews::Action::MERGE_IGNORE;

CAR_INTERFACE_IMPL(RemoteViews::Action, IParcelable)

RemoteViews::Action::Action()
    : mViewId(0)
{}

RemoteViews::Action::Action(
    /* [in] */ Int32 viewId)
    : mViewId(viewId)
{}

Int32 RemoteViews::Action::DescribeContents()
{
    return 0;
}

void RemoteViews::Action::UpdateMemoryUsageEstimate(
    /* [in] */ MemoryUsageCounter* counter)
{
    // We currently only calculate Bitmap memory usage, so by default, don't do anything
    // here
    return;
}

void RemoteViews::Action::SetBitmapCache(
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    // Do nothing
}

Int32 RemoteViews::Action::MergeBehavior()
{
    return MERGE_REPLACE;
}

String RemoteViews::Action::GetUniqueKey()
{
    StringBuilder sb(GetActionName());
    sb += "[viewId:0x";
    sb += StringUtils::Int32ToHexString(mViewId);
    sb += "]";
    return sb.ToString();
}


//==============================================================================
//                  RemoteViews::_SetEmptyView
//==============================================================================
const Int32 RemoteViews::_SetEmptyView::TAG;

RemoteViews::_SetEmptyView::_SetEmptyView()
    : mEmptyViewId(0)
{}

RemoteViews::_SetEmptyView::_SetEmptyView(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 emptyViewId)
    : Action(viewId)
    , mEmptyViewId(emptyViewId)
{}

void RemoteViews::_SetEmptyView::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IView> view;
    root->FindViewById(mViewId, (IView**)&view);
    AutoPtr<IAdapterView> adapterView = IAdapterView::Probe(view);
    if (adapterView == NULL) return;

    AutoPtr<IView> emptyView;
    root->FindViewById(mEmptyViewId, (IView**)&emptyView);
    if (emptyView == NULL) return;

    adapterView->SetEmptyView(emptyView);
}

String RemoteViews::_SetEmptyView::GetActionName()
{
    return String("SetEmptyView");
}

ECode RemoteViews::_SetEmptyView::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    source->ReadInt32(&mEmptyViewId);
    return NOERROR;
}

ECode RemoteViews::_SetEmptyView::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInt32(mEmptyViewId);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::_SetOnClickFillInIntent
//==============================================================================
const Int32 RemoteViews::_SetOnClickFillInIntent::TAG;

RemoteViews::_SetOnClickFillInIntent::_SetOnClickFillInIntent(
    /* [in] */ RemoteViews* host)
    : mHost(host)
{}

RemoteViews::_SetOnClickFillInIntent::_SetOnClickFillInIntent(
    /* [in] */ Int32 id,
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ RemoteViews* host)
    : Action(id)
    , mFillInIntent(fillInIntent)
    , mHost(host)
{}

void RemoteViews::_SetOnClickFillInIntent::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IView> target;
    root->FindViewById(mViewId, (IView**)&target);
    if (target == NULL) return;

    if (!mHost->mIsWidgetCollectionChild) {
        return;
    }
    if (target.Get() == root) {
        AutoPtr<IObject> obj = IObject::Probe(mFillInIntent);
        assert(0 && "TODO");
        target->SetTagInternal(R::id::fillInIntent, obj);
    }
    else if (target && mFillInIntent) {
        AutoPtr<FillInIntentClickListener> listener = new FillInIntentClickListener(mFillInIntent, handler);
        target->SetOnClickListener(listener.Get());
    }
}

String RemoteViews::_SetOnClickFillInIntent::GetActionName()
{
    return String("SetOnClickFillInIntent");
}

ECode RemoteViews::_SetOnClickFillInIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    mFillInIntent = NULL;
    source->ReadInterfacePtr((Handle32*)&mFillInIntent);
    return NOERROR;
}

ECode RemoteViews::_SetOnClickFillInIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInterfacePtr(mFillInIntent);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::_SetPendingIntentTemplate
//==============================================================================
const Int32 RemoteViews::_SetPendingIntentTemplate::TAG;

RemoteViews::_SetPendingIntentTemplate::_SetPendingIntentTemplate()
{}

RemoteViews::_SetPendingIntentTemplate::_SetPendingIntentTemplate(
    /* [in] */ Int32 id,
    /* [in] */ IPendingIntent* pendingIntentTemplate)
    : Action(id)
    , mPendingIntentTemplate(pendingIntentTemplate)
{}

void RemoteViews::_SetPendingIntentTemplate::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IView> target;
    root->FindViewById(mViewId, (IView**)&target);
    if (target == NULL) return;

    AutoPtr<IAdapterView> av = IAdapterView::Probe(target);
    if (av) {
        AutoPtr<IntentTemplateOnItemClickListener> listener = new IntentTemplateOnItemClickListener(mPendingIntentTemplate, handler);
        av->SetOnItemClickListener(listener);
        av->SetTag(mPendingIntentTemplate);
    }
    else {
        return;
    }
}

String RemoteViews::_SetPendingIntentTemplate::GetActionName()
{
    return String("SetPendingIntentTemplate");
}

// interface IParcelable
ECode RemoteViews::_SetPendingIntentTemplate::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    mPendingIntentTemplate = NULL;
    source->ReadInterfacePtr((Handle32*)&mPendingIntentTemplate);
    //pendingIntentTemplate = PendingIntent.readPendingIntentOrNullFromParcel(parcel);
    return NOERROR;
}

ECode RemoteViews::_SetPendingIntentTemplate::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInterfacePtr(mPendingIntentTemplate);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::SetRemoteViewsAdapterIntent
//==============================================================================
const Int32 RemoteViews::SetRemoteViewsAdapterIntent::TAG;

RemoteViews::SetRemoteViewsAdapterIntent::SetRemoteViewsAdapterIntent()
{}

RemoteViews::SetRemoteViewsAdapterIntent::SetRemoteViewsAdapterIntent(
    /* [in] */ Int32 id,
    /* [in] */ IIntent* intent)
    : Action(id)
    , mIntent(intent)
{}

void RemoteViews::SetRemoteViewsAdapterIntent::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr <IView> target;
    root->FindViewById(mViewId, (IView**)&target);
    if (target == NULL) return;

    assert(0 && "TODO");
    /*if (!
    IAppWidgetHostView::Probe(rootParent)) {
        return;
    }
    if (!IAbsListView::Probe(target) && !IAdapterViewAnimator::Probe(target)) {
        return;
    }
    AutoPtr<IAppWidgetHostView> host = IAppWidgetHostView::Probe(rootParent);
    Int32 hostId = 0;
    host->GetAppWidgetId(&hostId);
    mIntent->PutExtra(EXTRA_REMOTEADAPTER_APPWIDGET_ID, hostId);
    AutoPtr<IAbsListView> absListView = IAbsListView::Probe(target);
    AutoPtr<IAdapterViewAnimator> viewAnimator = IAdapterViewAnimator::Probe(target);
    if (absListView) {
        absListView->SetRemoteViewsAdapter(mIntent);
        absListView->SetRemoteViewsOnClickHandler(handler);
    } else if (viewAnimator) {
        viewAnimator->SetRemoteViewsAdapter(mIntent);
        viewAnimator->SetRemoteViewsOnClickHandler(handler);
    }*/
}

String RemoteViews::SetRemoteViewsAdapterIntent::GetActionName()
{
    return String("SetRemoteViewsAdapterIntent");
}

// interface IParcelable
ECode RemoteViews::SetRemoteViewsAdapterIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    mIntent = NULL;
    source->ReadInterfacePtr((Handle32*)&mIntent);
    return NOERROR;
}

ECode RemoteViews::SetRemoteViewsAdapterIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInterfacePtr(mIntent);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::_SetOnClickPendingIntent
//==============================================================================
const Int32 RemoteViews::_SetOnClickPendingIntent::TAG;

RemoteViews::_SetOnClickPendingIntent::_SetOnClickPendingIntent(
    /* [in] */ RemoteViews* host)
    : mHost(host)
{}

RemoteViews::_SetOnClickPendingIntent::_SetOnClickPendingIntent(
    /* [in] */ Int32 id,
    /* [in] */ IPendingIntent* pendingIntent,
    /* [in] */ RemoteViews* host)
    : Action(id)
    , mPendingIntent(pendingIntent)
    , mHost(host)
{}

void RemoteViews::_SetOnClickPendingIntent::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IView> target;
    root->FindViewById(mViewId, (IView**)&target);
    if (target == NULL) return;

    if (mHost->mIsWidgetCollectionChild) {
        AutoPtr<IContext> context;
        root->GetContext((IContext**)&context);
        AutoPtr<IApplicationInfo> appInfo;
        context->GetApplicationInfo((IApplicationInfo**)&appInfo);
        Int32 version = 0;
        if (appInfo != NULL &&
                (appInfo->GetTargetSdkVersion(&version), version >= Build::VERSION_CODES::JELLY_BEAN)) {
            return;
        }
    }

    if (target != NULL) {
        AutoPtr<PendingIntentClickListener> listener;
        if (mPendingIntent) {
            listener = new PendingIntentClickListener(mPendingIntent, handler);
            target->SetOnClickListener(listener.Get());
        }
    }
}

String RemoteViews::_SetOnClickPendingIntent::GetActionName()
{
    return String("SetOnClickPendingIntent");
}

// interface IParcelable
ECode RemoteViews::_SetOnClickPendingIntent::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);

    Int32 temp = 0;
    source->ReadInt32(&temp);
    if (temp == 1) {
        mPendingIntent = NULL;
        source->ReadInterfacePtr((Handle32*)&mPendingIntent);
    }

    return NOERROR;
}

ECode RemoteViews::_SetOnClickPendingIntent::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    if (mPendingIntent != NULL) {
        dest->WriteInt32(1);
        dest->WriteInterfacePtr(mPendingIntent);
    }
    else {
        dest->WriteInt32(0);
    }
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::_SetDrawableParameters
//==============================================================================
const Int32 RemoteViews::_SetDrawableParameters::TAG;

RemoteViews::_SetDrawableParameters::_SetDrawableParameters()
    : mTargetBackground(FALSE)
    , mAlpha(0)
    , mColorFilter(0)
    , mFilterMode(PorterDuffMode_CLEAR)
    , mLevel(0)
{}

RemoteViews::_SetDrawableParameters::_SetDrawableParameters(
    /* [in] */ Int32 id,
    /* [in] */ Boolean targetBackground,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 colorFilter,
    /* [in] */ PorterDuffMode mode,
    /* [in] */ Int32 level)
    : Action(id)
    , mTargetBackground(targetBackground)
    , mAlpha(alpha)
    , mColorFilter(colorFilter)
    , mFilterMode(mode)
    , mLevel(level)
{}

void RemoteViews::_SetDrawableParameters::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IView> target;
    root->FindViewById(mViewId, (IView**)&target);
    if (target == NULL) return;

    AutoPtr<IDrawable> targetDrawable;
    if (mTargetBackground) {
        target->GetBackground((IDrawable**)&targetDrawable);
    }
    else if (IImageView::Probe(target) !=  NULL) {
        AutoPtr<IImageView> imageview = IImageView::Probe(target);
        imageview->GetDrawable((IDrawable**)&targetDrawable);
    }

    if (targetDrawable != NULL) {
        if (mAlpha != -1) {
            targetDrawable->SetAlpha(mAlpha);
        }
        if (mColorFilter != -1 && mFilterMode) {
            targetDrawable->SetColorFilter(mColorFilter, mFilterMode);
        }
        if (mLevel != -1) {
            Boolean res = FALSE;
            targetDrawable->SetLevel(mLevel, &res);
        }
    }
}

String RemoteViews::_SetDrawableParameters::GetActionName()
{
    return String("SetDrawableParameters");
}

ECode RemoteViews::_SetDrawableParameters::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    Int32 background = 0;
    source->ReadInt32(&background);
    mTargetBackground = background != 0;
    source->ReadInt32(&mAlpha);
    source->ReadInt32(&mColorFilter);
    Int32 mode = 0;
    source->ReadInt32(&mode);
    Boolean hasMode = mode != 0;
    if (hasMode) {
        assert(0 && "TODO");
        //mFilterMode = PorterDuff.Mode.valueOf(parcel.readString());
    }
    else {
        mFilterMode = PorterDuffMode_CLEAR;
    }
    source->ReadInt32(&mLevel);
    return NOERROR;
}

ECode RemoteViews::_SetDrawableParameters::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInt32(mTargetBackground ? 1 : 0);
    dest->WriteInt32(mAlpha);
    dest->WriteInt32(mColorFilter);
    if (mFilterMode != PorterDuffMode_CLEAR) {
        dest->WriteInt32(1);
        assert(0 && "TODO");
        //dest->WriteString(mFilterMode.tostring());
    }
    else {
        dest->WriteInt32(0);
    }
    dest->WriteInt32(mLevel);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::ReflectionActionWithoutParams
//==============================================================================
const Int32 RemoteViews::ReflectionActionWithoutParams::TAG;

RemoteViews::ReflectionActionWithoutParams::ReflectionActionWithoutParams()
{}

RemoteViews::ReflectionActionWithoutParams::ReflectionActionWithoutParams(
    /* [in] */ Int32 id,
    /* [in] */ const String& methodName)
    : Action(id)
    , mMethodName(methodName)
{}

void RemoteViews::ReflectionActionWithoutParams::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr <IView> view;
    root->FindViewById(mViewId, (IView**)&view);
    if (view == NULL) return;

    assert(0 && "TODO");
//    final View view = root.findViewById(viewId);
//    if (view == null) return;
//
//    Class klass = view.getClass();
//    Method method;
//    try {
//        method = klass.getMethod(this.methodName);
//    } catch (NoSuchMethodException ex) {
//        throw new ActionException("view: " + klass.getName() + " doesn't have method: "
//                + this.methodName + "()");
//    }
//
//    if (!method.isAnnotationPresent(RemotableViewMethod.class)) {
//        throw new ActionException("view: " + klass.getName()
//                + " can't use method with RemoteViews: "
//                + this.methodName + "()");
//    }
//
//    try {
//        //noinspection ConstantIfStatement
//        if (false) {
//            Log.d("RemoteViews", "view: " + klass.getName() + " calling method: "
//                + this.methodName + "()");
//        }
//        method.invoke(view);
//    } catch (Exception ex) {
//        throw new ActionException(ex);
//    }
}

String RemoteViews::ReflectionActionWithoutParams::GetActionName()
{
    return String("ReflectionActionWithoutParams");
}

Int32 RemoteViews::ReflectionActionWithoutParams::MergeBehavior()
{
    if (mMethodName.Equals("ShowNext") || mMethodName.Equals("ShowPrevious")) {
        return MERGE_IGNORE;
    }
    else {
        return MERGE_REPLACE;
    }
}

// interface IParcelable
ECode RemoteViews::ReflectionActionWithoutParams::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    source->ReadString(&mMethodName);
    return NOERROR;
}

ECode RemoteViews::ReflectionActionWithoutParams::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteString(mMethodName);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::BitmapReflectionAction
//==============================================================================
const Int32 RemoteViews::BitmapReflectionAction::TAG;

RemoteViews::BitmapReflectionAction::BitmapReflectionAction(
    /* [in] */ RemoteViews* host)
    : mBitmapId(0)
    , mHost(host)
{}

RemoteViews::BitmapReflectionAction::BitmapReflectionAction(
    /* [in] */ Int32 id,
    /* [in] */ const String& methodName,
    /* [in] */ IBitmap* bitmap,
    /* [in] */ RemoteViews* host)
    : Action(id)
    , mBitmapId(0)
    , mBitmap(bitmap)
    , mMethodName(methodName)
    , mHost(host)
{
    mHost->mBitmapCache->GetBitmapId(bitmap, &mBitmapId);
}

void RemoteViews::BitmapReflectionAction::SetBitmapCache(
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    bitmapCache->GetBitmapId(mBitmap, &mBitmapId);
}

void RemoteViews::BitmapReflectionAction::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<ReflectionAction> ra = new ReflectionAction(mViewId, mMethodName, ReflectionAction::BITMAP, mBitmap);
    ra->Apply(root, rootParent, handler);
}

String RemoteViews::BitmapReflectionAction::GetActionName()
{
    return String("BitmapReflectionAction");
}

// interface IParcelable
ECode RemoteViews::BitmapReflectionAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    source->ReadString(&mMethodName);
    source->ReadInt32(&mBitmapId);
    mBitmap = NULL;
    mHost->mBitmapCache->GetBitmapForId(mBitmapId, (IBitmap**)&mBitmap);
    return NOERROR;
}

ECode RemoteViews::BitmapReflectionAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteString(mMethodName);
    dest->WriteInt32(mBitmapId);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::ReflectionAction
//==============================================================================
const Int32 RemoteViews::ReflectionAction::TAG;
const Int32 RemoteViews::ReflectionAction::BOOLEAN;
const Int32 RemoteViews::ReflectionAction::BYTE;
const Int32 RemoteViews::ReflectionAction::SHORT;
const Int32 RemoteViews::ReflectionAction::INT;
const Int32 RemoteViews::ReflectionAction::LONG;
const Int32 RemoteViews::ReflectionAction::FLOAT;
const Int32 RemoteViews::ReflectionAction::DOUBLE;
const Int32 RemoteViews::ReflectionAction::CHAR;
const Int32 RemoteViews::ReflectionAction::STRING;
const Int32 RemoteViews::ReflectionAction::CHAR_SEQUENCE;
const Int32 RemoteViews::ReflectionAction::URI;
const Int32 RemoteViews::ReflectionAction::BITMAP;
const Int32 RemoteViews::ReflectionAction::BUNDLE;
const Int32 RemoteViews::ReflectionAction::INTENT;

RemoteViews::ReflectionAction::ReflectionAction()
    : mType(0)
{
}

RemoteViews::ReflectionAction::ReflectionAction(
    /* [in] */ Int32 id,
    /* [in] */ const String& methodName,
    /* [in] */ Int32 type,
    /* [in] */ IInterface* value)
    : Action(id)
    , mMethodName(methodName)
    , mType(type)
    , mValue(value)
{}

String RemoteViews::ReflectionAction::GetParameterType()
{
    switch (mType) {
        case BOOLEAN:
        {
            return String("Boolean");
        }
        case BYTE:
        {
            return String("Byte");
        }
        case SHORT:
        {
            return String("Int16");
        }
        case INT:
        {
            return String("Int32");
        }
        case LONG:
        {
            return String("Int64");
        }
        case FLOAT:
        {
            return String("Float");
        }
        case DOUBLE:
        {
            return String("Double");
        }
        case CHAR:
        {
            return String("Char32");
        }
        case STRING:
        {
            return String("String");
        }
        case CHAR_SEQUENCE:
        {
            return String("ICharSequence");
        }
        case URI:
        {
            return String("Uri");
        }
        case BITMAP:
        {
            return String("IBitmap");
        }
        case BUNDLE:
        {
            return String("IBundle");
        }
        case INTENT:
        {
            return String("IIntent");
        }
        default:
            return String("");
    }
}

Int32 RemoteViews::ReflectionAction::MergeBehavior()
{
    if (mMethodName.Equals(String("SmoothScrollBy"))) {
        return MERGE_APPEND;
    }
    else {
        return MERGE_REPLACE;
    }
}

void RemoteViews::ReflectionAction::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    // PEL("RemoteViews::ReflectionAction::Apply()")
    assert(root != NULL);
    AutoPtr<IView> view;
    root->FindViewById(mViewId, (IView**)&view);
    if (view == NULL) return;

    String param = GetParameterType();
    // PFL_EX(" <> mMethodName=[%s], mViewId=[0x%08x], param=[%s]", mMethodName.string(), mViewId, param.string());
    if (param.IsNull()) {
        // throw new ActionException("bad type: " + this.type);
        assert(0);
    }

    AutoPtr<IClassInfo> classInfo;
    _CObject_ReflectClassInfo(view, (IClassInfo**)&classInfo);
    assert(classInfo != NULL);

    AutoPtr<IMethodInfo> methodInfo;
    classInfo->GetMethodInfo(mMethodName, (IMethodInfo**)&methodInfo);
    assert(methodInfo != NULL);

    AutoPtr<IArgumentList> argumentList;
    methodInfo->CreateArgumentList((IArgumentList**)&argumentList);
    assert(argumentList != NULL);

    if (param.Equals(String("Boolean"))) {
        AutoPtr<IBoolean> value = IBoolean::Probe(mValue);
        assert(value != NULL);
        Boolean v = FALSE;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfBoolean(0, v);
    }
    else if (param.Equals(String("Byte"))) {
        AutoPtr<IByte> value = IByte::Probe(mValue);
        assert(value != NULL);
        Byte v = 0;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfByte(0, v);
    }
    else if (param.Equals(String("Int16"))) {
        AutoPtr<IInteger16> value = IInteger16::Probe(mValue);
        assert(value != NULL);
        Int16 v = 0;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfInt16(0, v);
    }
    else if (param.Equals(String("Int32"))) {
        AutoPtr<IInteger32> value = IInteger32::Probe(mValue);
        assert(value != NULL);
        Int32 v = 0;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfInt32(0, v);
    }
    else if (param.Equals(String("Int64"))) {
        AutoPtr<IInteger64> value = IInteger64::Probe(mValue);
        assert(value != NULL);
        Int64 v = 0;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfInt64(0, v);
    }
    else if (param.Equals(String("Float"))) {
        AutoPtr<IFloat> value = IFloat::Probe(mValue);
        assert(value != NULL);
        Float v = 0.f;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfFloat(0, v);
    }
    else if (param.Equals(String("Double"))) {
        AutoPtr<IDouble> value = IDouble::Probe(mValue);
        assert(value != NULL);
        Double v = 0.0;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfDouble(0, v);
    }
    else if (param.Equals(String("Char32"))) {
        AutoPtr<IChar32> value = IChar32::Probe(mValue);
        assert(value != NULL);
        Char32 v = 0;
        value->GetValue(&v);
        argumentList->SetInputArgumentOfInt32(0, v);
    }
    else if (param.Equals(String("String"))) {
        String v;
        if (mValue != NULL) {
            AutoPtr<ICharSequence> value = ICharSequence::Probe(mValue);
            value->ToString(&v);
        }
        argumentList->SetInputArgumentOfString(0, v);
    }
    else if (param.Equals(String("ICharSequence"))) {
        AutoPtr<ICharSequence> value = ICharSequence::Probe(mValue);
        argumentList->SetInputArgumentOfObjectPtr(0, value);
    }
    else {
        argumentList->SetInputArgumentOfObjectPtr(0, mValue);
    }

    ECode ec = methodInfo->Invoke(view, argumentList);
    if (FAILED(ec)) {
        assert(0);
        return;
    }
}

String RemoteViews::ReflectionAction::GetActionName()
{
    String str("ReflectionAction");
    str += mMethodName;
    str += StringUtils::ToString(mType);
    return str;
}

// interface IParcelable
ECode RemoteViews::ReflectionAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    source->ReadString(&mMethodName);
    source->ReadInt32(&mType);
    // Logger::D("RemoteViews", "+ RemoteViews::ReflectionAction::ReadFromParcel(), got type = %d; viewid = %d; methodName = %s", mType, mViewId, mMethodName.string());

    switch (mType) {
        case BOOLEAN:
        {
            Int32 res = 0;
            source->ReadInt32(&res);
            Boolean result = FALSE;
            result = res != 0;
            AutoPtr<IBoolean> b;
            CBoolean::New(result, (IBoolean**)&b);
            mValue = b->Probe(EIID_IInterface);
            break;
        }
        case BYTE:
        {
            Byte byte;
            source->ReadByte(&byte);
            AutoPtr<IByte> ibyte;
            CByte::New(byte, (IByte**)&ibyte);
            mValue = ibyte->Probe(EIID_IInterface);
            break;
        }
        case SHORT:
        {
            //this is right?
            Int16 s = 0;
            source->ReadInt16(&s);
            AutoPtr<IInteger16> integer16;
            CInteger16::New(s, (IInteger16**)&integer16);
            mValue = integer16->Probe(EIID_IInterface);
            break;
        }
        case INT:
        {
            Int32 i = 0;
            source->ReadInt32(&i);
            AutoPtr<IInteger32> integer32;
            CInteger32::New(i, (IInteger32**)&integer32);
            mValue = integer32->Probe(EIID_IInterface);
            break;
        }
        case LONG:
        {
            Int64 l = 0;
            source->ReadInt64(&l);
            AutoPtr<IInteger64> integer64;
            CInteger64::New(l, (IInteger64**)&integer64);
            mValue = integer64->Probe(EIID_IInterface);
            break;
        }
        case FLOAT:
        {
            Float f = 0;
            source->ReadFloat(&f);
            AutoPtr<IFloat> fl;
            CFloat::New(f, (IFloat**)&fl);
            mValue = fl->Probe(EIID_IInterface);
            break;
        }
        case DOUBLE:
        {
            Double d = 0;
            source->ReadDouble(&d);
            AutoPtr<IDouble> dou;
            CDouble::New(d, (IDouble**)&dou);
            mValue = dou->Probe(EIID_IInterface);
            break;
        }
        case CHAR:
        {
            // this is right?
            Int32 c;
            source->ReadInt32(&c);
            AutoPtr<IChar32> ch;
            CChar32::New((Char32)c, (IChar32**)&ch);
            mValue = ch->Probe(EIID_IInterface);
            break;
        }
        case STRING:
        {
            String str;
            source->ReadString(&str);
            AutoPtr<ICharSequence> csq;
            if (!str.IsNull()) {
                CStringWrapper::New(str, (ICharSequence**)&csq);
                mValue = csq->Probe(EIID_IInterface);
            }
            break;
        }
        case CHAR_SEQUENCE:
        {
            AutoPtr<ICharSequence> charCsq;
            FAIL_RETURN(TextUtils::CHAR_SEQUENCE_CREATOR::CreateFromParcel(source, (ICharSequence**)&charCsq));
            mValue = charCsq ? charCsq->Probe(EIID_IInterface) : NULL;
            break;
        }
        case URI:
        {
            Int32 flag = 0;
            source->ReadInt32(&flag);
            if (flag != 0) {
                AutoPtr<IUri> uri;
                source->ReadInterfacePtr((Handle32*)&uri);

                if(uri != NULL){
                    mValue = uri;
                }
            }
            break;
        }
        case BITMAP:
        {
            Int32 bitmap = 0;
            source->ReadInt32(&bitmap);
            if(bitmap != 0) {
                AutoPtr<IBitmap> bmp;
                CBitmap::New(0, NULL, FALSE, NULL, 0, (IBitmap**)&bmp);
                AutoPtr<CBitmap> cbmp = (CBitmap*)bmp.Get();
                cbmp->ReadFromParcel(source);
                mValue = bmp->Probe(EIID_IInterface);
            }
            break;
        }
        case BUNDLE:
        {
            AutoPtr<IBundle> bundle;
            CBundle::New((IBundle**)&bundle);
            AutoPtr<CBundle> cBundle = (CBundle*)bundle.Get();
            cBundle->ReadFromParcel(source);
            mValue = bundle->Probe(EIID_IInterface);
            break;
        }
        case INTENT:
        {
            Int32 intent = 0;
            source->ReadInt32(&intent);
            if (intent) {
                AutoPtr<IIntent> intent;
                CIntent::New((IIntent**)&intent);
                AutoPtr<CIntent> cIntent = (CIntent*)intent.Get();
                cIntent->ReadFromParcel(source);
                mValue = intent->Probe(EIID_IInterface);
            }
            break;
        }
        default:
            break;
    }
    // Logger::D("RemoteViews", "- RemoteViews::ReflectionAction::ReadFromParcel()");
    return NOERROR;
}

ECode RemoteViews::ReflectionAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    // Logger::D("RemoteViews", "+ RemoteViews::ReflectionAction::WriteToParcel(), type = %d; viewid = %d; method = %s", mType, mViewId, mMethodName.string());
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteString(mMethodName);
    dest->WriteInt32(mType);

    switch (mType) {
        case BOOLEAN:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type BOOLEAN");
            AutoPtr<IBoolean> b = IBoolean::Probe(mValue);
            Boolean res = FALSE;
            b->GetValue(&res);
            dest->WriteInt32(res ? 1 : 0);
            break;
        }
        case BYTE:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type BYTE");
            AutoPtr<IByte> ib = IByte::Probe(mValue);
            Byte by;
            ib->GetValue(&by);
            dest->WriteByte(by);
            break;
        }
        case SHORT:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type SHORT");
            AutoPtr<IInteger16> integer16 = IInteger16::Probe(mValue);
            Int16 i16 = 0;
            integer16->GetValue(&i16);
            dest->WriteInt16(i16);
            break;
        }
        case INT:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type INT");
            AutoPtr<IInteger32> integer32 = IInteger32::Probe(mValue);
            Int32 i32 = 0;
            integer32->GetValue(&i32);
            dest->WriteInt32(i32);
            break;
        }
        case LONG:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type LONG");
            AutoPtr<IInteger64> integer64 = IInteger64::Probe(mValue);
            Int64 i64 = 0;
            integer64->GetValue(&i64);
            dest->WriteInt64(i64);
            break;
        }
        case FLOAT:
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type FLOAT");
        {
            AutoPtr<IFloat> f = IFloat::Probe(mValue);
            Float fl = 0;
            f->GetValue(&fl);
            dest->WriteFloat(fl);
            break;
        }
        case DOUBLE:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type DOUBLE");
            AutoPtr<IDouble> d = IDouble::Probe(mValue);
            Double dou = 0;
            d->GetValue(&dou);
            dest->WriteDouble(dou);
            break;
        }
        case CHAR:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type CHAR");
            AutoPtr<IChar32> c = IChar32::Probe(mValue);
            Char32 ch;
            c->GetValue(&ch);
            dest->WriteInt32((Int32)ch);
            break;
        }
        case STRING:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type STRING");
            String str;
            if (mValue != NULL) {
                AutoPtr<ICharSequence> csq = ICharSequence::Probe(mValue);
                csq->ToString(&str);
            }
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type STRING, str = %s", str.string());
            dest->WriteString(str);
            break;
        }
        case CHAR_SEQUENCE:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type CHAR_SEQUENCE");
            AutoPtr<ICharSequence> charSeq = ICharSequence::Probe(mValue);
            // String str;
            // if (charSeq) {
            //     charSeq->ToString(&str);
            // }
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type CHAR_SEQUENCE, str = %s", str.string());
            TextUtils::WriteToParcel(charSeq, dest);
            // dest->WriteString(str);
            break;
        }
        case URI:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type URI");
            AutoPtr<IUri> uri = IUri::Probe(mValue);
            if(uri) {
                dest->WriteInt32(1);
                dest->WriteInterfacePtr(uri.Get());
            } else {
                dest->WriteInt32(0);
            }
            break;
        }
        case BITMAP:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type BITMAP");
            if(mValue) {
                dest->WriteInt32(1);
            } else {
                dest->WriteInt32(0);
            }

            if (mValue) {
                AutoPtr<CBitmap> bmp = (CBitmap*)IBitmap::Probe(mValue);
                bmp->WriteToParcel(dest);
            }
            break;
        }
        case BUNDLE:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type BUNDLE");
            if (mValue) {
                AutoPtr<CBundle> bundle = (CBundle*)IBundle::Probe(mValue);
                bundle->WriteToParcel(dest);
            }
            break;
        }
        case INTENT:
        {
            // Logger::D("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type INTENT");
            if(mValue) {
                dest->WriteInt32(1);
            } else {
                dest->WriteInt32(0);
            }
            if (mValue) {
                AutoPtr<CIntent> intent = (CIntent*)IIntent::Probe(mValue);
                intent->WriteToParcel(dest);
            }
            break;
        }
        default:
            Logger::W("RemoteViews", "RemoteViews::ReflectionAction::WriteToParcel(), write type unknown");
            break;
    }

    // Logger::D("RemoteViews", "- RemoteViews::ReflectionAction::WriteToParcel()");
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::ViewGroupAction
//==============================================================================
const Int32 RemoteViews::ViewGroupAction::TAG;

RemoteViews::ViewGroupAction::ViewGroupAction(
    /* [in] */ RemoteViews* host)
    : mHost(host)
{}

RemoteViews::ViewGroupAction::ViewGroupAction(
    /* [in] */ Int32 viewId,
    /* [in] */ IRemoteViews* nestedViews,
    /* [in] */ RemoteViews* host)
    : Action(viewId)
    , mNestedViews(nestedViews)
    , mHost(host)
{
    if (nestedViews != NULL) {
        mHost->ConfigureRemoteViewsAsChild(nestedViews);
    }
}

Int32 RemoteViews::ViewGroupAction::MergeBehavior()
{
    return MERGE_APPEND;
}

void RemoteViews::ViewGroupAction::UpdateMemoryUsageEstimate(
    /* [in] */ MemoryUsageCounter* counter)
{
    if (mNestedViews != NULL) {
        RemoteViews* v = reinterpret_cast<RemoteViews*>(mNestedViews.Get());
        counter->Increment(v->EstimateMemoryUsage());
    }
}

void RemoteViews::ViewGroupAction::SetBitmapCache(
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    if (mNestedViews != NULL) {
        RemoteViews* v = reinterpret_cast<RemoteViews*>(mNestedViews.Get());
        v->SetBitmapCache(bitmapCache);
    }
}

void RemoteViews::ViewGroupAction::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IContext> context;
    root->GetContext((IContext**)&context);
    AutoPtr<IView> v;
    root->FindViewById(mViewId, (IView**)&v);
    AutoPtr<IViewGroup> target = IViewGroup::Probe(v);
    if (target == NULL) return;
    if (mNestedViews != NULL) {
        RemoteViews* v = reinterpret_cast<RemoteViews*>(mNestedViews.Get());
        AutoPtr<IView> view = v->Apply(context, target, handler);
        target->AddView(view);
    }
    else {
        target->RemoveAllViews();
    }
}

String RemoteViews::ViewGroupAction::GetActionName()
{
    String str("ViewGroupAction");
    if (mNestedViews) {
        str += "Add";
    }
    else {
        str += "Remove";
    }
    return str;
}

// interface IParcelable
ECode RemoteViews::ViewGroupAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    return NOERROR;
}

ECode RemoteViews::ViewGroupAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    if (mNestedViews) {
        dest->WriteInt32(1);
        // TODO:
        //nestedViews.writeToParcel(dest, flags);
    }
    else {
        dest->WriteInt32(0);
    }
    return NOERROR;
}

ECode RemoteViews::ViewGroupAction::ReadFromParcel(
    /* [in] */ IParcel* source,
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    // need not to read TAG again!
    ReadFromParcel(source);

    source->ReadInt32(&mViewId);
    Int32 res = 0;
    source->ReadInt32(&res);
    Boolean nestedViewsNull = res == 0;
    if (!nestedViewsNull) {
        PFL_EX("TODO:")
        //This is right?
        //mNestedViews = new RemoteViews(source, bitmapCache);
    }
    else {
        mNestedViews = NULL;
    }
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::TextViewDrawableAction
//==============================================================================
const Int32 RemoteViews::TextViewDrawableAction::TAG;

RemoteViews::TextViewDrawableAction::TextViewDrawableAction()
    : mIsRelative(FALSE)
    , mD1(0)
    , mD2(0)
    , mD3(0)
    , mD4(0)
{}

RemoteViews::TextViewDrawableAction::TextViewDrawableAction(
    /* [in] */ Int32 viewId,
    /* [in] */ Boolean isRelative,
    /* [in] */ Int32 d1,
    /* [in] */ Int32 d2,
    /* [in] */ Int32 d3,
    /* [in] */ Int32 d4)
    : Action(viewId)
    , mIsRelative(isRelative)
    , mD1(d1)
    , mD2(d2)
    , mD3(d3)
    , mD4(d4)
{}

void RemoteViews::TextViewDrawableAction::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IContext> context;
    root->GetContext((IContext**)&context);
    AutoPtr<IView> v;
    root->FindViewById(mViewId, (IView**)&v);
    AutoPtr<ITextView> target = ITextView::Probe(v);
    if (target == NULL) return;
    if (mIsRelative) {
        target->SetCompoundDrawablesRelativeWithIntrinsicBounds(mD1, mD2, mD3, mD4);
    }
    else {
        target->SetCompoundDrawablesWithIntrinsicBounds(mD1, mD2, mD3, mD4);
    }
}

String RemoteViews::TextViewDrawableAction::GetActionName()
{
    return String("TextViewDrawableAction");
}

// interface IParcelable
ECode RemoteViews::TextViewDrawableAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    // need not to read TAG again!
    source->ReadInt32(&mViewId);
    Int32 res = 0;
    source->ReadInt32(&res);
    mIsRelative = res != 0;
    source->ReadInt32(&mD1);
    source->ReadInt32(&mD2);
    source->ReadInt32(&mD3);
    source->ReadInt32(&mD4);
    return NOERROR;
}

ECode RemoteViews::TextViewDrawableAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInt32(mIsRelative ? 1 : 0);
    dest->WriteInt32(mD1);
    dest->WriteInt32(mD2);
    dest->WriteInt32(mD3);
    dest->WriteInt32(mD4);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::TextViewSizeAction
//==============================================================================
const Int32 RemoteViews::TextViewSizeAction::TAG;

RemoteViews::TextViewSizeAction::TextViewSizeAction()
    : mUnits(0)
    , mSize(0.0)
{}

RemoteViews::TextViewSizeAction::TextViewSizeAction(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 units,
    /* [in] */ Float size)
    : Action(viewId)
    , mUnits(units)
    , mSize(size)
{}

void RemoteViews::TextViewSizeAction::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IContext> context;
    root->GetContext((IContext**)&context);
    AutoPtr<IView> v;
    root->FindViewById(mViewId, (IView**)&v);
    AutoPtr<ITextView> target = ITextView::Probe(v);
    if (target == NULL) return;
    target->SetTextSize(mUnits, mSize);
}

String RemoteViews::TextViewSizeAction::GetActionName()
{
    return String("TextViewSizeAction");
}

// interface IParcelable
ECode RemoteViews::TextViewSizeAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mViewId);
    source->ReadInt32(&mUnits);
    source->ReadFloat(&mSize);
    return NOERROR;
}

ECode RemoteViews::TextViewSizeAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInt32(mUnits);
    dest->WriteFloat(mSize);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::ViewPaddingAction
//==============================================================================
const Int32 RemoteViews::ViewPaddingAction::TAG;

RemoteViews::ViewPaddingAction::ViewPaddingAction()
    : mLeft(0)
    , mTop(0)
    , mRight(0)
    , mBottom(0)
{}

RemoteViews::ViewPaddingAction::ViewPaddingAction(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
    : Action(viewId)
    , mLeft(left)
    , mTop(top)
    , mRight(right)
    , mBottom(bottom)
{}

void RemoteViews::ViewPaddingAction::Apply(
    /* [in] */ IView* root,
    /* [in] */ IViewGroup* rootParent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IContext> context;
    root->GetContext((IContext**)&context);
    AutoPtr<IView> target;
    root->FindViewById(mViewId, (IView**)&target);
    if (target == NULL) return;
    target->SetPadding(mLeft, mTop, mRight, mBottom);
}

String RemoteViews::ViewPaddingAction::GetActionName()
{
    return String("ViewPaddingAction");
}

// interface IParcelable
ECode RemoteViews::ViewPaddingAction::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadInt32(&mViewId);
    source->ReadInt32(&mLeft);
    source->ReadInt32(&mTop);
    source->ReadInt32(&mRight);
    source->ReadInt32(&mBottom);
    return NOERROR;
}

ECode RemoteViews::ViewPaddingAction::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    dest->WriteInt32(TAG);
    dest->WriteInt32(mViewId);
    dest->WriteInt32(mLeft);
    dest->WriteInt32(mTop);
    dest->WriteInt32(mRight);
    dest->WriteInt32(mBottom);
    return NOERROR;
}

//==============================================================================
//                  RemoteViews::FillInIntentClickListener
//==============================================================================
CAR_INTERFACE_IMPL(RemoteViews::FillInIntentClickListener, IViewOnClickListener)

RemoteViews::FillInIntentClickListener::FillInIntentClickListener(
    /* [in] */ IIntent* fillInIntent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
    : mIntent(fillInIntent)
    , mHandler(handler)
{

}

ECode RemoteViews::FillInIntentClickListener::FillInIntentClickListener::OnClick(
    /* [in] */ IView* v)
{
    // Insure that this view is a child of an AdapterView
    AutoPtr<IViewParent> pa;
    v->GetParent((IViewParent**)&pa);
    AutoPtr<IView> parent = IView::Probe(pa);
    assert(0 && "TODO");
    //TODO
    // while (parent && IAdapterView::Probe(parent) == NULL && IAppWidgetHostView::Probe(parent) == NULL) {
    //     AutoPtr<IViewParent> p;
    //     parent->GetParent((IViewParent**)&p);
    //     parent = IView::Probe(p);
    // }

    // if (IAppWidgetHostView::Probe(parent) || parent == NULL) {
    //     // Somehow they've managed to get this far without having
    //     // and AdapterView as a parent.
    //     // Log.e("RemoteViews", "Collection item doesn't have AdapterView parent");
    //     return NOERROR;
    // }

    // Insure that a template pending intent has been set on an ancestor
    AutoPtr<IInterface> inter;
    // parent->GetTag((IInterface**)&inter);
    // if (!IPendingIntent::Probe(inter)) {
    //     // Log.e("RemoteViews", "Attempting setOnClickFillInIntent without" +
    //     //         " calling setPendingIntentTemplate on parent.");
    //     return NOERROR;
    // }

    AutoPtr<IPendingIntent> pendingIntent = IPendingIntent::Probe(inter);
    AutoPtr<IContext> context;
    v->GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ICompatibilityInfo> info;
    resources->GetCompatibilityInfo((ICompatibilityInfo**)&info);
    Float appScale = 0;
    info->GetApplicationScale(&appScale);
    AutoPtr< ArrayOf<Int32> > pos = ArrayOf<Int32>::Alloc(2);
    v->GetLocationOnScreen(&((*pos)[0]), &((*pos)[1]));

    AutoPtr<IRect> rect;
    CRect::New((IRect**)&rect);
    AutoPtr<CRect> crect = (CRect*)rect.Get();
    crect->mLeft = (Int32)(((*pos)[0]) * appScale + 0.5f);
    crect->mTop = (Int32)(((*pos)[1]) * appScale + 0.5f);
    Int32 width = 0, height = 0;
    v->GetWidth(&width);
    v->GetHeight(&height);
    crect->mRight = (Int32)((((*pos)[0]) + width) * appScale + 0.5f);
    crect->mBottom = (Int32)((((*pos)[1]) + height) * appScale + 0.5f);

    mIntent->SetSourceBounds(rect);
    Boolean succeed = FALSE;
    mHandler->OnClickHandler(v, pendingIntent, mIntent, &succeed);

    return NOERROR;
}

//==============================================================================
//                  RemoteViews::PendingIntentClickListener
//==============================================================================
CAR_INTERFACE_IMPL(RemoteViews::PendingIntentClickListener, IViewOnClickListener)

RemoteViews::PendingIntentClickListener::PendingIntentClickListener(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
    : mIntent(intent)
    , mHandler(handler)
{

}

ECode RemoteViews::PendingIntentClickListener::OnClick(
    /* [in] */ IView* v)
{
    AutoPtr<IContext> context;
    v->GetContext((IContext**)&context);
    AutoPtr<IResources> resources;
    context->GetResources((IResources**)&resources);
    AutoPtr<ICompatibilityInfo> info;
    resources->GetCompatibilityInfo((ICompatibilityInfo**)&info);
    Float appScale = 0;
    info->GetApplicationScale(&appScale);

    AutoPtr< ArrayOf<Int32> > pos = ArrayOf<Int32>::Alloc(2);
    v->GetLocationOnScreen(&((*pos)[0]), &((*pos)[1]));

    AutoPtr<IRect> rect;
    CRect::New((IRect**)&rect);
    AutoPtr<CRect> crect = (CRect*)rect.Get();
    crect->mLeft = (Int32)(((*pos)[0]) * appScale + 0.5f);
    crect->mTop = (Int32)(((*pos)[1]) * appScale + 0.5f);
    Int32 width = 0, height = 0;
    v->GetWidth(&width);
    v->GetHeight(&height);
    crect->mRight = (Int32)((((*pos)[0]) + width) * appScale + 0.5f);
    crect->mBottom = (Int32)((((*pos)[1]) + height) * appScale + 0.5f);

    AutoPtr<IIntent> intent;
    CIntent::New((IIntent**)&intent);
    intent->SetSourceBounds(rect);
    Boolean succeed = FALSE;
    mHandler->OnClickHandler(v, mIntent, intent, &succeed);

    return NOERROR;
}

//==============================================================================
//                  RemoteViews::IntentTemplateOnItemClickListener
//==============================================================================
CAR_INTERFACE_IMPL(RemoteViews::IntentTemplateOnItemClickListener, IAdapterViewOnItemClickListener)

RemoteViews::IntentTemplateOnItemClickListener::IntentTemplateOnItemClickListener(
    /* [in] */ IPendingIntent* intent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
    : mIntent(intent)
    , mHandler(handler)
{

}

ECode RemoteViews::IntentTemplateOnItemClickListener::OnItemClick(
    /* [in] */ IAdapterView* parent,
    /* [in] */ IView* view,
    /* [in] */ Int32 position,
    /* [in] */ Int64 id)
{
    if (IViewGroup::Probe(view)) {
        AutoPtr<IViewGroup> vg = IViewGroup::Probe(view);
        if (IAdapterViewAnimator::Probe(parent)) {
            AutoPtr<IView> v;
            vg->GetChildAt(0, (IView**)&v);
            vg = IViewGroup::Probe(v);
        }
        if (!vg) return NOERROR;
        AutoPtr<IIntent> fillInIntent;
        Int32 childCount = 0;
        vg->GetChildCount(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IView> iview;
            vg->GetChildAt(i, (IView**)&iview);
            AutoPtr<IInterface> tag;
            iview->GetTag(R::id::fillInIntent, (IInterface**)&tag);
            if (IIntent::Probe(tag)) {
                fillInIntent = IIntent::Probe(tag);
                break;
            }
        }

        if (!fillInIntent) return NOERROR;

        AutoPtr<IContext> context;
        view->GetContext((IContext**)&context);
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        AutoPtr<ICompatibilityInfo> info;
        resources->GetCompatibilityInfo((ICompatibilityInfo**)&info);
        Float appScale = 0;
        info->GetApplicationScale(&appScale);

        AutoPtr< ArrayOf<Int32> > pos = ArrayOf<Int32>::Alloc(2);
        view->GetLocationOnScreen(&((*pos)[0]), &((*pos)[1]));

        AutoPtr<IRect> rect;
        CRect::New((IRect**)&rect);
        AutoPtr<CRect> crect = (CRect*)rect.Get();
        crect->mLeft = (Int32)(((*pos)[0]) * appScale + 0.5f);
        crect->mTop = (Int32)(((*pos)[1]) * appScale + 0.5f);
        Int32 width = 0, height = 0;
        view->GetWidth(&width);
        view->GetHeight(&height);
        crect->mRight = (Int32)((((*pos)[0]) + width) * appScale + 0.5f);
        crect->mBottom = (Int32)((((*pos)[1]) + height) * appScale + 0.5f);

        AutoPtr<IIntent> intent;
        CIntent::New((IIntent**)&intent);
        intent->SetSourceBounds(rect);
        Boolean succeed = FALSE;
        mHandler->OnClickHandler(view, mIntent, fillInIntent, &succeed);
    }
    return NOERROR;
}

//==============================================================================
//                  RemoteViews
//==============================================================================
RemoteViews::RemoteViews()
    : mIsRoot(TRUE)
    , mIsWidgetCollectionChild(FALSE)
{
    Process::MyUserHandle((IUserHandle**)&mUser);
}


RemoteViews::RemoteViews(
    /* [in] */ IParcel* parcel)
    : mLayoutId(-1)
    , mIsRoot(TRUE)
    , mIsWidgetCollectionChild(FALSE)
{
    Init(parcel);
}

RemoteViews::RemoteViews(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 layoutId)
    : mPackage(packageName)
    , mLayoutId(layoutId)
    , mIsRoot(TRUE)
    , mIsWidgetCollectionChild(FALSE)
{
    CRemoteViewsBitmapCache::New((IRemoteViewsBitmapCache**)&mBitmapCache);
    mMemoryUsageCounter = new MemoryUsageCounter();
    RecalculateMemoryUsage();
}

RemoteViews::RemoteViews(
    /* [in] */ IRemoteViews* landscape,
    /* [in] */ IRemoteViews* portrait)
{
    Init(landscape, portrait);
}

ECode RemoteViews::Init(
    /* [in] */ IParcel* parcel)
{
    return Init(parcel, NULL);
}

ECode RemoteViews::Init(
    /* [in] */ const String& packageName,
    /* [in] */ Int32 layoutId)
{
    mLayoutId = layoutId;
    mPackage = packageName;
    CRemoteViewsBitmapCache::New((IRemoteViewsBitmapCache**)&mBitmapCache);
    mMemoryUsageCounter = new MemoryUsageCounter();
    RecalculateMemoryUsage();
    return NOERROR;
}

ECode RemoteViews::Init(
    /* [in] */ IRemoteViews* landscape,
    /* [in] */ IRemoteViews* portrait)
{
    if (!landscape || !portrait) {
        Slogger::E(TAG, "Both RemoteViews must be non-null");
        return E_RUNTIME_EXCEPTION;
    }

    String landStr, portStr;
    landscape->GetPackage(&landStr);
    portrait->GetPackage(&portStr);
    if (landStr.Compare(portStr) != 0) {
        Slogger::E(TAG, "Both RemoteViews must share the same package");
        return E_RUNTIME_EXCEPTION;
    }

    portrait->GetPackage(&mPackage);
    portrait->GetLayoutId(&mLayoutId);

    mLandscape = landscape;
    mPortrait = portrait;

    mMemoryUsageCounter = new MemoryUsageCounter();
    mBitmapCache = NULL;
    CRemoteViewsBitmapCache::New((IRemoteViewsBitmapCache**)&mBitmapCache);
    ConfigureRemoteViewsAsChild(landscape);
    ConfigureRemoteViewsAsChild(portrait);

    RecalculateMemoryUsage();
    return NOERROR;
}

RemoteViews::RemoteViews(
    /* [in] */ IParcel* parcel,
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
    : mLayoutId(-1)
    , mIsRoot(FALSE)
    , mIsWidgetCollectionChild(FALSE)
{
    Init(parcel, bitmapCache);
}

ECode RemoteViews::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return Init(source);
}

ECode RemoteViews::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    if (!HasLandscapeAndPortraitLayouts()) {
        dest->WriteInt32(MODE_NORMAL);
        if (mIsRoot) {
            if (mBitmapCache != NULL) {
                dest->WriteInt32(1);
                IParcelable* parcelable = IParcelable::Probe(mBitmapCache.Get());
                parcelable->WriteToParcel(dest);
            }
            else {
                dest->WriteInt32(0);
            }
        }

        dest->WriteString(mPackage);
        dest->WriteInt32(mLayoutId);
        dest->WriteInt32(mIsWidgetCollectionChild ? 1 : 0);
        Int32 count = mActions.GetSize();
        dest->WriteInt32(count);
        List< AutoPtr<Action> >::Iterator it;
        for (it = mActions.Begin(); it != mActions.End(); ++it) {
            AutoPtr<Action> a = *it;
            a->WriteToParcel(dest);
        }
    }
    else {
        assert(0 && "not support now");
        dest->WriteInt32(MODE_HAS_LANDSCAPE_AND_PORTRAIT);
        if (mIsRoot) {
            if (mBitmapCache != NULL) {
                dest->WriteInt32(1);
                IParcelable* parcelable = IParcelable::Probe(mBitmapCache.Get());
                parcelable->WriteToParcel(dest);
            }
            else {
                dest->WriteInt32(0);
            }
        }

        if (mLandscape != NULL) {
            dest->WriteInt32(1);
            IParcelable* parcelable = IParcelable::Probe(mLandscape.Get());
            parcelable->WriteToParcel(dest);
        }
        else {
            dest->WriteInt32(0);
        }

        if (mPortrait != NULL) {
            dest->WriteInt32(1);
            IParcelable* parcelable = IParcelable::Probe(mPortrait.Get());
            parcelable->WriteToParcel(dest);
        }
        else {
            dest->WriteInt32(0);
        }
    }
    return NOERROR;
}

ECode RemoteViews::InitBitmapCache(
    /* [in] */ IParcel* parcel,
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    if (!bitmapCache) {
        Int32 temp;
        parcel->ReadInt32(&temp);
        mBitmapCache = NULL;
        if (temp != 0) {
            mBitmapCache = NULL;
            CRemoteViewsBitmapCache::New((IRemoteViewsBitmapCache**)&mBitmapCache);
            IParcelable* parcelable = IParcelable::Probe(mBitmapCache.Get());
            parcelable->ReadFromParcel(parcel);
        }
    }
    else {
        SetBitmapCache(bitmapCache);
        SetNotRoot();
    }
    return NOERROR;
}

ECode RemoteViews::Init(
    /* [in] */ IParcel* parcel,
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    Int32 mode = 0;
    parcel->ReadInt32(&mode);

    InitBitmapCache(parcel, bitmapCache);

    if (mode == MODE_NORMAL) {
        Int32 isWidget = 0;
        parcel->ReadString(&mPackage);
        parcel->ReadInt32(&mLayoutId);
        parcel->ReadInt32(&isWidget);
        mIsWidgetCollectionChild = (isWidget == 1);

        Int32 count = 0;
        parcel->ReadInt32(&count);
        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), read MODE_NORMAL, action count = %d", count);

        if (count > 0) {
            Int32 tag = 0;
            for (Int32 i = 0; i < count; i++) {
                parcel->ReadInt32(&tag);
                switch (tag) {
                    case _SetOnClickPendingIntent::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and SetOnClickPendingIntent action");
                        AutoPtr<Action> action = new _SetOnClickPendingIntent(this);
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case _SetDrawableParameters::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and SetDrawableParameters action");
                        AutoPtr<Action> action = new _SetDrawableParameters();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case ReflectionAction::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and ReflectionAction action");
                        AutoPtr<Action> action = new ReflectionAction();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case ViewGroupAction::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and ViewGroupAction action");
                        AutoPtr<ViewGroupAction> action = new ViewGroupAction(this);
                        action->ReadFromParcel(parcel, mBitmapCache);
                        mActions.PushBack((Action*)action.Get());
                        break;
                    }

                    case ReflectionActionWithoutParams::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and ReflectionActionWithoutParams action");
                        AutoPtr<Action> action = new ReflectionActionWithoutParams();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case _SetEmptyView::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and RemoteViewsSetEmptyView action");
                        AutoPtr<Action> action = new _SetEmptyView();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case _SetPendingIntentTemplate::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and SetPendingIntentTemplate action");
                        AutoPtr<Action> action = new _SetPendingIntentTemplate();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case _SetOnClickFillInIntent::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and SetOnClickFillInIntent action");
                        AutoPtr<Action> action = new _SetOnClickFillInIntent(this);
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case SetRemoteViewsAdapterIntent::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and SetRemoteViewsAdapterIntent action");
                        AutoPtr<Action> action = new SetRemoteViewsAdapterIntent();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case TextViewDrawableAction::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and TextViewDrawableAction action");
                        AutoPtr<Action> action = new TextViewDrawableAction();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case TextViewSizeAction::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and TextViewSizeAction action");
                        AutoPtr<Action> action = new TextViewSizeAction();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case ViewPaddingAction::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and ViewPaddingAction action");
                        AutoPtr<Action> action = new ViewPaddingAction();
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }

                    case BitmapReflectionAction::TAG: {
                        // Slogger::D("RemoteViews", "+ RemoteViews::Init(IP, IRB), Got and BitmapReflectionAction action");
                        AutoPtr<Action> action = new BitmapReflectionAction(this);
                        action->ReadFromParcel(parcel);
                        mActions.PushBack(action);
                        break;
                    }
                    default:
                        break;
                }
            }
        }
    }
    else {
        assert(0 && "not sopport now");
        Int32 temp;

        mLandscape = NULL;
        parcel->ReadInt32(&temp);
        if (temp != 0) {
            CRemoteViews::New((IRemoteViews**)&mLandscape);
            RemoteViews* tmp = reinterpret_cast<RemoteViews*>(mLandscape->Probe(EIID_RemoteViews));
            tmp->Init(parcel, mBitmapCache);
        }

        mLayoutId = 0;
        mPackage = NULL;

        mPortrait = NULL;
        parcel->ReadInt32(&temp);
        if (temp != 0) {
            CRemoteViews::New((IRemoteViews**)&mPortrait);
            RemoteViews* tmp = reinterpret_cast<RemoteViews*>(mPortrait->Probe(EIID_RemoteViews));
            tmp->Init(parcel, mBitmapCache);

            mPortrait->GetPackage(&mPackage);
            mPortrait->GetLayoutId(&mLayoutId);
        }
    }

    mMemoryUsageCounter = new MemoryUsageCounter();
    RecalculateMemoryUsage();
    return NOERROR;
}

ECode RemoteViews::SetNotRoot()
{
    mIsRoot = FALSE;
    return NOERROR;
}

void RemoteViews::ConfigureRemoteViewsAsChild(
    /* [in] */ IRemoteViews* rv)
{
    RemoteViews* remote = reinterpret_cast<RemoteViews*>(rv->Probe(EIID_RemoteViews));
    mBitmapCache->Assimilate(remote->mBitmapCache);
    remote->SetBitmapCache(mBitmapCache);
    remote->SetNotRoot();
}

Boolean RemoteViews::HasLandscapeAndPortraitLayouts()
{
    return mLandscape && mPortrait;
}

AutoPtr<IRemoteViews> RemoteViews::GetRemoteViewsToApply(
    /* [in] */ IContext* context)
{
    if (HasLandscapeAndPortraitLayouts()) {
        AutoPtr<IResources> resources;
        context->GetResources((IResources**)&resources);
        AutoPtr<IConfiguration> config;
        resources->GetConfiguration((IConfiguration**)&config);
        Int32 orientation = 0;
        config->GetOrientation(&orientation);
        if (orientation == IConfiguration::ORIENTATION_LANDSCAPE) {
            return mLandscape;
        } else {
            return mPortrait;
        }
    }

    return (IRemoteViews*)this->Probe(EIID_IRemoteViews);
}

void RemoteViews::PerformApply(
    /* [in] */ IView* v,
    /* [in] */ IViewGroup* parent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    if (!mActions.IsEmpty()) {
        if (!handler) {
            handler = DEFAULT_ON_CLICK_HANDLER;
        }
        List< AutoPtr<Action> >::Iterator it;
        for (it = mActions.Begin(); it != mActions.End(); ++it) {
            AutoPtr<Action> a = *it;
            a->Apply(v, parent, handler);
        }
    }
}

AutoPtr<IContext> RemoteViews::PrepareContext(
    /* [in] */ IContext* context)
{
    assert(context != NULL);
    AutoPtr<IContext> c;
    String packageName = mPackage;
    if (!packageName.IsNull()) {
//        try {
        ECode ec = context->CreatePackageContextAsUser(packageName, IContext::CONTEXT_RESTRICTED, mUser, (IContext**)&c);
        if (FAILED(ec)) {
            c = context;
        }
//        } catch (NameNotFoundException e) {
//            Log.e(LOG_TAG, "Package name " + packageName + " not found");
//            c = context;
//        }
    } else {
        c = context;
    }
    return c;
}

ECode RemoteViews::MergeRemoteViews(
    /* [in] */ IRemoteViews* newRv)
{
    if (!newRv) return NOERROR;

    Slogger::E(TAG, "TODO: RemoteViews::MergeRemoteViews() not finish!");
    return NOERROR;

    // AutoPtr<IRemoteViews> copy;
    // newRv->Clone((IRemoteViews**)&copy);

    // HashMap<String, AutoPtr<Action> > map;
    // Int32 count = mActions.GetSize();
    // for (Int32 i = 0; i < count; i++) {
    //     AutoPtr<Action> a = mActions[i];
    //     //map.put(a.getUniqueKey(), a);
    // }
    // assert(0 && "TODO");
    // RemoteViews* mCopy = reinterpret_cast<RemoteViews*>(copy->Probe(EIID_RemoteViews));
    // List<AutoPtr<Action> > newActions = mCopy->mActions;
    // if (newActions.IsEmpty()) return NOERROR;
    // count = newActions.GetSize();
    // for (Int32 i = 0; i < count; i++) {
    //     AutoPtr<Action> action = newActions[i];
    //     String key = action->GetUniqueKey();
    //     Int32 mergeBehavior = action->MergeBehavior();
    //     /*if (map.containsKey(key) && mergeBehavior == Action.MERGE_REPLACE) {
    //         mActions.remove(map.get(key));
    //         map.remove(key);
    //     }*/

    //     if(mergeBehavior == Action::MERGE_REPLACE || mergeBehavior == Action::MERGE_APPEND) {
    //         mActions.PushBack(action);
    //     }
    // }

    // mBitmapCache = NULL;
    // CRemoteViewsBitmapCache::New((IRemoteViewsBitmapCache**)&mBitmapCache);
    // SetBitmapCache(mBitmapCache);

    return NOERROR;
}

AutoPtr<IRemoteViews> RemoteViews::Clone()
{
    AutoPtr<IParcel> source;
    CParcel::New((IParcel**)&source);
    WriteToParcel(source);
    source->SetDataPosition(0);
    AutoPtr<IRemoteViews> remoteViews;
    CRemoteViews::New((IRemoteViews**)&remoteViews);
    IParcelable::Probe(remoteViews)->ReadFromParcel(source);
    return remoteViews;
}

ECode RemoteViews::SetUser(
    /* [in] */ IUserHandle* user)
{
    mUser = user;
    return NOERROR;
}

String RemoteViews::GetPackage()
{
    return mPackage;
}

Int32 RemoteViews::GetLayoutId()
{
    return mLayoutId;
}

ECode RemoteViews::SetIsWidgetCollectionChild(
    /* [in] */ Boolean isWidgetCollectionChild)
{
    mIsWidgetCollectionChild = isWidgetCollectionChild;
    return NOERROR;
}

void RemoteViews::RecalculateMemoryUsage()
{
    mMemoryUsageCounter->Clear();
    if (!HasLandscapeAndPortraitLayouts()) {
        if (!mActions.IsEmpty()) {
            List< AutoPtr<Action> >::Iterator it;
            for (it = mActions.Begin(); it != mActions.End(); ++it) {
                (*it)->UpdateMemoryUsageEstimate(mMemoryUsageCounter);
            }
        }
        if (mIsRoot) {
            CRemoteViewsBitmapCache* bc = (CRemoteViewsBitmapCache*)mBitmapCache.Get();
            bc->AddBitmapMemory(mMemoryUsageCounter);
        }
    }
    else {
        Int32 land = 0, port = 0;
        mLandscape->EstimateMemoryUsage(&land);
        mPortrait->EstimateMemoryUsage(&port);
        mMemoryUsageCounter->Increment(land);
        mMemoryUsageCounter->Increment(port);
        CRemoteViewsBitmapCache* bc = (CRemoteViewsBitmapCache*)mBitmapCache.Get();
        bc->AddBitmapMemory(mMemoryUsageCounter);
    }
}

void RemoteViews::SetBitmapCache(
    /* [in] */ IRemoteViewsBitmapCache* bitmapCache)
{
    mBitmapCache = bitmapCache;
    if (!HasLandscapeAndPortraitLayouts()) {
        if (!mActions.IsEmpty()) {
            List< AutoPtr<Action> >::Iterator it;
            for (it = mActions.Begin(); it != mActions.End(); ++it) {
                (*it)->SetBitmapCache(bitmapCache);
            }
        }
    }
    else {
        assert(0 && "TODO");
        RemoteViews* land = reinterpret_cast<RemoteViews*>(mLandscape->Probe(EIID_RemoteViews));
        RemoteViews* prot = reinterpret_cast<RemoteViews*>(mLandscape->Probe(EIID_RemoteViews));
        land->SetBitmapCache(bitmapCache);
        prot->SetBitmapCache(bitmapCache);
    }
}

Int32 RemoteViews::EstimateMemoryUsage()
{
    return mMemoryUsageCounter->GetMemoryUsage();
}

ECode RemoteViews::AddAction(
    /* [in] */ Action* a)
{
    if (HasLandscapeAndPortraitLayouts()) {
        return E_RUNTIME_EXCEPTION;
    }
    mActions.PushBack(a);
    a->UpdateMemoryUsageEstimate(mMemoryUsageCounter);
    return NOERROR;
}

ECode RemoteViews::AddView(
    /* [in] */ Int32 viewId,
    /* [in] */ IRemoteViews* nestedView)
{
    AutoPtr<Action> action = new ViewGroupAction(viewId, nestedView, this);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::RemoveAllViews(
    /* [in] */ Int32 viewId)
{
    AutoPtr<Action> action = new ViewGroupAction(viewId, NULL, this);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::ShowNext(
    /* [in] */ Int32 viewId)
{
    AutoPtr<Action> action = new ReflectionActionWithoutParams(viewId, String("ShowNext"));
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::ShowPrevious(
    /* [in] */ Int32 viewId)
{
    AutoPtr<Action> action = new ReflectionActionWithoutParams(viewId, String("ShowPrevious"));
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetDisplayedChild(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 childIndex)
{
    SetInt32(viewId, String("SetDisplayedChild"), childIndex);
    return NOERROR;
}

ECode RemoteViews::SetViewVisibility(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 visibility)
{
    SetInt32(viewId, String("SetVisibility"), visibility);
    return NOERROR;
}

ECode RemoteViews::SetTextViewText(
    /* [in] */ Int32 viewId,
    /* [in] */ ICharSequence* text)
{
    SetCharSequence(viewId, String("SetText"), text);
    return NOERROR;
}

ECode RemoteViews::SetTextViewTextSize(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 units,
    /* [in] */ Float size)
{
    AutoPtr<Action> action = new TextViewSizeAction(viewId, units, size);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetTextViewCompoundDrawables(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<Action> action = new TextViewDrawableAction(viewId, FALSE, left, top, right, bottom);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetTextViewCompoundDrawablesRelative(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 start,
    /* [in] */ Int32 top,
    /* [in] */ Int32 end,
    /* [in] */ Int32 bottom)
{
    AutoPtr<Action> action = new TextViewDrawableAction(viewId, TRUE, start, top, end, bottom);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetImageViewResource(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 srcId)
{
    SetInt32(viewId, String("SetImageResource"), srcId);
    return NOERROR;
}

ECode RemoteViews::SetImageViewUri(
    /* [in] */ Int32 viewId,
    /* [in] */ IUri* uri)
{
    SetUri(viewId, String("SetImageURI"), uri);
    return NOERROR;
}

ECode RemoteViews::SetImageViewBitmap(
    /* [in] */ Int32 viewId,
    /* [in] */ IBitmap* bitmap)
{
    SetBitmap(viewId, String("SetImageBitmap"), bitmap);
    return NOERROR;
}

ECode RemoteViews::SetEmptyView(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 emptyViewId)
{
    AutoPtr<Action> action = new _SetEmptyView(viewId, emptyViewId);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetChronometer(
    /* [in] */ Int32 viewId,
    /* [in] */ Int64 base,
    /* [in] */ const String& format,
    /* [in] */ Boolean started)
{
    SetInt64(viewId, String("SetBase"), base);
    SetString(viewId, String("SetFormat"), format);
    SetBoolean(viewId, String("SetStarted"), started);
    return NOERROR;
}

ECode RemoteViews::SetProgressBar(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 max,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean indeterminate)
{
    SetBoolean(viewId, String("SetIndeterminate"), indeterminate);
    if (!indeterminate) {
        SetInt32(viewId, String("SetMax"), max);
        SetInt32(viewId, String("SetProgress"), progress);
    }
    return NOERROR;
}

ECode RemoteViews::SetOnClickPendingIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ IPendingIntent* pendingIntent)
{
    AutoPtr<Action> action = new _SetOnClickPendingIntent(viewId, pendingIntent, this);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetPendingIntentTemplate(
    /* [in] */ Int32 viewId,
    /* [in] */ IPendingIntent* pendingIntentTemplate)
{
    AutoPtr<Action> action = new _SetPendingIntentTemplate(viewId, pendingIntentTemplate);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetOnClickFillInIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ IIntent* fillInIntent)
{
    AutoPtr<Action> action = new _SetOnClickFillInIntent(viewId, fillInIntent, this);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetDrawableParameters(
    /* [in] */ Int32 viewId,
    /* [in] */ Boolean targetBackground,
    /* [in] */ Int32 alpha,
    /* [in] */ Int32 colorFilter,
    /* [in] */ PorterDuffMode mode,
    /* [in] */ Int32 level)
{
    AutoPtr<Action> action = new _SetDrawableParameters(viewId, targetBackground, alpha, colorFilter, mode, level);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetTextColor(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 color)
{
    SetInt32(viewId, String("SetTextColor"), color);
    return NOERROR;
}

ECode RemoteViews::SetRemoteAdapter(
    /* [in] */ Int32 viewId,
    /* [in] */ IIntent* intent)
{
    AutoPtr<Action> action = new SetRemoteViewsAdapterIntent(viewId, intent);
    AddAction(action);
    return NOERROR;
}

ECode RemoteViews::SetRemoteAdapter(
    /* [in] */ Int32 appWidgetId,
    /* [in] */ Int32 viewId,
    /* [in] */ IIntent* intent)
{
    SetRemoteAdapter(viewId, intent);
    return NOERROR;
}

ECode RemoteViews::SetScrollPosition(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 position)
{
    SetInt32(viewId, String("SmoothScrollToPosition"), position);
    return NOERROR;
}

ECode RemoteViews::SetRelativeScrollPosition(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 offset)
{
    SetInt32(viewId, String("SmoothScrollByOffset"), offset);
    return NOERROR;
}

ECode RemoteViews::SetViewPadding(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    AutoPtr<Action> action = new ViewPaddingAction(viewId, left, top, right, bottom);
    return AddAction(action);
}

ECode RemoteViews::SetBoolean(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Boolean value)
{
    AutoPtr<IBoolean> b;
    CBoolean::New(value, (IBoolean**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::BOOLEAN, inter);
    return AddAction(action);
}

ECode RemoteViews::SetByte(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Byte value)
{
    AutoPtr<IByte> b;
    CByte::New(value, (IByte**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::BYTE, inter);
    return AddAction(action);
}

ECode RemoteViews::SetInt16(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int16 value)
{
    AutoPtr<IInteger16> b;
    CInteger16::New(value, (IInteger16**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::SHORT, inter);
    return AddAction(action);
}

ECode RemoteViews::SetInt32(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int32 value)
{
    AutoPtr<IInteger32> b;
    CInteger32::New(value, (IInteger32**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::INT, inter);
    return AddAction(action);
}

ECode RemoteViews::SetInt64(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Int64 value)
{
    AutoPtr<IInteger64> b;
    CInteger64::New(value, (IInteger64**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::LONG, inter);
    return AddAction(action);
}

ECode RemoteViews::SetFloat(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Float value)
{
    AutoPtr<IFloat> b;
    CFloat::New(value, (IFloat**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::FLOAT, inter);
    return AddAction(action);
}

ECode RemoteViews::SetDouble(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Double value)
{
    AutoPtr<IDouble> b;
    CDouble::New(value, (IDouble**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::DOUBLE, inter);
    return AddAction(action);
}

ECode RemoteViews::SetChar(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ Char32 value)
{
    AutoPtr<IChar32> b;
    CChar32::New(value, (IChar32**)&b);
    AutoPtr<IInterface> inter = b->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::CHAR, inter);
    return AddAction(action);
}

ECode RemoteViews::SetString(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ const String& value)
{
    AutoPtr<IInterface> inter;
    if (!value.IsNull()) {
        AutoPtr<ICharSequence> b;
        CStringWrapper::New(value, (ICharSequence**)&b);
        inter = b->Probe(EIID_IInterface);
    }
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::STRING, inter);
    return AddAction(action);
}

ECode RemoteViews::SetCharSequence(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ ICharSequence* value)
{
    AutoPtr<IInterface> inter = value ? value->Probe(EIID_IInterface) : NULL;
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::CHAR_SEQUENCE, inter);

    return AddAction(action);
}

ECode RemoteViews::SetUri(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IUri* value)
{
    assert(value);
    AutoPtr<IUri> temp;
    value->GetCanonicalUri((IUri**)&temp);
    AutoPtr<IInterface> inter = temp->Probe(EIID_IInterface);
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::URI, inter);
    return AddAction(action);
}

ECode RemoteViews::SetBitmap(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IBitmap* value)
{
    AutoPtr<Action> action = new BitmapReflectionAction(viewId, methodName, value, this);
    return AddAction(action);
}

ECode RemoteViews::SetBundle(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IBundle* value)
{
    AutoPtr<IInterface> inter = value ? value->Probe(EIID_IInterface) : NULL;
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::BUNDLE, inter);

    return AddAction(action);
}

ECode RemoteViews::SetIntent(
    /* [in] */ Int32 viewId,
    /* [in] */ const String& methodName,
    /* [in] */ IIntent* value)
{
    AutoPtr<IInterface> inter = value ? value->Probe(EIID_IInterface) : NULL;
    AutoPtr<Action> action = new ReflectionAction(viewId, methodName, ReflectionAction::INTENT, inter);

    return AddAction(action);
}

ECode RemoteViews::SetContentDescription(
    /* [in] */ Int32 viewId,
    /* [in] */ ICharSequence* contentDescription)
{
    SetCharSequence(viewId, String("SetContentDescription"), contentDescription);
    return NOERROR;
}

ECode RemoteViews::SetLabelFor(
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 labeledId)
{
    SetInt32(viewId, String("SetLabelFor"), labeledId);
    return NOERROR;
}

AutoPtr<IView> RemoteViews::Apply(
    /* [in] */ IContext* context,
    /* [in] */ IViewGroup* parent)
{
    return Apply(context, parent, NULL);
}

AutoPtr<IView> RemoteViews::Apply(
    /* [in] */ IContext* context,
    /* [in] */ IViewGroup* parent,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IRemoteViews> rvToApply = GetRemoteViewsToApply(context);
    AutoPtr<IContext> c = PrepareContext(context);
    // result doesn't hold the reference of c, so we put it in mContextCache in order to
    // make c alive.
    if (c.Get() != context) mContextCache.PushBack(c);

    AutoPtr<IInterface> inter;
    ASSERT_SUCCEEDED(c->GetSystemService(IContext::LAYOUT_INFLATER_SERVICE, (IInterface**)&inter));
    AutoPtr<ILayoutInflater> inflater = ILayoutInflater::Probe(inter);
    assert(inflater);
    ASSERT_SUCCEEDED(inflater->CloneInContext(c, (ILayoutInflater**)&inflater));
    ASSERT_SUCCEEDED(inflater->SetFilter((ILayoutInflaterFilter*)this->Probe(EIID_ILayoutInflaterFilter)));

    Int32 id = 0;
    rvToApply->GetLayoutId(&id);

    AutoPtr<IView> result;
    ASSERT_SUCCEEDED(inflater->Inflate(id, parent, FALSE, (IView**)&result));

    RemoteViews* remote = reinterpret_cast<RemoteViews*>(rvToApply->Probe(EIID_RemoteViews));
    remote->PerformApply(result, parent, handler);
    return result;
}

ECode RemoteViews::Reapply(
    /* [in] */ IContext* context,
    /* [in] */ IView* v)
{
    return Reapply(context, v, NULL);
}

ECode RemoteViews::Reapply(
    /* [in] */ IContext* context,
    /* [in] */ IView* v,
    /* [in] */ IRemoteViewsOnClickHandler* handler)
{
    AutoPtr<IRemoteViews> rvToApply = GetRemoteViewsToApply(context);
    if (HasLandscapeAndPortraitLayouts()) {
        Int32 vId = 0, rvId = 0;
        v->GetId(&vId);
        rvToApply->GetLayoutId(&rvId);
        if (vId != rvId) {
            assert(0);
//            throw new RuntimeException("Attempting to re-apply RemoteViews to a view that" +
//                    " that does not share the same root layout id.");
        }
    }

    PrepareContext(context);
    AutoPtr<IViewParent> parent;
    v->GetParent((IViewParent**)&parent);
    RemoteViews* remote = reinterpret_cast<RemoteViews*>(rvToApply->Probe(EIID_RemoteViews));
    remote->PerformApply(v, IViewGroup::Probe(parent), handler);

    return NOERROR;
}

ECode RemoteViews::OnLoadClass(
    /* [in] */ Handle32 clazz,
    /* [out] */ Boolean* res)
{
    VALIDATE_NOT_NULL(res);
//    return clazz.isAnnotationPresent(RemoteView.class);
    *res = TRUE;
    return NOERROR;
}

String RemoteViews::ToString()
{
    StringBuilder sb("IRemoteViews(0x");
    sb += StringUtils::Int32ToHexString((Int32)(THIS_PROBE(IRemoteViews)));
    sb += ")={package=";
    sb += mPackage.IsNull() ? "NULL" : mPackage;
    sb += ", layoutId=0x";
    sb += StringUtils::Int32ToHexString(mLayoutId);
    sb += ", actions={";

    List< AutoPtr<Action> >::Iterator it;
    for (it = mActions.Begin(); it != mActions.End(); ++it) {
        if (it != mActions.Begin()) {
            sb += ", ";
        }
        if ((*it) != NULL) {
            sb += (*it)->GetUniqueKey();
        }
        else {
            sb += "NULL";
        }
    }

    sb += "}";
    sb += "}";
    return sb.ToString();
}


} // namespace Widget
} // namespace Droid
} // namespace Elastos
