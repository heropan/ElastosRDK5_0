//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CCallbackMethodInfo.h"
#include "CDelegateProxy.h"

CCallbackMethodInfo::CCallbackMethodInfo()
    : m_uEventNum(0)
    , m_pMethodDescriptor(NULL)
{}

UInt32 CCallbackMethodInfo::AddRef()
{
    return ElLightRefBase::AddRef();
}

UInt32 CCallbackMethodInfo::Release()
{
    return ElLightRefBase::Release();
}

PInterface  CCallbackMethodInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_ICallbackMethodInfo) {
        return (ICallbackMethodInfo *)this;
    }
    else if (riid == EIID_IFunctionInfo) {
        return (IFunctionInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CCallbackMethodInfo::GetInterfaceID(
    /* [in] */ IInterface *pObject,
    /* [out] */ InterfaceID *pIID)
{
    return E_NOT_IMPLEMENTED;
}

ECode CCallbackMethodInfo::Init(
    /* [in] */ CClsModule * pCClsModule,
    /* [in] */ UInt32 uEventNum,
    /* [in] */ MethodDescriptor *pMethodDescriptor,
    /* [in] */ UInt32 uIndex)
{
    m_pMethodDescriptor = pMethodDescriptor;
    m_uEventNum = uEventNum;

    m_pMethodInfo = NULL;
    return g_objInfoList.AcquireMethodInfo(pCClsModule, pMethodDescriptor,
        uIndex, (IInterface **)&m_pMethodInfo);
}

ECode CCallbackMethodInfo::GetName(
    /* [out] */ String * pName)
{
    if (pName == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *pName = adjustNameAddr(m_pMethodInfo->m_pCClsModule->m_nBase,
                    m_pMethodDescriptor->pszName);
    return NOERROR;
}

ECode CCallbackMethodInfo::GetParamCount(
    /* [out] */ Int32 * pCount)
{
    return m_pMethodInfo->GetParamCount(pCount);
}

ECode CCallbackMethodInfo::GetAllParamInfos(
    /* [out] */ ArrayOf<IParamInfo *> * pParamInfos)
{
    return m_pMethodInfo->GetAllParamInfos(pParamInfos);
}

ECode CCallbackMethodInfo::GetParamInfoByIndex(
    /* [in] */ Int32 index,
    /* [out] */ IParamInfo ** ppParamInfo)
{
    return m_pMethodInfo->GetParamInfoByIndex(index, ppParamInfo);
}

ECode CCallbackMethodInfo::GetParamInfoByName(
    /* [in] */ CString name,
    /* [out] */ IParamInfo ** ppParamInfo)
{
    return m_pMethodInfo->GetParamInfoByName(name, ppParamInfo);
}

ECode CCallbackMethodInfo::AddCallback(
    /* [in] */ PInterface server,
    /* [in] */ EventHandler handler)
{
    if (!server) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<ICallbackSink> pSink;
    ECode ec = _CObject_AcquireCallbackSink(server, (ICallbackSink**)&pSink);
    if (FAILED(ec)) return ec;
    return pSink->AddCallback(m_uEventNum, handler);
}

ECode CCallbackMethodInfo::RemoveCallback(
    /* [in] */ PInterface server,
    /* [in] */ EventHandler handler)
{
    if (!server) {
        return E_INVALID_ARGUMENT;
    }

    AutoPtr<ICallbackSink> pSink;
    ECode ec = _CObject_AcquireCallbackSink(server, (ICallbackSink**)&pSink);
    if (FAILED(ec)) return ec;
    return pSink->RemoveCallback(m_uEventNum, handler);
}

ECode CCallbackMethodInfo::CreateCBArgumentList(
    /* [out] */ ICallbackArgumentList ** ppCBArgumentList)
{
    return m_pMethodInfo->CreateCBArgumentList(this, ppCBArgumentList);
}

ECode CCallbackMethodInfo::CreateDelegateProxy(
    /* [in] */ PVoid targetObject,
    /* [in] */ PVoid targetMethod,
    /* [in] */ ICallbackInvocation * pCallbackInvocation,
    /* [out] */ IDelegateProxy ** ppDelegateProxy)
{
    if (!pCallbackInvocation || !ppDelegateProxy) {
        return E_INVALID_ARGUMENT;
    }

    *ppDelegateProxy = new CDelegateProxy(this, pCallbackInvocation,
        targetObject, targetMethod);
    if (!*ppDelegateProxy) {
        return E_OUT_OF_MEMORY;
    }

    (*ppDelegateProxy)->AddRef();

    return NOERROR;
}
