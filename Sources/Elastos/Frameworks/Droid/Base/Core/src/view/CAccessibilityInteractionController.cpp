#include "view/CAccessibilityInteractionController.h"
#include "view/ViewRootImpl.h"
#include "view/ViewGroup.h"
#include "view/accessibility/CAccessibilityNodeInfoHelper.h"
#include "view/accessibility/CAccessibilityInteractionClientHelper.h"
#include "os/Process.h"
#include "os/SomeArgs.h"
#include <elastos/utility/logging/Slogger.h>

using Elastos::Utility::Logging::Slogger;
using Elastos::Core::IInteger64;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CString;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::SomeArgs;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClient;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfoHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityNodeInfoHelper;
using Elastos::Droid::View::Accessibility::IAccessibilityInteractionClientHelper;
using Elastos::Droid::View::Accessibility::CAccessibilityInteractionClientHelper;

namespace Elastos {
namespace Droid {
namespace View {

const Int32 CAccessibilityInteractionController::PrivateHandler::MSG_PERFORM_ACCESSIBILITY_ACTION = 1;
const Int32 CAccessibilityInteractionController::PrivateHandler::MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_ACCESSIBILITY_ID = 2;
const Int32 CAccessibilityInteractionController::PrivateHandler::MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_VIEW_ID = 3;
const Int32 CAccessibilityInteractionController::PrivateHandler::MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_TEXT = 4;
const Int32 CAccessibilityInteractionController::PrivateHandler::MSG_FIND_FOCUS = 5;
const Int32 CAccessibilityInteractionController::PrivateHandler::MSG_FOCUS_SEARCH = 6;

const Int32 CAccessibilityInteractionController::AccessibilityNodePrefetcher::MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE = 50;

//========================================================================================
// CAccessibilityInteractionController::AccessibilityNodePrefetcher
//========================================================================================

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchAccessibilityNodeInfos(
    /* [in] */ IView* view,
    /* [in] */ Int32 virtualViewId,
    /* [in] */ Int32 prefetchFlags,
    /* [in] */ IObjectContainer* outInfos)
{
    AutoPtr<IAccessibilityNodeProvider> provider;
    view->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
    if (provider == NULL) {
        AutoPtr<IAccessibilityNodeInfo> root;
        view->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&root);
        if (root != NULL) {
            outInfos->Add(root);
            if ((prefetchFlags & IAccessibilityNodeInfo::FLAG_PREFETCH_PREDECESSORS) != 0) {
                PrefetchPredecessorsOfRealNode(view, outInfos);
            }
            if ((prefetchFlags & IAccessibilityNodeInfo::FLAG_PREFETCH_SIBLINGS) != 0) {
                PrefetchSiblingsOfRealNode(view, outInfos);
            }
            if ((prefetchFlags & IAccessibilityNodeInfo::FLAG_PREFETCH_DESCENDANTS) != 0) {
                PrefetchDescendantsOfRealNode(view, outInfos);
            }
        }
    }
    else {
        AutoPtr<IAccessibilityNodeInfo> root;
        provider->CreateAccessibilityNodeInfo(virtualViewId, (IAccessibilityNodeInfo**)&root);
        if (root != NULL) {
            outInfos->Add(root);
            if ((prefetchFlags & IAccessibilityNodeInfo::FLAG_PREFETCH_PREDECESSORS) != 0) {
                PrefetchPredecessorsOfVirtualNode(root, view, provider, outInfos);
            }
            if ((prefetchFlags & IAccessibilityNodeInfo::FLAG_PREFETCH_SIBLINGS) != 0) {
                PrefetchSiblingsOfVirtualNode(root, view, provider, outInfos);
            }
            if ((prefetchFlags & IAccessibilityNodeInfo::FLAG_PREFETCH_DESCENDANTS) != 0) {
                PrefetchDescendantsOfVirtualNode(root, provider, outInfos);
            }
        }
    }
}

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchPredecessorsOfRealNode(
    /* [in] */ IView* view,
    /* [in] */ IObjectContainer* outInfos)
{
    AutoPtr<IViewParent> parent;
    view->GetParentForAccessibility((IViewParent**)&parent);
    Int32 count;
    while (IView::Probe(parent) != NULL
        && (outInfos->GetObjectCount(&count), count < MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE)) {
        AutoPtr<IView> parentView = IView::Probe(parent);
        AutoPtr<IAccessibilityNodeInfo> info;
        parentView->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&info);
        if (info != NULL) {
            outInfos->Add(info);
        }
        AutoPtr<IViewParent> temp;
        parentView->GetParentForAccessibility((IViewParent**)&temp);
        parent = temp;
    }
}

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchSiblingsOfRealNode(
    /* [in] */ IView* current,
    /* [in] */ IObjectContainer* outInfos)
{
    AutoPtr<IViewParent> parent;
    current->GetParentForAccessibility((IViewParent**)&parent);
    IViewGroup* group = IViewGroup::Probe(parent);
    if (group != NULL) {
        ViewGroup* parentGroup = reinterpret_cast<ViewGroup*>(group->Probe(EIID_ViewGroup));
        // List<AutoPtr<IView> >& children = mTempViewList;
        // children.Clear();

        AutoPtr<IObjectContainer> bc;
        CObjectContainer::New((IObjectContainer**)&bc);
        parentGroup->AddChildrenForAccessibility(bc);

        Int32 childViewId, currentViewId;
        current->GetAccessibilityViewId(&currentViewId);

        Int32 count;
        AutoPtr<IObjectEnumerator> emu;
        bc->GetObjectEnumerator((IObjectEnumerator**)&emu);
        Boolean hasNext;
        while (emu->MoveNext(&hasNext), hasNext) {
            if (outInfos->GetObjectCount(&count), count >= MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
                return;
            }

            AutoPtr<IInterface> obj;
            emu->Current((IInterface**)&obj);
            IView* child = IView::Probe(obj);
            child->GetAccessibilityViewId(&childViewId);

            if (childViewId != currentViewId && mHost->IsShown(child)) {
                AutoPtr<IAccessibilityNodeInfo> info;
                AutoPtr<IAccessibilityNodeProvider> provider;
                child->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
                if (provider == NULL) {
                    child->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&info);
                }
                else {
                    provider->CreateAccessibilityNodeInfo(
                        IAccessibilityNodeInfo::UNDEFINED, (IAccessibilityNodeInfo**)&info);
                }

                if (info != NULL) {
                    outInfos->Add(info);
                }
            }
        }

        // children.Clear();
    }
}

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchDescendantsOfRealNode(
    /* [in] */ IView* root,
    /* [in] */ IObjectContainer* outInfos)
{
    if (IViewGroup::Probe(root) == NULL) {
        return;
    }

    HashMap<AutoPtr<IView>, AutoPtr<IAccessibilityNodeInfo> > addedChildren;
    // ArrayList<View> children = mTempViewList;
    // children.clear();

    AutoPtr<IObjectContainer> bc;
    CObjectContainer::New((IObjectContainer**)&bc);
    root->AddChildrenForAccessibility(bc);

    AutoPtr<IObjectEnumerator> emu;
    bc->GetObjectEnumerator((IObjectEnumerator**)&emu);
    Int32 count;
    Boolean hasNext;
    while (emu->MoveNext(&hasNext), hasNext) {
        if (outInfos->GetObjectCount(&count), count >= MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
            return;
        }

        AutoPtr<IInterface> obj;
        emu->Current((IInterface**)&obj);
        IView* child = IView::Probe(obj);

        if (mHost->IsShown(child)) {
            AutoPtr<IAccessibilityNodeProvider> provider;
            child->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
            if (provider == NULL) {
                AutoPtr<IAccessibilityNodeInfo> info;
                child->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&info);
                if (info != NULL) {
                    outInfos->Add(info);
                    addedChildren[child] = NULL;
                }
            }
            else {
                AutoPtr<IAccessibilityNodeInfo> info;
                provider->CreateAccessibilityNodeInfo(
                    IAccessibilityNodeInfo::UNDEFINED, (IAccessibilityNodeInfo**)&info);
                if (info != NULL) {
                    outInfos->Add(info);
                    addedChildren[child] = info;
                }
            }
        }
    }

    // children.clear();

    if (outInfos->GetObjectCount(&count), count < MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
        HashMap<AutoPtr<IView>, AutoPtr<IAccessibilityNodeInfo> >::Iterator it;
        for (it = addedChildren.Begin(); it != addedChildren.End(); ++it) {
            AutoPtr<IView> addedChild = it->mFirst;
            AutoPtr<IAccessibilityNodeInfo> virtualRoot = it->mSecond;
            if (virtualRoot == NULL) {
                PrefetchDescendantsOfRealNode(addedChild, outInfos);
            }
            else {
                AutoPtr<IAccessibilityNodeProvider> provider;
                addedChild->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
                PrefetchDescendantsOfVirtualNode(virtualRoot, provider, outInfos);
            }
        }
    }
}

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchPredecessorsOfVirtualNode(
    /* [in] */ IAccessibilityNodeInfo* root,
    /* [in] */ IView* providerHost,
    /* [in] */ IAccessibilityNodeProvider* provider,
    /* [in] */ IObjectContainer* outInfos)
{
    Int64 parentNodeId;
    root->GetParentNodeId(&parentNodeId);
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
    Int32 accessibilityViewId, virtualDescendantId, hostAccessViewId;
    helper->GetAccessibilityViewId(parentNodeId, &accessibilityViewId);
    providerHost->GetAccessibilityViewId(&hostAccessViewId);
    Int32 count;

    while (accessibilityViewId != IAccessibilityNodeInfo::UNDEFINED) {
        if (outInfos->GetObjectCount(&count), count >= MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
            return;
        }

        helper->GetVirtualDescendantId(parentNodeId, &virtualDescendantId);
        if (virtualDescendantId != IAccessibilityNodeInfo::UNDEFINED
            || accessibilityViewId == hostAccessViewId) {
            AutoPtr<IAccessibilityNodeInfo> parent;
            provider->CreateAccessibilityNodeInfo(
                virtualDescendantId, (IAccessibilityNodeInfo**)&parent);
            if (parent != NULL) {
                outInfos->Add(parent);
            }
            parent->GetParentNodeId(&parentNodeId);
            helper->GetAccessibilityViewId(parentNodeId, &accessibilityViewId);
        }
        else {
            PrefetchPredecessorsOfRealNode(providerHost, outInfos);
            return;
        }
    }
}

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchSiblingsOfVirtualNode(
    /* [in] */ IAccessibilityNodeInfo* current,
    /* [in] */ IView* providerHost,
    /* [in] */ IAccessibilityNodeProvider* provider,
    /* [in] */ IObjectContainer* outInfos)
{
    Int64 parentNodeId;
    current->GetParentNodeId(&parentNodeId);
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);

    Int32 parentAccessibilityViewId, parentVirtualDescendantId, hostAccessViewId;
    helper->GetAccessibilityViewId(parentNodeId, &parentAccessibilityViewId);
    helper->GetVirtualDescendantId(parentNodeId, &parentVirtualDescendantId);
    providerHost->GetAccessibilityViewId(&hostAccessViewId);

    if (parentVirtualDescendantId != IAccessibilityNodeInfo::UNDEFINED
        || parentAccessibilityViewId == hostAccessViewId)
    {
        AutoPtr<IAccessibilityNodeInfo> parent;
        provider->CreateAccessibilityNodeInfo(parentVirtualDescendantId, (IAccessibilityNodeInfo**)&parent);
        if (parent != NULL) {
            AutoPtr<IObjectInt32Map> childNodeIds;
            parent->GetChildNodeIds((IObjectInt32Map**)&childNodeIds);
            Int32 childCount;
            childNodeIds->GetSize(&childCount);

            Int64 sourceNodeId, childNodeId;
            current->GetSourceNodeId(&sourceNodeId);
            Int32 childVirtualDescendantId;
            Int32 count;

            for (Int32 i = 0; i < childCount; i++) {
                if (outInfos->GetObjectCount(&count), count >= MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
                    return;
                }

                AutoPtr<IInterface> obj;
                childNodeIds->Get(i, (IInterface**)&obj);
                AutoPtr<IInteger64> cid = IInteger64::Probe(obj);
                cid->GetValue(&childNodeId);

                if (childNodeId != sourceNodeId) {
                    helper->GetVirtualDescendantId(childNodeId, &childVirtualDescendantId);
                    AutoPtr<IAccessibilityNodeInfo> child;
                    provider->CreateAccessibilityNodeInfo(
                        childVirtualDescendantId, (IAccessibilityNodeInfo**)&child);
                    if (child != NULL) {
                        outInfos->Add(child);
                    }
                }
            }
        }
    }
    else {
        PrefetchSiblingsOfRealNode(providerHost, outInfos);
    }
}

