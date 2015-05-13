#ifndef __CACCESSIBILITYNODEINFO_H__
#define __CACCESSIBILITYNODEINFO_H__

#include "ext/frameworkdef.h"
#include "_CAccessibilityNodeInfo.h"
#include <elastos/Mutex.h>
#include <elastos/HashMap.h>

using Elastos::Utility::HashMap;
using Elastos::Core::ICharSequence;
using Elastos::Core::Threading::Mutex;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Droid::View::IView;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::Os::IBundle;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityNodeInfo)
{
public:
    CAccessibilityNodeInfo();

    /**
     * Gets the accessibility view id which identifies a View in the view three.
     *
     * @param accessibilityNodeId The id of an {@link AccessibilityNodeInfo}.
     * @return The accessibility view id part of the node id.
     *
     * @hide
     */
    static CARAPI_(Int32) GetAccessibilityViewId(
        /* [in] */ Int64 accessibilityNodeId);

    /**
     * Gets the virtual descendant id which identifies an imaginary view in a
     * containing View.
     *
     * @param accessibilityNodeId The id of an {@link AccessibilityNodeInfo}.
     * @return The virtual view id part of the node id.
     *
     * @hide
     */
    static CARAPI_(Int32) GetVirtualDescendantId(
        /* [in] */ Int64 accessibilityNodeId);

    /**
     * Makes a node id by shifting the <code>virtualDescendantId</code>
     * by {@link #VIRTUAL_DESCENDANT_ID_SHIFT} and taking
     * the bitwise or with the <code>accessibilityViewId</code>.
     *
     * @param accessibilityViewId A View accessibility id.
     * @param virtualDescendantId A virtual descendant id.
     * @return The node id.
     *
     * @hide
     */
    static CARAPI_(Int64) MakeNodeId(
        /* [in] */ Int32 accessibilityViewId,
        /* [in] */ Int32 virtualDescendantId);

    CARAPI SetSource(
        /* [in] */ IView* source);

    CARAPI SetSourceEx(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId);

    CARAPI FindFocus(
        /* [in] */ Int32 focus,
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI FocusSearch(
        /* [in] */ Int32 direction,
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI GetWindowId(
        /* [out] */ Int32* id);

    CARAPI GetChildNodeIds(
        /* [out] */ IObjectInt32Map** ids);

    CARAPI GetChildCount(
        /* [out] */ Int32* count);

    CARAPI GetChild(
        /* [in] */ Int32 index,
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI AddChild(
        /* [in] */ IView* child);

    CARAPI AddChildEx(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId);

    CARAPI GetActions(
        /* [out] */ Int32* actions);

    CARAPI AddAction(
        /* [in] */ Int32 action);

    CARAPI SetMovementGranularities(
        /* [in] */ Int32 granularities);

    CARAPI GetMovementGranularities(
        /* [out] */ Int32* granularities);

    CARAPI PerformAction(
        /* [in] */ Int32 action,
        /* [out] */ Boolean* result);

    CARAPI PerformActionEx(
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* result);

    CARAPI FindAccessibilityNodeInfosByText(
        /* [in] */ const String& text,
        /* [out] */ IObjectContainer** container);

    CARAPI GetParent(
        /* [out] */ IAccessibilityNodeInfo** parent);

    CARAPI GetParentNodeId(
        /* [out] */ Int64* id);

    CARAPI SetParent(
        /* [in] */ IView* parent);

    CARAPI SetParentEx(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId);

    CARAPI GetBoundsInParent(
        /* [in] */ IRect* outBounds);

    CARAPI SetBoundsInParent(
        /* [in] */ IRect* bounds);

    CARAPI GetBoundsInScreen(
        /* [in] */ IRect* outBounds);

    CARAPI SetBoundsInScreen(
        /* [in] */ IRect* bounds);

    CARAPI IsCheckable(
        /* [out] */ Boolean* checkable);

    CARAPI SetCheckable(
        /* [in] */ Boolean checkable);

    CARAPI IsChecked(
        /* [out] */ Boolean* checked);

    CARAPI SetChecked(
        /* [in] */ Boolean checked);

    CARAPI IsFocusable(
        /* [out] */ Boolean* focusable);

    CARAPI SetFocusable(
        /* [in] */ Boolean focusable);

    CARAPI IsFocused(
        /* [out] */ Boolean* focused);

    CARAPI SetFocused(
        /* [in] */ Boolean focused);

    CARAPI IsVisibleToUser(
        /* [out] */ Boolean* visible);

    CARAPI SetVisibleToUser(
        /* [in] */ Boolean visible);

    CARAPI IsAccessibilityFocused(
        /* [out] */ Boolean* focused);

    CARAPI SetAccessibilityFocused(
        /* [in] */ Boolean focusable);

    CARAPI IsSelected(
        /* [out] */ Boolean* selected);

    CARAPI SetSelected(
        /* [in] */ Boolean selected);

    CARAPI IsClickable(
        /* [out] */ Boolean* clickable);

    CARAPI SetClickable(
        /* [in] */ Boolean clickable);

    CARAPI IsLongClickable(
        /* [out] */ Boolean* longClickable);

    CARAPI SetLongClickable(
        /* [in] */ Boolean longClickable);

    CARAPI IsEnabled(
        /* [out] */ Boolean* result);

    CARAPI SetEnabled(
        /* [in] */ Boolean enabled);

    CARAPI IsPassword(
        /* [out] */ Boolean* isPassword);

    CARAPI SetPassword(
        /* [in] */ Boolean isPassword);

    CARAPI IsScrollable(
        /* [out] */ Boolean* scrollable);

    CARAPI SetScrollable(
        /* [in] */ Boolean scrollable);

    CARAPI GetPackageName(
        /* [out] */ ICharSequence** packageName);

    CARAPI SetPackageName(
        /* [in] */ ICharSequence* packageName);

    CARAPI GetClassName(
        /* [out] */ ICharSequence** className);

    CARAPI SetClassName(
        /* [in] */ ICharSequence* className);

    CARAPI GetText(
        /* [out] */ ICharSequence** text);

    CARAPI SetText(
        /* [in] */ ICharSequence* text);

    CARAPI GetContentDescription(
        /* [out] */ ICharSequence** description);

    CARAPI SetContentDescription(
        /* [in] */ ICharSequence* description);

    CARAPI SetLabelFor(
        /* [in] */ IView* labeled);

    CARAPI SetLabelForEx(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId);

    CARAPI GetLabelFor(
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI SetLabeledBy(
        /* [in] */ IView* label);

    CARAPI SetLabeledByEx(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId);

    CARAPI GetLabeledBy(
        /* [out] */ IAccessibilityNodeInfo** info);

    CARAPI SetConnectionId(
        /* [in] */ Int32 connectionId);

    CARAPI DescribeContents(
        /* [out] */ Int32* contents);

    CARAPI GetSourceNodeId(
        /* [out] */ Int64* id);

    CARAPI SetSealed(
        /* [in] */ Boolean sealed);

    CARAPI IsSealed(
        /* [out] */ Boolean* sealed);

    static CARAPI Obtain(
        /* [in] */ IView* source,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Returns a cached instance if such is available otherwise a new one
     * and sets the source.
     *
     * @param root The root of the virtual subtree.
     * @param virtualDescendantId The id of the virtual descendant.
     * @return An instance.
     *
     * @see #setSource(View, int)
     */
    static CARAPI ObtainEx(
        /* [in] */ IView* root,
        /* [in] */ Int32 virtualDescendantId,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Returns a cached instance if such is available otherwise a new one.
     *
     * @return An instance.
     */
    static CARAPI ObtainEx2(
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Returns a cached instance if such is available or a new one is
     * create. The returned instance is initialized from the given
     * <code>info</code>.
     *
     * @param info The other info.
     * @return An instance.
     */
    static CARAPI ObtainEx3(
        /* [in] */ IAccessibilityNodeInfo* info,
        /* [out] */ IAccessibilityNodeInfo** infoClone);

    CARAPI Recycle();

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI Equals(
        /* [in] */ IInterface* other,
        /* [out] */ Boolean* result);

    CARAPI GetHashCode(
        /* [out] */ Int32* hash);

    CARAPI ToString(
        /* [out] */ String* s);

protected:
    /**
     * Enforces that this instance is sealed.
     *
     * @throws IllegalStateException If this instance is not sealed.
     *
     * @hide
     */
    CARAPI EnforceSealed();

    /**
     * Enforces that this instance is not sealed.
     *
     * @throws IllegalStateException If this instance is sealed.
     *
     * @hide
     */
    CARAPI EnforceNotSealed();

private:
    /**
     * Gets the value of a boolean property.
     *
     * @param property The property.
     * @return The value.
     */
    CARAPI_(Boolean) GetBooleanProperty(
        /* [in] */ Int32 property);

    /**
     * Sets a boolean property.
     *
     * @param property The property.
     * @param value The value.
     *
     * @throws IllegalStateException If called from an AccessibilityService.
     */
    CARAPI SetBooleanProperty(
        /* [in] */ Int32 property,
        /* [in] */ Boolean value);

    CARAPI EnforceValidFocusDirection(
        /* [in] */ Int32 direction);

    CARAPI EnforceValidFocusType(
        /* [in] */ Int32 focusType);

    /**
     * Initializes this instance from another one.
     *
     * @param other The other instance.
     */
    // @SuppressWarnings("unchecked")
    CARAPI_(void) Init(
        /* [in] */ CAccessibilityNodeInfo* other);

    /**
     * Creates a new instance from a {@link Parcel}.
     *
     * @param parcel A parcel containing the state of a {@link AccessibilityNodeInfo}.
     */
    CARAPI_(void) InitFromParcel(
        /* [in] */ IParcel* parcel);

    /**
     * Clears the state of this instance.
     */
    CARAPI_(void) Clear();

    /**
     * Gets the human readable action symbolic name.
     *
     * @param action The action.
     * @return The symbolic name.
     */
    static CARAPI_(String) GetActionSymbolicName(
        /* [in] */ Int32 action);

    /**
     * Gets the human readable movement granularity symbolic name.
     *
     * @param granularity The granularity.
     * @return The symbolic name.
     */
    static CARAPI_(String) GetMovementGranularitySymbolicName(
        /* [in] */ Int32 granularity);

    CARAPI_(Boolean) CanPerformRequestOverConnection(
        /* [in] */ Int64 accessibilityNodeId);

private:
    static const String TAG;
    static const Boolean DEBUG;

    // Boolean attributes.

    static const Int32 PROPERTY_CHECKABLE;

    static const Int32 PROPERTY_CHECKED;

    static const Int32 PROPERTY_FOCUSABLE;

    static const Int32 PROPERTY_FOCUSED;

    static const Int32 PROPERTY_SELECTED;

    static const Int32 PROPERTY_CLICKABLE;

    static const Int32 PROPERTY_LONG_CLICKABLE;

    static const Int32 PROPERTY_ENABLED;

    static const Int32 PROPERTY_PASSWORD;

    static const Int32 PROPERTY_SCROLLABLE;

    static const Int32 PROPERTY_ACCESSIBILITY_FOCUSED;

    static const Int32 PROPERTY_VISIBLE_TO_USER;

    /**
     * Bits that provide the id of a virtual descendant of a view.
     */
    static const Int64 VIRTUAL_DESCENDANT_ID_MASK;

    /**
     * Bit shift of {@link #VIRTUAL_DESCENDANT_ID_MASK} to get to the id for a
     * virtual descendant of a view. Such a descendant does not exist in the view
     * hierarchy and is only reported via the accessibility APIs.
     */
    static const Int32 VIRTUAL_DESCENDANT_ID_SHIFT;

    // Housekeeping.
    static const Int32 MAX_POOL_SIZE;
    static AutoPtr<CAccessibilityNodeInfo> sPool;
    static Mutex sPoolLock;
    static Int32 sPoolSize;
    AutoPtr<CAccessibilityNodeInfo> mNext;
    Boolean mIsInPool;
    Boolean mSealed;

    // Data.
    Int32 mWindowId;
    Int64 mSourceNodeId;
    Int64 mParentNodeId;
    Int64 mLabelForId;
    Int64 mLabeledById;

    Int32 mBooleanProperties;
    AutoPtr<IRect> mBoundsInParent;
    AutoPtr<IRect> mBoundsInScreen;

    AutoPtr<ICharSequence> mPackageName;
    AutoPtr<ICharSequence> mClassName;
    AutoPtr<ICharSequence> mText;
    AutoPtr<ICharSequence> mContentDescription;

    HashMap<Int32, Int64> mChildNodeIds;
    Int32 mActions;

    Int32 mMovementGranularities;

    Int32 mConnectionId;
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__CACCESSIBILITYNODEINFO_H__
