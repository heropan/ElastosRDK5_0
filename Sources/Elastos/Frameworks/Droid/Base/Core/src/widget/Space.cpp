
#include "widget/Space.h"
#include <elastos/core/Math.h>

using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace Widget {

Space::Space()
{}

Space::Space(
    /* [in] */ IContext* context)
{
    InitImpl(context, NULL, 0);
}

Space::Space(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    InitImpl(context, attrs, 0);
}

Space::Space(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    InitImpl(context, attrs, defStyle);
}

ECode Space::Init(
    /* [in] */ IContext* context)
{
    return Init(context, NULL);
}

ECode Space::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return Init(context, attrs, 0);
}

ECode Space::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    ASSERT_SUCCEEDED(View::Init(context, attrs, defStyle));
    return InitImpl(context, attrs, defStyle);
}

ECode Space::InitImpl(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs,
    /* [in] */ Int32 defStyle)
{
    if (GetVisibility() == IView::VISIBLE) {
        SetVisibility(IView::INVISIBLE);
    }
    return NOERROR;
}

/**
 * Draw nothing.
 *
 * @param canvas an unused parameter.
 */
//@Override
ECode Space::Draw(
    /* [in] */ ICanvas* canvas)
{
    return NOERROR;
}

void Space::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    SetMeasuredDimension(
        GetDefaultSize2(GetSuggestedMinimumWidth(), widthMeasureSpec),
        GetDefaultSize2(GetSuggestedMinimumHeight(), heightMeasureSpec));
}

/**
 * Compare to: {@link View#getDefaultSize(Int32, Int32)}
 * If mode is AT_MOST, return the child size instead of the parent size
 * (unless it is too big).
 */
Int32 Space::GetDefaultSize2(
    /* [in] */ Int32 size,
    /* [in] */ Int32 measureSpec)
{
    using Elastos::Core::Math;
    Int32 result = size;
    Int32 specMode = MeasureSpec::GetMode(measureSpec);
    Int32 specSize = MeasureSpec::GetSize(measureSpec);

    switch (specMode) {
        case MeasureSpec::UNSPECIFIED:
            result = size;
            break;
        case MeasureSpec::AT_MOST:
            result = Math::Min(size, specSize);
            break;
        case MeasureSpec::EXACTLY:
            result = specSize;
            break;
    }
    return result;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
