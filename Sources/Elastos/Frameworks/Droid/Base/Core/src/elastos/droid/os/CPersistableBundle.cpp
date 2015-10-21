
#include "elastos/droid/os/CPersistableBundle.h"
#include "elastos/droid/utility/CArrayMap.h"

using Elastos::Droid::Utility::CArrayMap;

namespace Elastos {
namespace Droid {
namespace Os {

static AutoPtr<IPersistableBundle> InitEMPTY()
{
    AutoPtr<CPersistableBundle> cpb;
    CPersistableBundle::NewByFriend((CPersistableBundle**)&cpb);
    cpb->mMap = CArrayMap::EMPTY;
    return (IPersistableBundle*)cpb.Get();
}

const AutoPtr<IPersistableBundle> CPersistableBundle::EMPTY;

const String CPersistableBundle::TAG_PERSISTABLEMAP("pbundle_as_map");

CAR_INTERFACE_IMPL_4(CPersistableBundle, BaseBundle, IPersistableBundle, ICloneable, IParcelable, IXmlUtilsWriteMapCallback)

CAR_OBJECT_IMPL(CPersistableBundle)

ECode CPersistableBundle::constructor()
{
    return BaseBundle::constructor();
}

ECode CPersistableBundle::constructor(
    /* [in] */ Int32 capacity)
{
    return BaseBundle::constructor(capacity);
}

ECode CPersistableBundle::constructor(
    /* [in] */ IPersistableBundle* b)
{
    return BaseBundle::constructor(IBaseBundle::Probe(b));
}

ECode CPersistableBundle::constructor(
    /* [in] */ IMap* map)
{
    FAIL_RETURN(BaseBundle::constructor())

    // First stuff everything in.
    PutAll(map);

    // Now verify each item throwing an exception if there is a violation.
    AutoPtr<ISet> keys;
    map->GetKeySet((ISet**)&keys)

    AutoPtr<IIterator> iterator;
    keys->GetIterator((IIterator**)&iterator);
    Boolean result;
    while (iterator->HasNext(&result), result) {
        AutoPtr<IInterface> key;
        iterator->GetNext((IInterface**)&key);
        AutoPtr<IInterface> value;
        map->Get(key, (IInterface**)&value);

        if (IMap::Probe(value) != NULL) {
            // Fix up any Maps by replacing them with PersistableBundles.
            putPersistableBundle(key, new PersistableBundle((Map<String, Object>) value));
        }
        else if (!(value instanceof Integer) && !(value instanceof Long) &&
                !(value instanceof Double) && !(value instanceof String) &&
                !(value instanceof int[]) && !(value instanceof long[]) &&
                !(value instanceof double[]) && !(value instanceof String[]) &&
                !(value instanceof PersistableBundle) && (value != null)) {
            throw new IllegalArgumentException("Bad value in PersistableBundle key=" + key +
                    " value=" + value);
        }
    }

    return NOERROR;
}

AutoPtr<IPersistableBundle> CPersistableBundle::ForPair(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
    return NULL;
}

ECode CPersistableBundle::Clone(
    /* [out] */ IInterface* obj)
{
    return NOERROR;
}

ECode CPersistableBundle::PutPersistableBundle(
    /* [in] */ const String& key,
    /* [in] */ IPersistableBundle* value)
{
    return NOERROR;
}

ECode CPersistableBundle::GetPersistableBundle(
    /* [in] */ const String& key,
    /* [out] */ IPersistableBundle** value)
{
    return NOERROR;
}

ECode CPersistableBundle::WriteUnknownObject(
    /* [in] */ IInterface v,
    /* [in] */ const String& name,
    /* [in] */ IXmlSerializer* out)
{
    return NOERROR;
}

ECode CPersistableBundle::SaveToXml(
    /* [in] */ IXmlSerializer* out)
{
    return NOERROR;
}

ECode CPersistableBundle::RestoreFromXml(
    /* [in] */ IXmlPullParser* in,
    /* [out] */ IPersistableBundle** pb)
{
    return NOERROR;
}

ECode CPersistableBundle::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    return NOERROR;
}

ECode CPersistableBundle::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    return NOERROR;
}

ECode CPersistableBundle::ToString(
    /* [out] */ String* str)
{
    return NOERROR;
}


} // namespace Os
} // namespace Droid
} // namespace Elastos