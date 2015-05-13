#include "CSlidingTab.h"

using Elastos::Droid::View::EIID_View;
using Elastos::Droid::View::EIID_ViewGroup;

namespace Elastos{
namespace Droid{
namespace Widget{
namespace Internal{

IVIEW_METHODS_IMPL(CSlidingTab, SlidingTab)

IVIEWGROUP_METHODS_IMPL(CSlidingTab, SlidingTab)

IVIEWPARENT_METHODS_IMPL(CSlidingTab, SlidingTab)

IVIEWMANAGER_METHODS_IMPL(CSlidingTab, SlidingTab)

IDRAWABLECALLBACK_METHODS_IMPL(CSlidingTab, SlidingTab)

IKEYEVENTCALLBACK_METHODS_IMPL(CSlidingTab, SlidingTab)

IACCESSIBILITYEVENTSOURCE_METHODS_IMPL(CSlidingTab, SlidingTab)

PInterface CSlidingTab::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_View) {
        return reinterpret_cast<PInterface>((View*)this);
    }
    if (riid == EIID_ViewGroup) {
        return reinterpret_cast<PInterface>((ViewGroup*)this);
    }
    return _CSlidingTab::Probe(riid);
}

ECode CSlidingTab::constructor(
    /* [in] */ IContext* context)
{
    return SlidingTab::Init(context);
}

ECode CSlidingTab::constructor(
    /* [in] */ IContext* context,
    /* [in] */ IAttributeSet* attrs)
{
    return SlidingTab::Init(context, attrs);
}

/**
 * Reset the tabs to their original state and stop any existing animation.
 * Animate them back into place if animate is true.
 *
 * @param animate
 */
ECode CSlidingTab::Reset(
    /* [in] */ Boolean animate)
{
    return SlidingTab::Reset(animate);
}

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
ECode CSlidingTab::SetLeftTabResources(
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 targetId,
    /* [in] */ Int32 barId,
    /* [in] */ Int32 tabId)
{
    return SlidingTab::SetLeftTabResources(iconId, targetId, barId, tabId);
}

/**
 * Sets the left handle hint text to a given resource string.
 *
 * @param resId
 */
ECode CSlidingTab::SetLeftHintText(
    /* [in] */ Int32 resId)
{
    return SlidingTab::SetLeftHintText(resId);
}

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
ECode CSlidingTab::SetRightTabResources(
    /* [in] */ Int32 iconId,
    /* [in] */ Int32 targetId,
    /* [in] */ Int32 barId,
    /* [in] */ Int32 tabId)
{
    return SlidingTab::SetRightTabResources(iconId, targetId, barId, tabId);
}

/**
 * Sets the left handle hint text to a given resource string.
 *
 * @param resId
 */
ECode CSlidingTab::SetRightHintText(
    /* [in] */ Int32 resId)
{
    return SlidingTab::SetRightHintText(resId);
}

ECode CSlidingTab::SetHoldAfterTrigger(
    /* [in] */ Boolean holdLeft,
    /* [in] */ Boolean holdRight)
{
    return SlidingTab::SetHoldAfterTrigger(holdLeft, holdRight);
}

/**
 * Registers a callback to be invoked when the user triggers an event.
 *
 * @param listener the OnDialTriggerListener to attach to this view
 */
ECode CSlidingTab::SetISlidingTabOnTriggerListener(
    /* [in] */ ISlidingTabOnTriggerListener* listener)
{
    return SlidingTab::SetOnTriggerListener(listener);
}

}// namespace Internal
}// namespace Widget
}// namespace Droid
}// namespace Elastos
