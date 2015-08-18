#include "os/CBundle.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuilder.h>
#include <elastos/core/CoreUtils.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::CoreUtils;
using Elastos::Core::EIID_ICloneable;
using Elastos::Utility::Logging::Logger;

namespace Elastos {
namespace Droid {
namespace Os {

static AutoPtr<IBundle> InitEMPTY()
{
    AutoPtr<CBundle> empty;
    CBundle::NewByFriend((CBundle**)&empty);
    //TODO empty->mMap = ArrayMap::EMPTY;
    return empty;
}

AutoPtr<IBundle> CBundle::EMPTY = InitEMPTY();

CAR_INTERFACE_IMPL_3(CBundle, BaseBundle, IBundle, ICloneable, IParcelable)

CAR_OBJECT_IMPL(CBundle)

CBundle::CBundle()
    : mHasFds(FALSE)
    , mFdsKnown(TRUE)
    , mAllowFds(TRUE)
{
}

CBundle::~CBundle()
{
    // TODO: for debug
    mParcelledData = NULL;
    mClassLoader = NULL;

    mMap = NULL;
}

ECode CBundle::constructor()
{
    return BaseBundle::constructor();
}

ECode CBundle::constructor(
    /* [in] */ IClassLoader* loader)
{
    return BaseBundle::constructor(loader);
}

ECode CBundle::constructor(
    /* [in] */ Int32 capacity)
{
    return BaseBundle::constructor(capacity);
}

ECode CBundle::constructor(
    /* [in] */ IBundle* bundle)
{
    FAIL_RETURN(BaseBundle::constructor(IBaseBundle::Probe(bundle)))

    CBundle* b = (CBundle*)bundle;

    mHasFds = b->mHasFds;
    mFdsKnown = b->mFdsKnown;
    return NOERROR;
}

ECode CBundle::constructor(
    /* [in] */ IPersistableBundle* bundle)
{
    return BaseBundle::constructor(IBaseBundle::Probe(bundle));
}

AutoPtr<IBundle> CBundle::ForPair(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    AutoPtr<CBundle> b;
    CBundle::NewByFriend(1, (CBundle**)&b);
    b->PutString(key, value);
    AutoPtr<IBundle> ib = (IBundle*)b.Get();
    return ib;
}

ECode CBundle::SetAllowFds(
    /* [in] */ Boolean allowFds)
{
    Boolean prev;
    return SetAllowFds(allowFds, &prev);
}

ECode CBundle::SetAllowFds(
    /* [in] */ Boolean allowFds,
    /* [out] */ Boolean* prev)
{
    VALIDATE_NOT_NULL(prev);
    *prev = mAllowFds;
    mAllowFds = allowFds;
    return NOERROR;
}

ECode CBundle::Clone(
    /* [out] */ IInterface** obj)
{
    VALIDATE_NOT_NULL(obj)
    AutoPtr<IBundle> bundle;
    CBundle::New(THIS_PROBE(IBundle), (IBundle**)&bundle);
    *obj = bundle;
    REFCOUNT_ADD(*obj)
    return NOERROR;
}

ECode CBundle::Clear()
{
    FAIL_RETURN(BaseBundle::Clear())

    mHasFds = FALSE;
    mFdsKnown = TRUE;
    return NOERROR;
}

ECode CBundle::PutAll(
    /* [in] */ IBundle* b)
{
    Unparcel();

    CBundle* bundle = (CBundle*)b;
    bundle->Unparcel();
    IMap::Probe(mMap)->PutAll(IMap::Probe(bundle->mMap));

    // fd state is now known if and only if both bundles already knew
    mHasFds |= bundle->mHasFds;
    mFdsKnown = mFdsKnown && bundle->mFdsKnown;
    return NOERROR;
}

ECode CBundle::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result)

