
#include "elastos/droid/systemui/CBatteryMeterView.h"
#include "elastos/droid/R.h"
#include "elastos/droid/content/CIntent.h"
#include "elastos/droid/content/CIntentFilter.h"
#include "elastos/droid/graphics/CPath.h"
#include "elastos/droid/graphics/CRectF.h"
//#include "elastos/droid/provider/Settings.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CTypefaceHelper.h"
#include "elastos/droid/graphics/CPaintFontMetrics.h"

#include <elastos/core/Math.h>
#include <elastos/core/StringUtils.h>

using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::IIntentFilter;
using Elastos::Droid::Content::CIntentFilter;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Content::Res::ITypedArray;
using Elastos::Droid::Graphics::CPath;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::ITypeface;
using Elastos::Droid::Graphics::ITypefaceHelper;
using Elastos::Droid::Graphics::CTypefaceHelper;
using Elastos::Droid::Graphics::PaintStyle_FILL_AND_STROKE;
using Elastos::Droid::Graphics::PaintAlign_CENTER;
using Elastos::Droid::Graphics::IPaintFontMetrics;
using Elastos::Droid::Graphics::CPaintFontMetrics;
using Elastos::Droid::Graphics::PathOp_DIFFERENCE;
using Elastos::Droid::Graphics::PathOp_INTERSECT;
using Elastos::Droid::Graphics::PathDirection_CCW;
//using Elastos::Droid::Provider::Settings;
using Elastos::Droid::Os::IBatteryManager;
using Elastos::Droid::View::IView;
using Elastos::Droid::SystemUI::StatusBar::Policy::EIID_IBatteryStateChangeCallback;

using Elastos::Core::StringUtils;

namespace Elastos {
namespace Droid {
namespace SystemUI {

//===============================================================
// CBatteryMeterView::BatteryTracker
//===============================================================
Int32 CBatteryMeterView::BatteryTracker::UNKNOWN_LEVEL = -1;
CBatteryMeterView::BatteryTracker::BatteryTracker(
    /* [in] */ CBatteryMeterView* host)
    : mLevel(UNKNOWN_LEVEL)
    , mPlugType(0)
    , mPlugged(FALSE)
    , mHealth(0)
    , mStatus(0)
    , mVoltage(0)
    , mTemperature(0)
    , mTestmode(FALSE)
    , mHost(host)
{
}

ECode CBatteryMeterView::BatteryTracker::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IIntent::ACTION_BATTERY_CHANGED)) {
        Boolean bExt = FALSE;
        if (mTestmode && ! (intent->GetBooleanExtra(String("testmode"), FALSE, &bExt), bExt))
            return NOERROR;

        Int32 ext1 = 0, ext2 = 0;
        intent->GetInt32Extra(IBatteryManager::EXTRA_LEVEL, 0, &ext1);
        intent->GetInt32Extra(IBatteryManager::EXTRA_SCALE, 100, &ext2);
        mLevel = (Int32)(100.0 * ext1 / ext2);

        intent->GetInt32Extra(IBatteryManager::EXTRA_PLUGGED, 0, &mPlugType);
        mPlugged = mPlugType != 0;
        intent->GetInt32Extra(IBatteryManager::EXTRA_HEALTH,
                IBatteryManager::BATTERY_HEALTH_UNKNOWN, &mHealth);
        intent->GetInt32Extra(IBatteryManager::EXTRA_STATUS,
                IBatteryManager::BATTERY_STATUS_UNKNOWN, &mStatus);
        intent->GetStringExtra(IBatteryManager::EXTRA_TECHNOLOGY, &mTechnology);
        intent->GetInt32Extra(IBatteryManager::EXTRA_VOLTAGE, 0, &mVoltage);
        intent->GetInt32Extra(IBatteryManager::EXTRA_TEMPERATURE, 0, &mTemperature);

        String str;
        context->GetString(R::string::accessibility_battery_level, mLevel, &str);
        mHost->SetContentDescription(str);
        mHost->PostInvalidate();
    }
    else if (action.Equals(ACTION_LEVEL_TEST)) {
        mTestmode = TRUE;
        AutoPtr<IRunnable> r = new Runnable_1();
        Boolean result = FALSE;
        mHost->Post(r, &result);
    }
    return NOERROR;
}

