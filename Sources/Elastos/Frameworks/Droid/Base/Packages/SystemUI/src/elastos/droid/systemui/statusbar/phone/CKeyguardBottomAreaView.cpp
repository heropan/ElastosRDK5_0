
#include "elastos/droid/systemui/statusbar/phone/CKeyguardBottomAreaView.h"
#include "elastos/droid/systemui/statusbar/phone/KeyguardTouchDelegate.h"
#include "elastos/droid/systemui/statusbar/policy/PreviewInflater.h"
#include "../../R.h"
#include "Elastos.Droid.App.h"
#include "Elastos.Droid.Content.h"
#include "Elastos.Droid.Provider.h"
#include <elastos/droid/app/ActivityManagerNative.h>
#include <elastos/droid/os/AsyncTask.h>
#include <elastos/droid/R.h>

using Elastos::Droid::App::ActivityManagerNative;
using Elastos::Droid::App::Admin::IDevicePolicyManager;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IUserInfo;
using Elastos::Droid::Internal::Widget::CLockPatternUtils;
using Elastos::Droid::Internal::Widget::ILockPatternUtils;
using Elastos::Droid::Os::AsyncTask;
using Elastos::Droid::Os::CUserHandleHelper;
using Elastos::Droid::Os::IUserHandle;
using Elastos::Droid::Os::IUserHandleHelper;
using Elastos::Droid::Provider::IMediaStore;
using Elastos::Droid::SystemUI::StatusBar::Policy::EIID_IAccessibilityStateChangedCallback;
using Elastos::Droid::View::Accessibility::CAccessibilityNodeInfoAccessibilityAction;
using Elastos::Droid::View::EIID_IViewOnClickListener;
using Elastos::Droid::View::EIID_IViewOnLongClickListener;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfoAccessibilityAction;
using Elastos::Droid::Widget::IImageView;
using Elastos::Core::CString;
using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace SystemUI {
namespace StatusBar {
namespace Phone {

CKeyguardBottomAreaView::IntrinsicSizeDrawable::IntrinsicSizeDrawable(
    /* [in] */ IDrawable* drawable,
    /* [in] */ Int32 intrinsicWidth,
    /* [in] */ Int32 intrinsicHeight)
    : mIntrinsicWidth(intrinsicWidth)
    , mIntrinsicHeight(intrinsicHeight)
{
    InsetDrawable::constructor(drawable, 0);
}

ECode CKeyguardBottomAreaView::IntrinsicSizeDrawable::GetIntrinsicWidth(
    /* [out] */ Int32* width)
{
    VALIDATE_NOT_NULL(width);
    *width = mIntrinsicWidth;
    return NOERROR;
}

ECode CKeyguardBottomAreaView::IntrinsicSizeDrawable::GetIntrinsicHeight(
    /* [out] */ Int32* height)
{
    VALIDATE_NOT_NULL(height);
    *height = mIntrinsicHeight;
    return NOERROR;
}

CKeyguardBottomAreaView::KBAAccessibilityDelegate::KBAAccessibilityDelegate(
    /* [in] */ CKeyguardBottomAreaView* host)
    : mHost(host)
{}

ECode CKeyguardBottomAreaView::KBAAccessibilityDelegate::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IView* host,
    /* [in] */ IAccessibilityNodeInfo* info)
{
    AccessibilityDelegate::OnInitializeAccessibilityNodeInfo(host, info);
    String label;
    AutoPtr<IResources> res;
    mHost->GetResources((IResources**)&res);
    if (host == IView::Probe(mHost->mLockIcon)) {
        res->GetString(R::string::unlock_label, &label);
    }
    else if (host == IView::Probe(mHost->mCameraImageView)) {
        res->GetString(R::string::camera_label, &label);
    }
    else if (host == IView::Probe(mHost->mPhoneImageView)) {
        res->GetString(R::string::phone_label, &label);
    }

    AutoPtr<IAccessibilityNodeInfoAccessibilityAction> a;
    AutoPtr<ICharSequence> l;
    CString::New(label, (ICharSequence**)&l);
    CAccessibilityNodeInfoAccessibilityAction::New(IAccessibilityNodeInfo::ACTION_CLICK, l
            , (IAccessibilityNodeInfoAccessibilityAction**)&a);
    info->AddAction(a);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::KBAAccessibilityDelegate::PerformAccessibilityAction(
    /* [in] */ IView* host,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* args,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    if (action == IAccessibilityNodeInfo::ACTION_CLICK) {
        if (host == IView::Probe(mHost->mLockIcon)) {
            mHost->mPhoneStatusBar->AnimateCollapsePanels(
                    ICommandQueue::FLAG_EXCLUDE_NONE, TRUE /* force */);
            *result = TRUE;
            return NOERROR;
        }
        else if (host == IView::Probe(mHost->mCameraImageView)) {
            mHost->LaunchCamera();
            *result = TRUE;
            return NOERROR;
        }
        else if (host == IView::Probe(mHost->mPhoneImageView)) {
            mHost->LaunchPhone();
            *result = TRUE;
            return NOERROR;
        }
    }
    return AccessibilityDelegate::PerformAccessibilityAction(host, action, args, result);
}

CKeyguardBottomAreaView::DevicePolicyReceiver::DevicePolicyReceiverRun::DevicePolicyReceiverRun(
    /* [in] */ CKeyguardBottomAreaView* host)
    : mHost(host)
{}

ECode CKeyguardBottomAreaView::DevicePolicyReceiver::DevicePolicyReceiverRun::Run()
{
    mHost->UpdateCameraVisibility();
    return NOERROR;
}

CKeyguardBottomAreaView::DevicePolicyReceiver::DevicePolicyReceiver(
    /* [in] */ CKeyguardBottomAreaView* host)
    : mHost(host)
{}

ECode CKeyguardBottomAreaView::DevicePolicyReceiver::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    AutoPtr<DevicePolicyReceiverRun> run = new DevicePolicyReceiverRun(mHost);
    Boolean tmp = FALSE;
    mHost->Post(run, &tmp);
    return NOERROR;
}

CAR_INTERFACE_IMPL(CKeyguardBottomAreaView::UnlockMethodChangedListener, Object, IOnUnlockMethodChangedListener);
CKeyguardBottomAreaView::UnlockMethodChangedListener::UnlockMethodChangedListener(
    /* [in] */ CKeyguardBottomAreaView* host)
    : mHost(host)
{}

ECode CKeyguardBottomAreaView::UnlockMethodChangedListener::OnMethodSecureChanged(
    /* [in] */ Boolean methodSecure)
{
    return mHost->OnMethodSecureChanged(methodSecure);
}

CAR_INTERFACE_IMPL(CKeyguardBottomAreaView::OnClickListener, Object, IViewOnClickListener);
CKeyguardBottomAreaView::OnClickListener::OnClickListener(
    /* [in] */ CKeyguardBottomAreaView* host)
    : mHost(host)
{}

ECode CKeyguardBottomAreaView::OnClickListener::OnClick(
    /* [in] */ IView* v)
{
    return mHost->OnClick(v);
}

CAR_INTERFACE_IMPL(CKeyguardBottomAreaView::OnLongClickListener, Object, IViewOnLongClickListener);
CKeyguardBottomAreaView::OnLongClickListener::OnLongClickListener(
    /* [in] */ CKeyguardBottomAreaView* host)
    : mHost(host)
{}

ECode CKeyguardBottomAreaView::OnLongClickListener::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    return mHost->OnLongClick(v, result);
}

CKeyguardBottomAreaView::TaskRunnable::TaskRunnable(
    /* [in] */ ITelecomManager* tm)
    : mTm(tm)
{}

ECode CKeyguardBottomAreaView::TaskRunnable::Run()
{
    mTm->ShowInCallScreen(FALSE /* showDialpad */);
    return NOERROR;
}

const String CKeyguardBottomAreaView::TAG("PhoneStatusBar/KeyguardBottomAreaView");
Boolean CKeyguardBottomAreaView::sInit = InitStatics();
AutoPtr<IIntent> CKeyguardBottomAreaView::SECURE_CAMERA_INTENT;
AutoPtr<IIntent> CKeyguardBottomAreaView::INSECURE_CAMERA_INTENT;
AutoPtr<IIntent> CKeyguardBottomAreaView::PHONE_INTENT;

CAR_OBJECT_IMPL(CKeyguardBottomAreaView);
CAR_INTERFACE_IMPL_5(CKeyguardBottomAreaView, FrameLayout, IKeyguardBottomAreaView, IViewOnClickListener \
        , IOnUnlockMethodChangedListener, IAccessibilityStateChangedCallback, IViewOnLongClickListener);
CKeyguardBottomAreaView::CKeyguardBottomAreaView()
    : mLastUnlockIconRes(0)
{
    mAccessibilityDelegate = new KBAAccessibilityDelegate(this);
    mDevicePolicyReceiver = new DevicePolicyReceiver(this);
}

Boolean CKeyguardBottomAreaView::InitStatics()
{
    CIntent::New(IMediaStore::INTENT_ACTION_STILL_IMAGE_CAMERA_SECURE, (IIntent**)&SECURE_CAMERA_INTENT);
    SECURE_CAMERA_INTENT->AddFlags(IIntent::FLAG_ACTIVITY_EXCLUDE_FROM_RECENTS);
    CIntent::New(IMediaStore::INTENT_ACTION_STILL_IMAGE_CAMERA, (IIntent**)&INSECURE_CAMERA_INTENT);
    CIntent::New(IIntent::ACTION_DIAL, (IIntent**)&PHONE_INTENT);
    return TRUE;
}

ECode CKeyguardBottomAreaView::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL);
}

