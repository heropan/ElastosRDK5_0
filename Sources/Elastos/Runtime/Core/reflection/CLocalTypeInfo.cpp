//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CLocalTypeInfo.h"

CLocalTypeInfo::CLocalTypeInfo(
    /* [in] */ MemorySize size)
    : m_size(size)
{}

UInt32 CLocalTypeInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CLocalTypeInfo::Release()
{
    return ElLightRefBase::Release();
}

PInterface CLocalTypeInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IDataTypeInfo) {
        return (IDataTypeInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CLocalTypeInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CLocalTypeInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = g_cDataTypeList[CarDataType_LocalType].name;
    return NOERROR;
}

ECode CLocalTypeInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    if (!pSize) {
        return E_INVALID_ARGUMENT;
    }

    *pSize = m_size;

    return NOERROR;
}

ECode CLocalTypeInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = CarDataType_LocalType;

    return NOERROR;
}