void CAccessibilityInteractionController::AccessibilityNodePrefetcher::PrefetchDescendantsOfVirtualNode(
    /* [in] */ IAccessibilityNodeInfo* root,
    /* [in] */ IAccessibilityNodeProvider* provider,
    /* [in] */ IObjectContainer* outInfos)
{
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
    AutoPtr<IObjectInt32Map> childNodeIds;
    root->GetChildNodeIds((IObjectInt32Map**)&childNodeIds);
    Int32 childCount, virtualDescendantId;
    childNodeIds->GetSize(&childCount);
    Int32 initialOutInfosSize;
    outInfos->GetObjectCount(&initialOutInfosSize);
    Int64 childNodeId;
    Int32 count;

    for (Int32 i = 0; i < childCount; i++) {
        if (outInfos->GetObjectCount(&count), count >= MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
            return;
        }

        AutoPtr<IInterface> obj;
        childNodeIds->Get(i, (IInterface**)&obj);
        AutoPtr<IInteger64> cid = IInteger64::Probe(obj);
        cid->GetValue(&childNodeId);

        helper->GetVirtualDescendantId(childNodeId, &virtualDescendantId);
        AutoPtr<IAccessibilityNodeInfo> child;
        provider->CreateAccessibilityNodeInfo(
            virtualDescendantId, (IAccessibilityNodeInfo**)&child);
        if (child != NULL) {
            outInfos->Add(child);
        }
    }

    outInfos->GetObjectCount(&count);
    if (count < MAX_ACCESSIBILITY_NODE_INFO_BATCH_SIZE) {
        Int32 addedChildCount = count - initialOutInfosSize;
        AutoPtr<IObjectEnumerator> emu;
        outInfos->GetObjectEnumerator((IObjectEnumerator**)&emu);
        Boolean hasNext;
        Int32 i = 0;
        while ((emu->MoveNext(&hasNext), hasNext)) {
            if (i >= initialOutInfosSize + addedChildCount) {
                break;
            }

            if (i >= initialOutInfosSize) {
                AutoPtr<IInterface> obj;
                emu->Current((IInterface**)&obj);
                AutoPtr<IAccessibilityNodeInfo> child = IAccessibilityNodeInfo::Probe(obj);
                PrefetchDescendantsOfVirtualNode(child, provider, outInfos);
            }

            ++i;
        }
    }
}

