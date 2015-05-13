
#ifndef __SELECTION_H__
#define __SELECTION_H__

#include "ext/frameworkext.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

/**
 * Utility class for manipulating cursors and selections in CharSequences.
 * A cursor is a selection where the start and end are at the same offset.
 */
class Selection
{
private:
    Selection()
    {
        /* cannot be instantiated */
    }

public:
    /*
     * Retrieving the selection
     */

    /**
     * Return the offset of the selection anchor or cursor, or -1 if
     * there is no selection or cursor.
     */
    static CARAPI_(Int32) GetSelectionStart(
        /* [in] */ ICharSequence* text);

    /**
     * Return the offset of the selection edge or cursor, or -1 if
     * there is no selection or cursor.
     */
    static CARAPI_(Int32) GetSelectionEnd(
        /* [in] */ ICharSequence* text);

    /*
     * Setting the selection
     */

    /**
     * Set the selection anchor to <code>start</code> and the selection edge
     * to <code>stop</code>.
     */
    static CARAPI_(void) SetSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    /**
     * Move the cursor to offset <code>index</code>.
     */
    static CARAPI_(void) SetSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 index);

    /**
     * Select the entire text.
     */
    static CARAPI_(void) SelectAll(
        /* [in] */ ISpannable* text);

    /**
     * Move the selection edge to offset <code>index</code>.
     */
    static CARAPI_(void) ExtendSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 index);

    /**
     * Remove the selection or cursor, if any, from the text.
     */
    static CARAPI_(void) RemoveSelection(
        /* [in] */ ISpannable* text);

    /*
     * Moving the selection within the layout
     */

    /**
     * Move the cursor to the buffer offset physically above the current
     * offset, or return FALSE if the cursor is already on the top line.
     */
    static CARAPI_(Boolean) MoveUp(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the cursor to the buffer offset physically below the current
     * offset, or return FALSE if the cursor is already on the bottom line.
     */
    static CARAPI_(Boolean) MoveDown(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the cursor to the buffer offset physically to the left of
     * the current offset, or return FALSE if the cursor is already
     * at the left edge of the line and there is not another line to move it to.
     */
    static CARAPI_(Boolean) MoveLeft(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the cursor to the buffer offset physically to the right of
     * the current offset, or return FALSE if the cursor is already at
     * at the right edge of the line and there is not another line
     * to move it to.
     */
    static CARAPI_(Boolean) MoveRight(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the selection end to the buffer offset physically above
     * the current selection end.
     */
    static CARAPI_(Boolean) ExtendUp(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the selection end to the buffer offset physically below
     * the current selection end.
     */
    static CARAPI_(Boolean) ExtendDown(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the selection end to the buffer offset physically to the left of
     * the current selection end.
     */
    static CARAPI_(Boolean) ExtendLeft(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /**
     * Move the selection end to the buffer offset physically to the right of
     * the current selection end.
     */
    static CARAPI_(Boolean) ExtendRight(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    static CARAPI_(Boolean) ExtendToLeftEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    static CARAPI_(Boolean) ExtendToRightEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    static CARAPI_(Boolean) MoveToLeftEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    static CARAPI_(Boolean) MoveToRightEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout);

    /** {@hide} */
    static CARAPI_(Boolean) MoveToPreceding(
        /* [in] */ ISpannable* text,
        /* [in] */ ISelectionPositionIterator* iter,
        /* [in] */ Boolean extendSelection);

    /** {@hide} */
    static CARAPI_(Boolean) MoveToFollowing(
        /* [in] */ ISpannable* text,
        /* [in] */ ISelectionPositionIterator* iter,
        /* [in] */ Boolean extendSelection);

private:
    static CARAPI_(Int32) FindEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [in] */ Int32 dir);

    static CARAPI_(Int32) ChooseHorizontal(
        /* [in] */ ILayout* layout,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 off1,
        /* [in] */ Int32 off2);

private:
    class SelectionObject : public ElRefBase, public INoCopySpan
    {
    public:
        CARAPI_(PInterface) Probe(
            /* [in]  */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface *pObject,
            /* [out] */ InterfaceID *pIID);
    };

    /*
     * Public constants
     */

public:
    static AutoPtr<IInterface> SELECTION_START;
    static AutoPtr<IInterface> SELECTION_END;
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif //__SELECTION_H__
