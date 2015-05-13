
#include "ext/frameworkext.h"
#include "view/accessibility/AccessibilityNodeInfoCache.h"
#include "view/accessibility/CAccessibilityNodeInfo.h"
#include <elastos/Slogger.h>
#include <elastos/HashSet.h>
#include <elastos/List.h>

using Elastos::Core::IInteger64;
using Elastos::Utility::HashSet;
using Elastos::Utility::List;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Utility::Logging::Slogger;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;


#ifndef HASH_FUNC_FOR_AUTOPTR_IACCESSIBILITYNODEINFO
#define HASH_FUNC_FOR_AUTOPTR_IACCESSIBILITYNODEINFO
DEFINE_HASH_FUNC_FOR_AUTOPTR_USING_ADDR(IAccessibilityNodeInfo)
#endif

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const String AccessibilityNodeInfoCache::TAG("AccessibilityNodeInfoCache");//AccessibilityNodeInfoCache.class.getSimpleName();
const Boolean AccessibilityNodeInfoCache::ENABLED = TRUE;
const Boolean AccessibilityNodeInfoCache::DEBUG = FALSE;
const Boolean AccessibilityNodeInfoCache::CHECK_INTEGRITY = TRUE;

AccessibilityNodeInfoCache::AccessibilityNodeInfoCache()
    : mWindowId(0)
{
    if (ENABLED) {
        mCacheImpl = new HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >();
    }
    else {
        mCacheImpl = NULL;
    }
}

AccessibilityNodeInfoCache::~AccessibilityNodeInfoCache()
{
    mCacheImpl = NULL;
}

void AccessibilityNodeInfoCache::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    if (ENABLED) {
        Int32 eventType;
        event->GetEventType(&eventType);
        switch (eventType) {
            case IAccessibilityEvent::TYPE_WINDOW_STATE_CHANGED: {
                // New window so we clear the cache.
                event->GetWindowId(&mWindowId);
                Clear();
            } break;
            case IAccessibilityEvent::TYPE_VIEW_HOVER_ENTER:
            case IAccessibilityEvent::TYPE_VIEW_HOVER_EXIT: {
                Int32 windowId;
                event->GetWindowId(&windowId);
                if (mWindowId != windowId) {
                    // New window so we clear the cache.
                    mWindowId = windowId;
                    Clear();
                }
            } break;
            case IAccessibilityEvent::TYPE_VIEW_FOCUSED:
            case IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED:
            case IAccessibilityEvent::TYPE_VIEW_SELECTED:
            case IAccessibilityEvent::TYPE_VIEW_TEXT_CHANGED:
            case IAccessibilityEvent::TYPE_VIEW_TEXT_SELECTION_CHANGED: {
                // Since we prefetch the descendants of a node we
                // just remove the entire subtree since when the node
                // is fetched we will gets its descendant anyway.
                Mutex::Autolock lock(mLock);
                Int64 sourceId;
                event->GetSourceNodeId(&sourceId);
                ClearSubTreeLocked(sourceId);
                if (eventType == IAccessibilityEvent::TYPE_VIEW_FOCUSED) {
                    ClearSubtreeWithOldInputFocusLocked(sourceId);
                }
                if (eventType == IAccessibilityEvent::TYPE_VIEW_ACCESSIBILITY_FOCUSED) {
                    ClearSubtreeWithOldAccessibilityFocusLocked(sourceId);
                }
            } break;
            case IAccessibilityEvent::TYPE_WINDOW_CONTENT_CHANGED:
            case IAccessibilityEvent::TYPE_VIEW_SCROLLED: {
                Mutex::Autolock lock(mLock);
                Int64 accessibilityNodeId;
                event->GetSourceNodeId(&accessibilityNodeId);
                ClearSubTreeLocked(accessibilityNodeId);
            } break;
        }
        // if (Build.IS_DEBUGGABLE && CHECK_INTEGRITY) {
        //     checkIntegrity();
        // }
    }
}

AutoPtr<IAccessibilityNodeInfo> AccessibilityNodeInfoCache::Get(
    /* [in] */ Int64 accessibilityNodeId)
{
    if (ENABLED) {
        Mutex::Autolock lock(mLock);
        HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it
                = mCacheImpl->Find(accessibilityNodeId);
        AutoPtr<IAccessibilityNodeInfo> info;
        if (it != mCacheImpl->End()) {
            info = it->mSecond;
        }
        if (info != NULL) {
            // Return a copy since the client calls to AccessibilityNodeInfo#recycle()
            // will wipe the data of the cached info.
            CAccessibilityNodeInfo::ObtainEx3(info, (IAccessibilityNodeInfo**)&info);
        }
        // if (DEBUG) {
        //     Log.i(TAG, "get(" + accessibilityNodeId + ") = " + info);
        // }
        return info;
    }
    else {
        return NULL;
    }
}

