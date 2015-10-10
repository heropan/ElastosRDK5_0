
#ifndef __ELASTOS_DROID_TEXT_CSELECTION_H__
#define __ELASTOS_DROID_TEXT_CSELECTION_H__

#include "_Elastos_Droid_Text_CSelection.h"
#include <elastos/core/Object.h>

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace Text {

CarClass(CSelection)
{
public:
    CARAPI GetSelectionStart(
        /* [in] */ ICharSequence* text,
        /* [out] */ Int32* start);

    CARAPI GetSelectionEnd(
        /* [in] */ ICharSequence* text,
        /* [out] */ Int32* end);

    CARAPI SetSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 start,
        /* [in] */ Int32 stop);

    CARAPI SetSelection2(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 index);

    CARAPI SelectAll(
        /* [in] */ ISpannable* text);

    CARAPI ExtendSelection(
        /* [in] */ ISpannable* text,
        /* [in] */ Int32 index);

    CARAPI RemoveSelection(
        /* [in] */ ISpannable* text);

    CARAPI MoveUp(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveDown(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveLeft(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveRight(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendUp(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendDown(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendLeft(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendRight(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendToLeftEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI ExtendToRightEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveToLeftEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    CARAPI MoveToRightEdge(
        /* [in] */ ISpannable* text,
        /* [in] */ ILayout* layout,
        /* [out] */ Boolean* result);

    /** {@hide} */
    CARAPI MoveToPreceding(
        /* [in] */ ISpannable* text,
        /* [in] */ ISelectionPositionIterator* iter,
        /* [in] */ Boolean extendSelection,
        /* [out] */ Boolean* result);

    /** {@hide} */
    CARAPI MoveToFollowing(
        /* [in] */ ISpannable* text,
        /* [in] */ ISelectionPositionIterator* iter,
        /* [in] */ Boolean extendSelection,
        /* [in] */ Boolean* result);

    CARAPI GetSelectionStartObject(
        /* [out] */ IInterface** start);

    CARAPI GetSelectionEndObject(
        /* [out] */ IInterface** end);
};

} // namespace Text
} // namepsace Droid
} // namespace Elastos

#endif // __CSELECTIONHELPER_H__
