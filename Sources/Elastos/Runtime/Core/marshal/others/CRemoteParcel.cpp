
#include <dbus/dbus.h>
#include "CRemoteParcel.h"

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo **ppClassInfo);

ECode GetRemoteClassInfo(
    /* [in] */ const char* connectionName,
    /* [in] */ REMuid clsId,
    /* [out] */ CIClassInfo ** ppClassInfo);

#define ROUND4(n)       (((n)+3)&~3)   // round up to multiple of 4 bytes
#define ROUND8(n)       (((n)+7)&~7)   // round up to multiple of 8 bytes

enum Type {
    Type_Unknown = 0,

    Type_Byte, Type_Boolean, Type_Char32,
    Type_Int16, Type_Int32, Type_Int64, Type_Float, Type_Double,
    Type_String, Type_Struct, Type_EMuid, Type_EGuid,
    Type_ArrayOf, Type_ArrayOfString, Type_InterfacePtr,
};

CRemoteParcel::CRemoteParcel() :
    m_nRefs(1),
	m_freeDataTag(FALSE)
{
    m_elemBufCapacity = 1024;
    m_elemBuf = (UInt32*)calloc(1, m_elemBufCapacity + sizeof(MarshalHeader));
    m_elemBuf = (UInt32*)((MarshalHeader*)m_elemBuf + 1);
    m_elemPtr = m_elemBuf;
}

CRemoteParcel::CRemoteParcel(
    /* [in] */ UInt32 *elemBuf) :
    m_nRefs(1),
	m_freeDataTag(FALSE)
{
    m_elemBuf = (UInt32*)((MarshalHeader*)elemBuf + 1);
    m_elemPtr = m_elemBuf;
}

CRemoteParcel::~CRemoteParcel()
{
    if (m_freeDataTag) {
        free(m_elemBuf);
    }
}

PInterface CRemoteParcel::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IParcel) {
        return (IParcel*)this;
    }

    return NULL;
}

UInt32 CRemoteParcel::AddRef()
{
    Int32 nRefs = atomic_inc(&m_nRefs);

    return (UInt32)nRefs;
}

UInt32 CRemoteParcel::Release()
{
    Int32 nRefs = atomic_dec(&m_nRefs);

    if (nRefs == 0) {
        this->~CRemoteParcel();
        free(this);
    }

    return (UInt32)nRefs;
}

ECode CRemoteParcel::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    if (NULL == pIID) return E_INVALID_ARGUMENT;

    if (pObject == (IInterface *)(IParcel *)this) {
        *pIID = EIID_IParcel;
    }
    else {
        return E_INVALID_ARGUMENT;
    }
    return NOERROR;
}