ECode CKeyguardBottomAreaView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CKeyguardBottomAreaView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr)
{
    return constructor(context, attrs, defStyleAttr, 0);
}

ECode CKeyguardBottomAreaView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyleAttr,
    /* [in] */ Int32 defStyleRes)
{
    FrameLayout::constructor(context, attrs, defStyleAttr, defStyleRes);
    mTrustDrawable = new TrustDrawable(mContext);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::OnFinishInflate()
{
    FrameLayout::OnFinishInflate();
    CLockPatternUtils::New(mContext, (ILockPatternUtils**)&mLockPatternUtils);
    AutoPtr<IView> view;
    FindViewById(R::id::preview_container, (IView**)&view);
    mPreviewContainer = IViewGroup::Probe(view);

    view = NULL;
    FindViewById(R::id::camera_button, (IView**)&view);
    mCameraImageView = IKeyguardAffordanceView::Probe(view);

    view = NULL;
    FindViewById(R::id::phone_button, (IView**)&view);
    mPhoneImageView = IKeyguardAffordanceView::Probe(view);

    view = NULL;
    FindViewById(R::id::lock_icon, (IView**)&view);
    mLockIcon = IKeyguardAffordanceView::Probe(view);

    view = NULL;
    FindViewById(R::id::keyguard_indication_text, (IView**)&view);
    mIndicationText = ITextView::Probe(view);
    WatchForCameraPolicyChanges();
    UpdateCameraVisibility();
    UpdatePhoneVisibility();

    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    mUnlockMethodCache = UnlockMethodCache::GetInstance(ctx);

    AutoPtr<UnlockMethodChangedListener> listener = new UnlockMethodChangedListener(this);
    mUnlockMethodCache->AddListener(listener);
    UpdateLockIcon();
    SetClipChildren(FALSE);
    SetClipToPadding(FALSE);

    AutoPtr<ILockPatternUtils> lu;
    CLockPatternUtils::New(mContext, (ILockPatternUtils**)&lu);
    mPreviewInflater = new PreviewInflater(mContext, lu);
    InflatePreviews();

    AutoPtr<OnClickListener> click = new OnClickListener(this);
    IView::Probe(mLockIcon)->SetOnClickListener(click);
    IView::Probe(mLockIcon)->SetBackground(mTrustDrawable);

    AutoPtr<OnLongClickListener> longClick = new OnLongClickListener(this);
    IView::Probe(mLockIcon)->SetOnLongClickListener(longClick);
    IView::Probe(mCameraImageView)->SetOnClickListener(click);
    IView::Probe(mPhoneImageView)->SetOnClickListener(click);
    InitAccessibility();
    return NOERROR;
}

void CKeyguardBottomAreaView::InitAccessibility()
{
    IView::Probe(mLockIcon)->SetAccessibilityDelegate(mAccessibilityDelegate);
    IView::Probe(mPhoneImageView)->SetAccessibilityDelegate(mAccessibilityDelegate);
    IView::Probe(mCameraImageView)->SetAccessibilityDelegate(mAccessibilityDelegate);
}

void CKeyguardBottomAreaView::OnConfigurationChanged(
    /* [in] */ IConfiguration* newConfig)
{
    FrameLayout::OnConfigurationChanged(newConfig);
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    Int32 indicationBottomMargin = 0;
    res->GetDimensionPixelSize(R::dimen::keyguard_indication_margin_bottom, &indicationBottomMargin);

    AutoPtr<IViewGroupLayoutParams> vp;
    IView::Probe(mIndicationText)->GetLayoutParams((IViewGroupLayoutParams**)&vp);
    AutoPtr<IViewGroupMarginLayoutParams> mlp = IViewGroupMarginLayoutParams::Probe(vp);
    Int32 bottomMargin = 0;
    mlp->GetBottomMargin(&bottomMargin);
    if (bottomMargin != indicationBottomMargin) {
        mlp->SetBottomMargin(indicationBottomMargin);
        IView::Probe(mIndicationText)->SetLayoutParams(IViewGroupLayoutParams::Probe(mlp));
    }

    // Respect font size setting.
    Int32 size = 0;
    res->GetDimensionPixelSize(Elastos::Droid::R::dimen::text_size_small_material, &size);
    mIndicationText->SetTextSize(ITypedValue::COMPLEX_UNIT_PX, size);
}

ECode CKeyguardBottomAreaView::SetActivityStarter(
    /* [in] */ IActivityStarter* activityStarter)
{
    mActivityStarter = activityStarter;
    return NOERROR;
}

ECode CKeyguardBottomAreaView::SetFlashlightController(
    /* [in] */ IFlashlightController* flashlightController)
{
    mFlashlightController = flashlightController;
    return NOERROR;
}

ECode CKeyguardBottomAreaView::SetAccessibilityController(
    /* [in] */ IAccessibilityController* accessibilityController)
{
    mAccessibilityController = accessibilityController;
    accessibilityController->AddStateChangedCallback(this);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::SetPhoneStatusBar(
    /* [in] */ IPhoneStatusBar* phoneStatusBar)
{
    mPhoneStatusBar = phoneStatusBar;
    return NOERROR;
}

AutoPtr<IIntent> CKeyguardBottomAreaView::GetCameraIntent()
{
    assert(0 && "TODO: need the app Keyguard.");
    // KeyguardUpdateMonitor updateMonitor = KeyguardUpdateMonitor.getInstance(mContext);
    // Boolean currentUserHasTrust = updateMonitor.getUserHasTrust(
    //         mLockPatternUtils.getCurrentUser());
    // return mLockPatternUtils.isSecure() && !currentUserHasTrust
    //         ? SECURE_CAMERA_INTENT : INSECURE_CAMERA_INTENT;
    return NULL;
}

void CKeyguardBottomAreaView::UpdateCameraVisibility()
{
    AutoPtr<IResolveInfo> resolved;
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    Int32 user = 0;
    mLockPatternUtils->GetCurrentUser(&user);
    pm->ResolveActivityAsUser(GetCameraIntent(),
            IPackageManager::MATCH_DEFAULT_ONLY,
            user, (IResolveInfo**)&resolved);

    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    Boolean tmp = FALSE;
    Boolean visible = !IsCameraDisabledByDpm() && resolved != NULL
            && (res->GetBoolean(R::bool_::config_keyguardShowCameraAffordance, &tmp), tmp);
    IView::Probe(mCameraImageView)->SetVisibility(visible ? IView::VISIBLE : IView::GONE);
}

void CKeyguardBottomAreaView::UpdatePhoneVisibility()
{
    Boolean visible = IsPhoneVisible();
    IView::Probe(mPhoneImageView)->SetVisibility(visible ? IView::VISIBLE : IView::GONE);
}

Boolean CKeyguardBottomAreaView::IsPhoneVisible()
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IResolveInfo> rv;
    Boolean has = FALSE;
    pm->HasSystemFeature(IPackageManager::FEATURE_TELEPHONY, &has);
    return has && (pm->ResolveActivity(PHONE_INTENT, 0, (IResolveInfo**)&rv), rv) != NULL;
}

Boolean CKeyguardBottomAreaView::IsCameraDisabledByDpm()
{
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    AutoPtr<IInterface> obj;
    ctx->GetSystemService(IContext::DEVICE_POLICY_SERVICE, (IInterface**)&obj);
    AutoPtr<IDevicePolicyManager> dpm = IDevicePolicyManager::Probe(obj);
    if (dpm != NULL) {
        ECode ec = NOERROR;
        do {
            AutoPtr<IUserInfo> ui;
            ec = ActivityManagerNative::GetDefault()->GetCurrentUser((IUserInfo**)&ui);
            if (FAILED(ec)) {
                break;
            }
            Int32 userId = 0;
            ec = ui->GetId(&userId);
            if (FAILED(ec)) {
                break;
            }

            Int32 disabledFlags = 0;
            ec = dpm->GetKeyguardDisabledFeatures(NULL, userId, &disabledFlags);
            if (FAILED(ec)) {
                break;
            }

            Boolean tmp = FALSE;
            Boolean disabledBecauseKeyguardSecure =
                    (disabledFlags & IDevicePolicyManager::KEYGUARD_DISABLE_SECURE_CAMERA) != 0
                            && (KeyguardTouchDelegate::GetInstance(ctx)->IsSecure(&tmp), tmp);

            Boolean result = FALSE;
            ec = dpm->GetCameraDisabled(NULL, &result);
            if (FAILED(ec)) {
                break;
            }
            return result || disabledBecauseKeyguardSecure;
        } while (0) ;

        if (FAILED(ec)) {
            Logger::E(TAG, "Can't get userId");
        }
    }
    return FALSE;
}

void CKeyguardBottomAreaView::WatchForCameraPolicyChanges()
{
    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IDevicePolicyManager::ACTION_DEVICE_POLICY_MANAGER_STATE_CHANGED);
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    AutoPtr<IUserHandleHelper> helper;
    CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
    AutoPtr<IUserHandle> ALL;
    helper->GetALL((IUserHandle**)&ALL);

    AutoPtr<IIntent> i;
    ctx->RegisterReceiverAsUser(mDevicePolicyReceiver, ALL, filter, String(NULL), NULL, (IIntent**)&i);
    assert(0 && "TODO: need the app Keyguard.");
    // KeyguardUpdateMonitor.getInstance(mContext).registerCallback(mUpdateMonitorCallback);
}

