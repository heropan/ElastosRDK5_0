#ifndef __ACCESSIBILITYNODEINFOCACHE_H__
#define __ACCESSIBILITYNODEINFOCACHE_H__

#include "Elastos.Droid.Core_server.h"
#include <elastos/utility/etl/HashMap.h>

using Elastos::Utility::Etl::HashMap;
namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

class AccessibilityNodeInfoCache : public ElRefBase
{
public:
    AccessibilityNodeInfoCache();
    ~AccessibilityNodeInfoCache();

    /**
     * The cache keeps track of {@link AccessibilityEvent}s and invalidates
     * cached nodes as appropriate.
     *
     * @param event An event.
     */
    CARAPI_(void) OnAccessibilityEvent(
        /* [in] */ IAccessibilityEvent* event);

    /**
     * Gets a cached {@link AccessibilityNodeInfo} given its accessibility node id.
     *
     * @param accessibilityNodeId The info accessibility node id.
     * @return The cached {@link AccessibilityNodeInfo} or null if such not found.
     */
    CARAPI_(AutoPtr<IAccessibilityNodeInfo>) Get(
        /* [in] */ Int64 accessibilityNodeId);

    /**
     * Caches an {@link AccessibilityNodeInfo} given its accessibility node id.
     *
     * @param info The {@link AccessibilityNodeInfo} to cache.
     */
    CARAPI_(void) Add(
        /* [in] */ IAccessibilityNodeInfo* info);

    /**
     * Clears the cache.
     */
    CARAPI_(void) Clear();

private:
    /**
     * Clears a subtree rooted at the node with the given id.
     *
     * @param rootNodeId The root id.
     */
    CARAPI_(void) ClearSubTreeLocked(
        /* [in] */ Int64 rootNodeId);

    /**
     * We are enforcing the invariant for a single input focus.
     *
     * @param currentInputFocusId The current input focused node.
     */
    CARAPI_(void) ClearSubtreeWithOldInputFocusLocked(
        /* [in] */ Int64 currentInputFocusId);

    /**
     * We are enforcing the invariant for a single accessibility focus.
     *
     * @param currentAccessibilityFocusId The current input focused node.
     */
    CARAPI_(void) ClearSubtreeWithOldAccessibilityFocusLocked(
        /* [in] */ Int64 currentAccessibilityFocusId);

    /**
     * Check the integrity of the cache which is it does not have nodes
     * from more than one window, there are no duplicates, all nodes are
     * connected, there is a single input focused node, and there is a
     * single accessibility focused node.
     */
    CARAPI_(void) CheckIntegrity();

private:
    static const String TAG;

    static const Boolean ENABLED;

    static const Boolean DEBUG;

    static const Boolean CHECK_INTEGRITY;

    Mutex mLock;

    AutoPtr< HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> > > mCacheImpl;

    Int32 mWindowId;
};

} // Accessibility
} // View
} // Droid
} // Elastos

#endif //__ACCESSIBILITYNODEINFOCACHE_H__
