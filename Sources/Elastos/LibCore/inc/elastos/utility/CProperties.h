#ifndef __UTILITY_CPROPERTIES_H__
#define __UTILITY_CPROPERTIES_H__

#include "_CProperties.h"
#include "Properties.h"

namespace Elastos{
namespace Utility{

CarClass(CProperties), public Properties
{
public:
    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetProperty(
        /* [in] */ const String& name,
        /* [out] */ String* str);

    CARAPI GetProperty(
        /* [in] */ const String& name,
        /* [in] */ const String& defaultValue,
        /* [out] */ String* str);

    CARAPI List(
        /* [in] */ IPrintStream* outstream);

    CARAPI List(
        /* [in] */ IPrintWriter* outwriter);

    CARAPI Load(
        /* [in] */ IInputStream* instream);

    CARAPI Load(
        /* [in] */ IReader* inreader);

    CARAPI PropertyNames(
        /* [out] */ IEnumeration** names);

    CARAPI StringPropertyNames(
        /* [out] */ ISet** strNames);

    CARAPI Save(
        /* [in] */ IOutputStream* outstream,
        /* [in] */ const String& comment);

    CARAPI SetProperty(
        /* [in] */ const String& name,
        /* [in] */ const String& value ,
        /* [out] */ String* obj);

    CARAPI Store(
        /* [in] */ IOutputStream* outstream,
        /* [in] */ const String& comment);

    CARAPI Store(
        /* [in] */ IWriter* writer,
        /* [in] */ const String& comment);

    CARAPI LoadFromXML(
        /* [in] */ IInputStream* instream);

    CARAPI StoreToXML(
        /* [in] */ IOutputStream* os,
        /* [in] */ const String& comment);

    CARAPI StoreToXML(
        /* [in] */ IOutputStream* os,
        /* [in] */ const String& comment,
        /* [in] */ const String& encoding);

    CARAPI Clear();

    CARAPI ContainsKey(
        /* [in] */ IInterface* key,
        /* [out] */ Boolean* result);

    CARAPI ContainsValue(
        /* [in] */ IInterface* value,
        /* [out] */ Boolean* result);

    CARAPI EntrySet(
        /* [out] */ ISet** entries);

    CARAPI Equals(
        /* [in] */ IInterface* object,
        /* [out] */ Boolean* result);

    CARAPI Get(
        /* [in] */ PInterface key,
        /* [out] */ PInterface* value);

    CARAPI GetHashCode(
        /* [out] */ Int32* hashCode);

    CARAPI IsEmpty(
        /* [out] */ Boolean* result);

    CARAPI KeySet(
        /* [out] */ ISet** keySet);

    CARAPI Put(
        /* [in] */ PInterface key,
        /* [in] */ PInterface value,
        /* [out] */ PInterface* oldValue);

    CARAPI PutAll(
        /* [in] */ IMap* map);

    CARAPI Remove(
        /* [in] */ PInterface key,
        /* [out] */ PInterface* value);

    CARAPI GetSize(
        /* [out] */ Int32* size);

    CARAPI Values(
        /* [out] */ ICollection** value);

    CARAPI Clone(
        /* [out] */ IInterface** object);

    CARAPI Elements(
        /* [out] */ IEnumeration** enm);

    CARAPI Keys(
        /* [out] */ IEnumeration** enm);

    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ IProperties* properties);

protected:
    CARAPI_(Mutex*) GetSelfLock();
};

} // namespace Utility
} // namespace Elastos

#endif // __UTILITY_CPROPERTIES_H__