ECode CKeyguardBottomAreaView::OnStateChanged(
    /* [in] */ Boolean accessibilityEnabled,
    /* [in] */ Boolean touchExplorationEnabled)
{
    IView::Probe(mCameraImageView)->SetClickable(touchExplorationEnabled);
    IView::Probe(mPhoneImageView)->SetClickable(touchExplorationEnabled);
    IView::Probe(mCameraImageView)->SetFocusable(accessibilityEnabled);
    IView::Probe(mPhoneImageView)->SetFocusable(accessibilityEnabled);
    UpdateLockIconClickability();
    return NOERROR;
}

void CKeyguardBottomAreaView::UpdateLockIconClickability()
{
    if (mAccessibilityController == NULL) {
        return;
    }
    Boolean clickToUnlock = FALSE;
    mAccessibilityController->IsTouchExplorationEnabled(&clickToUnlock);
    Boolean enabled = FALSE;
    Boolean clickToForceLock = mUnlockMethodCache->IsTrustManaged()
            && (mAccessibilityController->IsAccessibilityEnabled(&enabled), !enabled);
    Boolean longClickToForceLock = mUnlockMethodCache->IsTrustManaged()
            && !clickToForceLock;
    IView::Probe(mLockIcon)->SetClickable(clickToForceLock || clickToUnlock);
    IView::Probe(mLockIcon)->SetLongClickable(longClickToForceLock);
    IView::Probe(mLockIcon)->SetFocusable(enabled);
}

