
#ifndef __ELASTOS_DROID_WIDGET_SPACE_H__
#define __ELASTOS_DROID_WIDGET_SPACE_H__

#include "elastos/droid/view/View.h"

namespace Elastos {
namespace Droid {
namespace Widget {

/**
 * Space is a lightweight View subclass that may be used to create gaps between components
 * in general purpose layouts.
 */
class Space : public Elastos::Droid::View::View
{
public:
    Space();

    Space(
        /* [in] */ IContext* context);

    Space(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    Space(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

protected:
    CARAPI Init(
        /* [in] */ IContext* context);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);

    CARAPI InitImpl(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs,
        /* [in] */ Int32 defStyle);


    /**
     * Draw nothing.
     *
     * @param canvas an unused parameter.
     */
    //@Override
    virtual ECode Draw(
        /* [in] */ ICanvas* canvas);

protected:

    virtual void OnMeasure(
        /* [in] */ Int32 widthMeasureSpec,
        /* [in] */ Int32 heightMeasureSpec);
private:

    /**
     * Compare to: {@link View#getDefaultSize(int, int)}
     * If mode is AT_MOST, return the child size instead of the parent size
     * (unless it is too big).
     */
    static Int32 GetDefaultSize2(
        /* [in] */ Int32 size,
        /* [in] */ Int32 measureSpec);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos


#endif //__ELASTOS_DROID_WIDGET_SPACE_H__
