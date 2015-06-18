//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================
#include "CModuleInfo.h"
#include "_pubcrt.h"

CModuleInfo::CModuleInfo(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ const String& path)
{
    m_pCClsModule = pCClsModule;
    m_pClsMod = pCClsModule->m_pClsMod;
    m_sbPath = path;

    m_iAliasCount = 0;
}

UInt32 CModuleInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CModuleInfo::Release()
{
    g_objInfoList.LockHashTable(EntryType_Module);
    Int32 nRef = atomic_dec(&mRef);

    if (0 == nRef) {
        g_objInfoList.RemoveModuleInfo(m_sbPath);
        delete this;
    }
    g_objInfoList.UnlockHashTable(EntryType_Module);
    assert(nRef >= 0);
    return nRef;
}

PInterface CModuleInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IModuleInfo) {
        return (IModuleInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CModuleInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CModuleInfo::GetPath(
    /* [out] */ String * pPath)
{
    *pPath = m_sbPath;
    return NOERROR;
}

ECode CModuleInfo::GetVersion(
    /* [out] */ Int32 * pMajor,
    /* [out] */ Int32 * pMinor,
    /* [out] */ Int32 * pBuild,
    /* [out] */ Int32 * pRevision)
{
    *pMajor = m_pClsMod->cMajorVersion;
    *pMinor = m_pClsMod->cMinorVersion;
    return NOERROR;
}

ECode CModuleInfo::GetClassCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pClsMod->cClasses;
    return NOERROR;
}

ECode CModuleInfo::AcquireClassList()
{
    ECode ec = NOERROR;
    g_objInfoList.LockHashTable(EntryType_Class);
    if (!m_pClassList) {
        m_pClassList = new CEntryList(EntryType_Class,
            m_pClsMod, m_pClsMod->cClasses, m_pCClsModule);
        if (!m_pClassList) {
            ec = E_OUT_OF_MEMORY;
        }
    }
    g_objInfoList.UnlockHashTable(EntryType_Class);

    return ec;
}

ECode CModuleInfo::GetAllClassInfos(
    /* [out] */ ArrayOf<IClassInfo *> * pClassInfos)
{
    ECode ec = AcquireClassList();
    if (FAILED(ec)) return ec;

    return m_pClassList->GetAllObjInfos((PTypeInfos)pClassInfos);
}

ECode CModuleInfo::GetClassInfo(
    /* [in] */ const String& fullName,
    /* [out] */ IClassInfo ** ppClassInfo)
{
    if (fullName.IsNull() || NULL == ppClassInfo) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = AcquireClassList();
    if (FAILED(ec)) return ec;

    Int32 start = fullName.IndexOf('L');
    Int32 end = fullName.IndexOf(';');
    String strName = fullName.Substring(start >= 0 ? start + 1 : 0, end > 0 ? end : fullName.GetLength()).Replace('/', '.');
    return m_pClassList->AcquireObjByName(strName, (IInterface **)ppClassInfo);
}

ECode CModuleInfo::GetInterfaceCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pClsMod->cInterfaces;
    return NOERROR;
}

ECode CModuleInfo::AcquireInterfaceList()
{
    ECode ec = NOERROR;
    g_objInfoList.LockHashTable(EntryType_Interface);
    if (!m_pInterfaceList) {
        m_pInterfaceList = new CEntryList(EntryType_Interface,
            m_pClsMod, m_pClsMod->cInterfaces, m_pCClsModule);
        if (!m_pInterfaceList) {
            ec = E_OUT_OF_MEMORY;
        }
    }
    g_objInfoList.UnlockHashTable(EntryType_Interface);

    return ec;
}

ECode CModuleInfo::GetAllInterfaceInfos(
    /* [out] */ ArrayOf<IInterfaceInfo *> * pInterfaceInfos)
{
    ECode ec = AcquireInterfaceList();
    if (FAILED(ec)) return ec;

    return m_pInterfaceList->GetAllObjInfos((PTypeInfos)pInterfaceInfos);
}

