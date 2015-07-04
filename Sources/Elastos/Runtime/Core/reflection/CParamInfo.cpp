//==========================================================================
// Copyright (c) 2000-2008,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "CParamInfo.h"
#include "CMethodInfo.h"
#include "CLocalPtrInfo.h"
#include "CLocalTypeInfo.h"

CParamInfo::CParamInfo(
    /* [in] */ CClsModule* clsModule,
    /* [in] */ IMethodInfo* methodInfo,
    /* [in] */ ParmElement* parmElement,
    /* [in] */ ParamDescriptor* paramDescriptor,
    /* [in] */ Int32 index)
{
    mClsModule = clsModule;
    mMethodInfo = methodInfo;
    mParmElement = parmElement;
    mParamDescriptor = paramDescriptor;
    mIndex = index;
}

UInt32 CParamInfo::AddRef()
{
    return mMethodInfo->AddRef();
}

UInt32 CParamInfo::Release()
{
    return mMethodInfo->Release();
}

PInterface CParamInfo::Probe(
    /* [in] */ REIID riid)
{
    if (riid == EIID_IInterface) {
        return (PInterface)this;
    }
    else if (riid == EIID_IParamInfo) {
        return (IParamInfo *)this;
    }
    else {
        return NULL;
    }
}

ECode CParamInfo::GetInterfaceID(
    /* [in] */ IInterface* object,
    /* [out] */ InterfaceID* iid)
{
    return E_NOT_IMPLEMENTED;
}

ECode CParamInfo::GetMethodInfo(
    /* [out] */ IMethodInfo** methodInfo)
{
    if (!methodInfo) {
        return E_INVALID_ARGUMENT;
    }

    *methodInfo = mMethodInfo;
    (*methodInfo)->AddRef();

    return NOERROR;
}

ECode CParamInfo::GetName(
    /* [out] */ String* name)
{
    if (name == NULL) {
        return E_INVALID_ARGUMENT;
    }

    *name = adjustNameAddr(mClsModule->mBase, mParamDescriptor->pszName);
    return NOERROR;
}

ECode CParamInfo::GetIndex(
    /* [out] */ Int32* index)
{
    if (!index) {
        return E_INVALID_ARGUMENT;
    }

    *index = mIndex;
    return NOERROR;
}

ECode CParamInfo::GetIOAttribute(
    /* [out] */ ParamIOAttribute* ioAttrib)
{
    if (!ioAttrib) {
        return E_INVALID_ARGUMENT;
    }

    *ioAttrib = mParmElement->mAttrib;

    return NOERROR;
}

ECode CParamInfo::IsReturnValue(
    /* [out] */ Boolean* returnValue)
{
    // TODO: Add your code here
    return E_NOT_IMPLEMENTED;
}

ECode CParamInfo::GetTypeInfo(
    /* [out] */ IDataTypeInfo** typeInfo)
{
    if (!typeInfo) {
        return E_INVALID_ARGUMENT;
    }

    if (mParmElement->mType == CarDataType_LocalPtr) {
        return g_objInfoList.AcquireLocalPtrInfo(mClsModule,
                &mParamDescriptor->type,
                mParmElement->mPointer, (ILocalPtrInfo **)typeInfo);
    }
    else {
        return g_objInfoList.AcquireDataTypeInfo(mClsModule,
                &mParamDescriptor->type, typeInfo);
    }
}

ECode CParamInfo::GetAdvisedCapacity(
    /* [out] */ Int32* advisedCapacity)
{
    if (!advisedCapacity) {
        return E_INVALID_ARGUMENT;
    }

    if (mParmElement->mType != CarDataType_ArrayOf) {
        return E_INVALID_OPERATION;
    }

    if (mParamDescriptor->type.nSize <= 0) {
        //if the size of carquient isn't assigned, then it's -1;
        return E_INVALID_OPERATION;
    }

    *advisedCapacity = mParamDescriptor->type.nSize;

    return NOERROR;
}

ECode CParamInfo::IsUsingTypeAlias(
    /* [out] */ Boolean* usingTypeAlias)
{
    if (!usingTypeAlias) {
        return E_INVALID_ARGUMENT;
    }

    UInt32 index = mParamDescriptor->type.sIndex;
    if ((mParamDescriptor->type.type == Type_alias)
        && !IsSysAlaisType(mClsModule, index)) {
        *usingTypeAlias = TRUE;
    }
    else {
        *usingTypeAlias = FALSE;
    }

    return NOERROR;
}

ECode CParamInfo::GetUsedTypeAliasInfo(
    /* [out] */ ITypeAliasInfo** usedTypeAliasInfo)
{
    if (!usedTypeAliasInfo) {
        return E_INVALID_ARGUMENT;
    }

    UInt32 index = mParamDescriptor->type.sIndex;
    if ((mParamDescriptor->type.type != Type_alias) ||
        IsSysAlaisType(mClsModule, index)) {
        return E_DOES_NOT_EXIST;
    }

    *usedTypeAliasInfo = NULL;
    return g_objInfoList.AcquireTypeAliasInfo(mClsModule,
            getAliasDirAddr(mClsModule->mBase,
                    mClsModule->mClsMod->ppAliasDir, index),
            (IInterface **)usedTypeAliasInfo);
}