ECode CRemoteParcel::Marshall(
    /* [out, callee] */ ArrayOf<Byte>** bytes)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::Unmarshall(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::GrowElemBuffer(
    /* [in] */ Int32 minSize)
{
    UInt32* orgElemBuf;
    Int32 offset;

    orgElemBuf = (UInt32*)((MarshalHeader*)m_elemBuf - 1);
    offset = (Byte*)m_elemPtr - (Byte*)orgElemBuf;

    m_elemBufCapacity += (minSize > 512)? minSize + 512 : 512;
    m_elemBuf = (UInt32*)calloc(1, m_elemBufCapacity + sizeof(MarshalHeader));
    if (NULL == m_elemBuf) return E_OUT_OF_MEMORY;
    memcpy(m_elemBuf, orgElemBuf, offset);

    m_elemPtr = (UInt32*)((Byte*)m_elemBuf + offset);
    m_elemBuf = (UInt32*)((MarshalHeader*)m_elemBuf + 1);

    free(orgElemBuf);

    return NOERROR;
}

ECode CRemoteParcel::AppendFrom(
    /* [in] */ IParcel* parcel,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadValue(PVoid pValue, Int32 type)
{
    switch(type) {
        case Type_Byte:
            *(Byte*)pValue = *(Byte*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_Boolean:
            *(Boolean*)pValue = *(Boolean*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_Char16:
            *(Char16*)pValue = *(Char16*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_Char32:
            *(Char32*)pValue = *(Char32*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_Int16:
            *(Int16*)pValue = *(Int16*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_Int32:
            *(Int32*)pValue = *(Int32*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_Int64:
        case Type_Double:
#if defined(_arm) && defined(__GNUC__) && (__GNUC__ >= 4)
            m_elemPtr = (UInt32*)ROUND8((Int32)m_elemPtr);
#endif
            *(Int64*)pValue = *(Int32*)m_elemPtr;
            m_elemPtr++;
            *(Int64*)pValue = *(Int64*)pValue | ((Int64)(*(Int32*)m_elemPtr) << 32);
            m_elemPtr++;
            break;

        case Type_Float:
            *(Float*)pValue = *(Float*)m_elemPtr;
            m_elemPtr++;
            break;

        case Type_String:
            if (*m_elemPtr == MSH_NOT_NULL) {
                m_elemPtr++;

                Int32 len = (Int32)*m_elemPtr;
                m_elemPtr++;

                *(String*)pValue = (char*)m_elemPtr;
                assert(len == (Int32) MSH_ALIGN_4(strlen((char*)m_elemPtr) + 1));
                m_elemPtr += len / 4;
            }
            else {
                assert(*m_elemPtr == MSH_NULL);
                m_elemPtr++;
                *(String*)pValue = NULL;
            }
            break;

        case Type_Struct:
            {
                Int32 len = (Int32)*m_elemPtr;
                m_elemPtr++;

                memcpy(pValue, m_elemPtr, len);
                m_elemPtr += len / 4;
            }
            break;

        case Type_EMuid:
            *(EMuid*)pValue = *(EMuid*)m_elemPtr;
            m_elemPtr += sizeof(EMuid) / 4;

            break;

        case Type_EGuid:
            {
                *(EGuid*)pValue = *(EGuid*)m_elemPtr;
                m_elemPtr += sizeof(EGuid) / 4;
                m_elemPtr++;

                Int32 len = (Int32)*m_elemPtr;
                m_elemPtr++;

                (*(EGuid*)pValue).pUunm = (char*)malloc(len + 1);
                strcpy((*(EGuid*)pValue).pUunm, (char*)m_elemPtr);
                m_elemPtr += len / 4;
            }
            break;

        case Type_ArrayOf:
            memcpy(((PCARQUINTET)(UInt32*)pValue)->m_pBuf, \
                    (Byte *)m_elemPtr + sizeof(CarQuintet),
                    ((PCARQUINTET)m_elemPtr)->m_used);

            ((PCARQUINTET)(UInt32*)pValue)->m_used = \
                    ((PCARQUINTET)m_elemPtr)->m_used;

            m_elemPtr = (UInt32 *)((Byte *)m_elemPtr +
                    MSH_ALIGN_4(sizeof(CarQuintet)\
                    + ((PCARQUINTET)(UInt32 *)pValue)->m_size));
            break;

        case Type_InterfacePtr:
            if (*m_elemPtr != MSH_NULL) {
                assert(MSH_NOT_NULL == *m_elemPtr);
                m_elemPtr++;

                InterfacePack *pItfPack = NULL;
                CIClassInfo *pClassInfo = NULL;
                IParcelable *pParcelable = NULL;
                ClassID classId;
                InterfaceID iid;
                ECode ec;

                pItfPack = (InterfacePack*)m_elemPtr;
                m_elemPtr = (UInt32*)((InterfacePack*)m_elemPtr + 1);
                if (IsParcelable(pItfPack, &pClassInfo)) {
                    classId.clsid = pItfPack->m_clsid;
                    classId.pUunm = pClassInfo->pszUunm;

                    ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
                            EIID_IParcelable, (IInterface**)&pParcelable);
                    if (FAILED(ec)) return ec;

                    pParcelable->ReadFromParcel(this);
                    iid = pClassInfo->interfaces[pItfPack->m_uIndex]->iid;
                    *(IInterface**)pValue = pParcelable->Probe(iid);
                }
                else {
                    ec = StdUnmarshalInterface(
                            UnmarshalFlag_Noncoexisting,
                            pItfPack,
                            (IInterface **)pValue);
                    if (FAILED(ec)) {
                        MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                "MshProc: unmsh interface, ec = %x\n", ec));
                        return ec;
                    }
                }
            }
            else {
                *(IInterface**)pValue = NULL;
                m_elemPtr++;
            }
            break;

        default:
            assert(0);
            break;
    }

    return NOERROR;
}

ECode CRemoteParcel::WriteValue(PVoid pValue, Int32 type, Int32 size)
{
    ECode ec;

    if ((Byte*)m_elemPtr - (Byte*)m_elemBuf + size > m_elemBufCapacity) {
        ec = GrowElemBuffer(size);
        if (FAILED(ec)) return ec;
    }

    switch(type) {
        case Type_Byte:
        case Type_Boolean:
            *m_elemPtr = *((Byte*)pValue);
            m_elemPtr++;

            break;

        case Type_Char16:
        case Type_Int16:
            *m_elemPtr = *((Int16*)pValue);
            m_elemPtr++;
            break;

        case Type_Char32:
        case Type_Int32:
        case Type_Float:
            *m_elemPtr = *(Int32*)pValue;
            m_elemPtr++;
            break;

        case Type_Int64:
        case Type_Double:
#if defined(_arm) && defined(__GNUC__) && (__GNUC__ >= 4)
            m_elemPtr = (UInt32*)ROUND8((Int32)m_elemPtr);
#endif
            *m_elemPtr = (Int32)(*((Int64*)pValue) & 0xffffffff);
            m_elemPtr++;
            *m_elemPtr = (Int32)((*((Int64*)pValue) >> 32) & 0xffffffff);
            m_elemPtr++;
            break;

        case Type_String:
            *m_elemPtr = pValue ? MSH_NOT_NULL : MSH_NULL;
            m_elemPtr++;

            if (pValue) {
                Int32 len = size - sizeof(UInt32) * 2;
                *m_elemPtr = len;
                m_elemPtr++;

                memcpy(m_elemPtr, (const void *)pValue, len);
                m_elemPtr = (UInt32 *)((Byte *)m_elemPtr + len);
            }
            break;

        case Type_InterfacePtr:
            *m_elemPtr = pValue ? MSH_NOT_NULL : MSH_NULL;
            m_elemPtr++;

            if (pValue) {
                ec = StdMarshalInterface(
                        (IInterface*)pValue,
                        (InterfacePack*)m_elemPtr);
                if (FAILED(ec)) return ec;

                m_elemPtr = (UInt32 *)((Byte *)m_elemPtr + sizeof(InterfacePack));

                IParcelable *pParcelable = \
                        (IParcelable*)((IInterface*)pValue)->Probe(EIID_IParcelable);
                if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            }
            break;

        case Type_Struct:
            *m_elemPtr = pValue ? MSH_NOT_NULL : MSH_NULL;
            m_elemPtr++;

            if (pValue) {
                *m_elemPtr = size;
                m_elemPtr++;

                memcpy((PByte)m_elemPtr, (PByte)pValue, size);
                m_elemPtr += size / 4;
            }

            break;

        case Type_EMuid:
            *(EMuid *)m_elemPtr = *(EMuid *)pValue;
            m_elemPtr += sizeof(EMuid) / 4;
            break;

        case Type_EGuid:
            {
                *(EGuid *)m_elemPtr = *(EGuid *)pValue;
                m_elemPtr += sizeof(EGuid) / 4;
                *m_elemPtr = MSH_NOT_NULL;
                m_elemPtr++;

                Int32 len = MSH_ALIGN_4(strlen(((EGuid *)pValue)->pUunm) + 1);
                *m_elemPtr = len;
                m_elemPtr++;

                strcpy((char*)m_elemPtr, ((EGuid *)pValue)->pUunm);
                m_elemPtr += len / 4;
            }
            break;

        case Type_ArrayOf:
            *m_elemPtr = pValue ? MSH_NOT_NULL : MSH_NULL;
            m_elemPtr++;

            if (pValue) {
                memcpy(m_elemPtr, (Byte *)pValue, sizeof(CarQuintet)); // copy head
                m_elemPtr = (UInt32 *)((Byte *)m_elemPtr + sizeof(CarQuintet));

                if (CarQuintetFlag_Type_IObject
                    != (((PCARQUINTET)pValue)->m_flags
                            & CarQuintetFlag_TypeMask)) {
                    // copy the storaged data
                    //
                    memcpy(m_elemPtr, ((PCARQUINTET)pValue)->m_pBuf,
                            ((PCARQUINTET)pValue)->m_size);

                    m_elemPtr = (UInt32 *)((Byte *)m_elemPtr +
                            MSH_ALIGN_4(((PCARQUINTET)pValue)->m_size));
                }
                else {
                    Int32 used = ((PCARQUINTET)pValue)->m_used
                                / sizeof(IInterface *);
                    Int32 *pBuf = (Int32*)((PCARQUINTET)pValue)->m_pBuf;
                    PCARQUINTET carquient = (PCARQUINTET)m_elemPtr;
                    for (Int32 i = 0; i < used; i++) {
                        if (pBuf[i]) {
                            ec = StdMarshalInterface(
                                    (IInterface *)pBuf[i],
                                    (InterfacePack *)m_elemPtr);
                            if (FAILED(ec)) {
                                MARSHAL_DBGOUT(MSHDBG_ERROR, printf(
                                        "MshProc: marshal interface, ec = %x\n", ec));
                                return ec;
                            }

                            m_elemPtr = (UInt32 *)((Byte *)m_elemPtr +
                                    sizeof(InterfacePack));

                            IParcelable *pParcelable = \
                                    (IParcelable*)(*(IInterface**)pValue)->Probe(EIID_IParcelable);
                            if (pParcelable != NULL) pParcelable->WriteToParcel(this);
                        }
                        else {  // null pointer
                            *m_elemPtr = MSH_NULL;
                            m_elemPtr++;
                        }
                    }
                    carquient = (PCARQUINTET)((Byte *)carquient +
                            MSH_ALIGN_4(((PCARQUINTET)pValue)->m_size));
                    m_elemPtr = (UInt32 *)MAX(m_elemPtr, (UInt32*)carquient);
                }
            }
            break;

        case Type_ArrayOfString:
        default:
            assert(0);
            break;
    }

    return NOERROR;
}

ECode CRemoteParcel::ReadByte(
    /* [out] */ Byte *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Byte);
}

ECode CRemoteParcel::ReadBoolean(
    /* [out] */ Boolean *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Boolean);
}

ECode CRemoteParcel::ReadChar(
    /* [out] */ Char32 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Char32);
}

ECode CRemoteParcel::ReadInt16(
    /* [out] */ Int16 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Int16);
}

ECode CRemoteParcel::ReadInt32(
    /* [out] */ Int32 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Int32);
}