ECode CKeyguardBottomAreaView::OnClick(
    /* [in] */ IView* v)
{
    if (v == IView::Probe(mCameraImageView)) {
        LaunchCamera();
    } else if (v == IView::Probe(mPhoneImageView)) {
        LaunchPhone();
    }
    if (v == IView::Probe(mLockIcon)) {
        Boolean enabled = FALSE;
        mAccessibilityController->IsAccessibilityEnabled(&enabled);
        if (!enabled) {
            HandleTrustCircleClick();
        }
        else {
            mPhoneStatusBar->AnimateCollapsePanels(
                    ICommandQueue::FLAG_EXCLUDE_NONE, TRUE /* force */);
        }
    }
    return NOERROR;
}

ECode CKeyguardBottomAreaView::OnLongClick(
    /* [in] */ IView* v,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    HandleTrustCircleClick();
    *result = TRUE;
    return NOERROR;
}

void CKeyguardBottomAreaView::HandleTrustCircleClick()
{
    mIndicationController->ShowTransientIndication(
            R::string::keyguard_indication_trust_disabled);
    Int32 user = 0;
    mLockPatternUtils->GetCurrentUser(&user);
    mLockPatternUtils->RequireCredentialEntry(user);
}

ECode CKeyguardBottomAreaView::LaunchCamera()
{
    mFlashlightController->KillFlashlight();
    AutoPtr<IIntent> intent = GetCameraIntent();
    Int32 user = 0;
    mLockPatternUtils->GetCurrentUser(&user);
    Boolean wouldLaunchResolverActivity = PreviewInflater::WouldLaunchResolverActivity(
            mContext, intent, user);
    if (intent == SECURE_CAMERA_INTENT && !wouldLaunchResolverActivity) {
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        AutoPtr<IUserHandle> CURRENT;
        helper->GetCURRENT((IUserHandle**)&CURRENT);
        mContext->StartActivityAsUser(intent, CURRENT);
    }
    else {
        // We need to delay starting the activity because ResolverActivity finishes itself if
        // launched behind lockscreen.
        mActivityStarter->StartActivity(intent, FALSE /* dismissShade */);
    }
    return NOERROR;
}

