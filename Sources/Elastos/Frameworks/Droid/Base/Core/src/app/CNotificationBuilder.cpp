#include "CNotificationBuilder.h"
#include "R.h"
#include "os/CBundle.h"
#include "os/SystemClock.h"
#include "app/CNotificationAction.h"
#include "app/CNotification.h"
#include "widget/CRemoteViews.h"
#include <elastos/Logger.h>

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Text::INumberFormatHelper;
using Elastos::Text::CNumberFormatHelper;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Widget::CRemoteViews;

namespace Elastos{
namespace Droid{
namespace App{

const Int32 CNotificationBuilder::MAX_ACTION_BUTTONS = 3;
const Boolean CNotificationBuilder::DBG = FALSE;

CNotificationBuilder::CNotificationBuilder() :
    mWhen(0),
    mSmallIcon(0),
    mSmallIconLevel(0),
    mNumber(0),
    mAudioStreamType(0),
    mLedArgb(0),
    mLedOnMs(0),
    mLedOffMs(0),
    mDefaults(0),
    mFlags(0),
    mProgressMax(0),
    mProgress(0),
    mProgressIndeterminate(FALSE),
    mPriority(0),
    mUseChronometer(FALSE),
    mShowWhen(TRUE)
{
}

CNotificationBuilder::~CNotificationBuilder()
{
}

ECode CNotificationBuilder::constructor(
    /* [in] */ IContext *context)
{
    mContext = context;
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mWhen);
    mAudioStreamType = INotification::STREAM_DEFAULT;
    mPriority = INotification::PRIORITY_DEFAULT;
    return NOERROR;
}

/**
 * Add a timestamp pertaining to the notification (usually the time the event occurred).
 * It will be shown in the notification content view by default; use
 * {@link Builder#setShowWhen(boolean) setShowWhen} to control this.
 *
 * @see Notification#when
 */
ECode CNotificationBuilder::SetWhen(
    /* [in] */ Int64 when)
{
    mWhen = when;
    return NOERROR;
}

/**
 * Control whether the timestamp set with {@link Builder#setWhen(long) setWhen} is shown
 * in the content view.
 */
ECode CNotificationBuilder::SetShowWhen(
    /* [in] */ Boolean show)
{
    mShowWhen = show;
    return NOERROR;
}

/**
 * Show the {@link Notification#when} field as a stopwatch.
 *
 * Instead of presenting <code>when</code> as a timestamp, the notification will show an
 * automatically updating display of the minutes and seconds since <code>when</code>.
 *
 * Useful when showing an elapsed time (like an ongoing phone call).
 *
 * @see android.widget.Chronometer
 * @see Notification#when
 */
ECode CNotificationBuilder::SetUsesChronometer(
    /* [in] */ Boolean b)
{
    mUseChronometer = b;
    return NOERROR;
}

/**
 * Set the small icon resource, which will be used to represent the notification in the
 * status bar.
 *

 * The platform template for the expanded view will draw this icon in the left, unless a
 * {@link #setLargeIcon(Bitmap) large icon} has also been specified, in which case the small
 * icon will be moved to the right-hand side.
 *

 * @param icon
 *            A resource ID in the application's package of the drawable to use.
 * @see Notification#icon
 */
ECode CNotificationBuilder::SetSmallIcon(
    /* [in] */ Int32 icon)
{
    mSmallIcon = icon;
    return NOERROR;
}

/**
 * A variant of {@link #setSmallIcon(int) setSmallIcon(int)} that takes an additional
 * level parameter for when the icon is a {@link android.graphics.drawable.LevelListDrawable
 * LevelListDrawable}.
 *
 * @param icon A resource ID in the application's package of the drawable to use.
 * @param level The level to use for the icon.
 *
 * @see Notification#icon
 * @see Notification#iconLevel
 */
ECode CNotificationBuilder::SetSmallIconEx(
    /* [in] */ Int32 icon,
    /* [in] */ Int32 level)
{
    mSmallIcon = icon;
    mSmallIconLevel = level;
    return NOERROR;
}

/**
 * Set the first line of text in the platform notification template.
 */
ECode CNotificationBuilder::SetContentTitle(
    /* [in] */ ICharSequence *title)
{
    mContentTitle = title;
    return NOERROR;
}

/**
 * Set the second line of text in the platform notification template.
 */
ECode CNotificationBuilder::SetContentText(
    /* [in] */ ICharSequence *text)
{
    mContentText = text;
    return NOERROR;
}

/**
 * Set the third line of text in the platform notification template.
 * Don't use if you're also using {@link #setProgress(int, int, boolean)}; they occupy the same location in the standard template.
 */
ECode CNotificationBuilder::SetSubText(
    /* [in] */ ICharSequence *text)
{
    mSubText = text;
    return NOERROR;
}

/**
 * Set the large number at the right-hand side of the notification.  This is
 * equivalent to setContentInfo, although it might show the number in a different
 * font size for readability.
 */
ECode CNotificationBuilder::SetNumber(
    /* [in] */ Int32 number)
{
    mNumber = number;
    return NOERROR;
}

/**
 * A small piece of additional information pertaining to this notification.
 *
 * The platform template will draw this on the last line of the notification, at the far
 * right (to the right of a smallIcon if it has been placed there).
 */
ECode CNotificationBuilder::SetContentInfo(
    /* [in] */ ICharSequence *info)
{
    mContentInfo = info;
    return NOERROR;
}

/**
 * Set the progress this notification represents.
 *
 * The platform template will represent this using a {@link ProgressBar}.
 */
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

/**
 * Supply a custom RemoteViews to use instead of the platform template.
 *
 * @see Notification#contentView
 */
ECode CNotificationBuilder::SetContent(
    /* [in] */ IRemoteViews *views)
{
    mContentView = views;
    return NOERROR;
}

/**
 * Supply a {@link PendingIntent} to be sent when the notification is clicked.
 *
 * As of {@link android.os.Build.VERSION_CODES#HONEYCOMB}, if this field is unset and you
 * have specified a custom RemoteViews with {@link #setContent(RemoteViews)}, you can use
 * {@link RemoteViews#setOnClickPendingIntent RemoteViews.setOnClickPendingIntent(int,PendingIntent)}
 * to assign PendingIntents to individual views in that custom layout (i.e., to create
 * clickable buttons inside the notification view).
 *
 * @see Notification#contentIntent Notification.contentIntent
 */
ECode CNotificationBuilder::SetContentIntent(
    /* [in] */ IPendingIntent *intent)
{
    mContentIntent = intent;
    return NOERROR;
}

/**
 * Supply a {@link PendingIntent} to send when the notification is cleared explicitly by the user.
 *
 * @see Notification#deleteIntent
 */
ECode CNotificationBuilder::SetDeleteIntent(
    /* [in] */ IPendingIntent *intent)
{
    mDeleteIntent = intent;
    return NOERROR;
}

/**
 * An intent to launch instead of posting the notification to the status bar.
 * Only for use with extremely high-priority notifications demanding the user's
 * <strong>immediate</strong> attention, such as an incoming phone call or
 * alarm clock that the user has explicitly set to a particular time.
 * If this facility is used for something else, please give the user an option
 * to turn it off and use a normal notification, as this can be extremely
 * disruptive.
 *
 * @param intent The pending intent to launch.
 * @param highPriority Passing true will cause this notification to be sent
 *          even if other notifications are suppressed.
 *
 * @see Notification#fullScreenIntent
 */
ECode CNotificationBuilder::SetFullScreenIntent(
    /* [in] */ IPendingIntent *intent,
    /* [in] */ Boolean highPriority)
{
    mFullScreenIntent = intent;
    SetFlag(INotification::FLAG_HIGH_PRIORITY, highPriority);
    return NOERROR;
}

/**
 * Set the "ticker" text which is displayed in the status bar when the notification first
 * arrives.
 *
 * @see Notification#tickerText
 */
ECode CNotificationBuilder::SetTicker(
    /* [in] */ ICharSequence *tickerText)
{
    mTickerText = tickerText;
    return NOERROR;
}

/**
 * Set the text that is displayed in the status bar when the notification first
 * arrives, and also a RemoteViews object that may be displayed instead on some
 * devices.
 *
 * @see Notification#tickerText
 * @see Notification#tickerView
 */
ECode CNotificationBuilder::SetTickerEx(
    /* [in] */ ICharSequence *tickerText,
    /* [in] */ IRemoteViews *views)
{
    mTickerText = tickerText;
    mTickerView = views;
    return NOERROR;
}

/**
 * Add a large icon to the notification (and the ticker on some devices).
 *
 * In the platform template, this image will be shown on the left of the notification view
 * in place of the {@link #setSmallIcon(int) small icon} (which will move to the right side).
 *
 * @see Notification#largeIcon
 */
ECode CNotificationBuilder::SetLargeIcon(
    /* [in] */ IBitmap *icon)
{
    mLargeIcon = icon;
    return NOERROR;
}

/**
 * Set the sound to play.
 *
 * It will be played on the {@link #STREAM_DEFAULT default stream} for notifications.
 *
 * @see Notification#sound
 */
ECode CNotificationBuilder::SetSound(
    /* [in] */ IUri *sound)
{
    mSound = sound;
    mAudioStreamType = INotification::STREAM_DEFAULT;
    return NOERROR;
}

/**
 * Set the sound to play, along with a specific stream on which to play it.
 *
 * See {@link android.media.AudioManager} for the <code>STREAM_</code> constants.
 *
 * @see Notification#sound
 */
ECode CNotificationBuilder::SetSoundEx(
    /* [in] */ IUri *sound,
    /* [in] */ Int32 streamType)
{
    mSound = sound;
    mAudioStreamType = streamType;
    return NOERROR;
}

/**
 * Set the vibration pattern to use.
 *

 * See {@link android.os.Vibrator#vibrate(long[], int)} for a discussion of the
 * <code>pattern</code> parameter.
 *

 * @see Notification#vibrate
 */
ECode CNotificationBuilder::SetVibrate(
    /* [in] */ ArrayOf<Int64> *pattern)
{
    mVibrate = pattern;
    return NOERROR;
}

/**
 * Set the desired color for the indicator LED on the device, as well as the
 * blink duty cycle (specified in milliseconds).
 *

 * Not all devices will honor all (or even any) of these values.
 *

 * @see Notification#ledARGB
 * @see Notification#ledOnMS
 * @see Notification#ledOffMS
 */
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

/**
 * Set whether this is an "ongoing" notification.
 *

 * Ongoing notifications cannot be dismissed by the user, so your application or service
 * must take care of canceling them.
 *

 * They are typically used to indicate a background task that the user is actively engaged
 * with (e.g., playing music) or is pending in some way and therefore occupying the device
 * (e.g., a file download, sync operation, active network connection).
 *

 * @see Notification#FLAG_ONGOING_EVENT
 * @see Service#setForeground(boolean)
 */
ECode CNotificationBuilder::SetOngoing(
    /* [in] */ Boolean ongoing)
{
    SetFlag(INotification::FLAG_ONGOING_EVENT, ongoing);
    return NOERROR;
}

/**
 * Set this flag if you would only like the sound, vibrate
 * and ticker to be played if the notification is not already showing.
 *
 * @see Notification#FLAG_ONLY_ALERT_ONCE
 */
ECode CNotificationBuilder::SetOnlyAlertOnce(
    /* [in] */ Boolean onlyAlertOnce)
{
    SetFlag(INotification::FLAG_ONLY_ALERT_ONCE, onlyAlertOnce);
    return NOERROR;
}

/**
 * Make this notification automatically dismissed when the user touches it. The
 * PendingIntent set with {@link #setDeleteIntent} will be sent when this happens.
 *
 * @see Notification#FLAG_AUTO_CANCEL
 */
ECode CNotificationBuilder::SetAutoCancel(
    /* [in] */ Boolean autoCancel)
{
    SetFlag(INotification::FLAG_AUTO_CANCEL, autoCancel);
    return NOERROR;
}

/**
 * Set which notification properties will be inherited from system defaults.
 * <p>
 * The value should be one or more of the following fields combined with
 * bitwise-or:
 * {@link #DEFAULT_SOUND}, {@link #DEFAULT_VIBRATE}, {@link #DEFAULT_LIGHTS}.
 * <p>
 * For all default values, use {@link #DEFAULT_ALL}.
 */
ECode CNotificationBuilder::SetDefaults(
    /* [in] */ Int32 defaults)
{
    mDefaults = defaults;
    return NOERROR;
}

/**
 * Set the priority of this notification.
 *
 * @see Notification#priority
 */
ECode CNotificationBuilder::SetPriority(
    /* [in] */ Int32 pri)
{
    mPriority = pri;
    return NOERROR;
}

/**
 * @hide
 *
 * Add a kind (category) to this notification. Optional.
 *
 * @see Notification#kind
 */
ECode CNotificationBuilder::AddKind(
    /* [in] */ const String &k)
{
    mKindList.PushBack(k);
    return NOERROR;
}

/**
 * Add metadata to this notification.
 *
 * A reference to the Bundle is held for the lifetime of this Builder, and the Bundle's
 * current contents are copied into the Notification each time {@link #build()} is
 * called.
 *
 * @see Notification#extras
 * @hide
 */
ECode CNotificationBuilder::SetExtras(
    /* [in] */ IBundle *bag)
{
    mExtras = bag;
    return NOERROR;
}

/**
 * Add an action to this notification. Actions are typically displayed by
 * the system as a button adjacent to the notification content.
 *
 * @param icon Resource ID of a drawable that represents the action.
 * @param title Text describing the action.
 * @param intent PendingIntent to be fired when the action is invoked.
 */
ECode CNotificationBuilder::AddAction(
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence *title,
    /* [in] */ IPendingIntent *intent)
{
    AutoPtr<INotificationAction> action;
    CNotificationAction::New(icon, title, intent, (INotificationAction**)&action);
    mActions.PushBack(action);
    return NOERROR;
}

/**
 * Add a rich notification style to be applied at build time.
 *
 * @param style Object responsible for modifying the notification style.
 */
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

/**
 * @deprecated Use {@link #build()} instead.
 */
//@Deprecated
ECode CNotificationBuilder::GetNotification(
    /* [out] */ INotification **notification)
{
    VALIDATE_NOT_NULL(notification);
    return Build(notification);
}

/**
 * Combine all of the options that have been set and return a new {@link Notification}
 * object.
 */
ECode CNotificationBuilder::Build(
    /* [out] */ INotification **notification)
{
    VALIDATE_NOT_NULL(notification);

    if (mStyle != NULL) {
        return mStyle->Build(notification);
    }
    else {
        return BuildUnstyled(notification);
    }
}

ECode CNotificationBuilder::GetSubText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mSubText;
    INTERFACE_ADDREF(*text);
    return NOERROR;
}

ECode CNotificationBuilder::GetContentText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mContentText;
    INTERFACE_ADDREF(*text);
    return NOERROR;
}

