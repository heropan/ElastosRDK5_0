
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Ui {

DropdownDividerDrawable::DropdownDividerDrawable()
{
	CPaint::New((IPaint**)&mPaint);
	CRect::New((IRect**)&mDividerRect);
}

//@Override
ECode DropdownDividerDrawable::Draw(
    /* in */ ICanvas* canvas)
{
	VALIDATE_NOT_NULL(canvas);
    canvas->DrawRect(mDividerRect, mPaint);
    return NOERROR;
}

ECode DropdownDividerDrawable::OnBoundsChange(
    /* in */ IRect* bounds)
{
	VALIDATE_NOT_NULL(bounds);
    mDividerRect->Set(0, 0, bounds->Width(), mDividerRect->Height());
    return NOERROR;
}

ECode DropdownDividerDrawable::SetHeight(
    /* in */ Int32 height)
{
    mDividerRect->Set(0, 0, mDividerRect->Right(), height);
    return NOERROR;
}

ECode DropdownDividerDrawable::SetColor(
    /* in */ Int32 color)
{
    mPaint->SetColor(color);
    return NOERROR;
}

//@Override
ECode DropdownDividerDrawable::SetAlpha(
    /* in */ Int32 alpha)
{
	return NOERROR;
}

//@Override
ECode DropdownDividerDrawable::SetColorFilter(
    /* in */ IColorFilter* cf)
{
	VALIDATE_NOT_NULL(cf);
	return NOERROR;
}

//@Override
Int32 DropdownDividerDrawable::GetOpacity()
{
    return IPixelFormat::OPAQUE;
}

} // namespace Ui
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
