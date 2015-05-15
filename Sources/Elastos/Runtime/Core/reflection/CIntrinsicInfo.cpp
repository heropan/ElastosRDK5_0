//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CIntrinsicInfo.h"
#include "CObjInfoList.h"
#include "refutil.h"

CIntrinsicInfo::CIntrinsicInfo(
    /* [in] */ CarDataType dataType,
    /* [in] */ UInt32 uSize)
{
    m_dataType = dataType;
    m_uSize = uSize;
}

UInt32 CIntrinsicInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CIntrinsicInfo::Release()
{
    return ElLightRefBase::Release();
}

PInterface CIntrinsicInfo::Probe(
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

ECode CIntrinsicInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CIntrinsicInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = g_cDataTypeList[m_dataType].name;
    return NOERROR;
}

ECode CIntrinsicInfo::GetSize(
    /* [out] */ MemorySize * pSize)
{
    if (!m_uSize) {
        return E_INVALID_OPERATION;
    }

    if (!pSize) {
        return E_INVALID_ARGUMENT;
    }

    *pSize = m_uSize;
    return NOERROR;
}

ECode CIntrinsicInfo::GetDataType(
    /* [out] */ CarDataType * pDataType)
{
    if (!pDataType) {
        return E_INVALID_ARGUMENT;
    }

    *pDataType = m_dataType;
    return NOERROR;
}