//===============================================================
// CBatteryMeterView::Runnable_1
//===============================================================
CBatteryMeterView::Runnable_1::Runnable_1(
    /* [in] */ Int32 level,
    /* [in] */ Int32 plugType,
    /* [in] */ Boolean testmode,
    /* [in] */ CBatteryMeterView* owner)
{
    mCurLevel = 0;
    mIncr = 1;
    mSaveLevel = level;
    mSavePlugged = plugType;
    mTestmode = testmode;
    mOwner = owner;
    CIntent::New(IIntent::ACTION_BATTERY_CHANGED, (IIntent**)&mDummy);
}

ECode CBatteryMeterView::Runnable_1::Run()
{
    if (mCurLevel < 0) {
        mTestmode = FALSE;
        mDummy->PutExtra(String("level"), mSaveLevel);
        mDummy->PutExtra(String("plugged"), mSavePlugged);
        mDummy->PutExtra(String("testmode"), FALSE);
    }
    else {
        mDummy->PutExtra(String("level"), mCurLevel);
        mDummy->PutExtra(String("plugged"), mIncr > 0 ? IBatteryManager::BATTERY_PLUGGED_AC : 0);
        mDummy->PutExtra(String("testmode"), TRUE);
    }
    AutoPtr<IContext> ctx;
    assert(0 && "TODO");
//    mOwner->GetContext((IContext**)&ctx);
    ctx->SendBroadcast(mDummy);

    if (!mTestmode) return NOERROR;

    mCurLevel += mIncr;
    if (mCurLevel == 100) {
        mIncr *= -1;
    }
    assert(0 && "TODO");
//    mOwner->PostDelayed(this, 200);
    return NOERROR;
}

//===============================================================
// CBatteryMeterView::
//===============================================================
String CBatteryMeterView::TAG("BatteryMeterView");// = BatteryMeterView.class.getSimpleName();
String CBatteryMeterView::ACTION_LEVEL_TEST = String("com.android.systemui.BATTERY_LEVEL_TEST");
Boolean CBatteryMeterView::ENABLE_PERCENT = TRUE;
Boolean CBatteryMeterView::SINGLE_DIGIT_PERCENT = FALSE;
Boolean CBatteryMeterView::SHOW_100_PERCENT = FALSE;
Int32 CBatteryMeterView::FULL = 96;
Float CBatteryMeterView::BOLT_LEVEL_THRESHOLD = 0.3f;  // opaque bolt below this fraction

CAR_OBJECT_IMPL(CBatteryMeterView);
CAR_INTERFACE_IMPL_2(CBatteryMeterView, View, IDemoMode, IBatteryStateChangeCallback);
CBatteryMeterView::CBatteryMeterView()
    : mShowPercent(TRUE)
    , mButtonHeightFraction(0)
    , mSubpixelSmoothingLeft(0)
    , mSubpixelSmoothingRight(0)
    , mTextHeight(0)
    , mWarningTextHeight(0)
    , mHeight(0)
    , mWidth(0)
    , mCriticalLevel(0)
    , mChargeColor(0)
    , mPowerSaveEnabled(FALSE)
    , mDemoMode(FALSE)
{
    CPath::New((IPath**)&mBoltPath);
    CRectF::New((IRectF**)&mFrame);
    CRectF::New((IRectF**)&mButtonFrame);
    CRectF::New((IRectF**)&mBoltFrame);
    CPath::New((IPath**)&mShapePath);
    CPath::New((IPath**)&mClipPath);
    CPath::New((IPath**)&mTextPath);

    mTracker = new BatteryTracker();
    mDemoTracker = new BatteryTracker();
}

ECode CBatteryMeterView::OnAttachedToWindow()
{
    assert(0 && "TODO");
//    View::OnAttachedToWindow();

    AutoPtr<IIntentFilter> filter;
    CIntentFilter::New((IIntentFilter**)&filter);
    filter->AddAction(IIntent::ACTION_BATTERY_CHANGED);
    filter->AddAction(ACTION_LEVEL_TEST);
    AutoPtr<IContext> ctx;
//    GetContext((IContext**)&ctx);
    AutoPtr<IIntent> sticky;
    ctx->RegisterReceiver(mTracker, filter, (IIntent**)&sticky);
    if (sticky != NULL) {
        // preload the battery level
        AutoPtr<IContext> c;
//        GetContext((IContext**)&c);
        mTracker->OnReceive(c, sticky);
    }
    mBatteryController->AddStateChangedCallback(this);
    return NOERROR;
}

