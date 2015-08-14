#ifndef __ELASTOS_DROID_WIDGET_CLINEARLAYOUTLAYOUTPARAMS_H__
#define __ELASTOS_DROID_WIDGET_CLINEARLAYOUTLAYOUTPARAMS_H__

#include "_CRelativeLayoutLayoutParams.h"
#include "widget/RelativeLayoutLayoutParams.h"


namespace Elastos{
namespace Droid{
namespace Widget{

CarClass(CRelativeLayoutLayoutParams), public RelativeLayoutLayoutParams
{
public:

    IVIEWGROUPLP_METHODS_DECL()
    IVIEWGROUPMARGINLP_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    CARAPI constructor(
        /* [in] */ Int32 width,
        /* [in] */ Int32 height);

    CARAPI constructor(
        /* [in] */ IViewGroupLayoutParams* source);

    CARAPI constructor(
        /* [in] */ IViewGroupMarginLayoutParams* source);


    /**
     * Adds a layout rule to be interpreted by the RelativeLayout. This
     * method should only be used for constraints that don't refer to another sibling
     * (e.g., CENTER_IN_PARENT) or take a boolean value ({@link RelativeLayout#TRUE}
     * for true or - for false). To specify a verb that takes a subject, use
     * {@link #addRule(int, int)} instead.
     *
     * @param verb One of the verbs defined by
     *        {@link android.widget.RelativeLayout RelativeLayout}, such as
     *        ALIGN_WITH_PARENT_LEFT.
     * @see #addRule(int, int)
     */
    CARAPI AddRule(
        /* [in] */ Int32 verb);

    /**
     * Adds a layout rule to be interpreted by the RelativeLayout. Use this for
     * verbs that take a target, such as a sibling (ALIGN_RIGHT) or a boolean
     * value (VISIBLE).
     *
     * @param verb One of the verbs defined by
     *        {@link android.widget.RelativeLayout RelativeLayout}, such as
     *         ALIGN_WITH_PARENT_LEFT.
     * @param anchor The id of another view to use as an anchor,
     *        or a boolean value(represented as {@link RelativeLayout#TRUE})
     *        for true or 0 for false).  For verbs that don't refer to another sibling
     *        (for example, ALIGN_WITH_PARENT_BOTTOM) just use -1.
     * @see #addRule(int)
     */
    CARAPI AddRule(
        /* [in] */ Int32 verb,
        /* [in] */ Int32 anchor);

     /**
      * Removes a layout rule to be interpreted by the RelativeLayout.
      *
      * @param verb One of the verbs defined by
      *        {@link android.widget.RelativeLayout RelativeLayout}, such as
      *         ALIGN_WITH_PARENT_LEFT.
      * @see #addRule(int)
      * @see #addRule(int, int)
      */
     CARAPI RemoveRule(
         /* [in] */ Int32 verb);

    /**
     * Retrieves a complete list of all supported rules, where the index is the rule
     * verb, and the element value is the value specified, or "false" if it was never
     * set.
     *
     * @return the supported rules
     * @see #addRule(int, int)
     */
     CARAPI GetRules(
        /* [out, callee] */ ArrayOf<Int32>** rules);

    /**
     * Retrieves a complete list of all supported rules, where the index is the rule
     * verb, and the element value is the value specified, or "false" if it was never
     * set. If there are relative rules defined (*_START / *_END), they will be resolved
     * depending on the layout direction.
     *
     * @param layoutDirection the direction of the layout.
     *                        Should be either {@link View#LAYOUT_DIRECTION_LTR}
     *                        or {@link View#LAYOUT_DIRECTION_RTL}
     * @return the supported rules
     * @see #addRule(int, int)
     *
     * @hide
     */
    CARAPI GetRules(
        /* [in] */ Int32 layoutDirection,
        /* [out, callee] */ ArrayOf<Int32>** rules);

    CARAPI SetAlignWithParent(
        /* [in] */ Boolean align);

    CARAPI GetAlignWithParent(
        /* [out] */ Boolean* align);
};

}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_CLINEARLAYOUTLAYOUTPARAMS_H__