    if (!mFdsKnown) {
        Boolean fdFound = FALSE;    // keep going until we find one or run out of data

        if (mParcelledData != NULL) {
            Boolean bval;
            mParcelledData->HasFileDescriptors(&bval);
            if (bval) {
                fdFound = TRUE;
            }
        }
        else {
            // It's been unparcelled, so we need to walk the map

        //     AutoPtr<ISet> outset;
        //     IMap::Probe(map)->GetEntrySet((ISet**)&outset);
        //     AutoPtr<IIterator> it;
        //     outset->GetIterator((IIterator**)&it);
        //     Boolean hasNext = FALSE;
        //     String key;
        //     while ((it->HasNext(&hasNext), hasNext)) {
        //         AutoPtr<IInterface> outface;
        //         it->GetNext((IInterface**)&outface);
        //         AutoPtr<IMapEntry> entry = IMapEntry::Probe(outface);
        //         AutoPtr<IInterface> obj;
        //         entry->GetValue((IInterface**)&obj);

        //         if (IParcelable::Probe(obj) != NULL) {
        //             if ((((Parcelable)obj).describeContents()
        //                     & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
        //                 fdFound = TRUE;
        //                 break;
        //             }
        //         }
        //         else if (obj instanceof Parcelable[]) {
        //             Parcelable[] array = (Parcelable[]) obj;
        //             for (int n = array.length - 1; n >= 0; n--) {
        //                 if ((array[n].describeContents()
        //                         & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
        //                     fdFound = TRUE;
        //                     break;
        //                 }
        //             }
        //         }
        //         else if (obj instanceof SparseArray) {
        //             SparseArray<? extends Parcelable> array =
        //                     (SparseArray<? extends Parcelable>) obj;
        //             for (int n = array.size() - 1; n >= 0; n--) {
        //                 if ((array.valueAt(n).describeContents()
        //                         & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0) {
        //                     fdFound = TRUE;
        //                     break;
        //                 }
        //             }
        //         }
        //         else if (obj instanceof ArrayList) {
        //             ArrayList array = (ArrayList) obj;
        //             // an ArrayList here might contain either Strings or
        //             // Parcelables; only look inside for Parcelables
        //             if (!array.isEmpty() && (array.get(0) instanceof Parcelable)) {
        //                 for (int n = array.size() - 1; n >= 0; n--) {
        //                     Parcelable p = (Parcelable) array.get(n);
        //                     if (p != null && ((p.describeContents()
        //                             & Parcelable.CONTENTS_FILE_DESCRIPTOR) != 0)) {
        //                         fdFound = TRUE;
        //                         break;
        //                     }
        //                 }
        //             }
        //         }
        //     }
        }

        mHasFds = fdFound;
        mFdsKnown = TRUE;
    }