ECode CKeyguardBottomAreaView::LaunchPhone()
{
    assert(0 && "TODO: Not implement TelecomManager::From.");
    AutoPtr<ITelecomManager> tm/* = TelecomManager::From(mContext)*/;
    Boolean call = FALSE;
    if (tm->IsInCall(&call), call) {
        AutoPtr<TaskRunnable> run = new TaskRunnable(tm);
        AsyncTask::Execute(run);
    }
    else {
        mActivityStarter->StartActivity(PHONE_INTENT, FALSE /* dismissShade */);
    }
    return NOERROR;
}

ECode CKeyguardBottomAreaView::OnVisibilityChanged(
    /* [in] */ IView* changedView,
    /* [in] */ Int32 visibility)
{
    FrameLayout::OnVisibilityChanged(changedView, visibility);
    Boolean shown = FALSE;
    if (IsShown(&shown), shown) {
        mTrustDrawable->Start();
    }
    else {
        mTrustDrawable->Stop();
    }
    if (changedView == this && visibility == IView::VISIBLE) {
        UpdateLockIcon();
        UpdateCameraVisibility();
    }
    return NOERROR;
}

ECode CKeyguardBottomAreaView::OnDetachedFromWindow()
{
    FrameLayout::OnDetachedFromWindow();
    mTrustDrawable->Stop();
    return NOERROR;
}

