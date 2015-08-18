#ifndef __ELASTOS_DROID_APP_CNOTIFICATIONBUILDER_H__
#define __ELASTOS_DROID_APP_CNOTIFICATIONBUILDER_H__

#include "_Elastos_Droid_App_CNotificationBuilder.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::ISystem;
using Elastos::Core::CSystem;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Text::INumberFormat;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::View::IView;
using Elastos::Droid::Utility::ITypedValue;
using Elastos::Droid::Widget::IRemoteViews;
using Elastos::Droid::Graphics::IBitmap;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::Res::IResources;

namespace Elastos{
namespace Droid{
namespace App{

CarClass(CNotificationBuilder)
{
public:
    CNotificationBuilder();

    ~CNotificationBuilder();

    CARAPI constructor(
        /* [in] */ IContext * context);

    /**
     * Add a timestamp pertaining to the notification (usually the time the event occurred).
     * It will be shown in the notification content view by default; use
     * {@link Builder#setShowWhen(boolean) setShowWhen} to control this.
     *
     * @see Notification#when
     */
    CARAPI SetWhen(
        /* [in] */ Int64 when);

    /**
     * Control whether the timestamp set with {@link Builder#setWhen(long) setWhen} is shown
     * in the content view.
     */
    CARAPI SetShowWhen(
        /* [in] */ Boolean show);

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
    CARAPI SetUsesChronometer(
        /* [in] */ Boolean b);

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
    CARAPI SetSmallIcon(
        /* [in] */ Int32 icon);

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
    CARAPI SetSmallIcon(
        /* [in] */ Int32 icon,
        /* [in] */ Int32 level);

    /**
     * Set the first line of text in the platform notification template.
     */
    CARAPI SetContentTitle(
        /* [in] */ ICharSequence * title);

    /**
     * Set the second line of text in the platform notification template.
     */
    CARAPI SetContentText(
        /* [in] */ ICharSequence * text);

    /**
     * Set the third line of text in the platform notification template.
     * Don't use if you're also using {@link #setProgress(int, int, boolean)}; they occupy the same location in the standard template.
     */
    CARAPI SetSubText(
        /* [in] */ ICharSequence * text);

    /**
     * Set the large number at the right-hand side of the notification.  This is
     * equivalent to setContentInfo, although it might show the number in a different
     * font size for readability.
     */
    CARAPI SetNumber(
        /* [in] */ Int32 number);

    /**
     * A small piece of additional information pertaining to this notification.
     *
     * The platform template will draw this on the last line of the notification, at the far
     * right (to the right of a smallIcon if it has been placed there).
     */
    CARAPI SetContentInfo(
        /* [in] */ ICharSequence * info);

    /**
     * Set the progress this notification represents.
     *
     * The platform template will represent this using a {@link ProgressBar}.
     */
    CARAPI SetProgress(
        /* [in] */ Int32 max,
        /* [in] */ Int32 progress,
        /* [in] */ Boolean indeterminate);

    /**
     * Supply a custom RemoteViews to use instead of the platform template.
     *
     * @see Notification#contentView
     */
    CARAPI SetContent(
        /* [in] */ IRemoteViews * views);

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
    CARAPI SetContentIntent(
        /* [in] */ IPendingIntent * intent);

    /**
     * Supply a {@link PendingIntent} to send when the notification is cleared explicitly by the user.
     *
     * @see Notification#deleteIntent
     */
    CARAPI SetDeleteIntent(
        /* [in] */ IPendingIntent * intent);

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
    CARAPI SetFullScreenIntent(
        /* [in] */ IPendingIntent * intent,
        /* [in] */ Boolean highPriority);

    /**
     * Set the "ticker" text which is displayed in the status bar when the notification first
     * arrives.
     *
     * @see Notification#tickerText
     */
    CARAPI SetTicker(
        /* [in] */ ICharSequence * tickerText);

    /**
     * Set the text that is displayed in the status bar when the notification first
     * arrives, and also a RemoteViews object that may be displayed instead on some
     * devices.
     *
     * @see Notification#tickerText
     * @see Notification#tickerView
     */
    CARAPI SetTicker(
        /* [in] */ ICharSequence * tickerText,
        /* [in] */ IRemoteViews * views);

    /**
     * Add a large icon to the notification (and the ticker on some devices).
     *
     * In the platform template, this image will be shown on the left of the notification view
     * in place of the {@link #setSmallIcon(int) small icon} (which will move to the right side).
     *
     * @see Notification#largeIcon
     */
    CARAPI SetLargeIcon(
        /* [in] */ IBitmap * icon);

    /**
     * Set the sound to play.
     *
     * It will be played on the {@link #STREAM_DEFAULT default stream} for notifications.
     *
     * @see Notification#sound
     */
    CARAPI SetSound(
        /* [in] */ IUri * sound);

    /**
     * Set the sound to play, along with a specific stream on which to play it.
     *
     * See {@link android.media.AudioManager} for the <code>STREAM_</code> constants.
     *
     * @see Notification#sound
     */
    CARAPI SetSound(
        /* [in] */ IUri * sound,
        /* [in] */ Int32 streamType);

    /**
     * Set the vibration pattern to use.
     *

     * See {@link android.os.Vibrator#vibrate(long[], int)} for a discussion of the
     * <code>pattern</code> parameter.
     *

     * @see Notification#vibrate
     */
    CARAPI SetVibrate(
        /* [in] */ ArrayOf<Int64> *pattern);

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
    CARAPI SetLights(
        /* [in] */ Int32 argb,
        /* [in] */ Int32 onMs,
        /* [in] */ Int32 offMs);

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
    CARAPI SetOngoing(
        /* [in] */ Boolean ongoing);

    /**
     * Set this flag if you would only like the sound, vibrate
     * and ticker to be played if the notification is not already showing.
     *
     * @see Notification#FLAG_ONLY_ALERT_ONCE
     */
    CARAPI SetOnlyAlertOnce(
        /* [in] */ Boolean onlyAlertOnce);

    /**
     * Make this notification automatically dismissed when the user touches it. The
     * PendingIntent set with {@link #setDeleteIntent} will be sent when this happens.
     *
     * @see Notification#FLAG_AUTO_CANCEL
     */
    CARAPI SetAutoCancel(
        /* [in] */ Boolean autoCancel);

    /**
     * Set which notification properties will be inherited from system defaults.
     * <p>
     * The value should be one or more of the following fields combined with
     * bitwise-or:
     * {@link #DEFAULT_SOUND}, {@link #DEFAULT_VIBRATE}, {@link #DEFAULT_LIGHTS}.
     * <p>
     * For all default values, use {@link #DEFAULT_ALL}.
     */
    CARAPI SetDefaults(
        /* [in] */ Int32 defaults);

    /**
     * Set the priority of this notification.
     *
     * @see Notification#priority
     */
    CARAPI SetPriority(
        /* [in] */ Int32 pri);

    /**
     * @hide
     *
     * Add a kind (category) to this notification. Optional.
     *
     * @see Notification#kind
     */
    CARAPI AddKind(
        /* [in] */ const String & k);

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
    CARAPI SetExtras(
        /* [in] */ IBundle * bag);

    /**
     * Add an action to this notification. Actions are typically displayed by
     * the system as a button adjacent to the notification content.
     *
     * @param icon Resource ID of a drawable that represents the action.
     * @param title Text describing the action.
     * @param intent PendingIntent to be fired when the action is invoked.
     */
    CARAPI AddAction(
        /* [in] */ Int32 icon,
        /* [in] */ ICharSequence * title,
        /* [in] */ IPendingIntent * intent);

    /**
     * Add a rich notification style to be applied at build time.
     *
     * @param style Object responsible for modifying the notification style.
     */
    CARAPI SetStyle(
        /* [in] */ INotificationStyle * style);

    /**
     * @deprecated Use {@link #build()} instead.
     */
    //@Deprecated
    CARAPI GetNotification(
        /* [out] */ INotification **notification);

    /**
     * Combine all of the options that have been set and return a new {@link Notification}
     * object.
     */
    CARAPI Build(
        /* [out] */ INotification **notification);

    /**
     * Apply the unstyled operations and return a new {@link Notification} object.
     */
    CARAPI BuildUnstyled(
        /* [out] */ INotification **notification);

    CARAPI GetSubText(
        /* [out] */ ICharSequence** text);

    CARAPI GetContentText(
        /* [out] */ ICharSequence** text);

    CARAPI ApplyStandardTemplateWithActions(
        /* [in] */ Int32 layoutId,
        /* [out] */ IRemoteViews** remoteViews);

public:
    const static Int32 MAX_ACTION_BUTTONS;
    const static Boolean DBG;

    AutoPtr<IContext> mContext;

    Int64 mWhen;
    Int32 mSmallIcon;
    Int32 mSmallIconLevel;
    Int32 mNumber;
    AutoPtr<ICharSequence> mContentTitle;
    AutoPtr<ICharSequence> mContentText;
    AutoPtr<ICharSequence> mContentInfo;
    AutoPtr<ICharSequence> mSubText;
    AutoPtr<IPendingIntent> mContentIntent;
    AutoPtr<IRemoteViews> mContentView;
    AutoPtr<IPendingIntent> mDeleteIntent;
    AutoPtr<IPendingIntent> mFullScreenIntent;
    AutoPtr<ICharSequence> mTickerText;
    AutoPtr<IRemoteViews> mTickerView;
    AutoPtr<IBitmap> mLargeIcon;
    AutoPtr<IUri> mSound;
    Int32 mAudioStreamType;
    AutoPtr<ArrayOf<Int64> > mVibrate;
    Int32 mLedArgb;
    Int32 mLedOnMs;
    Int32 mLedOffMs;
    Int32 mDefaults;
    Int32 mFlags;
    Int32 mProgressMax;
    Int32 mProgress;
    Boolean mProgressIndeterminate;
    List<String> mKindList;
    AutoPtr<IBundle> mExtras;
    Int32 mPriority;
    List<AutoPtr<INotificationAction> > mActions;
    Boolean mUseChronometer;
    AutoPtr<INotificationStyle> mStyle;
    Boolean mShowWhen;

private:
    CARAPI_(void) SetFlag(
        /* [in] */ Int32 mask,
        /* [in] */ Boolean value);

    CARAPI_(AutoPtr<IRemoteViews>) ApplyStandardTemplate(
        /* [in] */ Int32 resId,
        /* [in] */ Boolean fitIn1U);

    CARAPI_(AutoPtr<IRemoteViews>) MakeContentView();

    CARAPI_(AutoPtr<IRemoteViews>) MakeTickerView();

    CARAPI_(AutoPtr<IRemoteViews>) MakeBigContentView();

    CARAPI_(AutoPtr<IRemoteViews>) GenerateActionButton(
        /* [in] */ INotificationAction * action);
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CNOTIFICATIONBUILDER_H__
