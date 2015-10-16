
#ifndef __ELASTOS_DROID_WIDGET_RELATIVELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_RELATIVELAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {


class RelativeLayoutLayoutParams : public ViewGroupMarginLayoutParams
{
public:
    RelativeLayoutLayoutParams();

    RelativeLayoutLayoutParams(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    RelativeLayoutLayoutParams(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    RelativeLayoutLayoutParams(
        /* [in] */ ViewGroupLayoutParams* source);

    RelativeLayoutLayoutParams(
        /* [in] */ ViewGroupMarginLayoutParams* source);

    CARAPI AddRule(
        /* [in] */ Int32 verb);

    CARAPI AddRule(
        /* [in] */ Int32 verb,
        /* [in] */ Int32 anchor);

    CARAPI RemoveRule(
        /* [in] */ Int32 verb);

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetRules();

    CARAPI_(AutoPtr<ArrayOf<Int32> >) GetRules(
        /* [in] */ Int32 layoutDirection);

    CARAPI SetAlignWithParent(
        /* [in] */ Boolean align);

    CARAPI_(Boolean) GetAlignWithParent();

    virtual CARAPI ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection);

protected:
    CARAPI Init(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI Init(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI Init(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI Init(
        /* [in] */ IViewGroupMarginLayoutParams* source);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI_(void) ResolveRules(
        /* [in] */ Int32 layoutDirection);

    CARAPI_(Boolean) HasRelativeRules();

public:
    AutoPtr<ArrayOf<Int32> > mRules;
    AutoPtr<ArrayOf<Int32> > mInitialRules;

    Int32 mLeft;
    Int32 mTop;
    Int32 mRight;
    Int32 mBottom;

    Int32 mStart;
    Int32 mEnd;

    Boolean mRulesChanged;

    /**
     * When true, uses the parent as the anchor if the anchor doesn't exist or if
     * the anchor's visibility is GONE.
     */
    Boolean mAlignWithParent;
};

} // namespace Widget
} // namespace Droid
} // namespace Elastos

#endif // __RelativeLayoutLayoutParams_H__