//========================================================================================
// CAccessibilityInteractionController::PrivateHandler
//========================================================================================
CAccessibilityInteractionController::PrivateHandler::PrivateHandler(
    /* [in] */ ILooper* looper,
    /* [in] */ CAccessibilityInteractionController* host)
    : HandlerBase(looper)
    , mHost(host)
{}

String CAccessibilityInteractionController::PrivateHandler::GetMessageNameImpl(
    /* [in] */ IMessage* message)
{
    Int32 type;
    message->GetWhat(&type);
    switch (type) {
        case MSG_PERFORM_ACCESSIBILITY_ACTION:
            return String("MSG_PERFORM_ACCESSIBILITY_ACTION");
        case MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_ACCESSIBILITY_ID:
            return String("MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_ACCESSIBILITY_ID");
        case MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_VIEW_ID:
            return String("MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_VIEW_ID");
        case MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_TEXT:
            return String("MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_TEXT");
        case MSG_FIND_FOCUS:
            return String("MSG_FIND_FOCUS");
        case MSG_FOCUS_SEARCH:
            return String("MSG_FOCUS_SEARCH");
        default:
            // throw new IllegalArgumentException("Unknown message type: " + type);
            break;
    }
    return String("unknown message");
}

ECode CAccessibilityInteractionController::PrivateHandler::HandleMessage(
    /* [in] */ IMessage* message)
{
    Int32 type;
    message->GetWhat(&type);
    switch (type) {
        case MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_ACCESSIBILITY_ID: {
            mHost->FindAccessibilityNodeInfoByAccessibilityIdUiThread(message);
        } break;
        case MSG_PERFORM_ACCESSIBILITY_ACTION: {
            mHost->PerfromAccessibilityActionUiThread(message);
        } break;
        case MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_VIEW_ID: {
            mHost->FindAccessibilityNodeInfoByViewIdUiThread(message);
        } break;
        case MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_TEXT: {
            mHost->FindAccessibilityNodeInfosByTextUiThread(message);
        } break;
        case MSG_FIND_FOCUS: {
            mHost->FindFocusUiThread(message);
        } break;
        case MSG_FOCUS_SEARCH: {
            mHost->FocusSearchUiThread(message);
        } break;
        default:
            return E_ILLEGAL_ARGUMENT_EXCEPTION;
            // throw new IllegalArgumentException("Unknown message type: " + type);
    }
    return NOERROR;
}

