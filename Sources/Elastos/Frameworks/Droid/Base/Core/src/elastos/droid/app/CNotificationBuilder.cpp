#include "CNotificationBuilder.h"
#include "elastos/droid/R.h"
#include "elastos/droid/os/CBundle.h"
#include "elastos/droid/os/SystemClock.h"
#include "elastos/droid/app/CNotificationAction.h"
#include "elastos/droid/app/CNotification.h"
#include "elastos/droid/widget/CRemoteViews.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Text::INumberFormatHelper;
using Elastos::Text::CNumberFormatHelper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Widget::CRemoteViews;

namespace Elastos {
namespace Droid {
namespace App {

const Int32 CNotificationBuilder::MAX_ACTION_BUTTONS = 3;
const Float CNotificationBuilder::LARGE_TEXT_SCALE = 1.3f;
const Boolean CNotificationBuilder::DBG = FALSE;

const String CNotificationBuilder::EXTRA_REBUILD_CONTEXT_APPLICATION_INFO("android.rebuild.applicationInfo");

const Boolean CNotificationBuilder::STRIP_AND_REBUILD = TRUE;

CNotificationBuilder::CNotificationBuilder()
    : mWhen(0)
    , mSmallIcon(0)
    , mSmallIconLevel(0)
    , mNumber(0)
    , mAudioStreamType(0)
    , mLedArgb(0)
    , mLedOnMs(0)
    , mLedOffMs(0)
    , mDefaults(0)
    , mFlags(0)
    , mProgressMax(0)
    , mProgress(0)
    , mProgressIndeterminate(FALSE)
    , mPriority(0)
    , mUseChronometer(FALSE)
    , mShowWhen(TRUE)
    , mVisibility(VISIBILITY_PRIVATE)
    , mColor(COLOR_DEFAULT)
    , mOriginatingUserId(0)
    , mHasThreeLines(FALSE)
{
}

CNotificationBuilder::~CNotificationBuilder()
{
}

ECode CNotificationBuilder::constructor(
    /* [in] */ IContext *context)
{
    /*
     * Important compatibility note!
     * Some apps out in the wild create a Notification.Builder in their Activity subclass
     * constructor for later use. At this point Activities - themselves subclasses of
     * ContextWrapper - do not have their inner Context populated yet. This means that
     * any calls to Context methods from within this constructor can cause NPEs in existing
     * apps. Any data populated from mContext should therefore be populated lazily to
     * preserve compatibility.
     */
    mContext = context;

    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mWhen);
    mAudioStreamType = INotification::STREAM_DEFAULT;
    mAudioAttributes = AUDIO_ATTRIBUTES_DEFAULT;
    mPriority = INotification::PRIORITY_DEFAULT;

