//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CTypeAliasInfo.h"
#include "CObjInfoList.h"

CTypeAliasInfo::CTypeAliasInfo(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ AliasDirEntry *pAliasDirEntry)
{
    m_pCClsModule = pCClsModule;
    m_pAliasDirEntry = pAliasDirEntry;
}

UInt32 CTypeAliasInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CTypeAliasInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_TypeAliase);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        g_objInfoList.RemoveTypeAliasInfo(m_pAliasDirEntry);
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_TypeAliase);
    assert(nRef >= 0);
    return nRef;
}

PInterface CTypeAliasInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ITypeAliasInfo) {
        return (ITypeAliasInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CTypeAliasInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CTypeAliasInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = adjustNameAddr(m_pCClsModule->mBase, m_pAliasDirEntry->pszName);
    return NOERROR;
}

ECode CTypeAliasInfo::GetTypeInfo(
    /* [out] */ IDataTypeInfo ** ppTypeInfo)
{
    return g_objInfoList.AcquireDataTypeInfo(m_pCClsModule,
        &m_pAliasDirEntry->type, ppTypeInfo);
}

ECode CTypeAliasInfo::GetModuleInfo(
    /* [out] */ IModuleInfo ** ppModuleInfo)
{
    return m_pCClsModule->GetModuleInfo(ppModuleInfo);
}

ECode CTypeAliasInfo::IsDummy(
    /* [out] */ Boolean * pIsDummy)
{
    if (!pIsDummy) {
        return E_INVALID_ARGUMENT;
    }

    *pIsDummy = m_pAliasDirEntry->bDummyType;
    return NOERROR;
}

ECode CTypeAliasInfo::GetPtrLevel(
    /* [out] */ Int32 *pLevel)
{
    if (!pLevel) {
        return E_INVALID_ARGUMENT;
    }

    *pLevel = m_pAliasDirEntry->type.nPointer;
    return NOERROR;
}
