
#include "view/FocusFinder.h"
#include "view/View.h"
#include "view/ViewGroup.h"
#include "view/CViewConfiguration.h"
#include <elastos/core/Math.h>
#include <elastos/utility/logging/Logger.h>

using Elastos::Core::CObjectContainer;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace View {

static void FocusFinderDestructor(void* st)
{
    FocusFinder* obj = static_cast<FocusFinder*>(st);
    if (obj) {
        obj->Release();
    }
}

static Boolean InitsKeyInstance()
{
    ASSERT_TRUE(pthread_key_create(&FocusFinder::sKeyFocusFinder, FocusFinderDestructor) == 0);
    return TRUE;
}

pthread_key_t FocusFinder::sKeyFocusFinder;
Boolean FocusFinder::sKeyFocusFinderInitialized = InitsKeyInstance();

FocusFinder::SequentialFocusComparator::SequentialFocusComparator()
{
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mFirstRect));
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&mSecondRect));
}

void FocusFinder::SequentialFocusComparator::Recycle()
{
    mRoot = NULL;
}

void FocusFinder::SequentialFocusComparator::SetRoot(
    /* [in] */ IViewGroup* root)
{
    mRoot = root;
}

Int32 FocusFinder::SequentialFocusComparator::Compare(
    /* [in] */ IView* first,
    /* [in] */ IView* second)
{
    if (first == second) {
        return 0;
    }

    GetRect(first, mFirstRect.Get());
    GetRect(second, mSecondRect.Get());

    if (mFirstRect->mTop < mSecondRect->mTop) {
        return -1;
    }
    else if (mFirstRect->mTop > mSecondRect->mTop) {
        return 1;
    }
    else if (mFirstRect->mLeft < mSecondRect->mLeft) {
        return -1;
    }
    else if (mFirstRect->mLeft > mSecondRect->mLeft) {
        return 1;
    }
    else if (mFirstRect->mBottom < mSecondRect->mBottom) {
        return -1;
    }
    else if (mFirstRect->mBottom > mSecondRect->mBottom) {
        return 1;
    }
    else if (mFirstRect->mRight < mSecondRect->mRight) {
        return -1;
    }
    else if (mFirstRect->mRight > mSecondRect->mRight) {
        return 1;
    }
    else {
        // The view are distinct but completely coincident so we consider
        // them equal for our purposes.  Since the sort is stable, this
        // means that the views will retain their layout order relative to one another.
        return 0;
    }
}

void FocusFinder::SequentialFocusComparator::GetRect(
    /* [in] */ IView* view,
    /* [in] */ IRect* rect)
{
    view->GetDrawingRect(rect);
    mRoot->OffsetDescendantRectToMyCoords(view, rect);
}

CAR_INTERFACE_IMPL(FocusFinder, IFocusFinder);

AutoPtr<FocusFinder> FocusFinder::GetInstance()
{
    AutoPtr<FocusFinder> focusFinder = (FocusFinder*)pthread_getspecific(sKeyFocusFinder);
    if (focusFinder == NULL) {
        focusFinder = new FocusFinder();
        ASSERT_TRUE(pthread_setspecific(sKeyFocusFinder, focusFinder.Get()) == 0);
        focusFinder->AddRef();
    }
    assert(focusFinder != NULL);
    return focusFinder;
}

// enforce thread local access
FocusFinder::FocusFinder()
{
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mFocusedRect));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mOtherRect));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mBestCandidateRect));
    mSequentialFocusComparator = new SequentialFocusComparator();
}

ECode FocusFinder::FindNextFocus(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction,
    /* [out] */ IView** nextFocus)
{
    VALIDATE_NOT_NULL(nextFocus);
    AutoPtr<IView> temp = FindNextFocus(root, focused, NULL, direction);
    *nextFocus = temp;
    REFCOUNT_ADD(*nextFocus);

    return NOERROR;
}

ECode FocusFinder::FindNextFocusFromRect(
    /* [in] */ IViewGroup* root,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction,
    /* [out] */ IView** nextFocus)
{
    VALIDATE_NOT_NULL(nextFocus);
    mFocusedRect->Set(focusedRect);
    AutoPtr<IView> temp = FindNextFocus(root, NULL, mFocusedRect, direction);
    *nextFocus = temp;
    REFCOUNT_ADD(*nextFocus);

    return NOERROR;
}

AutoPtr<IView> FocusFinder::FindNextFocus(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction)
{
    AutoPtr<IView> next = NULL;
    if (focused != NULL) {
        next = FindNextUserSpecifiedFocus(root, focused, direction);
    }

    if (next != NULL) {
        return next;
    }

    List<AutoPtr<IView> >& focusables = mTempList;
    focusables.Clear();

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    root->AddFocusables(container, direction);
    AutoPtr<IObjectEnumerator> focusedIt;
    container->GetObjectEnumerator((IObjectEnumerator**)&focusedIt);
    Boolean hasNext;
    while (focusedIt->MoveNext(&hasNext), hasNext) {
        AutoPtr<IInterface> p;
        focusedIt->Current((IInterface**)&p);
        focusables.PushBack(IView::Probe(p));
    }

    if (!focusables.IsEmpty()) {
        next = FindNextFocus(root, focused, focusedRect, direction, focusables);
    }

    focusables.Clear();

    return next;
}

AutoPtr<IView> FocusFinder::FindNextUserSpecifiedFocus(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ Int32 direction)
{
    // check for user specified next focus
    AutoPtr<IView> userSetNextFocus =
        reinterpret_cast<View*>(focused->Probe(EIID_View))->FindUserSetNextFocus(root, direction);
    Boolean isFocusable, isInTouchMode, isFocusableInTouchMode;
    if (userSetNextFocus != NULL
        && (userSetNextFocus->IsFocusable(&isFocusable), isFocusable)
        && (!(userSetNextFocus->IsInTouchMode(&isInTouchMode), isInTouchMode)
        || (userSetNextFocus->IsFocusableInTouchMode(&isFocusableInTouchMode), isFocusableInTouchMode))) {
        return userSetNextFocus;
    }
    return NULL;
}

AutoPtr<IView> FocusFinder::FindNextFocus(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction,
    /* [in] */ List<AutoPtr<IView> >& focusables)
{
    if (focused != NULL) {
        if (focusedRect == NULL) {
            focusedRect = mFocusedRect;
        }
        // fill in interesting rect from focused
        focused->GetFocusedRect(focusedRect);
        root->OffsetDescendantRectToMyCoords(focused, focusedRect);
    }
    else {
        if (focusedRect == NULL) {
            focusedRect = mFocusedRect;
            Boolean isLayoutRtl;
            // make up a rect at top left or bottom right of root
            switch (direction) {
                case IView::FOCUS_RIGHT:
                case IView::FOCUS_DOWN:
                    SetFocusTopLeft(root, focusedRect);
                    break;
                case IView::FOCUS_FORWARD:
                    if (root->IsLayoutRtl(&isLayoutRtl), isLayoutRtl) {
                        SetFocusBottomRight(root, focusedRect);
                    }
                    else {
                        SetFocusTopLeft(root, focusedRect);
                    }
                    break;

                case IView::FOCUS_LEFT:
                case IView::FOCUS_UP:
                    SetFocusBottomRight(root, focusedRect);
                    break;
                case IView::FOCUS_BACKWARD:
                    if (root->IsLayoutRtl(&isLayoutRtl), isLayoutRtl) {
                        SetFocusTopLeft(root, focusedRect);
                    }
                    else {
                        SetFocusBottomRight(root, focusedRect);
                    break;
                }
            }
        }
    }

    switch (direction) {
        case IView::FOCUS_FORWARD:
        case IView::FOCUS_BACKWARD:
            return FindNextFocusInRelativeDirection(
                focusables, root, focused, focusedRect, direction);
        case IView::FOCUS_UP:
        case IView::FOCUS_DOWN:
        case IView::FOCUS_LEFT:
        case IView::FOCUS_RIGHT:
            return FindNextFocusInAbsoluteDirection(
                focusables, root, focused,focusedRect, direction);
        default:
            //throw new IllegalArgumentException("Unknown direction: " + direction);
            assert(0);
            return NULL;
    }
}

AutoPtr<IView> FocusFinder::FindNextFocusInRelativeDirection(
    /* [in] */ List<AutoPtr<IView> >& focusables,
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction)
{
    mSequentialFocusComparator->SetRoot(root);
    SortList(focusables, mSequentialFocusComparator);
    mSequentialFocusComparator->Recycle();

    const Int32 count = focusables.GetSize();
    switch (direction) {
        case IView::FOCUS_FORWARD:
            return GetForwardFocusable(root, focused, focusables, count);
        case IView::FOCUS_BACKWARD:
            return GetBackwardFocusable(root, focused, focusables, count);
    }
    return focusables[count - 1];
}

void FocusFinder::SetFocusBottomRight(
    /* [in] */ IViewGroup* root,
    /* [in] */ IRect* focusedRect)
{
    Int32 scrollX, scrollY, width, height;
    root->GetScrollX(&scrollX);
    root->GetScrollY(&scrollY);
    root->GetWidth(&width);
    root->GetHeight(&height);

    Int32 rootBottom = scrollY + height;
    Int32 rootRight = scrollX + width;

    focusedRect->Set(rootRight, rootBottom, rootRight, rootBottom);
}

void FocusFinder::SetFocusTopLeft(
    /* [in] */ IViewGroup* root,
    /* [in] */ IRect* focusedRect)
{
    Int32 rootTop, rootLeft;
    root->GetScrollY(&rootTop);
    root->GetScrollX(&rootLeft);
    focusedRect->Set(rootLeft, rootTop, rootLeft, rootTop);
}

AutoPtr<IView> FocusFinder::FindNextFocusInAbsoluteDirection(
    /* [in] */ List<AutoPtr<IView> >& focusables,
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ IRect* focusedRect,
    /* [in] */ Int32 direction)
{
    // initialize the best candidate to something impossible
    // (so the first plausible view will become the best choice)
    //
    mBestCandidateRect->Set(focusedRect);
    Int32 width, height;
    focusedRect->GetWidth(&width);
    focusedRect->GetHeight(&height);
    switch(direction) {
        case IView::FOCUS_LEFT:
            mBestCandidateRect->Offset(width + 1, 0);
            break;
        case IView::FOCUS_RIGHT:
            mBestCandidateRect->Offset(-(width + 1), 0);
            break;
        case IView::FOCUS_UP:
            mBestCandidateRect->Offset(0, height + 1);
            break;
        case IView::FOCUS_DOWN:
            mBestCandidateRect->Offset(0, -(height + 1));
            break;
        default:
            break;
    }

    AutoPtr<IView> closest;

    List<AutoPtr<IView> >::Iterator iter = focusables.Begin();
    for (; iter != focusables.End(); ++iter) {
        IView* focusable = iter->Get();

        // only interested in other non-root views
        if (focusable == focused || focusable == IView::Probe(root))
            continue;

        // get focus bounds of other view in same coordinate system
        focusable->GetFocusedRect(mOtherRect);
        root->OffsetDescendantRectToMyCoords(focusable, mOtherRect);

        if (IsBetterCandidate(direction, focusedRect, mOtherRect, mBestCandidateRect)) {
            mBestCandidateRect->Set(mOtherRect);
            closest = focusable;
        }
    }

    return closest;
}

AutoPtr<IView> FocusFinder::GetForwardFocusable(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ List<AutoPtr<IView> >& focusables,
    /* [in] */ Int32 count)
{
    Boolean isLayoutRtl;
    root->IsLayoutRtl(&isLayoutRtl);
    return isLayoutRtl ?
        GetPreviousFocusable(focused, focusables, count) :
        GetNextFocusable(focused, focusables, count);
}

AutoPtr<IView> FocusFinder::GetNextFocusable(
    /* [in] */ IView* focused,
    /* [in] */ List<AutoPtr<IView> >& focusables,
    /* [in] */ Int32 count)
{
    if (focused != NULL) {
        AutoPtr<IView> target = focused;
        List<AutoPtr<IView> >::ReverseIterator rit =
            Find(focusables.RBegin(), focusables.REnd(), target);
        if (rit != focusables.REnd() && rit.GetBase() != focusables.End()) {
            return *(rit.GetBase());
        }
    }

    if (!focusables.IsEmpty()) {
        return focusables.GetFront();
    }

    return NULL;
}

AutoPtr<IView> FocusFinder::GetBackwardFocusable(
    /* [in] */ IViewGroup* root,
    /* [in] */ IView* focused,
    /* [in] */ List<AutoPtr<IView> >& focusables,
    /* [in] */ Int32 count)
{
    Boolean isLayoutRtl;
    root->IsLayoutRtl(&isLayoutRtl);
    return isLayoutRtl ?
        GetNextFocusable(focused, focusables, count) :
        GetPreviousFocusable(focused, focusables, count);
}

AutoPtr<IView> FocusFinder::GetPreviousFocusable(
    /* [in] */ IView* focused,
    /* [in] */ List<AutoPtr<IView> >& focusables,
    /* [in] */ Int32 count)
{
    if (focused != NULL) {
        AutoPtr<IView> temp(focused);
        List<AutoPtr<IView> >::Iterator it =
            Find(focusables.Begin(), focusables.End(), temp);
        if (it != focusables.End() && it != focusables.Begin()) {
            return *(--it);
        }
    }

    if (!focusables.IsEmpty()) {
        return focusables.GetFront();
    }
    return NULL;
}

/**
 * Is rect1 a better candidate than rect2 for a focus search in a particular
 * direction from a source rect?  This is the core routine that determines
 * the order of focus searching.
 * @param direction the direction (up, down, left, right)
 * @param source The source we are searching from
 * @param rect1 The candidate rectangle
 * @param rect2 The current best candidate.
 * @return Whether the candidate is the new best.
 */
Boolean FocusFinder::IsBetterCandidate(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* rect1,
    /* [in] */ IRect* rect2)
{
    // to be a better candidate, need to at least be a candidate in the first
    // place :)
    if (!IsCandidate(source, rect1, direction)) {
        return FALSE;
    }

    // we know that rect1 is a candidate.. if rect2 is not a candidate,
    // rect1 is better
    if (!IsCandidate(source, rect2, direction)) {
        return TRUE;
    }

    // if rect1 is better by beam, it wins
    if (BeamBeats(direction, source, rect1, rect2)) {
        return TRUE;
    }

    // if rect2 is better, then rect1 cant' be :)
    if (BeamBeats(direction, source, rect2, rect1)) {
        return FALSE;
    }

    // otherwise, do fudge-tastic comparison of the major and minor axis
    return (GetWeightedDistanceFor(
                MajorAxisDistance(direction, source, rect1),
                MinorAxisDistance(direction, source, rect1))
            < GetWeightedDistanceFor(
                MajorAxisDistance(direction, source, rect2),
                MinorAxisDistance(direction, source, rect2)));
}

/**
 * One rectangle may be another candidate than another by virtue of being
 * exclusively in the beam of the source rect.
 * @return Whether rect1 is a better candidate than rect2 by virtue of it being in src's
 *      beam
 */
Boolean FocusFinder::BeamBeats(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* rect1,
    /* [in] */ IRect* rect2)
{
    const Boolean rect1InSrcBeam = BeamsOverlap(direction, source, rect1);
    const Boolean rect2InSrcBeam = BeamsOverlap(direction, source, rect2);

    // if rect1 isn't exclusively in the src beam, it doesn't win
    if (rect2InSrcBeam || !rect1InSrcBeam) {
        return FALSE;
    }

    // we know rect1 is in the beam, and rect2 is not

    // if rect1 is to the direction of, and rect2 is not, rect1 wins.
    // for example, for direction left, if rect1 is to the left of the source
    // and rect2 is below, then we always prefer the in beam rect1, since rect2
    // could be reached by going down.
    if (!IsToDirectionOf(direction, source, rect2)) {
        return TRUE;
    }

    // for horizontal directions, being exclusively in beam always wins
    if ((direction == IView::FOCUS_LEFT || direction == IView::FOCUS_RIGHT)) {
        return TRUE;
    }

    // for vertical directions, beams only beat up to a point:
    // now, as long as rect2 isn't completely closer, rect1 wins
    // e.g for direction down, completely closer means for rect2's top
    // edge to be closer to the source's top edge than rect1's bottom edge.
    return (MajorAxisDistance(direction, source, rect1)
            < MajorAxisDistanceToFarEdge(direction, source, rect2));
}

