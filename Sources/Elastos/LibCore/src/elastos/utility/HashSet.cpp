
#include "CHashSet.h"
#include "CHashMap.h"

using Elastos::Core::EIID_ICloneable;
using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::EIID_ISerializable;

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL_3(HashSet, AbstractSet, IHashSet, ICloneable, ISerializable)

ECode HashSet::constructor()
{
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New((IMap**)&hmap));
    return this->constructor(hmap);
}

ECode HashSet::constructor(
    /* [in] */ Int32 capacity)
{
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New(capacity, (IMap**)&hmap));
    return this->constructor(hmap);
}

ECode HashSet::constructor(
    /* [in] */ Int32 capacity,
    /* [in] */ Float loadFactor)
{
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New(capacity, loadFactor, (IMap**)&hmap));
    return this->constructor(hmap);
}

ECode HashSet::constructor(
    /* [in] */ ICollection* collection)
{
    Int32 clolen = 0;
    collection->GetSize(&clolen);
    AutoPtr<IMap> hmap;
    FAIL_RETURN(CHashMap::New(clolen < 6 ? 11 : clolen * 2, (IMap**)&hmap));
    this->constructor(hmap);
    AutoPtr< ArrayOf<IInterface*> > outarr;
    collection->ToArray((ArrayOf<IInterface*>**)&outarr);
    for (Int32 i = 0; i < outarr->GetLength(); i++) {
        Add((*outarr)[i]);
    }
    return NOERROR;
}

ECode HashSet::constructor(
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
    mBackingMap->GetKeySet((ISet**)&outset);
    return (IIterable::Probe(outset))->GetIterator(it);
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
    (IOutputStream::Probe(stream))->Write(((CHashMap*)mBackingMap.Get())->mTable->GetLength());
    assert(0 && "TODO");
    // stream->WriteFloat(CHashMap::DEFAULT_LOAD_FACTOR);
    Int32 sizelen = 0;
    GetSize(&sizelen);
    (IOutputStream::Probe(stream))->Write(sizelen);
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
    (IInputStream::Probe(stream))->Read(&length);
    Float loadFactor = 0;
    assert(0 && "TODO");
    // stream->ReadFloat(&loadFactor);
    mBackingMap = CreateBackingMap(length, loadFactor);
    Int32 elementCount = 0;
    (IInputStream::Probe(stream))->Read(&elementCount);
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
