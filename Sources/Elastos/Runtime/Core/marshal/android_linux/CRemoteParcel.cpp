
#include <stdio.h>
#include "CRemoteParcel.h"

#define E_INVALID_PARCEL_DATA 0x81ff0000

ECode LookupClassInfo(
    /* [in] */ REMuid rclsid,
    /* [out] */ CIClassInfo **ppClassInfo);

namespace Elastos {
namespace IPC {

ECode GetRemoteClassInfo(
    /* [in] */ android::IBinder* pRemoteObj,
    /* [in] */ REMuid clsId,
    /* [out] */ CIClassInfo ** ppClassInfo);

#define ROUND4(n)       (((n)+3)&~3)   // round up to multiple of 4 bytes
#define ROUND8(n)       (((n)+7)&~7)   // round up to multiple of 8 bytes

enum Type {
    Type_Unknown            = 0,
    Type_Byte               = 1,
    Type_Boolean            = 2,
    Type_Char32             = 3,
    Type_Int16              = 4,
    Type_Int32              = 5,
    Type_Int64              = 6,
    Type_Float              = 7,
    Type_Double             = 8,
    Type_CString            = 9,
    Type_String             = 10,
    Type_Struct             = 11,
    Type_EMuid              = 12,
    Type_EGuid              = 13,
    Type_ArrayOf            = 14,
    Type_ArrayOfCString     = 15,
    Type_ArrayOfString      = 16,
    Type_InterfacePtr       = 17,
    Type_Fd                 = 18,
    Type_Fd_Dup             = 19
};

CRemoteParcel::CRemoteParcel(
    /* [in] */ Boolean writeMarshalHeader)
    : m_pData(new android::Parcel())
    , m_freeDataTag(TRUE)
{
    if (writeMarshalHeader) m_pData->writeInplace(sizeof(MarshalHeader));
}

CRemoteParcel::CRemoteParcel(
    /* [in] */ android::Parcel *pData)
    : m_pData(pData)
    , m_freeDataTag(FALSE)
{
//    m_pData->writeInplace(sizeof(MarshalHeader));
}

CRemoteParcel::~CRemoteParcel()
{
    if (m_freeDataTag) {
        delete m_pData;
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
    return ElRefBase::AddRef();
}

UInt32 CRemoteParcel::Release()
{
    return ElRefBase::Release();
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
    if (NULL == bytes) return E_INVALID_ARGUMENT;
    *bytes = NULL;

    // do not marshall if there are binder objects in the parcel
    if (m_pData->objectsCount()) {
        ALOGD("RuntimeException Tried to marshall a Parcel that contained Binder objects.");
        return E_NOT_SUPPORTED;
    }

    ArrayOf<Byte>* array = ArrayOf<Byte>::Alloc(m_pData->dataSize());
    memcpy(array->GetPayload(), m_pData->data(), m_pData->dataSize());
    *bytes = array;
    (*bytes)->AddRef();

    return NOERROR;
}

ECode CRemoteParcel::Unmarshall(
    /* [in] */ ArrayOf<Byte>* data,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    if (data == NULL || length < 0 || length > data->GetLength()
            || offset < 0 || offset > data->GetLength() || (data->GetLength() - offset) < length) {
       return NOERROR;
    }

    m_pData->setDataSize(length);
    m_pData->setDataPosition(0);

    void* raw = m_pData->writeInplace(length);
    memcpy(raw, (data->GetPayload() + offset), length);

    return NOERROR;
}

ECode CRemoteParcel::GrowElemBuffer(
    /* [in] */ Int32 minSize)
{
    return NOERROR;
}

ECode CRemoteParcel::AppendFrom(
    /* [in] */ IParcel* parcel,
    /* [in] */ Int32 offset,
    /* [in] */ Int32 length)
{
    android::Parcel* parcelObj;
    parcel->GetElementPayload((Handle32*)&parcelObj);
    if (parcelObj == NULL) return E_INVALID_ARGUMENT;
    m_pData->appendFrom(parcelObj, offset, length);
    return NOERROR;
}

ECode CRemoteParcel::HasFileDescriptors(
    /* [out] */ Boolean* result)
{
    if (NULL == result) return E_INVALID_ARGUMENT;
    *result = m_pData->hasFileDescriptors();
    return NOERROR;
}

ECode CRemoteParcel::ReadValue(PVoid pValue, Int32 type)
{
    switch(type) {
        case Type_Byte:
            *(Byte*)pValue = (Byte)m_pData->readInt32();
            break;

        case Type_Boolean:
            *(Boolean*)pValue = (Boolean)m_pData->readInt32();
            break;

        case Type_Char32:
            *(Char32*)pValue = (Char32)m_pData->readInt32();
            break;

        case Type_Int16:
            *(Int16*)pValue = (Int16)m_pData->readInt32();
            break;

        case Type_Int32:
            *(Int32*)pValue = (Int32)m_pData->readInt32();
            break;

        case Type_Int64:
        case Type_Double:
            *(Int64*)pValue = (Int64)m_pData->readInt64();
            break;

        case Type_Float:
            *(Float*)pValue = (Float)m_pData->readFloat();
            break;

        case Type_CString:
            {
                Int32 tag = m_pData->readInt32();
                if (tag != MSH_NULL) {
                    const char* str = m_pData->readCString();
                    *(CString*)pValue = CString::Duplicate(str);
                }
                else {
                    *(CString*)pValue = NULL;
                }
            }
            break;

        case Type_String:
            {
                Int32 tag = m_pData->readInt32();
                if (tag != MSH_NULL) {
                    const char* str = m_pData->readCString();
                    *(String*)pValue = str;
                }
                else {
                    *(String*)pValue = NULL;
                }
            }
            break;

        case Type_Struct:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    Int32 len = (Int32)m_pData->readInt32();
                    m_pData->read(pValue, len);
                }
                else {
                    pValue = NULL;
                }
            }
            break;

        case Type_EMuid:
            m_pData->read(pValue, sizeof(EMuid));

            break;

        case Type_EGuid:
            {
                const char* str;

                m_pData->read(pValue, sizeof(EGuid));
                str = m_pData->readCString();
                (*(EGuid*)pValue).pUunm = (char*)malloc(strlen(str) + 1);
                strcpy((*(EGuid*)pValue).pUunm, str);
            }
            break;

        case Type_ArrayOf:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    CarQuintet q;
                    m_pData->read((void*)&q, sizeof(CarQuintet));
                    Int32 size = q.m_size;
                    PCarQuintet qq = _ArrayOf_Alloc(size, q.m_flags);
                    if (qq == NULL) {
                        *(PCARQUINTET*)pValue = NULL;
                        break;
                    }
                    _CarQuintet_AddRef(qq);
                    *(PCARQUINTET*)pValue = qq;
                    if (size != 0) {
                        if (CarQuintetFlag_Type_IObject
                            != (q.m_flags & CarQuintetFlag_TypeMask)) {
                            if (CarQuintetFlag_Type_String
                                == (q.m_flags & CarQuintetFlag_TypeMask)) {
                                ArrayOf<String>* strArr = (ArrayOf<String>*)(*(PCARQUINTET*)pValue);
                                for (Int32 i = 0; i < size / sizeof(String); i++) {
                                    Int32 tag = m_pData->readInt32();
                                    if (tag != MSH_NULL) {
                                        const char* str = m_pData->readCString();
                                        (*strArr)[i] = str;
                                    }
                                }
                            }
                            else {
                                m_pData->read((void*)qq->m_pBuf, size);
                            }
                        }
                        else {
                            IInterface** pBuf = (IInterface**)qq->m_pBuf;
                            size = size / sizeof(IInterface *);
                            for (int i = 0; i < size; i++) {
                                tag = (Int32)m_pData->readInt32();
                                if (tag != MSH_NULL) {
                                    InterfacePack ipack;
                                    CIClassInfo *pClassInfo = NULL;
                                    IParcelable *pParcelable = NULL;
                                    ClassID classId;
                                    InterfaceID iid;
                                    ECode ec;

                                    android::sp<android::IBinder> binder = m_pData->readStrongBinder();
                                    m_pData->read((void *)&ipack, sizeof(InterfacePack));
                                    assert(ipack.m_pBinder == NULL);
                                    ipack.m_pBinder = binder;
                                    if (IsParcelable(&ipack, &pClassInfo)) {
                                        classId.clsid = ipack.m_clsid;
                                        classId.pUunm = pClassInfo->pszUunm;

                                        ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
                                                EIID_IParcelable, (IInterface**)&pParcelable);
                                        if (FAILED(ec)) return ec;

                                        pParcelable->ReadFromParcel(this);
                                        iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
                                        *((IInterface**)pBuf + i) = pParcelable->Probe(iid);
                                    }
                                    else {
                                        ec = StdUnmarshalInterface(
                                                UnmarshalFlag_Noncoexisting,
                                                &ipack,
                                                (IInterface **)&pBuf[i]);
                                        if (FAILED(ec)) {
                                            MARSHAL_DBGOUT(MSHDBG_ERROR, ALOGE(
                                                    "MshProc: unmsh interface, ec = %x\n", ec));
                                            return ec;
                                        }
                                    }
                                }
                                else {
                                    pBuf[i] = NULL;
                                }
                            }
                        }
                    }
                }
                else {
                    *(UInt32*)pValue = NULL;
                }
            }
            break;

        case Type_ArrayOfString:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    CarQuintet buf;

                    m_pData->read((void*)&buf, sizeof(buf));
                    Int32 size = buf.m_size / sizeof(String);
                    ArrayOf<String>* strArray = ArrayOf<String>::Alloc(size);
                    for (Int32 i = 0; i < size; i++) {
                        Int32 tag = m_pData->readInt32();
                        if (tag != MSH_NULL) {
                            const char* str = m_pData->readCString();
                            (*strArray)[i] = str;
                        }
                    }
                    *(ArrayOf<String>**)pValue = strArray;
                    strArray->AddRef();
                }
            }
            break;

        case Type_InterfacePtr:
            {
                Int32 tag = (Int32)m_pData->readInt32();
                if (tag != MSH_NULL) {
                    InterfacePack ipack;
                    CIClassInfo *pClassInfo = NULL;
                    IParcelable *pParcelable = NULL;
                    ClassID classId;
                    InterfaceID iid;
                    ECode ec;

                    android::sp<android::IBinder> binder = m_pData->readStrongBinder();
                    (InterfacePack *)m_pData->read((void *)&ipack, sizeof(InterfacePack));
                    if (ipack.m_pBinder != NULL) {
                        memset(&ipack, 0, sizeof(InterfacePack));
                        return E_INVALID_PARCEL_DATA;
                    }
                    ipack.m_pBinder = binder;
                    if (IsParcelable(&ipack, &pClassInfo)) {
                        classId.clsid = ipack.m_clsid;
                        classId.pUunm = pClassInfo->pszUunm;

                        ec = _CObject_CreateInstance(classId, RGM_SAME_DOMAIN,
                                EIID_IParcelable, (IInterface**)&pParcelable);
                        if (FAILED(ec)) return ec;

                        ec = pParcelable->ReadFromParcel(this);
                        if (FAILED(ec)) return ec;

                        iid = pClassInfo->interfaces[ipack.m_uIndex]->iid;
                        *(IInterface**)pValue = pParcelable->Probe(iid);
                    }
                    else {
                        ec = StdUnmarshalInterface(
                                UnmarshalFlag_Noncoexisting,
                                &ipack,
                                (IInterface **)pValue);
                        if (FAILED(ec)) {
                            MARSHAL_DBGOUT(MSHDBG_ERROR, ALOGE(
                                    "MshProc: unmsh interface, ec = %x\n", ec));
                            return ec;
                        }
                    }
                }
                else {
                    *(IInterface**)pValue = NULL;
                }
            }
            break;

        case Type_Fd:
            *(Int32*)pValue = (Int32)m_pData->readFileDescriptor();
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

    switch(type) {
        case Type_Byte:
        case Type_Boolean:
            m_pData->writeInt32(*((Byte*)pValue));
            break;

        case Type_Int16:
            m_pData->writeInt32(*((Int16*)pValue));
            break;

        case Type_Char32:
        case Type_Int32:
            m_pData->writeInt32(*((Int32*)pValue));
            break;

        case Type_Float:
            m_pData->writeFloat(*((Float*)pValue));
            break;

        case Type_Int64:
            m_pData->writeInt64(*((Int64*)pValue));
            break;

        case Type_Double:
            m_pData->writeDouble(*((Double*)pValue));
            break;

        case Type_CString:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeCString((const char*)pValue);
            }
            break;

        case Type_String:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeCString((const char*)pValue);
            }
            break;

        case Type_InterfacePtr:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                InterfacePack itfPack;
                ec = StdMarshalInterface(
                        (IInterface*)pValue,
                        &itfPack);
                if (FAILED(ec)) return ec;

                m_pData->writeStrongBinder(itfPack.m_pBinder);
                itfPack.m_pBinder = NULL;
                m_pData->write((const void *)&itfPack, (int32_t)sizeof(InterfacePack));

                IParcelable *pParcelable = \
                        (IParcelable*)((IInterface*)pValue)->Probe(EIID_IParcelable);
                if (pParcelable != NULL) pParcelable->WriteToParcel(this);
            }
            break;

        case Type_Struct:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->writeInt32(size);
                m_pData->write(pValue, size);
            }
            break;

        case Type_EMuid:
            m_pData->write(pValue, sizeof(EMuid));

            break;

        case Type_EGuid:
            {
                m_pData->write(pValue, sizeof(EGuid));
                m_pData->writeCString(((EGuid *)pValue)->pUunm);
            }
            break;

        case Type_ArrayOf:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->write(pValue, sizeof(CarQuintet));

                if (CarQuintetFlag_Type_IObject
                    != (((PCARQUINTET)pValue)->m_flags
                            & CarQuintetFlag_TypeMask)) {
                    // copy the storaged data
                    //
                    if (CarQuintetFlag_Type_String == (((PCARQUINTET)pValue)->m_flags
                            & CarQuintetFlag_TypeMask)) {
                        Int32 size = ((PCARQUINTET)pValue)->m_size / sizeof(String);
                        String* pBuf = (String*)((PCARQUINTET)pValue)->m_pBuf;
                        for (Int32 i = 0; i < size; ++i) {
                            // ALOGD("i: %d, str: %s", i, pBuf[i].string());
                            if (!pBuf[i].IsNull()) {
                                m_pData->writeInt32(MSH_NOT_NULL);
                                m_pData->writeCString(pBuf[i].string());
                            }
                            else {  // null pointer
                                m_pData->writeInt32(MSH_NULL);
                            }
                        }
                    }
                    else {
                        m_pData->write(((PCARQUINTET)pValue)->m_pBuf,
                                ((PCARQUINTET)pValue)->m_size);
                    }
                }
                else {
                    Int32 size = ((PCARQUINTET)pValue)->m_size
                                / sizeof(IInterface *);
                    Int32 *pBuf = (Int32*)((PCARQUINTET)pValue)->m_pBuf;
                    for (Int32 i = 0; i < size; i++) {
                        if (pBuf[i]) {
                            m_pData->writeInt32(MSH_NOT_NULL);

                            InterfacePack itfPack;
                            ec = StdMarshalInterface(
                                    (IInterface *)pBuf[i],
                                    &itfPack);
                            if (FAILED(ec)) {
                                MARSHAL_DBGOUT(MSHDBG_ERROR, ALOGE(
                                        "MshProc: marshal interface, ec = %x\n", ec));
                                return ec;
                            }

                            m_pData->writeStrongBinder(itfPack.m_pBinder);
                            itfPack.m_pBinder = NULL;
                            m_pData->write((void*)&itfPack, sizeof(itfPack));

                            IParcelable *pParcelable = \
                                    (IParcelable*)((IInterface*)pBuf[i])->Probe(EIID_IParcelable);
                            if (pParcelable != NULL) pParcelable->WriteToParcel(this);
                        }
                        else {  // null pointer
                            m_pData->writeInt32(MSH_NULL);
                        }
                    }
                }
            }
            break;

        case Type_ArrayOfString:
            m_pData->writeInt32(pValue ? MSH_NOT_NULL : MSH_NULL);

            if (pValue) {
                m_pData->write(pValue, sizeof(CarQuintet));

                Int32 size = ((PCARQUINTET)pValue)->m_size
                            / sizeof(String);
                String *pBuf = (String*)((PCARQUINTET)pValue)->m_pBuf;
                for (Int32 i = 0; i < size; i++) {
                    if (!pBuf[i].IsNull()) {
                        m_pData->writeInt32(MSH_NOT_NULL);
                        m_pData->writeCString(pBuf[i].string());
                    }
                    else {  // null pointer
                        m_pData->writeInt32(MSH_NULL);
                    }
                }
            }
            break;

        case Type_Fd:
            m_pData->writeFileDescriptor(*((Int32*)pValue));
            break;

        case Type_Fd_Dup:
            m_pData->writeDupFileDescriptor(*((Int32*)pValue));
            break;

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

