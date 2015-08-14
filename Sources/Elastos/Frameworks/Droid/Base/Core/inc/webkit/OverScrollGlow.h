
#ifndef __ELASTOS_DROID_WEBKIT_OVERSCROLLGLOW_H__
#define __ELASTOS_DROID_WEBKIT_OVERSCROLLGLOW_H__

#include "ext/frameworkext.h"

using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Widget::IEdgeEffect;

namespace Elastos {
namespace Droid {
namespace Webkit {

class CWebViewClassic;

/**
 * This class manages the edge glow effect when a WebView is flung or pulled beyond the edges.
 * @hide
 */
class OverScrollGlow : public ElRefBase
{
 public:
    OverScrollGlow(
        /* [in] */ CWebViewClassic* host);

    /**
     * Pull leftover touch scroll distance into one of the edge glows as appropriate.
     *
     * @param x Current X scroll offset
     * @param y Current Y scroll offset
     * @param oldX Old X scroll offset
     * @param oldY Old Y scroll offset
     * @param maxX Maximum range for horizontal scrolling
     * @param maxY Maximum range for vertical scrolling
     */
    virtual CARAPI_(void) PullGlow(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 oldX,
        /* [in] */ Int32 oldY,
        /* [in] */ Int32 maxX,
        /* [in] */ Int32 maxY);

    /**
     * Set touch delta values indicating the current amount of overscroll.
     *
     * @param deltaX
     * @param deltaY
     */
    virtual CARAPI_(void) SetOverScrollDeltas(
        /* [in] */ Int32 deltaX,
        /* [in] */ Int32 deltaY);

    /**
     * Absorb leftover fling velocity into one of the edge glows as appropriate.
     *
     * @param x Current X scroll offset
     * @param y Current Y scroll offset
     * @param oldX Old X scroll offset
     * @param oldY Old Y scroll offset
     * @param rangeX Maximum range for horizontal scrolling
     * @param rangeY Maximum range for vertical scrolling
     */
    virtual CARAPI_(void) AbsorbGlow(
        /* [in] */ Int32 x, Int32 y,
        /* [in] */ Int32 oldX,
        /* [in] */ Int32 oldY,
        /* [in] */ Int32 rangeX,
        /* [in] */ Int32 rangeY);

    /**
     * Draw the glow effect along the sides of the widget. mEdgeGlow* must be non-null.
     *
     * @param canvas Canvas to draw into, transformed into view coordinates.
     * @return true if glow effects are still animating and the view should invalidate again.
     */
    virtual CARAPI_(Boolean) DrawEdgeGlows(
        /* [in] */ ICanvas* canvas);

    /**
     * @return True if any glow is still animating
     */
    virtual CARAPI_(Boolean) IsAnimating();

    /**
     * Release all glows from any touch pulls in progress.
     */
    virtual CARAPI_(void) ReleaseAll();

private:
    AutoPtr<CWebViewClassic> mHostView;

    AutoPtr<IEdgeEffect> mEdgeGlowTop;
    AutoPtr<IEdgeEffect> mEdgeGlowBottom;
    AutoPtr<IEdgeEffect> mEdgeGlowLeft;
    AutoPtr<IEdgeEffect> mEdgeGlowRight;

    Int32 mOverScrollDeltaX;
    Int32 mOverScrollDeltaY;
};

} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_WEBKIT_OVERSCROLLGLOW_H__
