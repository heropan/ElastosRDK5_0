//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CLocalPtrInfo.h"
#include "CObjInfoList.h"

CLocalPtrInfo::CLocalPtrInfo(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ TypeDescriptor *pTypeDescriptor,
    /* [in] */ Int32 iPointer)
{
    m_pCClsModule = pCClsModule;
    m_pTypeDescriptor = pTypeDescriptor;
    m_iPointer = iPointer;
}

UInt32 CLocalPtrInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CLocalPtrInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_Local);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        g_objInfoList.RemoveLocalPtrInfo(m_pTypeDescriptor, m_iPointer);
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_Local);
    assert(nRef >= 0);
    return nRef;
}

PInterface CLocalPtrInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ILocalPtrInfo) {
        return (ILocalPtrInfo *)this;
    }
    else if (riid == EIID_IDataTypeInfo) {
        return (IDataTypeInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CLocalPtrInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalPtrInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = g_cDataTypeList[CarDataType_LocalPtr].name;
    return NOERROR;
}

ECode CLocalPtrInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    if (!pSize) {
        return E_INVALID_ARGUMENT;
    }

    *pSize = sizeof(PVoid);

    return NOERROR;
}

ECode CLocalPtrInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = CarDataType_LocalPtr;

    return NOERROR;
}

ECode CLocalPtrInfo::GetTargetTypeInfo(
    /* [out] */ IDataTypeInfo ** ppDateTypeInfo)
{
    return g_objInfoList.AcquireDataTypeInfo(m_pCClsModule,
                                             m_pTypeDescriptor,
                                             ppDateTypeInfo);
}

ECode CLocalPtrInfo::GetPtrLevel(
    /* [out] */ Int32 *pLevel)
{
    if (!pLevel) {
        return E_INVALID_ARGUMENT;
    }

    *pLevel = m_iPointer;

    return NOERROR;
}
