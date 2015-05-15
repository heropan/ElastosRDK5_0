//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CMETHODINFO_H__
#define __CMETHODINFO_H__

#include "CClsModule.h"
#include "CEntryList.h"

class CMethodInfo
    : public ElLightRefBase
    , public IMethodInfo
{
public:
    CMethodInfo(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ MethodDescriptor *pMethodDescriptor,
        /* [in] */ UInt32 uIndex);

    virtual ~CMethodInfo();

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

    CARAPI GetParamCount(
        /* [out] */ Int32 * pCount);

    CARAPI GetAllParamInfos(
        /* [out] */ ArrayOf<IParamInfo *> * pParamInfos);

    CARAPI GetParamInfoByIndex(
        /* [in] */ Int32 index,
        /* [out] */ IParamInfo ** ppParamInfo);

    CARAPI GetParamInfoByName(
        /* [in] */ CString name,
        /* [out] */ IParamInfo ** ppParamInfo);

    CARAPI CreateArgumentList(
        /* [out] */ IArgumentList ** ppArgumentList);

    CARAPI Invoke(
        /* [in] */ PInterface target,
        /* [in] */ IArgumentList * pArgumentList);

    CARAPI SetParamElem(
        /* [in] */ ParamDescriptor *pParamDescriptor,
        /* [in] */ ParmElement *pParmElement);

    CARAPI InitParmElement();

    CARAPI InitParamInfos();

    CARAPI CreateFunctionArgumentList(
        /* [in] */ IFunctionInfo *pFunctionInfo,
        /* [in] */ Boolean bMethodInfo,
        /* [out] */ IArgumentList ** ppArgumentList);

    CARAPI CreateCBArgumentList(
        /* [in] */ ICallbackMethodInfo *pCallbackMethodInfo,
        /* [out] */ ICallbackArgumentList ** ppCBArgumentList);

public:
    MethodDescriptor   *m_pMethodDescriptor;
    UInt32              m_uIndex;
    CLSModule          *m_pClsMod;
    AutoPtr<CClsModule> m_pCClsModule;

private:
    ArrayOf<IParamInfo *> *m_pParameterInfos;
    ParmElement    *m_pParamElem;
    UInt32          m_uParamBufSize;
    Int32           m_nBase;
};

#endif // __CMETHODINFO_H__
