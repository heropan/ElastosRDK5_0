
#include "AbstractCollection.h"
#include <elastos/core/StringBuilder.h>

using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Utility {

// {8cabe5bf-91a0-4b31-80c2-b348d29620fd}
extern const _ELASTOS ClassID ECLSID_AbstractCollection =
        { 0x8cabe5bf, 0x91a0, 0x4b31, { 0x80, 0xc2, 0xb3, 0x48, 0xd2, 0x96, 0x20, 0xfd } };

CAR_INTERFACE_IMPL_2(AbstractCollection, Object, ICollection, IIterable)

AbstractCollection::AbstractCollection()
{
}

AbstractCollection::~AbstractCollection()
{
}

ECode AbstractCollection::Add(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* modified)
{
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode AbstractCollection::Add(
    /* [in] */ IInterface* object)
{
    Boolean result;
    return Add(object, &result);
}

ECode AbstractCollection::AddAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* modified)
{
    VALIDATE_NOT_NULL(modified)
    *modified = FALSE;
    VALIDATE_NOT_NULL(collection)

    IIterable* iterable = IIterable::Probe(collection);
    VALIDATE_NOT_NULL(iterable)

    Boolean result = FALSE;
    AutoPtr<IIterator> it;
    iterable->GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        Boolean flag = FALSE;
        FAIL_RETURN(Add(nextobject, &flag));
        if (flag) {
            result = TRUE;
        }
    }
    *modified = result;
    return NOERROR;
}

ECode AbstractCollection::AddAll(
    /* [in] */ ICollection* collection)
{
    Boolean result;
    return AddAll(collection, &result);
}

ECode AbstractCollection::Clear()
{
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        it->Remove();
    }
    return NOERROR;
}

ECode AbstractCollection::Contains(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    if (object != NULL) {
        while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            if (Object::Equals(object, nextobject)) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    else {
        while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            if (nextobject == NULL) {
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode AbstractCollection::ContainsAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(collection);

    IIterable* iterable = IIterable::Probe(collection);
    VALIDATE_NOT_NULL(iterable)

    AutoPtr<IIterator> it;
    iterable->GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    Boolean flag = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            Contains(nextobject, &flag);
            if (!flag) {
                *result = FALSE;
                return NOERROR;
            }
    }
    *result = TRUE;
    return NOERROR;
}

ECode AbstractCollection::IsEmpty(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    Int32 size;
    GetSize(&size);
    *result = size == 0;
    return  NOERROR;
}

ECode AbstractCollection::Remove(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    if (object != NULL) {
        while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            if (Object::Equals(object, nextobject)) {
                 it->Remove();
                 *result = TRUE;
                 return NOERROR;
            }
        }
    }
    else {
        while ((it->HasNext(&hasnext), hasnext)) {
            AutoPtr<IInterface> nextobject;
            it->Next((IInterface**)&nextobject);
            if (nextobject == NULL) {
                it->Remove();
                *result = TRUE;
                return NOERROR;
            }
        }
    }
    *result = FALSE;
    return NOERROR;
}

ECode AbstractCollection::Remove(
    /* [in] */ IInterface* object)
{
    Boolean result;
    return Remove(object, &result);
}

ECode AbstractCollection::RemoveAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(collection);
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        Boolean flag = FALSE;
        collection->Contains(nextobject, &flag);
        if (flag) {
            it->Remove();
            *result = TRUE;
        }
    }
    return NOERROR;
}

ECode AbstractCollection::RemoveAll(
    /* [in] */ ICollection* collection)
{
    Boolean result;
    return RemoveAll(collection, &result);
}

ECode  AbstractCollection::RetainAll(
    /* [in] */ ICollection* collection,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(collection);
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        Boolean flag = FALSE;
        collection->Contains(nextobject, &flag);
        if (!flag) {
            it->Remove();
            *result = TRUE;
        }
    }
    return NOERROR;
}

ECode AbstractCollection::ToArray(
    /* [out, callee] */ ArrayOf<IInterface*>** array)
{
    VALIDATE_NOT_NULL(array);
    Int32 size;
    GetSize(&size);
    Int32 index = 0;
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    AutoPtr<ArrayOf<IInterface*> > sparray = ArrayOf<IInterface*>::Alloc(size);
    while (index < size) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        sparray->Set(index++, nextobject);
    }
    *array = sparray;
    REFCOUNT_ADD(*array)
    return NOERROR;
}

ECode AbstractCollection::ToArray(
    /* [in] */ ArrayOf<IInterface*>* contents,
    /* [out, callee] */ ArrayOf<IInterface*>** outArray)
{
    VALIDATE_NOT_NULL(contents);
    VALIDATE_NOT_NULL(outArray);
    Int32 size;
    GetSize(&size);
    AutoPtr<ArrayOf<IInterface*> > array = size > contents->GetLength() ?
            ArrayOf<IInterface*>::Alloc(size) : contents;
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Int32 index = 0;
    while (index < size) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        array->Set(index++, nextobject);
    }
    if (index < contents->GetLength()) {
        array->Set(index, NULL);
    }
    *outArray = array;
    REFCOUNT_ADD(*outArray)
    return NOERROR;
}

ECode AbstractCollection::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    Boolean flag = FALSE;
    if (IsEmpty(&flag), flag) {
        *result = "[]";
        return NOERROR;
    }

    Int32 size;
    GetSize(&size);
    StringBuilder buffer(size * 16);
    buffer.Append('[');
    AutoPtr<IIterator> it;
    GetIterator((IIterator**)&it);
    Boolean hasnext = FALSE;
    while ((it->HasNext(&hasnext), hasnext)) {
        AutoPtr<IInterface> nextobject;
        it->Next((IInterface**)&nextobject);
        if (nextobject.Get() != this->Probe(EIID_IInterface)) {
            buffer.Append(nextobject);
        }
        else {
            buffer += "(this Collection)";
        }
        if ((it->HasNext(&hasnext), hasnext)) {
            buffer += ", ";
        }
    }
    buffer.Append(']');
    *result = buffer.ToString();
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