ECode CBatteryMeterView::OnDetachedFromWindow()
{
    assert(0 && "TODO");
//    View::OnDetachedFromWindow();

    AutoPtr<IContext> ctx;
//    GetContext((IContext**)&ctx);
    ctx->UnregisterReceiver(mTracker);
    mBatteryController->RemoveStateChangedCallback(this);
    return NOERROR;
}

ECode CBatteryMeterView::constructor(
    /* [in] */ IContext* context)
{
    return constructor(context, NULL, 0);
}

ECode CBatteryMeterView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return constructor(context, attrs, 0);
}

ECode CBatteryMeterView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    assert(0 && "TODO");
//    View::constructor(context, attrs, defStyle);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<ITypedArray> atts;
    // context->ObtainStyledAttributes(attrs, R::styleable::BatteryMeterView,
    //         defStyle, 0, (ITypedArray**)&atts);
    Int32 clr = 0;
//    res->GetColor(R::color::batterymeter_frame_color, &clr);
    Int32 frameColor = 0;
//    atts->GetColor(R::styleable::BatteryMeterView_frameColor, clr, &frameColor);
    AutoPtr<ITypedArray> levels;
//    res->ObtainTypedArray(R::array::batterymeter_color_levels, (ITypedArray**)&levels);
    AutoPtr<ITypedArray> colors;
//    res->ObtainTypedArray(R::array::batterymeter_color_values, (ITypedArray**)&colors);

    Int32 N = 0;
    levels->GetLength(&N);
    mColors = ArrayOf<Int32>::Alloc(2*N);
    for (Int32 i = 0; i < N; i++) {
        levels->GetInt32(i, 0, &((*mColors)[2*i]));
        colors->GetColor(i, 0, &((*mColors)[2*i+1]));
    }
    levels->Recycle();
    colors->Recycle();
    atts->Recycle();
    AutoPtr<IContentResolver> cr;
    context->GetContentResolver((IContentResolver**)&cr);
    Int32 iPercent = 0;
    // Settings::System::GetInt32(
    //         cr, "status_bar_show_battery_percent", 0, &iPercent);
    mShowPercent = ENABLE_PERCENT && 0 != iPercent;
//    context->GetString(R::string::battery_meter_very_low_overlay_symbol, &mWarningString);
    AutoPtr<IResources> r;
    context->GetResources((IResources**)&r);
    r->GetInteger(
            R::integer::config_criticalBatteryWarningLevel, &mCriticalLevel);
    // r->GetFraction(
    //         R::fraction::battery_button_height_fraction, 1, 1, &mButtonHeightFraction);
    // r->GetFraction(
    //         R::fraction::battery_subpixel_smoothing_left, 1, 1, &mSubpixelSmoothingLeft);
    // r->GetFraction(
    //         R::fraction::battery_subpixel_smoothing_right, 1, 1, &mSubpixelSmoothingRight);

    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mFramePaint);
    mFramePaint->SetColor(frameColor);
    mFramePaint->SetDither(TRUE);
    mFramePaint->SetStrokeWidth(0);
    mFramePaint->SetStyle(PaintStyle_FILL_AND_STROKE);

    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mBatteryPaint);
    mBatteryPaint->SetDither(TRUE);
    mBatteryPaint->SetStrokeWidth(0);
    mBatteryPaint->SetStyle(PaintStyle_FILL_AND_STROKE);

    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mTextPaint);
    AutoPtr<ITypefaceHelper> tfacehlp;
    CTypefaceHelper::AcquireSingleton((ITypefaceHelper**)&tfacehlp);
    AutoPtr<ITypeface> font;
    tfacehlp->Create(String("sans-serif-condensed"), ITypeface::BOLD, (ITypeface**)&font);
    mTextPaint->SetTypeface(font);
    mTextPaint->SetTextAlign(PaintAlign_CENTER);

    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mWarningTextPaint);
    mWarningTextPaint->SetColor((*mColors)[1]);
    tfacehlp->Create(String("sans-serif"), ITypeface::BOLD, (ITypeface**)&font);
    mWarningTextPaint->SetTypeface(font);
    mWarningTextPaint->SetTextAlign(PaintAlign_CENTER);

    AutoPtr<IResources> rClr;
