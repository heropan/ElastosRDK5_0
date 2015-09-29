
#include "elastos/droid/webkit/native/ui/ColorPickerMoreButton.h"

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

//=====================================================================
//                        ColorPickerMoreButton
//=====================================================================
ColorPickerMoreButton::ColorPickerMoreButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    // ==================before translated======================
    // super(context, attrs);
    // init();
}

ColorPickerMoreButton::ColorPickerMoreButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    // ==================before translated======================
    // super(context, attrs, defStyle);
    // init();
}

ECode ColorPickerMoreButton::Init()
{
    // ==================before translated======================
    // mBorderPaint = new Paint();
    // mBorderPaint.setStyle(Paint.Style.STROKE);
    // mBorderPaint.setColor(Color.WHITE);
    // // Set the width to one pixel.
    // mBorderPaint.setStrokeWidth(1.0f);
    // // And make sure the border doesn't bleed into the outside.
    // mBorderPaint.setAntiAlias(false);
    assert(0);
    return NOERROR;
}

ECode ColorPickerMoreButton::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // canvas.drawRect(0.5f, 0.5f, getWidth() - 1.5f, getHeight() - 1.5f, mBorderPaint);
    // super.onDraw(canvas);
    assert(0);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


