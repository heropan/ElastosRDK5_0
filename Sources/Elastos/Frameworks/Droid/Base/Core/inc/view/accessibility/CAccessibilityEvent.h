#ifndef __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYEVENT_H__
#define __ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYEVENT_H__

#include "ext/frameworkdef.h"
#include "_Elastos_Droid_View_Accessibility_CAccessibilityEvent.h"
#include "view/accessibility/AccessibilityRecord.h"
#include <elastos/utility/etl/List.h>

using Elastos::Core::ICharSequence;
using Elastos::Utility::Etl::List;
using Elastos::Droid::View::IView;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityEvent), public AccessibilityRecord
{
public:
    CAccessibilityEvent();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    /**
     * Sets the event source.
     *
     * @param source The source.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetSource(
        /* [in] */ IView* source);

    /**
     * Sets the source to be a virtual descendant of the given <code>root</code>.
     * If <code>virtualDescendantId</code> equals to {@link View#NO_ID} the root
     * is set as the source.
     * <p>
     * A virtual descendant is an imaginary View that is reported as a part of the view
     * hierarchy for accessibility purposes. This enables custom views that draw complex
     * content to report them selves as a tree of virtual views, thus conveying their
     * logical structure.
     * </p>
     *
     * @param root The root of the virtual subtree.
     * @param virtualDescendantId The id of the virtual descendant.
     */
    CARAPI SetSource(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId);

    /**
     * Gets the {@link AccessibilityNodeInfo} of the event source.
     * <p>
     *   <strong>Note:</strong> It is a client responsibility to recycle the received info
     *   by calling {@link AccessibilityNodeInfo#recycle() AccessibilityNodeInfo#recycle()}
     *   to avoid creating of multiple instances.
     * </p>
     * @return The info of the source.
     */
    CARAPI GetSource(
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Sets the window id.
     *
     * @param windowId The window id.
     *
     * @hide
     */
    CARAPI SetWindowId(
        /* [in] */ Int32 windowId);

    /**
     * Gets the id of the window from which the event comes from.
     *
     * @return The window id.
     */
    CARAPI GetWindowId(
        /* [out] */ Int32* windowId);

    /**
     * Gets if the source is checked.
     *
     * @return True if the view is checked, false otherwise.
     */
    CARAPI IsChecked(
        /* [out] */ Boolean* checked);

    /**
     * Sets if the source is checked.
     *
     * @param isChecked True if the view is checked, false otherwise.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetChecked(
        /* [in] */ Boolean isChecked);

    /**
     * Gets if the source is enabled.
     *
     * @return True if the view is enabled, false otherwise.
     */
    CARAPI IsEnabled(
        /* [out] */ Boolean* enabled);

    /**
     * Sets if the source is enabled.
     *
     * @param isEnabled True if the view is enabled, false otherwise.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    /**
     * Gets if the source is a password field.
     *
     * @return True if the view is a password field, false otherwise.
     */
    CARAPI IsPassword(
        /* [out] */ Boolean* password);

    /**
     * Sets if the source is a password field.
     *
     * @param isPassword True if the view is a password field, false otherwise.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetPassword(
        /* [in] */ Boolean password);

    /**
     * Gets if the source is taking the entire screen.
     *
     * @return True if the source is full screen, false otherwise.
     */
    CARAPI IsFullScreen(
        /* [out] */ Boolean* isFull);

    /**
     * Sets if the source is taking the entire screen.
     *
     * @param isFullScreen True if the source is full screen, false otherwise.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetFullScreen(
        /* [in] */ Boolean full);

    /**
     * Gets if the source is scrollable.
     *
     * @return True if the source is scrollable, false otherwise.
     */
    CARAPI IsScrollable(
        /* [out] */ Boolean* scrollable);

    /**
     * Sets if the source is scrollable.
     *
     * @param scrollable True if the source is scrollable, false otherwise.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetScrollable(
        /* [in] */ Boolean scrollable);

    /**
     * Gets if the source is important for accessibility.
     *
     * <strong>Note:</strong> Used only internally to determine whether
     * to deliver the event to a given accessibility service since some
     * services may want to regard all views for accessibility while others
     * may want to regard only the important views for accessibility.
     *
     * @return True if the source is important for accessibility,
     *        false otherwise.
     *
     * @hide
     */
    CARAPI IsImportantForAccessibility(
        /* [out] */ Boolean* important);

    /**
     * Gets the number of items that can be visited.
     *
     * @return The number of items.
     */
    CARAPI GetItemCount(
        /* [out] */ Int32* count);

    /**
     * Sets the number of items that can be visited.
     *
     * @param itemCount The number of items.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetItemCount(
        /* [in] */ Int32 count);

    /**
     * Gets the index of the source in the list of items the can be visited.
     *
     * @return The current item index.
     */
    CARAPI GetCurrentItemIndex(
        /* [out] */ Int32* index);

    /**
     * Sets the index of the source in the list of items that can be visited.
     *
     * @param currentItemIndex The current item index.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetCurrentItemIndex(
        /* [in] */ Int32 index);

    /**
     * Gets the index of the first character of the changed sequence,
     * or the beginning of a text selection or the index of the first
     * visible item when scrolling.
     *
     * @return The index of the first character or selection
     *        start or the first visible item.
     */
    CARAPI GetFromIndex(
        /* [out] */ Int32* index);

    /**
     * Sets the index of the first character of the changed sequence
     * or the beginning of a text selection or the index of the first
     * visible item when scrolling.
     *
     * @param fromIndex The index of the first character or selection
     *        start or the first visible item.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetFromIndex(
        /* [in] */ Int32 index);

    /**
     * Gets the index of text selection end or the index of the last
     * visible item when scrolling.
     *
     * @return The index of selection end or last item index.
     */
    CARAPI GetToIndex(
        /* [out] */ Int32* index);

    /**
     * Sets the index of text selection end or the index of the last
     * visible item when scrolling.
     *
     * @param toIndex The index of selection end or last item index.
     */
    CARAPI SetToIndex(
        /* [in] */ Int32 index);

    /**
     * Gets the scroll offset of the source left edge in pixels.
     *
     * @return The scroll.
     */
    CARAPI GetScrollX(
        /* [out] */ Int32* x);

    /**
     * Sets the scroll offset of the source left edge in pixels.
     *
     * @param scrollX The scroll.
     */
    CARAPI SetScrollX(
        /* [in] */ Int32 x);

    /**
     * Gets the scroll offset of the source top edge in pixels.
     *
     * @return The scroll.
     */
    CARAPI GetScrollY(
        /* [out] */ Int32* y);

    /**
     * Sets the scroll offset of the source top edge in pixels.
     *
     * @param scrollY The scroll.
     */
    CARAPI SetScrollY(
        /* [in] */ Int32 y);

    /**
     * Gets the max scroll offset of the source left edge in pixels.
     *
     * @return The max scroll.
     */
    CARAPI GetMaxScrollX(
        /* [out] */ Int32* x);

    /**
     * Sets the max scroll offset of the source left edge in pixels.
     *
     * @param maxScrollX The max scroll.
     */
    CARAPI SetMaxScrollX(
        /* [in] */ Int32 x);

    /**
     * Gets the max scroll offset of the source top edge in pixels.
     *
     * @return The max scroll.
     */
    CARAPI GetMaxScrollY(
        /* [out] */ Int32* y);

    /**
     * Sets the max scroll offset of the source top edge in pixels.
     *
     * @param maxScrollY The max scroll.
     */
    CARAPI SetMaxScrollY(
        /* [in] */ Int32 y);

    /**
     * Gets the number of added characters.
     *
     * @return The number of added characters.
     */
    CARAPI GetAddedCount(
        /* [out] */ Int32* count);

    /**
     * Sets the number of added characters.
     *
     * @param addedCount The number of added characters.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetAddedCount(
        /* [in] */ Int32 count);

    /**
     * Gets the number of removed characters.
     *
     * @return The number of removed characters.
     */
    CARAPI GetRemovedCount(
        /* [out] */ Int32* count);

    /**
     * Sets the number of removed characters.
     *
     * @param removedCount The number of removed characters.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetRemovedCount(
        /* [in] */ Int32 count);

    /**
     * Gets the class name of the source.
     *
     * @return The class name.
     */
    CARAPI GetClassName(
        /* [out] */ ICharSequence** name);

    /**
     * Sets the class name of the source.
     *
     * @param className The lass name.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetClassName(
        /* [in] */ ICharSequence* name);

    /**
     * Gets the text of the event. The index in the list represents the priority
     * of the text. Specifically, the lower the index the higher the priority.
     *
     * @return The text.
     */
    CARAPI GetText(
        /* [out] */ IObjectContainer** container);

    /**
     * Sets the text before a change.
     *
     * @return The text before the change.
     */
    CARAPI GetBeforeText(
        /* [out] */ ICharSequence** text);

    /**
     * Sets the text before a change.
     *
     * @param beforeText The text before the change.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetBeforeText(
        /* [in] */ ICharSequence* text);

    /**
     * Gets the description of the source.
     *
     * @return The description.
     */
    CARAPI GetContentDescription(
        /* [out] */ ICharSequence** contentDescription);

    /**
     * Sets the description of the source.
     *
     * @param contentDescription The description.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetContentDescription(
        /* [in] */ ICharSequence* contentDescription);

    /**
     * Gets the {@link Parcelable} data.
     *
     * @return The parcelable data.
     */
    CARAPI GetParcelableData(
        /* [out] */ IParcelable** parcelableData);

    /**
     * Sets the {@link Parcelable} data of the event.
     *
     * @param parcelableData The parcelable data.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetParcelableData(
        /* [in] */ IParcelable* parcelableData);

    /**
     * Gets the id of the source node.
     *
     * @return The id.
     *
     * @hide
     */
    CARAPI GetSourceNodeId(
        /* [out] */ Int64* nodeId);

    /**
     * Sets the unique id of the IIAccessibilityServiceConnection over which
     * this instance can send requests to the system.
     *
     * @param connectionId The connection id.
     *
     * @hide
     */
    CARAPI SetConnectionId(
        /* [in] */ Int32 connectionId);

    /**
     * Sets if this instance is sealed.
     *
     * @param sealed Whether is sealed.
     *
     * @hide
     */
    CARAPI SetSealed(
        /* [in] */ Boolean sealed);

    /**
     * Return an instance back to be reused.
     * <p>
     * <strong>Note:</strong> You must not touch the object after calling this function.
     *
     * @throws IllegalStateException If the record is already recycled.
     */
    CARAPI Recycle();

    /**
     * Initialize an event from another one.
     *
     * @param event The event to initialize from.
     */
    CARAPI_(void) Init(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Gets the number of records contained in the event.
     *
     * @return The number of records.
     */
    CARAPI GetRecordCount(
        /* [out] */ Int32* count);

    /**
     * Appends an {@link AccessibilityRecord} to the end of event records.
     *
     * @param record The record to append.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI AppendRecord(
        /* [in] */ IAccessibilityRecord* record);

    /**
     * Gets the record at a given index.
     *
     * @param index The index.
     * @return The record at the specified index.
     */
    CARAPI GetRecord(
        /* [in] */ Int32 index,
        /* [out] */ IAccessibilityRecord** record);

    /**
     * Gets the event type.
     *
     * @return The event type.
     */
    CARAPI GetEventType(
        /* [out] */ Int32* type);

    /**
     * Sets the event type.
     *
     * @param eventType The event type.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetEventType(
        /* [in] */ Int32 eventType);

    /**
     * Gets the time in which this event was sent.
     *
     * @return The event time.
     */
    CARAPI GetEventTime(
        /* [out] */ Int64* eventTime);

    /**
     * Sets the time in which this event was sent.
     *
     * @param eventTime The event time.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetEventTime(
        /* [in] */ Int64 eventTime);

    /**
     * Gets the package name of the source.
     *
     * @return The package name.
     */
    CARAPI GetPackageName(
        /* [out] */ ICharSequence** packageName);

    /**
     * Sets the package name of the source.
     *
     * @param packageName The package name.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetPackageName(
        /* [in] */ ICharSequence* packageName);

    /**
     * Sets the movement granularity that was traversed.
     *
     * @param granularity The granularity.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetMovementGranularity(
        /* [in] */ Int32 granularity);

    /**
     * Gets the movement granularity that was traversed.
     *
     * @return The granularity.
     */
    CARAPI GetMovementGranularity(
        /* [out] */ Int32* granularity);

    /**
     * Sets the performed action that triggered this event.
     *
     * @param action The action.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetAction(
        /* [in] */ Int32 action);

    /**
     * Gets the performed action that triggered this event.
     *
     * @return The action.
     */
    CARAPI GetAction(
        /* [out] */ Int32* action);

    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated with its type property set.
     *
     * @param eventType The event type.
     * @return An instance.
     */
    static CARAPI Obtain(
        /* [in] */ Int32 eventType,
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Returns a cached instance if such is available or a new one is
     * created. The returned instance is initialized from the given
     * <code>event</code>.
     *
     * @param event The other event.
     * @return An instance.
     */
    static CARAPI Obtain(
        /* [in] */ IAccessibilityEvent* otherEvent,
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Returns a cached instance if such is available or a new one is
     * instantiated.
     *
     * @return An instance.
     */
    static CARAPI Obtain(
        /* [out] */ IAccessibilityEvent** event);

    /**
     * Creates a new instance from a {@link Parcel}.
     *
     * @param parcel A parcel containing the state of a {@link AccessibilityEvent}.
     */
    CARAPI InitFromParcel(
        /* [in] */ IParcel* parcel);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Returns the string representation of an event type. For example,
     * {@link #TYPE_VIEW_CLICKED} is represented by the string TYPE_VIEW_CLICKED.
     *
     * @param eventType The event type
     * @return The string representation.
     */
    static CARAPI_(String) EventTypeToString(
        /* [in] */ Int32 eventType);

protected:
    /**
     * Clears the state of this instance.
     *
     * @hide
     */
    // @Override
    CARAPI_(void) Clear();

private:
    /**
     * Reads an {@link AccessibilityRecord} from a parcel.
     *
     * @param record The record to initialize.
     * @param parcel The parcel to read from.
     */
    CARAPI_(void) ReadAccessibilityRecordFromParcel(
        /* [in] */ IAccessibilityRecord* record,
        /* [in] */ IParcel* parcel);

    /**
     * Writes an {@link AccessibilityRecord} to a parcel.
     *
     * @param record The record to write.
     * @param parcel The parcel to which to write.
     */
    CARAPI_(void) WriteAccessibilityRecordToParcel(
        /* [in] */ IAccessibilityRecord* record,
        /* [in] */ IParcel* parcel);

private:
    static const String TAG;
    static const Int32 MAX_POOL_SIZE;
    static AutoPtr<CAccessibilityEvent> sPool;
    static Object sPoolLock;
    static Int32 sPoolSize;
    AutoPtr<CAccessibilityEvent> mNext;
    Boolean mIsInPool;

    Int32 mEventType;
    AutoPtr<ICharSequence> mPackageName;
    Int64 mEventTime;
    Int32 mMovementGranularity;
    Int32 mAction;

    List<AutoPtr<IAccessibilityRecord> > mRecords;
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_ACCESSIBILITY_CACCESSIBILITYEVENT_H__