//========================================================================================
// CAccessibilityInteractionController
//========================================================================================
CAccessibilityInteractionController::CAccessibilityInteractionController()
    : mViewRootImpl(NULL)
    , mMyLooperThreadId(0)
    , mMyProcessId(0)
{
    CRect::New((IRect**)&mTempRect);
}

ECode CAccessibilityInteractionController::constructor(
    /* [in] */ Handle32 viewRootImpl)
{
    mViewRootImpl = (ViewRootImpl*)viewRootImpl;
    AutoPtr<ILooper> looper;
    mViewRootImpl->mHandler->GetLooper((ILooper**)&looper);
    AutoPtr<IThread> thread;
    looper->GetThread((IThread**)&thread);
    thread->GetId(&mMyLooperThreadId);
    mMyProcessId = Process::MyPid();
    mHandler = new PrivateHandler(looper, this);
    mPrefetcher = new AccessibilityNodePrefetcher(this);
    return NOERROR;
}

ECode CAccessibilityInteractionController::FindAccessibilityNodeInfoByAccessibilityIdClientThread(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 interactionId,
    /* [in] */ IAccessibilityInteractionConnectionCallback* callback,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 interrogatingPid,
    /* [in] */ Int64 interrogatingTid)
{
    AutoPtr<IMessage> message;
    mHandler->ObtainMessage(
        PrivateHandler::MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_ACCESSIBILITY_ID,
        flags, 0, (IMessage**)&message);

    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    helper->GetAccessibilityViewId(accessibilityNodeId, &args->mArgi1);
    helper->GetVirtualDescendantId(accessibilityNodeId, &args->mArgi2);
    args->mArgi3 = interactionId;
    args->mArg1 = callback;
    message->SetObj(args);

    // If the interrogation is performed by the same thread as the main UI
    // thread in this process, set the message as a static reference so
    // after this call completes the same thread but in the interrogating
    // client can handle the message to generate the result.
    if (interrogatingPid == mMyProcessId && interrogatingTid == mMyLooperThreadId) {
        AutoPtr<IAccessibilityInteractionClientHelper> aicHelper;
        CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&aicHelper);
        AutoPtr<IAccessibilityInteractionClient> aic;
        aicHelper->GetInstanceForThread(interrogatingTid, (IAccessibilityInteractionClient**)&aic);
        aic->SetSameThreadMessage(message);
    }
    else {
        Boolean result;
        mHandler->SendMessage(message, &result);
    }
    return NOERROR;
}

ECode CAccessibilityInteractionController::FindAccessibilityNodeInfoByViewIdClientThread(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 viewId,
    /* [in] */ Int32 interactionId,
    /* [in] */ IAccessibilityInteractionConnectionCallback* callback,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 interrogatingPid,
    /* [in] */ Int64 interrogatingTid)
{
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
    Int32 avId;
    helper->GetAccessibilityViewId(accessibilityNodeId, &avId);

    AutoPtr<IMessage> message;
    mHandler->ObtainMessage(
        PrivateHandler::MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_VIEW_ID,
        flags, avId, (IMessage**)&message);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi1 = viewId;
    args->mArgi2 = interactionId;
    args->mArg1 = callback;
    message->SetObj(args);

    // If the interrogation is performed by the same thread as the main UI
    // thread in this process, set the message as a static reference so
    // after this call completes the same thread but in the interrogating
    // client can handle the message to generate the result.
    if (interrogatingPid == mMyProcessId && interrogatingTid == mMyLooperThreadId) {
        AutoPtr<IAccessibilityInteractionClientHelper> aicHelper;
        CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&aicHelper);
        AutoPtr<IAccessibilityInteractionClient> aic;
        aicHelper->GetInstanceForThread(interrogatingTid, (IAccessibilityInteractionClient**)&aic);
        aic->SetSameThreadMessage(message);
    }
    else {
        Boolean result;
        mHandler->SendMessage(message, &result);
    }
    return NOERROR;
}

