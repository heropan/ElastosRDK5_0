//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCLASSINFO_H__
#define __CCLASSINFO_H__

#include "CEntryList.h"

struct CBMethodDesc
{
    MethodDescriptor *pDesc;
    UInt32 uIndex;
    UInt32 uEventNum;
};

class CClassInfo
    : public ElLightRefBase
    , public IClassInfo
{
public:
    CClassInfo(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ ClassDirEntry *pClsDirEntry);

    virtual ~CClassInfo();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Init();

    CARAPI GetName(
        /* [out] */ String * pName);

    CARAPI GetNamespace(
        /* [out] */ String * pNamespace);

    CARAPI GetId(
        /* [out] */ ClassID * pclsid);

    CARAPI GetModuleInfo(
        /* [out] */ IModuleInfo ** ppModuleInfo);

    CARAPI IsSingleton(
        /* [out] */ Boolean * pIsSingleton);

    CARAPI GetThreadingModel(
        /* [out] */ ThreadingModel *pThreadingModel);

    CARAPI IsPrivate(
        /* [out] */ Boolean * pIsPrivate);

    CARAPI IsReturnValue(
        /* [out] */ Boolean * pIsReturnValue);

    CARAPI IsBaseClass(
        /* [out] */ Boolean * pIsBaseClass);

    CARAPI HasBaseClass(
        /* [out] */ Boolean * pHasBaseClass);

    CARAPI GetBaseClassInfo(
        /* [out] */ IClassInfo ** ppBaseClassInfo);

    CARAPI IsGeneric(
        /* [out] */ Boolean * pIsGeneric);

    CARAPI HasGeneric(
        /* [out] */ Boolean * pHasGeneric);

    CARAPI GetGenericInfo(
        /* [out] */ IClassInfo ** ppGenericInfo);

    CARAPI IsRegime(
        /* [out] */ Boolean * pIsRegime);

    CARAPI GetAspectCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllAspectInfos(
        /* [out] */ ArrayOf<IClassInfo *> * pAspectInfos);

    CARAPI GetAspectInfo(
        /* [in] */ const String& name,
        /* [out] */ IClassInfo ** ppAspectInfo);

    CARAPI IsAspect(
        /* [out] */ Boolean * pIsAspect);

    CARAPI GetAggregateeCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllAggregateeInfos(
        /* [out] */ ArrayOf<IClassInfo *> * pAggregateeInfos);

    CARAPI GetAggregateeInfo(
        /* [in] */ const String& name,
        /* [out] */ IClassInfo ** ppAggregateeInfo);

    CARAPI GetConstructorCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllConstructorInfos(
        /* [out] */ ArrayOf<IConstructorInfo *> * pConstructorInfos);

    CARAPI GetConstructorInfoByParamNames(
        /* [in] */ const String& name,
        /* [out] */ IConstructorInfo **ppConstructorInfo);

    CARAPI GetConstructorInfoByParamCount(
        /* [in] */ Int32 count,
        /* [out] */ IConstructorInfo **ppConstructorInfo);

    CARAPI GetInterfaceCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllInterfaceInfos(
        /* [out] */ ArrayOf<IInterfaceInfo *> * pInterfaceInfos);

    CARAPI GetInterfaceInfo(
        /* [in] */ const String& fullName,
        /* [out] */ IInterfaceInfo ** ppInterfaceInfo);

    CARAPI GetCallbackInterfaceCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllCallbackInterfaceInfos(
        /* [out] */ ArrayOf<IInterfaceInfo *> * pCallbackInterfaceInfos);

    CARAPI GetCallbackInterfaceInfo(
        /* [in] */ const String& name,
        /* [out] */ IInterfaceInfo ** ppCallbackInterfaceInfo);

    CARAPI GetMethodCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllMethodInfos(
        /* [out] */ ArrayOf<IMethodInfo *> * pMethodInfos);

    CARAPI GetMethodInfo(
        /* [in] */ const String& name,
        /* [in] */ const String& signature,
        /* [out] */ IMethodInfo ** ppMethodInfo);

    CARAPI GetCallbackMethodCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllCallbackMethodInfos(
        /* [out] */ ArrayOf<ICallbackMethodInfo *> * pCallbackMethodInfos);

    CARAPI GetCallbackMethodInfo(
        /* [in] */ const String& name,
        /* [out] */ ICallbackMethodInfo ** ppCallbackMethodInfo);

    CARAPI RemoveAllCallbackHandlers(
        /* [in] */ PInterface server);

    CARAPI CreateObject(
        /* [out] */ PInterface * pObject);

    CARAPI CreateObjectInRegime(
        /* [in] */ PRegime rgm,
        /* [out] */ PInterface * pObject);

    CARAPI CreateIFList();

    CARAPI AcquireSpecialMethodList(
        /* [in] */ EntryType type,
        /* [out] */ CEntryList ** ppEntryList);

    CARAPI AcquireAggregateeList();

    CARAPI AcquireAspectList();

    CARAPI AcquireMethodList();

    CARAPI AcquireInterfaceList();

    CARAPI AcquireCBInterfaceList();

    CARAPI AcquireConstructorList();

    CARAPI AcquireCBMethodList();

    CARAPI CreateObjInRgm(
        /* [in] */ PRegime rgm,
        /* [out] */ PInterface * pObject);

public:
    ClassDirEntry   *m_pClassDirEntry;
    ClassID          m_clsId;

    IFIndexEntry    *m_pCBIFList;
    IFIndexEntry    *m_pIFList;
    CBMethodDesc    *m_pCBMethodDesc;

    UInt32           m_uIFCount;
    UInt32           m_uCBIFCount;

private:
    AutoPtr<CClsModule>  m_pCClsModule;
    CLSModule           *m_pClsMod;
    ClassDescriptor     *m_pDesc;
    Int32                m_nBase;

    char   m_szUrn[_MAX_PATH];

    UInt32 m_uMethodCount;
    UInt32 m_uCBMethodCount;
    UInt32 m_uCtorCount;

    AutoPtr<CEntryList> m_pAspectList;
    AutoPtr<CEntryList> m_pAggregateeList;
    AutoPtr<CEntryList> m_pMethodList;
    AutoPtr<CEntryList> m_pCBInterfaceList;
    AutoPtr<CEntryList> m_pInterfaceList;
    AutoPtr<CEntryList> m_pCtorList;
    AutoPtr<CEntryList> m_pCBMethodList;

    AutoPtr<CClassInfo> m_pCtorClassInfo;
    AutoPtr<IClassInfo> m_pGenericInfo;
};

#endif // __CCLASSINFO_H__
