
#include "elastos/droid/systemui/CBatteryMeterView.h"

namespace Elastos {
namespace Droid {
namespace SystemUI {

//===============================================================
// CBatteryMeterView::BatteryTracker::
//===============================================================

Int32 CBatteryMeterView::BatteryTracker::UNKNOWN_LEVEL = -1;

CBatteryMeterView::BatteryTracker::BatteryTracker()
{
    // current battery status
    mLevel = UNKNOWN_LEVEL;
    mPlugType = 0;
    mPlugged = FALSE;
    mHealth = 0;
    mStatus = 0;
    mVoltage = 0;
    mTemperature = 0;
    mTestmode = FALSE;
}

ECode CBatteryMeterView::BatteryTracker::OnReceive(
    /* [in] */ IContext* context,
    /* [in] */ IIntent* intent)
{
    String action;
    intent->GetAction(&action);
    if (action.Equals(IIntent::ACTION_BATTERY_CHANGED)) {
        Boolean bExt = FALSE;
        if (testmode && ! (intent->GetBooleanExtra("testmode", FALSE, &bExt), bExt))
            return NOERROR;

        Int32 ext1 = 0, ext2 = 0;
        intent->GetInt32Extra(IBatteryManager::EXTRA_LEVEL, 0, &ext1);
        intent->GetInt32Extra(IBatteryManager::EXTRA_SCALE, 100, &ext2);
        level = (Int32)(100f * ext1 / ext2);

        intent->GetInt32Extra(IBatteryManager::EXTRA_PLUGGED, 0, &plugType);
        plugged = plugType != 0;
        intent->GetInt32Extra(IBatteryManager::EXTRA_HEALTH,
                IBatteryManager::BATTERY_HEALTH_UNKNOWN, &health);
        intent->GetInt32Extra(IBatteryManager::EXTRA_STATUS,
                IBatteryManager::BATTERY_STATUS_UNKNOWN, &status);
        intent->GetStringExtra(IBatteryManager::EXTRA_TECHNOLOGY, &technology);
        intent->GetInt32Extra(IBatteryManager::EXTRA_VOLTAGE, 0, &voltage);
        intent->GetInt32Extra(IBatteryManager::EXTRA_TEMPERATURE, 0, &temperature);

        SetContentDescription(
                context->GetString(R::string::accessibility_battery_level, level));
        PostInvalidate();
    }
    else if (action.Equals(ACTION_LEVEL_TEST)) {
        testmode = TRUE;
        Post(new Runnable());
    }
    return NOERROR;
}

//===============================================================
// CBatteryMeterView::Runnable_1::
//===============================================================
CBatteryMeterView::Runnable_1::Runnable_1()
{
    mCurLevel = 0;
    mIncr = 1;
    mSaveLevel = level;
    mSavePlugged = plugType;
    mDummy = new Intent(IIntent::ACTION_BATTERY_CHANGED);
}

ECode CBatteryMeterView::Runnable_1::Run()
{
    if (mCurLevel < 0) {
        testmode = FALSE;
        mDummy->PutExtra("level", saveLevel);
        mDummy->PutExtra("plugged", savePlugged);
        mDummy->PutExtra("testmode", FALSE);
    }
    else {
        mDummy->PutExtra("level", curLevel);
        mDummy->PutExtra("plugged", incr > 0 ? IBatteryManager::BATTERY_PLUGGED_AC : 0);
        mDummy->PutExtra("testmode", TRUE);
    }
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    ctx->SendBroadcast(mDummy);

    if (!testmode) return NOERROR;

    curLevel += incr;
    if (curLevel == 100) {
        incr *= -1;
    }
    PostDelayed(this, 200);
    return NOERROR;
}

//===============================================================
// CBatteryMeterView::
//===============================================================
String CBatteryMeterView::TAG;// = BatteryMeterView.class.getSimpleName();
String CBatteryMeterView::ACTION_LEVEL_TEST = String("com.android.systemui.BATTERY_LEVEL_TEST");

Boolean CBatteryMeterView::ENABLE_PERCENT = TRUE;
Boolean CBatteryMeterView::SINGLE_DIGIT_PERCENT = FALSE;
Boolean CBatteryMeterView::SHOW_100_PERCENT = FALSE;

Int32 CBatteryMeterView::FULL = 96;

Float CBatteryMeterView::BOLT_LEVEL_THRESHOLD = 0.3f;  // opaque bolt below this fraction

CAR_OBJECT_IMPL(CBatteryMeterView)

CAR_INTERFACE_IMPL_2(CBatteryMeterView, View, IDemoMode, IBatteryStateChangeCallback)

CBatteryMeterView::CBatteryMeterView()
{
    mShowPercent = TRUE;

    CPath::New((IPath**)&mBoltPath);

    CRectF::New((IRectF**)&mFrame);
    CRectF::New((IRectF**)&mButtonFrame);
    CRectF::New((IRectF**)&mBoltFrame);

    CPath::New((IPath**)&mShapePath);
    CPath::New((IPath**)&mClipPath);
    CPath::New((IPath**)&mTextPath);

    mPowerSaveEnabled = FALSE;
//    mTracker = new BatteryTracker();

    mDemoMode = FALSE;
//    mDemoTracker = new BatteryTracker();
}

ECode CBatteryMeterView::OnAttachedToWindow()
{
    View::OnAttachedToWindow();

    AutoPtr<IIntentFilter> filter = new IntentFilter();
    filter->AddAction(IIntent::ACTION_BATTERY_CHANGED);
    filter->AddAction(ACTION_LEVEL_TEST);
    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
    AutoPtr<IIntent> sticky;
    ctx->RegisterReceiver(mTracker, filter, (IIntent**)&sticky);
    if (sticky != NULL) {
        // preload the battery level
        AutoPtr<IContext> c;
        GetContext((IContext**)&c);
        mTracker->OnReceive(c, sticky);
    }
    mBatteryController->AddStateChangedCallback(this);
    return NOERROR;
}

ECode CBatteryMeterView::OnDetachedFromWindow()
{
    View::OnDetachedFromWindow();

    AutoPtr<IContext> ctx;
    GetContext((IContext**)&ctx);
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
    View::constructor(context, attrs, defStyle);

    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<ITypedArray> atts;
    context->ObtainStyledAttributes(attrs, R::styleable::BatteryMeterView,
            defStyle, 0, (ITypedArray**)&atts);
    Int32 clr = 0;
    res->GetColor(R::color::batterymeter_frame_color, &clr);
    Int32 frameColor = 0;
    atts->GetColor(R::styleable::BatteryMeterView_frameColor, clr, &frameColor);
    AutoPtr<ITypedArray> levels;
    res->ObtainTypedArray(R::array::batterymeter_color_levels, (ITypedArray**)&levels);
    AutoPtr<ITypedArray> colors;
    res->ObtainTypedArray(R::array::batterymeter_color_values, (ITypedArray**)&colors);

    Int32 N = levels->GetLength();
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
    Settings::System::GetInt32(
            cr, "status_bar_show_battery_percent", 0, &iPercent);
    mShowPercent = ENABLE_PERCENT && 0 != iPercent;
    context->GetString(R::string::battery_meter_very_low_overlay_symbol, &mWarningString);
    AutoPtr<IResources> r;
    mContext->GetResources((IResources**)&r);
    r->GetInteger(
            R::integer::config_criticalBatteryWarningLevel, &mCriticalLevel);
    r->GetFraction(
            R::fraction::battery_button_height_fraction, 1, 1, &mButtonHeightFraction);
    r->GetFraction(
            R::fraction::battery_subpixel_smoothing_left, 1, 1, &mSubpixelSmoothingLeft);
    r->GetFraction(
            R::fraction::battery_subpixel_smoothing_right, 1, 1, &mSubpixelSmoothingRight);

    mFramePaint = new Paint(IPaint::ANTI_ALIAS_FLAG);
    mFramePaint->SetColor(frameColor);
    mFramePaint->SetDither(TRUE);
    mFramePaint->SetStrokeWidth(0);
    mFramePaint->SetStyle(IPaint::Style::FILL_AND_STROKE);

    mBatteryPaint = new Paint(IPaint::ANTI_ALIAS_FLAG);
    mBatteryPaint->SetDither(TRUE);
    mBatteryPaint->SetStrokeWidth(0);
    mBatteryPaint->SetStyle(IPaint::Style::FILL_AND_STROKE);

    mTextPaint = new Paint(IPaint::ANTI_ALIAS_FLAG);
    AutoPtr<ITypefaceHelper> tfacehlp;
    CTransitionUtils::AcquireSingleton((ITypefaceHelper**)&tfacehlp);
    AutoPtr<ITypeface> font;
    tfacehlp->Create("sans-serif-condensed", ITypeface::BOLD, (ITypeface**)&font);
    mTextPaint->SetTypeface(font);
    mTextPaint->SetTextAlign(IPaint::Align::CENTER);

    mWarningTextPaint = new Paint(IPaint::ANTI_ALIAS_FLAG);
    mWarningTextPaint->SetColor((*mColors)[1]);
    tfacehlp->Create("sans-serif", ITypeface::BOLD, (ITypeface**)&font);
    mWarningTextPaint->SetTypeface(font);
    mWarningTextPaint->SetTextAlign(IPaint::Align::CENTER);

    AutoPtr<IResources> rClr;
    GetResources((IResources**)&rClr);
    rClr->GetColor(R::color::batterymeter_charge_color, &mChargeColor);

    mBoltPaint = new Paint(IPaint::ANTI_ALIAS_FLAG);
    Int32 bolt_clr = 0;
    res->GetColor(R::color::batterymeter_bolt_color, &bolt_clr)
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
    Invalidate();
    return NOERROR;
}

AutoPtr<ArrayOf<Float> > CBatteryMeterView::LoadBoltPoints(
    /* [in] */ IResources* res)
{
    AutoPtr<ArrayOf<Int32> > pts;
    res->GetIntArray(R::array::batterymeter_bolt_points, (ArrayOf<Int32>**)&pts);
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
    mWarningTextHeight = -mWarningTextPaint->GetFontMetrics()->mAscent;
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
    AutoPtr<IBatteryTracker> tracker = mDemoMode ? mDemoTracker : mTracker;
    Int32 level = tracker->mLevel;

    if (level == BatteryTracker::UNKNOWN_LEVEL) return NOERROR;

    Float drawFrac = (float) level / 100f;
    Int32 pt = 0;
    GetPaddingTop(&pt);
    Int32 pl = 0;
    GetPaddingLeft(&pl);
    Int32 pr = 0;
    GetPaddingRight(&pr);
    Int32 pb = 0;
    GetPaddingBottom(&pb);
    Int32 height = mHeight - pt - pb;
    Int32 width = mWidth - pl - pr;

    Int32 buttonHeight = (Int32) (height * mButtonHeightFraction);

    mFrame->Set(0, 0, width, height);
    mFrame->Offset(pl, pt);

    // button-frame: area above the battery body
    mButtonFrame->Set(
            mFrame->mLeft + Elastos::Core::Math::Round(width * 0.25f),
            mFrame->mTop,
            mFrame->mRight - Elastos::Core::Math::Round(width * 0.25f),
            mFrame->mTop + buttonHeight);

    mButtonFrame->mTop += mSubpixelSmoothingLeft;
    mButtonFrame->mLeft += mSubpixelSmoothingLeft;
    mButtonFrame->mRight -= mSubpixelSmoothingRight;

    // frame: battery body area
    mFrame->mTop += buttonHeight;
    mFrame->mLeft += mSubpixelSmoothingLeft;
    mFrame->mTop += mSubpixelSmoothingLeft;
    mFrame->mRight -= mSubpixelSmoothingRight;
    mFrame->mBottom -= mSubpixelSmoothingRight;

    // set the battery charging color
    mBatteryPaint->SetColor(tracker->mPlugged ? mChargeColor : GetColorForLevel(level));

    if (level >= FULL) {
        drawFrac = 1f;
    }
    else if (level <= mCriticalLevel) {
        drawFrac = 0f;
    }

    Float levelTop = drawFrac == 1f ? mButtonFrame.top
            : (mFrame->mTop + (mFrame->Height() * (1f - drawFrac)));

    // define the battery shape
    mShapePath->Reset();
    mShapePath->MoveTo(mButtonFrame->mLeft, mButtonFrame->mTop);
    mShapePath->LineTo(mButtonFrame->mRight, mButtonFrame->mTop);
    mShapePath->LineTo(mButtonFrame->mRight, mFrame->mTop);
    mShapePath->LineTo(mFrame->mRight, mFrame->mTop);
    mShapePath->LineTo(mFrame->mRight, mFrame->mBottom);
    mShapePath->LineTo(mFrame->mLeft, mFrame->mBottom);
    mShapePath->LineTo(mFrame->mLeft, mFrame->mTop);
    mShapePath->LineTo(mButtonFrame->mLeft, mFrame->mTop);
    mShapePath->LineTo(mButtonFrame->mLeft, mButtonFrame->mTop);

    if (tracker->mPlugged) {
        // define the bolt shape
        Float bl = mFrame->mLeft + mFrame->GetWidth() / 4.5f;
        Float bt = mFrame->mTop + mFrame->GetHeight() / 6f;
        Float br = mFrame->mRight - mFrame->GetWidth() / 7f;
        Float bb = mFrame->mBottom - mFrame->GetHeight() / 10f;
        if (mBoltFrame->mLeft != bl || mBoltFrame->mTop != bt
                || mBoltFrame->mRight != br || mBoltFrame->mBottom != bb) {
            mBoltFrame->Set(bl, bt, br, bb);
            mBoltPath->Reset();
            mBoltPath->MoveTo(
                    mBoltFrame->mLeft + (*mBoltPoints)[0] * mBoltFrame->GetWidth(),
                    mBoltFrame->mTop + (*mBoltPoints)[1] * mBoltFrame->GetHeight());
            for (Int32 i = 2; i < mBoltPoints->GetLength(); i += 2) {
                mBoltPath->LineTo(
                        mBoltFrame->mLeft + (*mBoltPoints)[i] * mBoltFrame->GetWidth(),
                        mBoltFrame->mTop + (*mBoltPoints)[i + 1] * mBoltFrame->GetHeight());
            }
            mBoltPath->LineTo(
                    mBoltFrame->mLeft + (*mBoltPoints)[0] * mBoltFrame->GetWidth(),
                    mBoltFrame->mTop + (*mBoltPoints)[1] * mBoltFrame->GetHeight());
        }

        Float boltPct = (mBoltFrame->mBottom - levelTop) / (mBoltFrame->mBottom - mBoltFrame->mTop);
        boltPct = Math::Min(Math::Max(boltPct, 0), 1);
        if (boltPct <= BOLT_LEVEL_THRESHOLD) {
            // draw the bolt if opaque
            c->DrawPath(mBoltPath, mBoltPaint);
        }
        else {
            // otherwise cut the bolt out of the overall shape
            mShapePath->Op(mBoltPath, Path::Op::DIFFERENCE);
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
        mTextHeight = -mTextPaint->GetFontMetrics()->mAscent;
        pctText = String::ValueOf(SINGLE_DIGIT_PERCENT ? (level/10) : level);
        pctX = mWidth * 0.5f;
        pctY = (mHeight + mTextHeight) * 0.47f;
        pctOpaque = levelTop > pctY;
        if (!pctOpaque) {
            mTextPath->Reset();
            mTextPaint->GetTextPath(pctText, 0, pctText->GetLength(), pctX, pctY, mTextPath);
            // cut the percentage text out of the overall shape
            mShapePath->Op(mTextPath, Path::Op::DIFFERENCE);
        }
    }

    // draw the battery shape background
    c->DrawPath(mShapePath, mFramePaint);

    // draw the battery shape, clipped to charging level
    mFrame->mTop = levelTop;
    mClipPath->Reset();
    mClipPath->AddRect(mFrame,  Path::Direction::CCW);
    mShapePath->Op(mClipPath, Path::Op::INTERSECT);
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
        PostInvalidate();
    }
    else if (mDemoMode && command.Equals(COMMAND_BATTERY)) {
       String level;
       args->GetString("level", &level);
       String plugged;
       args->GetString("plugged", &plugged);
       if (level != NULL) {
           mDemoTracker->mLevel = Elastos::Core::Math::Min(Elastos::Core::Math::Max(StringUtils::ParseInt32(level), 0), 100);
       }
       if (plugged != NULL) {
           mDemoTracker->mPlugged = StringUtils::ParseBoolean(plugged);
       }
       PostInvalidate();
    }
    return NOERROR;
}

} // namespace SystemUI
} // namepsace Droid
} // namespace Elastos