void AccessibilityNodeInfoCache::Add(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    if (ENABLED) {
        Mutex::Autolock lock(mLock);
        // if (DEBUG) {
        //     Log.i(TAG, "add(" + info + ")");
        // }

        Int64 sourceId;
        info->GetSourceNodeId(&sourceId);
        HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it
                = mCacheImpl->Find(sourceId);
        AutoPtr<IAccessibilityNodeInfo> oldInfo;
        if (it != mCacheImpl->End()) {
            oldInfo = it->mSecond;
        }
        if (oldInfo != NULL) {
            // If the added node is in the cache we have to be careful if
            // the new one represents a source state where some of the
            // children have been removed to avoid having disconnected
            // subtrees in the cache.
            AutoPtr<IObjectInt32Map> oldChildrenIds;
            oldInfo->GetChildNodeIds((IObjectInt32Map**)&oldChildrenIds);
            AutoPtr<IObjectInt32Map> newChildrenIds;
            info->GetChildNodeIds((IObjectInt32Map**)&newChildrenIds);
            Int32 oldChildCount;
            oldChildrenIds->GetSize(&oldChildCount);
            for (Int32 i = 0; i < oldChildCount; i++) {
                AutoPtr<IInteger64> value;
                oldChildrenIds->Get(i, (IInterface**)&value);
                Int64 oldChildId;
                value->GetValue(&oldChildId);
                assert(0);
                // if (newChildrenIds.indexOfValue(oldChildId) < 0) {
                //     clearSubTreeLocked(oldChildId);
                // }
            }

            // Also be careful if the parent has changed since the new
            // parent may be a predecessor of the old parent which will
            // make the cached tree cyclic.
            Int64 oldParentId;
            oldInfo->GetParentNodeId(&oldParentId);
            Int64 newParentId;
            info->GetParentNodeId(&newParentId);
            if (newParentId != oldParentId) {
                ClearSubTreeLocked(oldParentId);
            }
        }

        // Cache a copy since the client calls to AccessibilityNodeInfo#recycle()
        // will wipe the data of the cached info.
        AutoPtr<IAccessibilityNodeInfo> clone;
        CAccessibilityNodeInfo::ObtainEx3(info, (IAccessibilityNodeInfo**)&clone);
        (*mCacheImpl)[sourceId] = clone;
    }
}

void AccessibilityNodeInfoCache::Clear()
{
    if (ENABLED) {
        Mutex::Autolock lock(mLock);
        // if (DEBUG) {
        //     Log.i(TAG, "clear()");
        // }
        // Recycle the nodes before clearing the cache.
        HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it = mCacheImpl->Begin();
        for (; it != mCacheImpl->End(); ++it) {
            AutoPtr<IAccessibilityNodeInfo> info = it->mSecond;
            info->Recycle();
        }
        mCacheImpl->Clear();
    }
}

void AccessibilityNodeInfoCache::ClearSubTreeLocked(
    /* [in] */ Int64 rootNodeId)
{
    HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it
            = mCacheImpl->Find(rootNodeId);
    AutoPtr<IAccessibilityNodeInfo> current;
    if (it != mCacheImpl->End()) {
        current = it->mSecond;
    }
    if (current == NULL) {
        return;
    }
    mCacheImpl->Erase(it);
    AutoPtr<IObjectInt32Map> childNodeIds;
    current->GetChildNodeIds((IObjectInt32Map**)&childNodeIds);
    Int32 childCount;
    childNodeIds->GetSize(&childCount);
    for (Int32 i = 0; i < childCount; i++) {
        AutoPtr<IInteger64> value;
        childNodeIds->Get(i, (IInterface**)&value);
        Int64 childNodeId;
        value->GetValue(&childNodeId);
        ClearSubTreeLocked(childNodeId);
    }
}

void AccessibilityNodeInfoCache::ClearSubtreeWithOldInputFocusLocked(
    /* [in] */ Int64 currentInputFocusId)
{
    HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it = mCacheImpl->Begin();
    for (; it != mCacheImpl->End(); ++it) {
        AutoPtr<IAccessibilityNodeInfo> info = it->mSecond;
        Int64 infoSourceId;
        info->GetSourceNodeId(&infoSourceId);
        Boolean focused;
        if (infoSourceId != currentInputFocusId && (info->IsFocused(&focused), focused)) {
            ClearSubTreeLocked(infoSourceId);
            return;
        }
    }
}

