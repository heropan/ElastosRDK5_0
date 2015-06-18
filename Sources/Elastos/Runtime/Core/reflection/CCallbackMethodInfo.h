//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#ifndef __CCALLBACKMETHODINFO_H__
#define __CCALLBACKMETHODINFO_H__

#include "CMethodInfo.h"

class CCallbackMethodInfo
    : public ElLightRefBase
    , public ICallbackMethodInfo
{
public:
    CCallbackMethodInfo();

    CARAPI_(PInterface) Probe(
        /* [in] */ REIID riid);

    CARAPI_(UInt32) AddRef();

    CARAPI_(UInt32) Release();

    CARAPI GetInterfaceID(
        /* [in] */ IInterface *pObject,
        /* [out] */ InterfaceID *pIID);

    CARAPI Init(
        /* [in] */ CClsModule * pCClsModule,
        /* [in] */ UInt32 uEventNum,
        /* [in] */ MethodDescriptor *pMethodDescriptor,
        /* [in] */ UInt32 uIndex);

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
        /* [in] */ const String& name,
        /* [out] */ IParamInfo ** ppParamInfo);

    CARAPI AddCallback(
        /* [in] */ PInterface server,
        /* [in] */ EventHandler handler);

    CARAPI RemoveCallback(
        /* [in] */ PInterface server,
        /* [in] */ EventHandler handler);

    CARAPI CreateDelegateProxy(
        /* [in] */ PVoid targetObject,
        /* [in] */ PVoid targetMethod,
        /* [in] */ ICallbackInvocation * pCallbackInvocation,
        /* [out] */ IDelegateProxy ** ppDelegateProxy);

    CARAPI CreateCBArgumentList(
        /* [out] */ ICallbackArgumentList ** ppCBArgumentList);

private:
    AutoPtr<CMethodInfo> m_pMethodInfo;
    UInt32               m_uEventNum;
    MethodDescriptor    *m_pMethodDescriptor;
};

#endif // __CCALLBACKMETHODINFO_H__