//    GetResources((IResources**)&rClr);
//    rClr->GetColor(R::color::batterymeter_charge_color, &mChargeColor);

    CPaint::New(IPaint::ANTI_ALIAS_FLAG, (IPaint**)&mBoltPaint);
    Int32 bolt_clr = 0;
//    res->GetColor(R::color::batterymeter_bolt_color, &bolt_clr)
    mBoltPaint->SetColor(bolt_clr);
    mBoltPoints = LoadBoltPoints(res);
    return NOERROR;
}

ECode CBatteryMeterView::SetBatteryController(
    /* [in] */ IBatteryController* batteryController)
{
    mBatteryController = batteryController;
    mBatteryController->IsPowerSave(&mPowerSaveEnabled);
    return NOERROR;
}

ECode CBatteryMeterView::OnBatteryLevelChanged(
    /* [in] */ Int32 level,
    /* [in] */ Boolean pluggedIn,
    /* [in] */ Boolean charging)
{
    // TODO: Use this callback instead of own broadcast receiver.
    return NOERROR;
}

ECode CBatteryMeterView::OnPowerSaveChanged()
{
    mBatteryController->IsPowerSave(&mPowerSaveEnabled);
    // View::Invalidate();
    return NOERROR;
}

AutoPtr<ArrayOf<Float> > CBatteryMeterView::LoadBoltPoints(
    /* [in] */ IResources* res)
{
    assert(0 && "TODO");
    AutoPtr<ArrayOf<Int32> > pts;
//    res->GetInt32Array(R::array::batterymeter_bolt_points, (ArrayOf<Int32>**)&pts);
    Int32 maxX = 0, maxY = 0;
    for (Int32 i = 0; i < pts->GetLength(); i += 2) {
        maxX = Elastos::Core::Math::Max(maxX, (*pts)[i]);
        maxY = Elastos::Core::Math::Max(maxY, (*pts)[i + 1]);
    }
    AutoPtr<ArrayOf<Float> > ptsF = ArrayOf<Float>::Alloc(pts->GetLength());
    for (Int32 i = 0; i < pts->GetLength(); i += 2) {
        (*ptsF)[i] = (Float)(*pts)[i] / maxX;
        (*ptsF)[i + 1] = (Float)(*pts)[i + 1] / maxY;
    }
    return ptsF;
}

ECode CBatteryMeterView::OnSizeChanged(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 oldw,
    /* [in] */ Int32 oldh)
{
    mHeight = h;
    mWidth = w;
    mWarningTextPaint->SetTextSize(h * 0.75f);
    AutoPtr<IPaintFontMetrics> fm;
    mWarningTextPaint->GetFontMetrics((IPaintFontMetrics**)&fm);
    fm->GetAscent(&mWarningTextHeight);
    mWarningTextHeight = -mWarningTextHeight;
    return NOERROR;
}

Int32 CBatteryMeterView::GetColorForLevel(
    /* [in] */ Int32 percent)
{

    // If we are in power save mode, always use the normal color.
    if (mPowerSaveEnabled) {
        return (*mColors)[mColors->GetLength()-1];
    }
    Int32 thresh = 0, color = 0;
    for (Int32 i = 0; i < mColors->GetLength(); i+=2) {
        thresh = (*mColors)[i];
        color = (*mColors)[i+1];
        if (percent <= thresh) return color;
    }
    return color;
}

