
#include "elastos/droid/packages/launcher2/PackageChangedReceiver.h"
#include "Elastos.Droid.Service.h"
#include "R.h"

namespace Elastos {
namespace Droid {
namespace Packages {
namespace Launcher2 {

CAR_INTERFACE_IMPL(BubbleTextView, TextView, IBubbleTextView);

BubbleTextView::BubbleTextView()
    : mPrevAlpha(-1)
    , mDidInvalidateForPressedState(FALSE)
    , mFocusedOutlineColor(0)
    , mFocusedGlowColor(0)
    , mPressedOutlineColor(0)
    , mPressedGlowColor(0)
    , mBackgroundSizeChanged(FALSE)
    , mStayPressed(FALSE)
{
    mOutlineHelper = new HolographicOutlineHelper();
    CCanvas::New((ICanvas**)&mTempCanvas);
    CRect::New((IRect**)&mTempRect);
}

ECode BubbleTextView::constructor(
    /* [in] */ IContext* context)
{
    TextView::constructor(context);
    return Init();
}

ECode BubbleTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    TextView::constructor(context, attrs);
    return Init();
}

ECode BubbleTextView::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    TextView::constructor(context, attrs, defStyle);
    return Init();
}

ECode BubbleTextView::Init()
{
    mLongPressHelper = new CheckLongPressHelper(this);
    GetBackground(&mBackground);

    AutoPtr<IContext> context;
    GetContext((IContext**)&context);
    AutoPtr<IResources> res;
    context->GetResources((IResources**)&res);
    res->GetColor(android.R.color.white, &mFocusedOutlineColor);
    mFocusedGlowColor = mPressedOutlineColor =
            mPressedGlowColor = mFocusedOutlineColor;

    return SetShadowLayer(SHADOW_LARGE_RADIUS, 0.0f,
            SHADOW_Y_OFFSET, SHADOW_LARGE_COLOUR);
}

ECode BubbleTextView::ApplyFromShortcutInfo(
    /* [in] */ IShortcutInfo* info,
    /* [in] */ IconCache* iconCache)
{
    AutoPtr<IBitmap> b;
    info->GetIcon(iconCache, (IBitmap**)&b);

    AutoPtr<ShortcutInfo> _info = (ShortcutInfo*)info;
    SetCompoundDrawablesWithIntrinsicBounds(NULL,
            new FastBitmapDrawable(b),
            NULL, NULL);
    SetText(_info->mTitle);
    if (_info->mContentDescription != NULL) {
        setContentDescription(_info->mContentDescription);
    }
    return SetTag(TO_IINTERFACE(info));
}

ECode BubbleTextView::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Int32 _left;
    GetLeft(&_left);
    Int32 _right;
    GetRight(&_right);
    Int32 _top;
    GetTop(&_top);
    Int32 _bottom;
    GetBottom(&_bottom);

