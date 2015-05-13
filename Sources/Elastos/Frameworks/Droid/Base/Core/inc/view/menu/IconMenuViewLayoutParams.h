
#ifndef _ICONMENUVIEWLAYOUTPARAMS_H__
#define _ICONMENUVIEWLAYOUTPARAMS_H__

#include "view/ViewGroupMarginLayoutParams.h"

namespace Elastos {
namespace Droid {
namespace View {
namespace Menu {

/**
 * Layout parameters specific to IconMenuView (stores the left, top, right, bottom from the
 * measure pass).
 */
class IconMenuViewLayoutParams : public ViewGroupMarginLayoutParams
{
public:
    IconMenuViewLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    IconMenuViewLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

protected:
    IconMenuViewLayoutParams();

    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

protected:
    Int32 mLeft, mTop, mRight, mBottom;
    Int32 mDesiredWidth;
    Int32 mMaxNumItemsOnRow;
};

} // namespace Menu
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif   //_ICONMENUVIEWLAYOUTPARAMS_H__
