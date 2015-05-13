//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "lube.h"

int CondWithInterface(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    InterfaceDirEntry *pOrigInterface, *pOrigParent;

    if (0 == pCtx->m_pModule->cInterfaces) return LUBE_OK;

    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;

    if (bFirst) {
        pCtx->m_pInterface = pCtx->m_pFirstInterface;
    }
    else {
        pCtx->m_pInterface = pCtx->m_pLastInterface;
    }
    pCtx->m_pIntfParent = pCtx->m_pModule-> \
            ppInterfaceDir[pCtx->m_pInterface->pDesc->sParentIndex];
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;
    return nRet;
}

int CondWithClass(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ClassDirEntry *pOrigClass, *pOrigParent;

    if (0 == pCtx->m_pModule->cClasses) return LUBE_OK;

    pOrigClass = pCtx->m_pClass;
    pOrigParent = pCtx->m_pClassParent;

    if (bFirst) {
        pCtx->m_pClass = pCtx->m_pFirstClass;
    }
    else {
        pCtx->m_pClass = pCtx->m_pLastClass;
    }
    if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
        pCtx->m_pClassParent = pCtx->m_pModule-> \
                ppClassDir[pCtx->m_pClass->pDesc->sParentIndex];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pClassParent = pOrigParent;
    pCtx->m_pClass = pOrigClass;
    return nRet;
}

int CondWithSuper(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ClassDirEntry *pOrigClass, *pOrigParent;

    if (0 == pCtx->m_pModule->cClasses) return LUBE_OK;

    pOrigClass = pCtx->m_pClass;
    pOrigParent = pCtx->m_pClassParent;

    if (bFirst) {
        pCtx->m_pClass = pCtx->m_pFirstClass;
    }
    else {
        pCtx->m_pClass = pCtx->m_pLastClass;
    }

    if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
        pCtx->m_pClassParent = pCtx->m_pModule-> \
                ppClassDir[pCtx->m_pClass->pDesc->sParentIndex];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pClassParent = pOrigParent;
    pCtx->m_pClass = pOrigClass;
    return nRet;
}