ECode CRemoteParcel::ReadInt64(
    /* [out] */ Int64 *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Int64);
}

ECode CRemoteParcel::ReadFloat(
    /* [out] */ Float *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Float);
}

ECode CRemoteParcel::ReadDouble(
    /* [out] */ Double *pValue)
{
    if (pValue == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pValue, Type_Double);
}

ECode CRemoteParcel::ReadString(
    /* [out] */ String* pStr)
{
    if (pStr == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pStr, Type_String);
}

ECode CRemoteParcel::ReadStruct(
    /* [out] */ Handle32 *paddr)
{
    return ReadValue((PVoid)paddr, Type_Struct);
}

ECode CRemoteParcel::ReadEMuid(EMuid *pId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadEGuid(EGuid *pId)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadInterfacePtr(
    /* [out] */ Handle32 *pItfPtr)
{
    assert(pItfPtr != NULL);

    return ReadValue((PVoid)pItfPtr, Type_InterfacePtr);
}

ECode CRemoteParcel::ReadArrayOf(
    /* [out] */ Handle32 *ppArray)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadArrayOfString(
    /* [out, callee] */ ArrayOf<String>** ppArray)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadFileDescriptor(
    /* [out] */ Int32* fd)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteByte(Byte value)
{
    return WriteValue((PVoid)&value, Type_Byte, 4);
}

ECode CRemoteParcel::WriteBoolean(
    /* [in] */ Boolean value)
{
    return WriteValue((PVoid)&value, Type_Boolean, 4);
}

ECode CRemoteParcel::WriteChar(
    /* [in] */ Char32 value)
{
    return WriteValue((PVoid)&value, Type_Char32, 4);
}

ECode CRemoteParcel::WriteInt16(Int16 value)
{
    return WriteValue((PVoid)&value, Type_Int16, 4);
}

ECode CRemoteParcel::WriteInt32(Int32 value)
{
    return WriteValue((PVoid)&value, Type_Int32, 4);
}

ECode CRemoteParcel::WriteInt64(
    /* [in] */ Int64 value)
{
    return WriteValue((PVoid)&value, Type_Int64, 8);
}

ECode CRemoteParcel::WriteFloat(
    /* [in] */ Float value)
{
    return WriteValue((PVoid)&value, Type_Float, 4);
}

ECode CRemoteParcel::WriteDouble(
    /* [in] */ Double value)
{
    return WriteValue((PVoid)&value, Type_Double, 8);
}

ECode CRemoteParcel::WriteString(const String& str)
{
    Int32 size = sizeof(UInt32);

    if (!str.IsNull()) {
        size += MSH_ALIGN_4(strlen((const char*)(str)) + 1) + sizeof(UInt32);
    }

    return WriteValue((PVoid)(const char*)(str), Type_String, size);
}

ECode CRemoteParcel::WriteInterfacePtr(IInterface* pValue)
{
    return WriteValue((PVoid)pValue, Type_InterfacePtr, sizeof(UInt32) + sizeof(InterfacePack));
}

ECode CRemoteParcel::WriteStruct(Handle32 pValue, Int32 size)
{
    return WriteValue((PVoid)pValue, Type_Struct, size + sizeof(UInt32));
}

ECode CRemoteParcel::WriteEMuid(const EMuid& id)
{
    return WriteValue((PVoid)&id, Type_EMuid, sizeof(EMuid));
}

ECode CRemoteParcel::WriteEGuid(const EGuid& id)
{
    return WriteValue((PVoid)&id, Type_EGuid, sizeof(EGuid) +
                      MSH_ALIGN_4(strlen(id.pUunm) + 1) + sizeof(UInt32) * 2);
}

ECode CRemoteParcel::WriteArrayOf(
    /* [in] */ Handle32 pArray)
{
    Int32 size = sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)pArray)->m_size;
    return WriteValue((PVoid)pArray, Type_ArrayOf, size);
}

