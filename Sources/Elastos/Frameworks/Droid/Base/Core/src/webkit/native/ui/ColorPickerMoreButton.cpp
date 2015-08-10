
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

ColorPickerMoreButton::ColorPickerMoreButton(
    /* in */ IContext* context,
    /* in */ IAttributeSet* attrs)
    : Button(context, attrs)
{
    Init();
}

ColorPickerMoreButton::ColorPickerMoreButton(
    /* in */ IContext* context,
    /* in */ IAttributeSet* attrs,
    /* in */ Int32 defStyle)
    : Button(context, attrs, defStyle)
{
    Init();
}

/**
 * Sets up the paint to use for drawing the border.
 */
ECode ColorPickerMoreButton::Init()
{
	CPaint::New((IPaint**)&mBorderPaint);
    mBorderPaint->SetStyle(IPaint::Style::STROKE);
    mBorderPaint->SetColor(IColor::WHITE);
    // Set the width to one pixel.
    mBorderPaint->SetStrokeWidth(1.0f);
    // And make sure the border doesn't bleed into the outside.
    mBorderPaint->SetAntiAlias(FALSE);
    return NOERROR;
}

/**
 * Draws the border around the edge of the button.
 *
 * @param canvas The canvas to draw on.
 */
//@Override
ECode ColorPickerMoreButton::OnDraw(
    /* in */ ICanvas* canvas)
{
	VALIDATE_NOT_NULL(canvas);
    canvas->DrawRect(0.5f, 0.5f, GetWidth() - 1.5f, GetHeight() - 1.5f, mBorderPaint);
    Button::OnDraw(canvas);
    return NOERROR;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
