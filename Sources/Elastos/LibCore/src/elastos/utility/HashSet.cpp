
#include "CHashSet.h"
#include "CHashMap.h"

namespace Elastos {
namespace Utility {

ECode HashSet::Init()
{
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New((IMap**)&hmap));
    return this->Init(hmap);
}

ECode HashSet::Init(
    /* [in] */ Int32 capacity)
{
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New(capacity, (IMap**)&hmap));
    return this->Init(hmap);
}

ECode HashSet::Init(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New(capacity, loadFactor, (IMap**)&hmap));
    return this->Init(hmap);
}

ECode HashSet::Init(
    /* [in] */ ICollection* collection)
{
    Int32 clolen = 0;
    collection->GetSize(&clolen);
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New(clolen < 6 ? 11 : clolen * 2, (IMap**)&hmap));
    this->Init(hmap);
    AutoPtr< ArrayOf<IInterface*> > outarr;
    collection->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        AutoPtr<IInterface> e = (*outarr)[i];
        Boolean isflag = FALSE;
        Add(e, &isflag);
    }
    return NOERROR;
}

ECode HashSet::Init(
    /* [in] */ IMap* backingMap)
{
    mBackingMap = backingMap;
    return NOERROR;
}

ECode HashSet::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    AutoPtr<IInterface> outface;
    mBackingMap->Put(object, this->Probe(EIID_IInterface), (IInterface**)&outface);
    *modified = outface == NULL;
    return NOERROR;
}

ECode HashSet::Clear()
{
    return mBackingMap->Clear();
}

ECode HashSet::Clone(
    /* [out] */ IInterface** outface)
{
    VALIDATE_NOT_NULL(outface)

    // try {
    // HashSet<E> clone = (HashSet<E>) super.clone();
    // clone.backingMap = (HashMap<E, HashSet<E>>) backingMap.clone();
    AutoPtr<CHashSet> clone;
    FAIL_RETURN(CHashSet::NewByFriend(mBackingMap, (CHashSet**)&clone));
    *outface = ISet::Probe(clone);
    REFCOUNT_ADD(*outface)
    // } catch (CloneNotSupportedException e) {
    //     throw new AssertionError(e);
    // }
    return NOERROR;
}

ECode HashSet::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return mBackingMap->ContainsKey(object, result);
}

ECode HashSet::IsEmpty(
    /* [out] */ Boolean* result)
{
    return mBackingMap->IsEmpty(result);
}

ECode HashSet::GetIterator(
    /* [out] */ IIterator** it)
{
    VALIDATE_NOT_NULL(it)

    AutoPtr<ISet> outset;
    mBackingMap->KeySet((ISet**)&outset);
    return outset->GetIterator(it);
}

ECode HashSet::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)

    AutoPtr<IInterface> outface;
    mBackingMap->Remove(object, (IInterface**)&outface);
    *modified = outface != NULL;
    return NOERROR;
}

ECode HashSet::GetSize(
    /* [out] */ Int32* size)
{
    return mBackingMap->GetSize(size);
}

ECode HashSet::WriteObject(
    /* [in] */ IObjectOutputStream* stream)
{
    stream->DefaultWriteObject();
    stream->Write(((CHashMap*)mBackingMap.Get())->mTable->GetLength());
    assert(0 && "TODO");
    // stream->WriteFloat(CHashMap::DEFAULT_LOAD_FACTOR);
    Int32 sizelen = 0;
    GetSize(&sizelen);
    stream->Write(sizelen);
    // for (E e : this) {
    //     stream.writeObject(e);
    // }
    return E_NOT_IMPLEMENTED;
}

ECode HashSet::ReadObject(
    /* [in] */ IObjectInputStream* stream)
{
    stream->DefaultReadObject();
    Int32 length = 0;
    stream->Read(&length);
    Float loadFactor = 0;
    assert(0 && "TODO");
    // stream->ReadFloat(&loadFactor);
    mBackingMap = CreateBackingMap(length, loadFactor);
    Int32 elementCount = 0;
    stream->Read(&elementCount);
    for (Int32 i = elementCount; --i >= 0;) {
        AutoPtr<IInterface> key;
        // stream->ReadObject((IInterface**)&key);
        AutoPtr<IInterface> outface;
        mBackingMap->Put(key, this->Probe(EIID_IInterface), (IInterface**)&outface);
    }
    return E_NOT_IMPLEMENTED;
}

AutoPtr<IMap> HashSet::CreateBackingMap(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    AutoPtr<IMap> outmap;

    CHashMap::New(capacity, loadFactor, (IMap**)&outmap);
    return outmap;
}

} // namespace Utility
} // namespace Elastos