void CKeyguardBottomAreaView::UpdateLockIcon()
{
    Boolean shown = FALSE;
    IsShown(&shown);
    assert(0 && "TODO: need the app Keyguard.");
    Boolean visible = shown/* && KeyguardUpdateMonitor.getInstance(mContext).isScreenOn()*/;
    if (visible) {
        mTrustDrawable->Start();
    }
    else {
        mTrustDrawable->Stop();
    }
    if (!visible) {
        return;
    }
    // TODO: Real icon for facelock.
    Int32 iconRes = mUnlockMethodCache->IsFaceUnlockRunning()
            ? Elastos::Droid::R::drawable::ic_account_circle
            : mUnlockMethodCache->IsMethodInsecure() ? R::drawable::ic_lock_open_24dp
            : R::drawable::ic_lock_24dp;
    if (mLastUnlockIconRes != iconRes) {
        AutoPtr<IDrawable> icon;
        mContext->GetDrawable(iconRes, (IDrawable**)&icon);
        AutoPtr<IResources> res;
        GetResources((IResources**)&res);
        Int32 iconHeight = 0;
        res->GetDimensionPixelSize(R::dimen::keyguard_affordance_icon_height, &iconHeight);
        Int32 iconWidth = 0;
        res->GetDimensionPixelSize(R::dimen::keyguard_affordance_icon_width, &iconWidth);
        Int32 h = 0, w = 0;
        icon->GetIntrinsicHeight(&h);
        if (h != iconHeight || (icon->GetIntrinsicWidth(&w), w) != iconWidth) {
            icon = new IntrinsicSizeDrawable(icon, iconWidth, iconHeight);
        }
        IImageView::Probe(mLockIcon)->SetImageDrawable(icon);
    }
    Boolean trustManaged = mUnlockMethodCache->IsTrustManaged();
    mTrustDrawable->SetTrustManaged(trustManaged);
    UpdateLockIconClickability();
}

