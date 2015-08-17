#include "net/CCompareResult.h"
#include "net/CLinkProperties.h"
#include "net/CProxyProperties.h"
#include "util/CParcelableObjectContainer.h"
#include "text/TextUtils.h"
#include <elastos/core/StringBuilder.h>
#include <elastos/utility/etl/Algorithm.h>

using Elastos::Net::IInetAddressHelper;
using Elastos::Net::CInetAddressHelper;
using Elastos::Core::CObjectContainer;
using Elastos::Droid::Text::TextUtils;
using Elastos::Droid::Net::CProxyProperties;
using Elastos::Droid::Net::CCompareResult;
using Elastos::Droid::Utility::CParcelableObjectContainer;
using Elastos::Core::ICharSequence;
using Elastos::Core::ICharSequence;
using Elastos::Core::CString;
using Elastos::Core::StringBuilder;

namespace Elastos {
namespace Droid {
namespace Net {

ECode CLinkProperties::constructor()
{
    return NOERROR;
}

// copy constructor instead of clone
ECode CLinkProperties::constructor(
    /* [in] */ ILinkProperties* source)
{
    if (source != NULL) {
        source->GetInterfaceName(&mIfaceName);

        AutoPtr<IObjectContainer> addr;
        source->GetLinkAddresses((IObjectContainer**)&addr);
        AutoPtr<IObjectEnumerator> emu;
        FAIL_RETURN(addr->GetObjectEnumerator((IObjectEnumerator**)&emu));
        Boolean hasNext;
        while (emu->MoveNext(&hasNext), hasNext) {
            AutoPtr<ILinkAddress> linkaddr;
            emu->Current((IInterface**)&linkaddr);
            AddLinkAddress(linkaddr);
        }

        AutoPtr<IObjectContainer> dns;
        source->GetLinkAddresses((IObjectContainer**)&dns);
        AutoPtr<IObjectEnumerator> emu2;
        FAIL_RETURN(dns->GetObjectEnumerator((IObjectEnumerator**)&emu2));
        Boolean hasNext2;
        while (emu->MoveNext(&hasNext2), hasNext2) {
            AutoPtr<IInetAddress> inetaddr;
            emu2->Current((IInterface**)&inetaddr);
            AddDns(inetaddr);
        }

        AutoPtr<IObjectContainer> route;
        source->GetLinkAddresses((IObjectContainer**)&route);
        AutoPtr<IObjectEnumerator> emu3;
        FAIL_RETURN(route->GetObjectEnumerator((IObjectEnumerator**)&emu3));
        Boolean hasNext3;
        while (emu->MoveNext(&hasNext3), hasNext3) {
            AutoPtr<IRouteInfo> routeinfo;
            emu3->Current((IInterface**)&routeinfo);
            AddRoute(routeinfo);
        }

        AutoPtr<IProxyProperties> proxy;
        source->GetHttpProxy((IProxyProperties**)&proxy);
        if (proxy == NULL)
            mHttpProxy = NULL;
        else {
            CProxyProperties::New(proxy, (IProxyProperties**)&mHttpProxy);
        }
    }
    return NOERROR;
}

ECode CLinkProperties::SetInterfaceName(
    /* [in] */ const String& iface)
{
    mIfaceName = iface;
    return NOERROR;
}

ECode CLinkProperties::GetInterfaceName(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);
    *result = mIfaceName;
    return NOERROR;
}

ECode CLinkProperties::GetAddresses(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    AutoPtr<IObjectContainer> addresses;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&addresses));

    List< AutoPtr<ILinkAddress> >::Iterator iter;
    for (iter = mLinkAddresses.Begin(); iter != mLinkAddresses.End(); ++iter) {
        AutoPtr<IInetAddress> InetAddr;
        (*iter)->GetAddress((IInetAddress**)&InetAddr);
        addresses->Add(InetAddr);
    }
    *result = addresses.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CLinkProperties::AddLinkAddress(
    /* [in] */ ILinkAddress* address)
{
    if (address != NULL) mLinkAddresses.PushBack(address);
    return NOERROR;
}

ECode CLinkProperties::GetLinkAddresses(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    AutoPtr<IObjectContainer> out;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&out));

    List< AutoPtr<ILinkAddress> >::Iterator iter;
    for (iter = mLinkAddresses.Begin(); iter != mLinkAddresses.End(); ++iter) {
        out->Add(*iter);
    }
    *result = out.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CLinkProperties::AddDns(
    /* [in] */ IInetAddress* dns)
{
    if (dns != NULL) mDnses.PushBack(dns);
    return NOERROR;
}

ECode CLinkProperties::GetDnses(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    AutoPtr<IObjectContainer> out;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&out));

    List< AutoPtr<IInetAddress> >::Iterator iter;
    for (iter = mDnses.Begin(); iter != mDnses.End(); ++iter) {
        out->Add(*iter);
    }
    *result = out.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CLinkProperties::AddRoute(
    /* [in] */ IRouteInfo* route)
{
    if (route != NULL) {
        mRoutes.PushBack(route);
    }
    return NOERROR;
}

ECode CLinkProperties::GetRoutes(
    /* [out] */ IObjectContainer** result)
{
    VALIDATE_NOT_NULL(result);
    *result = NULL;
    AutoPtr<IObjectContainer> out;
    FAIL_RETURN(CParcelableObjectContainer::New((IObjectContainer**)&out));

    List< AutoPtr<IRouteInfo> >::Iterator iter;
    for (iter = mRoutes.Begin(); iter != mRoutes.End(); ++iter) {
        out->Add(*iter);
    }
    *result = out.Get();
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CLinkProperties::SetHttpProxy(
    /* [in] */ IProxyProperties* proxy)
{
    mHttpProxy = proxy;
    return NOERROR;
}

ECode CLinkProperties::GetHttpProxy(
    /* [out] */ IProxyProperties** result)
{
    VALIDATE_NOT_NULL(result);
    *result = mHttpProxy;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

ECode CLinkProperties::Clear()
{
    mIfaceName = NULL;
    mLinkAddresses.Clear();
    mDnses.Clear();
    mRoutes.Clear();
    mHttpProxy = NULL;
    return NOERROR;
}

/**
 * Implement the Parcelable interface
 * @hide
 */

//@Override
ECode CLinkProperties::ToString(
    /* [out] */ String* result)
{
    VALIDATE_NOT_NULL(result);

    StringBuilder sb("InterfaceName: ");

    if (mIfaceName.IsNull()) {
        sb += "NULL";
    }
    else {
        sb += mIfaceName;
    }

    sb += " LinkAddresses: [";
    String str;
    List< AutoPtr<ILinkAddress> >::Iterator iter1;
    for (iter1 = mLinkAddresses.Begin(); iter1 != mLinkAddresses.End(); ++iter1) {
        if (iter1 != mLinkAddresses.Begin())
            sb += ", ";
        (*iter1)->ToString(&str);
        sb += str;
    }
    sb += String("]");

    sb += " DnsAddresses: [";
    List< AutoPtr<IInetAddress> >::Iterator iter2;
    for (iter2 = mDnses.Begin(); iter2 != mDnses.End(); ++iter2) {
        if (iter2 != mLinkAddresses.Begin())
            sb += ", ";
        (*iter2)->GetHostAddress(&str);
        sb += str;
    }
    sb += String("] ");

    sb += " Routes: [";
    List< AutoPtr<IRouteInfo> >::Iterator iter3;
    for (iter3 = mRoutes.Begin(); iter3 != mRoutes.End(); ++iter3) {
        if (iter3 != mLinkAddresses.Begin())
            sb += ", ";
        (*iter3)->ToString(&str);
        sb += str;
    }
    sb += String("] ");

    sb += " HttpProxy: ";
    if (mHttpProxy == NULL) {
        sb += "NULL";
    }
    else {
        mHttpProxy->ToString(&str);
        sb += str;
    }

    *result = sb.ToString();
    return NOERROR;
}

/**
 * Compares this {@code LinkProperties} interface name against the target
 *
 * @param target LinkProperties to compare.
 * @return {@code TRUE} if both are identical, {@code FALSE} otherwise.
 */
ECode CLinkProperties::IsIdenticalInterfaceName(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(target);

    String str1, str2;
    GetInterfaceName(&str1);
    target->GetInterfaceName(&str2);
    AutoPtr<ICharSequence> chars1;
    CString::New(str1, (ICharSequence**)&chars1);
    AutoPtr<ICharSequence> chars2;
    CString::New(str2, (ICharSequence**)&chars2);
    *result = TextUtils::Equals(chars1, chars2);
    return NOERROR;
}

/**
 * Compares this {@code LinkProperties} interface name against the target
 *
 * @param target LinkProperties to compare.
 * @return {@code TRUE} if both are identical, {@code FALSE} otherwise.
 */
ECode CLinkProperties::IsIdenticalAddresses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(target);

    AutoPtr<IObjectContainer> targetAddr;
    target->GetAddresses((IObjectContainer**)&targetAddr);

    AutoPtr<IObjectContainer> sourceAddr;
    GetAddresses((IObjectContainer**)&sourceAddr);

    Int32 targetSize, sourceSize;
    targetAddr->GetObjectCount(&targetSize);
    sourceAddr->GetObjectCount(&sourceSize);
    if (targetSize == sourceSize) {
        sourceAddr->ContainsAll(targetAddr, result);
    }
    return NOERROR;
}

/**
 * Compares this {@code LinkProperties} DNS addresses against the target
 *
 * @param target LinkProperties to compare.
 * @return {@code TRUE} if both are identical, {@code FALSE} otherwise.
 */
ECode CLinkProperties::IsIdenticalDnses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(target);

    AutoPtr<IObjectContainer> targetDnses;
    target->GetDnses((IObjectContainer**)&targetDnses);

    AutoPtr<IObjectContainer> sourceDnses;
    GetDnses((IObjectContainer**)&sourceDnses);

    Int32 targetSize, sourceSize;
    targetDnses->GetObjectCount(&targetSize);
    sourceDnses->GetObjectCount(&sourceSize);
    if (targetSize == sourceSize) {
        sourceDnses->ContainsAll(targetDnses, result);
    }
    return NOERROR;
}

/**
 * Compares this {@code LinkProperties} Routes against the target
 *
 * @param target LinkProperties to compare.
 * @return {@code TRUE} if both are identical, {@code FALSE} otherwise.
 */
ECode CLinkProperties::IsIdenticalRoutes(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(target);

    AutoPtr<IObjectContainer> targetRoutes;
    target->GetRoutes((IObjectContainer**)&targetRoutes);

    AutoPtr<IObjectContainer> sourceRoutes;
    GetRoutes((IObjectContainer**)&sourceRoutes);

    Int32 targetSize, sourceSize;
    targetRoutes->GetObjectCount(&targetSize);
    sourceRoutes->GetObjectCount(&sourceSize);
    if (targetSize == sourceSize) {
        sourceRoutes->ContainsAll(targetRoutes, result);
    }
    return NOERROR;
}

/**
 * Compares this {@code LinkProperties} HttpProxy against the target
 *
 * @param target LinkProperties to compare.
 * @return {@code TRUE} if both are identical, {@code FALSE} otherwise.
 */
ECode CLinkProperties::IsIdenticalHttpProxy(
    /* [in] */ ILinkProperties* target,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(target);

    AutoPtr<IProxyProperties> sourceProxy;
    AutoPtr<IProxyProperties> targerProxy;

    GetHttpProxy((IProxyProperties**)&sourceProxy);
    target->GetHttpProxy((IProxyProperties**)&targerProxy);
    if (sourceProxy == NULL) {
        *result = (targerProxy == NULL);
    }
    else {
        sourceProxy->Equals(targerProxy, result);
    }
    return NOERROR;
}

/**
 * Compares this {@code LinkProperties} instance against the target
 * LinkProperties in {@code obj}. Two LinkPropertieses are equal if
 * all their fields are equal in values.
 *
 * For collection fields, such as mDnses, containsAll() is used to check
 * if two collections contains the same elements, independent of order.
 * There are two thoughts regarding containsAll()
 * 1. Duplicated elements. eg, (A, B, B) and (A, A, B) are equal.
 * 2. Worst case performance is O(n^2).
 *
 * @param obj the object to be tested for equality.
 * @return {@code TRUE} if both objects are equal, {@code FALSE} otherwise.
 */
ECode CLinkProperties::Equals(
    /* [in] */ ILinkProperties* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(obj);

    Boolean bo1, bo2, bo3, bo4, bo5;
    IsIdenticalInterfaceName(obj, &bo1);
    IsIdenticalAddresses(obj, &bo2);
    IsIdenticalDnses(obj, &bo3);
    IsIdenticalRoutes(obj, &bo4);
    IsIdenticalHttpProxy(obj, &bo5);

    *result = (bo1 && bo2 && bo3 && bo4 && bo5);
    return NOERROR;
}

ECode CLinkProperties::Equals(
    /* [in] */ IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;
    VALIDATE_NOT_NULL(obj);
    return Equals(ILinkProperties::Probe(obj), result);
}

/**
 * Return two lists, a list of addresses that would be removed from
 * mLinkAddresses and a list of addresses that would be added to
 * mLinkAddress which would then result in target and mLinkAddresses
 * being the same list.
 *
 * @param target is a LinkProperties with the new list of addresses
 * @return the removed and added lists.
 */
ECode CLinkProperties::CompareAddresses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ICompareResult** result)
{
    VALIDATE_NOT_NULL(result);

    /*
     * Duplicate the LinkAddresses into removed, we will be removing
     * address which are common between mLinkAddresses and target
     * leaving the addresses that are different. And address which
     * are in target but not in mLinkAddresses are placed in the
     * addedAddresses.
     */

    List< AutoPtr<ILinkAddress> > cprRemoved(mLinkAddresses);
    List< AutoPtr<ILinkAddress> >::Iterator iter;
    AutoPtr<ICompareResult> cprResult;
    CCompareResult::New((ICompareResult**)&cprResult);

    if (target != NULL) {
        AutoPtr<IObjectContainer> container;
        FAIL_RETURN(target->GetLinkAddresses((IObjectContainer **)&container));
        AutoPtr<IObjectEnumerator> emu;
        FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator **)&emu))

        Boolean hasNext;
        while (emu->MoveNext(&hasNext), hasNext) {
            AutoPtr<ILinkAddress> newAddress;
            emu->Current((IInterface**)&newAddress);

            iter = Find(cprRemoved.Begin(), cprRemoved.End(), newAddress);
            if (iter == cprRemoved.End()) {
                cprResult->AddAdded(newAddress);
            }
            else {
                cprRemoved.Erase(iter);
            }
        }
    }

    for (iter = cprRemoved.Begin(); iter != cprRemoved.End(); ++iter) {
        cprResult->AddRemoved(*iter);
    }

    *result = cprResult;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return two lists, a list of dns addresses that would be removed from
 * mDnses and a list of addresses that would be added to
 * mDnses which would then result in target and mDnses
 * being the same list.
 *
 * @param target is a LinkProperties with the new list of dns addresses
 * @return the removed and added lists.
 */
ECode CLinkProperties::CompareDnses(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ICompareResult** result)
{
    VALIDATE_NOT_NULL(result);

    /*
     * Duplicate the InetAddresses into removed, we will be removing
     * dns address which are common between mDnses and target
     * leaving the addresses that are different. And dns address which
     * are in target but not in mDnses are placed in the
     * addedAddresses.
     */

    List< AutoPtr<IInetAddress> > cprRemoved(mDnses);
    List< AutoPtr<IInetAddress> >::Iterator iter;
    AutoPtr<ICompareResult> cprResult;
    CCompareResult::New((ICompareResult**)&cprResult);

    if (target != NULL) {
        AutoPtr<IObjectContainer> container;
        FAIL_RETURN(target->GetDnses((IObjectContainer **)&container));
        AutoPtr<IObjectEnumerator> emu;
        FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator **)&emu))
        Boolean hasNext;
        while(emu->MoveNext(&hasNext), hasNext) {
            AutoPtr<IInetAddress> newDns;
            emu->Current((IInterface**)&newDns);

            iter = Find(cprRemoved.Begin(), cprRemoved.End(), newDns);
            if (iter == cprRemoved.End()) {
                cprResult->AddAdded(newDns);
            }
            else {
                cprRemoved.Erase(iter);
            }
        }
    }

    for (iter = cprRemoved.Begin(); iter != cprRemoved.End(); ++iter) {
        cprResult->AddRemoved(*iter);
    }
    *result = cprResult;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

/**
 * Return two lists, a list of routes that would be removed from
 * mRoutes and a list of routes that would be added to
 * mRoutes which would then result in target and mRoutes
 * being the same list.
 *
 * @param target is a LinkProperties with the new list of routes
 * @return the removed and added lists.
 */
ECode CLinkProperties::CompareRoutes(
    /* [in] */ ILinkProperties* target,
    /* [out] */ ICompareResult** result)
{
    VALIDATE_NOT_NULL(result);

    List< AutoPtr<IRouteInfo> > cprRemoved(mRoutes);
    List< AutoPtr<IRouteInfo> >::Iterator iter;
    AutoPtr<ICompareResult> cprResult;
    CCompareResult::New((ICompareResult**)&cprResult);

    if (target != NULL) {
        AutoPtr<IObjectContainer> container;
        FAIL_RETURN(target->GetRoutes((IObjectContainer **)&container));
        AutoPtr<IObjectEnumerator> emu;
        FAIL_RETURN(container->GetObjectEnumerator((IObjectEnumerator **)&emu))
        Boolean hasNext;
        while(emu->MoveNext(&hasNext), hasNext) {
            AutoPtr<IRouteInfo> newRoute;
            emu->Current((IInterface**)&newRoute);

            iter = Find(cprRemoved.Begin(), cprRemoved.End(), newRoute);
            if (iter == cprRemoved.End()) {
                cprResult->AddAdded(newRoute);
            }
            else {
                cprRemoved.Erase(iter);
            }
        }
    }

    for (iter = cprRemoved.Begin(); iter != cprRemoved.End(); ++iter) {
        cprResult->AddRemoved(*iter);
    }
    *result = cprResult;
    REFCOUNT_ADD(*result);
    return NOERROR;
}