ECode CRemoteParcel::WriteArrayOfString(
    /* [in] */ ArrayOf<String>* array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteFileDescriptor(
    /* [in] */ Int32 fd)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteDupFileDescriptor(
    /* [in] */ Int32 fd)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::Clone(
    /* [in] */ IParcel* pSrcParcel)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::GetDataPosition(
    /* [out] */ Int32* position)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::SetDataPosition(
    /* [in] */ Int32 position)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::GetElementPayload(
    /* [out] */ Handle32* pBuffer)
{
    *pBuffer = (Handle32)((MarshalHeader*)m_elemBuf - 1);

    return NOERROR;
}

ECode CRemoteParcel::GetElementSize(
    /* [in] */ Int32* pSize)
{
    *pSize = (Int32)((Byte*)m_elemPtr - (Byte*)m_elemBuf + sizeof(MarshalHeader));

    return NOERROR;
}

MarshalHeader* CRemoteParcel::GetMarshalHeader()
{
    return ((MarshalHeader*)m_elemBuf - 1);
}

Boolean CRemoteParcel::IsParcelable(
    /* [in] */ InterfacePack *pInterfacePack,
    /* [out] */ CIClassInfo **ppClassInfo)
{
    DBusError err;
    DBusConnection *pconn = NULL;
    DBusMessage *pmsg = NULL;
    CIInterfaceInfo *pInterfaceInfo = NULL;
    ECode ec;

    ec = LookupClassInfo(pInterfacePack->m_clsid, ppClassInfo);
    if (FAILED(ec)) {
        ec = GetRemoteClassInfo(pInterfacePack->m_stubConnName,
                                pInterfacePack->m_clsid,
                                ppClassInfo);
        if (FAILED(ec)) return FALSE;
    }

    for (UInt16 i = 0; i < (*ppClassInfo)->interfaceNum; i++) {
        pInterfaceInfo = (*ppClassInfo)->interfaces[i];
        if (pInterfaceInfo->iid == EIID_IParcelable) {
            // initialiset the errors
            dbus_error_init(&err);

            // connect to the system bus and check for errors
#ifdef _MSC_VER
			pconn = dbus_bus_get_private(DBUS_BUS_SESSION, &err);
#else
    		pconn = dbus_bus_get_private(DBUS_BUS_SYSTEM, &err);
#endif
            if (dbus_error_is_set(&err)) {
                MARSHAL_DBGOUT(MSHDBG_ERROR,
                        printf("Connection Error (%s).\n", err.message));
                dbus_error_free(&err);
                goto Exit;
            }
            // create a new method call and check for errors
            pmsg = dbus_message_new_method_call(
                    pInterfacePack->m_stubConnName, // target for the method call
                    STUB_OBJECT_DBUS_OBJECT_PATH, // object to call on
                    STUB_OBJECT_DBUS_INTERFACE, // interface to call on
                    "Release");  // method name
            if (pmsg == NULL) {
                MARSHAL_DBGOUT(MSHDBG_ERROR,
                        printf("Message Null.\n"));
                goto Exit;
            }

            if (dbus_connection_send(pconn, pmsg, NULL) == FALSE) {
                MARSHAL_DBGOUT(MSHDBG_ERROR,
                        printf("Send error.\n"));
                goto Exit;
            }
            dbus_connection_flush(pconn);


Exit:
            if (pmsg != NULL) dbus_message_unref(pmsg);

            // free connection
            if (pconn != NULL) {
                dbus_connection_close(pconn);
                dbus_connection_unref(pconn);
            }
            return TRUE;
        }
    }

    return FALSE;
}

ELAPI _CParcel_New(
    /* [out] */ IParcel **ppObj)
{
    if (ppObj == NULL) return E_INVALID_ARGUMENT;
    CRemoteParcel *pObj = NULL;
    void* pLocation = calloc(sizeof(CRemoteParcel), 1);
    if (!pLocation) return E_OUT_OF_MEMORY;

    pObj = (CRemoteParcel *)new(pLocation) CRemoteParcel(FALSE);
    pObj->AddRef();

    *ppObj = (IParcel*)pObj;

    return NOERROR;
}