ECode CAccessibilityInteractionController::FindAccessibilityNodeInfosByTextClientThread(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ const String& text,
    /* [in] */ Int32 interactionId,
    /* [in] */ IAccessibilityInteractionConnectionCallback* callback,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 interrogatingPid,
    /* [in] */ Int64 interrogatingTid)
{
    AutoPtr<IMessage> message;
    mHandler->ObtainMessage(
        PrivateHandler::MSG_FIND_ACCESSIBLITY_NODE_INFO_BY_TEXT,
        flags, 0, (IMessage**)&message);

    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);

    AutoPtr<ICharSequence> seq;
    CString::New(text, (ICharSequence**)&seq);
    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArg1 = seq;
    args->mArg2 = callback;
    helper->GetAccessibilityViewId(accessibilityNodeId, &args->mArgi1);
    helper->GetVirtualDescendantId(accessibilityNodeId, &args->mArgi2);
    args->mArgi3 = interactionId;

    message->SetObj(args);

    // If the interrogation is performed by the same thread as the main UI
    // thread in this process, set the message as a static reference so
    // after this call completes the same thread but in the interrogating
    // client can handle the message to generate the result.
    if (interrogatingPid == mMyProcessId && interrogatingTid == mMyLooperThreadId) {
        AutoPtr<IAccessibilityInteractionClientHelper> aicHelper;
        CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&aicHelper);
        AutoPtr<IAccessibilityInteractionClient> aic;
        aicHelper->GetInstanceForThread(interrogatingTid, (IAccessibilityInteractionClient**)&aic);
        aic->SetSameThreadMessage(message);
    }
    else {
        Boolean result;
        mHandler->SendMessage(message, &result);
    }
    return NOERROR;
}

ECode CAccessibilityInteractionController::FindFocusClientThread(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 focusType,
    /* [in] */ Int32 interactionId,
    /* [in] */ IAccessibilityInteractionConnectionCallback* callback,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 interogatingPid,
    /* [in] */ Int64 interrogatingTid)
{
    AutoPtr<IMessage> message;
    mHandler->ObtainMessage(
        PrivateHandler::MSG_FIND_FOCUS,
        flags, focusType, (IMessage**)&message);

    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi1 = interactionId;
    helper->GetAccessibilityViewId(accessibilityNodeId, &args->mArgi2);
    helper->GetVirtualDescendantId(accessibilityNodeId, &args->mArgi3);
    args->mArg1 = callback;

    message->SetObj(args);

    // If the interrogation is performed by the same thread as the main UI
    // thread in this process, set the message as a static reference so
    // after this call completes the same thread but in the interrogating
    // client can handle the message to generate the result.
    if (interogatingPid == mMyProcessId && interrogatingTid == mMyLooperThreadId) {
        AutoPtr<IAccessibilityInteractionClientHelper> aicHelper;
        CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&aicHelper);
        AutoPtr<IAccessibilityInteractionClient> aic;
        aicHelper->GetInstanceForThread(interrogatingTid, (IAccessibilityInteractionClient**)&aic);
        aic->SetSameThreadMessage(message);
    }
    else {
        Boolean result;
        mHandler->SendMessage(message, &result);
    }
    return NOERROR;
}

ECode CAccessibilityInteractionController::FocusSearchClientThread(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 direction,
    /* [in] */ Int32 interactionId,
    /* [in] */ IAccessibilityInteractionConnectionCallback* callback,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 interogatingPid,
    /* [in] */ Int64 interrogatingTid)
{
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
    Int32 avId;
    helper->GetAccessibilityViewId(accessibilityNodeId, &avId);

    AutoPtr<IMessage> message;
    mHandler->ObtainMessage(
        PrivateHandler::MSG_FOCUS_SEARCH,
        flags, avId, (IMessage**)&message);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    args->mArgi2 = direction;
    args->mArgi3 = interactionId;
    args->mArg1 = callback;

    message->SetObj(args);

    // If the interrogation is performed by the same thread as the main UI
    // thread in this process, set the message as a static reference so
    // after this call completes the same thread but in the interrogating
    // client can handle the message to generate the result.
    if (interogatingPid == mMyProcessId && interrogatingTid == mMyLooperThreadId) {
        AutoPtr<IAccessibilityInteractionClientHelper> aicHelper;
        CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&aicHelper);
        AutoPtr<IAccessibilityInteractionClient> aic;
        aicHelper->GetInstanceForThread(interrogatingTid, (IAccessibilityInteractionClient**)&aic);
        aic->SetSameThreadMessage(message);
    }
    else {
        Boolean result;
        mHandler->SendMessage(message, &result);
    }
    return NOERROR;
}

ECode CAccessibilityInteractionController::PerformAccessibilityActionClientThread(
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [in] */ Int32 interactionId,
    /* [in] */ IAccessibilityInteractionConnectionCallback* callback,
    /* [in] */ Int32 flags,
    /* [in] */ Int32 interogatingPid,
    /* [in] */ Int64 interrogatingTid)
{
    AutoPtr<IAccessibilityNodeInfoHelper> helper;
    CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
    Int32 avId;
    helper->GetAccessibilityViewId(accessibilityNodeId, &avId);

    AutoPtr<IMessage> message;
    mHandler->ObtainMessage(
        PrivateHandler::MSG_PERFORM_ACCESSIBILITY_ACTION,
        flags, avId, (IMessage**)&message);

    AutoPtr<SomeArgs> args = SomeArgs::Obtain();
    helper->GetVirtualDescendantId(accessibilityNodeId, &args->mArgi1);
    args->mArgi2 = action;
    args->mArgi3 = interactionId;
    args->mArg1 = callback;
    args->mArg2 = arguments;

    message->SetObj(args);

    // If the interrogation is performed by the same thread as the main UI
    // thread in this process, set the message as a static reference so
    // after this call completes the same thread but in the interrogating
    // client can handle the message to generate the result.
    if (interogatingPid == mMyProcessId && interrogatingTid == mMyLooperThreadId) {
        AutoPtr<IAccessibilityInteractionClientHelper> aicHelper;
        CAccessibilityInteractionClientHelper::AcquireSingleton((IAccessibilityInteractionClientHelper**)&aicHelper);
        AutoPtr<IAccessibilityInteractionClient> aic;
        aicHelper->GetInstanceForThread(interrogatingTid, (IAccessibilityInteractionClient**)&aic);
        aic->SetSameThreadMessage(message);
    }
    else {
        Boolean result;
        mHandler->SendMessage(message, &result);
    }
    return NOERROR;
}