ECode CBatteryMeterView::Draw(
    /* [in] */ ICanvas* c)
{
    AutoPtr<BatteryTracker> tracker = mDemoMode ? mDemoTracker : mTracker;
    Int32 level = tracker->mLevel;

    if (level == BatteryTracker::UNKNOWN_LEVEL) return NOERROR;

    Float drawFrac = (float) level / 100.0f;
    Int32 pt = 0;
    assert(0 && "TODO");
    // View::GetPaddingTop(&pt);
    Int32 pl = 0;
    // View::GetPaddingLeft(&pl);
    Int32 pr = 0;
    // View::GetPaddingRight(&pr);
    Int32 pb = 0;
    // View::GetPaddingBottom(&pb);
    Int32 height = mHeight - pt - pb;
    Int32 width = mWidth - pl - pr;

    Int32 buttonHeight = (Int32) (height * mButtonHeightFraction);

    mFrame->Set(0, 0, width, height);
    mFrame->Offset(pl, pt);

    // button-frame: area above the battery body
    AutoPtr<CRectF> cfm = (CRectF*)mFrame.Get();
    mButtonFrame->Set(
            cfm->mLeft + Elastos::Core::Math::Round(width * 0.25f),
            cfm->mTop,
            cfm->mRight - Elastos::Core::Math::Round(width * 0.25f),
            cfm->mTop + buttonHeight);

    AutoPtr<CRectF> cbfm = (CRectF*)mButtonFrame.Get();
    cbfm->mTop += mSubpixelSmoothingLeft;
    cbfm->mLeft += mSubpixelSmoothingLeft;
    cbfm->mRight -= mSubpixelSmoothingRight;

    // frame: battery body area
    cfm->mTop += buttonHeight;
    cfm->mLeft += mSubpixelSmoothingLeft;
    cfm->mTop += mSubpixelSmoothingLeft;
    cfm->mRight -= mSubpixelSmoothingRight;
    cfm->mBottom -= mSubpixelSmoothingRight;

    // set the battery charging color
    mBatteryPaint->SetColor(tracker->mPlugged ? mChargeColor : GetColorForLevel(level));

    if (level >= FULL) {
        drawFrac = 1.0;
    }
    else if (level <= mCriticalLevel) {
        drawFrac = 0.f;
    }

    Float fh = 0;
    mFrame->GetHeight(&fh);
    Float levelTop = drawFrac == 1.0f ? cbfm->mTop
            : (cfm->mTop + (fh * (1.0f - drawFrac)));

    // define the battery shape
    mShapePath->Reset();
    mShapePath->MoveTo(cbfm->mLeft, cbfm->mTop);
    mShapePath->LineTo(cbfm->mRight, cbfm->mTop);
    mShapePath->LineTo(cbfm->mRight, cfm->mTop);
    mShapePath->LineTo(cfm->mRight, cfm->mTop);
    mShapePath->LineTo(cfm->mRight, cfm->mBottom);
    mShapePath->LineTo(cfm->mLeft, cfm->mBottom);
    mShapePath->LineTo(cfm->mLeft, cfm->mTop);
    mShapePath->LineTo(cbfm->mLeft, cfm->mTop);
    mShapePath->LineTo(cbfm->mLeft, cbfm->mTop);

    if (tracker->mPlugged) {
        // define the bolt shape
        Float w, h;
        mFrame->GetWidth(&w);
        mFrame->GetHeight(&h);
        Float bl = cfm->mLeft + w / 4.5f;
        Float bt = cfm->mTop + h / 6.0f;
        Float br = cfm->mRight - w / 7.0f;
        Float bb = cfm->mBottom - h / 10.0f;
        AutoPtr<CRectF> cblfm = (CRectF*)mBoltFrame.Get();
        if (cblfm->mLeft != bl || cblfm->mTop != bt
                || cblfm->mRight != br || cblfm->mBottom != bb) {
            mBoltFrame->Set(bl, bt, br, bb);
            mBoltPath->Reset();
            Float bw, bh;
            mBoltFrame->GetWidth(&bw);
            mBoltFrame->GetHeight(&bh);
            mBoltPath->MoveTo(
                    cblfm->mLeft + (*mBoltPoints)[0] * bw,
                    cblfm->mTop + (*mBoltPoints)[1] * bh);
            for (Int32 i = 2; i < mBoltPoints->GetLength(); i += 2) {
                mBoltPath->LineTo(
                        cblfm->mLeft + (*mBoltPoints)[i] * bw,
                        cblfm->mTop + (*mBoltPoints)[i + 1] * bh);
            }
            mBoltPath->LineTo(
                    cblfm->mLeft + (*mBoltPoints)[0] * bw,
                    cblfm->mTop + (*mBoltPoints)[1] * bh);
        }

        Float boltPct = (cblfm->mBottom - levelTop) / (cblfm->mBottom - cblfm->mTop);
        boltPct = Elastos::Core::Math::Min(Elastos::Core::Math::Max(boltPct, 0.f), 1.f);
        if (boltPct <= BOLT_LEVEL_THRESHOLD) {
            // draw the bolt if opaque
            c->DrawPath(mBoltPath, mBoltPaint);
        }
        else {
            // otherwise cut the bolt out of the overall shape
            Boolean b = FALSE;
            mShapePath->Op(mBoltPath, PathOp_DIFFERENCE, &b);
        }
    }

    // compute percentage text
    Boolean pctOpaque = FALSE;
    Float pctX = 0, pctY = 0;
    String pctText;
    if (!tracker->mPlugged && level > mCriticalLevel && mShowPercent
            && !(tracker->mLevel == 100 && !SHOW_100_PERCENT)) {
        mTextPaint->SetColor(GetColorForLevel(level));
        mTextPaint->SetTextSize(height *
                (SINGLE_DIGIT_PERCENT ? 0.75f
                        : (tracker->mLevel == 100 ? 0.38f : 0.5f)));
        AutoPtr<IPaintFontMetrics> pm;
        mTextPaint->GetFontMetrics((IPaintFontMetrics**)&pm);
        pm->GetAscent(&mTextHeight);
        mTextHeight = -mTextHeight;
        assert(0 && "TODO");
        // pctText = String::ValueOf(SINGLE_DIGIT_PERCENT ? (level/10) : level);
        pctX = mWidth * 0.5f;
        pctY = (mHeight + mTextHeight) * 0.47f;
        pctOpaque = levelTop > pctY;
        if (!pctOpaque) {
            mTextPath->Reset();
            mTextPaint->GetTextPath(pctText, 0, pctText.GetLength(), pctX, pctY, mTextPath);
            // cut the percentage text out of the overall shape
            Boolean bRes = FALSE;
            mShapePath->Op(mTextPath, PathOp_DIFFERENCE, &bRes);
        }
    }

    // draw the battery shape background
    c->DrawPath(mShapePath, mFramePaint);

    // draw the battery shape, clipped to charging level
    cfm->mTop = levelTop;
    mClipPath->Reset();
    mClipPath->AddRect(mFrame,  PathDirection_CCW);
    Boolean b = FALSE;
    mShapePath->Op(mClipPath, PathOp_INTERSECT, &b);
    c->DrawPath(mShapePath, mBatteryPaint);

    if (!tracker->mPlugged) {
        if (level <= mCriticalLevel) {
            // draw the warning text
            Float x = mWidth * 0.5f;
            Float y = (mHeight + mWarningTextHeight) * 0.48f;
            c->DrawText(mWarningString, x, y, mWarningTextPaint);
        }
        else if (pctOpaque) {
            // draw the percentage text
            c->DrawText(pctText, pctX, pctY, mTextPaint);
        }
    }
    return NOERROR;
}

