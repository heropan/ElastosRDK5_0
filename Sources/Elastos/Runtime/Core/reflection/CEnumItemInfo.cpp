//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CEnumItemInfo.h"
#include "CEnumInfo.h"

CEnumItemInfo::CEnumItemInfo(
    /* [in] */ IEnumInfo * pEnumInfo,
    /* [in] */ const String& name,
    /* [in] */ Int32 value)
    : m_pEnumInfo(pEnumInfo)
    , m_sName(name)
    , m_iValue(value)
{}

UInt32 CEnumItemInfo::AddRef()
{
    return m_pEnumInfo->AddRef();
}

UInt32 CEnumItemInfo::Release()
{
    return m_pEnumInfo->Release();
}

PInterface CEnumItemInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IEnumItemInfo) {
        return (IEnumItemInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CEnumItemInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CEnumItemInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = m_sName;
    return NOERROR;
}

ECode CEnumItemInfo::GetEnumInfo(
    /* [out] */ IEnumInfo ** ppEnumInfo)
{
    if (!ppEnumInfo) {
        return E_INVALID_ARGUMENT;
    }

    *ppEnumInfo = m_pEnumInfo;
    (*ppEnumInfo)->AddRef();
    return NOERROR;
}

ECode CEnumItemInfo::GetValue(
    /* [out] */ Int32 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }

    *pValue = m_iValue;
    return NOERROR;
}
