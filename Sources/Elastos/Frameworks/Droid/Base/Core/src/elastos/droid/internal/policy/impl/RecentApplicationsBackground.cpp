
#include "elastos/droid/internal/policy/impl/RecentApplicationsBackground.h"

namespace Elastos {
namespace Droid {
namespace Internal {
namespace Policy {
namespace Impl {

//=====================================================================
//                     RecentApplicationsBackground
//=====================================================================
CAR_INTERFACE_IMPL(RecentApplicationsBackground, LinearLayout, IRecentApplicationsBackground)

const String RecentApplicationsBackground::TAG("RecentApplicationsBackground");

RecentApplicationsBackground::RecentApplicationsBackground()
{
}

CARAPI RecentApplicationsBackground::constructor(
    /* [in] */ IContext* ctx)
{
    // ==================before translated======================
    // this(context, null);
    // init();
    return NOERROR;
}

CARAPI RecentApplicationsBackground::constructor(
    /* [in] */ IContext* ctx,
    /* [in] */ IAttributeSet* attrs)
{
    // ==================before translated======================
    // super(context, attrs);
    // init();
    return NOERROR;
}

ECode RecentApplicationsBackground::JumpDrawablesToCurrentState()
{
    // ==================before translated======================
    // super.jumpDrawablesToCurrentState();
    // if (mBackground != null) mBackground.jumpToCurrentState();
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsBackground::Draw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // final Drawable background = mBackground;
    // if (background != null) {
    //     if (mBackgroundSizeChanged) {
    //         mBackgroundSizeChanged = false;
    //         Rect chld = mTmp0;
    //         Rect bkg = mTmp1;
    //         mBackground.getPadding(bkg);
    //         getChildBounds(chld);
    //         // This doesn't clamp to this view's bounds, which is what we want,
    //         // so that the drawing is clipped.
    //         final int top = chld.top - bkg.top;
    //         final int bottom = chld.bottom + bkg.bottom;
    //         // The background here is a gradient that wants to
    //         // extend the full width of the screen (whatever that
    //         // may be).
    //         int left, right;
    //         if (false) {
    //             // This limits the width of the drawable.
    //             left = chld.left - bkg.left;
    //             right = chld.right + bkg.right;
    //         } else {
    //             // This expands it to full width.
    //             left = 0;
    //             right = getRight();
    //         }
    //         background.setBounds(left, top, right, bottom);
    //     }
    // }
    // mBackground.draw(canvas);
    //
    // if (false) {
    //     android.graphics.Paint p = new android.graphics.Paint();
    //     p.setColor(0x88ffff00);
    //     canvas.drawRect(background.getBounds(), p);
    // }
    // canvas.drawARGB((int)(0.75*0xff), 0, 0, 0);
    //
    // super.draw(canvas);
    assert(0);
    return NOERROR;
}

Boolean RecentApplicationsBackground::SetFrame(
    /* [in] */ Int32 left,
    /* [in] */ Int32 top,
    /* [in] */ Int32 right,
    /* [in] */ Int32 bottom)
{
    // ==================before translated======================
    // setWillNotDraw(false);
    // if (mLeft != left || mRight != right || mTop != top || mBottom != bottom) {
    //     mBackgroundSizeChanged = true;
    // }
    // return super.setFrame(left, top, right, bottom);
    assert(0);
    return FALSE;
}

Boolean RecentApplicationsBackground::VerifyDrawable(
    /* [in] */ IDrawable* who)
{
    // ==================before translated======================
    // return who == mBackground || super.verifyDrawable(who);
    assert(0);
    return FALSE;
}

ECode RecentApplicationsBackground::DrawableStateChanged()
{
    // ==================before translated======================
    // Drawable d = mBackground;
    // if (d != null && d.isStateful()) {
    //     d.setState(getDrawableState());
    // }
    // super.drawableStateChanged();
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsBackground::OnAttachedToWindow()
{
    // ==================before translated======================
    // super.onAttachedToWindow();
    // mBackground.setCallback(this);
    // setWillNotDraw(false);
    assert(0);
    return NOERROR;
}

ECode RecentApplicationsBackground::OnDetachedFromWindow()
{
    // ==================before translated======================
    // super.onDetachedFromWindow();
    // mBackground.setCallback(null);
    assert(0);
    return NOERROR;
}

void RecentApplicationsBackground::Init()
{
    // ==================before translated======================
    // mBackground = getBackground();
    // setBackgroundDrawable(null);
    // setPadding(0, 0, 0, 0);
    // setGravity(Gravity.CENTER);
    assert(0);
}

void RecentApplicationsBackground::GetChildBounds(
    /* [in] */ IRect* r)
{
    // ==================before translated======================
    // r.left = r.top = Integer.MAX_VALUE;
    // r.bottom = r.right = Integer.MIN_VALUE;
    // final int N = getChildCount();
    // for (int i=0; i<N; i++) {
    //     View v = getChildAt(i);
    //     if (v.getVisibility() == View.VISIBLE) {
    //         r.left = Math.min(r.left, v.getLeft());
    //         r.top = Math.min(r.top, v.getTop());
    //         r.right = Math.max(r.right, v.getRight());
    //         r.bottom = Math.max(r.bottom, v.getBottom());
    //     }
    // }
    assert(0);
}

} // namespace Impl
} // namespace Policy
} // namespace Internal
} // namespace Droid
} // namespace Elastos

