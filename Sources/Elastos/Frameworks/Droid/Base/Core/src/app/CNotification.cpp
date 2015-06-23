
#include "ext/frameworkext.h"
#include <elastos/core/StringUtils.h>
#include <elastos/core/StringBuilder.h>
#include "os/UserHandle.h"
#include "os/CBundle.h"
#include "text/TextUtils.h"
#include "graphics/CBitmap.h"
#include "app/CNotification.h"
#include "app/CPendingIntentHelper.h"
#include "app/CPendingIntent.h"
#include "app/CNotificationAction.h"
#include "widget/CRemoteViews.h"
#include "R.h"

using Elastos::Core::StringUtils;
using Elastos::Core::StringBuilder;
using Elastos::Text::INumberFormatHelper;
using Elastos::Text::CNumberFormatHelper;
using Elastos::Droid::R;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Os::UserHandle;
using Elastos::Droid::Os::CBundle;
using Elastos::Droid::Widget::CRemoteViews;

namespace Elastos{
namespace Droid{
namespace App{

CNotification::CNotification()
    : mWhen(0)
    , mIcon(0)
    , mIconLevel(0)
    , mNumber(0)
    , mAudioStreamType(INotification::STREAM_DEFAULT)
    , mLedARGB(0)
    , mLedOnMS(0)
    , mLedOffMS(0)
    , mDefaults(0)
    , mFlags(0)
    , mPriority(0)
{}

CNotification::~CNotification()
{}

ECode CNotification::constructor()
{
    AutoPtr<ISystem> system;
    Elastos::Core::CSystem::AcquireSingleton((ISystem**)&system);
    system->GetCurrentTimeMillis(&mWhen);
    mPriority = INotification::PRIORITY_DEFAULT;
    return NOERROR;
}

ECode CNotification::constructor(
    /* [in] */ IContext* context,
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence* tickerText,
    /* [in] */ Int64 when,
    /* [in] */ ICharSequence* contentTitle,
    /* [in] */ ICharSequence* contentText,
    /* [in] */ IIntent* contentIntent)
{
    mWhen = when;
    mIcon = icon;
    mTickerText = tickerText;
    AutoPtr<IPendingIntentHelper> helper;
    CPendingIntentHelper::AcquireSingleton((IPendingIntentHelper**)&helper);
    AutoPtr<IPendingIntent> pendingIntent;
    helper->GetActivity(context, 0, contentIntent, 0, (IPendingIntent**)&pendingIntent);
    return SetLatestEventInfo(context, contentTitle, contentText, pendingIntent);
}

ECode CNotification::constructor(
    /* [in] */ Int32 icon,
    /* [in] */ ICharSequence* tickerText,
    /* [in] */ Int64 when)
{
    mIcon = icon;
    mTickerText = tickerText;
    mWhen = when;

    return NOERROR;
}

ECode CNotification::GetWhen(
    /* [out] */ Int64* when)
{
    VALIDATE_NOT_NULL(when);
    *when = mWhen;
    return NOERROR;
}

ECode CNotification::SetWhen(
    /* [in] */ Int64 when)
{
    mWhen = when;
    return NOERROR;
}

ECode CNotification::GetIcon(
    /* [out] */ Int32* icon)
{
    VALIDATE_NOT_NULL(icon);
    *icon = mIcon;
    return NOERROR;
}

ECode CNotification::SetIcon(
    /* [in] */ Int32 icon)
{
    mIcon = icon;
    return NOERROR;
}

ECode CNotification::GetIconLevel(
    /* [out] */ Int32 * iconLevel)
{
    VALIDATE_NOT_NULL(iconLevel);
    *iconLevel = mIconLevel;
    return NOERROR;
}

ECode CNotification::SetIconLevel(
    /* [in] */ Int32 iconLevel)
{
    mIconLevel = iconLevel;
    return NOERROR;
}

ECode CNotification::GetNumber(
    /* [out] */ Int32 * number)
{
    VALIDATE_NOT_NULL(number);
    *number = mNumber;
    return NOERROR;
}

ECode CNotification::SetNumber(
    /* [in] */ Int32 number)
{
    mNumber = number;
    return NOERROR;
}

ECode CNotification::GetContentIntent(
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mContentIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode CNotification::SetContentIntent(
    /* [in] */ IPendingIntent* intent)
{
    mContentIntent = intent;
    return NOERROR;
}

ECode CNotification::GetDeleteIntent(
    /* [out] */ IPendingIntent** intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mDeleteIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode CNotification::SetDeleteIntent(
    /* [in] */ IPendingIntent* intent)
{
    mDeleteIntent = intent;
    return NOERROR;
}

ECode CNotification::GetFullScreenIntent(
    /* [out] */ IPendingIntent **intent)
{
    VALIDATE_NOT_NULL(intent);
    *intent = mFullScreenIntent;
    REFCOUNT_ADD(*intent);
    return NOERROR;
}

ECode CNotification::SetFullScreenIntent(
    /* [in] */ IPendingIntent * intent)
{
    mFullScreenIntent = intent;
    return NOERROR;
}

ECode CNotification::GetTickerText(
    /* [out] */ ICharSequence** text)
{
    VALIDATE_NOT_NULL(text);
    *text = mTickerText;
    REFCOUNT_ADD(*text);
    return NOERROR;
}

ECode CNotification::SetTickerText(
    /* [in] */ ICharSequence* text)
{
    mTickerText = text;
    return NOERROR;
}

ECode CNotification::GetTickerView(
    /* [out] */ IRemoteViews **view)
{
    VALIDATE_NOT_NULL(view);
    *view = mTickerView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CNotification::SetTickerView(
    /* [in] */ IRemoteViews * view)
{
    mTickerView = view;
    return NOERROR;
}

ECode CNotification::GetContentView(
    /* [out] */ IRemoteViews** view)
{
    VALIDATE_NOT_NULL(view);
    *view = mContentView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CNotification::SetContentView(
    /* [in] */ IRemoteViews* view)
{
    mContentView = view;
    return NOERROR;
}

ECode CNotification::GetBigContentView(
    /* [out] */ IRemoteViews **view)
{
    VALIDATE_NOT_NULL(view);
    *view = mBigContentView;
    REFCOUNT_ADD(*view);
    return NOERROR;
}

ECode CNotification::SetBigContentView(
    /* [in] */ IRemoteViews * view)
{
    mBigContentView = view;
    return NOERROR;
}

ECode CNotification::GetLargeIcon(
    /* [out] */ IBitmap **bitmap)
{
    VALIDATE_NOT_NULL(bitmap);
    *bitmap = mLargeIcon;
    REFCOUNT_ADD(*bitmap);
    return NOERROR;
}

ECode CNotification::SetLargeIcon(
    /* [in] */ IBitmap * bitmap)
{
    mLargeIcon = bitmap;
    return NOERROR;
}

ECode CNotification::GetSound(
    /* [out] */ IUri** sound)
{
    VALIDATE_NOT_NULL(sound);
    *sound = mSound;
    REFCOUNT_ADD(*sound)
    return NOERROR;
}

ECode CNotification::SetSound(
    /* [in] */ IUri* sound)
{
    mSound = sound;
    return NOERROR;
}

ECode CNotification::GetAudioStreamType(
    /* [out] */ Int32* type)
{
    VALIDATE_NOT_NULL(type);
    *type = mAudioStreamType;
    return NOERROR;
}

ECode CNotification::SetAudioStreamType(
    /* [in] */ Int32 type)
{
    mAudioStreamType = type;
    return NOERROR;
}

ECode CNotification::GetVibrate(
    /* [out, callee] */ ArrayOf<Int64>** vibrate)
{
    VALIDATE_NOT_NULL(vibrate);
    *vibrate = mVibrate;
    REFCOUNT_ADD(*vibrate);
    return NOERROR;
}

ECode CNotification::SetVibrate(
    /* [in] */ ArrayOf<Int64>* vibrate)
{
    mVibrate = vibrate;
    return NOERROR;
}

ECode CNotification::GetLedARGB(
    /* [out] */ Int32* argb)
{
    VALIDATE_NOT_NULL(argb);
    *argb = mLedARGB;
    return NOERROR;
}

ECode CNotification::SetLedARGB(
    /* [in] */ Int32 argb)
{
    mLedARGB = argb;
    return NOERROR;
}

ECode CNotification::GetLedOnMS(
    /* [out] */ Int32* onMS)
{
    VALIDATE_NOT_NULL(onMS);
    *onMS = mLedOnMS;
    return NOERROR;
}

ECode CNotification::SetLedOnMS(
    /* [in] */ Int32 onMS)
{
    mLedOnMS = onMS;
    return NOERROR;
}

ECode CNotification::GetLedOffMS(
    /* [out] */ Int32* offMS)
{
    VALIDATE_NOT_NULL(offMS);
    *offMS = mLedOffMS;
    return NOERROR;
}

ECode CNotification::SetLedOffMS(
    /* [in] */ Int32 offMS)
{
    mLedOffMS = offMS;
    return NOERROR;
}

ECode CNotification::GetDefaults(
    /* [out] */ Int32* defaults)
{
    VALIDATE_NOT_NULL(defaults);
    *defaults = mDefaults;
    return NOERROR;
}

ECode CNotification::SetDefaults(
    /* [in] */ Int32 defaults)
{
    mDefaults = defaults;
    return NOERROR;
}

ECode CNotification::GetFlags(
    /* [out] */ Int32* flags)
{
    VALIDATE_NOT_NULL(flags);
    *flags = mFlags;
    return NOERROR;
}

ECode CNotification::SetFlags(
    /* [in] */ Int32 flags)
{
    mFlags = flags;
    return NOERROR;
}

ECode CNotification::GetPriority(
    /* [out] */ Int32 * priority)
{
    VALIDATE_NOT_NULL(priority);
    *priority = mPriority;
    return NOERROR;
}

ECode CNotification::SetPriority(
    /* [in] */ Int32 priority)
{
    mPriority = priority;
    return NOERROR;
}

ECode CNotification::GetKind(
    /* [out, callee] */ ArrayOf<String> **kind)
{
    VALIDATE_NOT_NULL(kind);
    *kind = mKind;
    REFCOUNT_ADD(*kind);
    return NOERROR;
}

ECode CNotification::SetKind(
    /* [in] */ ArrayOf<String> *kind)
{
    mKind = kind;
    return NOERROR;
}

ECode CNotification::GetExtras(
    /* [out, callee] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;
    REFCOUNT_ADD(*extras)
    return NOERROR;
}

ECode CNotification::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;
    return NOERROR;
}

ECode CNotification::GetActions(
    /* [out, callee] */ ArrayOf<INotificationAction*>** actions)
{
    VALIDATE_NOT_NULL(actions);
    *actions = mActions;
    REFCOUNT_ADD(*actions);
    return NOERROR;
}

ECode CNotification::SetActions(
    /* [in] */ ArrayOf<INotificationAction*>* actions)
{
    mActions = actions;
    return NOERROR;
}

ECode CNotification::SetLatestEventInfo(
    /* [in] */ IContext* context,
    /* [in] */ ICharSequence* contentTitle,
    /* [in] */ ICharSequence* contentText,
    /* [in] */ IPendingIntent* contentIntent)
{
    AutoPtr<IRemoteViews> contentView;
    String packageName;

    FAIL_RETURN(context->GetPackageName(&packageName));
    FAIL_RETURN(CRemoteViews::New(packageName, R::layout::notification_template_base,
        (IRemoteViews**)&contentView));

    if (mIcon != 0) {
        contentView->SetImageViewResource(R::id::icon, mIcon);
    }
    if (mPriority < INotification::PRIORITY_LOW) {
        contentView->SetInt32(R::id::icon,
                String("SetBackgroundResource"),
                R::drawable::notification_template_icon_low_bg);
        contentView->SetInt32(R::id::status_bar_latest_event_content,
                String("SetBackgroundResource"),
                R::drawable::notification_bg_low);
    }
    if (contentTitle != NULL) {
        contentView->SetTextViewText(R::id::title, contentTitle);
    }
    if (contentText != NULL) {
        contentView->SetTextViewText(R::id::text, contentText);
    }
    if (mWhen != 0) {
        contentView->SetViewVisibility(R::id::time, IView::VISIBLE);
        contentView->SetInt64(R::id::time, String("SetTime"), mWhen);
    }
    if (mNumber != 0) {
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

    mContentView = contentView;
    mContentIntent = contentIntent;

    return NOERROR;
}

ECode CNotification::SetUser(
    /* [in] */ IUserHandle* user)
{
    if (user != NULL) {
        AutoPtr<IUserHandle> realUser = user;
        Int32 id;
        user->GetIdentifier(&id);
        if (id == IUserHandle::USER_ALL) {
            realUser = UserHandle::ALL;
        }

        if (mTickerView != NULL) {
            mTickerView->SetUser(user);
        }
        if (mContentView != NULL) {
            mContentView->SetUser(user);
        }
        if (mBigContentView != NULL) {
            mBigContentView->SetUser(user);
        }
    }

    return NOERROR;
}

ECode CNotification::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    VALIDATE_NOT_NULL(parcel);
    IParcelable* parcelable;
    Int32 value, version;

    parcel->ReadInt32(&version);
    parcel->ReadInt64(&mWhen);
    parcel->ReadInt32(&mIcon);
    parcel->ReadInt32(&mNumber);

    mContentIntent = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        AutoPtr<IInterface> obj;
        parcel->ReadInterfacePtr((Handle32*)&obj);
        mContentIntent = IPendingIntent::Probe(obj);
    }

    mDeleteIntent = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        AutoPtr<IInterface> obj;
        parcel->ReadInterfacePtr((Handle32*)&obj);
        mDeleteIntent = IPendingIntent::Probe(obj);
    }

    mTickerText = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        FAIL_RETURN(TextUtils::CHAR_SEQUENCE_CREATOR::CreateFromParcel(parcel, (ICharSequence**)&mTickerText));
    }

    mTickerView = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        CRemoteViews::New((IRemoteViews**)&mTickerView);
        parcelable = IParcelable::Probe(mTickerView);
        parcelable->ReadFromParcel(parcel);
    }

    mContentView = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        CRemoteViews::New((IRemoteViews**)&mContentView);
        parcelable = IParcelable::Probe(mContentView);
        parcelable->ReadFromParcel(parcel);
    }

