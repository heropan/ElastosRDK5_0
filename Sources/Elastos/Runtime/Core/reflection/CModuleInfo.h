//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CMODULEINFO_H__
#define __CMODULEINFO_H__

#include "CEntryList.h"

class CModuleInfo
    : public ElLightRefBase
    , public IModuleInfo
{
public:
    CModuleInfo(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ const String& path);

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI GetPath(
        /* [out] */ String * pPath);

    CARAPI GetVersion(
        /* [out] */ Int32 * pMajor,
        /* [out] */ Int32 * pMinor,
        /* [out] */ Int32 * pBuild,
        /* [out] */ Int32 * pRevision);

    CARAPI GetClassCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllClassInfos(
        /* [out] */ ArrayOf<IClassInfo *> * pClassInfos);

    CARAPI GetClassInfo(
        /* [in] */ CString fullName,
        /* [out] */ IClassInfo ** ppClassInfo);

    CARAPI GetInterfaceCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllInterfaceInfos(
        /* [out] */ ArrayOf<IInterfaceInfo *> * pInterfaceInfos);

    CARAPI GetInterfaceInfo(
        /* [in] */ CString fullName,
        /* [out] */ IInterfaceInfo ** ppInterfaceInfo);

    CARAPI GetStructCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllStructInfos(
        /* [out] */ ArrayOf<IStructInfo *> * pStructInfos);

    CARAPI GetStructInfo(
        /* [in] */ CString name,
        /* [out] */ IStructInfo ** ppStructInfo);

    CARAPI GetEnumCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllEnumInfos(
        /* [out] */ ArrayOf<IEnumInfo *> * pEnumInfos);

    CARAPI GetEnumInfo(
        /* [in] */ CString fullName,
        /* [out] */ IEnumInfo ** ppEnumInfo);

    CARAPI GetTypeAliasCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllTypeAliasInfos(
        /* [out] */ ArrayOf<ITypeAliasInfo *> * pTypeAliasInfos);

    CARAPI GetTypeAliasInfo(
        /* [in] */ CString name,
        /* [out] */ ITypeAliasInfo ** ppTypeAliasInfo);

    CARAPI GetConstantCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllConstantInfos(
        /* [out] */ ArrayOf<IConstantInfo *> * pConstantInfos);

    CARAPI GetConstantInfo(
        /* [in] */ CString name,
        /* [out] */ IConstantInfo ** ppConstantInfo);

    CARAPI GetImportModuleInfoCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllImportModuleInfos(
        /* [out] */ ArrayOf<IModuleInfo *> * pModuleInfos);

    CARAPI AcquireClassList();

    CARAPI AcquireInterfaceList();

    CARAPI AcquireStructList();

    CARAPI AcquireEnumList();

    CARAPI AcquireAliasList();

    CARAPI AcquireConstantList();

public:
    AutoPtr<CClsModule>  m_pCClsModule;
    CLSModule           *m_pClsMod;

private:
    AutoPtr<CEntryList>  m_pClassList;
    AutoPtr<CEntryList>  m_pInterfaceList;
    AutoPtr<CEntryList>  m_pStructList;
    AutoPtr<CEntryList>  m_pEnumList;
    AutoPtr<CEntryList>  m_pAliasList;
    AutoPtr<CEntryList>  m_pConstantList;

    String               m_sbPath;
    Int32                m_iAliasCount;

    CRITICAL_SECTION     m_lockList;
};

#endif // __CMODULEINFO_H__
