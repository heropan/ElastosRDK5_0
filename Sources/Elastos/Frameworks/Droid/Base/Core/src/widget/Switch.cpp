
#include <elastos/core/Math.h>
#include "graphics/Typeface.h"
#include "text/CStaticLayout.h"
#include "text/TextUtils.h"
#include "text/CTextPaint.h"
#include "text/CLayoutHelper.h"
#include "text/method/CAllCapsTransformationMethod.h"
#include "view/CViewConfiguration.h"
#include "view/CMotionEvent.h"
#include "widget/Switch.h"
#include "content/res/CTypedArray.h"
#include "content/res/CAssetManager.h"

using Elastos::Core::CStringWrapper;
using Elastos::Core::StringBuilder;
using Elastos::Droid::Content::Res::ICompatibilityInfo;
using Elastos::Droid::Graphics::Typeface;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Text::CTextPaint;
using Elastos::Droid::Text::IStaticLayout;
using Elastos::Droid::Text::CStaticLayout;
using Elastos::Droid::Text::ILayoutHelper;
using Elastos::Droid::Text::CLayoutHelper;
using Elastos::Droid::Text::ALIGN_NORMAL;
using Elastos::Droid::Text::Method::CAllCapsTransformationMethod;
using Elastos::Droid::Text::Method::IAllCapsTransformationMethod;
using Elastos::Droid::View::EIID_IView;
using Elastos::Droid::View::IGravity;
using Elastos::Droid::View::CViewConfiguration;
using Elastos::Droid::View::CMotionEvent;
using Elastos::Droid::Content::Res::CTypedArray;
using Elastos::Droid::Content::Res::CAssetManager;

namespace Elastos {
namespace Droid {
namespace Widget {

static AutoPtr<ArrayOf<Int32> > InitArray()
{
    AutoPtr< ArrayOf<Int32> > temp = ArrayOf<Int32>::Alloc(1);
    (*temp)[0] = R::attr::state_checked;
    return temp;
}

const String Switch::SWITCH_NAME = String("Switch");
const Int32 Switch::TOUCH_MODE_IDLE;
const Int32 Switch::TOUCH_MODE_DOWN;
const Int32 Switch::TOUCH_MODE_DRAGGING;
const Int32 Switch::SANS;
const Int32 Switch::SERIF;
const Int32 Switch::MONOSPACE;
const AutoPtr<ArrayOf<Int32> > Switch::CHECKED_STATE_SET = InitArray();

Switch::Switch()
    : mThumbTextPadding(0)
    , mSwitchMinWidth(0)
    , mSwitchPadding(0)
    , mTouchMode(0)
    , mTouchSlop(0)
    , mTouchX(0.0f)
    , mTouchY(0.0f)
    , mMinFlingVelocity(0)
    , mThumbPosition(0.0f)
    , mSwitchWidth(0)
    , mSwitchHeight(0)
    , mThumbWidth(0)
    , mSwitchLeft(0)
    , mSwitchTop(0)
    , mSwitchRight(0)
    , mSwitchBottom(0)

{
    mVelocityTracker = VelocityTracker::Obtain();
    CRect::New((IRect**)&mTempRect);
}

Switch::Switch(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    : CompoundButton(context,attrs, defStyle)
{
    mVelocityTracker = VelocityTracker::Obtain();
    CRect::New((IRect**)&mTempRect);
    InitImpl(context, attrs, defStyle);
}

ECode Switch::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    CompoundButton::Init(context, attrs, defStyle);
    InitImpl(context, attrs, defStyle);
    return NOERROR;
}

ECode Switch::InitImpl(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    PEL("Switch::InitImpl")
    CTextPaint::New(IPaint::ANTI_ALIAS_FLAG, (ITextPaint**)&mTextPaint);
    AutoPtr<IResources> res = GetResources();
    AutoPtr<IDisplayMetrics> metrics;
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);
    Float density;
    metrics->GetDensity(&density);
    mTextPaint->SetDensity(density);
    AutoPtr<ICompatibilityInfo> info;
    res->GetCompatibilityInfo((ICompatibilityInfo**)&info);
    Float applicationScale = 0;
    info->GetApplicationScale(&applicationScale);
    mTextPaint->SetCompatibilityScaling(applicationScale);

