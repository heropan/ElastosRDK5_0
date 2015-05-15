//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CConstantInfo.h"

CConstantInfo::CConstantInfo(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ ConstDirEntry *pConstDirEntry)
    : m_pCClsModule(pCClsModule)
    , m_pConstDirEntry(pConstDirEntry)
{}

UInt32 CConstantInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CConstantInfo::Release()
{
    return ElLightRefBase::Release();
}

PInterface CConstantInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IConstantInfo) {
        return (IConstantInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CConstantInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CConstantInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = adjustNameAddr(m_pCClsModule->m_nBase, m_pConstDirEntry->pszName);
    return NOERROR;
}

ECode CConstantInfo::GetValue(
    /* [out] */ Int32 * pValue)
{
    if (!pValue) {
        return E_INVALID_ARGUMENT;
    }

    if (m_pConstDirEntry->type == TYPE_INTEGER32) {
        *pValue = m_pConstDirEntry->v.intValue.nValue;
        return NOERROR;
    }
    else return E_INVALID_OPERATION;
}

ECode CConstantInfo::GetModuleInfo(
    /* [out] */ IModuleInfo ** ppModuleInfo)
{
    return m_pCClsModule->GetModuleInfo(ppModuleInfo);
}