Boolean CAccessibilityInteractionController::IsShown(
    /* [in] */ IView* view)
{
    // The first two checks are made also made by IsShown() which
    // however traverses the tree up to the parent to catch that.
    // Therefore, we do some fail fast check to minimize the up
    // tree traversal.
    if (view == NULL) {
        return FALSE;
    }

    View* viewObj = reinterpret_cast<View*>(view->Probe(EIID_View));
    assert(viewObj != NULL);

    return (viewObj->mAttachInfo != NULL
        && viewObj->mAttachInfo->mWindowVisibility == IView::VISIBLE
        && viewObj->IsShown());
}

void CAccessibilityInteractionController::FindAccessibilityNodeInfoByAccessibilityIdUiThread(
    /* [in] */ IMessage* message)
{
    Int32 flags;
    message->GetArg1(&flags);

    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    SomeArgs* args = (SomeArgs*)obj.Get();
    const Int32 accessibilityViewId = args->mArgi1;
    const Int32 virtualDescendantId = args->mArgi2;
    const Int32 interactionId = args->mArgi3;
    AutoPtr<IAccessibilityInteractionConnectionCallback> callback =
        IAccessibilityInteractionConnectionCallback::Probe(args->mArg1);;

    args->Recycle();

    AutoPtr<IObjectContainer> infos = mTempAccessibilityNodeInfoList;
    infos->Dispose();

    if (mViewRootImpl->mView == NULL || mViewRootImpl->mAttachInfo == NULL) {
        return;
    }
    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews =
        (flags & IAccessibilityNodeInfo::INCLUDE_NOT_IMPORTANT_VIEWS) != 0;
    AutoPtr<IView> root;
    if (accessibilityViewId == IAccessibilityNodeInfo::UNDEFINED) {
        root = mViewRootImpl->mView;
    }
    else {
        root = FindViewByAccessibilityId(accessibilityViewId);
    }
    if (root != NULL && IsShown(root)) {
        mPrefetcher->PrefetchAccessibilityNodeInfos(root, virtualDescendantId, flags, infos);
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews = FALSE;
    ApplyApplicationScaleIfNeeded(infos);
    callback->SetFindAccessibilityNodeInfosResult(infos, interactionId);
    infos->Dispose();
}

void CAccessibilityInteractionController::FindAccessibilityNodeInfoByViewIdUiThread(
    /* [in] */ IMessage* message)
{
    Int32 flags;
    message->GetArg1(&flags);
    Int32 accessibilityViewId;
    message->GetArg2(&accessibilityViewId);

    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    SomeArgs* args = (SomeArgs*)obj.Get();
    const Int32 viewId = args->mArgi1;
    const Int32 interactionId = args->mArgi2;
    AutoPtr<IAccessibilityInteractionConnectionCallback> callback =
        IAccessibilityInteractionConnectionCallback::Probe(args->mArg1);;

    args->Recycle();

    AutoPtr<IAccessibilityNodeInfo> info;

    if (mViewRootImpl->mView == NULL || mViewRootImpl->mAttachInfo == NULL) {
        return;
    }
    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews =
        (flags & IAccessibilityNodeInfo::INCLUDE_NOT_IMPORTANT_VIEWS) != 0;
    AutoPtr<IView> root;
    if (accessibilityViewId != IAccessibilityNodeInfo::UNDEFINED) {
        root = FindViewByAccessibilityId(accessibilityViewId);
    }
    else {
        root = mViewRootImpl->mView;
    }

    if (root != NULL) {
        AutoPtr<IView> target;
        root->FindViewById(viewId, (IView**)&target);
        if (target != NULL && IsShown(target)) {
            target->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&info);
        }
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews = FALSE;
    ApplyApplicationScaleIfNeeded(info);
    callback->SetFindAccessibilityNodeInfoResult(info, interactionId);
}

void CAccessibilityInteractionController::FindAccessibilityNodeInfosByTextUiThread(
    /* [in] */ IMessage* message)
{
    Int32 flags;
    message->GetArg1(&flags);

    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    SomeArgs* args = (SomeArgs*)obj.Get();
    AutoPtr<ICharSequence> seq = ICharSequence::Probe(args->mArg1);
    String text;
    seq->ToString(&text);
    AutoPtr<IAccessibilityInteractionConnectionCallback> callback =
        IAccessibilityInteractionConnectionCallback::Probe(args->mArg2);
    const Int32 accessibilityViewId = args->mArgi1;
    const Int32 virtualDescendantId = args->mArgi2;
    const Int32 interactionId = args->mArgi3;
    args->Recycle();

    AutoPtr<IObjectContainer> infos;
    if (mViewRootImpl->mView == NULL || mViewRootImpl->mAttachInfo == NULL) {
        return;
    }
    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews =
        (flags & IAccessibilityNodeInfo::INCLUDE_NOT_IMPORTANT_VIEWS) != 0;
    AutoPtr<IView> root;
    if (accessibilityViewId != IAccessibilityNodeInfo::UNDEFINED) {
        root = FindViewByAccessibilityId(accessibilityViewId);
    }
    else {
        root = mViewRootImpl->mView;
    }

    if (root != NULL && IsShown(root)) {
        AutoPtr<IAccessibilityNodeProvider> provider;
        root->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
        if (provider != NULL) {
            provider->FindAccessibilityNodeInfosByText(text,
                virtualDescendantId, (IObjectContainer**)&infos);
        }
        else if (virtualDescendantId == IAccessibilityNodeInfo::UNDEFINED) {
            AutoPtr<IObjectContainer> foundViews;
            CObjectContainer::New((IObjectContainer**)&foundViews);
            root->FindViewsWithText(foundViews, seq,
                IView::FIND_VIEWS_WITH_TEXT
                    | IView::FIND_VIEWS_WITH_CONTENT_DESCRIPTION
                    | IView::FIND_VIEWS_WITH_ACCESSIBILITY_NODE_PROVIDERS);
            Int32 viewCount;
            foundViews->GetObjectCount(&viewCount);
            if (viewCount > 0) {
                infos = mTempAccessibilityNodeInfoList;
                infos->Dispose();

                AutoPtr<IObjectEnumerator> emu;
                foundViews->GetObjectEnumerator((IObjectEnumerator**)&emu);
                AutoPtr<IView> foundView;
                Boolean hasNext, ifpHasNext;
                while (emu->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> obj;
                    emu->Current((IInterface**)&obj);
                    foundView = IView::Probe(obj);
                    if (IsShown(foundView)) {
                        provider = NULL;
                        foundView->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
                        if (provider != NULL) {
                            AutoPtr<IObjectContainer> infosFromProvider;
                            provider->FindAccessibilityNodeInfosByText(text,
                                IAccessibilityNodeInfo::UNDEFINED, (IObjectContainer**)&infosFromProvider);
                            if (infosFromProvider != NULL) {
                                AutoPtr<IObjectEnumerator> ifpEmu;
                                infosFromProvider->GetObjectEnumerator((IObjectEnumerator**)&ifpEmu);
                                while (ifpEmu->MoveNext(&ifpHasNext), ifpHasNext) {
                                    AutoPtr<IInterface> ifpObj;
                                    ifpEmu->Current((IInterface**)&ifpObj);
                                    infos->Add(ifpObj);
                                }
                            }
                        }
                        else {
                            AutoPtr<IAccessibilityNodeInfo> nodeInfo;
                            foundView->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&nodeInfo);
                            infos->Add(nodeInfo);
                        }
                    }
                }
            }
        }
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews = FALSE;
    ApplyApplicationScaleIfNeeded(infos);
    callback->SetFindAccessibilityNodeInfosResult(infos, interactionId);
}

void CAccessibilityInteractionController::FindFocusUiThread(
    /* [in] */ IMessage* message)
{
    Int32 flags;
    message->GetArg1(&flags);
    Int32 focusType;
    message->GetArg2(&focusType);

    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    SomeArgs* args = (SomeArgs*)obj.Get();
    const Int32 interactionId = args->mArgi1;
    const Int32 accessibilityViewId = args->mArgi2;
    const Int32 virtualDescendantId = args->mArgi3;
    AutoPtr<IAccessibilityInteractionConnectionCallback> callback =
        IAccessibilityInteractionConnectionCallback::Probe(args->mArg1);;

    args->Recycle();

    AutoPtr<IAccessibilityNodeInfo> focused;

    if (mViewRootImpl->mView == NULL || mViewRootImpl->mAttachInfo == NULL) {
        return;
    }
    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews =
        (flags & IAccessibilityNodeInfo::INCLUDE_NOT_IMPORTANT_VIEWS) != 0;
    AutoPtr<IView> root;
    if (accessibilityViewId != IAccessibilityNodeInfo::UNDEFINED) {
        root = FindViewByAccessibilityId(accessibilityViewId);
    }
    else {
        root = mViewRootImpl->mView;
    }

    if (root != NULL && IsShown(root)) {
        switch (focusType) {
            case IAccessibilityNodeInfo::FOCUS_ACCESSIBILITY: {
                AutoPtr<IView> host = mViewRootImpl->mAccessibilityFocusedHost;
                // If there is no accessibility focus host or it is not a descendant
                // of the root from which to start the search, then the search failed.
                if (host == NULL || !ViewRootImpl::IsViewDescendantOf(host, root)) {
                    break;
                }
                // If the host has a provider ask this provider to search for the
                // focus instead fetching all provider nodes to do the search here.
                AutoPtr<IAccessibilityNodeProvider> provider;
                host->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
                if (provider != NULL) {
                    if (mViewRootImpl->mAccessibilityFocusedVirtualView != NULL) {
                        AutoPtr<IAccessibilityNodeInfoHelper> helper;
                        CAccessibilityNodeInfoHelper::AcquireSingleton((IAccessibilityNodeInfoHelper**)&helper);
                        helper->Obtain(
                            mViewRootImpl->mAccessibilityFocusedVirtualView,
                            (IAccessibilityNodeInfo**)&focused);
                    }
                }
                else if (virtualDescendantId == IView::NO_ID) {
                    host->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&focused);
                }
            } break;

            case IAccessibilityNodeInfo::FOCUS_INPUT: {
                // Input focus cannot go to virtual views.
                AutoPtr<IView> target;
                root->FindFocus((IView**)&target);
                if (target != NULL && IsShown(target)) {
                    target->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&focused);
                }
            } break;

            default:
                Slogger::E("CAccessibilityInteractionController", "Unknown focus type:  %d", focusType);
                break;
                // throw new IllegalArgumentException("Unknown focus type: " + focusType);
        }
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews = FALSE;
    ApplyApplicationScaleIfNeeded(focused);
    callback->SetFindAccessibilityNodeInfoResult(focused, interactionId);
}