    AutoPtr< ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::Switch),
            ARRAY_SIZE(R::styleable::Switch));
    AutoPtr<ITypedArray> a;
    FAIL_RETURN(context->ObtainStyledAttributes(attrs, attrIds, defStyle, 0, (ITypedArray**)&a));

    a->GetDrawable(R::styleable::Switch_thumb, (IDrawable**)&mThumbDrawable);
    a->GetDrawable(R::styleable::Switch_track, (IDrawable**)&mTrackDrawable);
    a->GetText(R::styleable::Switch_textOn, (ICharSequence**)&mTextOn);
    a->GetText(R::styleable::Switch_textOff, (ICharSequence**)&mTextOff);

    a->GetDimensionPixelSize(R::styleable::Switch_thumbTextPadding, 0, &mThumbTextPadding);
    a->GetDimensionPixelSize(R::styleable::Switch_switchMinWidth, 0, &mSwitchMinWidth);
    a->GetDimensionPixelSize(R::styleable::Switch_switchPadding, 0, &mSwitchPadding);

    Int32 appearance = 0;
    a->GetResourceId(R::styleable::Switch_switchTextAppearance, 0, &appearance);

    if(appearance != 0) {
        SetSwitchTextAppearance(context, appearance);
    }
    a->Recycle();

    AutoPtr<CViewConfiguration> config = CViewConfiguration::Get(context);
    config->GetScaledTouchSlop(&mTouchSlop);
    config->GetScaledMinimumFlingVelocity(&mMinFlingVelocity);

    RefreshDrawableState();
    SetChecked(IsChecked());
    return NOERROR;
}

ECode Switch::SetSwitchTextAppearance(
    /* [in] */ IContext* context,
    /* [in] */ Int32 resid)
{
    AutoPtr< ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::TextAppearance),
            ARRAY_SIZE(R::styleable::TextAppearance));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(resid, attrIds, (ITypedArray**)&a);

    AutoPtr<IColorStateList> colors;
    a->GetColorStateList(R::styleable::TextAppearance_textColor, (IColorStateList**)&colors);

    if (colors) {
        mTextColors = colors;
    } else {
        mTextColors = GetTextColors();
    }

    Int32 ts = 0;
    a->GetDimensionPixelSize(R::styleable::TextAppearance_textSize, 0, &ts);
    if (ts != 0) {
        Float textSize = 0;
        mTextPaint->GetTextSize(&textSize);
        if (ts != textSize) {
            mTextPaint->SetTextSize(ts);
            RequestLayout();
        }
    }

    Int32 typefaceIndex = 0, styleIndex = 0;

    a->GetInt32(R::styleable::TextAppearance_typeface, -1, &typefaceIndex);
    a->GetInt32(R::styleable::TextAppearance_textStyle, -1, &styleIndex);

    SetSwitchTypefaceByIndex(typefaceIndex, styleIndex);

    Boolean allCaps = FALSE;
    a->GetBoolean(R::styleable::TextAppearance_textAllCaps, FALSE, &allCaps);
    if (allCaps) {
        AutoPtr<IAllCapsTransformationMethod> atm;
        CAllCapsTransformationMethod::New(GetContext(), (IAllCapsTransformationMethod**)&atm);
        mSwitchTransformationMethod = IAllCapsTransformationMethod::Probe(atm.Get());
        mSwitchTransformationMethod->SetLengthChangesAllowed(TRUE);
    } else {
        mSwitchTransformationMethod = NULL;
    }

    a->Recycle();

    return NOERROR;
}

void Switch::SetSwitchTypefaceByIndex(
    /* [in] */ Int32 typefaceIndex,
    /* [in] */ Int32 styleIndex)
{
    AutoPtr<ITypeface> tf;
    switch (typefaceIndex) {
        case SANS:
            tf = Typeface::SANS_SERIF;
            break;
        case SERIF:
            tf = Typeface::SERIF;
            break;
        case MONOSPACE:
            tf = Typeface::MONOSPACE;
            break;
    }

    SetSwitchTypeface(tf, styleIndex);
}

