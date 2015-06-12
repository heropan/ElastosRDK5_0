
#include "CObservable.h"
#include "CArrayList.h"

namespace Elastos {
namespace Utility {

CAR_INTERFACE_IMPL(CObservable, Object, IObservable)

CAR_OBJECT_IMPL(CObservable)

ECode CObservable::constructor()
{
    AutoPtr<IArrayList> outlist;
    FAIL_RETURN(CArrayList::New((IArrayList**)&outlist));
    mObservers = IList::Probe(outlist);
    mChanged = FALSE;
    return NOERROR;
}

ECode CObservable::AddObserver(
    /* [in] */ IObserver* observer)
{
    if (observer == NULL) {
        // throw new NullPointerException("observer == null");
        return E_NULL_POINTER_EXCEPTION;
    }
    {
        Object::Autolock lock(this);
        Boolean isflag = FALSE;
        if (!((ICollection::Probe(mObservers))->Contains(observer, &isflag), isflag))
            mObservers->Add(observer, &isflag);
    }
    return NOERROR;
}

ECode CObservable::CountObservers(
    /* [out] */ Int32* value)
{
    return (ICollection::Probe(mObservers))->GetSize(value);
}

ECode CObservable::DeleteObserver(
    /* [in] */ IObserver* observer)
{
    Object::Autolock lock(this);
    return mObservers->Remove(observer);
}

ECode CObservable::DeleteObservers()
{
    Object::Autolock lock(this);
    return (ICollection::Probe(mObservers))->Clear();
}

ECode CObservable::HasChanged(
    /* [out] */ Boolean* value)
{
    VALIDATE_NOT_NULL(value)

    *value = mChanged;
    return NOERROR;
}

ECode CObservable::NotifyObservers()
{
    return NotifyObservers(NULL);
}

ECode CObservable::NotifyObservers(
    /* [in] */ IInterface* data)
{
    Int32 size = 0;
    AutoPtr< ArrayOf<IInterface*> > arrays;
    {
        Object::Autolock lock(this);
        Boolean isflag = FALSE;
        if (HasChanged(&isflag), isflag) {
            ClearChanged();
            (ICollection::Probe(mObservers))->GetSize(&size);
            (ICollection::Probe(mObservers))->ToArray((ArrayOf<IInterface*>**)&arrays);
        }
    }
    if (arrays != NULL) {
        for (Int32 i = 0; i < arrays->GetLength(); i++) {
            AutoPtr<IObserver> observer = IObserver::Probe((*arrays)[i]);
            observer->Update(THIS_PROBE(IObservable), data);
        }
    }
    return NOERROR;
}

ECode CObservable::ClearChanged()
{
    mChanged = FALSE;
    return NOERROR;
}

ECode CObservable::SetChanged()
{
    mChanged = TRUE;
    return NOERROR;
}

} // namespace Utility
} // namespace Elastos