void CAccessibilityInteractionController::FocusSearchUiThread(
    /* [in] */ IMessage* message)
{
    Int32 flags;
    message->GetArg1(&flags);
    Int32 accessibilityViewId;
    message->GetArg2(&accessibilityViewId);

    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    SomeArgs* args = (SomeArgs*)obj.Get();
    const Int32 direction = args->mArgi2;
    const Int32 interactionId = args->mArgi3;
    AutoPtr<IAccessibilityInteractionConnectionCallback> callback =
        IAccessibilityInteractionConnectionCallback::Probe(args->mArg1);;

    args->Recycle();

    AutoPtr<IAccessibilityNodeInfo> next;
    if (mViewRootImpl->mView == NULL || mViewRootImpl->mAttachInfo == NULL) {
        return;
    }
    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews =
        (flags & IAccessibilityNodeInfo::INCLUDE_NOT_IMPORTANT_VIEWS) != 0;
    AutoPtr<IView> root;
    if (accessibilityViewId != IAccessibilityNodeInfo::UNDEFINED) {
        root = FindViewByAccessibilityId(accessibilityViewId);
    }
    else {
        root = mViewRootImpl->mView;
    }

    if (root != NULL && IsShown(root)) {
        AutoPtr<IView> nextView;
        root->FocusSearch(direction, (IView**)&nextView);
        if (nextView != NULL) {
            nextView->CreateAccessibilityNodeInfo((IAccessibilityNodeInfo**)&next);
        }
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews = FALSE;
    ApplyApplicationScaleIfNeeded(next);
    callback->SetFindAccessibilityNodeInfoResult(next, interactionId);
}

void CAccessibilityInteractionController::PerfromAccessibilityActionUiThread(
    /* [in] */ IMessage* message)
{
    Int32 flags;
    message->GetArg1(&flags);
    Int32 accessibilityViewId;
    message->GetArg2(&accessibilityViewId);

    AutoPtr<IInterface> obj;
    message->GetObj((IInterface**)&obj);
    SomeArgs* args = (SomeArgs*)obj.Get();
    const Int32 virtualDescendantId = args->mArgi1;
    const Int32 action = args->mArgi2;
    const Int32 interactionId = args->mArgi3;
    AutoPtr<IAccessibilityInteractionConnectionCallback> callback =
        IAccessibilityInteractionConnectionCallback::Probe(args->mArg1);;
    AutoPtr<IBundle> arguments = IBundle::Probe(args->mArg2);

    args->Recycle();

    Boolean succeeded = FALSE;
    if (mViewRootImpl->mView == NULL || mViewRootImpl->mAttachInfo == NULL) {
        return;
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews =
        (flags & IAccessibilityNodeInfo::INCLUDE_NOT_IMPORTANT_VIEWS) != 0;
    AutoPtr<IView> target;
    if (accessibilityViewId != IAccessibilityNodeInfo::UNDEFINED) {
        target = FindViewByAccessibilityId(accessibilityViewId);
    }
    else {
        target = mViewRootImpl->mView;
    }

    if (target != NULL && IsShown(target)) {
        AutoPtr<IAccessibilityNodeProvider> provider;
        target->GetAccessibilityNodeProvider((IAccessibilityNodeProvider**)&provider);
        if (provider != NULL) {
            provider->PerformAction(virtualDescendantId, action, arguments, &succeeded);
        }
        else if (virtualDescendantId == IView::NO_ID) {
            target->PerformAccessibilityAction(action, arguments, &succeeded);
        }
    }

    mViewRootImpl->mAttachInfo->mIncludeNotImportantViews = FALSE;
    callback->SetPerformAccessibilityActionResult(succeeded, interactionId);
}

AutoPtr<IView> CAccessibilityInteractionController::FindViewByAccessibilityId(
    /* [in] */ Int32 accessibilityId)
{
    AutoPtr<IView> root = mViewRootImpl->mView;
    if (root == NULL) {
        return NULL;
    }

    View* rootObj = reinterpret_cast<View*>(root->Probe(EIID_View));
    AutoPtr<IView> foundView = rootObj->FindViewByAccessibilityId(accessibilityId);
    if (foundView != NULL && !IsShown(foundView)) {
        return NULL;
    }
    return foundView;
}

void CAccessibilityInteractionController::ApplyApplicationScaleIfNeeded(
    /* [in] */ IObjectContainer* infos)
{
    if (infos == NULL) {
        return;
    }
    const Float applicationScale = mViewRootImpl->mAttachInfo->mApplicationScale;
    if (applicationScale != 1.0f) {
        AutoPtr<IObjectEnumerator> emu;
        infos->GetObjectEnumerator((IObjectEnumerator**)&emu);
        Boolean hasNext;
        while (emu->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInterface> obj;
            emu->Current((IInterface**)&obj);
            ApplyApplicationScaleIfNeeded(IAccessibilityNodeInfo::Probe(obj));
        }
    }
}

void CAccessibilityInteractionController::ApplyApplicationScaleIfNeeded(
    /* [in] */ IAccessibilityNodeInfo* info)
{
    if (info == NULL) {
        return;
    }

    const Float applicationScale = mViewRootImpl->mAttachInfo->mApplicationScale;
    if (applicationScale != 1.0f) {
        AutoPtr<IRect> bounds = mTempRect;

        info->GetBoundsInParent(bounds);
        bounds->Scale(applicationScale);
        info->SetBoundsInParent(bounds);

        info->GetBoundsInScreen(bounds);
        bounds->Scale(applicationScale);
        info->SetBoundsInScreen(bounds);
    }
}

} // View
} // Droid
} // Elastos
