
#include "elastos/droid/webkit/ByteArrayBuilder.h"
#include <elastos/core/Math.h>

using Elastos::Utility::CLinkedList;
using Elastos::Utility::IListIterator;

namespace Elastos {
namespace Droid {
namespace Webkit {

//===============================================================
//                   ByteArrayBuilder::Chunk
//===============================================================
ByteArrayBuilder::Chunk::Chunk(
    /* [in] */ Int32 length)
    : mLength(0)
{
    mArray = ArrayOf<Byte>::Alloc(length);
}

PInterface ByteArrayBuilder::Chunk::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IWeakReferenceSource) {
        return (IWeakReferenceSource*)this;
    }
    return NULL;
}

UInt32 ByteArrayBuilder::Chunk::AddRef()
{
    return ElRefBase::AddRef();
}

UInt32 ByteArrayBuilder::Chunk::Release()
{
    return ElRefBase::Release();
}

ECode ByteArrayBuilder::Chunk::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode ByteArrayBuilder::Chunk::GetWeakReference(
    /* [out] */ IWeakReference** weakReference)
{
    VALIDATE_NOT_NULL(weakReference)
    *weakReference = new WeakReferenceImpl(Probe(EIID_IInterface), CreateWeak(this));
    REFCOUNT_ADD(*weakReference)
    return NOERROR;
}

/**
 * Release the chunk and make it available for reuse.
 */
void ByteArrayBuilder::Chunk::ReleaseResources()
{
    mLength = 0;
    {
        AutoLock lock(sPoolObject);
        // Add the chunk back to the pool as a SoftReference so it can
        // be gc'd if needed.
        AutoPtr<IWeakReference> weakChunk;
        GetWeakReference((IWeakReference**)&weakChunk);
        Boolean result = FALSE;
        sPool->Offer(weakChunk, &result);
        sPoolObject.NotifyAll();
    }
}

//===============================================================
//                          ByteArrayBuilder
//===============================================================

static AutoPtr<ILinkedList> sPoolCreate()
{
    AutoPtr<ILinkedList> pool;
    CLinkedList::New((ILinkedList**)&pool);
    return pool;
}

const Int32 ByteArrayBuilder::DEFAULT_CAPACITY;

// Global pool of chunks to be used by other ByteArrayBuilders.
const AutoPtr<ILinkedList> ByteArrayBuilder::sPool = sPoolCreate();
Object ByteArrayBuilder::sPoolObject;

ByteArrayBuilder::ByteArrayBuilder()
{
    CLinkedList::New((ILinkedList**)&mChunks);
}

//synchronized
void ByteArrayBuilder::Append(
    /* [in] */ ArrayOf<Byte>* array,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    AutoLock lock(mLock);

    while (length > 0) {
        AutoPtr<Chunk> c;
        Boolean bFlag = FALSE;
        mChunks->IsEmpty(&bFlag);
        if (bFlag) {
            c = ObtainChunk(length);
            mChunks->AddLast(c);
        }
        else {
            AutoPtr<IInterface> last;
            mChunks->GetLast((IInterface**)&last);
            c = (Chunk*)last.Get();
            if (c->mLength == c->mArray->GetLength()) {
                c = ObtainChunk(length);
                mChunks->AddLast(c);
            }
        }
        Int32 amount = Elastos::Core::Math::Min(length, c->mArray->GetLength() - c->mLength);
        for (Int32 i = 0; i < amount; i++) {
            array->Set(offset+i, (*(c->mArray))[c->mLength+i]);
        }
        c->mLength += amount;
        length -= amount;
        offset += amount;
    }
}

/**
 * The fastest way to retrieve the data is to iterate through the
 * chunks.  This returns the first chunk.  Note: this pulls the
 * chunk out of the queue.  The caller must call Chunk.release() to
 * dispose of it.
 */
//synchronized
AutoPtr<ByteArrayBuilder::Chunk> ByteArrayBuilder::GetFirstChunk()
{
    AutoLock lock(mLock);

    Boolean bFlag = FALSE;
    mChunks->IsEmpty(&bFlag);
    if (bFlag) return NULL;
    AutoPtr<IInterface> chunk;
    mChunks->RemoveFirst((IInterface**)&chunk);
    return (ByteArrayBuilder::Chunk*)chunk.Get();
}

//synchronized
Boolean ByteArrayBuilder::IsEmpty()
{
    AutoLock lock(mLock);

    Boolean bFlag = FALSE;
    mChunks->IsEmpty(&bFlag);
    return bFlag;
}

//synchronized
Int32 ByteArrayBuilder::GetByteSize()
{
    AutoLock lock(mLock);

    Int32 total = 0;
    AutoPtr<IListIterator> it;
    mChunks->GetListIterator(0, (IListIterator**)&it);
    Boolean bFlag = FALSE;
    it->HasNext(&bFlag);
    while (bFlag) {
        AutoPtr<IInterface> _c;
        it->Next((IInterface**)&_c);
        AutoPtr<Chunk> c = (Chunk*)_c.Get();
        total += c->mLength;
    }
    return total;
}

//synchronized
void ByteArrayBuilder::Clear()
{
    AutoLock lock(mLock);

    AutoPtr<Chunk> c = GetFirstChunk();
    while (c != NULL) {
        c->Release();
        c = GetFirstChunk();
    }
}

// Must be called with lock held on sPool.
void ByteArrayBuilder::ProcessPoolLocked()
{
    assert(0);
    // while (true) {
    //     SoftReference<Chunk> entry = (SoftReference<Chunk>) sQueue.poll();
    //     if (entry == null) {
    //         break;
    //     }
    //     sPool.remove(entry);
    // }
}

AutoPtr<ByteArrayBuilder::Chunk> ByteArrayBuilder::ObtainChunk(
    /* [in] */ Int32 length)
{
    // Correct a small length.
    if (length < DEFAULT_CAPACITY) {
        length = DEFAULT_CAPACITY;
    }

    {
        AutoLock lock(sPoolObject);
        // Process any queued references and remove them from the pool.
        ProcessPoolLocked();
        Boolean bFlag = FALSE;
        sPool->IsEmpty(&bFlag);
        if (!bFlag) {
            AutoPtr<IInterface> _c;
            sPool->RemoveFirst((IInterface**)&_c);
            AutoPtr<IWeakReference> weakChunk = IWeakReference::Probe(_c);
            AutoPtr<IInterface> chunk;
            weakChunk->Resolve(EIID_IInterface, (IInterface**)&chunk);
            AutoPtr<Chunk> c = (Chunk*)chunk.Get();
            // The first item may have been queued after processPoolLocked
            // so check for null.
            if (c != NULL) {
                return c;
            }
        }
        return new Chunk(length);
    }
}

} // namespace Webkit
} // namepsace Droid
} // namespace Elastos