    mColorUtil = context.getApplicationInfo().targetSdkVersion < Build.VERSION_CODES.LOLLIPOP ?
            NotificationColorUtil.getInstance(mContext) : null;
    return NOERROR;
}

ECode CNotificationBuilder::constructor(
    /* [in] */ IContext* context,
    /* [in] */ INotification* n)
{
    FAIL_RETURN(constructor(context))
    mRebuildNotification = n;
    restoreFromNotification(n);

    Style style = null;
    Bundle extras = n.extras;
    String templateClass = extras.getString(EXTRA_TEMPLATE);
    if (!TextUtils.isEmpty(templateClass)) {
        Class<? extends Style> styleClass = getNotificationStyleClass(templateClass);
        if (styleClass == null) {
            Log.d(TAG, "Unknown style class: " + styleClass);
            return;
        }

        try {
            Constructor<? extends Style> constructor = styleClass.getConstructor();
            style = constructor.newInstance();
            style.restoreFromExtras(extras);
        } catch (Throwable t) {
            Log.e(TAG, "Could not create Style", t);
            return;
        }
    }
    if (style != null) {
        setStyle(style);
    }
    return NOERROR;
}

ECode CNotificationBuilder::SetWhen(
    /* [in] */ Int64 when)
{
    mWhen = when;
    return NOERROR;
}

ECode CNotificationBuilder::SetShowWhen(
    /* [in] */ Boolean show)
{
    mShowWhen = show;
    return NOERROR;
}

ECode CNotificationBuilder::SetUsesChronometer(
    /* [in] */ Boolean b)
{
    mUseChronometer = b;
    return NOERROR;
}

ECode CNotificationBuilder::SetSmallIcon(
    /* [in] */ Int32 icon)
{
    mSmallIcon = icon;
    return NOERROR;
}

ECode CNotificationBuilder::SetSmallIcon(
    /* [in] */ Int32 icon,
    /* [in] */ Int32 level)
{
    mSmallIcon = icon;
    mSmallIconLevel = level;
    return NOERROR;
}

ECode CNotificationBuilder::SetContentTitle(
    /* [in] */ ICharSequence *title)
{
    mContentTitle = SafeCharSequence(title);
    return NOERROR;
}

ECode CNotificationBuilder::SetContentText(
    /* [in] */ ICharSequence *text)
{
    mContentText = SafeCharSequence(text);
    return NOERROR;
}

ECode CNotificationBuilder::SetSubText(
    /* [in] */ ICharSequence *text)
{
    mSubText = SafeCharSequence(text);
    return NOERROR;
}

ECode CNotificationBuilder::SetNumber(
    /* [in] */ Int32 number)
{
    mNumber = number;
    return NOERROR;
}

ECode CNotificationBuilder::SetContentInfo(
    /* [in] */ ICharSequence *info)
{
    mContentInfo = SafeCharSequence(info);
    return NOERROR;
}

ECode CNotificationBuilder::SetProgress(
    /* [in] */ Int32 max,
    /* [in] */ Int32 progress,
    /* [in] */ Boolean indeterminate)
{
    mProgressMax = max;
    mProgress = progress;
    mProgressIndeterminate = indeterminate;
    return NOERROR;
}

ECode CNotificationBuilder::SetContent(
    /* [in] */ IRemoteViews *views)
{
    mContentView = views;
    return NOERROR;
}

ECode CNotificationBuilder::SetContentIntent(
    /* [in] */ IPendingIntent *intent)
{
    mContentIntent = intent;
    return NOERROR;
}

ECode CNotificationBuilder::SetDeleteIntent(
    /* [in] */ IPendingIntent *intent)
{
    mDeleteIntent = intent;
    return NOERROR;
}

ECode CNotificationBuilder::SetFullScreenIntent(
    /* [in] */ IPendingIntent *intent,
    /* [in] */ Boolean highPriority)
{
    mFullScreenIntent = intent;
    SetFlag(INotification::FLAG_HIGH_PRIORITY, highPriority);
    return NOERROR;
}

ECode CNotificationBuilder::SetTicker(
    /* [in] */ ICharSequence *tickerText)
{
    mTickerText = SafeCharSequence(tickerText);
    return NOERROR;
}

ECode CNotificationBuilder::SetTicker(
    /* [in] */ ICharSequence *tickerText,
    /* [in] */ IRemoteViews *views)
{
    mTickerText = SafeCharSequence(tickerText);
    mTickerView = views; // we'll save it for you anyway
    return NOERROR;
}

ECode CNotificationBuilder::SetLargeIcon(
    /* [in] */ IBitmap *icon)
{
    mLargeIcon = icon;
    return NOERROR;
}

ECode CNotificationBuilder::SetSound(
    /* [in] */ IUri *sound)
{
    mSound = sound;
    mAudioAttributes = AUDIO_ATTRIBUTES_DEFAULT;
    return NOERROR;
}

ECode CNotificationBuilder::SetSound(
    /* [in] */ IUri *sound,
    /* [in] */ Int32 streamType)
{
    mSound = sound;
    mAudioStreamType = streamType;
    return NOERROR;
}

ECode CNotificationBuilder::SetSound(
    /* [in] */ IUri *sound,
    /* [in] */ IAudioAttributes* audioAttributes)
{
    mSound = sound;
    mAudioAttributes = audioAttributes;
    return NOERROR;
}

ECode CNotificationBuilder::SetVibrate(
    /* [in] */ ArrayOf<Int64> *pattern)
{
    mVibrate = pattern;
    return NOERROR;
}

ECode CNotificationBuilder::SetLights(
    /* [in] */ Int32 argb,
    /* [in] */ Int32 onMs,
    /* [in] */ Int32 offMs)
{
    mLedArgb = argb;
    mLedOnMs = onMs;
    mLedOffMs = offMs;
    return NOERROR;
}

ECode CNotificationBuilder::SetOngoing(
    /* [in] */ Boolean ongoing)
{
    SetFlag(INotification::FLAG_ONGOING_EVENT, ongoing);
    return NOERROR;
}

ECode CNotificationBuilder::SetOnlyAlertOnce(
    /* [in] */ Boolean onlyAlertOnce)
{
    SetFlag(INotification::FLAG_ONLY_ALERT_ONCE, onlyAlertOnce);
    return NOERROR;
}

ECode CNotificationBuilder::SetAutoCancel(
    /* [in] */ Boolean autoCancel)
{
    SetFlag(INotification::FLAG_AUTO_CANCEL, autoCancel);
    return NOERROR;
}

ECode CNotificationBuilder::SetLocalOnly(
    /* [in] */ Boolean localOnly)
{
    return SetFlag(FLAG_LOCAL_ONLY, localOnly);;
}

ECode CNotificationBuilder::SetDefaults(
    /* [in] */ Int32 defaults)
{
    mDefaults = defaults;
    return NOERROR;
}

ECode CNotificationBuilder::SetPriority(
    /* [in] */ Int32 pri)
{
    mPriority = pri;
    return NOERROR;
}

ECode CNotificationBuilder::SetGroup(
    /* [in] */ const String& groupKey)
{
    mGroupKey = groupKey;
    return NOERROR;
}

ECode CNotificationBuilder::SetGroupSummary(
    /* [in] */ Boolean isGroupSummary)
{
    return SetFlag(FLAG_GROUP_SUMMARY, isGroupSummary);
}

ECode CNotificationBuilder::SetSortKey(
    /* [in] */ const String& groupKey)
{
    mSortKey = sortKey;
    return NOERROR;
}

ECode CNotificationBuilder::SetCategory(
    /* [in] */ const String& category)
{
    mCategory = category;
    return NOERROR;
}

ECode CNotificationBuilder::AddPerson(
    /* [in] */ const String& person)
{
    mPeople.PushBack(person);
    return NOERROR;
}

ECode CNotificationBuilder::AddExtras(
    /* [in] */ IBundle * extras)
{
    if (extras != null) {
        if (mExtras == null) {
            mExtras = new Bundle(extras);
        } else {
            mExtras.putAll(extras);
        }
    }
    return NOERROR;
}

ECode CNotificationBuilder::SetExtras(
    /* [in] */ IBundle *extras)
{
    mExtras = extras;
    return NOERROR;
}

ECode CNotificationBuilder::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras)
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

ECode CNotificationBuilder::AddAction(
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence *title,
    /* [in] */ IPendingIntent *intent)
{
    AutoPtr<INotificationAction> action;
    CNotificationAction::New(icon, SafeCharSequence(title), intent, (INotificationAction**)&action);
    mActions.PushBack(action);
    return NOERROR;
}

ECode CNotificationBuilder::AddAction(
    /* [in] */ INotificationAction* action)
{
    mActions.PushBack(action);
    return NOERROR;
}

ECode CNotificationBuilder::SetStyle(
    /* [in] */ INotificationStyle *style)
{
    if (mStyle.Get() != style) {
        mStyle = style;
        if (mStyle != NULL) {
            mStyle->SetBuilder(this);
        }
    }
    return NOERROR;
}

ECode CNotificationBuilder::SetVisibility(
    /* [in] */ Int32 visibility);
{
    mVisibility = visibility;
    return NOERROR;
}

ECode CNotificationBuilder::SetPublicVersion(
    /* [in] */ INotification* n)
{
    mPublicVersion = n;
    return NOERROR;
}

ECode CNotificationBuilder::Extend(
    /* [in] */ IExtender* extender)
{
    extender->Extend(this);
    return NOERROR;
}

ECode CNotificationBuilder::SetColor(
    /* [in] */ Int32 argb);
{
    mColor = argb;
    return NOERROR;
}

AutoPtr<IDrawable> CNotificationBuilder::GetProfileBadgeDrawable()
{
    // Note: This assumes that the current user can read the profile badge of the
    // originating user.
    return mContext.getPackageManager().getUserBadgeForDensity(
            new UserHandle(mOriginatingUserId), 0);
}

AutoPtr<IBitmap> CNotificationBuilder::GetProfileBadge()
{
    Drawable badge = getProfileBadgeDrawable();
    if (badge == null) {
        return null;
    }
    final int size = mContext.getResources().getDimensionPixelSize(
            R.dimen.notification_badge_size);
    Bitmap bitmap = Bitmap.createBitmap(size, size, Bitmap.Config.ARGB_8888);
    Canvas canvas = new Canvas(bitmap);
    badge.setBounds(0, 0, size, size);
    badge.draw(canvas);
    return bitmap;
}

Boolean CNotificationBuilder::AddProfileBadge(
    /* [in] */ IRemoteViews* contentView,
    /* [in] */ Int32 resId)
{
    Bitmap profileBadge = getProfileBadge();

    contentView->SetViewVisibility(R::id::profile_badge_large_template, View.GONE);
    contentView->SetViewVisibility(R::id::profile_badge_line2, View.GONE);
    contentView->SetViewVisibility(R::id::profile_badge_line3, View.GONE);

    if (profileBadge != null) {
        contentView->SetImageViewBitmap(resId, profileBadge);
        contentView->SetViewVisibility(resId, View.VISIBLE);

        // Make sure Line 3 is visible. As badge will be here if there
        // is no text to display.
        if (resId == R::id::profile_badge_line3) {
            contentView->SetViewVisibility(R::id::line3, View.VISIBLE);
        }
        return true;
    }
    return false;
}

ECode CNotificationBuilder::ShrinkLine3Text(
    /* [in] */ IRemoteViews* contentView)
{
    float subTextSize = mContext.getResources().getDimensionPixelSize(
            R.dimen.notification_subtext_size);
    contentView->SetTextViewTextSize(R::id::text, TypedValue.COMPLEX_UNIT_PX, subTextSize);
}

ECode CNotificationBuilder::UnshrinkLine3Text(
    /* [in] */ IRemoteViews* contentView)
{
    float regularTextSize = mContext.getResources().getDimensionPixelSize(
            com.android.internal.R.dimen.notification_text_size);
    contentView->SetTextViewTextSize(R::id::text, TypedValue.COMPLEX_UNIT_PX, regularTextSize);
}

ECode CNotificationBuilder::ResetStandardTemplate(
    /* [in] */ IRemoteViews* contentView)
{
    removeLargeIconBackground(contentView);
    contentView->SetViewPadding(R::id::icon, 0, 0, 0, 0);
    contentView->SetImageViewResource(R::id::icon, 0);
    contentView->SetInt(R::id::icon, "setBackgroundResource", 0);
    contentView->SetViewVisibility(R::id::right_icon, View.GONE);
    contentView->SetInt(R::id::right_icon, "setBackgroundResource", 0);
    contentView->SetImageViewResource(R::id::right_icon, 0);
    contentView->SetImageViewResource(R::id::icon, 0);
    contentView->SetTextViewText(R::id::title, null);
    contentView->SetTextViewText(R::id::text, null);
    unshrinkLine3Text(contentView);
    contentView->SetTextViewText(R::id::text2, null);
    contentView->SetViewVisibility(R::id::text2, View.GONE);
    contentView->SetViewVisibility(R::id::info, View.GONE);
    contentView->SetViewVisibility(R::id::time, View.GONE);
    contentView->SetViewVisibility(R::id::line3, View.GONE);
    contentView->SetViewVisibility(R::id::overflow_divider, View.GONE);
    contentView->SetViewVisibility(R::id::progress, View.GONE);
    contentView->SetViewVisibility(R::id::chronometer, View.GONE);
    contentView->SetViewVisibility(R::id::time, View.GONE);
}

AutoPtr<IRemoteViews> CNotificationBuilder::ApplyStandardTemplate(
    /* [in] */ Int32 resId)
{
    return ApplyStandardTemplate(resId, true /* hasProgress */);
}

ECode CNotificationBuilder::GetNotification(
    /* [out] */ INotification **notification)
{
    VALIDATE_NOT_NULL(notification);
    return Build(notification);
}

ECode CNotificationBuilder::Build(
    /* [out] */ INotification **notification)
{
    VALIDATE_NOT_NULL(notification);

    // if (mStyle != NULL) {
    //     return mStyle->Build(notification);
    // }
    // else {
    //     return BuildUnstyled(notification);
    // }

    mOriginatingUserId = mContext.getUserId();
    mHasThreeLines = hasThreeLines();

    Notification n = buildUnstyled();

    if (mStyle != null) {
        n = mStyle.buildStyled(n);
    }

    if (mExtras != null) {
        n.extras.putAll(mExtras);
    }

    if (mRebuildBundle.size() > 0) {
        n.extras.putAll(mRebuildBundle);
        mRebuildBundle.clear();
    }

    populateExtras(n.extras);
    if (mStyle != null) {
        mStyle.addExtras(n.extras);
    }

    mHasThreeLines = false;
    return n;
}

AutoPtr<INotification> CNotificationBuilder::BuildInto(
    /* [in] */ INotification* n)
{
    build().cloneInto(n, true);
    return n;
}

ECode CNotificationBuilder::GetSubText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mSubText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CNotificationBuilder::GetContentText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mContentText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

Int32 CNotificationBuilder::GetBaseLayoutResource();
{
    return R_layout_notification_template_material_base;
}

Int32 CNotificationBuilder::GetBigBaseLayoutResource()
{
    return R_layout_notification_template_material_big_base;
}

Int32 CNotificationBuilder::GetBigPictureLayoutResource()
{
    return R_layout_notification_template_material_big_picture;
}

Int32 CNotificationBuilder::GetBigTextLayoutResource()
{
    return R_layout_notification_template_material_big_text;
}

Int32 CNotificationBuilder::GetInboxLayoutResource()
 {
    return R_layout_notification_template_material_inbox;
}

Int32 CNotificationBuilder::GetActionLayoutResource()
{
    return R_layout_notification_material_action;
}

Int32 CNotificationBuilder::GetActionTombstoneLayoutResource() {
    return R_layout_notification_material_action_tombstone;
}

ECode CNotificationBuilder::ApplyStandardTemplateWithActions(
    /* [in] */ Int32 layoutId,
    /* [out] */ IRemoteViews** remoteViews)
{
    VALIDATE_NOT_NULL(remoteViews);
    *remoteViews = NULL;

    AutoPtr<IRemoteViews> big = ApplyStandardTemplate(layoutId);

    ResetStandardTemplateWithActions(big);

    Int32 N = mActions.GetSize();
    if (N > 0) {
        rv->SetViewVisibility(R::id::actions, IView::VISIBLE);
        rv->SetViewVisibility(R::id::action_divider, IView::VISIBLE);
        if (N > MAX_ACTION_BUTTONS) N = MAX_ACTION_BUTTONS;

        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IRemoteViews> button = GenerateActionButton(mActions[i]);
            big->AddView(R::id::actions, button);
        }
    }

