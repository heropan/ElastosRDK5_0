#include "widget/RecycleListView.h"

namespace Elastos {
namespace Droid {
namespace Widget {

Boolean RecycleListView::Draw(
    /* [in] */ ICanvas* canvas,
    /* [in] */ IViewGroup* parent,
    /* [in] */ Int64 drawingTime)
{
    return ListView::Draw(canvas, parent, drawingTime);
}

RecycleListView::RecycleListView()
    : mRecycleOnMeasure(TRUE)
{
}

Boolean RecycleListView::GetRecycleOnMeasure()
{
    return mRecycleOnMeasure;
}

ECode RecycleListView::SetRecycleOnMeasure(
    /* [in] */ Boolean recycleOnMeasure)
{
    mRecycleOnMeasure = recycleOnMeasure;
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos
