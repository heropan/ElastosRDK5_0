
#include "CX500Principal.h"
#include "CStringWrapper.h"
#include <StringBuilder.h>

using Elastos::Core::CStringWrapper;
using Elastos::Core::ICharSequence;
using Elastos::Core::StringBuilder;

namespace Elastosx {
namespace Security {
namespace Auth {
namespace X500 {

ECode CX500Principal::GetEncoded(
    /* [out, callee] */ ArrayOf<Byte> **encodedName)
{
    //Apache...Todo later
    /*
        byte[] src = dn.getEncoded();
        byte[] dst = new byte[src.length];
        System.arraycopy(src, 0, dst, 0, dst.length);
        return dst;
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::GetNameEx(
    /* [in] */ const String& format,
    /* [out] */ String *name)
{
    if (CANONICAL.Equals(format)) {
        return GetCanonicalName(name);
    }
    //Apache...Todo later
    /*
        return dn.getName(format);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::GetNameEx2(
    /* [in] */ const String& format,
    /* [in] */ IMap *oidMap,
    /* [out] */ String * pName)
{
    //Apache...Todo later
    /*
        String rfc1779Name = dn.getName(RFC1779);
        String rfc2253Name = dn.getName(RFC2253);

        if (format.equalsIgnoreCase("RFC1779")) {
            StringBuilder resultName = new StringBuilder(rfc1779Name);
            int fromIndex = resultName.length();
            int equalIndex = -1;
            while (-1 != (equalIndex = resultName.lastIndexOf("=", fromIndex))) {
                int commaIndex = resultName.lastIndexOf(",", equalIndex);
                String subName = resultName.substring(commaIndex + 1,
                        equalIndex).trim();
                if (subName.length() > 4
                        && subName.substring(0, 4).equals("OID.")) {
                    String subSubName = subName.substring(4);
                    if (oidMap.containsKey(subSubName)) {
                        String replaceName = oidMap.get(subSubName);
                        if(commaIndex > 0) replaceName = " " + replaceName;
                        resultName.replace(commaIndex + 1, equalIndex, replaceName);
                    }
                }
                fromIndex = commaIndex;
            }
            return resultName.toString();
        } else if (format.equalsIgnoreCase("RFC2253")) {
            StringBuilder resultName = new StringBuilder(rfc2253Name);
            StringBuilder subsidyName = new StringBuilder(rfc1779Name);

            int fromIndex = resultName.length();
            int subsidyFromIndex = subsidyName.length();
            int equalIndex = -1;
            int subsidyEqualIndex = -1;
            while (-1 != (equalIndex = resultName.lastIndexOf("=", fromIndex))) {
                subsidyEqualIndex = subsidyName.lastIndexOf("=",
                        subsidyFromIndex);
                int commaIndex = resultName.lastIndexOf(",", equalIndex);
                String subName = resultName.substring(commaIndex + 1,
                        equalIndex).trim();
                if (oidMap.containsKey(subName)) {
                    int subOrignalEndIndex = resultName
                            .indexOf(",", equalIndex);
                    if (subOrignalEndIndex == -1)
                        subOrignalEndIndex = resultName.length();
                    int subGoalEndIndex = subsidyName.indexOf(",",
                            subsidyEqualIndex);
                    if (subGoalEndIndex == -1)
                        subGoalEndIndex = subsidyName.length();
                    resultName.replace(equalIndex + 1, subOrignalEndIndex,
                            subsidyName.substring(subsidyEqualIndex + 1,
                                    subGoalEndIndex));
                    resultName.replace(commaIndex + 1, equalIndex, oidMap
                            .get(subName));
                }
                fromIndex = commaIndex;
                subsidyFromIndex = subsidyEqualIndex - 1;
            }
            return resultName.toString();
        } else {
            throw new IllegalArgumentException("invalid format specified: " + format);
        }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::Equals(
    /* [in] */ IInterface *o,
    /* [out] */ Boolean *result)
{
    VALIDATE_NOT_NULL(result)
    if ((CX500Principal*)(IX500Principal*)o == this) {
        *result = TRUE;
        return NOERROR;
    }
    InterfaceID id1, id2;
    this->GetInterfaceID(this->Probe(EIID_IInterface), &id1);
    o->GetInterfaceID(o, &id2);
    if (o == NULL || id1 == id2) {
        *result = FALSE;
        return NOERROR;
    }
    AutoPtr<IX500Principal> principal = IX500Principal::Probe(o);
    String name1, name2;
    this->GetCanonicalName(&name1);
    ((CX500Principal*)principal.Get())->GetCanonicalName(&name2);
    *result = name1.Equals(name2);
    return NOERROR;
}

ECode CX500Principal::GetName(
    /* [out] */ String * pName)
{
    //Apache...Todo later
    /* return dn.getName(RFC2253); */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::GetHashCode(
    /* [out] */ Int32 *hashCode)
{
    VALIDATE_NOT_NULL(hashCode)
    String name;
    GetCanonicalName(&name);
    *hashCode = name.GetHashCode();
    return NOERROR;
}

ECode CX500Principal::ToString(
    /* [out] */ String * pStr)
{
    //Apache...Todo later
    /*
        return dn.getName(RFC1779);
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::constructor(
    /* [in] */ ArrayOf<Byte> *name)
{
    if (name == NULL) {
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    //Apache...Todo later
    /*
    try {
        // FIXME dn = new Name(name);
        dn = (Name) Name.ASN1.decode(name);
    } catch (IOException e) {
        throw incorrectInputEncoding(e);
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::constructor(
    /* [in] */ IInputStream *in)
{
    if (in == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    //Apache...Todo later
    /*
    try {
        // FIXME dn = new Name(is);
        dn = (Name) Name.ASN1.decode(in);
    } catch (IOException e) {
        throw incorrectInputEncoding(e);
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::constructor(
    /* [in] */ const String& name)
{
    if (name.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    //Apache...Todo later
    /*
    try {
        dn = new Name(name);
    } catch (IOException e) {
        throw incorrectInputName(e, name);
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::constructor(
    /* [in] */ const String& name,
    /* [in] */ IMap *keywordMap)
{
    if (name.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }
    //Apache...Todo later
    /*
    try {
        dn = new Name(substituteNameFromMap(name, keywordMap));
    } catch (IOException e) {
        throw incorrectInputName(e, name);
    }
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::GetCanonicalName(
        /* [out] */ String *name)
{
    VALIDATE_NOT_NULL(name)
    AutoLock lock(mLock);
    if (mCanonicalName.IsNull()) {
        //Apache...Todo later
        //canonicalName = dn.getName(CANONICAL);
    }
    *name = mCanonicalName;
    return NOERROR;
}

ECode CX500Principal::WriteObject(
    /* [in] */ IObjectOutputStream *out)
{
    //Apache...Todo later
    /*
        out.writeObject(dn.getEncoded());
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::ReadObject(
    /* [in] */ IObjectInputStream *in)
{
    //Apache...Todo later
    /*
        dn = (Name) Name.ASN1.decode((byte[]) in.readObject());
    */
    return E_NOT_IMPLEMENTED;
}

ECode CX500Principal::SubstituteNameFromMap(
    /* [in] */ const String& name,
    /* [in] */ IMap *keywordMap,
    /* [out] */ String *nm)
{
    StringBuilder sbName(name);
    Int32 fromIndex;
    sbName.GetLength(&fromIndex);
    Int32 equalIndex;
    String tmpName;
    Int32 tmpIndex;
    while (-1 != (equalIndex = (sbName.LastIndexOfEx(String("="), fromIndex, &tmpIndex), tmpIndex))) {
        Int32 commaIndex;
        sbName.LastIndexOfEx(String(","), equalIndex, &commaIndex);
        String subName;
        subName = (sbName.SubstringEx(commaIndex + 1, equalIndex, &subName), subName).Trim();
        AutoPtr<ICharSequence> key;
        CStringWrapper::New(subName, (ICharSequence**)&key);
        Boolean isContained;
        keywordMap->ContainsKey(key, &isContained);
        if (isContained) {
            AutoPtr<IInterface> elem;
            keywordMap->Get(key, (IInterface**)&elem);
            String elemString;
            ICharSequence::Probe(elem)->ToString(&elemString);
            sbName.Replace(commaIndex + 1, equalIndex, elemString);
        }
        fromIndex = commaIndex;
    }
    return sbName.ToString(nm);
}

}
}
}
}