/**
 * Fudge-factor opportunity: how to calculate distance given major and minor
 * axis distances.  Warning: this fudge factor is finely tuned, be sure to
 * run all focus tests if you dare tweak it.
 */
Int32 FocusFinder::GetWeightedDistanceFor(
    /* [in] */ Int32 majorAxisDistance,
    /* [in] */ Int32 minorAxisDistance)
{
    return 13 * majorAxisDistance * majorAxisDistance
            + minorAxisDistance * minorAxisDistance;
}

/**
 * Is destRect a candidate for the next focus given the direction?  This
 * checks whether the dest is at least partially to the direction of (e.g left of)
 * from source.
 *
 * Includes an edge case for an empty rect (which is used in some cases when
 * searching from a point on the screen).
 */
Boolean FocusFinder::IsCandidate(
    /* [in] */ IRect* srcRect,
    /* [in] */ IRect* destRect,
    /* [in] */ Int32 direction)
{
    assert(srcRect && destRect);
    CRect* src = (CRect*)srcRect;
    CRect* dest = (CRect*)destRect;
    switch (direction) {
    case IView::FOCUS_LEFT:
        return (src->mRight > dest->mRight || src->mLeft >= dest->mRight)
                && src->mLeft > dest->mLeft;
    case IView::FOCUS_RIGHT:
        return (src->mLeft < dest->mLeft || src->mRight <= dest->mLeft)
                && src->mRight < dest->mRight;
    case IView::FOCUS_UP:
        return (src->mBottom > dest->mBottom || src->mTop >= dest->mBottom)
                && src->mTop > dest->mTop;
    case IView::FOCUS_DOWN:
        return (src->mTop < dest->mTop|| src->mBottom <= dest->mTop)
                && src->mBottom < dest->mBottom;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * Do the "beams" w.r.t the given direcition's axos of rect1 and rect2 overlap?
 * @param direction the direction (up, down, left, right)
 * @param rect1 The first rectangle
 * @param rect2 The second rectangle
 * @return whether the beams overlap
 */
Boolean FocusFinder::BeamsOverlap(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* rect1,
    /* [in] */ IRect* rect2)
{
    assert(rect1 && rect2);
    CRect* crect1 = (CRect*)rect1;
    CRect* crect2 = (CRect*)rect2;
    switch (direction) {
    case IView::FOCUS_LEFT:
    case IView::FOCUS_RIGHT:
        return (crect2->mBottom >= crect1->mTop) && (crect2->mTop <= crect1->mBottom);
    case IView::FOCUS_UP:
    case IView::FOCUS_DOWN:
        return (crect2->mRight >= crect1->mLeft) && (crect2->mLeft <= crect1->mRight);
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * e.g for left, is 'to left of'
 */
Boolean FocusFinder::IsToDirectionOf(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* src,
    /* [in] */ IRect* dest)
{
    assert(src && dest);
    CRect* srcRect = (CRect*)src;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case IView::FOCUS_LEFT:
        return srcRect->mLeft >= destRect->mRight;
    case IView::FOCUS_RIGHT:
        return srcRect->mRight <= destRect->mLeft;
    case IView::FOCUS_UP:
        return srcRect->mTop >= destRect->mBottom;
    case IView::FOCUS_DOWN:
        return srcRect->mBottom <= destRect->mTop;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * @return The distance from the edge furthest in the given direction
 *   of source to the edge nearest in the given direction of dest.  If the
 *   dest is not in the direction from source, return 0.
 */
Int32 FocusFinder::MajorAxisDistance(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    return Elastos::Core::Math::Max(0, MajorAxisDistanceRaw(direction, source, dest));
}

Int32 FocusFinder::MajorAxisDistanceRaw(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    assert(source && dest);
    CRect* sourceRect = (CRect*)source;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case IView::FOCUS_LEFT:
        return sourceRect->mLeft - destRect->mRight;
    case IView::FOCUS_RIGHT:
        return destRect->mLeft - sourceRect->mRight;
    case IView::FOCUS_UP:
        return sourceRect->mTop - destRect->mBottom;
    case IView::FOCUS_DOWN:
        return destRect->mTop - sourceRect->mBottom;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * @return The distance along the major axis w.r.t the direction from the
 *   edge of source to the far edge of dest. If the
 *   dest is not in the direction from source, return 1 (to break ties with
 *   {@link #majorAxisDistance}).
 */
Int32 FocusFinder::MajorAxisDistanceToFarEdge(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    return Elastos::Core::Math::Max(1, MajorAxisDistanceToFarEdgeRaw(direction, source, dest));
}

Int32 FocusFinder::MajorAxisDistanceToFarEdgeRaw(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    assert(source && dest);
    CRect* sourceRect = (CRect*)source;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case IView::FOCUS_LEFT:
        return sourceRect->mLeft - destRect->mLeft;
    case IView::FOCUS_RIGHT:
        return destRect->mRight - sourceRect->mRight;
    case IView::FOCUS_UP:
        return sourceRect->mTop - destRect->mTop;
    case IView::FOCUS_DOWN:
        return destRect->mBottom - sourceRect->mBottom;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

/**
 * Find the distance on the minor axis w.r.t the direction to the nearest
 * edge of the destination rectange.
 * @param direction the direction (up, down, left, right)
 * @param source The source rect.
 * @param dest The destination rect.
 * @return The distance.
 */
Int32 FocusFinder::MinorAxisDistance(
    /* [in] */ Int32 direction,
    /* [in] */ IRect* source,
    /* [in] */ IRect* dest)
{
    assert(source && dest);
    CRect* sourceRect = (CRect*)source;
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case IView::FOCUS_LEFT:
    case IView::FOCUS_RIGHT:
    {
        Int32 sh, dh;
        sourceRect->GetHeight(&sh);
        destRect->GetHeight(&dh);
        // the distance between the center verticals
        return Elastos::Core::Math::Abs((sourceRect->mTop + sh / 2) - (destRect->mTop + dh / 2));
    }
    case IView::FOCUS_UP:
    case IView::FOCUS_DOWN:
    {
        Int32 sw, dw;
        sourceRect->GetWidth(&sw);
        destRect->GetWidth(&dw);
        // the distance between the center horizontals
        return Elastos::Core::Math::Abs((sourceRect->mLeft + sw / 2) - (destRect->mLeft + dw / 2));
    }
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

ECode FocusFinder::FindNearestTouchable(
    /* [in] */ IViewGroup* root,
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ Int32 direction,
    /* [in] */ ArrayOf<Int32>* deltas,
    /* [out] */ IView** touchable)
{
    AutoPtr<IObjectContainer> touchables;
    root->GetTouchables((IObjectContainer**)&touchables);

    Int32 minDistance = Elastos::Core::Math::INT32_MAX_VALUE;

    AutoPtr<IContext> context;
    root->GetContext((IContext**)&context);
    assert(context.Get());
    Int32 edgeSlop;
    CViewConfiguration::Get(context)->GetScaledEdgeSlop(&edgeSlop);

    AutoPtr<CRect> closestBounds;
    ASSERT_SUCCEEDED(CRect::NewByFriend((CRect**)&closestBounds));

    CRect* touchableBounds = (CRect*)mOtherRect.Get();

    AutoPtr<IView> closest;

    AutoPtr<IObjectEnumerator> objEmu;
    touchables->GetObjectEnumerator((IObjectEnumerator**)&objEmu);

    Boolean isSucceeded = FALSE;
    while (objEmu->MoveNext(&isSucceeded), isSucceeded) {
        AutoPtr<IInterface> obj;
        objEmu->Current((IInterface**)&obj);
        IView* item = (IView*)obj->Probe(EIID_IView);

        // get visible bounds of other view in same coordinate system
        item->GetDrawingRect((IRect*)touchableBounds);

        reinterpret_cast<ViewGroup*>(root->Probe(EIID_ViewGroup))
            ->OffsetRectBetweenParentAndChild(item, touchableBounds, TRUE, TRUE);

        if (!IsTouchCandidate(x, y, touchableBounds, direction)) {
            continue;
        }

        Int32 distance = Elastos::Core::Math::INT32_MAX_VALUE;

        switch (direction) {
        case IView::FOCUS_LEFT:
            distance = x - touchableBounds->mRight + 1;
            break;
        case IView::FOCUS_RIGHT:
            distance = touchableBounds->mLeft;
            break;
        case IView::FOCUS_UP:
            distance = y - touchableBounds->mBottom + 1;
            break;
        case IView::FOCUS_DOWN:
            distance = touchableBounds->mTop;
            break;
        }

        if (distance < edgeSlop) {
            Boolean isContains1, isContains2;
            closestBounds->Contains(touchableBounds, &isContains1);
            touchableBounds->Contains(closestBounds, &isContains2);
            // Give preference to innermost views
            if (closest == NULL || isContains1 ||
                (!isContains2 && distance < minDistance)) {
                minDistance = distance;
                closest = item;
                closestBounds->Set(touchableBounds);
                switch (direction) {
                case IView::FOCUS_LEFT:
                    (*deltas)[0] = -distance;
                    break;
                case IView::FOCUS_RIGHT:
                    (*deltas)[0] = distance;
                    break;
                case IView::FOCUS_UP:
                    (*deltas)[1] = -distance;
                    break;
                case IView::FOCUS_DOWN:
                    (*deltas)[1] = distance;
                    break;
                }
            }
        }
    }

    *touchable = closest;
    REFCOUNT_ADD(*touchable);

    return NOERROR;
}

/**
 * Is destRect a candidate for the next touch given the direction?
 */
Boolean FocusFinder::IsTouchCandidate(
    /* [in] */ Int32 x,
    /* [in] */ Int32 y,
    /* [in] */ IRect* dest,
    /* [in] */ Int32 direction)
{
    assert(dest);
    CRect* destRect = (CRect*)dest;
    switch (direction) {
    case IView::FOCUS_LEFT:
        return destRect->mLeft <= x && destRect->mTop <= y && y <= destRect->mBottom;
    case IView::FOCUS_RIGHT:
        return destRect->mLeft >= x && destRect->mTop <= y && y <= destRect->mBottom;
    case IView::FOCUS_UP:
        return destRect->mTop <= y && destRect->mLeft <= x && x <= destRect->mRight;
    case IView::FOCUS_DOWN:
        return destRect->mTop >= y && destRect->mLeft <= x && x <= destRect->mRight;
    default:
        Logger::E("FocusFinder",
            "direction must be one of {FOCUS_UP, FOCUS_DOWN, FOCUS_LEFT, FOCUS_RIGHT}.");
        assert(0);
        break;
    }

    return FALSE;
}

void FocusFinder::Merge(
    /* [in] */ List<AutoPtr<IView> >& list1,
    /* [in] */ List<AutoPtr<IView> >& list2,
    /* [in] */ SequentialFocusComparator* comparator)
{
    List<AutoPtr<IView> >::Iterator first1 = list1.Begin();
    List<AutoPtr<IView> >::Iterator last1 = list1.End();
    List<AutoPtr<IView> >::Iterator first2 = list2.Begin();
    List<AutoPtr<IView> >::Iterator last2 = list2.End();
    while (first1 != last1 && first2 != last2) {
        if (comparator->Compare(first1->Get(), first2->Get()) < 0) {
            List<AutoPtr<IView> >::Iterator next = first2;
            ++next;
            list1.Splice(first1, list2, first2);
            first2 = next;
        }
        else {
            ++first1;
        }
    }
    if (first2 != last2)
        list1.Splice(last1, list2, first2, last2);
}

void FocusFinder::SortList(
    /* [in] */ List<AutoPtr<IView> >& list,
    /* [in] */ SequentialFocusComparator* comparator)
{
    // Do nothing if the list has length 0 or 1.
    if (list.GetSize() > 1) {
        List<AutoPtr<IView> > carry;
        List<AutoPtr<IView> > counter[64];
        Int32 fill = 0;
        while (!list.IsEmpty()) {
            carry.Splice(carry.Begin(), list, list.Begin());
            Int32 i = 0;
            while(i < fill && !counter[i].IsEmpty()) {
                Merge(counter[i], carry, comparator);
                carry.Swap(counter[i++]);
            }
            carry.Swap(counter[i]);
            if (i == fill) ++fill;
        }

        for (Int32 i = 1; i < fill; ++i) {
            Merge(counter[i], counter[i-1], comparator);
        }
        list.Swap(counter[fill-1]);
    }
}

} // namespace View
} // namepsace Droid
} // namespace Elastos
