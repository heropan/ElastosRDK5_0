#include "CAbsListViewLayoutParams.h"

using Elastos::Droid::Graphics::ICanvas;

namespace Elastos {
namespace Droid {
namespace Widget {

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ IContext* c,
    /* [in] */ IAttributeSet* attrs)
{
    AbsListViewLayoutParams::Init(c, attrs);
    return NOERROR;
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h)
{
    AbsListViewLayoutParams::Init(w, h);
    return NOERROR;
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ Int32 w,
    /* [in] */ Int32 h,
    /* [in] */ Int32 viewType)
{
    AbsListViewLayoutParams::Init(w, h, viewType);
    return NOERROR;
}

ECode CAbsListViewLayoutParams::constructor(
    /* [in] */ IViewGroupLayoutParams* source)
{
    AbsListViewLayoutParams::Init(source);
    return NOERROR;
}

ECode CAbsListViewLayoutParams::GetViewType(
    /* [out] */ Int32* viewType)
{
    return AbsListViewLayoutParams::GetViewType(viewType);
}

ECode CAbsListViewLayoutParams::SetViewType(
    /* [in] */ Int32 viewType)
{
    return AbsListViewLayoutParams::SetViewType(viewType);
}

ECode CAbsListViewLayoutParams::GetRecycledHeaderFooter(
    /* [out] */ Boolean* footer)
{
    return AbsListViewLayoutParams::GetRecycledHeaderFooter(footer);
}

ECode CAbsListViewLayoutParams::SetRecycledHeaderFooter(
    /* [in] */ Boolean footer)
{
    return AbsListViewLayoutParams::SetRecycledHeaderFooter(footer);
}

ECode CAbsListViewLayoutParams::GetForceAdd(
    /* [out] */ Boolean* force)
{
    return AbsListViewLayoutParams::GetForceAdd(force);
}

ECode CAbsListViewLayoutParams::SetForceAdd(
    /* [in] */ Boolean force)
{
    return AbsListViewLayoutParams::SetForceAdd(force);
}

ECode CAbsListViewLayoutParams::GetItemId(
    /* [out] */ Int32* itemId)
{
    return AbsListViewLayoutParams::GetItemId(itemId);
}

ECode CAbsListViewLayoutParams::SetItemId(
    /* [in] */ Int32 itemId)
{
    return AbsListViewLayoutParams::SetItemId(itemId);
}

ECode CAbsListViewLayoutParams::GetScrappedFromPosition(
    /* [out] */ Int32* scrappedFromPosition)
{
    return AbsListViewLayoutParams::GetScrappedFromPosition(scrappedFromPosition);
}

ECode CAbsListViewLayoutParams::SetScrappedFromPosition(
    /* [in] */  Int32 scrappedFromPosition)
{
    return AbsListViewLayoutParams::SetScrappedFromPosition(scrappedFromPosition);
}

ECode CAbsListViewLayoutParams::SetWidth(
    /* [in] */ Int32 width)
{
    return AbsListViewLayoutParams::SetWidth(width);
}

ECode CAbsListViewLayoutParams::SetHeight(
    /* [in] */ Int32 height)
{
    return AbsListViewLayoutParams::SetHeight(height);
}

ECode CAbsListViewLayoutParams::GetWidth(
    /* [out] */ Int32* width)
{
    *width = AbsListViewLayoutParams::GetWidth();
    return NOERROR;
}

ECode CAbsListViewLayoutParams::GetHeight(
    /* [out] */ Int32* height)
{
    *height = AbsListViewLayoutParams::GetHeight();
    return NOERROR;
}

ECode CAbsListViewLayoutParams::SetLayoutAnimationParameters(
    /* [in] */ IAnimationParameters* ap)
{
    return AbsListViewLayoutParams::SetLayoutAnimationParameters(ap);
}

ECode CAbsListViewLayoutParams::GetLayoutAnimationParameters(
    /* [out] */ IAnimationParameters** ap)
{
    return AbsListViewLayoutParams::GetLayoutAnimationParameters(ap);
}

/**
 * Resolve layout parameters depending on the layout direction. Subclasses that care about
 * layoutDirection changes should override this method. The default implementation does
 * nothing.
 *
 * @param layoutDirection the direction of the layout
 *
 * {@link View#LAYOUT_DIRECTION_LTR}
 * {@link View#LAYOUT_DIRECTION_RTL}
 */
ECode CAbsListViewLayoutParams::ResolveLayoutDirection(
    /* [in] */ Int32 layoutDirection)
{
    return AbsListViewLayoutParams::ResolveLayoutDirection(layoutDirection);
}

/**
 * Use {@code canvas} to draw suitable debugging annotations for these LayoutParameters.
 *
 * @param view the view that contains these layout parameters
 * @param canvas the canvas on which to draw
 *
 * @hide
 */
ECode CAbsListViewLayoutParams::OnDebugDraw(
    /* [in] */ IView* view,
    /* [in] */ ICanvas* canvas)
{
    return AbsListViewLayoutParams::OnDebugDraw(view, canvas);
}
}// namespace Widget
}// namespace Droid
}// namespoace Elastos
