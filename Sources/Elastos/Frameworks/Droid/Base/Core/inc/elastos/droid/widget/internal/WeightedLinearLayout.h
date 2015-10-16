
#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_WEIGHTEDLINEARLAYOUT_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_WEIGHTEDLINEARLAYOUT_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/widget/LinearLayout.h"

using Elastos::Droid::Widget::LinearLayout;

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {


/**
 * A special layout when measured in AT_MOST will take up a given percentage of
 * the available space.
 */
class WeightedLinearLayout : public LinearLayout
{
public:
    WeightedLinearLayout(
        /* [in] */ IContext* context);

    /**
     * <p>Creates a new TableLayout for the given context and with the
     * specified set attributes.</p>
     *
     * @param context the application environment
     * @param attrs a collection of attributes
     */
    WeightedLinearLayout(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

protected:
    WeightedLinearLayout();

    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    //@Override
    virtual CARAPI_(void) OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);

private:
    Float mMajorWeightMin;
    Float mMinorWeightMin;
    Float mMajorWeightMax;
    Float mMinorWeightMax;
};

} // namespace Internal
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_WEIGHTEDLINEARLAYOUT_H__