//@Override
/**
 * generate hashcode based on significant fields
 * Equal objects must produce the same hash code, while unequal objects
 * may have the same hash codes.
 */
ECode CLinkProperties::GetHashCode(
    /* [out] */ Int32* result)
{
    VALIDATE_NOT_NULL(result);
    if (mIfaceName.IsNull()) {
        *result = 0;
    }
    else {
        *result = mIfaceName.GetHashCode();
    }

    *result += mLinkAddresses.GetSize() * 31;
    *result += mDnses.GetSize() * 37;
    *result += mRoutes.GetSize() * 41;

    if (mHttpProxy == NULL) {
        *result += 0;
    }
    else {
        Int32 proxyHashcode;
        mHttpProxy->GetHashCode(&proxyHashcode);
        *result += proxyHashcode;
    }
    return NOERROR;
}

/**
 * Implement the Parcelable interface.
 * @hide
 */
ECode CLinkProperties::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    String name;
    GetInterfaceName(&name);
    dest->WriteString(name);
    dest->WriteInt32(mLinkAddresses.GetSize());

    List< AutoPtr<ILinkAddress> >::Iterator iter1;
    for (iter1 = mLinkAddresses.Begin(); iter1 != mLinkAddresses.End(); ++iter1)  {
        dest->WriteInterfacePtr(*iter1);
    }

    dest->WriteInt32(mDnses.GetSize());
    List< AutoPtr<IInetAddress> >::Iterator iter2;
    for (iter2 = mDnses.Begin(); iter2 != mDnses.End(); ++iter2) {
        AutoPtr<IInetAddress> inetaddr = *iter2;
        AutoPtr< ArrayOf<Byte> > addresses;
        inetaddr->GetAddress((ArrayOf<Byte>**)&addresses);
        dest->WriteArrayOf((Handle32)addresses.Get());
    }

    dest->WriteInt32(mRoutes.GetSize());
    List< AutoPtr<IRouteInfo> >::Iterator iter3;
    for (iter3 = mRoutes.Begin(); iter3 != mRoutes.End(); ++iter3) {
        dest->WriteInterfacePtr(*iter3);
    }

    if (mHttpProxy != NULL) {
        dest->WriteByte(1);
        dest->WriteInterfacePtr(mHttpProxy);
    }
    else {
        dest->WriteByte(0);
    }
    return NOERROR;
}

ECode CLinkProperties::ReadFromParcel(
    /* [in] */ IParcel* source)
{
    source->ReadString(&mIfaceName);

    Int32 addressCount;
    source->ReadInt32(&addressCount);
    for (Int32 i = 0; i < addressCount; i++) {
        AutoPtr<ILinkAddress> linkaddr;
        source->ReadInterfacePtr((Handle32*)&linkaddr);
        AddLinkAddress(linkaddr);
    }

    AutoPtr<IInetAddressHelper> helper;
    CInetAddressHelper::AcquireSingleton((IInetAddressHelper**)&helper);
    source->ReadInt32(&addressCount);
    for (Int32 i = 0; i < addressCount; i++) {
        AutoPtr<ArrayOf<Byte> > address;
        source->ReadArrayOf((Handle32*)&address);

        AutoPtr<IInetAddress> inetaddr;
        helper->GetByAddress(address, (IInetAddress**)&inetaddr);
        AddDns(inetaddr);
    }

    source->ReadInt32(&addressCount);
    for (Int32 i = 0; i < addressCount; i++) {
        AutoPtr<IRouteInfo> routeinfo;
        source->ReadInterfacePtr((Handle32*)&routeinfo);
        AddRoute(routeinfo);
    }

    Byte by = 0;
    source->ReadByte(&by);
    if (by == 1) {
        source->ReadInterfacePtr((Handle32*)&mHttpProxy);
    }
    return NOERROR;
}

} // namespace Net
} // namepsace Droid
} // namespace Elastos