ECode CModuleInfo::GetInterfaceInfo(
    /* [in] */ const String& fullName,
    /* [out] */ IInterfaceInfo ** ppInterfaceInfo)
{
    if (fullName.IsNull() || NULL == ppInterfaceInfo) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = AcquireInterfaceList();
    if (FAILED(ec)) return ec;

    Int32 start = fullName.IndexOf('L');
    Int32 end = fullName.IndexOf(';');
    String strName = fullName.Substring(start >= 0 ? start + 1 : 0, end > 0 ? end : fullName.GetLength()).Replace('/', '.');
    return m_pInterfaceList->AcquireObjByName(
        strName, (IInterface **)ppInterfaceInfo);
}

ECode CModuleInfo::GetStructCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pClsMod->cStructs;
    return NOERROR;
}

ECode CModuleInfo::AcquireStructList()
{
    ECode ec = NOERROR;
    g_objInfoList.LockHashTable(EntryType_Struct);
    if (!m_pStructList) {
        m_pStructList = new CEntryList(EntryType_Struct,
            m_pClsMod, m_pClsMod->cStructs, m_pCClsModule);
        if (!m_pStructList) {
            ec = E_OUT_OF_MEMORY;
        }
    }
    g_objInfoList.UnlockHashTable(EntryType_Struct);

    return ec;
}

ECode CModuleInfo::GetAllStructInfos(
    /* [out] */ ArrayOf<IStructInfo *> * pStructInfos)
{
    ECode ec = AcquireStructList();
    if (FAILED(ec)) return ec;

    return m_pStructList->GetAllObjInfos((PTypeInfos)pStructInfos);
}

ECode CModuleInfo::GetStructInfo(
    /* [in] */ const String& name,
    /* [out] */ IStructInfo ** ppStructInfo)
{
    if (name.IsNull() || !ppStructInfo) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pClsMod->cStructs) {
        return E_DOES_NOT_EXIST;
    }

    ECode ec = AcquireStructList();
    if (FAILED(ec)) return ec;

    return m_pStructList->AcquireObjByName(name, (IInterface **)ppStructInfo);
}

ECode CModuleInfo::GetEnumCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pClsMod->cEnums;
    return NOERROR;
}

ECode CModuleInfo::AcquireEnumList()
{
    ECode ec = NOERROR;
    g_objInfoList.LockHashTable(EntryType_Enum);
    if (!m_pEnumList) {
        m_pEnumList = new CEntryList(EntryType_Enum,
            m_pClsMod, m_pClsMod->cEnums, m_pCClsModule);
        if (!m_pEnumList) {
            ec = E_OUT_OF_MEMORY;
        }
    }
    g_objInfoList.UnlockHashTable(EntryType_Enum);

    return ec;
}

ECode CModuleInfo::GetAllEnumInfos(
    /* [out] */ ArrayOf<IEnumInfo *> * pEnumInfos)
{
    ECode ec = AcquireEnumList();
    if (FAILED(ec)) return ec;

    return m_pEnumList->GetAllObjInfos((PTypeInfos)pEnumInfos);
}

ECode CModuleInfo::GetEnumInfo(
    /* [in] */ const String& fullName,
    /* [out] */ IEnumInfo ** ppEnumInfo)
{
    if (fullName.IsNull() || !ppEnumInfo) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pClsMod->cEnums) {
        return E_DOES_NOT_EXIST;
    }

    ECode ec = AcquireEnumList();
    if (FAILED(ec)) return ec;

    Int32 start = fullName.IndexOf('L');
    Int32 end = fullName.IndexOf(';');
    String strName = fullName.Substring(start >= 0 ? start + 1 : 0, end > 0 ? end : fullName.GetLength()).Replace('/', '.');
    return m_pEnumList->AcquireObjByName(strName, (IInterface **)ppEnumInfo);
}

ECode CModuleInfo::GetTypeAliasCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    ECode ec = AcquireAliasList();
    if (FAILED(ec)) return ec;

    *pCount = m_iAliasCount;
    return NOERROR;
}

