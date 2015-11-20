
#ifndef __ELASTOS_DROID_WIDGET_RELATIVELAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_RELATIVELAYOUTLAYOUTPARAMS_H__

#include "elastos/droid/view/ViewGroupMarginLayoutParams.h"

using Elastos::Droid::View::IViewGroupLayoutParams;
using Elastos::Droid::View::IViewGroupMarginLayoutParams;
using Elastos::Droid::View::ViewGroupMarginLayoutParams;

namespace Elastos {
namespace Droid {
namespace Widget {

class RelativeLayoutLayoutParams
    : public ViewGroupMarginLayoutParams
    , public IRelativeLayoutLayoutParams
{
public:
    CAR_INTERFACE_DECL();

    RelativeLayoutLayoutParams();

    CARAPI AddRule(
        /* [in] */ Int32 verb);

    CARAPI AddRule(
        /* [in] */ Int32 verb,
        /* [in] */ Int32 anchor);

    CARAPI RemoveRule(
        /* [in] */ Int32 verb);

    CARAPI GetRules(
        /* [out, callee] */ ArrayOf<Int32>** rules);

    CARAPI GetRules(
        /* [in] */ Int32 layoutDirection,
        /* [out, callee] */ ArrayOf<Int32>** rules);

    CARAPI SetAlignWithParent(
        /* [in] */ Boolean align);

    CARAPI GetAlignWithParent(
        /* [out] */ Boolean* align);

    virtual CARAPI ResolveLayoutDirection(
        /* [in] */ Int32 layoutDirection);

    CARAPI constructor(
        /* [in] */ IContext* c,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);

    /**
     * Copy constructor. Clones the width, height, margin values, and rules
     * of the source.
     *
     * @param source The layout params to copy from.
     */
    CARAPI constructor(
        /* [in] */ IRelativeLayoutLayoutParams* source);

private:
    CARAPI InitFromAttributes(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    // The way we are resolving rules depends on the layout direction and if we are pre JB MR1
    // or not.
    //
    // If we are pre JB MR1 (said as "RTL compatibility mode"), "left"/"right" rules are having
    // predominance over any "start/end" rules that could have been defined. A special case:
    // if no "left"/"right" rule has been defined and "start"/"end" rules are defined then we
    // resolve those "start"/"end" rules to "left"/"right" respectively.
    //
    // If we are JB MR1+, then "start"/"end" rules are having predominance over "left"/"right"
    // rules. If no "start"/"end" rule is defined then we use "left"/"right" rules.
    //
    // In all cases, the result of the resolution should clear the "start"/"end" rules to leave
    // only the "left"/"right" rules at the end.
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

    Boolean mRulesChanged;
    Boolean mIsRtlCompatibilityMode;

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