ECode CKeyguardBottomAreaView::GetPhoneView(
    /* [out] */ IKeyguardAffordanceView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mPhoneImageView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::GetCameraView(
    /* [out] */ IKeyguardAffordanceView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mCameraImageView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::GetPhonePreview(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mPhonePreview;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::GetCameraPreview(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mCameraPreview;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::GetLockIcon(
    /* [out] */ IKeyguardAffordanceView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mLockIcon;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::GetIndicationView(
    /* [out] */ IView** view)
{
    VALIDATE_NOT_NULL(view);
    *view = IView::Probe(mIndicationText);
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CKeyguardBottomAreaView::HasOverlappingRendering(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    return NOERROR;
}

ECode CKeyguardBottomAreaView::OnMethodSecureChanged(
    /* [in] */ Boolean methodSecure)
{
    UpdateLockIcon();
    UpdateCameraVisibility();
    return NOERROR;
}

void CKeyguardBottomAreaView::InflatePreviews()
{
    mPreviewInflater->InflatePreview(PHONE_INTENT, (IView**)&mPhonePreview);
    mPreviewInflater->InflatePreview(GetCameraIntent(), (IView**)&mCameraPreview);
    if (mPhonePreview != NULL) {
        mPreviewContainer->AddView(mPhonePreview);
        mPhonePreview->SetVisibility(IView::INVISIBLE);
    }
    if (mCameraPreview != NULL) {
        mPreviewContainer->AddView(mCameraPreview);
        mCameraPreview->SetVisibility(IView::INVISIBLE);
    }
}

ECode CKeyguardBottomAreaView::SetKeyguardIndicationController(
    /* [in] */ IKeyguardIndicationController* keyguardIndicationController)
{
    mIndicationController = keyguardIndicationController;
    return NOERROR;
}

} // namespace Phone
} // namespace StatusBar
} // namespace SystemUI
} // namespace Droid
} // namespace Elastos