void AccessibilityNodeInfoCache::ClearSubtreeWithOldAccessibilityFocusLocked(
    /* [in] */ Int64 currentAccessibilityFocusId)
{
    HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it = mCacheImpl->Begin();
    for (; it != mCacheImpl->End(); ++it) {
        AutoPtr<IAccessibilityNodeInfo> info = it->mSecond;
        Int64 infoSourceId;
        info->GetSourceNodeId(&infoSourceId);
        Boolean focused;
        if (infoSourceId != currentAccessibilityFocusId
                && (info->IsAccessibilityFocused(&focused), focused)) {
            ClearSubTreeLocked(infoSourceId);
            return;
        }
    }
}

void AccessibilityNodeInfoCache::CheckIntegrity()
{
    Mutex::Autolock lock(mLock);
    // Get the root.
    if (mCacheImpl->Begin() == mCacheImpl->End()) {
        return;
    }

    // If the cache is a tree it does not matter from
    // which node we start to search for the root.
    AutoPtr<IAccessibilityNodeInfo> root = mCacheImpl->Begin()->mSecond;
    AutoPtr<IAccessibilityNodeInfo> parent = root;
    while (parent != NULL) {
        root = parent;
        Int64 parentId;
        parent->GetParentNodeId(&parentId);
        HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it
                = mCacheImpl->Find(parentId);
        if (it != mCacheImpl->End()) {
            parent = it->mSecond;
        }
        else {
            parent = NULL;
        }
    }

    // Traverse the tree and do some checks.
    Int32 windowId;
    root->GetWindowId(&windowId);
    AutoPtr<IAccessibilityNodeInfo> accessFocus;
    AutoPtr<IAccessibilityNodeInfo> inputFocus;
    HashSet<AutoPtr<IAccessibilityNodeInfo> > seen;
    List<AutoPtr<IAccessibilityNodeInfo> > fringe;;
    fringe.PushBack(root);

    while (fringe.Begin() != fringe.End()) {
        AutoPtr<IAccessibilityNodeInfo> current = *fringe.Begin();
        fringe.PopFront();
        // Check for duplicates
        seen.Insert(current);
        // if (!seen.add(current)) {
        //     Log.e(TAG, "Duplicate node: " + current);
        //     return;
        // }

        // Check for one accessibility focus.
        Boolean focused;
        current->IsAccessibilityFocused(&focused);
        if (focused) {
            if (accessFocus != NULL) {
                Slogger::E(TAG, "Duplicate accessibility focus:%p", current.Get());
            }
            else {
                accessFocus = current;
            }
        }

        // Check for one input focus.
        Boolean isFocused;
        if (current->IsFocused(&isFocused), isFocused) {
            if (inputFocus != NULL) {
                Slogger::E(TAG, "Duplicate input focus: %p", current.Get());
            }
            else {
                inputFocus = current;
            }
        }

        AutoPtr<IObjectInt32Map> childIds;
        current->GetChildNodeIds((IObjectInt32Map**)&childIds);
        Int32 childCount;
        childIds->GetSize(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IInteger64> value;
            childIds->Get(i, (IInterface**)&value);
            Int64 childId;
            value->GetValue(&childId);
            AutoPtr<IAccessibilityNodeInfo> child;
            HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it
                    = mCacheImpl->Find(childId);
            if (it != mCacheImpl->End()) {
                child = it->mSecond;
            }
            if (child != NULL) {
                fringe.PushBack(child);
            }
        }
    }

    // Check for disconnected nodes or ones from another window.
    HashMap<Int64, AutoPtr<IAccessibilityNodeInfo> >::Iterator it = mCacheImpl->Begin();
    for (; it != mCacheImpl->End(); ++it) {
        AutoPtr<IAccessibilityNodeInfo> info = it->mSecond;
        if (seen.Find(info) == seen.End()) {
            Int32 infoWinId;
            info->GetWindowId(&infoWinId);
            if (infoWinId == windowId) {
                Slogger::E(TAG, "Disconneced node: ");
            }
            else {
                Slogger::E(TAG, "Node from: %d not from:%d %p", infoWinId
                        , windowId, info.Get());
            }
        }
    }
}

} // Accessibility
} // View
} // Droid
} // Elastos
