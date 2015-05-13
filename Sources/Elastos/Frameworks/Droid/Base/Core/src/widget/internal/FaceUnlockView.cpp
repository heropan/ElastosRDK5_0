
#include "internal/FaceUnlockView.h"
#include <elastos/Math.h>

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

const String FaceUnlockView::FaceUnlockView_TAG = String("FaceUnlockView");

FaceUnlockView::FaceUnlockView()
{}

FaceUnlockView::FaceUnlockView(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs) : RelativeLayout(context, attrs)
{}

ECode FaceUnlockView::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return RelativeLayout::Init(context, attrs);
}

Int32 FaceUnlockView::ResolveMeasured(
    /* [in] */ Int32 measureSpec,
    /* [in] */ Int32 desired)
{
    Int32 result = 0;
    Int32 specSize = MeasureSpec::GetSize(measureSpec);
    switch (MeasureSpec::GetMode(measureSpec)) {
        case MeasureSpec::UNSPECIFIED:
            result = desired;
            break;
        case MeasureSpec::AT_MOST:
            result = Elastos::Core::Math::Max(specSize, desired);
            break;
        case MeasureSpec::EXACTLY:
        default:
            result = specSize;
    }
    return result;
}


void FaceUnlockView::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    Int32 minimumWidth = GetSuggestedMinimumWidth();
    Int32 minimumHeight = GetSuggestedMinimumHeight();
    Int32 viewWidth = ResolveMeasured(widthMeasureSpec, minimumWidth);
    Int32 viewHeight = ResolveMeasured(heightMeasureSpec, minimumHeight);

    Int32 chosenSize = Elastos::Core::Math::Min(viewWidth, viewHeight);
    Int32 newWidthMeasureSpec =
        MeasureSpec::MakeMeasureSpec(chosenSize, MeasureSpec::AT_MOST);
    Int32 newHeightMeasureSpec =
        MeasureSpec::MakeMeasureSpec(chosenSize, MeasureSpec::AT_MOST);

    RelativeLayout::OnMeasure(newWidthMeasureSpec, newHeightMeasureSpec);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