ECode CNotificationBuilder::ApplyStandardTemplateWithActions(
    /* [in] */ Int32 layoutId,
    /* [out] */ IRemoteViews** remoteViews)
{
    VALIDATE_NOT_NULL(remoteViews);
    *remoteViews = NULL;

    AutoPtr<IRemoteViews> rv;
    rv = ApplyStandardTemplate(layoutId, FALSE);

    Int32 N = mActions.GetSize();
    if (N > 0) {
        String info;
        if (mContentText != NULL) {
            mContentText->ToString(&info);
            if (DBG) Logger::D("Notification", "has actions: %s", info.string());
        }

        rv->SetViewVisibility(R::id::actions, IView::VISIBLE);
        rv->SetViewVisibility(R::id::action_divider, IView::VISIBLE);
        if (N > MAX_ACTION_BUTTONS) N = MAX_ACTION_BUTTONS;
        rv->RemoveAllViews(R::id::actions);
        for (Int32 i = 0; i < N; i++) {
            AutoPtr<IRemoteViews> button = GenerateActionButton(mActions[i]);
            rv->AddView(R::id::actions, button);

            if (DBG) {
                AutoPtr<ICharSequence> seq;
                mActions[i]->GetTitle((ICharSequence**)&seq);
                if (seq) {
                    seq->ToString(&info);
                    Logger::D("Notification", "adding action %d, title: %s.", i, info.string());
                }
                else {
                    Logger::D("Notification", "adding action %d, title: %s", i, "No title.");
                }
            }
        }
    }

    *remoteViews = rv;
    INTERFACE_ADDREF(*remoteViews);
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
    /* [in] */ Boolean fitIn1U)
{
    AutoPtr<IRemoteViews> contentView;
    String name;
    mContext->GetPackageName(&name);
    CRemoteViews::New(name, resId, (IRemoteViews**)&contentView);
    Boolean showLine3 = FALSE;
    Boolean showLine2 = FALSE;
    Int32 smallIconImageViewId = R::id::icon;

    if (mLargeIcon != NULL) {
        contentView->SetImageViewBitmap(R::id::icon, mLargeIcon);
        smallIconImageViewId = R::id::right_icon;
    }

    if (mPriority < INotification::PRIORITY_LOW) {
        String funcName("SetBackgroundResource");
        contentView->SetInt32(R::id::icon,
                funcName, R::drawable::notification_template_icon_low_bg);
        contentView->SetInt32(R::id::status_bar_latest_event_content,
                funcName, R::drawable::notification_bg_low);
    }

    if (mSmallIcon != 0) {
        contentView->SetImageViewResource(smallIconImageViewId, mSmallIcon);
        contentView->SetViewVisibility(smallIconImageViewId, IView::VISIBLE);
    }
    else {
        contentView->SetViewVisibility(smallIconImageViewId, IView::GONE);
    }

    if (mContentTitle != NULL) {
        contentView->SetTextViewText(R::id::title, mContentTitle);
    }

    if (mContentText != NULL) {
        contentView->SetTextViewText(R::id::text, mContentText);
        showLine3 = TRUE;
    }

    if (mContentInfo != NULL) {
        contentView->SetTextViewText(R::id::info, mContentInfo);
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
            CStringWrapper::New(str, (ICharSequence**)&cs);
            contentView->SetTextViewText(R::id::info, cs);
        }
        else {
            AutoPtr<INumberFormatHelper> helper;
            CNumberFormatHelper::AcquireSingleton((INumberFormatHelper**)&helper);
            AutoPtr<INumberFormat> f;
            helper->GetIntegerInstance((INumberFormat**)&f);

			AutoPtr<ICharSequence> cs;
            String str;
            f->FormatInt64(mNumber, &str);
            CStringWrapper::New(str, (ICharSequence**)&cs);
            contentView->SetTextViewText(R::id::info, cs);
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
        if (mProgressMax != 0 || mProgressIndeterminate) {
            contentView->SetProgressBar(
                    R::id::progress, mProgressMax, mProgress, mProgressIndeterminate);
            contentView->SetViewVisibility(R::id::progress, IView::VISIBLE);
            showLine2 = TRUE;
        }
        else {
            contentView->SetViewVisibility(R::id::progress, IView::GONE);
        }
    }

    if (showLine2) {
        if (fitIn1U) {
            // need to shrink all the type to make sure everything fits
            AutoPtr<IResources> res;
            mContext->GetResources((IResources**)&res);
            Float subTextSize;
            res->GetDimensionPixelSize(
                    R::dimen::notification_subtext_size, (Int32*)&subTextSize);
            contentView->SetTextViewTextSize(R::id::text, ITypedValue::COMPLEX_UNIT_PX, subTextSize);
        }
        // vertical centering
        contentView->SetViewPadding(R::id::line1, 0, 0, 0, 0);
    }

    if (mWhen != 0 && mShowWhen) {
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
    else {
        contentView->SetViewVisibility(R::id::time, IView::GONE);
    }

    contentView->SetViewVisibility(R::id::line3, showLine3 ? IView::VISIBLE : IView::GONE);
    contentView->SetViewVisibility(R::id::overflow_divider, showLine3 ? IView::VISIBLE : IView::GONE);
    return contentView;
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeContentView()
{
    if (mContentView != NULL) {
        return mContentView;
    }
    else {
        return ApplyStandardTemplate(R::layout::notification_template_base, TRUE); // no more special large_icon flavor
    }
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeTickerView()
{
    if (mTickerView != NULL) {
        return mTickerView;
    }
    else {
        if (mContentView == NULL) {
            return ApplyStandardTemplate(mLargeIcon == NULL
                    ? R::layout::status_bar_latest_event_ticker
                    : R::layout::status_bar_latest_event_ticker_large_icon, TRUE);
        }
        else {
            return NULL;
        }
    }
}

AutoPtr<IRemoteViews> CNotificationBuilder::MakeBigContentView()
{
    if (mActions.IsEmpty()) return NULL;
    AutoPtr<IRemoteViews> rv;
    ApplyStandardTemplateWithActions(R::layout::notification_template_big_base, (IRemoteViews**)&rv);
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
        tombstone ? R::layout::notification_action_tombstone : R::layout::notification_action,
        (IRemoteViews**)&button);

    Int32 icon;
    action->GetIcon(&icon);
    AutoPtr<ICharSequence> title;
    action->GetTitle((ICharSequence**)&title);
    button->SetTextViewCompoundDrawables(R::id::action0, icon, 0, 0, 0);
    button->SetTextViewText(R::id::action0, title);
    if (!tombstone) {
        button->SetOnClickPendingIntent(R::id::action0, actionIntent);
    }
    button->SetContentDescription(R::id::action0, title);
    return button;
}

/**
 * Apply the unstyled operations and return a new {@link Notification} object.
 */
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
    n->SetContentIntent(mContentIntent);
    n->SetDeleteIntent(mDeleteIntent);
    n->SetFullScreenIntent(mFullScreenIntent);
    n->SetTickerText(mTickerText);
    n->SetLargeIcon(mLargeIcon);
    n->SetSound(mSound);
    n->SetAudioStreamType(mAudioStreamType);
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
    if (mLedOnMs != 0 && mLedOffMs != 0) {
        flag |= INotification::FLAG_SHOW_LIGHTS;
    }
    if ((mDefaults & INotification::DEFAULT_LIGHTS) != 0) {
        flag |= INotification::FLAG_SHOW_LIGHTS;
    }
    n->SetFlags(flag);


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
    AutoPtr<IBundle> bundle;
    if (mExtras != NULL) {
        CBundle::New(mExtras, (IBundle**)&bundle);
    }
    else {
        bundle = NULL;
    }
    n->SetExtras(bundle);

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

    *notification = n;
    INTERFACE_ADDREF(*notification);
    return NOERROR;
}

}
}
}
