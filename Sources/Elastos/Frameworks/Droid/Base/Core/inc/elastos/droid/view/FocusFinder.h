
#ifndef __ELASTOS_DROID_VIEW_FOCUSFINDER_H__
#define __ELASTOS_DROID_VIEW_FOCUSFINDER_H__

#include "elastos/droid/ext/frameworkext.h"
#include "elastos/droid/graphics/CRect.h"
#include <pthread.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace View {

/**
 * The algorithm used for finding the next focusable view in a given direction
 * from a view that currently has focus.
 */
class FocusFinder :
    public ElRefBase,
    public IFocusFinder
{
private:
    /**
     * Sorts views according to their visual layout and geometry for default tab order.
     * This is used for sequential focus traversal.
     */
    class SequentialFocusComparator : public ElRefBase
    {
    public:
        SequentialFocusComparator();

        void Recycle();

        void SetRoot(
            /* [in] */ IViewGroup* root);

        Int32 Compare(
            /* [in] */ IView* first,
            /* [in] */ IView* second);

    private:
        void GetRect(
            /* [in] */ IView* view,
            /* [in] */ IRect* rect);

    private:
        AutoPtr<CRect> mFirstRect;
        AutoPtr<CRect> mSecondRect;
        AutoPtr<IViewGroup> mRoot;
    };

public:
    /**
     * Get the focus finder for this thread.
     */
    static CARAPI_(AutoPtr<FocusFinder>) GetInstance();

protected:
    static CARAPI_(Int32) MajorAxisDistance(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MajorAxisDistanceRaw(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MajorAxisDistanceToFarEdge(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MajorAxisDistanceToFarEdgeRaw(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

    static CARAPI_(Int32) MinorAxisDistance(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* dest);

private:
    static CARAPI_(AutoPtr<IView>) GetForwardFocusable(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ List<AutoPtr<IView> >& focusables,
        /* [in] */ Int32 count);

    static CARAPI_(AutoPtr<IView>) GetNextFocusable(
        /* [in] */ IView* focused,
        /* [in] */ List<AutoPtr<IView> >& focusables,
        /* [in] */ Int32 count);

    static CARAPI_(AutoPtr<IView>) GetBackwardFocusable(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ List<AutoPtr<IView> >& focusables,
        /* [in] */ Int32 count);

    static CARAPI_(AutoPtr<IView>) GetPreviousFocusable(
        /* [in] */ IView* focused,
        /* [in] */ List<AutoPtr<IView> >& focusables,
        /* [in] */ Int32 count);

    // add by Elastos
    static CARAPI_(void) Merge(
        /* [in] */ List<AutoPtr<IView> >& list1,
        /* [in] */ List<AutoPtr<IView> >& list2,
        /* [in] */ SequentialFocusComparator* comparator);

    static CARAPI_(void) SortList(
        /* [in] */ List<AutoPtr<IView> >& list,
        /* [in] */ SequentialFocusComparator* comparator);

private:
    FocusFinder();

public:
    CAR_INTERFACE_DECL();

    /**
     * Find the next view to take focus in root's descendants, starting from the view
     * that currently is focused.
     * @param root Contains focused. Cannot be null.
     * @param focused Has focus now.
     * @param direction Direction to look.
     * @return The next focusable view, or null if none exists.
     */
    CARAPI FindNextFocus(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction,
        /* [out] */ IView** nextFocus);

    /**
     * Find the next view to take focus in root's descendants, searching from
     * a particular rectangle in root's coordinates.
     * @param root Contains focusedRect. Cannot be null.
     * @param focusedRect The starting point of the search.
     * @param direction Direction to look.
     * @return The next focusable view, or null if none exists.
     */
    CARAPI FindNextFocusFromRect(
        /* [in] */ IViewGroup* root,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction,
        /* [out] */ IView** nextFocus);

    /**
     * Find the nearest touchable view to the specified view.
     *
     * @param root The root of the tree in which to search
     * @param x X coordinate from which to start the search
     * @param y Y coordinate from which to start the search
     * @param direction Direction to look
     * @param deltas Offset from the <x, y> to the edge of the nearest view. Note that this array
     *        may already be populated with values.
     * @return The nearest touchable view, or null if none exists.
     */
    CARAPI FindNearestTouchable(
        /* [in] */ IViewGroup* root,
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ Int32 direction,
        /* [in] */ ArrayOf<Int32>* deltas,
        /* [out] */ IView** touchable);

protected:
    CARAPI_(AutoPtr<IView>) FindNextFocusInAbsoluteDirection(
        /* [in] */ List<AutoPtr<IView> >& focusables,
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction);

    CARAPI_(Boolean) IsBetterCandidate(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* rect1,
        /* [in] */ IRect* rect2);

    CARAPI_(Boolean) BeamBeats(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* source,
        /* [in] */ IRect* rect1,
        /* [in] */ IRect* rect2);

    CARAPI_(Int32) GetWeightedDistanceFor(
        /* [in] */ Int32 majorAxisDistance,
        /* [in] */ Int32 minorAxisDistance);

    CARAPI_(Boolean) IsCandidate(
        /* [in] */ IRect* srcRect,
        /* [in] */ IRect* destRect,
        /* [in] */ Int32 direction);

    CARAPI_(Boolean) BeamsOverlap(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* rect1,
        /* [in] */ IRect* rect2);

    CARAPI_(Boolean) IsToDirectionOf(
        /* [in] */ Int32 direction,
        /* [in] */ IRect* src,
        /* [in] */ IRect* dest);

private:
    CARAPI_(AutoPtr<IView>) FindNextFocus(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction);

    CARAPI_(AutoPtr<IView>) FindNextUserSpecifiedFocus(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ Int32 direction);

    CARAPI_(AutoPtr<IView>) FindNextFocus(
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction,
        /* [in] */ List<AutoPtr<IView> >& focusables);

    CARAPI_(AutoPtr<IView>) FindNextFocusInRelativeDirection(
        /* [in] */ List<AutoPtr<IView> >& focusables,
        /* [in] */ IViewGroup* root,
        /* [in] */ IView* focused,
        /* [in] */ IRect* focusedRect,
        /* [in] */ Int32 direction);

    CARAPI_(void) SetFocusBottomRight(
        /* [in] */ IViewGroup* root,
        /* [in] */ IRect* focusedRect);

    CARAPI_(void) SetFocusTopLeft(
        /* [in] */ IViewGroup* root,
        /* [in] */ IRect* focusedRect);


    CARAPI_(Boolean) IsTouchCandidate(
        /* [in] */ Int32 x,
        /* [in] */ Int32 y,
        /* [in] */ IRect* dest,
        /* [in] */ Int32 direction);

public:
    static pthread_key_t sKeyFocusFinder;
protected:
    AutoPtr<IRect> mFocusedRect;
    AutoPtr<IRect> mOtherRect;
    AutoPtr<IRect> mBestCandidateRect;

    AutoPtr<SequentialFocusComparator> mSequentialFocusComparator;

private:
    static Boolean sKeyFocusFinderInitialized;
    List<AutoPtr<IView> > mTempList;
};

} // namespace View
} // namepsace Droid
} // namespace Elastos

#endif  //__ELASTOS_DROID_VIEW_FOCUSFINDER_H__