    mLargeIcon = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        CBitmap::New((IBitmap**)&mLargeIcon);
        parcelable = IParcelable::Probe(mLargeIcon);
        parcelable->ReadFromParcel(parcel);
    }

    parcel->ReadInt32(&mDefaults);
    parcel->ReadInt32(&mFlags);

    mSound = NULL;
    parcel->ReadInt32(&value);
    if (value != 0) {
        AutoPtr<IInterface> obj;
        parcel->ReadInterfacePtr((Handle32*)&obj);
        mSound = IUri::Probe(obj);
    }

    parcel->ReadInt32(&mAudioStreamType);

    mVibrate = NULL;
    parcel->ReadArrayOf((Handle32*)(&mVibrate));

    parcel->ReadInt32(&mLedARGB);
    parcel->ReadInt32(&mLedOnMS);
    parcel->ReadInt32(&mLedOffMS);
    parcel->ReadInt32(&mIconLevel);

    parcel->ReadInt32(&value);
    if (value != 0) {
        CPendingIntent::New((IPendingIntent**)&mFullScreenIntent);
        parcelable = IParcelable::Probe(mFullScreenIntent);
        parcelable->ReadFromParcel(parcel);
    }

    parcel->ReadInt32(&mPriority);

    mKind = NULL;
    parcel->ReadArrayOfString((ArrayOf<String> **)&mKind); // may set kind to null

