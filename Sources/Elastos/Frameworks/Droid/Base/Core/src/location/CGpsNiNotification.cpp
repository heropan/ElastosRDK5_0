#include "location/CGpsNiNotification.h"
#include "ext/frameworkext.h"

namespace Elastos {
namespace Droid {
namespace Location {

CGpsNiNotification::CGpsNiNotification()
                  : mNotificationId(0)
                  , mNiType(0)
                  , mNeedNotify(FALSE)
                  , mNeedVerify(FALSE)
                  , mPrivacyOverride(FALSE)
                  , mTimeout(0)
                  , mDefaultResponse(0)
                  , mRequestorId(NULL)
                  , mText(NULL)
                  , mRequestorIdEncoding(0)
                  , mTextEncoding(0)
{}

ECode CGpsNiNotification::constructor()
{
    return NOERROR;
}

ECode CGpsNiNotification::GetNotificationId(
    /* [out] */ Int32* notificationId)
{
    VALIDATE_NOT_NULL(notificationId);
    *notificationId = mNotificationId;
    return NOERROR;
}

ECode CGpsNiNotification::SetNotificationId(
    /* [in] */ Int32 notificationId)
{
    mNotificationId = notificationId;
    return NOERROR;
}

ECode CGpsNiNotification::GetNiType(
    /* [out] */ Int32* niType)
{
    VALIDATE_NOT_NULL(niType);
    *niType = mNiType;
    return NOERROR;
}

ECode CGpsNiNotification::SetNiType(
    /* [in] */ Int32 niType)
{
    mNiType = niType;
    return NOERROR;
}

ECode CGpsNiNotification::GetNeedNotify(
    /* [out] */ Boolean* needNotify)
{
    VALIDATE_NOT_NULL(needNotify);
    *needNotify = mNeedNotify;
    return NOERROR;
}

ECode CGpsNiNotification::SetNeedNotify(
    /* [in] */ Boolean needNotify)
{
    mNeedNotify = needNotify;
    return NOERROR;
}

ECode CGpsNiNotification::GetNeedVerify(
    /* [out] */ Boolean* needVerify)
{
    VALIDATE_NOT_NULL(needVerify);
    *needVerify = mNeedVerify;
    return NOERROR;
}

ECode CGpsNiNotification::SetNeedVerify(
    /* [in] */ Boolean needVerify)
{
    mNeedVerify = needVerify;
    return NOERROR;
}

ECode CGpsNiNotification::GetPrivacyOverride(
    /* [out] */ Boolean* privacyOverride)
{
    VALIDATE_NOT_NULL(privacyOverride);
    *privacyOverride = mPrivacyOverride;
    return NOERROR;
}

ECode CGpsNiNotification::SetPrivacyOverride(
    /* [in] */ Boolean privacyOverride)
{
    mPrivacyOverride = privacyOverride;
    return NOERROR;
}

ECode CGpsNiNotification::GetTimeOut(
    /* [out] */ Int32* timeout)
{
    VALIDATE_NOT_NULL(timeout);
    *timeout = mTimeout;
    return NOERROR;
}

ECode CGpsNiNotification::SetTimeOut(
    /* [in] */ Int32 timeout)
{
    mTimeout = timeout;
    return NOERROR;
}

ECode CGpsNiNotification::GetDefaultResponse(
    /* [out] */ Int32* defaultResponse)
{
    VALIDATE_NOT_NULL(defaultResponse);
    *defaultResponse = mDefaultResponse;
    return NOERROR;
}

ECode CGpsNiNotification::SetDefaultResponse(
    /* [in] */ Int32 defaultResponse)
{
    mDefaultResponse = defaultResponse;
    return NOERROR;
}

ECode CGpsNiNotification::GetRequestorId(
    /* [out] */ String* requestorId)
{
    VALIDATE_NOT_NULL(requestorId);
    *requestorId = mRequestorId;
    return NOERROR;
}

ECode CGpsNiNotification::SetRequestorId(
    /* [in] */ const String& requestorId)
{
    mRequestorId = requestorId;
    return NOERROR;
}

ECode CGpsNiNotification::GetText(
    /* [out] */ String* text)
{
    VALIDATE_NOT_NULL(text);
    *text = mText;
    return NOERROR;
}

ECode CGpsNiNotification::SetText(
    /* [in] */ const String& text)
{
    mText = text;
    return NOERROR;
}

ECode CGpsNiNotification::GetRequestorIdEncoding(
    /* [out] */ Int32* requestorIdEncoding)
{
    VALIDATE_NOT_NULL(requestorIdEncoding);
    *requestorIdEncoding = mRequestorIdEncoding;
    return NOERROR;
}

ECode CGpsNiNotification::SetRequestorIdEncoding(
    /* [in] */ Int32 requestorIdEncoding)
{
    mRequestorIdEncoding = requestorIdEncoding;
    return NOERROR;
}

ECode CGpsNiNotification::GetTextEncoding(
    /* [out] */ Int32* textEncoding)
{
    VALIDATE_NOT_NULL(textEncoding);
    *textEncoding = mTextEncoding;
    return NOERROR;
}

ECode CGpsNiNotification::SetTextEncoding(
    /* [in] */ Int32 textEncoding)
{
    mTextEncoding = textEncoding;
    return NOERROR;
}

ECode CGpsNiNotification::GetExtras(
    /* [out] */ IBundle** extras)
{
    VALIDATE_NOT_NULL(extras);
    *extras = mExtras;
    INTERFACE_ADDREF(*extras);
    return NOERROR;
}

ECode CGpsNiNotification::SetExtras(
    /* [in] */ IBundle* extras)
{
    mExtras = extras;
    return NOERROR;
}

}//namespace Location
}//namespace Droid
}//namespace Elastos
