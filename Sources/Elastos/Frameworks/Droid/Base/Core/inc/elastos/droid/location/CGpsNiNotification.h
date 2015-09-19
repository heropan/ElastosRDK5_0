#ifndef __ELASTOS_DROID_LOCATION_CGPSNINOTIFICATION_H__
#define __ELASTOS_DROID_LOCATION_CGPSNINOTIFICATION_H__
#include "_Elastos_Droid_Location_CGpsNiNotification.h"
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace Location {

CarClass(CGpsNiNotification)
{
public:
    CGpsNiNotification();

    constructor();

    CARAPI GetNotificationId(
        /* [out] */ Int32* notifacationId);

    CARAPI SetNotificationId(
        /* [in] */ Int32 notifacationId);

    CARAPI GetNiType(
        /* [out] */ Int32* niType);

    CARAPI SetNiType(
        /* [in] */ Int32 niType);

    CARAPI GetNeedNotify(
        /* [out] */ Boolean* needNotify);

    CARAPI SetNeedNotify(
        /* [in] */ Boolean needNotify);

    CARAPI GetNeedVerify(
        /* [out] */ Boolean* needVerify);

    CARAPI SetNeedVerify(
        /* [in] */ Boolean needVerify);

    CARAPI GetPrivacyOverride(
        /* [out] */ Boolean* privacyOverride);

    CARAPI SetPrivacyOverride(
        /* [in] */ Boolean privacyOverride);

    CARAPI GetTimeOut(
        /* [out] */ Int32* timeout);

    CARAPI SetTimeOut(
        /* [in] */ Int32 timeOut);

    CARAPI GetDefaultResponse(
        /* [out] */ Int32* defaultResponse);

    CARAPI SetDefaultResponse(
        /* [in] */ Int32 defaultResponse);

    CARAPI GetRequestorId(
        /* [out] */ String* requestorId);

    CARAPI SetRequestorId(
        /* [in] */ const String& requestorId);

    CARAPI GetText(
        /* [out] */ String* text);

    CARAPI SetText(
        /* [in] */ const String& text);

    CARAPI GetRequestorIdEncoding(
        /* [out] */ Int32* requestorIdEncoding);

    CARAPI SetRequestorIdEncoding(
        /* [in] */ Int32 requestorIdEncoding);

    CARAPI GetTextEncoding(
        /* [out] */ Int32* textEncoding);

    CARAPI SetTextEncoding(
        /* [in] */ Int32 textEncoding);

    CARAPI GetExtras(
        /* [out] */ IBundle** extras);

    CARAPI SetExtras(
        /* [in] */ IBundle* extras);
private:
    Int32 mNotificationId;
    Int32 mNiType;
    Boolean mNeedNotify;
    Boolean mNeedVerify;
    Boolean mPrivacyOverride;
    Int32 mTimeout;
    Int32 mDefaultResponse;
    String mRequestorId;
    String mText;
    Int32 mRequestorIdEncoding;
    Int32 mTextEncoding;
    AutoPtr<IBundle> mExtras;
};

}//namespace Location
}//namespace Droid
}//namespace Elastos
#endif
