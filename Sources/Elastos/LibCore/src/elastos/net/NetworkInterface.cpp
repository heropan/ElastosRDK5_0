
#include "NetworkInterface.h"
#include "CFile.h"
#include "IoUtils.h"
#include "CBufferedReader.h"
#include "CFileReader.h"
#include "StringBuilder.h"
#include "StringUtils.h"
#include "CInet4Address.h"
#include "CInet6Address.h"
#include "InterfaceAddress.h"
#include "CLibcore.h"
#include "CCollections.h"
#include "CArrayList.h"
#include "Arrays.h"

#include <net/if.h>
#include <sys/socket.h>
#include <linux/sockios.h>

using Elastos::Core::StringBuilder;
using Elastos::Core::StringUtils;
using Elastos::Core::CObjectContainer;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IoUtils;
using Elastos::IO::CBufferedReader;
using Elastos::IO::IBufferedReader;
using Elastos::IO::CFileReader;
using Elastos::IO::IFileReader;
using Elastos::IO::ICloseable;
using Libcore::IO::IOs;
using Libcore::IO::ILibcore;
using Libcore::IO::CLibcore;
using Elastos::Utility::ICollections;
using Elastos::Utility::CCollections;
using Elastos::Utility::IArrayList;
using Elastos::Utility::CArrayList;
using Elastos::Utility::Arrays;

namespace Elastos {
namespace Net {

CAR_INTERFACE_IMPL(NetworkInterface, Object, INetworkInterface)

NetworkInterface::NetworkInterface()
    : mInterfaceIndex(-1)
{
}

NetworkInterface::NetworkInterface(
    /* [in] */ const String& name,
    /* [in] */ Int32 interfaceIndex,
    /* [in] */ IList * addresses,
    /* [in] */ IList * interfaceAddresses)
    : mName(name)
    , mInterfaceIndex(interfaceIndex)
{
    mAddresses = addresses;
    mInterfaceAddresses = interfaceAddresses;
}

NetworkInterface::~NetworkInterface()
{
}

ECode NetworkInterface::constructor(
    /* [in] */ const String& name,
    /* [in] */ Int32 interfaceIndex,
    /* [in] */ IList * addresses,
    /* [in] */ IList * interfaceAddresses)
{
    mName = name;
    mInterfaceIndex = interfaceIndex;
    mAddresses = addresses;
    mInterfaceAddresses = interfaceAddresses;
}

AutoPtr<INetworkInterface> NetworkInterface::ForUnboundMulticastSocket()
{
    // This is what the RI returns for a MulticastSocket that hasn't been constrained
    // to a specific interface.
    AutoPtr< ArrayOf<IInterface*> > outarr = ArrayOf<IInterface*>::Alloc(1);
    outarr->Set(0, CInet6Address::ANY);
    AutoPtr<IList> outlist1;
    Arrays::AsList(outarr, (IList**)&outlist1);
    AutoPtr<IList> outlist2;
    CCollections::_GetEmptyList((IList**)&outlist2);
    return new NetworkInterface(String(NULL), -1, outlist1, outlist2);
}

ECode NetworkInterface::GetIndex(
    /* [out] */ Int32* index)
{
    VALIDATE_NOT_NULL(index);
    *index = mInterfaceIndex;
    return NOERROR;
}

ECode NetworkInterface::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode NetworkInterface::GetInetAddresses(
    /* [out] */ IEnumeration** addresses)
{
    VALIDATE_NOT_NULL(addresses);
    // return CCollections::_NewEnumeration(mAddresses, addresses);
}

ECode NetworkInterface::GetDisplayName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode NetworkInterface::Equals(
    /* [in] */  IInterface* obj,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);
    *result = FALSE;

    INetworkInterface* otherni = INetworkInterface::Probe(obj);
    if (otherni == NULL) {
        return NOERROR;
    }

