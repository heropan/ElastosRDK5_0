#include "widget/ListView.h"

using Elastos::Droid::View::IViewGroup;

namespace Elastos {
namespace Droid {
namespace Widget {

class RecycleListView : public ListView
{
public:

    CARAPI_(Boolean) GetRecycleOnMeasure();

    CARAPI SetRecycleOnMeasure(
        /* [in] */ Boolean recycleOnMeasure);

    using ListView::Draw;

protected:

    virtual CARAPI_(Boolean) Draw(
        /* [in] */ ICanvas* canvas,
        /* [in] */ IViewGroup* parent,
        /* [in] */ Int64 drawingTime);

    RecycleListView();

private:

    Boolean mRecycleOnMeasure;

};
}// namespace Widget
}// namespace Droid
}// namespace Elastos