ECode Switch::SetSwitchTypeface(
    /* [in] */ ITypeface* tf,
    /* [in] */ Int32 style)
{
    if (style > 0) {
        AutoPtr<ITypeface> typeface;
        if (!tf) {
            Typeface::DefaultFromStyle(style, (ITypeface**)&typeface);
            tf = typeface;
        } else {
            Typeface::Create(tf, style, (ITypeface**)&typeface);
            tf = typeface;
        }

        SetSwitchTypeface(tf);
        Int32 sty = 0;
        tf->GetStyle(&sty);
        Int32 typefaceStyle = tf ? sty : 0;
        Int32 need = style & ~typefaceStyle;
        mTextPaint->SetFakeBoldText((need & ITypeface::BOLD) != 0);
        mTextPaint->SetTextSkewX((need & ITypeface::BOLD) != 0 ? -0.25f : 0);
    } else {
        mTextPaint->SetFakeBoldText(FALSE);
        mTextPaint->SetTextSkewX(0);
        SetSwitchTypeface(tf);
    }
    return NOERROR;
}

ECode Switch::SetSwitchTypeface(
    /* [in] */ ITypeface* tf)
{
    AutoPtr<ITypeface> face;
    mTextPaint->GetTypeface((ITypeface**)&face);
    if (tf != face) {
        mTextPaint->SetTypeface(tf);

        RequestLayout();
        Invalidate();
    }
    return NOERROR;
}

ECode Switch::SetSwitchPadding(
    /* [in] */ Int32 pixels)
{
    mSwitchPadding = pixels;
    RequestLayout();
    return NOERROR;
}

Int32 Switch::GetSwitchPadding()
{
    return mSwitchPadding;
}

ECode Switch::SetSwitchMinWidth(
    /* [in] */ Int32 pixels)
{
    mSwitchMinWidth = pixels;
    RequestLayout();
    return NOERROR;
}

Int32 Switch::GetSwitchMinWidth()
{
    return mSwitchMinWidth;
}

ECode Switch::SetThumbTextPadding(
    /* [in] */ Int32 pixels)
{
    mThumbTextPadding = pixels;
    RequestLayout();
    return NOERROR;
}

Int32 Switch::GetThumbTextPadding()
{
    return mThumbTextPadding;
}

ECode Switch::SetTrackDrawable(
    /* [in] */ IDrawable* track)
{
    mTrackDrawable = track;
    RequestLayout();
    return NOERROR;
}

ECode Switch::SetTrackResource(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> context = GetContext();
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDrawable> drawable;
    res->GetDrawable(resId, (IDrawable**)&drawable);
    SetTrackDrawable(drawable);
    return NOERROR;
}

AutoPtr<IDrawable> Switch::GetTrackDrawable()
{
    return mTrackDrawable;
}

ECode Switch::SetThumbDrawable(
    /* [in] */ IDrawable* thumb)
{
    mThumbDrawable = thumb;
    RequestLayout();
    return NOERROR;
}

ECode Switch::SetThumbResource(
    /* [in] */ Int32 resId)
{
    AutoPtr<IContext> context = GetContext();
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    AutoPtr<IDrawable> drawable;
    res->GetDrawable(resId, (IDrawable**)&drawable);
    SetThumbDrawable(drawable);
    return NOERROR;
}

AutoPtr<IDrawable> Switch::GetThumbDrawable()
{
    return mThumbDrawable;
}

AutoPtr<ICharSequence> Switch::GetTextOn()
{
    return mTextOn;
}

ECode Switch::SetTextOn(
    /* [in] */ ICharSequence* textOn)
{
    mTextOn = textOn;
    RequestLayout();
    return NOERROR;
}

AutoPtr<ICharSequence> Switch::GetTextOff()
{
    return mTextOff;
}