ECode CModuleInfo::AcquireAliasList()
{
    ECode ec = NOERROR;
    g_objInfoList.LockHashTable(EntryType_TypeAliase);
    if (!m_pAliasList) {
        m_iAliasCount = 0;
        for (Int32 i = 0; i < m_pClsMod->cAliases; i++) {
            if (!IsSysAlaisType(m_pCClsModule, i)) m_iAliasCount++;
        }

        m_pAliasList = new CEntryList(EntryType_TypeAliase,
            m_pClsMod, m_iAliasCount, m_pCClsModule);
        if (!m_pAliasList) {
            ec = E_OUT_OF_MEMORY;
        }
    }
    g_objInfoList.UnlockHashTable(EntryType_TypeAliase);

    return ec;
}

ECode CModuleInfo::GetAllTypeAliasInfos(
    /* [out] */ ArrayOf<ITypeAliasInfo *> * pTypeAliasInfos)
{
    ECode ec = AcquireAliasList();
    if (FAILED(ec)) return ec;

    return m_pAliasList->GetAllObjInfos((PTypeInfos)pTypeAliasInfos);
}

ECode CModuleInfo::GetTypeAliasInfo(
    /* [in] */ const String& name,
    /* [out] */ ITypeAliasInfo ** ppTypeAliasInfo)
{
    if (name.IsNull() || !ppTypeAliasInfo) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pClsMod->cAliases) {
        return E_DOES_NOT_EXIST;
    }

    ECode ec = AcquireAliasList();
    if (FAILED(ec)) return ec;

    return m_pAliasList->AcquireObjByName(name, (IInterface **)ppTypeAliasInfo);
}

ECode CModuleInfo::GetConstantCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pClsMod->cConsts;
    return NOERROR;
}

ECode CModuleInfo::AcquireConstantList()
{
    ECode ec = NOERROR;
    g_objInfoList.LockHashTable(EntryType_Constant);
    if (!m_pConstantList) {
        m_pConstantList = new CEntryList(EntryType_Constant,
            m_pClsMod, m_pClsMod->cConsts, m_pCClsModule);
        if (!m_pConstantList) {
            ec = E_OUT_OF_MEMORY;
        }
    }
    g_objInfoList.UnlockHashTable(EntryType_Constant);

    return ec;
}

ECode CModuleInfo::GetAllConstantInfos(
    /* [out] */ ArrayOf<IConstantInfo *> * pConstantInfos)
{
    ECode ec = AcquireConstantList();
    if (FAILED(ec)) return ec;

    return m_pConstantList->GetAllObjInfos((PTypeInfos)pConstantInfos);
}

ECode CModuleInfo::GetConstantInfo(
    /* [in] */ const String& name,
    /* [out] */ IConstantInfo ** ppConstantInfo)
{
    if (name.IsNull() || !ppConstantInfo) {
        return E_INVALID_ARGUMENT;
    }

    if (!m_pClsMod->cConsts) {
        return E_DOES_NOT_EXIST;
    }

    ECode ec = AcquireConstantList();
    if (FAILED(ec)) return ec;

    return m_pConstantList->AcquireObjByName(name, (IInterface **)ppConstantInfo);
}

ECode CModuleInfo::GetImportModuleInfoCount(
    /* [out] */ Int32 * pCount)
{
    if (!pCount) {
        return E_INVALID_ARGUMENT;
    }

    *pCount = m_pClsMod->cLibraries;
    return NOERROR;
}

ECode CModuleInfo::GetAllImportModuleInfos(
    /* [out] */ ArrayOf<IModuleInfo *> * pModuleInfos)
{
    if (!pModuleInfos) {
        return E_INVALID_ARGUMENT;
    }

    Int32 nCapacity = pModuleInfos->GetLength();
    if (!nCapacity) {
        return E_INVALID_ARGUMENT;
    }

    Int32 nTotalCount = m_pClsMod->cLibraries;
    if (!nTotalCount) {
        return NOERROR;
    }

    Int32 nCount = nCapacity < nTotalCount ? nCapacity : nTotalCount;
    ECode ec = NOERROR;
    for (Int32 i = 0; i < nCount; i++) {
        String libNames(getLibNameAddr(m_pCClsModule->m_nBase, m_pClsMod->ppLibNames, i));
        AutoPtr<IModuleInfo> pObject;
        ec = g_objInfoList.AcquireModuleInfo(libNames, (IModuleInfo**)&pObject);
        if (FAILED(ec)) return ec;
        pModuleInfos->Set(i, pObject);
    }

    return NOERROR;
}
