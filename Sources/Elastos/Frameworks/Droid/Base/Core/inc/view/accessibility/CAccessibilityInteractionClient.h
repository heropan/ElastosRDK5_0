#ifndef __CACCESSIBILITYINTERACTIONCLIENT_H__
#define __CACCESSIBILITYINTERACTIONCLIENT_H__

#include "ext/frameworkext.h"
#include "_CAccessibilityInteractionClient.h"
#include "view/accessibility/AccessibilityNodeInfoCache.h"
#include <elastos/Core/Object.h>
#include <elastos/utility/etl/HashMap.h>
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::HashMap;
using Elastos::Utility::Etl::List;
using Elastos::Core::Object;
using Elastos::Utility::Concurrent::Atomic::IAtomicInteger32;
using Elastos::Droid::Os::IMessage;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::AccessibilityService::IIAccessibilityServiceConnection;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

CarClass(CAccessibilityInteractionClient)
{
public:
    CAccessibilityInteractionClient();

    ~CAccessibilityInteractionClient();

    /**
     * @return The client for the current thread.
     */
    static CARAPI_(AutoPtr<CAccessibilityInteractionClient>) GetInstance();

    /**
     * <strong>Note:</strong> We keep one instance per interrogating thread since
     * the instance contains state which can lead to undesired thread interleavings.
     * We do not have a thread local variable since other threads should be able to
     * look up the correct client knowing a thread id. See ViewRootImpl for details.
     *
     * @return The client for a given <code>threadId</code>.
     */
    static CARAPI_(AutoPtr<CAccessibilityInteractionClient>) GetInstanceForThread(
        /* [in] */ Int64 threadId);

    /**
     * Sets the message to be processed if the interacted view hierarchy
     * and the interacting client are running in the same thread.
     *
     * @param message The message.
     */
    CARAPI SetSameThreadMessage(
        /* [in] */ IMessage* message);

    /**
     * Gets the root {@link AccessibilityNodeInfo} in the currently active window.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @return The root {@link AccessibilityNodeInfo} if found, null otherwise.
     */
    CARAPI GetRootInActiveWindow(
        /* [in] */ Int32 connectionId,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Finds an {@link AccessibilityNodeInfo} by accessibility id.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @param accessibilityWindowId A unique window id. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ACTIVE_WINDOW_ID}
     *     to query the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id from
     *     where to start the search. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ROOT_NODE_ID}
     *     to start from the root.
     * @param prefetchFlags flags to guide prefetching.
     * @return An {@link AccessibilityNodeInfo} if found, null otherwise.
     */
    CARAPI FindAccessibilityNodeInfoByAccessibilityId(
        /* [in] */ Int32 connectionId,
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 prefetchFlags,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Finds an {@link AccessibilityNodeInfo} by View id. The search is performed in
     * the window whose id is specified and starts from the node whose accessibility
     * id is specified.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @param accessibilityWindowId A unique window id. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ACTIVE_WINDOW_ID}
     *     to query the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id from
     *     where to start the search. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ROOT_NODE_ID}
     *     to start from the root.
     * @param viewId The id of the view.
     * @return An {@link AccessibilityNodeInfo} if found, null otherwise.
     */
    CARAPI FindAccessibilityNodeInfoByViewId(
        /* [in] */ Int32 connectionId,
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 viewId,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Finds {@link AccessibilityNodeInfo}s by View text. The match is case
     * insensitive containment. The search is performed in the window whose
     * id is specified and starts from the node whose accessibility id is
     * specified.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @param accessibilityWindowId A unique window id. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ACTIVE_WINDOW_ID}
     *     to query the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id from
     *     where to start the search. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ROOT_NODE_ID}
     *     to start from the root.
     * @param text The searched text.
     * @return A list of found {@link AccessibilityNodeInfo}s.
     */
    CARAPI FindAccessibilityNodeInfosByText(
        /* [in] */ Int32 connectionId,
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ const String& text,
        /* [out] */ IObjectContainer** infos);

    /**
     * Finds the {@link android.view.accessibility.AccessibilityNodeInfo} that has the
     * specified focus type. The search is performed in the window whose id is specified
     * and starts from the node whose accessibility id is specified.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @param accessibilityWindowId A unique window id. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ACTIVE_WINDOW_ID}
     *     to query the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id from
     *     where to start the search. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ROOT_NODE_ID}
     *     to start from the root.
     * @param focusType The focus type.
     * @return The accessibility focused {@link AccessibilityNodeInfo}.
     */
    CARAPI FindFocus(
        /* [in] */ Int32 connectionId,
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 focusType,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Finds the accessibility focused {@link android.view.accessibility.AccessibilityNodeInfo}.
     * The search is performed in the window whose id is specified and starts from the
     * node whose accessibility id is specified.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @param accessibilityWindowId A unique window id. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ACTIVE_WINDOW_ID}
     *     to query the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id from
     *     where to start the search. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ROOT_NODE_ID}
     *     to start from the root.
     * @param direction The direction in which to search for focusable.
     * @return The accessibility focused {@link AccessibilityNodeInfo}.
     */
    CARAPI FocusSearch(
        /* [in] */ Int32 connectionId,
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 direction,
        /* [out] */ IAccessibilityNodeInfo** info);

    /**
     * Performs an accessibility action on an {@link AccessibilityNodeInfo}.
     *
     * @param connectionId The id of a connection for interacting with the system.
     * @param accessibilityWindowId A unique window id. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ACTIVE_WINDOW_ID}
     *     to query the currently active window.
     * @param accessibilityNodeId A unique view id or virtual descendant id from
     *     where to start the search. Use
     *     {@link android.view.accessibility.AccessibilityNodeInfo#ROOT_NODE_ID}
     *     to start from the root.
     * @param action The action to perform.
     * @param arguments Optional action arguments.
     * @return Whether the action was performed.
     */
    CARAPI PerformAccessibilityAction(
        /* [in] */ Int32 connectionId,
        /* [in] */ Int32 accessibilityWindowId,
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [out] */ Boolean* result);

    CARAPI ClearCache();

    CARAPI OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    CARAPI SetFindAccessibilityNodeInfoResult(
        /* [in] */ IAccessibilityNodeInfo* info,
        /* [in] */ Int32 interactionId);

    CARAPI SetFindAccessibilityNodeInfosResult(
        /* [in] */ IObjectContainer* infos,
        /* [in] */ Int32 interactionId);

    CARAPI SetPerformAccessibilityActionResult(
        /* [in] */ Boolean succeeded,
        /* [in] */ Int32 interactionId);

    /**
     * Gets a cached accessibility service connection.
     *
     * @param connectionId The connection id.
     * @return The cached connection if such.
     */
    CARAPI GetConnection(
        /* [in] */ Int32 connectionId,
        /* [out] */ IIAccessibilityServiceConnection** connection);

    /**
     * Adds a cached accessibility service connection.
     *
     * @param connectionId The connection id.
     * @param connection The connection.
     */
    CARAPI AddConnection(
        /* [in] */ Int32 connectionId,
        /* [in] */ IIAccessibilityServiceConnection* connection);

    /**
     * Removes a cached accessibility service connection.
     *
     * @param connectionId The connection id.
     */
    CARAPI RemoveConnection(
        /* [in] */ Int32 connectionId);

