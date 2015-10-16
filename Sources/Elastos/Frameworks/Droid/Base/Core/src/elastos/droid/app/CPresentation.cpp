#include "CPresentation.h"
#include "elastos/droid/R.h"

namespace Elastos {
namespace Droid {
namespace App {

IDIALOG_METHODS_IMPL(CPresentation, Presentation)
IWINDOWCALLBACK_METHODS_IMPL(CPresentation, Presentation)
IKEYEVENTCALLBACK_METHODS_IMPL(CPresentation, Presentation)

ECode CPresentation::constructor(
    /* [in] */ IContext * outerContext,
    /* [in] */ IDisplay * display)
{
    return constructor(outerContext, display, 0);
}

ECode CPresentation::constructor(
    /* [in] */ IContext * outerContext,
    /* [in] */ IDisplay * display,
    /* [in] */ Int32 theme)
{
    return Presentation::Init(outerContext, display, theme);
}

PInterface CPresentation::Probe(
    /* [in] */ REIID riid)
{
    return _CPresentation::Probe(riid);
}

ECode CPresentation::OnCreateContextMenu(
    /* [in] */ IContextMenu* menu,
    /* [in] */ IView* v,
    /* [in] */ IContextMenuInfo* menuInfo)
{
    return Presentation::OnCreateContextMenu(menu, v, menuInfo);
}

/**
 * Gets the {@link Display} that this presentation appears on.
 *
 * @return The display.
 */
ECode CPresentation::GetDisplay(
    /* [out] */ IDisplay **display)
{
    return Presentation::GetDisplay(display);
}

/**
 * Gets the {@link Resources} that should be used to inflate the layout of this presentation.
 * This resources object has been configured according to the metrics of the
 * display that the presentation appears on.
 *
 * @return The presentation resources object.
 */
ECode CPresentation::GetResources(
    /* [out] */ IResources **resources)
{
    return Presentation::GetResources(resources);
}

/**
 * Called by the system when the {@link Display} to which the presentation
 * is attached has been removed.
 *
 * The system automatically calls {@link #cancel} to dismiss the presentation
 * after sending this event.
 *
 * @see #getDisplay
 */
ECode CPresentation::OnDisplayRemoved()
{
    return Presentation::OnDisplayRemoved();
}

/**
 * Called by the system when the properties of the {@link Display} to which
 * the presentation is attached have changed.
 *
 * If the display metrics have changed (for example, if the display has been
 * resized or rotated), then the system automatically calls
 * {@link #cancel} to dismiss the presentation.
 *
 * @see #getDisplay
 */
ECode CPresentation::OnDisplayChanged()
{
    return Presentation::OnDisplayChanged();
}

}
}
}