ECode CRemoteParcel::ReadCString(
    /* [out] */ CString* pStr)
{
    if (pStr == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)pStr, Type_CString);
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
    return ReadValue((PVoid)pId, Type_EMuid);
}

ECode CRemoteParcel::ReadEGuid(EGuid *pId)
{
    return ReadValue((PVoid)pId, Type_EGuid);
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
    assert(ppArray != NULL);

    return ReadValue((PVoid)ppArray, Type_ArrayOf);
}

ECode CRemoteParcel::ReadArrayOfCString(
    /* [out, callee] */ ArrayOf<CString>** ppArray)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::ReadArrayOfString(
    /* [out, callee] */ ArrayOf<String>** ppArray)
{
    return ReadValue((PVoid)ppArray, Type_ArrayOfString);
}

// Retrieve a file descriptor from the parcel.  This returns the raw fd
// in the parcel, which you do not own -- use dup() to get your own copy.
ECode CRemoteParcel::ReadFileDescriptor(
    /* [out] */ Int32* fd)
{
    if (fd == NULL) return E_INVALID_ARGUMENT;

    return ReadValue((PVoid)fd, Type_Fd);
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

ECode CRemoteParcel::WriteCString(CString str)
{
    Int32 size = sizeof(UInt32);

    if (!str.IsNull()) {
        size += MSH_ALIGN_4(strlen((const char*)(str)) + 1) + sizeof(UInt32);
    }

    return WriteValue((PVoid)(const char*)(str), Type_CString, size);
}

ECode CRemoteParcel::WriteString(const String& str)
{
    Int32 size = sizeof(UInt32);

    if (!str.IsNull()) {
        size += MSH_ALIGN_4(str.GetByteLength()+ 1) + sizeof(UInt32);
    }

    return WriteValue((PVoid)str.string(), Type_String, size);
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
    Int32 size = pArray != 0 ? sizeof(UInt32) + sizeof(CarQuintet) + ((CarQuintet*)pArray)->m_size
        : sizeof(UInt32);
    return WriteValue((PVoid)pArray, Type_ArrayOf, size);
}

ECode CRemoteParcel::WriteArrayOfCString(
    /* [in] */ ArrayOf<CString>* array)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::WriteArrayOfString(
    /* [in] */ ArrayOf<String>* array)
{
    Int32 size = array != NULL ? sizeof(UInt32) + sizeof(CarQuintet) + array->m_size : sizeof(UInt32);
    return WriteValue((PVoid)array, Type_ArrayOfString, size);
}

// Place a file descriptor into the parcel.  The given fd must remain
// valid for the lifetime of the parcel.
ECode CRemoteParcel::WriteFileDescriptor(
    /* [in] */ Int32 fd)
{
    return WriteValue((PVoid)&fd, Type_Fd, 4);
}

// Place a file descriptor into the parcel.  A dup of the fd is made, which
// will be closed once the parcel is destroyed.
ECode CRemoteParcel::WriteDupFileDescriptor(
    /* [in] */ Int32 fd)
{
    return WriteValue((PVoid)&fd, Type_Fd_Dup, 4);
}

ECode CRemoteParcel::Clone(
    /* [in] */ IParcel* pSrcParcel)
{
    return E_NOT_IMPLEMENTED;
}

ECode CRemoteParcel::GetDataPosition(
    /* [out] */ Int32* position)
{
    if (position == NULL) return E_NOT_IMPLEMENTED;

    *position = m_pData->dataPosition();
    return NOERROR;
}

ECode CRemoteParcel::SetDataPosition(
    /* [in] */ Int32 position)
{
    m_pData->setDataPosition(position);
    return NOERROR;
}

ECode CRemoteParcel::GetElementPayload(
    /* [out] */ Handle32* pBuffer)
{
    *pBuffer = (Handle32)m_pData;

    return NOERROR;
}

ECode CRemoteParcel::GetElementSize(
    /* [in] */ Int32* pSize)
{
    *pSize = (Int32)m_pData->dataSize();

    return NOERROR;
}

MarshalHeader* CRemoteParcel::GetMarshalHeader()
{
    return (MarshalHeader*)m_pData->data();
}

Boolean CRemoteParcel::IsParcelable(
    /* [in] */ InterfacePack *pInterfacePack,
    /* [out] */ CIClassInfo **ppClassInfo)
{
    CIInterfaceInfo *pInterfaceInfo = NULL;
    ECode ec;

    ec = LookupClassInfo(pInterfacePack->m_clsid, ppClassInfo);
    if (FAILED(ec)) {
        ec = GetRemoteClassInfo(pInterfacePack->m_pBinder.get(),
                                pInterfacePack->m_clsid,
                                ppClassInfo);
        if (FAILED(ec)) return FALSE;
    }

    for (UInt16 i = 0; i < (*ppClassInfo)->interfaceNum; i++) {
        pInterfaceInfo = (*ppClassInfo)->interfaces[i];
        if (pInterfaceInfo->iid == EIID_IParcelable) {
            return TRUE;
        }
    }

    return FALSE;
}

} // namespace IPC
} // namespace Elastos

ELAPI _CParcel_New(
    /* [out] */ IParcel **ppObj)
{
    if (ppObj == NULL) return E_INVALID_ARGUMENT;
    Elastos::IPC::CRemoteParcel *pObj = NULL;
    void* pLocation = calloc(sizeof(Elastos::IPC::CRemoteParcel), 1);
    if (!pLocation) return E_OUT_OF_MEMORY;

    pObj = (Elastos::IPC::CRemoteParcel *)new(pLocation) Elastos::IPC::CRemoteParcel(FALSE);
    pObj->AddRef();

    *ppObj = (IParcel*)pObj;

    return NOERROR;
}