    if (_left != left || _right != right || _top != top || _bottom != bottom) {
        mBackgroundSizeChanged = true;
    }
    return TextView::SetFrame(left, top, right, bottom);
}

ECode BubbleTextView::VerifyDrawable(
    /* [in] */ IDrawable* who,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (who == mBackground) {
        *result = TRUE;
        return NOERROR;
    }

    Boolean res;
    TextView::VerifyDrawable(who, &res);
    *result = res;
    return NOERROR;
}

ECode BubbleTextView::SetTag(
    /* [in] */ IInterface* tag)
{
    if (tag != NULL) {
        AutoPtr<IItemInfo> info = IItemInfo::Probe(tag);
        LauncherModel::CheckItemInfo(info);
    }
    return TextView::SetTag(tag);
}

ECode BubbleTextView::DrawableStateChanged()
{
    Boolean res;
    IsPressed(&res);
    if (res) {
        // In this case, we have already created the pressed outline on ACTION_DOWN,
        // so we just need to do an invalidate to trigger draw
        if (!mDidInvalidateForPressedState) {
            SetCellLayoutPressedOrFocusedIcon();
        }
    }
    else {
        // Otherwise, either clear the pressed/focused background, or create a background
        // for the focused state
        Boolean backgroundEmptyBefore = mPressedOrFocusedBackground == NULL;
        if (!mStayPressed) {
            mPressedOrFocusedBackground = NULL;
        }
        Boolean tmp;
        isFocused(&tmp);
        if (tmp) {
            AutoPtr<ILayout> layout;
            GetLayout((ILayout**)&layout);
            if (layout == NULL) {
                // In some cases, we get focus before we have been layed out. Set the
                // background to null so that it will get created when the view is drawn.
                mPressedOrFocusedBackground = NULL;
            }
            else {
                mPressedOrFocusedBackground = CreateGlowingOutline(
                        mTempCanvas, mFocusedGlowColor, mFocusedOutlineColor);
            }
            mStayPressed = FALSE;
            SetCellLayoutPressedOrFocusedIcon();
        }
        Boolean backgroundEmptyNow = mPressedOrFocusedBackground == NULL;
        if (!backgroundEmptyBefore && backgroundEmptyNow) {
            SetCellLayoutPressedOrFocusedIcon();
        }
    }

    AutoPtr<IDrawable> d = mBackground;
    if (d != NULL) {
        Boolean res;
        d->IsStateful(res);
        if (res) {
            AutoPtr<ArrayOf<Int32> > state;
            GetDrawableState((ArrayOf<Int32>**)&state);
            Boolean isStateful;
            d->SetState(state, &isStateful);
        }
    }
    return TextView::DrawableStateChanged();
}

void BubbleTextView::DrawWithPadding(
    /* [in] */ ICanvas* destCanvas,
    /* [in] */ Int32 padding)
{
    AutoPtr<IRect> clipRect = mTempRect;
    GetDrawingRect(clipRect);

    // adjust the clip rect so that we don't include the text label
    Int32 top;
    GetExtendedPaddingTop(&top);
    AutoPtr<ILayout> layout;
    GetLayout((ILayout**)&layout);
    Int32 pos;
    layout0>GetLineTop(0, &pos);
    Int32 bottom = top - (Int32)IBubbleTextView::PADDING_V + pos;
    clipRect->SetBottom(bottom);

    // Draw the View into the bitmap.
    // The translate of scrollX and scrollY is necessary when drawing TextViews, because
    // they set scrollX and scrollY to large values to achieve centered text
    destCanvas->Save();
    Float x;
    GetScaleX(&x);
    Float y;
    GetScaleY(&y);
    Int32 width;
    GetWidth(&width);
    Int32 height;
    GetHeight(&height);
    destCanvas->Scale(x, y, (width + padding) / 2, (height + padding) / 2);
    Int32 sx;
    GetScrollX(&sx);
    Int32 sy;
    GetScrollY(&sy);
    destCanvas->Translate(-sx + padding / 2, -sy + padding / 2);
    destCanvas->ClipRect(clipRect, Op.REPLACE);
    Draw(destCanvas);
    return destCanvas->Restore();
}

AutoPtr<IBitmap> BubbleTextView::CreateGlowingOutline(
    /* [in] */ ICanvas* canvas,
    /* [in] */ Int32 outlineColor,
    /* [in] */ Int32 glowColor)
{
    Int32 padding = HolographicOutlineHelper::MAX_OUTER_BLUR_RADIUS;

    Int32 width;
    GetWidth(&width);
    Int32 height;
    GetHeight(&height);
    AutoPtr<IBitmap> b;
    Bitmap::CreateBitmap(
            width + padding, height + padding,
            Bitmap::Config::ARGB_8888, (IBitmap**)&b);

    canvas->SetBitmap(b);
    DrawWithPadding(canvas, padding);
    mOutlineHelper->ApplyExtraThickExpensiveOutlineWithBlur(b,
            canvas, glowColor, outlineColor);
    canvas->SetBitmap(NULL);

    return b;
}

ECode BubbleTextView::OnTouchEvent(
    /* [in] */ IMotionEvent* event,
    /* [out] */ Boolean* ret)
{
    VALIDATE_NOT_NULL(ret);

    // Call the superclass onTouchEvent first, because sometimes it changes the state to
    // isPressed() on an ACTION_UP
    Boolean result;
    TextView::OnTouchEvent(event, &result);

    Int32 action;
    event->GetAction(&action);
    switch (action) {
        case IMotionEvent::ACTION_DOWN:
        {
            // So that the pressed outline is visible immediately when isPressed() is true,
            // we pre-create it on ACTION_DOWN (it takes a small but perceptible amount of time
            // to create it)
            if (mPressedOrFocusedBackground == NULL) {
                mPressedOrFocusedBackground = CreateGlowingOutline(
                        mTempCanvas, mPressedGlowColor, mPressedOutlineColor);
            }
            // Invalidate so the pressed state is visible, or set a flag so we know that we
            // have to call invalidate as soon as the state is "pressed"
            Boolean res;
            IsPressed(&res);
            if (res) {
                mDidInvalidateForPressedState = TRUE;
                SetCellLayoutPressedOrFocusedIcon();
            }
            else {
                mDidInvalidateForPressedState = FALSE;
            }

            mLongPressHelper->PostCheckForLongPress();
            break;
        }
        case IMotionEvent::ACTION_CANCEL:
        case IMotionEvent::ACTION_UP:
        {
            // If we've touched down and up on an item, and it's still not "pressed", then
            // destroy the pressed outline
            Boolean res;
            IsPressed(&res);
            if (!res) {
                mPressedOrFocusedBackground = NULL;
            }

            mLongPressHelper->CancelLongPress();
            break;
        }
    }
    *ret = result;
    return NOERROR;
}

ECode BubbleTextView::SetStayPressed(
    /* [in] */ Boolean stayPressed)
{
    mStayPressed = stayPressed;
    if (!stayPressed) {
        mPressedOrFocusedBackground = NULL;
    }
    return SetCellLayoutPressedOrFocusedIcon();
}

ECode BubbleTextView::SetCellLayoutPressedOrFocusedIcon()
{
    AutoPtr<IViewParent> res;
    GetParent((IViewParent**)&res);
    if (IShortcutAndWidgetContainer::Probe(res) ! = NULL) {
        AutoPtr<IShortcutAndWidgetContainer> parent =
                IShortcutAndWidgetContainer::Probe(res);
        if (parent != NULL) {
            AutoPtr<IViewParent> tmp;
            IViewGroup::Probe(parent)->GetParent((IViewParent**)&tmp);
            AutoPtr<ICellLayout> layout = ICellLayout::Probe(tmp);
            layout->SetPressedOrFocusedIcon((mPressedOrFocusedBackground != NULL) ?
                    this : NULL);
        }
    }
    return NOERROR;
}

ECode BubbleTextView::ClearPressedOrFocusedBackground()
{
    mPressedOrFocusedBackground = NULL;
    return SetCellLayoutPressedOrFocusedIcon();
}

ECode BubbleTextView::GetPressedOrFocusedBackground(
    /* [out] */ IBitmap** map)
{
    VALIDATE_NOT_NULL(map);

    *map = mPressedOrFocusedBackground;
    REFCOUNT_ADD(*map);
    return NOERROR;
}

ECode BubbleTextView::GetPressedOrFocusedBackgroundPadding(
    /* [out] */ Int32* res)
{
    VALIDATE_NOT_NULL(map);

    *res = HolographicOutlineHelper::MAX_OUTER_BLUR_RADIUS / 2;
    return NOERROR;
}

ECode BubbleTextView::Draw(
    /* [in] */ ICanvas* canvas)
{
    AutoPtr<IDrawable> background = mBackground;
    if (background != NULL) {
        Int32 scrollX;
        GetScrollX(&scrollX);
        Int32 scrollY;
        GetScrollY(&scrollY);

        if (mBackgroundSizeChanged) {
            Int32 right;
            GetRight(&right);
            Int32 left;
            GetLeft(&left);
            Int32 bottom;
            GetBottom(&bottom);
            Int32 top;
            GetTop(&top);
            background->SetBounds(0, 0,  right - left, bottom - top);
            mBackgroundSizeChanged = FALSE;
        }

        if ((scrollX | scrollY) == 0) {
            background->Draw(canvas);
        }
        else {
            canvas->Translate(scrollX, scrollY);
            background->Draw(canvas);
            canvas->Translate(-scrollX, -scrollY);
        }
    }

    // If text is transparent, don't draw any shadow
    Int32 colors;
    GetCurrentTextColor(&colors);
    AutoPtr<IResources> res;
    GetResources((IResources**)&res);
    Int32 _colors;
    res->getColor(android.R.color.transparent, &_colors);
    if (colors == _colors) {
        AutoPtr<ITextPaint> paint;
        GetPaint((ITextPaint**)&paint);
        paint->ClearShadowLayer();
        TextView::Draw(canvas);
        return NOERROR;
    }

    // We enhance the shadow by drawing the shadow twice
    AutoPtr<ITextPaint> paint;
    GetPaint((ITextPaint**)&paint);
    paint->SetShadowLayer(SHADOW_LARGE_RADIUS, 0.0f, SHADOW_Y_OFFSET,
            SHADOW_LARGE_COLOUR);
    TextView::Draw(canvas);
    canvas->Save(ICanvas::CLIP_SAVE_FLAG);

    Int32 scrollX;
    GetScrollX(&scrollX);
    Int32 scrollY;
    GetScrollY(&scrollY);
    Int32 top;
    GetExtendedPaddingTop(&top);
    Int32 width;
    GetWidth(&width);
    Int32 height;
    GetHeight(&height);
    canvas->ClipRect(scrollX, scrollY + top,
            scrollX + width,
            scrollY + height, IRegion::Op::INTERSECT);

    AutoPtr<ITextPaint> paint2;
    GetPaint((ITextPaint**)&paint2);
    paint2->SetShadowLayer(SHADOW_SMALL_RADIUS, 0.0f, 0.0f,
            SHADOW_SMALL_COLOUR);
    TextView::Draw(canvas);
    return canvas->Restore();
}

ECode BubbleTextView::OnAttachedToWindow()
{
    TextView::OnAttachedToWindow();
    if (mBackground != NULL) {
        return mBackground->SetCallback(this);
    }
    return NOERROR;
}

ECode BubbleTextView::OnDetachedFromWindow()
{
    TextView::OnDetachedFromWindow();
    if (mBackground != NULL) {
        return mBackground->SetCallback(NULL);
    }
    return NOERROR;
}

ECode BubbleTextView::OnSetAlpha(
    /* [in] */ Int32 alpha,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    if (mPrevAlpha != alpha) {
        mPrevAlpha = alpha;
        TextView::OnSetAlpha(alpha);
    }
    *result = TRUE;
    return NOERROR;
}

ECode BubbleTextView::CancelLongPress()
{
    TextView::CancelLongPress();

    return mLongPressHelper->CancelLongPress();
}

} // namespace Launcher2
} // namespace Packages
} // namespace Droid
} // namespace Elastos