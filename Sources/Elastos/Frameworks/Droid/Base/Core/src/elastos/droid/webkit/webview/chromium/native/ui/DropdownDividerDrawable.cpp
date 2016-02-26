
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CRect.h"
#include "elastos/droid/webkit/webview/chromium/native/ui/DropdownDividerDrawable.h"

using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Graphics::IPixelFormat;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Webview {
namespace Chromium {
namespace Ui {

//=====================================================================
//                       DropdownDividerDrawable
//=====================================================================
DropdownDividerDrawable::DropdownDividerDrawable()
{
    // ==================before translated======================
    // mPaint = new Paint();
    // mDividerRect = new Rect();

    CPaint::New((IPaint**)&mPaint);
    CRect::New((IRect**)&mDividerRect);
}

ECode DropdownDividerDrawable::Draw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // canvas.drawRect(mDividerRect, mPaint);

    canvas->DrawRect(mDividerRect, mPaint);
    return NOERROR;
}

void DropdownDividerDrawable::OnBoundsChange(
    /* [in] */ IRect* bounds)
{
    //VALIDATE_NOT_NULL(bounds);
    // ==================before translated======================
    // mDividerRect.set(0, 0, bounds.width(), mDividerRect.height());

    Int32 width = 0;
    Int32 height = 0;
    bounds->GetWidth(&width);
    mDividerRect->GetHeight(&height);
    mDividerRect->Set(0, 0, width, height);
}

ECode DropdownDividerDrawable::SetHeight(
    /* [in] */ Int32 height)
{
    // ==================before translated======================
    // mDividerRect.set(0, 0, mDividerRect.right, height);

    Int32 right = 0;
    mDividerRect->GetRight(&right);
    mDividerRect->Set(0, 0, right, height);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetColor(
    /* [in] */ Int32 color)
{
    // ==================before translated======================
    // mPaint.setColor(color);

    mPaint->SetColor(color);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetAlpha(
    /* [in] */ Int32 alpha)
{
    return NOERROR;
}

ECode DropdownDividerDrawable::SetColorFilter(
    /* [in] */ IColorFilter* cf)
{
    VALIDATE_NOT_NULL(cf);
    return NOERROR;
}

Int32 DropdownDividerDrawable::GetOpacity()
{
    // ==================before translated======================
    // return PixelFormat.OPAQUE;

    return IPixelFormat::OPAQUE;
}

ECode DropdownDividerDrawable::GetOpacity(
    /* [out] */ Int32* opacity)
{
    *opacity = 0;
    return NOERROR;
}

} // namespace Ui
} // namespace Chromium
} // namespace Webview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


