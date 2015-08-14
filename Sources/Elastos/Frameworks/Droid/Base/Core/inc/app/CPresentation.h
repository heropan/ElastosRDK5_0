#ifndef __ELASTOS_DROID_APP_CPRESENTATION_H__
#define __ELASTOS_DROID_APP_CPRESENTATION_H__

#include "_CPresentation.h"
#include "Presentation.h"




using Elastos::Droid::View::IDisplay;
using Elastos::Droid::Content::IContext;


namespace Elastos{
namespace Droid{
namespace App{

CarClass(CPresentation), public Presentation
{
public:
    IDIALOG_METHODS_DECL()
    IWINDOWCALLBACK_METHODS_DECL()
    IKEYEVENTCALLBACK_METHODS_DECL()

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IDisplay * display);

    CARAPI constructor(
        /* [in] */ IContext * context,
        /* [in] */ IDisplay * display,
        /* [in] */ Int32 theme);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI OnCreateContextMenu(
        /* [in] */ IContextMenu* menu,
        /* [in] */ IView* v,
        /* [in] */ IContextMenuInfo* menuInfo);

    /**
     * Gets the {@link Display} that this presentation appears on.
     *
     * @return The display.
     */
    CARAPI GetDisplay(
        /* [out] */ IDisplay **display);

    /**
     * Gets the {@link Resources} that should be used to inflate the layout of this presentation.
     * This resources object has been configured according to the metrics of the
     * display that the presentation appears on.
     *
     * @return The presentation resources object.
     */
    CARAPI GetResources(
        /* [out] */ IResources **resources);

    /**
     * Called by the system when the {@link Display} to which the presentation
     * is attached has been removed.
     *
     * The system automatically calls {@link #cancel} to dismiss the presentation
     * after sending this event.
     *
     * @see #getDisplay
     */
    CARAPI OnDisplayRemoved();

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
    CARAPI OnDisplayChanged();
};

} // namespace App
} // namespace Droid
} // namespace Elastos

#endif // __ELASTOS_DROID_APP_CPRESENTATION_H__
