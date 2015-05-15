//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CCppVectorInfo.h"
#include "CObjInfoList.h"
#include "CStructInfo.h"

CCppVectorInfo::CCppVectorInfo(
    /* [in] */ IDataTypeInfo *pElementTypeInfo,
    /* [in] */ Int32 length)
{
    m_pElementTypeInfo = pElementTypeInfo;
    pElementTypeInfo->GetSize(&m_iSize);
    m_iLength = length;
    m_iSize *= m_iLength;
}

UInt32 CCppVectorInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CCppVectorInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_DataType);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        g_objInfoList.DetachCppVectorInfo(this);
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_DataType);
    assert(nRef >= 0);
    return nRef;
}

PInterface CCppVectorInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICppVectorInfo) {
        return (ICppVectorInfo *)this;
    }
    else if (riid == EIID_IDataTypeInfo) {
        return (IDataTypeInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CCppVectorInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCppVectorInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = m_pElementTypeInfo->GetName(pName);
    if (FAILED(ec)) return ec;

    pName->Append("Vector");

    return NOERROR;
}

ECode CCppVectorInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    if (!pSize) {
        return E_INVALID_ARGUMENT;
    }

    *pSize = m_iSize;

    return NOERROR;
}

ECode CCppVectorInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = CarDataType_CppVector;

    return NOERROR;
}

ECode CCppVectorInfo::GetElementTypeInfo(
    /* [out] */ IDataTypeInfo ** ppElementTypeInfo)
{
    *ppElementTypeInfo = m_pElementTypeInfo;
    (*ppElementTypeInfo)->AddRef();

    return NOERROR;
}

ECode CCppVectorInfo::GetLength(
    /* [out] */ Int32 * pLength)
{
    if (!pLength) {
        return E_INVALID_ARGUMENT;
    }

    *pLength = m_iLength;

    return NOERROR;
}

ECode CCppVectorInfo::GetMaxAlignSize(
    /* [out] */ MemorySize * pAlignSize)
{
    Int32 size = 1;
    CarDataType dataType;
    m_pElementTypeInfo->GetDataType(&dataType);
    if (dataType == CarDataType_Struct) {
        ((CStructInfo *)m_pElementTypeInfo.Get())->GetMaxAlignSize(&size);
    }
    else {
        m_pElementTypeInfo->GetSize(&size);
    }

    if (size > ALIGN_BOUND) size = ALIGN_BOUND;

    *pAlignSize = size;

    return NOERROR;
}