int CondWithStruct(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    StructDirEntry *pOrigStruct;

    if (0 == pCtx->m_pModule->cStructs) return LUBE_OK;

    pOrigStruct = pCtx->m_pStruct;

    if (bFirst) {
        pCtx->m_pStruct = pCtx->m_pFirstStruct;
    }
    else {
        pCtx->m_pStruct = pCtx->m_pLastStruct;
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pStruct = pOrigStruct;
    return nRet;
}

int CondWithEnum(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    EnumDirEntry *pOrigEnum;

    if (0 == pCtx->m_pModule->cEnums) return LUBE_OK;

    pOrigEnum = pCtx->m_pEnum;

    if (bFirst) {
        pCtx->m_pEnum = pCtx->m_pFirstEnum;
    }
    else {
        pCtx->m_pEnum = pCtx->m_pLastEnum;
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pEnum = pOrigEnum;
    return nRet;
}

int CondWithConst(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ConstDirEntry *pOrigConst;

    if (0 == pCtx->m_pModule->cConsts) return LUBE_OK;

    pOrigConst = pCtx->m_pConst;

    if (bFirst) {
        pCtx->m_pConst = pCtx->m_pFirstConst;
    }
    else {
        pCtx->m_pConst = pCtx->m_pLastConst;
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pConst = pOrigConst;
    return nRet;
}

int CondWithTypedef(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    AliasDirEntry *pOrigTypedef;

    if (0 == pCtx->m_pModule->cAliases) return LUBE_OK;

    pOrigTypedef = pCtx->m_pTypedef;

    if (bFirst) {
        pCtx->m_pTypedef = pCtx->m_pFirstTypedef;
    }
    else {
        pCtx->m_pTypedef = pCtx->m_pLastTypedef;
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pTypedef = pOrigTypedef;
    return nRet;
}

int CondWithAspect(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ClassDirEntry *pOrigAspect;

    if (!(pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_aspect)) {
        pCtx->ErrMsg("No aspects defined with class %s.",
                pCtx->m_pClass->pszName);
        return LUBE_FAIL;
    }
    pOrigAspect = pCtx->m_pAspect;

    if (bFirst) {
        pCtx->m_pAspect = pCtx->m_pModule->ppClassDir
                [pCtx->m_pClass->pDesc->pAspectIndexs[0]];
    }
    else {
        pCtx->m_pAspect = pCtx->m_pModule->ppClassDir[pCtx->m_pClass-> \
                pDesc->pAspectIndexs[pCtx->m_pClass->pDesc->cAspects - 1]];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pAspect = pOrigAspect;
    return nRet;
}

int CondWithClassForAspect(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ClassDirEntry *pForClass;

    if (!(pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_specialAspect)) {
        pCtx->ErrMsg("No class specified with aspect %s.",
                pCtx->m_pClass->pszName);
        return LUBE_FAIL;
    }
    pForClass = pCtx->m_pForClass;

    if (bFirst) {
        pCtx->m_pForClass = pCtx->m_pModule->ppClassDir
                [pCtx->m_pClass->pDesc->pClassIndexs[0]];
    }
    else {
        pCtx->m_pForClass = pCtx->m_pModule->ppClassDir[pCtx->m_pClass-> \
                pDesc->pClassIndexs[pCtx->m_pClass->pDesc->cClasses - 1]];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pForClass = pForClass;
    return nRet;
}

int CondWithAggregate(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ClassDirEntry *pOrigAggregate;

    if (!(pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_aggregate)) {
        pCtx->ErrMsg("No aggregates defined with class %s.",
                pCtx->m_pClass->pszName);
        return LUBE_FAIL;
    }
    pOrigAggregate = pCtx->m_pAggregate;

    if (bFirst) {
        pCtx->m_pAggregate = pCtx->m_pModule->ppClassDir
                [pCtx->m_pClass->pDesc->pAggrIndexs[0]];
    }
    else {
        pCtx->m_pAggregate = pCtx->m_pModule->ppClassDir[pCtx->m_pClass-> \
                pDesc->pAggrIndexs[pCtx->m_pClass->pDesc->cAggregates - 1]];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pAggregate = pOrigAggregate;
    return nRet;
}

int CondWithLibrary(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    char *pOrigLib;

    if (0 == pCtx->m_pModule->cLibraries) return LUBE_OK;

    pOrigLib = pCtx->m_pLibrary;

    if (bFirst) {
        pCtx->m_pLibrary = pCtx->m_pFirstLibrary;
    }
    else {
        pCtx->m_pLibrary = pCtx->m_pLastLibrary;
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pLibrary = pOrigLib;
    return nRet;
}

int _CondWithMethod(
    PLUBECTX pCtx, PSTATEDESC pDesc,
    BOOL bFirst, InterfaceDirEntry *pIntf)
{
    int nRet;
    MethodDescriptor *pOrigMethod;

    pOrigMethod = pCtx->m_pMethod;

    if (bFirst) {
        pCtx->m_pMethod = pIntf->pDesc->ppMethods[0];
    }
    else {
        pCtx->m_pMethod = pIntf->pDesc->ppMethods[pIntf->pDesc->cMethods - 1];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pMethod = pOrigMethod;
    return nRet;
}

int CondWithIntfMethod(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    return _CondWithMethod(pCtx, pDesc, bFirst, pCtx->m_pInterface);
}

int CondWithIParentMtd(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    return _CondWithMethod(pCtx, pDesc, bFirst, pCtx->m_pIntfParent);
}

int CondWithParam(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ParamDescriptor *pOrigParam;

    if (0 == pCtx->m_pMethod->cParams) return LUBE_OK;

    pOrigParam = pCtx->m_pParam;

    if (bFirst) {
        pCtx->m_pParam = pCtx->m_pMethod->ppParams[0];
    }
    else {
        pCtx->m_pParam = pCtx->m_pMethod->ppParams
                [pCtx->m_pMethod->cParams - 1];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pParam = pOrigParam;
    return nRet;
}

int CondWithStcMember(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    StructElement *pOrigMember;

    pOrigMember = pCtx->m_pStructMember;

    if (bFirst) {
        pCtx->m_pStructMember = pCtx->m_pStruct->pDesc->ppElems[0];
    }
    else {
        pCtx->m_pStructMember = pCtx->m_pStruct->pDesc-> \
                ppElems[pCtx->m_pStruct->pDesc->cElems - 1];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pStructMember = pOrigMember;
    return nRet;
}

int CondWithEnumMember(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    EnumElement *pOrigMember;

    pOrigMember = pCtx->m_pEnumMember;

    if (bFirst) {
        pCtx->m_pEnumMember = pCtx->m_pEnum->pDesc->ppElems[0];
    }
    else {
        pCtx->m_pEnumMember = pCtx->m_pEnum->pDesc-> \
                ppElems[pCtx->m_pEnum->pDesc->cElems - 1];
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pEnumMember = pOrigMember;
    return nRet;
}

int CondWithClsIntf(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    int nRet;
    ClassInterface *pOrigClsIntf;
    InterfaceDirEntry *pOrigInterface, *pOrigParent;

    pOrigClsIntf = pCtx->m_pClsIntf;
    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;

    if (bFirst) {
        pCtx->m_pClsIntf = pCtx->m_pClass->pDesc->ppInterfaces[0];
    }
    else {
        pCtx->m_pClsIntf = pCtx->m_pClass->pDesc->ppInterfaces
                [pCtx->m_pClass->pDesc->cInterfaces - 1];
    }
    pCtx->m_pInterface = pCtx->m_pModule->ppInterfaceDir
                    [pCtx->m_pClsIntf->sIndex];
    pCtx->m_pIntfParent = pCtx->m_pModule-> \
            ppInterfaceDir[pCtx->m_pInterface->pDesc->sParentIndex];
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;
    pCtx->m_pClsIntf = pOrigClsIntf;
    return nRet;
}

int CondWith(PLUBECTX pCtx, PSTATEDESC pDesc, BOOL bFirst)
{
    switch (pDesc->object) {
        case Object_Class:
            return CondWithClass(pCtx, pDesc, bFirst);
        case Object_Super:
            return CondWithSuper(pCtx, pDesc, bFirst);
        case Object_Interface:
            return CondWithInterface(pCtx, pDesc, bFirst);
        case Object_Struct:
            return CondWithStruct(pCtx, pDesc, bFirst);
        case Object_Enum:
            return CondWithEnum(pCtx, pDesc, bFirst);
        case Object_Const:
            return CondWithConst(pCtx, pDesc, bFirst);
        case Object_Typedef:
            return CondWithTypedef(pCtx, pDesc, bFirst);
        case Object_IntfMethod:
            return CondWithIntfMethod(pCtx, pDesc, bFirst);
        case Object_Param:
            return CondWithParam(pCtx, pDesc, bFirst);
        case Object_StcMember:
            return CondWithStcMember(pCtx, pDesc, bFirst);
        case Object_EnumMember:
            return CondWithEnumMember(pCtx, pDesc, bFirst);
        case Object_ClsIntf:
            return CondWithClsIntf(pCtx, pDesc, bFirst);
        case Object_Aspect:
            return CondWithAspect(pCtx, pDesc, bFirst);
        case Object_Aggregate:
            return CondWithAggregate(pCtx, pDesc, bFirst);
        case Object_ClassForAspect:
            return CondWithClassForAspect(pCtx, pDesc, bFirst);
        case Object_IParentMtd:
            return CondWithIParentMtd(pCtx, pDesc, bFirst);
        case Object_Library:
            return CondWithLibrary(pCtx, pDesc, bFirst);
        case Object_Parent:
        case Object_ClassMethod:
        case Object_ClassConstructor:
        case Object_CParentMtd:
        case Object_CbCoalescence:
        default:
            pCtx->ErrMsg("The specified object in with has no order.");
            return LUBE_FAIL;
    }
}

int WithClassParent(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int nRet;
    ClassDirEntry *pOrigClass, *pOrigParent;

    if (NULL == pCtx->m_pClassParent) return LUBE_OK;

    pOrigClass = pCtx->m_pClass;
    pOrigParent = pCtx->m_pClassParent;

    pCtx->m_pClass = pCtx->m_pClassParent;

    if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
        pCtx->m_pClassParent = pCtx->m_pModule-> \
                ppClassDir[pCtx->m_pClass->pDesc->sParentIndex];
    }
    else {
        pCtx->m_pClassParent = NULL;
    }
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pClassParent = pOrigParent;
    pCtx->m_pClass = pOrigClass;
    return nRet;
}

int WithIntfParent(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int nRet;
    InterfaceDirEntry *pOrigInterface, *pOrigParent;

    if (0 == pCtx->m_pInterface->pDesc->sParentIndex) {
        return LUBE_OK;
    }
    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;

    pCtx->m_pInterface = pCtx->m_pIntfParent;
    pCtx->m_pIntfParent = pCtx->m_pModule-> \
            ppInterfaceDir[pCtx->m_pInterface->pDesc->sParentIndex];
    nRet = pCtx->ExecStatements(pDesc->pBlockette);

    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;
    return nRet;
}

int DoWith(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    switch (pDesc->object) {
        case Object_Super:
            return WithClassParent(pCtx, pDesc);
        case Object_Parent:
            return WithIntfParent(pCtx, pDesc);
        default:
            return pCtx->ExecStatements(pDesc->pBlockette);
    }
}

IMPL_STATEMENT(With)(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(Member_None == pDesc->member);

    if (0 == pDesc->dwCondition) {
        return DoWith(pCtx, pDesc);
    }
    return CondWith(pCtx, pDesc, Condition_First == pDesc->dwCondition);
}