    *remoteViews = big;
    REFCOUNT_ADD(*remoteViews);
    return NOERROR;
}

void CNotificationBuilder::SetFlag(
    /* [in] */ Int32 mask,
    /* [in] */ Boolean value)
{
    if (value) {
        mFlags |= mask;
    }
    else {
        mFlags &= ~mask;
    }
}

AutoPtr<IRemoteViews> CNotificationBuilder::ApplyStandardTemplate(
    /* [in] */ Int32 resId,
    /* [in] */ Boolean hasProgress)
{
    AutoPtr<IRemoteViews> contentView;
    AutoPtr<IApplicationInfo> ai;
    mContext->GetApplicationInfo((IApplicationInfo**)&ai);
    CRemoteViews::New(ai, resId, (IRemoteViews**)&contentView);

    Boolean showLine3 = FALSE;
    Boolean showLine2 = FALSE;
    Boolean contentTextInLine2 = FALSE;

    String name;
    mContext->GetPackageName(&name);

    if (mLargeIcon != NULL) {
        contentView->SetImageViewBitmap(R::id::icon, mLargeIcon);
        ProcessLargeLegacyIcon(mLargeIcon, contentView);
        contentView->SetImageViewResource(R::id::right_icon, mSmallIcon);
        contentView->SetViewVisibility(R::id::right_icon, View.VISIBLE);
        ProcessSmallRightIcon(mSmallIcon, contentView);
    }
    else { // small icon at left
        contentView->SetImageViewResource(R::id::icon, mSmallIcon);
        contentView->SetViewVisibility(R::id::icon, View.VISIBLE);
        ProcessSmallIconAsLarge(mSmallIcon, contentView);
    }

    if (mPriority < INotification::PRIORITY_LOW) {
        String funcName("SetBackgroundResource");
        contentView->SetInt32(R::id::icon,
                funcName, R::drawable::notification_template_icon_low_bg);
        contentView->SetInt32(R::id::status_bar_latest_event_content,
                funcName, R::drawable::notification_bg_low);
    }

    if (mContentTitle != NULL) {
        contentView->SetTextViewText(R::id::title, ProcessLegacyText(mContentTitle));
    }

    if (mContentText != NULL) {
        contentView->SetTextViewText(R::id::text, ProcessLegacyText(mContentText));
        showLine3 = TRUE;
    }

    if (mContentInfo != NULL) {
        contentView->SetTextViewText(R::id::info, ProcessLegacyText(mContentInfo));
        contentView->SetViewVisibility(R::id::info, IView::VISIBLE);
        showLine3 = TRUE;
    }
    else if (mNumber > 0) {
        AutoPtr<IResources> res;
        mContext->GetResources((IResources**)&res);
        Int32 tooBig;
        res->GetInteger(R::integer::status_bar_notification_info_maxnum, &tooBig);

        if (mNumber > tooBig) {
            AutoPtr<ICharSequence> cs;
            String str;
            res->GetString(R::string::status_bar_notification_info_overflow, &str);
            CString::New(str, (ICharSequence**)&cs);
            contentView->SetTextViewText(R::id::info, ProcessLegacyText(cs));
        }
        else {
            AutoPtr<INumberFormatHelper> helper;
            CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&helper);
            AutoPtr<INumberFormat> f;
            helper->GetIntegerInstance((INumberFormat**)&f);

            AutoPtr<ICharSequence> cs;
            String str;
            f->FormatInt64(mNumber, &str);
            CString::New(str, (ICharSequence**)&cs);
            contentView->SetTextViewText(R::id::info, ProcessLegacyText(cs));
        }
        contentView->SetViewVisibility(R::id::info, IView::VISIBLE);
        showLine3 = TRUE;
    }
    else {
        contentView->SetViewVisibility(R::id::info, IView::GONE);
    }

    // Need to show three lines?
    if (mSubText != NULL) {
        contentView->SetTextViewText(R::id::text, mSubText);
        if (mContentText != NULL) {
            contentView->SetTextViewText(R::id::text2, mContentText);
            contentView->SetViewVisibility(R::id::text2, IView::VISIBLE);
            showLine2 = TRUE;
        }
        else {
            contentView->SetViewVisibility(R::id::text2, IView::GONE);
        }
    }
    else {
        contentView->SetViewVisibility(R::id::text2, IView::GONE);
        if (hasProgress && (mProgressMax != 0 || mProgressIndeterminate)) {
            contentView->SetViewVisibility(R::id::progress, IView::VISIBLE);
            contentView->SetProgressBar(
                R::id::progress, mProgressMax, mProgress, mProgressIndeterminate);
            showLine2 = TRUE;
        }
        else {
            contentView->SetViewVisibility(R::id::progress, IView::GONE);
        }
    }

    if (showLine2) {
        // need to shrink all the type to make sure everything fits
        ShrinkLine3Text(contentView);
    }

    if (ShowsTimeOrChronometer()) {
        if (mUseChronometer) {
            AutoPtr<ISystem> system;
            Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
            contentView->SetViewVisibility(R::id::chronometer, IView::VISIBLE);
            Int64 millis;
            system->GetCurrentTimeMillis(&millis);
            contentView->SetInt64(R::id::chronometer, String("SetBase"),
                    mWhen + (SystemClock::GetElapsedRealtime() - millis));
            contentView->SetBoolean(R::id::chronometer, String("SetStarted"), TRUE);
        }
        else {
            contentView->SetViewVisibility(R::id::time, IView::VISIBLE);
            contentView->SetInt64(R::id::time, String("SetTime"), mWhen);
        }
    }

    // Adjust padding depending on line count and font size.
    contentView->SetViewPadding(R::id::line1, 0, CalculateTopPadding(mContext,
            mHasThreeLines, mContext.getResources().getConfiguration().fontScale),
            0, 0);

    // We want to add badge to first line of text.
    Boolean addedBadge = addProfileBadge(contentView,
            contentTextInLine2 ? R::id::profile_badge_line2 : R::id::profile_badge_line3);
    // If we added the badge to line 3 then we should show line 3.
    if (addedBadge && !contentTextInLine2) {
        showLine3 = true;
    }

    // Note getStandardView may hide line 3 again.
    contentView->SetViewVisibility(R::id::line3, showLine3 ? IView::VISIBLE : IView::GONE);
    contentView->SetViewVisibility(R::id::overflow_divider, showLine3 ? IView::VISIBLE : IView::GONE);
    return contentView;
}