ECode Switch::SetTextOff(
    /* [in] */ ICharSequence* textOff)
{
    mTextOff = textOff;
    RequestLayout();
    return NOERROR;
}

void Switch::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    if (!mOnLayout) {
        mOnLayout = MakeLayout(mTextOn);
    }
    if (!mOffLayout) {
        mOffLayout = MakeLayout(mTextOff);
    }
    Boolean padding = FALSE;
    mTrackDrawable->GetPadding(mTempRect, &padding);

    Int32 onWidth = 0, offWidth = 0;
    mOnLayout->GetWidth(&onWidth);
    mOffLayout->GetWidth(&offWidth);
    Int32 maxTextWidth = Elastos::Core::Math::Max(onWidth, offWidth);
    AutoPtr<CRect> rect = (CRect*)mTempRect.Get();
    Int32 switchWidth = Elastos::Core::Math::Max(mSwitchMinWidth,
        maxTextWidth * 2 + mThumbTextPadding * 4 + rect->mLeft + rect->mRight);
    Int32 switchHeight = 0;
    mTrackDrawable->GetIntrinsicHeight(&switchHeight);
    mThumbWidth = maxTextWidth + mThumbTextPadding * 2;

    mSwitchWidth = switchWidth;
    mSwitchHeight = switchHeight;
    CompoundButton::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    Int32 measuredHeight = GetMeasuredHeight();
    if (measuredHeight < switchHeight) {
        SetMeasuredDimension(GetMeasuredWidthAndState(), switchHeight);
    }
}

ECode Switch::OnPopulateAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    CompoundButton::OnPopulateAccessibilityEvent(event);
    AutoPtr<ILayout> layout = IsChecked() ? mOnLayout : mOffLayout;
    AutoPtr<ICharSequence> csq;
    layout->GetText((ICharSequence**)&csq);
    if (layout && !TextUtils::IsEmpty(csq)) {
        AutoPtr<IObjectContainer> container;
        event->GetText((IObjectContainer**)&container);
        container->Add(csq);
    }
    return NOERROR;
}

AutoPtr<ILayout> Switch::MakeLayout(
    /* [in] */ ICharSequence* text)
{
    AutoPtr<ICharSequence> transformed;
    if (mSwitchTransformationMethod) {
        mSwitchTransformationMethod->GetTransformation(text, (IView*)this->Probe(EIID_IView), (ICharSequence**)&transformed);
    } else {
        transformed = text;
    }

    AutoPtr<ILayoutHelper> helper;
    CLayoutHelper::AcquireSingleton((ILayoutHelper**)&helper);
    Float res = 0;
    helper->GetDesiredWidth(transformed, mTextPaint, &res);

    AutoPtr<IStaticLayout> layout;
    CStaticLayout::New(transformed, mTextPaint, (Int32)(Elastos::Core::Math::Ceil(res)), ALIGN_NORMAL, 1.f, 0, TRUE, (IStaticLayout**)&layout);
    return layout;
}

Boolean Switch::HitThumb(
    /* [in] */ Float x,
    /* [in] */ Float y)
{
    Boolean res = FALSE;
    mThumbDrawable->GetPadding(mTempRect, &res);
    Int32 thumbTop = mSwitchTop - mTouchSlop;
    Int32 thumbLeft = mSwitchLeft + (Int32)(mThumbPosition + 0.5f) - mTouchSlop;
    AutoPtr<CRect> rect = (CRect*)mTempRect.Get();
    Int32 thumbRight = thumbLeft + mThumbWidth + rect->mLeft + rect->mRight + mTouchSlop;
    Int32 thumbBottom = mSwitchBottom + mTouchSlop;
    return x > thumbLeft && x < thumbRight && y > thumbTop && y < thumbBottom;
}

Boolean Switch::OnTouchEvent(
    /* [in] */ IMotionEvent* ev)
{
    mVelocityTracker->AddMovement(ev);
    Int32 action = 0;
    ev->GetActionMasked(&action);

    switch (action) {
        case IMotionEvent::ACTION_DOWN:
        {
            Float x = 0, y = 0;
            ev->GetX(&x);
            ev->GetY(&y);
            if (IsEnabled() && HitThumb(x, y)) {
                mTouchMode = TOUCH_MODE_DOWN;
                mTouchX = x;
                mTouchY = y;
            }
            break;
        }

        case IMotionEvent::ACTION_MOVE:
        {
            switch (mTouchMode) {
                case TOUCH_MODE_IDLE:
                {
                    break;
                }

                case TOUCH_MODE_DOWN:
                {
                    Float x = 0, y = 0;
                    ev->GetX(&x);
                    ev->GetY(&y);
                    if (Elastos::Core::Math::Abs(x - mTouchX) > mTouchSlop ||
                        Elastos::Core::Math::Abs(y - mTouchY) > mTouchSlop) {
                        mTouchMode = TOUCH_MODE_DRAGGING;
                        GetParent()->RequestDisallowInterceptTouchEvent(TRUE);
                        mTouchX = x;
                        mTouchY = y;
                        return TRUE;
                    }
                    break;
                }

                case TOUCH_MODE_DRAGGING:
                {
                    Float x = 0, dx = 0;
                    ev->GetX(&x);
                    dx = x - mTouchX;
                    Float newPos = Elastos::Core::Math::Max(0, Elastos::Core::Math::Min((Int32)(mThumbPosition + dx), GetThumbScrollRange()));
                    if (newPos != mThumbPosition) {
                        mThumbPosition = newPos;
                        mTouchX = x;
                        Invalidate();
                    }
                    return TRUE;
                }
            }
            break;
        }

        case IMotionEvent::ACTION_UP:
        case IMotionEvent::ACTION_CANCEL:
        {
            if (mTouchMode == TOUCH_MODE_DRAGGING) {
                StopDrag(ev);
                return TRUE;
            }
            mTouchMode = TOUCH_MODE_IDLE;
            mVelocityTracker->Clear();
            break;
        }
    }

    return CompoundButton::OnTouchEvent(ev);
}

void Switch::CancelSuperTouch(
    /* [in] */ IMotionEvent* ev)
{
    AutoPtr<CMotionEvent> cev = (CMotionEvent*)ev;
    AutoPtr<CMotionEvent> cancle;
    CMotionEvent::Obtain(cev, (CMotionEvent**)&cancle);
    cancle->SetAction(IMotionEvent::ACTION_CANCEL);
    CompoundButton::OnTouchEvent(cancle);
    cancle->Recycle();
}

void Switch::StopDrag(
    /* [in] */ IMotionEvent* ev)
{
    mTouchMode = TOUCH_MODE_IDLE;
    Int32 action = 0;
    ev->GetAction(&action);
    Boolean commitChange = FALSE;
    commitChange = action == IMotionEvent::ACTION_UP && IsEnabled();

    CancelSuperTouch(ev);

    if (commitChange) {
        Boolean newState = FALSE;
        mVelocityTracker->ComputeCurrentVelocity(1000);
        Float xvel;
        mVelocityTracker->GetXVelocity(&xvel);

        if (Elastos::Core::Math::Abs(xvel) > mMinFlingVelocity) {
            newState = IsLayoutRtl() ? (xvel < 0) : (xvel > 0);
        } else {
            newState = GetTargetCheckedState();
        }
        AnimateThumbToCheckedState(newState);
    } else {
        AnimateThumbToCheckedState(IsChecked());
    }
}

void Switch::AnimateThumbToCheckedState(
    /* [in] */ Boolean newCheckedState)
{
    SetChecked(newCheckedState);
}

Boolean Switch::GetTargetCheckedState()
{
    if (IsLayoutRtl()) {
        return mThumbPosition <= GetThumbScrollRange() / 2;
    } else {
        return mThumbPosition >= GetThumbScrollRange() / 2;
    }
}