ECode CBatteryMeterView::HasOverlappingRendering(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)
    *result = FALSE;
    return NOERROR;
}

ECode CBatteryMeterView::DispatchDemoCommand(
    /* [in] */ const String& command,
    /* [in] */ IBundle* args)
{
    if (!mDemoMode && command.Equals(COMMAND_ENTER)) {
        mDemoMode = TRUE;
        mDemoTracker->mLevel = mTracker->mLevel;
        mDemoTracker->mPlugged = mTracker->mPlugged;
    }
    else if (mDemoMode && command.Equals(COMMAND_EXIT)) {
        mDemoMode = FALSE;
//         View::PostInvalidate();
    }
    else if (mDemoMode && command.Equals(COMMAND_BATTERY)) {
       String level;
       args->GetString(String("level"), &level);
       String plugged;
       args->GetString(String("plugged"), &plugged);
       if (level != NULL) {
           mDemoTracker->mLevel = Elastos::Core::Math::Min(Elastos::Core::Math::Max(StringUtils::ParseInt32(level), 0), 100);
       }
       if (plugged != NULL) {
        assert(0 && "TODO");
//           mDemoTracker->mPlugged = StringUtils::ParseBoolean(plugged);
       }
//         View::PostInvalidate();
    }
    return NOERROR;
}

} // namespace SystemUI
} // namepsace Droid
} // namespace Elastos