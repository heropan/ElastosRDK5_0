
#include "RouteSpecificPool.h"
#include <elastos/Logger.h>

using Elastos::Utility::CLinkedList;
using Elastos::Utility::IListIterator;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Conn::IOperatedClientConnection;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Conn {
namespace Tsccm {

RouteSpecificPool::RouteSpecificPool(
    /* [in] */ IHttpRoute* route,
    /* [in] */ Int32 maxEntries)
    : mRoute(route)
    , mMaxEntries(maxEntries)
    , mNumEntries(0)
{
    CLinkedList::New((ILinkedList**)&mfreeEntries);
    AutoPtr<ILinkedList> list;
    CLinkedList::New((ILinkedList**)&list);
    mWaitingThreads = IQueue::Probe(list);
}

AutoPtr<IHttpRoute> RouteSpecificPool::GetRoute()
{
    return mRoute;
}

Int32 RouteSpecificPool::GetMaxEntries()
{
    return mMaxEntries;
}

Boolean RouteSpecificPool::IsUnused()
{
    Boolean isEmpty;
    return (numEntries < 1) && (mWaitingThreads->IsEmpty(&isEmpty), isEmpty);
}

Int32 RouteSpecificPool::GetCapacity()
{
    return mMaxEntries - mNumEntries;
}

Int32 RouteSpecificPool::GetEntryCount()
{
    return mNumEntries;
}

AutoPtr<BasicPoolEntry> RouteSpecificPool::AllocEntry(
    /* [in] */ IObject* state)
{
    Boolean isEmpty;
    if (mfreeEntries->IsEmpty(&isEmpty), !isEmpty) {
        Int32 size;
        mfreeEntries->GetSize(&size);
        AutoPtr<IListIterator> it;
        mfreeEntries->GetListIterator(size, (IListIterator**)&it);
        Boolean hasPrevious;
        while(it->HasPrevious(&hasPrevious), hasPrevious) {
            AutoPtr<IInterface> value;
            it->GetPrevious((IInterface**)&value);
            AutoPtr<BasicPoolEntry> entry = (BasicPoolEntry*)value.Get();
            if (state != NULL) {
                AutoPtr<IObject> entryState = entry->GetState();
                Boolean equals;
                if (state->Equals(entryState, &equals), equals) {
                    it->Remove();
                    return entry;
                }
            }
        }
    }
    if (mfreeEntries->IsEmpty(&isEmpty), !isEmpty) {
        AutoPtr<IInterface> value;
        mfreeEntries->Remove((IInterface**)&value);
        AutoPtr<BasicPoolEntry> entry = (BasicPoolEntry*)value.Get();
        entry->SetState(NULL);
        AutoPtr<IOperatedClientConnection> conn = entry->GetConnection();
        // try {
        conn->Close();
        // } catch (IOException ex) {
        //     log.debug("I/O error closing connection", ex);
        // }
        return entry;
    }
    return NULL;
}

ECode RouteSpecificPool::FreeEntry(
    /* [in] */ BasicPoolEntry* entry)
{
    if (mNumEntries < 1) {
        Logger::E("RouteSpecificPool", "No entry created for this pool. %p", mRoute.Get());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    Int32 size;
    if (mfreeEntries->GetSize(&size), mNumEntries <= size) {
        Logger::E("RouteSpecificPool", "No entry allocated for this pool. %p", mRoute.Get());
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mfreeEntries->Add((IInterface*)entry);
    return NOERROR;
}

ECode RouteSpecificPool::CreatedEntry(
    /* [in] */ BasicPoolEntry* entry)
{
    AutoPtr<IHttpRoute> plannedRoute = entry->GetPlannedRoute();
    Boolean equals;
    if (IObject::Probe(mRoute)->Equals(plannedRoute, &equals), !equals) {
        Logger::Probe("RouteSpecificPool", "Entry not planned for this pool.\npool: %p\nplan: %p", mRoute.Get(), plannedRoute.Get());
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mNumEntries++;
    return NOERROR;
}

Boolean RouteSpecificPool::DeleteEntry(
    /* [in] */ BasicPoolEntry* entry)
{
    Boolean found;
    mfreeEntries->Remove((IInterface*)entry, &found);
    if (found)
        mNumEntries--;
    return found;
}

ECode RouteSpecificPool::DropEntry()
{
    if (mNumEntries < 1) {
        Logger::E("RouteSpecificPool", "There is no entry that could be dropped.");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mNumEntries--;
    return NOERROR;
}

ECode RouteSpecificPool::QueueThread(
    /* [in] */ WaitingThread* wt)
{
    if (wt == NULL) {
        Logger::E("RouteSpecificPool", "Waiting thread must not be null.");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    mWaitingThreads->Add(wt);
    return NOERROR;
}

Boolean RouteSpecificPool::HasThread()
{
    Boolean isEmpty;
    mWaitingThreads->IsEmpty(&isEmpty);
    return !isEmpty;
}

AutoPtr<WaitingThread> RouteSpecificPool::NextThread()
{
    AutoPtr<IInterface> value;
    mWaitingThreads->Peek((IInterface**)&value);
    return (WaitingThread*)value.Get();
}

void RouteSpecificPool::RemoveThread(
    /* [in] */ WaitingThread* wt)
{
    if (wt == NULL) return;

    mWaitingThreads->Remove((IInterface*)wt);
}


} // namespace Tsccm
} // namespace Conn
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org