void Switch::SetThumbPosition(
    /* [in] */ Boolean checked)
{
    if (IsLayoutRtl()) {
        mThumbPosition = checked ? 0 : GetThumbScrollRange();
    } else {
        mThumbPosition = checked ? GetThumbScrollRange() : 0;
    }
}

ECode Switch::SetChecked(
    /* [in] */ Boolean checked)
{
    CompoundButton::SetChecked(checked);
    SetThumbPosition(IsChecked());
    Invalidate();
    return NOERROR;
}

void Switch::OnLayout(
    /* [in] */ Boolean changed,
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    PEL("Switch::OnLayout")
    CompoundButton::OnLayout(changed, left, top, right, bottom);
    SetThumbPosition(IsChecked());
    Int32 switchRight = 0, switchLeft = 0;

    if (IsLayoutRtl()) {
        switchLeft = GetPaddingLeft();
        switchRight = switchLeft + mSwitchWidth;
    } else {
        switchRight = GetWidth() - GetPaddingRight();
        switchLeft = switchRight - mSwitchWidth;
    }

    Int32 switchTop = 0, switchBottom = 0;
    switch (GetGravity() & IGravity::VERTICAL_GRAVITY_MASK) {
        default:
        case IGravity::TOP:
            switchTop = GetPaddingTop();
            switchBottom = switchTop + mSwitchHeight;
            break;

        case IGravity::CENTER_VERTICAL:
            switchTop = (GetPaddingTop() + GetHeight() - GetPaddingBottom()) / 2 - mSwitchHeight / 2;
            switchBottom = switchTop + mSwitchHeight;
            break;

        case IGravity::BOTTOM:
            switchBottom = GetHeight() - GetPaddingBottom();
            switchTop = switchBottom - mSwitchHeight;
            break;
    }

    mSwitchLeft = switchLeft;
    mSwitchTop = switchTop;
    mSwitchBottom = switchBottom;
    mSwitchRight = switchRight;
}

void Switch::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    CompoundButton::OnDraw(canvas);

    Int32 switchLeft = mSwitchLeft;
    Int32 switchTop = mSwitchTop;
    Int32 switchRight = mSwitchRight;
    Int32 switchBottom = mSwitchBottom;

    mTrackDrawable->SetBounds(switchLeft, mSwitchTop, mSwitchRight, mSwitchBottom);
    mTrackDrawable->Draw(canvas);
    Int32 save = 0;
    canvas->Save(&save);

    Boolean padding = FALSE;
    mTrackDrawable->GetPadding(mTempRect, &padding);
    AutoPtr<CRect> rect = (CRect*)mTempRect.Get();
    Int32 switchInnerLeft = switchLeft + rect->mLeft;
    Int32 switchInnerTop = switchTop + rect->mTop;
    Int32 switchInnerRight = switchRight - rect->mRight;
    Int32 switchInnerBottom = switchBottom - rect->mBottom;
    canvas->ClipRect(switchInnerLeft, switchInnerTop, switchInnerRight, switchInnerBottom, &padding);

    mThumbDrawable->GetPadding(mTempRect, &padding);
    Int32 thumbPos = (Int32)(mThumbPosition + 0.5f);
    Int32 thumbLeft = switchInnerLeft - rect->mLeft + thumbPos;
    Int32 thumbRight = switchInnerLeft + thumbPos + mThumbWidth + rect->mRight;

    mThumbDrawable->SetBounds(thumbLeft, switchTop, thumbRight, switchBottom);
    mThumbDrawable->Draw(canvas);

    if (mTextColors) {
        Int32 state = 0, color = 0;
        mTextColors->GetDefaultColor(&color);
        mTextColors->GetColorForState(GetDrawableState(), color, &state);
        mTextPaint->SetColor(state);
    }

    AutoPtr<CTextPaint> paint = (CTextPaint*)mTextPaint.Get();
    paint->mDrawableState = GetDrawableState();

    AutoPtr<ILayout> switchText = GetTargetCheckedState() ? mOnLayout : mOffLayout;
    if (switchText) {
        Int32 width = 0, height = 0;
        switchText->GetWidth(&width);
        switchText->GetHeight(&height);
        canvas->Translate((thumbLeft + thumbRight) / 2 - width / 2,
            (switchInnerTop + switchInnerBottom) / 2 - height / 2);
        switchText->Draw(canvas);
    }

    canvas->Restore();

}