private:
    /**
     * Gets the the result of an async request that returns an {@link AccessibilityNodeInfo}.
     *
     * @param interactionId The interaction id to match the result with the request.
     * @return The result {@link AccessibilityNodeInfo}.
     */
    CARAPI_(AutoPtr<IAccessibilityNodeInfo>) GetFindAccessibilityNodeInfoResultAndClear(
        /* [in] */ Int32 interactionId);

    /**
     * Gets the the result of an async request that returns {@link AccessibilityNodeInfo}s.
     *
     * @param interactionId The interaction id to match the result with the request.
     * @return The result {@link AccessibilityNodeInfo}s.
     */
    CARAPI_(AutoPtr< List<AutoPtr<IAccessibilityNodeInfo> > >) GetFindAccessibilityNodeInfosResultAndClear(
        /* [in] */ Int32 interactionId);

    /**
     * Gets the result of a request to perform an accessibility action.
     *
     * @param interactionId The interaction id to match the result with the request.
     * @return Whether the action was performed.
     */
    CARAPI_(Boolean) GetPerformAccessibilityActionResultAndClear(
        /* [in] */ Int32 interactionId);

    /**
     * Clears the result state.
     */
    CARAPI_(void) ClearResultLocked();

    /**
     * Waits up to a given bound for a result of a request and returns it.
     *
     * @param interactionId The interaction id to match the result with the request.
     * @return Whether the result was received.
     */
    CARAPI_(Boolean) WaitForResultTimedLocked(
        /* [in] */ Int32 interactionId);

    /**
     * Applies compatibility scale to the info bounds if it is not equal to one.
     *
     * @param info The info whose bounds to scale.
     * @param scale The scale to apply.
     */
    CARAPI_(void) ApplyCompatibilityScaleIfNeeded(
        /* [in] */ IAccessibilityNodeInfo* info,
        /* [in] */ Float scale);

    /**
     * Finalize an {@link AccessibilityNodeInfo} before passing it to the client.
     *
     * @param info The info.
     * @param connectionId The id of the connection to the system.
     * @param windowScale The source window compatibility scale.
     */
    CARAPI_(void) FinalizeAndCacheAccessibilityNodeInfo(
        /* [in] */ IAccessibilityNodeInfo* info,
        /* [in] */ Int32 connectionId,
        /* [in] */ Float windowScale);

    /**
     * Finalize {@link AccessibilityNodeInfo}s before passing them to the client.
     *
     * @param infos The {@link AccessibilityNodeInfo}s.
     * @param connectionId The id of the connection to the system.
     * @param windowScale The source window compatibility scale.
     */
    CARAPI_(void) FinalizeAndCacheAccessibilityNodeInfos(
        /* [in] */ List<AutoPtr<IAccessibilityNodeInfo> >* infos,
        /* [in] */ Int32 connectionId,
        /* [in] */ Float windowScale);

    /**
     * Gets the message stored if the interacted and interacting
     * threads are the same.
     *
     * @return The message.
     */
    CARAPI_(AutoPtr<IMessage>) GetSameProcessMessageAndClear();

    /**
     * Checks whether the infos are a fully connected tree with no duplicates.
     *
     * @param infos The result list to check.
     */
    CARAPI_(void) CheckFindAccessibilityNodeInfoResultIntegrity(
        /* [in] */ List<AutoPtr<IAccessibilityNodeInfo> >* infos);

