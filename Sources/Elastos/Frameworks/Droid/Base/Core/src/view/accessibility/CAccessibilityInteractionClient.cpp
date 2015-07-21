
#include "view/accessibility/CAccessibilityInteractionClient.h"
#include "graphics/CRect.h"
#include "os/Binder.h"
#include "os/Build.h"
#include "os/Process.h"
#include "os/SystemClock.h"
#include <elastos/core/Thread.h>
#include <elastos/utility/logging/Slogger.h>
#include <elastos/utility/etl/HashSet.h>

using Elastos::Core::CObjectContainer;
using Elastos::Core::IInteger64;
using Elastos::Core::Thread;
using Elastos::Utility::Etl::HashSet;
using Elastos::Utility::IObjectInt32Map;
using Elastos::Utility::Logging::Slogger;
using Elastos::Utility::Concurrent::Atomic::CAtomicInteger32;
using Elastos::Droid::Graphics::CRect;
using Elastos::Droid::Os::Build;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Os::Process;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::IHandler;
using Elastos::Droid::View::Accessibility::IAccessibilityNodeInfo;

namespace Elastos {
namespace Droid {
namespace View {
namespace Accessibility {

const Int32 CAccessibilityInteractionClient::NO_ID = -1;
const String CAccessibilityInteractionClient::TAG("CAccessibilityInteractionClient");
const Boolean CAccessibilityInteractionClient::DEBUG = TRUE;
const Boolean CAccessibilityInteractionClient::CHECK_INTEGRITY = TRUE;
const Int64 CAccessibilityInteractionClient::TIMEOUT_INTERACTION_MILLIS = 5000;

Mutex CAccessibilityInteractionClient::sStaticLock;
HashMap<Int64, AutoPtr<CAccessibilityInteractionClient> > CAccessibilityInteractionClient::sClients;
HashMap<Int32, AutoPtr<IAccessibilityServiceConnection> > CAccessibilityInteractionClient::sConnectionCache;
Mutex CAccessibilityInteractionClient::sConnectionCacheLock;
const AutoPtr<AccessibilityNodeInfoCache> CAccessibilityInteractionClient::sAccessibilityNodeInfoCache
    = new AccessibilityNodeInfoCache();

CAccessibilityInteractionClient::CAccessibilityInteractionClient()
    : mInteractionId(0)
    , mPerformAccessibilityActionResult(FALSE)
{
    mFindAccessibilityNodeInfosResult = new List<AutoPtr<IAccessibilityNodeInfo> >();
    ASSERT_SUCCEEDED(CAtomicInteger32::New((IAtomicInteger32**)&mInteractionIdCounter));
    ASSERT_SUCCEEDED(CRect::New((IRect**)&mTempBounds));
}

CAccessibilityInteractionClient::~CAccessibilityInteractionClient()
{
}

AutoPtr<CAccessibilityInteractionClient> CAccessibilityInteractionClient::GetInstance()
{
    Int64 threadId;
    Thread::GetCurrentThread()->GetId(&threadId);
    return GetInstanceForThread(threadId);
}

AutoPtr<CAccessibilityInteractionClient> CAccessibilityInteractionClient::GetInstanceForThread(
    /* [in] */ Int64 threadId)
{
    AutoLock lock(sStaticLock);
    AutoPtr<CAccessibilityInteractionClient> client;
    HashMap<Int64, AutoPtr<CAccessibilityInteractionClient> >::Iterator it =  sClients.Find(threadId);
    if (it != sClients.End()) {
        client = it->mSecond;
    }
    if (client == NULL) {
        CAccessibilityInteractionClient::NewByFriend((CAccessibilityInteractionClient**)&client);
        sClients[threadId] = client;
    }
    return client;
}

ECode CAccessibilityInteractionClient::SetSameThreadMessage(
    /* [in] */ IMessage* message)
{
    AutoLock lock(mInstanceLock);
    mSameThreadMessage = message;
    mInstanceLock.NotifyAll();
    return NOERROR;
}

ECode CAccessibilityInteractionClient::GetRootInActiveWindow(
    /* [in] */ Int32 connectionId,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    return FindAccessibilityNodeInfoByAccessibilityId(connectionId,
        IAccessibilityNodeInfo::ACTIVE_WINDOW_ID, IAccessibilityNodeInfo::ROOT_NODE_ID,
        IAccessibilityNodeInfo::FLAG_PREFETCH_DESCENDANTS,
        info);
}

ECode CAccessibilityInteractionClient::FindAccessibilityNodeInfoByAccessibilityId(
    /* [in] */ Int32 connectionId,
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 prefetchFlags,
    /* [out] */ IAccessibilityNodeInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    // try {
    AutoPtr<IAccessibilityServiceConnection> connection;
    GetConnection(connectionId, (IAccessibilityServiceConnection**)&connection);
    if (connection != NULL) {
        AutoPtr<IAccessibilityNodeInfo> cachedInfo = sAccessibilityNodeInfoCache->Get(accessibilityNodeId);
        if (cachedInfo != NULL) {
            *info = cachedInfo;
            REFCOUNT_ADD(*info);
            return NOERROR;
        }

        Int32 interactionId;
        mInteractionIdCounter->GetAndIncrement(&interactionId);
        Int64 id;
        Thread::GetCurrentThread()->GetId(&id);
        Float windowScale;
        ECode ec = connection->FindAccessibilityNodeInfoByAccessibilityId(
            accessibilityWindowId, accessibilityNodeId, interactionId,
            THIS_PROBE(IAccessibilityInteractionConnectionCallback),
            prefetchFlags, id, &windowScale);
        if (FAILED(ec)) {
            if (DEBUG) {
                Slogger::W(TAG, "Error while calling remote" \
                    " findAccessibilityNodeInfoByAccessibilityId, ec=%08x", ec);
            }
            return NOERROR;
        }

        // If the scale is zero the call has failed.
        if (windowScale > 0) {
            AutoPtr< List<AutoPtr<IAccessibilityNodeInfo> > > infos
                = GetFindAccessibilityNodeInfosResultAndClear(interactionId);
            FinalizeAndCacheAccessibilityNodeInfos(infos, connectionId, windowScale);
            if (infos != NULL && !infos->IsEmpty()) {
                *info = *infos->Begin();
                REFCOUNT_ADD(*info);
                return NOERROR;
            }
        }
    }
    else {
        if (DEBUG) {
            Slogger::W(TAG, "No connection for connection id: %d", connectionId);
        }
    }
    // } catch (RemoteException re) {
    //     if (DEBUG) {
    //         Slogger::W(TAG, "Error while calling remote"
    //                 + " findAccessibilityNodeInfoByAccessibilityId", re);
    //     }
    // }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::FindAccessibilityNodeInfoByViewId(
    /* [in] */ Int32 connectionId,
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 viewId,
    /* [out] */ IAccessibilityNodeInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    // try {
    AutoPtr<IAccessibilityServiceConnection> connection;
    GetConnection(connectionId, (IAccessibilityServiceConnection**)&connection);
    if (connection != NULL) {
        Int32 interactionId;
        mInteractionIdCounter->GetAndIncrement(&interactionId);
        Int64 id;
        Thread::GetCurrentThread()->GetId(&id);
        Float windowScale;
        ECode ec = connection->FindAccessibilityNodeInfoByViewId(
            accessibilityWindowId, accessibilityNodeId, viewId, interactionId,
            THIS_PROBE(IAccessibilityInteractionConnectionCallback),
            id, &windowScale);
        if (FAILED(ec)) {
            if (DEBUG) {
                Slogger::W(TAG, "Error while calling remote" \
                    " findAccessibilityNodeInfoByViewIdInActiveWindow, ec=%d", ec);
            }
            return NOERROR;
        }
        // If the scale is zero the call has failed.
        if (windowScale > 0) {
            AutoPtr<IAccessibilityNodeInfo> info = GetFindAccessibilityNodeInfoResultAndClear(interactionId);
            FinalizeAndCacheAccessibilityNodeInfo(info, connectionId, windowScale);
            *result = info;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    else {
        if (DEBUG) {
            Slogger::W(TAG, "No connection for connection id: %d", connectionId);
        }
    }
    // } catch (RemoteException re) {
    //     if (DEBUG) {
    //         Slogger::W(TAG, "Error while calling remote"
    //                 + " findAccessibilityNodeInfoByViewIdInActiveWindow", re);
    //     }
    // }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::FindAccessibilityNodeInfosByText(
    /* [in] */ Int32 connectionId,
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ const String& text,
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);

    AutoPtr<IObjectContainer> container;
    CObjectContainer::New((IObjectContainer**)&container);
    *result = container;
    REFCOUNT_ADD(*result);

    // try {
    AutoPtr<IAccessibilityServiceConnection> connection;
    GetConnection(connectionId, (IAccessibilityServiceConnection**)&connection);
    if (connection != NULL) {
        Int32 interactionId;
        mInteractionIdCounter->GetAndIncrement(&interactionId);
        Int64 id;
        Thread::GetCurrentThread()->GetId(&id);
        Float windowScale;
        ECode ec = connection->FindAccessibilityNodeInfosByText(
            accessibilityWindowId, accessibilityNodeId, text, interactionId,
            THIS_PROBE(IAccessibilityInteractionConnectionCallback), id, &windowScale);
        if (FAILED(ec)) {
            if (DEBUG) {
                Slogger::W(TAG, "Error while calling remote" \
                    " findAccessibilityNodeInfosByViewText, ec=%08x", ec);
            }
            return ec;
        }

        // If the scale is zero the call has failed.
        if (windowScale > 0) {
            AutoPtr<List<AutoPtr<IAccessibilityNodeInfo> > > infos
                = GetFindAccessibilityNodeInfosResultAndClear(interactionId);
            FinalizeAndCacheAccessibilityNodeInfos(infos, connectionId, windowScale);
            if (infos != NULL && !infos->IsEmpty()) {
                List<AutoPtr<IAccessibilityNodeInfo> >::Iterator it = infos->Begin();
                for (; it != infos->End(); ++it) {
                    container->Add(*it);
                }
            }

            return NOERROR;
        }
    }
    else {
        if (DEBUG) {
            Slogger::W(TAG, "No connection for connection id: %d", connectionId);
        }
    }
    // } catch (RemoteException re) {
    //     if (DEBUG) {
    //         Slogger::W(TAG, "Error while calling remote"
    //                 + " findAccessibilityNodeInfosByViewText", re);
    //     }
    // }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::FindFocus(
    /* [in] */ Int32 connectionId,
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 focusType,
    /* [out] */ IAccessibilityNodeInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    // try {
    AutoPtr<IAccessibilityServiceConnection> connection;
    GetConnection(connectionId, (IAccessibilityServiceConnection**)&connection);
    if (connection != NULL) {
        Int32 interactionId;
        mInteractionIdCounter->GetAndIncrement(&interactionId);
        Int64 id;
        Thread::GetCurrentThread()->GetId(&id);
        Float windowScale;
        ECode ec = connection->FindFocus(accessibilityWindowId,
            accessibilityNodeId, focusType, interactionId,
            THIS_PROBE(IAccessibilityInteractionConnectionCallback), id, &windowScale);
        if (FAILED(ec)) {
            if (DEBUG) {
                Slogger::W(TAG, "Error while calling remote findAccessibilityFocus, ec=%08x", ec);
            }
            return ec;
        }
        // If the scale is zero the call has failed.
        if (windowScale > 0) {
            AutoPtr<IAccessibilityNodeInfo> info = GetFindAccessibilityNodeInfoResultAndClear(interactionId);
            FinalizeAndCacheAccessibilityNodeInfo(info, connectionId, windowScale);
            *result = info;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    else {
        if (DEBUG) {
            Slogger::W(TAG, "No connection for connection id: %d", connectionId);
        }
    }
    // } catch (RemoteException re) {
    //     if (DEBUG) {
    //         Slogger::W(TAG, "Error while calling remote findAccessibilityFocus", re);
    //     }
    // }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::FocusSearch(
    /* [in] */ Int32 connectionId,
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 direction,
    /* [out] */ IAccessibilityNodeInfo** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;

    AutoPtr<IAccessibilityServiceConnection> connection;
    GetConnection(connectionId, (IAccessibilityServiceConnection**)&connection);
    if (connection != NULL) {
        Int32 interactionId;
        mInteractionIdCounter->GetAndIncrement(&interactionId);
        Int64 id;
        Thread::GetCurrentThread()->GetId(&id);
        Float windowScale;
        ECode ec = connection->FindFocus(accessibilityWindowId,
            accessibilityNodeId, direction, interactionId,
            THIS_PROBE(IAccessibilityInteractionConnectionCallback), id, &windowScale);
        if (FAILED(ec)) {
            if (DEBUG) {
                Slogger::W(TAG, "Error while calling remote accessibilityFocusSearch, ec=%08x", ec);
            }
            return ec;
        }
        // If the scale is zero the call has failed.
        if (windowScale > 0) {
            AutoPtr<IAccessibilityNodeInfo> info = GetFindAccessibilityNodeInfoResultAndClear(interactionId);
            FinalizeAndCacheAccessibilityNodeInfo(info, connectionId, windowScale);
            *result = info;
            REFCOUNT_ADD(*result);
            return NOERROR;
        }
    }
    else {
        if (DEBUG) {
            Slogger::W(TAG, "No connection for connection id: %d", connectionId);
        }
    }
    // } catch (RemoteException re) {
    //     if (DEBUG) {
    //         Slogger::W(TAG, "Error while calling remote accessibilityFocusSearch", re);
    //     }
    // }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::PerformAccessibilityAction(
    /* [in] */ Int32 connectionId,
    /* [in] */ Int32 accessibilityWindowId,
    /* [in] */ Int64 accessibilityNodeId,
    /* [in] */ Int32 action,
    /* [in] */ IBundle* arguments,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    AutoPtr<IAccessibilityServiceConnection> connection;
    GetConnection(connectionId, (IAccessibilityServiceConnection**)&connection);
    if (connection != NULL) {
        Int32 interactionId;
        mInteractionIdCounter->GetAndIncrement(&interactionId);
        Int64 id;
        Thread::GetCurrentThread()->GetId(&id);
        Boolean success;
        ECode ec = connection->PerformAccessibilityAction(accessibilityWindowId,
            accessibilityNodeId, action, arguments, interactionId,
            THIS_PROBE(IAccessibilityInteractionConnectionCallback), id, &success);
        if (FAILED(ec)) {
            if (DEBUG) {
                Slogger::W(TAG, "Error while calling remote performAccessibilityAction, ec=%08x", ec);
            }
            return ec;
        }
        if (success) {
            *result = GetPerformAccessibilityActionResultAndClear(interactionId);
            return NOERROR;
        }
    }
    else {
        if (DEBUG) {
            Slogger::W(TAG, "No connection for connection id: %d", connectionId);
        }
    }
    // } catch (RemoteException re) {
    //     if (DEBUG) {
    //         Slogger::W(TAG, "Error while calling remote performAccessibilityAction", re);
    //     }
    // }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::ClearCache()
{
    sAccessibilityNodeInfoCache->Clear();
    return NOERROR;
}

ECode CAccessibilityInteractionClient::OnAccessibilityEvent(
    /* [in] */ IAccessibilityEvent* event)
{
    sAccessibilityNodeInfoCache->OnAccessibilityEvent(event);
    return NOERROR;
}

AutoPtr<IAccessibilityNodeInfo>
CAccessibilityInteractionClient::GetFindAccessibilityNodeInfoResultAndClear(
    /* [in] */ Int32 interactionId)
{
    AutoLock lock(mInstanceLock);
    Boolean success = WaitForResultTimedLocked(interactionId);
    AutoPtr<IAccessibilityNodeInfo> result = success ? mFindAccessibilityNodeInfoResult : NULL;
    ClearResultLocked();
    return result;
}

ECode CAccessibilityInteractionClient::SetFindAccessibilityNodeInfoResult(
    /* [in] */ IAccessibilityNodeInfo* info,
    /* [in] */ Int32 interactionId)
{
    AutoLock lock(mInstanceLock);
    if (interactionId > mInteractionId) {
        mFindAccessibilityNodeInfoResult = info;
        mInteractionId = interactionId;
    }
    return mInstanceLock.NotifyAll();
}

AutoPtr<List<AutoPtr<IAccessibilityNodeInfo> > >
CAccessibilityInteractionClient::GetFindAccessibilityNodeInfosResultAndClear(
    /* [in] */ Int32 interactionId)
{
    AutoLock lock(mInstanceLock);
    Boolean success = WaitForResultTimedLocked(interactionId);
    AutoPtr<List<AutoPtr<IAccessibilityNodeInfo> > > result;
    if (success) {
        result = mFindAccessibilityNodeInfosResult;
    }

    ClearResultLocked();
    if (Build::IS_DEBUGGABLE && CHECK_INTEGRITY) {
        CheckFindAccessibilityNodeInfoResultIntegrity(result);
    }
    return result;
}

ECode CAccessibilityInteractionClient::SetFindAccessibilityNodeInfosResult(
    /* [in] */ IObjectContainer* infos,
    /* [in] */ Int32 interactionId)
{
    AutoLock lock(mInstanceLock);
    if (interactionId > mInteractionId) {
        if (infos != NULL) {
            // If the call is not an IPC, i.e. it is made from the same process, we need to
            // instantiate new result list to avoid passing internal instances to clients.
            Boolean isIpcCall = (Binder::GetCallingPid() != Process::MyPid());
            if (!isIpcCall) {
                mFindAccessibilityNodeInfosResult->Clear();

                AutoPtr<IObjectEnumerator> enumerator;
                infos->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                Boolean hasNext = FALSE;
                while (enumerator->MoveNext(&hasNext), hasNext) {
                    AutoPtr<IInterface> info;
                    enumerator->Current((IInterface**)&info);
                    mFindAccessibilityNodeInfosResult->PushBack(IAccessibilityNodeInfo::Probe(info));
                }
            }
            else {
                if (!mFindAccessibilityNodeInfosResult->IsEmpty()) {
                    mFindAccessibilityNodeInfosResult->Clear();
                    AutoPtr<IObjectEnumerator> enumerator;
                    infos->GetObjectEnumerator((IObjectEnumerator**)&enumerator);
                    Boolean hasNext = FALSE;
                    while (enumerator->MoveNext(&hasNext), hasNext) {
                        AutoPtr<IInterface> info;
                        enumerator->Current((IInterface**)&info);
                        mFindAccessibilityNodeInfosResult->PushBack(IAccessibilityNodeInfo::Probe(info));
                    }
                }

            }
        }
        else {
            mFindAccessibilityNodeInfosResult->Clear();
        }
        mInteractionId = interactionId;
    }
    return mInstanceLock.NotifyAll();
}

Boolean CAccessibilityInteractionClient::GetPerformAccessibilityActionResultAndClear(
    /* [in] */ Int32 interactionId)
{
    AutoLock lock(mInstanceLock);
    Boolean success = WaitForResultTimedLocked(interactionId);
    Boolean result = success ? mPerformAccessibilityActionResult : FALSE;
    ClearResultLocked();
    return result;
}

ECode CAccessibilityInteractionClient::SetPerformAccessibilityActionResult(
    /* [in] */ Boolean succeeded,
    /* [in] */ Int32 interactionId)
{
    AutoLock lock(mInstanceLock);
    if (interactionId > mInteractionId) {
        mPerformAccessibilityActionResult = succeeded;
        mInteractionId = interactionId;
    }
    return mInstanceLock.NotifyAll();
}

void CAccessibilityInteractionClient::ClearResultLocked()
{
    mInteractionId = -1;
    mFindAccessibilityNodeInfoResult = NULL;
    mPerformAccessibilityActionResult = FALSE;

    mFindAccessibilityNodeInfosResult->Clear();
}

Boolean CAccessibilityInteractionClient::WaitForResultTimedLocked(
    /* [in] */ Int32 interactionId)
{
    Int64 waitTimeMillis = TIMEOUT_INTERACTION_MILLIS;
    Int64 startTimeMillis = SystemClock::GetUptimeMillis();
    while (TRUE) {
        // try {
        AutoPtr<IMessage> sameProcessMessage = GetSameProcessMessageAndClear();
        if (sameProcessMessage != NULL) {
            AutoPtr<IHandler> handler;
            sameProcessMessage->GetTarget((IHandler**)&handler);
            handler->HandleMessage(sameProcessMessage);
        }

        if (mInteractionId == interactionId) {
            return TRUE;
        }
        if (mInteractionId > interactionId) {
            return FALSE;
        }
        Int64 elapsedTimeMillis = SystemClock::GetUptimeMillis() - startTimeMillis;
        waitTimeMillis = TIMEOUT_INTERACTION_MILLIS - elapsedTimeMillis;
        if (waitTimeMillis <= 0) {
            return FALSE;
        }
        mInstanceLock.Wait(waitTimeMillis);
        // } catch (InterruptedException ie) {
        //     /* ignore */
        // }
    }
}

void CAccessibilityInteractionClient::ApplyCompatibilityScaleIfNeeded(
    /* [in] */ IAccessibilityNodeInfo* info,
    /* [in] */ Float scale)
{
    if (scale == 1.0f) {
        return;
    }
    AutoPtr<IRect> bounds = mTempBounds;
    info->GetBoundsInParent(bounds);
    bounds->Scale(scale);
    info->SetBoundsInParent(bounds);

    info->GetBoundsInScreen(bounds);
    bounds->Scale(scale);
    info->SetBoundsInScreen(bounds);
}

void CAccessibilityInteractionClient::FinalizeAndCacheAccessibilityNodeInfo(
    /* [in] */ IAccessibilityNodeInfo* info,
    /* [in] */ Int32 connectionId,
    /* [in] */ Float windowScale)
{
    if (info != NULL) {
        ApplyCompatibilityScaleIfNeeded(info, windowScale);
        info->SetConnectionId(connectionId);
        info->SetSealed(TRUE);
        sAccessibilityNodeInfoCache->Add(info);
    }
}

void CAccessibilityInteractionClient::FinalizeAndCacheAccessibilityNodeInfos(
    /* [in] */ List<AutoPtr<IAccessibilityNodeInfo> >* infos,
    /* [in] */ Int32 connectionId,
    /* [in] */ Float windowScale)
{
    if (infos != NULL) {
        List<AutoPtr<IAccessibilityNodeInfo> >::Iterator it = infos->Begin();
        for (; it != infos->End(); ++it) {
            AutoPtr<IAccessibilityNodeInfo> info = *it;
            FinalizeAndCacheAccessibilityNodeInfo(info, connectionId, windowScale);
        }
    }
}

AutoPtr<IMessage> CAccessibilityInteractionClient::GetSameProcessMessageAndClear()
{
    AutoLock lock(mInstanceLock);
    AutoPtr<IMessage> result = mSameThreadMessage;
    mSameThreadMessage = NULL;
    return result;
}

ECode CAccessibilityInteractionClient::GetConnection(
    /* [in] */ Int32 connectionId,
    /* [out] */ IAccessibilityServiceConnection** connection)
{
    VALIDATE_NOT_NULL(connection);
    *connection = NULL;

    AutoLock lock(sConnectionCacheLock);
    HashMap<Int32, AutoPtr<IAccessibilityServiceConnection> >::Iterator it
        = sConnectionCache.Find(connectionId);
    if (it != sConnectionCache.End()) {
        *connection = it->mSecond.Get();
        REFCOUNT_ADD(*connection);
    }
    return NOERROR;
}

ECode CAccessibilityInteractionClient::AddConnection(
    /* [in] */ Int32 connectionId,
    /* [in] */ IAccessibilityServiceConnection* connection)
{
    AutoLock lock(sConnectionCacheLock);
    sConnectionCache[connectionId] = connection;
    return NOERROR;
}

ECode CAccessibilityInteractionClient::RemoveConnection(
    /* [in] */ Int32 connectionId)
{
    AutoLock lock(sConnectionCacheLock);
    sConnectionCache.Erase(connectionId);
    return NOERROR;
}

void CAccessibilityInteractionClient::CheckFindAccessibilityNodeInfoResultIntegrity(
    /* [in] */ List<AutoPtr<IAccessibilityNodeInfo> >* infos)
{
    if (infos->Begin() == infos->End()) {
        return;
    }
    // Find the root node.
    AutoPtr<IAccessibilityNodeInfo> root = *infos->Begin();
    List<AutoPtr<IAccessibilityNodeInfo> >::Iterator it = ++infos->Begin();
    for (; it != infos->End(); ++it) {
        List<AutoPtr<IAccessibilityNodeInfo> >::Iterator it1 = it;
        for (; it1 != infos->End(); ++it1) {
            AutoPtr<IAccessibilityNodeInfo> candidate = *it1;
            Int64 id1, id2;
            root->GetParentNodeId(&id1);
            candidate->GetSourceNodeId(&id2);
            if (id1 == id2) {
                root = candidate;
                break;
            }
        }
    }
    if (root == NULL) {
        Slogger::E(TAG, "No root.");
    }
    // Check for duplicates.
    HashSet<AutoPtr<IAccessibilityNodeInfo> > seen;
    List<AutoPtr<IAccessibilityNodeInfo> > fringe;
    fringe.PushBack(root);

    List<AutoPtr<IAccessibilityNodeInfo> >::Iterator fringeIt;
    Int32 childCount;
    Int64 childId, nodeId;
    while (fringe.IsEmpty() == FALSE) {
        fringeIt = fringe.Begin();
        AutoPtr<IAccessibilityNodeInfo> current = *fringeIt;
        fringe.PopFront();
        Elastos::Utility::Pair<HashSet<AutoPtr<IAccessibilityNodeInfo> >::Iterator, Boolean> pair = seen.Insert(current);
        if (!pair.mSecond) {
            Slogger::E(TAG, "Duplicate node.");
            return;
        }

        AutoPtr<IObjectInt32Map> childIds;
        current->GetChildNodeIds((IObjectInt32Map**)&childIds);
        childIds->GetSize(&childCount);
        for (Int32 i = 0; i < childCount; i++) {
            AutoPtr<IInteger64> childIdInter;
            childIds->Get(i, (IInterface**)&childIdInter);
            childIdInter->GetValue(&childId);
            for (it = infos->Begin(); it != infos->End(); ++it) {
                AutoPtr<IAccessibilityNodeInfo> child = *it;
                child->GetSourceNodeId(&nodeId);
                if (nodeId == childId) {
                    fringe.PushBack(child);
                }
            }
        }
    }
    Int32 disconnectedCount = infos->GetSize() - seen.GetSize();
    if (disconnectedCount > 0) {
        Slogger::E(TAG, "%d Disconnected nodes.", disconnectedCount);
    }
}


} // Accessibility
} // View
} // Droid
} // Elastos