Int32 Switch::GetCompoundPaddingLeft()
{
    if (!IsLayoutRtl()) {
        return CompoundButton::GetCompoundPaddingLeft();
    }

    Int32 padding = CompoundButton::GetCompoundPaddingLeft() + mSwitchWidth;
    if (!TextUtils::IsEmpty(GetText())) {
        padding += mSwitchPadding;
    }

    return padding;
}

Int32 Switch::GetCompoundPaddingRight()
{
    if (IsLayoutRtl()) {
        return CompoundButton::GetCompoundPaddingRight();
    }

    Int32 padding = CompoundButton::GetCompoundPaddingRight() + mSwitchWidth;
    if (!TextUtils::IsEmpty(GetText())) {
        padding += mSwitchPadding;
    }

    return padding;
}

Int32 Switch::GetThumbScrollRange()
{
    if (!mTrackDrawable) {
        return 0;
    }
    Boolean res = FALSE;
    mTrackDrawable->GetPadding(mTempRect, &res);
    AutoPtr<CRect> rect = (CRect*)mTempRect.Get();
    return mSwitchWidth - mThumbWidth - rect->mLeft - rect->mRight;
}

ECode Switch::OnCreateDrawableState(
    /* [in] */ Int32 extraSpace,
    /* [out, callee] */ ArrayOf<Int32>** drawableState)
{
    VALIDATE_NOT_NULL(drawableState)
    CompoundButton::OnCreateDrawableState(extraSpace + 1, drawableState);
    if (IsChecked()) {
        MergeDrawableStates(*drawableState, (ArrayOf<Int32>*)CHECKED_STATE_SET);
    }
    return NOERROR;
}

ECode Switch::DrawableStateChanged()
{
    CompoundButton::DrawableStateChanged();
    AutoPtr< ArrayOf<Int32> > myDrawableState = GetDrawableState();

    Boolean res = FALSE;
    if (mThumbDrawable) mThumbDrawable->SetState(myDrawableState, &res);
    if (mTrackDrawable) mTrackDrawable->SetState(myDrawableState, &res);

    Invalidate();

    return NOERROR;
}

Boolean Switch::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    return CompoundButton::VerifyDrawable(who) || who == mThumbDrawable || who == mTrackDrawable;
}

ECode Switch::JumpDrawablesToCurrentState()
{
    CompoundButton::JumpDrawablesToCurrentState();
    if (mThumbDrawable) mThumbDrawable->JumpToCurrentState();
    if (mTrackDrawable) mTrackDrawable->JumpToCurrentState();

    return NOERROR;
}

ECode Switch::OnInitializeAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    CompoundButton::OnInitializeAccessibilityEvent(event);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CSwitch"), (ICharSequence**)&seq);
    return event->SetClassName(seq);
}

ECode Switch::OnInitializeAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    CompoundButton::OnInitializeAccessibilityNodeInfo(info);
    AutoPtr<ICharSequence> seq;
    CStringWrapper::New(String("CSwitch"), (ICharSequence**)&seq);
    info->SetClassName(seq);

    AutoPtr<ICharSequence> switchText = IsChecked() ? mTextOn : mTextOff;
    if (!TextUtils::IsEmpty(switchText)) {
        AutoPtr<ICharSequence> oldText;
        info->GetText((ICharSequence**)&oldText);
        if (TextUtils::IsEmpty(oldText)) {
            info->SetText(switchText);
        } else {
            StringBuilder sb;
            sb.AppendCharSequence(oldText);
            sb.AppendString(String(""));
            sb.AppendCharSequence(switchText);
            AutoPtr<ICharSequence> seq = sb.ToCharSequence();
            info->SetText(seq);
        }
    }
    return NOERROR;
}

} // namespace Widget
} // namespace Droid
} // namespace Elastos