public:
    static const Int32 NO_ID;

private:
    static const String TAG;

    static const Boolean DEBUG;

    static const Boolean CHECK_INTEGRITY;

    static const Int64 TIMEOUT_INTERACTION_MILLIS;

    static Object sStaticLock;

    static HashMap<Int64, AutoPtr<CAccessibilityInteractionClient> > sClients;

    AutoPtr<IAtomicInteger32> mInteractionIdCounter;

    Object mInstanceLock;

    Int32 mInteractionId;

    AutoPtr<IAccessibilityNodeInfo> mFindAccessibilityNodeInfoResult;

    AutoPtr<List<AutoPtr<IAccessibilityNodeInfo> > > mFindAccessibilityNodeInfosResult;

    Boolean mPerformAccessibilityActionResult;

    AutoPtr<IMessage> mSameThreadMessage;

    AutoPtr<IRect> mTempBounds;

    // The connection cache is shared between all interrogating threads.
    static HashMap<Int32, AutoPtr<IIAccessibilityServiceConnection> > sConnectionCache;
    static Object sConnectionCacheLock;

    // The connection cache is shared between all interrogating threads since
    // at any given time there is only one window allowing querying.
    static const AutoPtr<AccessibilityNodeInfoCache> sAccessibilityNodeInfoCache;
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__CACCESSIBILITYINTERACTIONCLIENT_H__
