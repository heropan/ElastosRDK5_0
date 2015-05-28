
#include "CProperties.h"

namespace Elastos{
namespace Utility{

PInterface CProperties::Probe(
    /* [in] */ REIID riid)
{
    return _CProperties::Probe(riid);
}

UInt32 CProperties::AddRef()
{
    return _CProperties::AddRef();
}

UInt32 CProperties::Release()
{
    return _CProperties::Release();
}

ECode CProperties::GetProperty(
    /* [in] */ const String& name,
    /* [out] */ String* str)
{
    return  Properties::GetProperty(name,str);
}

ECode CProperties::GetProperty(
    /* [in] */ const String& name,
    /* [in] */ const String& defaultValue,
    /* [out] */ String* str)
{
    return  Properties::GetProperty(name,defaultValue,str);
}

ECode CProperties::List(
    /* [in] */ IPrintStream* outstream)
{
    return  Properties::List(outstream);
}

ECode CProperties::List(
    /* [in] */ IPrintWriter* outwriter)
{
    return  Properties::List(outwriter);
}

ECode CProperties::Load(
    /* [in] */ IInputStream* instream)
{
    return  Properties::Load(instream);
}

ECode CProperties::Load(
    /* [in] */ IReader* inreader)
{
    return  Properties::Load(inreader);
}

ECode CProperties::PropertyNames(
    /* [out] */ IEnumeration** names)
{
    return  Properties::PropertyNames(names);
}

ECode CProperties::StringPropertyNames(
    /* [out] */ ISet** strNames)
{
    return  Properties::StringPropertyNames(strNames);
}

ECode CProperties::Save(
    /* [in] */ IOutputStream* outstream,
    /* [in] */ const String& comment)
{
    return  Properties::Save(outstream,comment);
}

ECode CProperties::SetProperty(
    /* [in] */ const String& name,
    /* [in] */ const String& value ,
    /* [out] */ String* oldValue)
{
    return  Properties::SetProperty(name, value, oldValue);
}

ECode CProperties::Store(
    /* [in] */ IOutputStream* outstream,
    /* [in] */ const String& comment)
{
    return  Properties::Store(outstream , comment);
}

ECode CProperties::Store(
    /* [in] */ IWriter* writer,
    /* [in] */ const String& comment)
{
    return  Properties::Store(writer, comment);
}

ECode CProperties::LoadFromXML(
    /* [in] */ IInputStream* instream)
{
    return  Properties::LoadFromXML(instream);
}

ECode CProperties::StoreToXML(
    /* [in] */ IOutputStream* os,
    /* [in] */ const String& comment)
{
    return  Properties::StoreToXML(os,comment);
}

ECode CProperties::StoreToXML(
    /* [in] */ IOutputStream* os,
    /* [in] */ const String& comment,
    /* [in] */ const String& encoding)
{
    return  Properties::StoreToXML(os,comment,encoding);
}

ECode CProperties::Clear()
{
    return Properties::Clear();
}

ECode CProperties::ContainsKey(
    /* [in] */ IInterface* key,
    /* [out] */ Boolean* result)
{
    return Properties::ContainsKey(key, result);
}

ECode CProperties::ContainsValue(
    /* [in] */ IInterface* value,
    /* [out] */ Boolean* result)
{
    return Properties::ContainsValue(value, result);
}

ECode CProperties::EntrySet(
    /* [out] */ ISet** entries)
{
    return Properties::EntrySet(entries);
}

ECode CProperties::Equals(
    /* [in] */ IInterface* object,
    /* [out] */ Boolean* result)
{
    return Properties::Equals(object, result);
}

ECode CProperties::Get(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return Properties::Get(key, value);
}

ECode CProperties::GetHashCode(
    /* [out] */ Int32* hashCode)
{
    return Properties::GetHashCode(hashCode);
}

ECode CProperties::IsEmpty(
    /* [out] */ Boolean* result)
{
    return Properties::IsEmpty(result);
}

ECode CProperties::KeySet(
    /* [out] */ ISet** keySet)
{
    return Properties::KeySet(keySet);
}

ECode CProperties::Put(
    /* [in] */ PInterface key,
    /* [in] */ PInterface value,
    /* [out] */ PInterface* oldValue)
{
    return Properties::Put(key, value, oldValue);
}

ECode CProperties::PutAll(
    /* [in] */ IMap* map)
{
    return Properties::PutAll(map);
}

ECode CProperties::Remove(
    /* [in] */ PInterface key,
    /* [out] */ PInterface* value)
{
    return Properties::Remove(key, value);
}

ECode CProperties::GetSize(
    /* [out] */ Int32* size)
{
    return Properties::GetSize(size);
}

ECode CProperties::Values(
    /* [out] */ ICollection** value)
{
    return Properties::Values(value);
}

ECode CProperties::Clone(
    /* [out] */ IInterface** object)
{
    return Properties::Clone(object);
}

ECode CProperties::Elements(
    /* [out] */ IEnumeration** enm)
{
    return Properties::Elements(enm);
}

ECode CProperties::Keys(
    /* [out] */ IEnumeration** enm)
{
    return Properties::Keys(enm);
}

ECode CProperties::constructor()
{
    return HashTable::Init();
}

ECode CProperties::constructor(
    /* [in] */ IProperties* properties)
{
    FAIL_RETURN(HashTable::Init());
    return Properties::Init((CProperties*)properties);
}

Mutex* CProperties::GetSelfLock()
{
    return &_m_syncLock;
}

} // namespace Utility
} // namespace Elastos
