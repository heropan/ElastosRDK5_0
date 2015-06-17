//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CCarArrayInfo.h"
#include "CVariableOfCarArray.h"
#include "CObjInfoList.h"
#include "CStructInfo.h"

CCarArrayInfo::CCarArrayInfo(
    /* [in] */ CarDataType quintetType,
    /* [in] */ IDataTypeInfo *pElementTypeInfo,
    /* [in] */ CarDataType dataType)
    : m_pElementTypeInfo(pElementTypeInfo)
    , m_elementDataType(dataType)
    , m_quintetType(quintetType)
{}

UInt32 CCarArrayInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CCarArrayInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_DataType);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        g_objInfoList.DetachCarArrayInfo(this);
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_DataType);
    assert(nRef >= 0);
    return nRef;
}

PInterface CCarArrayInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICarArrayInfo) {
        return (ICarArrayInfo *)this;
    }
    else if (riid == EIID_IDataTypeInfo) {
        return (IDataTypeInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CCarArrayInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCarArrayInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

//    if (m_elementDataType == CarDataType_LocalType) {
//        pName->Copy(g_cDataTypeList[CarDataType_LocalType].name);
//    }

    *pName = g_cDataTypeList[m_quintetType].name;

    String elementName;
    ECode ec = m_pElementTypeInfo->GetName(&elementName);
    if (FAILED(ec)) return ec;

    pName->Append("<");
    pName->Append(elementName);
    if (m_elementDataType == CarDataType_Interface) {
        pName->Append("*");
    }
    pName->Append(">");

    return NOERROR;
}

ECode CCarArrayInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    return E_INVALID_OPERATION;
}

ECode CCarArrayInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = m_quintetType;
    return NOERROR;
}

ECode CCarArrayInfo::GetElementTypeInfo(
    /* [out] */ IDataTypeInfo ** ppElementTypeInfo)
{
    if (!ppElementTypeInfo) {
        return E_INVALID_ARGUMENT;
    }

    *ppElementTypeInfo = m_pElementTypeInfo;
    (*ppElementTypeInfo)->AddRef();
    return NOERROR;
}

ECode CCarArrayInfo::CreateVariable(
    /* [in] */ Int32 capacity,
    /* [out] */ IVariableOfCarArray ** ppVariable)
{
    if (capacity <= 0 || !ppVariable) {
        return E_INVALID_ARGUMENT;
    }

    Int32 size = 0;
    ECode ec = m_pElementTypeInfo->GetSize(&size);
    if (FAILED(ec)) return ec;

    Int32 bufSize = capacity * size;
    PCarQuintet pCarQuintet =
        (PCarQuintet)calloc(sizeof(CarQuintet) + bufSize, sizeof(Byte));
    if (!pCarQuintet) {
        return E_OUT_OF_MEMORY;
    }

    pCarQuintet->m_flags = DataTypeToFlag(m_elementDataType);
    if (m_quintetType == CarDataType_ArrayOf) {
        pCarQuintet->m_used = bufSize;
    }

    pCarQuintet->m_size = bufSize;
    pCarQuintet->m_pBuf = (Byte *)pCarQuintet + sizeof(CarQuintet);

    CVariableOfCarArray *pCarArrayBox = new CVariableOfCarArray(this,
        pCarQuintet, TRUE);
    if (pCarArrayBox == NULL) {
        free(pCarQuintet);
        return E_OUT_OF_MEMORY;
    }

    *ppVariable = (IVariableOfCarArray *)pCarArrayBox;
    (*ppVariable)->AddRef();

    return NOERROR;
}

ECode CCarArrayInfo::CreateVariableBox(
    /* [in] */ PCarQuintet variableDescriptor,
    /* [out] */ IVariableOfCarArray ** ppVariable)
{
    if (!variableDescriptor || !ppVariable) {
        return E_INVALID_ARGUMENT;
    }

    Int32 size = 0;
    ECode ec = m_pElementTypeInfo->GetSize(&size);
    if (FAILED(ec)) return ec;
    if (variableDescriptor->m_size < size) {
        return E_INVALID_ARGUMENT;
    }

    if (!(variableDescriptor->m_flags & DataTypeToFlag(m_elementDataType))) {
        return E_INVALID_ARGUMENT;
    }

    CVariableOfCarArray *pCarArrayBox = new CVariableOfCarArray(this,
        variableDescriptor, FALSE);
    if (pCarArrayBox == NULL) {
        return E_OUT_OF_MEMORY;
    }

    *ppVariable = (IVariableOfCarArray *)pCarArrayBox;
    (*ppVariable)->AddRef();

    return NOERROR;
}

ECode CCarArrayInfo::GetMaxAlignSize(
    /* [out] */ MemorySize * pAlignSize)
{
    Int32 size = 1;

    if (m_elementDataType == CarDataType_Struct) {
        ((CStructInfo *)m_pElementTypeInfo.Get())->GetMaxAlignSize(&size);
    }
    else {
        m_pElementTypeInfo->GetSize(&size);
    }

    if (size > ALIGN_BOUND) size = ALIGN_BOUND;

    *pAlignSize = size;

    return NOERROR;
}