Boolean CNotificationBuilder::ShowsTimeOrChronometer()
{
    return mWhen != 0 && mShowWhen;
}

Boolean CNotificationBuilder::HasThreeLines()
{
    Boolean contentTextInLine2 = mSubText != null && mContentText != null;
    Boolean hasProgress = mStyle == null || mStyle.hasProgress();
    // If we have content text in line 2, badge goes into line 2, or line 3 otherwise
    Boolean badgeInLine3 = getProfileBadgeDrawable() != null && !contentTextInLine2;
    Boolean hasLine3 = mContentText != null || mContentInfo != null || mNumber > 0
            || badgeInLine3;
    Boolean hasLine2 = (mSubText != null && mContentText != null) ||
            (hasProgress && mSubText == null
                    && (mProgressMax != 0 || mProgressIndeterminate));
    return hasLine2 && hasLine3;
}

static Int32 CNotificationBuilder::CalculateTopPadding(
    /* [in] */ IContext* ctx,
    /* [in] */ Boolean* hasThreeLines,
    /* [in] */ Float fontScale)
{
    int padding = ctx.getResources().getDimensionPixelSize(hasThreeLines
            ? R.dimen.notification_top_pad_narrow
            : R.dimen.notification_top_pad);
    int largePadding = ctx.getResources().getDimensionPixelSize(hasThreeLines
            ? R.dimen.notification_top_pad_large_text_narrow
            : R.dimen.notification_top_pad_large_text);
    float largeFactor = (MathUtils.constrain(fontScale, 1.0f, LARGE_TEXT_SCALE) - 1f)
            / (LARGE_TEXT_SCALE - 1f);

    // Linearly interpolate the padding between large and normal with the font scale ranging
    // from 1f to LARGE_TEXT_SCALE
    return Math.round((1 - largeFactor) * padding + largeFactor * largePadding);
}

