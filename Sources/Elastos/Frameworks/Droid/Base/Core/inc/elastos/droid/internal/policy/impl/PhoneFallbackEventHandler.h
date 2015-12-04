#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEFALLBACKEVENTHANDLER_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEFALLBACKEVENTHANDLER_H__

#include "elastos/droid/ext/frameworkext.h"

using Elastos::Droid::Content::IContext;
using Elastos::Droid::App::IKeyguardManager;
using Elastos::Droid::App::ISearchManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::Media::IAudioManager;
using Elastos::Droid::Telephony::ITelephonyManager;
using Elastos::Droid::View::IFallbackEventHandler;
using Elastos::Droid::View::IKeyEvent;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

class PhoneFallbackEventHandler
    : public Object
    , public IPhoneFallbackEventHandler
    , public IFallbackEventHandler
{
public:
    CAR_INTERFACE_DECL()

    PhoneFallbackEventHandler();

    CARAPI constructor(
        /* [in] */ IContext* context);

    virtual CARAPI SetView(
        /* [in] */ IView* v);

    virtual CARAPI PreDispatchKeyEvent(
        /* [in] */ IKeyEvent* event);

    virtual CARAPI DispatchKeyEvent(
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    virtual CARAPI OnKeyDown(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    virtual CARAPI OnKeyUp(
        /* [in] */ Int32 keyCode,
        /* [in] */ IKeyEvent* event,
        /* [out] */ Boolean* result);

    virtual CARAPI StartCallActivity();

    virtual CARAPI GetSearchManager(
        /* [out] */ ISearchManager** result);

    virtual CARAPI GetTelephonyManager(
        /* [out] */ ITelephonyManager** result);

    virtual CARAPI GetKeyguardManager(
        /* [out] */ IKeyguardManager** result);

    virtual CARAPI GetAudioManager(
        /* [out] */ IAudioManager** result);

    virtual CARAPI SendCloseSystemWindows();

private:
    CARAPI_(void) HandleMediaKeyEvent(
        /* [in] */ IKeyEvent* keyEvent);

public:
    AutoPtr<IContext> mContext;
    AutoPtr<IView> mView;
    AutoPtr<IAudioManager> mAudioManager;
    AutoPtr<IKeyguardManager> mKeyguardManager;
    AutoPtr<ISearchManager> mSearchManager;
    AutoPtr<ITelephonyManager> mTelephonyManager;

private:
    static String TAG;
    static const Boolean DEBUG;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_PHONEFALLBACKEVENTHANDLER_H__