    NetworkInterface* rhs = (NetworkInterface*)otherni;
    if(rhs == this) {
        *result = TRUE;
        return NOERROR;
    }

    *result = (rhs->mInterfaceIndex == mInterfaceIndex)
        && (rhs->mName == mName)
        && (rhs->mAddresses == mAddresses);
    return NOERROR;
}

ECode NetworkInterface::ToString(
    /* [out] */ String* str)
{
    VALIDATE_NOT_NULL(str);
    StringBuilder sb(25);
    sb.AppendChar('[');
    sb.Append(mName);
    sb.Append("][");
    sb.Append(mInterfaceIndex);
    sb.AppendChar(']');

    AutoPtr<IIterator> outiter;
    mAddresses->GetIterator((IIterator**)&outiter);
    Boolean hasNext;
    while (outiter->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> address;
        outiter->Next((IInterface**)&address);
        String sAddress = Object::ToString(address);
        sb.AppendChar(']');
        sb.Append(sAddress);
        sb.AppendChar(']');
    }
    *str = sb.ToString();
    return NOERROR;
}

ECode NetworkInterface::GetByName(
    /* [in] */  const String& interfaceName,
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);
    *networkInterface = NULL;

    if (interfaceName.IsNull()) {
        return E_NULL_POINTER_EXCEPTION;
    }

    if (!IsValidInterfaceName(interfaceName)) {
        return NOERROR;
    }

    StringBuilder sb("/sys/class/net/");
    sb.Append(interfaceName);
    sb.Append("/ifindex");
    Int32 interfaceIndex = ReadIntFile(sb.ToString());

    AutoPtr<IList> addresses;
    AutoPtr<IList> interfaceAddresses;
    // CArrayList::New((IArrayList**)&addresses);
    // CArrayList::New((IArrayList**)&interfaceAddresses);
    CollectIpv6Addresses(interfaceName, interfaceIndex, addresses, interfaceAddresses);
    CollectIpv4Addresses(interfaceName, addresses, interfaceAddresses);

    *networkInterface = (INetworkInterface*)(new NetworkInterface(interfaceName, interfaceIndex, addresses, interfaceAddresses));
    REFCOUNT_ADD(*networkInterface);
    return NOERROR;
}

ECode NetworkInterface::FindAddress(
    /* [in] */ IInetAddress* address,
    /* [out] */ Boolean* found)
{
    VALIDATE_NOT_NULL(found);
    AutoPtr<IIterator> em;
    mAddresses->GetIterator((IIterator**)&em);
    Boolean hasNext;
    Boolean equals;
    while (em->HasNext(&hasNext), hasNext) {
        AutoPtr<IInterface> add;
        em->Next((IInterface**)&add);
        if (Object::Equals(add, address))
            break;
    }
    *found = hasNext;
    return NOERROR;
}

ECode NetworkInterface::GetChildren(
    /* [out] */ IList** children)
{
    VALIDATE_NOT_NULL(children);
    *children = mChildren;
    REFCOUNT_ADD(*children);
    return NOERROR;
}

ECode NetworkInterface::GetAddresses(
    /* [out] */ IList** addresses)
{
    VALIDATE_NOT_NULL(addresses);
    *addresses = mAddresses;
    REFCOUNT_ADD(*addresses);
    return NOERROR;
}

ECode NetworkInterface::GetByInetAddress(
    /* [in] */ IInetAddress* address,
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);
    if(address == NULL) {
        return E_NULL_POINTER_EXCEPTION;
    }
    AutoPtr<IList> em = GetNetworkInterfacesList();
    AutoPtr<IIterator> outiter;
    em->GetIterator((IIterator**)&outiter);
    Boolean hasNext;
    while(outiter->HasNext(&hasNext), hasNext) {
        AutoPtr<INetworkInterface> net;
        outiter->Next((IInterface**)&net);
        Boolean found;
        AutoPtr<IList> outlist;
        net->GetAddresses((IList**)&outlist);
        if(outlist->Contains(address, &found), found) {
            *networkInterface = net;
            REFCOUNT_ADD(*networkInterface);
            return NOERROR;
        }
    }
    *networkInterface = NULL;
    return NOERROR;
}

AutoPtr<IList> NetworkInterface::GetNetworkInterfacesList()
{
    AutoPtr<IFile> file;
    AutoPtr<ArrayOf<String> > interfaceNames;
    CFile::New(String("/sys/class/net"),(IFile**)&file);
    file->List((ArrayOf<String>**)&interfaceNames);

    AutoPtr<ArrayOf<INetworkInterface*> > interfaces =
            ArrayOf<INetworkInterface*>::Alloc(interfaceNames->GetLength());

    AutoPtr<ArrayOf<Boolean> > done =
            ArrayOf<Boolean>::Alloc(interfaceNames->GetLength());

    for(Int32 i = 0; i < interfaceNames->GetLength(); ++i) {
        (*done)[i] = FALSE;
        AutoPtr<INetworkInterface> temp;
        GetByName((*interfaceNames)[i], (INetworkInterface**)&temp);
        interfaces->Set(i, temp);
        if((*interfaces)[i] == NULL)
            (*done)[i] = TRUE;
    }

    AutoPtr<IList> result;
    CArrayList::New((IArrayList**)&result);

    for(Int32 counter1 = 0; counter1 < interfaces->GetLength(); counter1++) {
        // If this interface has been dealt with already, continue.
        if ((*done)[counter1]) {
            continue;
        }

        Int32 counter2 = counter1;
        Boolean hasNext = FALSE;
        // Checks whether the following interfaces are children.
        for (; counter2 < interfaces->GetLength(); counter2++) {
            if ((*done)[counter2]) {
                continue;
            }

            String name1;
            String name2;
            (*interfaces)[counter1]->GetName(&name1);
            (*interfaces)[counter2]->GetName(&name2);

            if (name2.StartWith(name1 + ":") ) {
                AutoPtr<IList> children1;
                AutoPtr<INetworkInterface> parent2;
                AutoPtr<IList> addresses1;
                AutoPtr<IList> addresses2;
                (*interfaces)[counter1]->GetChildren((IList**)&children1);
                (*interfaces)[counter2]->GetParent((INetworkInterface**)&parent2);
                (*interfaces)[counter1]->GetAddresses((IList**)&addresses1);
                (*interfaces)[counter2]->GetAddresses((IList**)&addresses2);

                children1->Add((*interfaces)[counter2], &hasNext);
                parent2 = (*interfaces)[counter1];
                addresses1->AddAll(addresses2, &hasNext);
                (*done)[counter2] = TRUE;
              }
        }
        result->Add((*interfaces)[counter1], &hasNext);
        (*done)[counter1] = TRUE;
    }
    return result;

}

ECode NetworkInterface::GetNetworkInterfaces(
    /* [out] */ IEnumeration** enumerator)
{
    VALIDATE_NOT_NULL(enumerator);

    AutoPtr<IList> interfaces = GetNetworkInterfacesList();
    return CCollections::_NewEnumeration(interfaces, enumerator);
}

ECode NetworkInterface::GetByIndex(
    /* [in] */ Int32 index,
    /* [out] */ INetworkInterface** networkInterface)
{
    VALIDATE_NOT_NULL(networkInterface);
    char buf[IF_NAMESIZE];
    char* name = if_indextoname(index, buf);
    if(name == NULL) {
        *networkInterface = NULL;
        return NOERROR;
    }
    GetByName(String(name), networkInterface);
    return NOERROR;
}

ECode NetworkInterface::GetInterfaceAddresses(
    /* [out] */ IList** addresses)
{
    VALIDATE_NOT_NULL(addresses);
    *addresses = mInterfaceAddresses;
    REFCOUNT_ADD(*addresses)

    return NOERROR;
}

ECode NetworkInterface::GetSubInterfaces(
    /* [out] */ IEnumeration** subInterfaces)
{
    VALIDATE_NOT_NULL(subInterfaces);

    return CCollections::_NewEnumeration(mChildren, subInterfaces);
}

ECode NetworkInterface::GetParent(
    /* [out] */ INetworkInterface** parent)
{
    VALIDATE_NOT_NULL(parent);
    *parent = mParent;
    REFCOUNT_ADD(*parent);
    return NOERROR;
}

ECode NetworkInterface::IsUp(
    /* [out] */ Boolean* isUp)
{
    VALIDATE_NOT_NULL(isUp);
    *isUp = HasFlag(IFF_UP);
    return NOERROR;
}

ECode NetworkInterface::IsLoopback(
    /* [out] */ Boolean* isLoopback)
{
    VALIDATE_NOT_NULL(isLoopback);
    *isLoopback = HasFlag(IFF_LOOPBACK);
    return NOERROR;
}

ECode NetworkInterface::IsPointToPoint(
    /* [out] */ Boolean* isPointToPoint)
{
    VALIDATE_NOT_NULL(isPointToPoint);
    *isPointToPoint = HasFlag(IFF_POINTOPOINT);
    return NOERROR;
}

ECode NetworkInterface::SupportsMulticast(
    /* [out] */ Boolean* multicast)
{
    VALIDATE_NOT_NULL(multicast);
    *multicast = HasFlag(IFF_MULTICAST);
    return NOERROR;
}

ECode NetworkInterface::GetHardwareAddress(
    /* [out, callee] */ ArrayOf<Byte>** address)
{
    VALIDATE_NOT_NULL(address);
    String s;
    IoUtils::ReadFileAsString(String("/sys/class/net/") + mName + String("/address"), &s);
    AutoPtr<ArrayOf<Byte> > result = ArrayOf<Byte>::Alloc(s.GetLength()/3);
    for(Int32 i = 0; i < result->GetLength(); ++i)
    {
        (*result)[i] = (Byte)(StringUtils::ParseInt32(s.Substring(3*i, 3*i + 2), 16));
    }

    // We only want to return non-zero hardware addresses.
    for (int i = 0; i < result->GetLength(); ++i) {
        if ((*result)[i] != 0) {
            *address = result;
            REFCOUNT_ADD(*address);
            return NOERROR;
        }
    }
    *address = NULL;
    return NOERROR;
}

ECode NetworkInterface::GetMTU(
    /* [out] */ Int32* mtu)
{
    VALIDATE_NOT_NULL(mtu);
    ReadIntFile(String("/sys/class/net/") + mName + String("/mtu"));
    return NOERROR;
}

ECode NetworkInterface::IsVirtual(
    /* [out] */ Boolean* isvirtual)
{
    VALIDATE_NOT_NULL(isvirtual);
    *isvirtual = (mParent!= NULL);
    return NOERROR;
}

