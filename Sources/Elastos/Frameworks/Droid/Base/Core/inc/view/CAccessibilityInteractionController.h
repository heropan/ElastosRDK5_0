#ifndef __ELASTOS_DROID_VIEW_CACCESSIBILITYINTERACTIONCONTROLLER_H__
#define __ELASTOS_DROID_VIEW_CACCESSIBILITYINTERACTIONCONTROLLER_H__

#include "_Elastos_Droid_View_CAccessibilityInteractionController.h"
#include "ext/frameworkext.h"
#include "os/HandlerBase.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Droid::Os::IBundle;
using Elastos::Droid::Os::HandlerBase;
using Elastos::Droid::Graphics::IRect;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeProvider;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionConnectionCallback;

namespace Elastos {
namespace Droid {
namespace View {

class ViewRootImpl;

/**
 * This class is an interface this ViewAncestor provides to the
 * AccessibilityManagerService to the latter can interact with
 * the view hierarchy in this ViewAncestor.
 */
CarClass(CAccessibilityInteractionController)
{
private:
    /**
     * This class encapsulates a prefetching strategy for the accessibility APIs for
     * querying window content. It is responsible to prefetch a batch of
     * AccessibilityNodeInfos in addition to the one for a requested node.
     */
    class AccessibilityNodePrefetcher : public ElRefBase {
    public:
        AccessibilityNodePrefetcher(
            /* [in] */ CAccessibilityInteractionController* host)
            : mHost(host)
        {}

        CARAPI_(void) PrefetchAccessibilityNodeInfos(
            /* [in] */ IView* view,
            /* [in] */ Int32 virtualViewId,
            /* [in] */ Int32 prefetchFlags,
            /* [in] */ IObjectContainer* outInfos);

    private:
        CARAPI_(void) PrefetchPredecessorsOfRealNode(
            /* [in] */ IView* view,
            /* [in] */ IObjectContainer* outInfos);

        CARAPI_(void) PrefetchSiblingsOfRealNode(
            /* [in] */ IView* current,
            /* [in] */ IObjectContainer* outInfos);

        CARAPI_(void) PrefetchDescendantsOfRealNode(
            /* [in] */ IView* root,
            /* [in] */ IObjectContainer* outInfos);

        CARAPI_(void) PrefetchPredecessorsOfVirtualNode(
            /* [in] */ IAccessibilityNodeInfo* root,
            /* [in] */ IView* providerHost,
            /* [in] */ IAccessibilityNodeProvider* provider,
            /* [in] */ IObjectContainer* outInfos);

        CARAPI_(void) PrefetchSiblingsOfVirtualNode(
            /* [in] */ IAccessibilityNodeInfo* current,
            /* [in] */ IView* providerHost,
            /* [in] */ IAccessibilityNodeProvider* provider,
            /* [in] */ IObjectContainer* outInfos);

        CARAPI_(void) PrefetchDescendantsOfVirtualNode(
            /* [in] */ IAccessibilityNodeInfo* root,
            /* [in] */ IAccessibilityNodeProvider* provider,
            /* [in] */ IObjectContainer* outInfos);

    private:
        static const Int32 MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE;
        // List<AutoPtr<IView> > mTempViewList;
        CAccessibilityInteractionController* mHost;
    };

    class PrivateHandler : public HandlerBase {
    public:
        static const Int32 MSG_PERFORM_ACCESSIBILITY_ACTION;// = 1;
        static const Int32 MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_ACCESSIBILITY_ID;// = 2;
        static const Int32 MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_VIEW_ID;// = 3;
        static const Int32 MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_TEXT;// = 4;
        static const Int32 MSG_FIND_FOCUS;// = 5;
        static const Int32 MSG_FOCUS_SEARCH;// = 6;

        PrivateHandler(
            /* [in] */ ILooper* looper,
            /* [in] */ CAccessibilityInteractionController* host);

        String GetMessageNameImpl(
            /* [in] */ IMessage* message);

        CARAPI HandleMessage(
            /* [in] */ IMessage* message);

    private:
        CAccessibilityInteractionController* mHost;
    };

public:
    CAccessibilityInteractionController();

    CARAPI constructor(
        /* [in] */ Handle32 viewRootImpl);

    CARAPI FindAccessibilityNodeInfoByAccessibilityIdClientThread(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 interactionId,
        /* [in] */ IAccessibilityInteractionConnectionCallback* cb,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 interrogatingPid,
        /* [in] */ Int64 interrogatingTid);

    CARAPI FindAccessibilityNodeInfoByViewIdClientThread(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 viewId,
        /* [in] */ Int32 interactionId,
        /* [in] */ IAccessibilityInteractionConnectionCallback* cb,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 interrogatingPid,
        /* [in] */ Int64 interrogatingTid);

    CARAPI FindAccessibilityNodeInfosByTextClientThread(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ const String& text,
        /* [in] */ Int32 interactionId,
        /* [in] */ IAccessibilityInteractionConnectionCallback* cb,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 interrogatingPid,
        /* [in] */ Int64 interrogatingTid);

    CARAPI FindFocusClientThread(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 focusType,
        /* [in] */ Int32 interactionId,
        /* [in] */ IAccessibilityInteractionConnectionCallback* cb,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 interogatingPid,
        /* [in] */ Int64 interrogatingTid);

    CARAPI FocusSearchClientThread(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 direction,
        /* [in] */ Int32 interactionId,
        /* [in] */ IAccessibilityInteractionConnectionCallback* cb,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 interogatingPid,
        /* [in] */ Int64 interrogatingTid);

    CARAPI PerformAccessibilityActionClientThread(
        /* [in] */ Int64 accessibilityNodeId,
        /* [in] */ Int32 action,
        /* [in] */ IBundle* arguments,
        /* [in] */ Int32 interactionId,
        /* [in] */ IAccessibilityInteractionConnectionCallback* cb,
        /* [in] */ Int32 flags,
        /* [in] */ Int32 interogatingPid,
        /* [in] */ Int64 interrogatingTid);

private:
    CARAPI_(Boolean) IsShown(
        /* [in] */ IView* view);

    CARAPI_(void) FindAccessibilityNodeInfoByAccessibilityIdUiThread(
        /* [in] */ IMessage* message);

    CARAPI_(void) FindAccessibilityNodeInfoByViewIdUiThread(
        /* [in] */ IMessage* message);

    CARAPI_(void) FindAccessibilityNodeInfosByTextUiThread(
        /* [in] */ IMessage* message);

    CARAPI_(void) FindFocusUiThread(
        /* [in] */ IMessage* message);

    CARAPI_(void) FocusSearchUiThread(
        /* [in] */ IMessage* message);

    CARAPI_(void) PerfromAccessibilityActionUiThread(
        /* [in] */ IMessage* message);

    AutoPtr<IView> FindViewByAccessibilityId(
        /* [in] */ Int32 accessibilityId);

    CARAPI_(void) ApplyApplicationScaleIfNeeded(
        /* [in] */ IObjectContainer* infos);

    CARAPI_(void) ApplyApplicationScaleIfNeeded(
        /* [in] */ IAccessibilityNodeInfo* info);

private:
    ViewRootImpl* mViewRootImpl;

    AutoPtr<IObjectContainer> mTempAccessibilityNodeInfoList;
    AutoPtr<IHandler> mHandler;
    AutoPtr<AccessibilityNodePrefetcher> mPrefetcher;
    Int64 mMyLooperThreadId;
    Int32 mMyProcessId;

    List<AutoPtr<IView> > mTempArrayList;
    AutoPtr<IRect> mTempRect;
};

} // View
} // Droid
} // Elastos

#endif //__ELASTOS_DROID_VIEW_CACCESSIBILITYINTERACTIONCONTROLLER_H__
