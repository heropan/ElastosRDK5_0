#ifndef __ELASTOS_DROID_WIDGET_INTERNAL_CSLIDINGTAB_H__
#define __ELASTOS_DROID_WIDGET_INTERNAL_CSLIDINGTAB_H__

#include "_Elastos_Droid_Widget_Internal_CSlidingTab.h"

#include "elastos/droid/widget/internal/SlidingTab.h"

namespace Elastos {
namespace Droid {
namespace Widget {
namespace Internal {

CarClass(CSlidingTab), public SlidingTab
{
public:
    IVIEW_METHODS_DECL()

    IVIEWGROUP_METHODS_DECL()

    IVIEWPARENT_METHODS_DECL()

    IVIEWMANAGER_METHODS_DECL()

    IDRAWABLECALLBACK_METHODS_DECL()

    IKEYEVENTCALLBACK_METHODS_DECL()

    IACCESSIBILITYEVENTSOURCE_METHODS_DECL()

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI constructor(
        /* [in] */ IContext* context);

    CARAPI constructor(
        /* [in] */ IContext* context,
        /* [in] */ IAttributeSet* attrs);

    /**
     * Reset the tabs to their original state and stop any existing animation.
     * Animate them back into place if animate is true.
     *
     * @param animate
     */
    CARAPI Reset(
        /* [in] */ Boolean animate);

    /**
     * Sets the left handle icon to a given resource.
     *
     * The resource should refer to a Drawable object, or use 0 to remove
     * the icon.
     *
     * @param iconId the resource ID of the icon drawable
     * @param targetId the resource of the target drawable
     * @param barId the resource of the bar drawable (stateful)
     * @param tabId the resource of the
     */
    CARAPI SetLeftTabResources(
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 targetId,
        /* [in] */ Int32 barId,
        /* [in] */ Int32 tabId);

    /**
     * Sets the left handle hint text to a given resource string.
     *
     * @param resId
     */
    CARAPI SetLeftHintText(
        /* [in] */ Int32 resId);

    /**
     * Sets the right handle icon to a given resource.
     *
     * The resource should refer to a Drawable object, or use 0 to remove
     * the icon.
     *
     * @param iconId the resource ID of the icon drawable
     * @param targetId the resource of the target drawable
     * @param barId the resource of the bar drawable (stateful)
     * @param tabId the resource of the
     */
    CARAPI SetRightTabResources(
        /* [in] */ Int32 iconId,
        /* [in] */ Int32 targetId,
        /* [in] */ Int32 barId,
        /* [in] */ Int32 tabId);

    /**
     * Sets the left handle hint text to a given resource string.
     *
     * @param resId
     */
    CARAPI SetRightHintText(
        /* [in] */ Int32 resId);

    CARAPI SetHoldAfterTrigger(
        /* [in] */ Boolean holdLeft,
        /* [in] */ Boolean holdRight);

    /**
     * Registers a callback to be invoked when the user triggers an event.
     *
     * @param listener the OnDialTriggerListener to attach to this view
     */
    CARAPI SetISlidingTabOnTriggerListener(
        /* [in] */ ISlidingTabOnTriggerListener* listener);
};

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos

#endif //__ELASTOS_DROID_WIDGET_INTERNAL_CSLIDINGTAB_H__
