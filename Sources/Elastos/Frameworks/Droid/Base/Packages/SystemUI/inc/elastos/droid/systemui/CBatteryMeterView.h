
#ifndef __ELASTOS_DROID_SYSTEMUI_CBATTERYMETERVIEW_H__
#define __ELASTOS_DROID_SYSTEMUI_CBATTERYMETERVIEW_H__

#include "_Elastos_Droid_SystemUI_CBatteryMeterView.h"
#include <elastos/droid/view/View.h>
#include <elastos/droid/content/BroadcastReceiver.h>
#include <elastos/droid/os/Runnable.h>

using Elastos::Droid::Content::BroadcastReceiver;
using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::IPath;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Os::Runnable;
using Elastos::Droid::View::View;
using Elastos::Droid::SystemUI::StatusBar::Policy::IBatteryController;
using Elastos::Droid::SystemUI::StatusBar::Policy::IBatteryStateChangeCallback;
using Elastos::Droid::Utility::IAttributeSet;

namespace Elastos {
namespace Droid {
namespace SystemUI {

CarClass(CBatteryMeterView)
    , public Elastos::Droid::View::View
    , public IDemoMode
    , public IBatteryStateChangeCallback
{
private:
    class BatteryTracker
        : public BroadcastReceiver
    {
    public:
        BatteryTracker(
            /* [in] */ CBatteryMeterView* host);

        CARAPI OnReceive(
            /* [in] */ IContext* context,
            /* [in] */ IIntent* intent);

    public:
        static Int32 UNKNOWN_LEVEL;

        // current battery status
        Int32 mLevel;
        String mPercentStr;
        Int32 mPlugType;
        Boolean mPlugged;
        Int32 mHealth;
        Int32 mStatus;
        String mTechnology;
        Int32 mVoltage;
        Int32 mTemperature;
        Boolean mTestmode;
        CBatteryMeterView* mHost;
    };

    class Runnable_1
        : public Runnable
    {
    public:
        Runnable_1(
            /* [in] */ Int32 level,
            /* [in] */ Int32 plugType,
            /* [in] */ Boolean testmode,
            /* [in] */ CBatteryMeterView* owner);

        CARAPI Run();

    public:
        Int32 mCurLevel;
        Int32 mIncr;
        Int32 mSaveLevel;
        Int32 mSavePlugged;
        AutoPtr<IIntent> mDummy;
        Boolean mTestmode;
        CBatteryMeterView* mOwner;
    };

public:
    CAR_OBJECT_DECL();

    CAR_INTERFACE_DECL();

    CBatteryMeterView();

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI OnAttachedToWindow();

    CARAPI OnDetachedFromWindow();

    CARAPI SetBatteryController(
        /* [in] */ IBatteryController* batteryController);

    CARAPI OnBatteryLevelChanged(
        /* [in] */ Int32 level,
        /* [in] */ Boolean pluggedIn,
        /* [in] */ Boolean charging);

    CARAPI OnPowerSaveChanged();

    CARAPI Draw(
        /* [in] */ ICanvas* c);

    CARAPI HasOverlappingRendering(
        /* [out] */ Boolean* result);

    CARAPI DispatchDemoCommand(
        /* [in] */ const String& command,
        /* [in] */ IBundle* args);

    CARAPI_(void) OnSizeChanged(
        /* [in] */ Int32 w,
        /* [in] */ Int32 h,
        /* [in] */ Int32 oldw,
        /* [in] */ Int32 oldh);

private:
    static AutoPtr<ArrayOf<Float> > LoadBoltPoints(
        /* [in] */ IResources* res);

    Int32 GetColorForLevel(
        /* [in] */ Int32 percent);

public:
    AutoPtr<BatteryTracker> mTracker;

    static String TAG;
    static String ACTION_LEVEL_TEST;

private:
    static Boolean ENABLE_PERCENT;
    static Boolean SINGLE_DIGIT_PERCENT;
    static Boolean SHOW_100_PERCENT;

    static Int32 FULL;

    static Float BOLT_LEVEL_THRESHOLD;  // opaque bolt below this fraction

    AutoPtr<ArrayOf<Int32> > mColors;

    Boolean mShowPercent;
    Float mButtonHeightFraction;
    Float mSubpixelSmoothingLeft;
    Float mSubpixelSmoothingRight;
    AutoPtr<IPaint> mFramePaint, mBatteryPaint, mWarningTextPaint, mTextPaint, mBoltPaint;
    Float mTextHeight;
    Float mWarningTextHeight;

    Int32 mHeight;
    Int32 mWidth;
    String mWarningString;
    Int32 mCriticalLevel;
    Int32 mChargeColor;
    AutoPtr<ArrayOf<Float> > mBoltPoints;
    AutoPtr<IPath> mBoltPath;

    AutoPtr<IRectF> mFrame;
    AutoPtr<IRectF> mButtonFrame;
    AutoPtr<IRectF> mBoltFrame;

    AutoPtr<IPath> mShapePath;
    AutoPtr<IPath> mClipPath;
    AutoPtr<IPath> mTextPath;

    AutoPtr<IBatteryController> mBatteryController;
    Boolean mPowerSaveEnabled;

    Boolean mDemoMode;
    AutoPtr<BatteryTracker> mDemoTracker;
};

} // SystemUI
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_SYSTEMUI_CBATTERYMETERVIEW_H__