    *result = mHasFds;
    return NOERROR;
}

ECode CBundle::PutParcelable(
    /* [in] */ const String& key,
    /* [in] */ IParcelable* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutSize(
    /* [in] */ const String& key,
    /* [in] */ ISize* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    return NOERROR;
}

ECode CBundle::PutSizeF(
    /* [in] */ const String& key,
    /* [in] */ ISizeF* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    return NOERROR;
}

ECode CBundle::PutParcelableArray(
    /* [in] */ const String& key,
    /* [in] */ ArrayOf<IParcelable*>* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    AutoPtr<IArrayOf> valueObj = CoreUtils::Convert(value);
    IMap::Probe(mMap)->Put(keyObj.Get(), valueObj.Get());
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableArrayList(
    /* [in] */ const String& key,
    /* [in] */ IArrayList* value) //ArrayList<? extends Parcelable> value
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutParcelableList(
    /* [in] */ const String& key,
    /* [in] */ IList* value)//List<? extends Parcelable> value
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutSparseParcelableArray(
    /* [in] */ const String& key,
    /* [in] */ ISparseArray* value)//SparseArray<? extends Parcelable> value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    mFdsKnown = FALSE;
    return NOERROR;
}

ECode CBundle::PutBundle(
    /* [in] */ const String& key,
    /* [in] */ IBundle* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    return NOERROR;
}

ECode CBundle::PutBinder(
    /* [in] */ const String& key,
    /* [in] */ IBinder* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    return NOERROR;
}

ECode CBundle::PutIBinder(
    /* [in] */ const String& key,
    /* [in] */ IBinder* value)
{
    Unparcel();
    AutoPtr<ICharSequence> keyObj = CoreUtils::Convert(key);
    IMap::Probe(mMap)->Put(keyObj.Get(), value);
    return NOERROR;
}

ECode CBundle::GetSize(
    /* [in] */ const String& key,
    /* [out] */ ISize** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();

    AutoPtr<IInterface> o = GetValue(key);
    if (ISize::Probe(o) == NULL) {
        TypeWarning(key, String("Size"));
        return NOERROR;
    }

    *value = ISize::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetSizeF(
    /* [in] */ const String& key,
    /* [out] */ ISizeF** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();

    AutoPtr<IInterface> o = GetValue(key);
    if (ISizeF::Probe(o) == NULL) {
        TypeWarning(key, String("SizeF"));
        return NOERROR;
    }

    *value = ISizeF::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetBundle(
    /* [in] */ const String& key,
    /* [out] */ IBundle** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();

    AutoPtr<IInterface> o = GetValue(key);
    if (IBundle::Probe(o) == NULL) {
        TypeWarning(key, String("Bundle"));
        return NOERROR;
    }

    *value = IBundle::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetParcelable(
    /* [in] */ const String& key,
    /* [out] */ IParcelable** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();

    AutoPtr<IInterface> o = GetValue(key);
    if (IParcelable::Probe(o) == NULL) {
        TypeWarning(key, String("Parcelable"));
        return NOERROR;
    }

    *value = IParcelable::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetParcelableArray(
    /* [in] */ const String& key,
    /* [out, callee] */ ArrayOf<IParcelable*>** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> o = GetValue(key);
    if (o == NULL) {
        return NOERROR;
    }

    IArrayOf* arrayObj = IArrayOf::Probe(o);
    if (arrayObj == NULL) {
        TypeWarning(key, String("Parcelable[]"));
        return NOERROR;
    }

    Int32 length = 0;
    arrayObj->GetLength(&length);
    AutoPtr<ArrayOf<IParcelable*> > array = ArrayOf<IParcelable*>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<IInterface> obj;
        arrayObj->Get(i, (IInterface**)&obj);
        if (obj) {
            assert(IParcelable::Probe(obj) != NULL);
        }
        array->Set(i, IParcelable::Probe(obj));
    }

    *value = array;
    REFCOUNT_ADD(*value)
    return NOERROR;
}

ECode CBundle::GetParcelableArrayList(
    /* [in] */ const String& key,
    /* [out] */ IArrayList** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> o = GetValue(key);
    if (o == NULL) {
        return NOERROR;
    }

    if (IArrayList::Probe(o) == NULL) {
        TypeWarning(key, String("ArrayList"));
        return NOERROR;
    }

    *value = IArrayList::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetSparseParcelableArray(
    /* [in] */ const String& key,
    /* [out] */ ISparseArray** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> o = GetValue(key);
    if (o == NULL) {
        return NOERROR;
    }

    if (ISparseArray::Probe(o) == NULL) {
        TypeWarning(key, String("SparseArray"));
        return NOERROR;
    }

    *value = ISparseArray::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetBinder(
    /* [in] */ const String& key,
    /* [out] */ IBinder** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> o = GetValue(key);
    if (o == NULL) {
        return NOERROR;
    }

    if (IBinder::Probe(o) == NULL) {
        TypeWarning(key, String("IBinder"));
        return NOERROR;
    }

    *value = IBinder::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::GetIBinder(
    /* [in] */ const String& key,
    /* [out] */ IBinder** value)
{
    VALIDATE_NOT_NULL(value)
    *value = NULL;

    Unparcel();
    AutoPtr<IInterface> o = GetValue(key);
    if (o == NULL) {
        return NOERROR;
    }

    if (IBinder::Probe(o) == NULL) {
        TypeWarning(key, String("IBinder"));
        return NOERROR;
    }

    *value = IBinder::Probe(o);
    REFCOUNT_ADD(*value);
    return NOERROR;
}

ECode CBundle::ReadFromParcel(
    /* [in] */ IParcel* parcel)
{
    BaseBundle::ReadFromParcelInner(parcel);
    mParcelledData->HasFileDescriptors(&mHasFds);
    mFdsKnown = TRUE;
    return NOERROR;
}

ECode CBundle::WriteToParcel(
    /* [in] */ IParcel* parcel)
{
    Boolean oldAllowFds;
    //parcel->PushAllowFds(mAllowFds, &oldAllowFds);
    // try {
    BaseBundle::WriteToParcelInner(parcel);
    // } finally {
    // parcel->RestoreAllowFds(oldAllowFds);
    // }
    return NOERROR;
}

ECode CBundle::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);

    if (mParcelledData != NULL) {
        if (mParcelledData == EMPTY_PARCEL) {
            *str = String("Bundle[EMPTY_PARCEL]");
            return NOERROR;
        }
        else {
            Int32 length;
            mParcelledData->GetElementSize(&length);
            StringBuilder sb("Bundle[mParcelledData.dataSize=");
            sb.Append(length);
            sb.Append("]");
            *str = sb.ToString();
            return NOERROR;
        }
    }

    StringBuilder sb("Bundle[");
    sb.Append(Object::ToString(mMap));
    sb.Append("]");
    *str = sb.ToString();
    return NOERROR;
}

} // namespace Os
} // namespace Droid
} // namespace Elastos
