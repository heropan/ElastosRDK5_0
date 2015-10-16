
#include "elastos/droid/widget/internal/WeightedLinearLayout.h"
#include "elastos/droid/R.h"

using Elastos::Droid::R;
using Elastos::Droid::Utility::IDisplayMetrics;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {


WeightedLinearLayout::WeightedLinearLayout()
    : mMajorWeightMin(0)
    , mMinorWeightMin(0)
    , mMajorWeightMax(0)
    , mMinorWeightMax(0)
{
}

WeightedLinearLayout::WeightedLinearLayout(
    /* [in] */ IContext* context)
    : LinearLayout(context)
    , mMajorWeightMin(0)
    , mMinorWeightMin(0)
    , mMajorWeightMax(0)
    , mMinorWeightMax(0)
{
}

WeightedLinearLayout::WeightedLinearLayout(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
    : mMajorWeightMin(0)
    , mMinorWeightMin(0)
    , mMajorWeightMax(0)
    , mMinorWeightMax(0)
{
    Init(context, attrs);
}

ECode WeightedLinearLayout::Init(
    /* [in] */ IContext* context)
{
    return LinearLayout::Init(context);
}

ECode WeightedLinearLayout::Init(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    LinearLayout::Init(context, attrs);

    AutoPtr<ArrayOf<Int32> > attrIds = ArrayOf<Int32>::Alloc(
            const_cast<Int32 *>(R::styleable::WeightedLinearLayout),
            ARRAY_SIZE(R::styleable::WeightedLinearLayout));
    AutoPtr<ITypedArray> a;
    context->ObtainStyledAttributes(attrs, attrIds, (ITypedArray**)&a);

    a->GetFloat(R::styleable::WeightedLinearLayout_majorWeightMin, 0.0f, &mMajorWeightMin);
    a->GetFloat(R::styleable::WeightedLinearLayout_minorWeightMin, 0.0f, &mMinorWeightMin);
    a->GetFloat(R::styleable::WeightedLinearLayout_majorWeightMax, 0.0f, &mMajorWeightMax);
    a->GetFloat(R::styleable::WeightedLinearLayout_minorWeightMax, 0.0f, &mMinorWeightMax);

    a->Recycle();
    return NOERROR;
}

void WeightedLinearLayout::OnMeasure(
    /* [in] */ Int32 widthMeasureSpec,
    /* [in] */ Int32 heightMeasureSpec)
{
    AutoPtr<IDisplayMetrics> metrics;
    AutoPtr<IResources> res;
    AutoPtr<IContext> context = GetContext();
    context->GetResources((IResources**)&res);
    res->GetDisplayMetrics((IDisplayMetrics**)&metrics);

    Int32 screenWidth, screenHeight;
    metrics->GetWidthPixels(&screenWidth);
    metrics->GetHeightPixels(&screenHeight);
    Boolean isPortrait = screenWidth < screenHeight;

    Int32 widthMode = MeasureSpec::GetMode(widthMeasureSpec);

    LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);

    Int32 width = GetMeasuredWidth();
    Boolean measure = FALSE;

    widthMeasureSpec = MeasureSpec::MakeMeasureSpec(width, MeasureSpec::EXACTLY);

    Float widthWeightMin = isPortrait ? mMinorWeightMin : mMajorWeightMin;
    Float widthWeightMax = isPortrait ? mMinorWeightMax : mMajorWeightMax;
    if (widthMode == MeasureSpec::AT_MOST) {
        Int32 weightedMin = (Int32) (screenWidth * widthWeightMin);
        Int32 weightedMax = (Int32) (screenWidth * widthWeightMin);
        if (widthWeightMin > 0.0f && width < weightedMin) {
            widthMeasureSpec = MeasureSpec::MakeMeasureSpec(weightedMin, MeasureSpec::EXACTLY);
            measure = TRUE;
        } else if (widthWeightMax > 0.0f && width > weightedMax) {
            widthMeasureSpec = MeasureSpec::MakeMeasureSpec(weightedMax, MeasureSpec::EXACTLY);
            measure = TRUE;
        }
    }

    // TODO: Support height?

    if (measure) {
        LinearLayout::OnMeasure(widthMeasureSpec, heightMeasureSpec);
    }
}

} // namespace Internal
} // namespace Widget
} // namespace Droid
} // namespace Elastos
