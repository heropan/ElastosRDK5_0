
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/webkit/native/ui/ColorPickerMoreButton.h"

using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IColor;
using Elastos::Droid::Graphics::PaintStyle_STROKE;

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
    //: Button(context, attrs)
    : mBorderPaint(NULL)
{
    // ==================before translated======================
    // super(context, attrs);
    // init();

    Init();
}

ColorPickerMoreButton::ColorPickerMoreButton(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
    //: Button(context, attrs, defStyle)
    : mBorderPaint(NULL)
{
    // ==================before translated======================
    // super(context, attrs, defStyle);
    // init();

    Init();
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

    CPaint::New((IPaint**)&mBorderPaint);
    mBorderPaint->SetStyle(PaintStyle_STROKE);
    mBorderPaint->SetColor(IColor::WHITE);
    // Set the width to one pixel.
    mBorderPaint->SetStrokeWidth(1.0f);
    // And make sure the border doesn't bleed into the outside.
    mBorderPaint->SetAntiAlias(FALSE);
    return NOERROR;
}

ECode ColorPickerMoreButton::OnDraw(
    /* [in] */ ICanvas* canvas)
{
    VALIDATE_NOT_NULL(canvas);
    // ==================before translated======================
    // canvas.drawRect(0.5f, 0.5f, getWidth() - 1.5f, getHeight() - 1.5f, mBorderPaint);
    // super.onDraw(canvas);

    canvas->DrawRect(0.5f, 0.5f, /*GetWidth() -*/ 1.5f, /*GetHeight() -*/ 1.5f, mBorderPaint);
    //Button::OnDraw(canvas);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