ECode NetworkInterface::CollectIpv6Addresses(
    /* [in] */ const String& interfaceName,
    /* [in] */ Int32 interfaceIndex,
    /* [in] */ IList* addresses,
    /* [in] */ IList* interfaceAddresses)
{
    AutoPtr<IBufferedReader> in;
    AutoPtr<IFileReader> fileReader;
    CFileReader::New(String("/proc/net/if_inet6"), (IFileReader**)&fileReader);
    CBufferedReader::New(fileReader, (IBufferedReader**)&in);
    String suffix = String(" ") + interfaceName;
    String line;
    while (in->ReadLine(&line), !line.IsNull()) {
        if (!line.EndWith(suffix)) {
            continue;
        }

        // Extract the IPv6 address.
        AutoPtr<ArrayOf<Byte> > addressBytes = ArrayOf<Byte>::Alloc(16);
        for (Int32 i = 0; i < addressBytes->GetLength(); ++i) {
            (*addressBytes)[i] = (Byte) (StringUtils::ParseInt32(line.Substring(2*i, 2*i + 2), 16));
        }

        // Extract the prefix length.
        // Skip the IPv6 address and its trailing space.
        Int32 prefixLengthStart = 32 + 1;
        // Skip the interface index and its trailing space.
        prefixLengthStart = line.IndexOf(' ', prefixLengthStart) + 1;
        Int32 prefixLengthEnd = line.IndexOf(' ', prefixLengthStart);
        Int32 prefixLength = StringUtils::ParseInt32(line.Substring(prefixLengthStart, prefixLengthEnd), 16);

        AutoPtr<IInet6Address> inet6Address;
        CInet6Address::New(addressBytes, String(NULL), interfaceIndex, (IInet6Address**)&inet6Address);
        Boolean isflag = FALSE;
        addresses->Add(inet6Address, &isflag);
        AutoPtr<InterfaceAddress> addres = new InterfaceAddress(inet6Address, prefixLength);
        interfaceAddresses->Add(addres, &isflag);
    }
    if(ICloseable::Probe(in)) {
        IoUtils::CloseQuietly(ICloseable::Probe(in));
    }
    return NOERROR;
}

ECode NetworkInterface::CollectIpv4Addresses(
    /* [in] */ const String& interfaceName,
    /* [in] */ IList* addresses,
    /* [in] */ IList* interfaceAddresses)
{
    Int32 fdInt;
    FAIL_RETURN(CLibcore::sOs->Socket(AF_INET, SOCK_DGRAM, 0, &fdInt));

    AutoPtr<IInetAddress> address;
    AutoPtr<IInetAddress> broadcast;
    AutoPtr<IInetAddress> netmask;

    FAIL_RETURN(CLibcore::sOs->IoctlInetAddress(fdInt, SIOCGIFADDR, interfaceName, (IInetAddress**)&address));
    FAIL_RETURN(CLibcore::sOs->IoctlInetAddress(fdInt, SIOCGIFBRDADDR, interfaceName, (IInetAddress**)&broadcast));
    FAIL_RETURN(CLibcore::sOs->IoctlInetAddress(fdInt, SIOCGIFNETMASK, interfaceName, (IInetAddress**)&netmask));
    Boolean isEquals;
    broadcast->Equals(CInet4Address::ANY, &isEquals);
    if(isEquals) {
        broadcast = NULL;
    }
    Boolean isflag = FALSE;
    addresses->Add(address, &isflag);
    AutoPtr<InterfaceAddress> ia = new InterfaceAddress(address, broadcast, netmask);
    interfaceAddresses->Add(ia, &isflag);
    return NOERROR;
}

Boolean NetworkInterface::IsValidInterfaceName(
    /* [in] */ const String& interfaceName)
{
    AutoPtr<IFile> file;
    CFile::New(String("/sys/class/net"), (IFile**)&file);
    AutoPtr<ArrayOf<String> > files;
    file->List((ArrayOf<String>**)&files);
    for(Int32 i = 0; i < files->GetLength(); ++i) {
        if((*files)[i] == interfaceName)
            return TRUE;
    }
    return FALSE;
}

Int32 NetworkInterface::ReadIntFile(
    /* [in] */ const String& path)
{
    String s;
    FAIL_RETURN(IoUtils::ReadFileAsString(path, &s);)
    s= s.Trim();
    if(s.StartWith(String("0x"))) {
        return StringUtils::ParseInt32(s.Substring(2), 16);
    }
    else {
        return StringUtils::ParseInt32(s);
    }
}

Boolean NetworkInterface::HasFlag(
    /* [in] */ Int32 mask)
{
    Int32 flags = ReadIntFile(String("/sys/class/net/") + mName + String("/flags"));
    return (flags & mask) != 0;
}

} // namespace Net
} // namespace Elastos