    parcel->ReadInt32(&value);
    if (value != 0) {
        mExtras = NULL;
        CBundle::New((IBundle**)&mExtras);
        parcelable = IParcelable::Probe(mExtras);
        parcelable->ReadFromParcel(parcel);
    }

    mActions = NULL;
    parcel->ReadInt32(&value);
    if (value >= 0) {
        Int32 size = value;
        mActions = ArrayOf<INotificationAction*>::Alloc(size);
        for (Int32 i = 0; i < size; ++i) {
            AutoPtr<INotificationAction> nc;
            parcel->ReadInt32(&value);
            if (value != 0) {
                CNotificationAction::New((INotificationAction**)&nc);
                parcelable = IParcelable::Probe(nc);
                parcelable->ReadFromParcel(parcel);
            }

            mActions->Set(i, nc);
        }
    }

    parcel->ReadInt32(&value);
    if (value != 0) {
        CRemoteViews::New((IRemoteViews**)&mBigContentView);
        parcelable = IParcelable::Probe(mBigContentView);
        parcelable->ReadFromParcel(parcel);
    }

    return NOERROR;
}

ECode CNotification::WriteToParcel(
    /* [in] */ IParcel *parcel)
{
    VALIDATE_NOT_NULL(parcel);
    IParcelable* parcelable;

    parcel->WriteInt32(1);

    parcel->WriteInt64(mWhen);
    parcel->WriteInt32(mIcon);
    parcel->WriteInt32(mNumber);

    if (mContentIntent != NULL) {
        parcel->WriteInt32(1);
        parcel->WriteInterfacePtr(mContentIntent);
    } else {
        parcel->WriteInt32(0);
    }

    if (mDeleteIntent != NULL) {
        parcel->WriteInt32(1);
        parcel->WriteInterfacePtr(mDeleteIntent);
    } else {
        parcel->WriteInt32(0);
    }

    if (mTickerText != NULL) {
        parcel->WriteInt32(1);
        TextUtils::WriteToParcel(mTickerText, parcel);
    } else {
        parcel->WriteInt32(0);
    }

    if (mTickerView != NULL) {
        parcel->WriteInt32(1);
        parcelable = IParcelable::Probe(mTickerView);
        parcelable->WriteToParcel(parcel);
    } else {
        parcel->WriteInt32(0);
    }

    if (mContentView != NULL) {
        parcel->WriteInt32(1);
        parcelable = IParcelable::Probe(mContentView);
        parcelable->WriteToParcel(parcel);
    } else {
        parcel->WriteInt32(0);
    }

    if (mLargeIcon != NULL) {
        parcel->WriteInt32(1);
        parcelable = IParcelable::Probe(mLargeIcon);
        parcelable->WriteToParcel(parcel);
    } else {
        parcel->WriteInt32(0);
    }

    parcel->WriteInt32(mDefaults);
    parcel->WriteInt32(mFlags);

    if (mSound != NULL) {
        parcel->WriteInt32(1);
        parcel->WriteInterfacePtr(mSound);
    } else {
        parcel->WriteInt32(0);
    }

    parcel->WriteInt32(mAudioStreamType);
    parcel->WriteArrayOf((Handle32)mVibrate.Get());
    parcel->WriteInt32(mLedARGB);
    parcel->WriteInt32(mLedOnMS);
    parcel->WriteInt32(mLedOffMS);
    parcel->WriteInt32(mIconLevel);

    if (mFullScreenIntent != NULL) {
        parcel->WriteInt32(1);
        parcelable = IParcelable::Probe(mFullScreenIntent);
        parcelable->WriteToParcel(parcel);
    } else {
        parcel->WriteInt32(0);
    }

    parcel->WriteInt32(mPriority);
    parcel->WriteArrayOfString(mKind); // ok for NULL

    if (mExtras != NULL) {
        parcel->WriteInt32(1);
        parcelable = IParcelable::Probe(mExtras);
        parcelable->WriteToParcel(parcel);
    } else {
        parcel->WriteInt32(0);
    }

    if (mActions == NULL) {
        parcel->WriteInt32(-1);
    }
    else {
        Int32 size = mActions->GetLength();
        parcel->WriteInt32(size);
        INotificationAction* nc;
        for (Int32 i = 0; i < size; ++i) {
            nc = (*mActions)[i];
            if (nc != NULL) {
                parcel->WriteInt32(1);
                parcelable = IParcelable::Probe(nc);
                parcelable->WriteToParcel(parcel);
            } else {
                parcel->WriteInt32(0);
            }
        }
    }

    if (mBigContentView != NULL) {
        parcel->WriteInt32(1);
        parcelable = IParcelable::Probe(mBigContentView);
        parcelable->WriteToParcel(parcel);
    } else {
        parcel->WriteInt32(0);
    }

    return NOERROR;
}

ECode CNotification::ToString(
    /* [out] */ String* info)
{
    VALIDATE_NOT_NULL(info);

    Int32 tmpInt;
    String tmpStr;

    StringBuilder sb("Notification={pri=");
    sb += mPriority;
    sb += ", contentView=";
    if (mContentView != NULL) {
        mContentView->ToString(&tmpStr);
        sb += tmpStr;
    } else {
        sb += ("NULL");
    }

    sb += ", ticker=";
    if (mTickerText != NULL) {
        mTickerText->ToString(&tmpStr);
        sb += tmpStr;
    }
    else {
        sb += ("NULL");
    }

    // TODO(dsandler): defaults take precedence over local values, so reorder the branches below
    sb += ", vibrate=";
    if (mVibrate != NULL && mVibrate->GetLength() > 0) {
        sb += "{";
        for (Int32 i = 0; i < mVibrate->GetLength(); i++) {
            if (i > 0) sb += ', ';
            sb += (*mVibrate)[i];
        }
        sb += "}";
    } else if ((mDefaults & DEFAULT_VIBRATE) != 0) {
        sb += "default";
    } else {
        sb += ("NULL");
    }

    sb += ", sound=";
    if (mSound != NULL) {
        mSound->ToString(&tmpStr);
        sb += tmpStr;
    } else if ((mDefaults & DEFAULT_SOUND) != 0) {
        sb += "default";
    } else {
        sb += "NULL";
    }

    sb += ", defaults=0x";
    sb += StringUtils::Int32ToHexString(mDefaults);
    sb += ", flags=0x";
    sb += StringUtils::Int32ToHexString(mFlags);
    sb += ", kind={";
    if (mKind == NULL) {
        sb += ("NULL");
    } else {
        for (Int32 i = 0; i < mKind->GetLength(); i++) {
            if (i > 0) sb += ", ";
            sb += (*mKind)[i];
        }
    }
    sb += ("}");

    if (mActions != NULL) {
        Int32 size = mActions->GetLength();
        sb += ", ";
        sb += size;
        sb += " action";
        if (size > 1) sb += "s";

        if (size > 0) {
            sb += "={";
            String tmpStr;
            AutoPtr<INotificationAction> action;
            for (Int32 i = 0; i < size; ++i) {
                if (i > 0) sb += ", ";
                sb += i;
                sb += "=";
                action = (*mActions)[i];
                if (action != NULL) {
                    AutoPtr<ICharSequence> tmpSeq;
                    action->GetTitle((ICharSequence**)&tmpSeq);
                    if (tmpSeq != NULL) {
                        tmpSeq->ToString(&tmpStr);
                        sb += tmpStr;
                    }
                }
                else {
                    sb += "NULL";
                }
            }
            sb += "}";
        }
    }
    else {
        sb += ", 0 action";
    }
    sb += ("}");

    *info = sb.ToString();
    return NOERROR;
}

}
}
}