ECode CNotificationBuilder::ResetStandardTemplateWithActions(
    /* [in] */ IRemoteViews* big)
{
    big.setViewVisibility(R.id.actions, View.GONE);
    big.setViewVisibility(R.id.action_divider, View.GONE);
    big.removeAllViews(R.id.actions);
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeContentView()
{
    if (mContentView != NULL) {
        return mContentView;
    }
    else {
        return ApplyStandardTemplate(GetBaseLayoutResource());
    }
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeTickerView()
{
    if (mTickerView != NULL) {
        return mTickerView;
    }
    return NULL; // tickers are not created by default anymore
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeBigContentView()
{
    if (mActions.IsEmpty()) return NULL;
    AutoPtr<IRemoteViews> rv;
    ApplyStandardTemplateWithActions(GetBigBaseLayoutResource(), (IRemoteViews**)&rv);
    return rv;
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeHeadsUpContentView()
{
    if (mActions.IsEmpty()) return NULL;
    AutoPtr<IRemoteViews> rv;
    ApplyStandardTemplateWithActions(GetBigBaseLayoutResource(), (IRemoteViews**)&rv);
    return rv;
}

AutoPtr<IRemoteViews> CNotificationBuilder::GenerateActionButton(
    /* [in] */ INotificationAction * action)
{
    assert(action != NULL);

    AutoPtr<IPendingIntent> actionIntent;
    action->GetActionIntent((IPendingIntent**)&actionIntent);
    Boolean tombstone = (actionIntent == NULL);
    AutoPtr<IRemoteViews> button;
    String name;
    mContext->GetPackageName(&name);

    CRemoteViews::New(name,
        tombstone ? GetActionTombstoneLayoutResource() : GetActionLayoutResource(),
        (IRemoteViews**)&button);

    Int32 icon;
    action->GetIcon(&icon);
    AutoPtr<ICharSequence> title;
    action->GetTitle((ICharSequence**)&title);
    button->SetTextViewCompoundDrawablesRelative(R::id::action0, icon, 0, 0, 0);
    button->SetTextViewText(R::id::action0, ProcessLegacyText(title));
    if (!tombstone) {
        button->SetOnClickPendingIntent(R::id::action0, actionIntent);
    }
    button->SetContentDescription(R::id::action0, title);
    ProcessLegacyAction(action, button);
    return button;
}

Boolean CNotificationBuilder::IsLegacy()
{
    return mColorUtil != NULL;
}

ECode CNotificationBuilder::ProcessLegacyAction(
    /* [in] */ INotificaitonAction* action,
    /* [in] */ IRemoteViews* button)
{
    if (!isLegacy() || mColorUtil.isGrayscaleIcon(mContext, action.icon)) {
        button.setTextViewCompoundDrawablesRelativeColorFilter(R.id.action0, 0,
                mContext.getResources().getColor(R.color.notification_action_color_filter),
                PorterDuff.Mode.MULTIPLY);
    }
}

AutoPtr<ICharSequence> CNotificationBuilder::ProcessLegacyText(
    /* [in] */ ICharSequence* charSequence)
{
    if (isLegacy()) {
        return mColorUtil.invertCharSequenceColors(charSequence);
    } else {
        return charSequence;
    }
}

ECode CNotificationBuilder::processSmallIconAsLarge(
    /* [in] */ Int32 largeIconId,
    /* [in] */ IRemoteViews* contentView)
{
    if (!isLegacy() || mColorUtil.isGrayscaleIcon(mContext, largeIconId)) {
        applyLargeIconBackground(contentView);
    }
}

ECode CNotificationBuilder::processLargeLegacyIcon(
    /* [in] */ IBitmap* largeIcon,
    /* [in] */ IRemoteViews* contentView)
{
    if (isLegacy() && mColorUtil.isGrayscaleIcon(largeIcon)) {
        applyLargeIconBackground(contentView);
    } else {
        removeLargeIconBackground(contentView);
    }
}

ECode CNotificationBuilder::ApplyLargeIconBackground(
    /* [in] */ IRemoteViews* contentView)
{
    contentView.setInt(R.id.icon, "setBackgroundResource",
            R.drawable.notification_icon_legacy_bg);

    contentView.setDrawableParameters(
            R.id.icon,
            true,
            -1,
            resolveColor(),
            PorterDuff.Mode.SRC_ATOP,
            -1);

    int padding = mContext.getResources().getDimensionPixelSize(
            R.dimen.notification_large_icon_circle_padding);
    contentView.setViewPadding(R.id.icon, padding, padding, padding, padding);
}

ECode CNotificationBuilder::RemoveLargeIconBackground(
    /* [in] */ IRemoteViews* contentView)
{
    contentView.setInt(R.id.icon, "setBackgroundResource", 0);
}

/**
 * Recolor small icons when used in the R.id.right_icon slot.
 */
ECode CNotificationBuilder::ProcessSmallRightIcon(
    /* [in] */ Int32 smallIconDrawableId,
    /* [in] */ IRemoteViews* contentView)
{
    if (!isLegacy() || mColorUtil.isGrayscaleIcon(mContext, smallIconDrawableId)) {
        contentView.setDrawableParameters(R.id.right_icon, false, -1,
                0xFFFFFFFF,
                PorterDuff.Mode.SRC_ATOP, -1);

        contentView.setInt(R.id.right_icon,
                "setBackgroundResource",
                R.drawable.notification_icon_legacy_bg);

        contentView.setDrawableParameters(
                R.id.right_icon,
                true,
                -1,
                resolveColor(),
                PorterDuff.Mode.SRC_ATOP,
                -1);
    }
}

Int32 CNotificationBuilder::SanitizeColor()
{
    if (mColor != COLOR_DEFAULT) {
        mColor |= 0xFF000000; // no alpha for custom colors
    }
    return mColor;
}

Int32 CNotificationBuilder::ResolveColor()
{
    if (mColor == COLOR_DEFAULT) {
        return mContext.getResources().getColor(R.color.notification_icon_bg_color);
    }
    return mColor;
}
        //

ECode CNotificationBuilder::BuildUnstyled(
    /* [out] */ INotification** notification)
{
    VALIDATE_NOT_NULL(notification);
    *notification = NULL;

    AutoPtr<INotification> n;
    CNotification::New((INotification**)&n);
    n->SetWhen(mWhen);
    n->SetIcon(mSmallIcon);
    n->SetIconLevel(mSmallIconLevel);
    n->SetNumber(mNumber);

    n->SetColor(SanitizeColor());

    SetBuilderContentView(n, MakeContentView());

    n->SetContentIntent(mContentIntent);
    n->SetDeleteIntent(mDeleteIntent);
    n->SetFullScreenIntent(mFullScreenIntent);
    n->SetTickerText(mTickerText);
    n->SetLargeIcon(mLargeIcon);
    n->SetSound(mSound);
    n->SetAudioStreamType(mAudioStreamType);
    n->SetAudioAttributes(mAudioAttributes);
    n->SetVibrate(mVibrate);
    n->SetLedARGB(mLedArgb);
    n->SetLedOnMS(mLedOnMs);
    n->SetLedOffMS(mLedOffMs);
    n->SetDefaults(mDefaults);

    AutoPtr<IRemoteViews> rv = MakeTickerView();
    n->SetTickerView(rv);
    rv = MakeContentView();
    n->SetContentView(rv);
    rv = MakeBigContentView();
    n->SetBigContentView(rv);

    Int32 flag = mFlags;
    n->SetFlags(flag);

    SetBuilderBigContentView(n, MakeBigContentView());
    SetBuilderHeadsUpContentView(n, MakeHeadsUpContentView());

    if (mLedOnMs != 0 || mLedOffMs != 0) {
        flag |= INotification::FLAG_SHOW_LIGHTS;
    }
    if ((mDefaults & INotification::DEFAULT_LIGHTS) != 0) {
        flag |= INotification::FLAG_SHOW_LIGHTS;
    }
    n->SetFlags(flag);

    n->SetCategory(mCategory);
    n->SetGroupKey(mGroupKey);
    n->SetSortKey(mSortKey);

    AutoPtr<ArrayOf<String> > strs;
    if (mKindList.IsEmpty() == FALSE) {
        strs = ArrayOf<String>::Alloc(mKindList.GetSize());
        //mKindList.ToArray(n->mKind);
        List<String>::Iterator it = mKindList.Begin();
        for(Int32 i = 0; i < mKindList.GetSize(); i++, it++) {
            (*strs)[i] = *it;
        }
        n->SetKind(strs);
    } else {
        n->SetKind(NULL);
    }

    n->SetPriority(mPriority);

    AutoPtr<ArrayOf<INotificationAction*> > actions;
    if (mActions.IsEmpty() == FALSE) {
        actions = ArrayOf<INotificationAction*>::Alloc(mActions.GetSize());
        //mActions.ToArray(n->mActions);
        List<AutoPtr<INotificationAction> >::Iterator it = mActions.Begin();
        for(Int32 i = 0; i < mActions.GetSize(); i++, it++) {
            actions->Set(i, *it);
        }
        n->SetActions(actions);
    }
    else {
        n->SetActions(NULL);
    }

    n->SetVisibility(mVisibility);

    if (mPublicVersion != NULL) {
        AutoPtr<INotification> ntfc;
        CNotification::New((INotification**)&ntfc);
        n->SetPublicVersion(ntfc);
        mPublicVersion->CloneInto(n.publicVersion, TRUE);
    }
    // Note: If you're adding new fields, also update restoreFromNotitification().

    *notification = n;
    REFCOUNT_ADD(*notification);
    return NOERROR;
}

/**
 * Capture, in the provided bundle, semantic information used in the construction of
 * this Notification object.
 * @hide
 */
CARAPI PopulateExtras(
    /* [in] */ IBundle* extras)
{
    // Store original information used in the construction of this object
    extras.putInt(EXTRA_ORIGINATING_USERID, mOriginatingUserId);
    extras.putParcelable(EXTRA_REBUILD_CONTEXT_APPLICATION_INFO,
            mContext.getApplicationInfo());
    extras.putCharSequence(EXTRA_TITLE, mContentTitle);
    extras.putCharSequence(EXTRA_TEXT, mContentText);
    extras.putCharSequence(EXTRA_SUB_TEXT, mSubText);
    extras.putCharSequence(EXTRA_INFO_TEXT, mContentInfo);
    extras.putInt(EXTRA_SMALL_ICON, mSmallIcon);
    extras.putInt(EXTRA_PROGRESS, mProgress);
    extras.putInt(EXTRA_PROGRESS_MAX, mProgressMax);
    extras.putBoolean(EXTRA_PROGRESS_INDETERMINATE, mProgressIndeterminate);
    extras.putBoolean(EXTRA_SHOW_CHRONOMETER, mUseChronometer);
    extras.putBoolean(EXTRA_SHOW_WHEN, mShowWhen);
    if (mLargeIcon != null) {
        extras.putParcelable(EXTRA_LARGE_ICON, mLargeIcon);
    }
    if (!mPeople.isEmpty()) {
        extras.putStringArray(EXTRA_PEOPLE, mPeople.toArray(new String[mPeople.size()]));
    }
    // NOTE: If you're adding new extras also update restoreFromNotification().
}


/**
 * @hide
 */
static CARAPI StripForDelivery(
    /* [in] */ INotification* n)
{
    if (!STRIP_AND_REBUILD) {
        return;
    }

    String templateClass = n.extras.getString(EXTRA_TEMPLATE);
    // Only strip views for known Styles because we won't know how to
    // re-create them otherwise.
    boolean stripViews = TextUtils.isEmpty(templateClass) ||
            getNotificationStyleClass(templateClass) != null;

    boolean isStripped = false;

    if (n.largeIcon != null && n.extras.containsKey(EXTRA_LARGE_ICON)) {
        // TODO: Would like to check for equality here, but if the notification
        // has been cloned, we can't.
        n.largeIcon = null;
        n.extras.putBoolean(EXTRA_REBUILD_LARGE_ICON, true);
        isStripped = true;
    }
    // Get rid of unmodified BuilderRemoteViews.

    if (stripViews &&
            n.contentView instanceof BuilderRemoteViews &&
            n.extras.getInt(EXTRA_REBUILD_CONTENT_VIEW_ACTION_COUNT, -1) ==
                    n.contentView.getSequenceNumber()) {
        n.contentView = null;
        n.extras.putBoolean(EXTRA_REBUILD_CONTENT_VIEW, true);
        n.extras.remove(EXTRA_REBUILD_CONTENT_VIEW_ACTION_COUNT);
        isStripped = true;
    }
    if (stripViews &&
            n.bigContentView instanceof BuilderRemoteViews &&
            n.extras.getInt(EXTRA_REBUILD_BIG_CONTENT_VIEW_ACTION_COUNT, -1) ==
                    n.bigContentView.getSequenceNumber()) {
        n.bigContentView = null;
        n.extras.putBoolean(EXTRA_REBUILD_BIG_CONTENT_VIEW, true);
        n.extras.remove(EXTRA_REBUILD_BIG_CONTENT_VIEW_ACTION_COUNT);
        isStripped = true;
    }
    if (stripViews &&
            n.headsUpContentView instanceof BuilderRemoteViews &&
            n.extras.getInt(EXTRA_REBUILD_HEADS_UP_CONTENT_VIEW_ACTION_COUNT, -1) ==
                    n.headsUpContentView.getSequenceNumber()) {
        n.headsUpContentView = null;
        n.extras.putBoolean(EXTRA_REBUILD_HEADS_UP_CONTENT_VIEW, true);
        n.extras.remove(EXTRA_REBUILD_HEADS_UP_CONTENT_VIEW_ACTION_COUNT);
        isStripped = true;
    }

    if (isStripped) {
        n.extras.putBoolean(EXTRA_NEEDS_REBUILD, true);
    }
}

/**
 * @hide
 */
AutoPtr<INotification> Rebuild(
    /* [in] */ IContext* context,
    /* [in] */ INotification* n)
{
    Bundle extras = n.extras;
    if (!extras.getBoolean(EXTRA_NEEDS_REBUILD)) return n;
    extras.remove(EXTRA_NEEDS_REBUILD);

    // Re-create notification context so we can access app resources.
    ApplicationInfo applicationInfo = extras.getParcelable(
            EXTRA_REBUILD_CONTEXT_APPLICATION_INFO);
    Context builderContext;
    try {
        builderContext = context.createApplicationContext(applicationInfo,
                Context.CONTEXT_RESTRICTED);
    } catch (NameNotFoundException e) {
        Log.e(TAG, "ApplicationInfo " + applicationInfo + " not found");
        builderContext = context;  // try with our context
    }

    Builder b = new Builder(builderContext, n);
    return b.rebuild();
}

/**
 * Rebuilds the notification passed in to the rebuild-constructor
 * {@link #Builder(Context, Notification)}.
 *
 * <p>
 * Throws IllegalStateException when invoked on a Builder that isn't in rebuild mode.
 *
 * @hide
 */
CARAPI Rebuild(
    /* [out] */ INotification** result)
{
    if (mRebuildNotification == null) {
        throw new IllegalStateException("rebuild() only valid when in 'rebuild' mode.");
    }
    mHasThreeLines = hasThreeLines();

    Bundle extras = mRebuildNotification.extras;

    if (extras.getBoolean(EXTRA_REBUILD_LARGE_ICON)) {
        mRebuildNotification.largeIcon = extras.getParcelable(EXTRA_LARGE_ICON);
    }
    extras.remove(EXTRA_REBUILD_LARGE_ICON);

    if (extras.getBoolean(EXTRA_REBUILD_CONTENT_VIEW)) {
        setBuilderContentView(mRebuildNotification, makeContentView());
        if (mStyle != null) {
            mStyle.populateContentView(mRebuildNotification);
        }
    }
    extras.remove(EXTRA_REBUILD_CONTENT_VIEW);

    if (extras.getBoolean(EXTRA_REBUILD_BIG_CONTENT_VIEW)) {
        setBuilderBigContentView(mRebuildNotification, makeBigContentView());
        if (mStyle != null) {
            mStyle.populateBigContentView(mRebuildNotification);
        }
    }
    extras.remove(EXTRA_REBUILD_BIG_CONTENT_VIEW);

    if (extras.getBoolean(EXTRA_REBUILD_HEADS_UP_CONTENT_VIEW)) {
        setBuilderHeadsUpContentView(mRebuildNotification, makeHeadsUpContentView());
        if (mStyle != null) {
            mStyle.populateHeadsUpContentView(mRebuildNotification);
        }
    }
    extras.remove(EXTRA_REBUILD_HEADS_UP_CONTENT_VIEW);

    mHasThreeLines = false;
    return mRebuildNotification;
}

// private static Class<? extends Style> GetNotificationStyleClass(
//     /* [in] */ const String& templateClass) {
//     Class<? extends Style>[] classes = new Class[]{
//             BigTextStyle.class, BigPictureStyle.class, InboxStyle.class, MediaStyle.class};
//     for (Class<? extends Style> innerClass : classes) {
//         if (templateClass.equals(innerClass.getName())) {
//             return innerClass;
//         }
//     }
//     return null;
// }

CARAPI SetBuilderContentView(
    /* [in] */ INotification* n,
    /* [in] */ IRemoteViews* contentView)
{
    n.contentView = contentView;
    if (contentView instanceof BuilderRemoteViews) {
        mRebuildBundle.putInt(Builder.EXTRA_REBUILD_CONTENT_VIEW_ACTION_COUNT,
                contentView.getSequenceNumber());
    }
}

CARAPI SetBuilderBigContentView(
    /* [in] */ INotification* n,
    /* [in] */ IRemoteViews* bigContentView)
{
    n.bigContentView = bigContentView;
    if (bigContentView instanceof BuilderRemoteViews) {
        mRebuildBundle.putInt(Builder.EXTRA_REBUILD_BIG_CONTENT_VIEW_ACTION_COUNT,
                bigContentView.getSequenceNumber());
    }
}

CARAPI SetBuilderHeadsUpContentView(
    /* [in] */ INotification* n,
    /* [in] */ IRemoteViews* headsUpContentView)
{
    n.headsUpContentView = headsUpContentView;
    if (headsUpContentView instanceof BuilderRemoteViews) {
        mRebuildBundle.putInt(Builder.EXTRA_REBUILD_HEADS_UP_CONTENT_VIEW_ACTION_COUNT,
                headsUpContentView.getSequenceNumber());
    }
}

CARAPI RestoreFromNotification(
    /* [in] */ INotification* n)
{

    // Notification fields.
    mWhen = n.when;
    mSmallIcon = n.icon;
    mSmallIconLevel = n.iconLevel;
    mNumber = n.number;

    mColor = n.color;

    mContentView = n.contentView;
    mDeleteIntent = n.deleteIntent;
    mFullScreenIntent = n.fullScreenIntent;
    mTickerText = n.tickerText;
    mTickerView = n.tickerView;
    mLargeIcon = n.largeIcon;
    mSound = n.sound;
    mAudioStreamType = n.audioStreamType;
    mAudioAttributes = n.audioAttributes;

    mVibrate = n.vibrate;
    mLedArgb = n.ledARGB;
    mLedOnMs = n.ledOnMS;
    mLedOffMs = n.ledOffMS;
    mDefaults = n.defaults;
    mFlags = n.flags;

    mCategory = n.category;
    mGroupKey = n.mGroupKey;
    mSortKey = n.mSortKey;
    mPriority = n.priority;
    mActions.clear();
    if (n.actions != null) {
        Collections.addAll(mActions, n.actions);
    }
    mVisibility = n.visibility;

    mPublicVersion = n.publicVersion;

    // Extras.
    Bundle extras = n.extras;
    mOriginatingUserId = extras.getInt(EXTRA_ORIGINATING_USERID);
    mContentTitle = extras.getCharSequence(EXTRA_TITLE);
    mContentText = extras.getCharSequence(EXTRA_TEXT);
    mSubText = extras.getCharSequence(EXTRA_SUB_TEXT);
    mContentInfo = extras.getCharSequence(EXTRA_INFO_TEXT);
    mSmallIcon = extras.getInt(EXTRA_SMALL_ICON);
    mProgress = extras.getInt(EXTRA_PROGRESS);
    mProgressMax = extras.getInt(EXTRA_PROGRESS_MAX);
    mProgressIndeterminate = extras.getBoolean(EXTRA_PROGRESS_INDETERMINATE);
    mUseChronometer = extras.getBoolean(EXTRA_SHOW_CHRONOMETER);
    mShowWhen = extras.getBoolean(EXTRA_SHOW_WHEN);
    if (extras.containsKey(EXTRA_LARGE_ICON)) {
        mLargeIcon = extras.getParcelable(EXTRA_LARGE_ICON);
    }
    if (extras.containsKey(EXTRA_PEOPLE)) {
        mPeople.clear();
        Collections.addAll(mPeople, extras.getStringArray(EXTRA_PEOPLE));
    }
}


}
}
}
