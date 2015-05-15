//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CFieldInfo.h"
#include "CObjInfoList.h"

CFieldInfo::CFieldInfo(
    /* [in] */ IStructInfo * pStructInfo,
    /* [in] */ const String& name,
    /* [in] */ IDataTypeInfo *pTypeInfo)
    : m_pStructInfo(pStructInfo)
    , m_sName(name)
    , m_pTypeInfo(pTypeInfo)
{}

UInt32 CFieldInfo::AddRef()
{
    return m_pStructInfo->AddRef();
}

UInt32 CFieldInfo::Release()
{
    return m_pStructInfo->Release();
}

PInterface CFieldInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IFieldInfo) {
        return (IFieldInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CFieldInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CFieldInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = m_sName;
    return NOERROR;
}

ECode CFieldInfo::GetTypeInfo(
    /* [out] */ IDataTypeInfo ** ppTypeInfo)
{
    if (!ppTypeInfo) {
        return E_INVALID_ARGUMENT;
    }

    *ppTypeInfo = m_pTypeInfo;
    (*ppTypeInfo)->AddRef();
    return NOERROR;
}
