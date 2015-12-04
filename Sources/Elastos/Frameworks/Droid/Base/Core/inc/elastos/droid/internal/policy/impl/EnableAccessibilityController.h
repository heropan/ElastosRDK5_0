#ifndef __ELASTOS_DROID_INTERNAL_POLICY_IMPL_ENABLEACCESSIBILITYCONTROLLER_H__
#define __ELASTOS_DROID_INTERNAL_POLICY_IMPL_ENABLEACCESSIBILITYCONTROLLER_H__

#include "elastos/droid/ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Droid::AccessibilityService::IAccessibilityServiceInfo;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Media::IRingtone;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IUserManager;
using Elastos::Droid::Speech::Tts::ITextToSpeech;
using Elastos::Droid::Speech::Tts::ITextToSpeechOnInitListener;
using Elastos::Droid::View::Accessibility::IIAccessibilityManager;
using Elastos::Droid::View::IIWindowManager;
using Elastos::Droid::View::IMotionEvent;

using Elastos::Core::IRunnable;
using Elastos::Utility::Etl::List;

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

class EnableAccessibilityController
    : public Object
    , public IEnableAccessibilityController
{
private:
    class InnerHandler
        : public Object
        , public IHandler
    {
    public:
        CAR_INTERFACE_DECL()

        InnerHandler(
            /* [in] */ EnableAccessibilityController* owner);

        // @Override
        CARAPI HandleMessage(
            /* [in] */ IMessage* message);

    private:
        EnableAccessibilityController* mOwner;
    };

    class InnerTextToSpeechOnInitListener1
        : public Object
        , public ITextToSpeechOnInitListener
    {
    public:
        CAR_INTERFACE_DECL()

        InnerTextToSpeechOnInitListener1(
            /* [in] */ EnableAccessibilityController* owner);

        // @Override
        CARAPI OnInit(
            /* [in] */ Int32 status);

    private:
        EnableAccessibilityController* mOwner;
    };

public:
    CAR_INTERFACE_DECL()

    EnableAccessibilityController();

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IRunnable* onAccessibilityEnabledCallback);

    static CARAPI_(Boolean) CanEnableAccessibilityViaGesture(
        /* [in] */ IContext* context);

    virtual CARAPI OnDestroy();

    virtual CARAPI OnInterceptTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

    virtual CARAPI OnTouchEvent(
        /* [in] */ IMotionEvent* event,
        /* [out] */ Boolean* result);

private:
    static CARAPI_(List<AutoPtr<IAccessibilityServiceInfo> >) GetInstalledSpeakingAccessibilityServices(
        /* [in] */ IContext* context);

    CARAPI_(void) Cancel();

    CARAPI_(void) EnableAccessibility();

public:
    static const Int32 MESSAGE_SPEAK_WARNING = 1;
    static const Int32 MESSAGE_SPEAK_ENABLE_CANCELED = 2;
    static const Int32 MESSAGE_ENABLE_ACCESSIBILITY = 3;

private:
    static const Int32 SPEAK_WARNING_DELAY_MILLIS = 2000;
    static const Int32 ENABLE_ACCESSIBILITY_DELAY_MILLIS = 6000;
    /*const*/ AutoPtr<IHandler> mHandler;
    /*const*/ AutoPtr<IIWindowManager> mWindowManager;
    /*const*/ AutoPtr<IIAccessibilityManager> mAccessibilityManager;
    /*const*/ AutoPtr<IContext> mContext;
    /*const*/ AutoPtr<IRunnable> mOnAccessibilityEnabledCallback;
    /*const*/ AutoPtr<IUserManager> mUserManager;
    /*const*/ AutoPtr<ITextToSpeech> mTts;
    /*const*/ AutoPtr<IRingtone> mTone;
    /*const*/ Float mTouchSlop;
    Boolean mDestroyed;
    Boolean mCanceled;
    Float mFirstPointerDownX;
    Float mFirstPointerDownY;
    Float mSecondPointerDownX;
    Float mSecondPointerDownY;
};

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_INTERNAL_POLICY_IMPL_ENABLEACCESSIBILITYCONTROLLER_H__

