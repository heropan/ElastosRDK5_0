//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include "lube.h"

int WithAllLibrary(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    char *pOrigLib = pCtx->m_pLibrary;

    for (n = 0; n < pCtx->m_pModule->mLibraryCount; n++) {
        pCtx->m_pLibrary = pCtx->m_pModule->mLibraryNames[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }

    pCtx->m_pLibrary = pOrigLib;
    return nRet;
}

int WithAllClass(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    ClassDirEntry *pOrigClass, *pOrigParent;

    pOrigClass = pCtx->m_pClass;
    pOrigParent = pCtx->m_pClassParent;

    for (n = 0; n < pCtx->m_pModule->mClassCount; n++) {
        if (!(pCtx->m_pModule->mClassDirs[n]->pDesc->dwAttribs & ClassAttrib_t_external) &&
            (pCtx->m_pModule->mClassDirs[n]->pszNameSpace == NULL ||
            strcmp("systypes", pCtx->m_pModule->mClassDirs[n]->pszNameSpace))) {
            pCtx->m_pClass = pCtx->m_pModule->mClassDirs[n];
            if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
                pCtx->m_pClassParent = pCtx->m_pModule-> \
                        mClassDirs[pCtx->m_pClass->pDesc->sParentIndex];
            }
            nRet = pCtx->ExecStatements(pDesc->pBlockette);
            if (nRet < 0) break;
        }
    }

    pCtx->m_pClassParent = pOrigParent;
    pCtx->m_pClass = pOrigClass;
    return nRet;
}

int WithAllInterface(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    InterfaceDirEntry *pOrigInterface, *pOrigParent;

    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;

    for (n = 0; n < pCtx->m_pModule->mDefinedInterfaceCount; n++) {
        pCtx->m_pInterface = pCtx->m_pModule->mInterfaceDirs\
                    [pCtx->m_pModule->mDefinedInterfaceIndexes[n]];
        pCtx->m_pIntfParent = pCtx->m_pModule->\
                    mInterfaceDirs[pCtx->m_pInterface->pDesc->sParentIndex];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;
    return nRet;
}

int WithAllClsIntf(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);

    int n, nRet = LUBE_OK;
    ClassInterface *pOrigClsIntf;
    InterfaceDirEntry *pOrigInterface, *pOrigParent;

    pOrigClsIntf = pCtx->m_pClsIntf;
    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;

    for (n = 0; n < pCtx->m_pClass->pDesc->mInterfaceCount; n++) {
        pCtx->m_pClsIntf = pCtx->m_pClass->pDesc->ppInterfaces[n];
        pCtx->m_pInterface = pCtx->m_pModule->mInterfaceDirs
                        [pCtx->m_pClsIntf->sIndex];
        pCtx->m_pIntfParent = pCtx->m_pModule-> \
                mInterfaceDirs[pCtx->m_pInterface->pDesc->sParentIndex];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }

    if (pCtx->m_pClass->pDesc->sCtorIndex) {
        ClassInterface clsCtorInterface;
        memset(&clsCtorInterface, 0, sizeof(ClassInterface));
        clsCtorInterface.sIndex= pCtx->m_pClass->pDesc->sCtorIndex;
        pCtx->m_pClsIntf = &clsCtorInterface;
        pCtx->m_pInterface = pCtx->m_pModule->mInterfaceDirs[pCtx->m_pClass->pDesc->sCtorIndex];
        pCtx->m_pIntfParent = pCtx->m_pModule-> \
                mInterfaceDirs[pCtx->m_pInterface->pDesc->sParentIndex];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
    }

    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;
    pCtx->m_pClsIntf = pOrigClsIntf;
    return nRet;
}

int WithAllStruct(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    StructDirEntry *pOrigStruct;

    pOrigStruct = pCtx->m_pStruct;

    for (n = 0; n < pCtx->m_pModule->mStructCount; n++) {
        if (!(pCtx->m_pModule->mStructDirs[n]->pDesc->mR.mReserved & StructAttrib_t_external) &&
            (pCtx->m_pModule->mStructDirs[n]->pszNameSpace == NULL ||
            strcmp("systypes", pCtx->m_pModule->mStructDirs[n]->pszNameSpace))) {
            pCtx->m_pStruct = pCtx->m_pModule->mStructDirs[n];
            nRet = pCtx->ExecStatements(pDesc->pBlockette);
            if (nRet < 0) break;
        }
    }
    pCtx->m_pStruct = pOrigStruct;
    return nRet;
}

int WithAllEnum(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    EnumDirEntry *pOrigEnum;

    pOrigEnum = pCtx->m_pEnum;

    for (n = 0; n < pCtx->m_pModule->mEnumCount; n++) {
        if (!(pCtx->m_pModule->mEnumDirs[n]->pDesc->mR.mReserved & EnumAttrib_t_external) &&
            (pCtx->m_pModule->mEnumDirs[n]->pszNameSpace == NULL ||
            strcmp("systypes", pCtx->m_pModule->mEnumDirs[n]->pszNameSpace))) {
            pCtx->m_pEnum = pCtx->m_pModule->mEnumDirs[n];
            nRet = pCtx->ExecStatements(pDesc->pBlockette);
            if (nRet < 0) break;
        }
    }
    pCtx->m_pEnum = pOrigEnum;
    return nRet;
}

int WithAllConst(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    ConstDirEntry *pOrigConst;

    pOrigConst = pCtx->m_pConst;

    for (n = 0; n < pCtx->m_pModule->mConstCount; n++) {
        if (NULL == pCtx->m_pModule->mConstDirs[n]->pszNameSpace) {
            pCtx->m_pConst = pCtx->m_pModule->mConstDirs[n];
            nRet = pCtx->ExecStatements(pDesc->pBlockette);
            if (nRet < 0) break;
        }
    }
    pCtx->m_pConst = pOrigConst;
    return nRet;
}

int WithAllTypedef(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int n, nRet = LUBE_OK;
    AliasDirEntry *pOrigTypedef;

    pOrigTypedef = pCtx->m_pTypedef;

    for (n = 0; n < pCtx->m_pModule->mAliasCount; n++) {
        if (NULL == pCtx->m_pModule->mAliasDirs[n]->pszNameSpace) {
            pCtx->m_pTypedef = pCtx->m_pModule->mAliasDirs[n];
            nRet = pCtx->ExecStatements(pDesc->pBlockette);
            if (nRet < 0) break;
        }
    }
    pCtx->m_pTypedef = pOrigTypedef;
    return nRet;
}

int _WithAllIntfConst(
    PLUBECTX pCtx, PSTATEDESC pDesc, InterfaceDirEntry *pIntf)
{
    int n, nRet = LUBE_OK;
    InterfaceConstDescriptor *pOrigInterfaceConst = pCtx->m_pInterfaceConst;

    for (n = 0; n < pIntf->pDesc->mConstCount; n++) {
        pCtx->m_pInterfaceConst = pIntf->pDesc->ppConsts[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }

    pCtx->m_pInterfaceConst = pOrigInterfaceConst;
    return nRet;
}

int WithAllIntfConst(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pInterface);
    return _WithAllIntfConst(pCtx, pDesc, pCtx->m_pInterface);
}

int _WithAllIntfMethod(
    PLUBECTX pCtx, PSTATEDESC pDesc, InterfaceDirEntry *pIntf)
{
    int n, nRet = LUBE_OK;
    MethodDescriptor *pOrigMethod = pCtx->m_pMethod;

    for (n = 0; n < pIntf->pDesc->cMethods; n++) {
        pCtx->m_pMethod = pIntf->pDesc->ppMethods[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }

    pCtx->m_pMethod = pOrigMethod;
    return nRet;
}

int WithAllIntfMethod(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pInterface);
    return _WithAllIntfMethod(pCtx, pDesc, pCtx->m_pInterface);
}

int WithAllIParentMtd(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pIntfParent);
    return _WithAllIntfMethod(pCtx, pDesc, pCtx->m_pIntfParent);
}

static int s_cCheckedIntfs = 0;
static int s_checkedIntfs[MAX_INTERFACE_NUMBER];

BOOL IsCheckedInterface(int nIndex)
{
    int n;

    for (n = 0; n < s_cCheckedIntfs; n++) {
        if (s_checkedIntfs[n] == nIndex) return TRUE;
    }
    s_checkedIntfs[s_cCheckedIntfs++] = nIndex;
    return FALSE;
}

int WithClsIntfMethod(
    PLUBECTX pCtx, PSTATEDESC pDesc, InterfaceDescriptor *pIntf)
{
    int n, nRet = LUBE_OK;

    if (0 != pIntf->sParentIndex) {
        if (!IsCheckedInterface(pIntf->sParentIndex)) {
            WithClsIntfMethod(pCtx, pDesc,
                pCtx->m_pModule->mInterfaceDirs[pIntf->sParentIndex]->pDesc);
        }
    }

    for (n = 0; n < pIntf->cMethods; n++) {
        pCtx->m_pMethod = pIntf->ppMethods[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    return nRet;
}

int _WithAllClassMethod(
    PLUBECTX pCtx, PSTATEDESC pDesc, ClassDirEntry *pClass)
{
    int n, nRet = LUBE_OK;
    InterfaceDirEntry *pOrigIntf = pCtx->m_pInterface;
    MethodDescriptor *pOrigMethod = pCtx->m_pMethod;

    for (n = 0; n < pClass->pDesc->mInterfaceCount; n++) {
        pCtx->m_pInterface = pCtx->m_pModule->mInterfaceDirs
                [pClass->pDesc->ppInterfaces[n]->sIndex];
        if (!(pClass->pDesc->ppInterfaces[n]->wAttribs
                                           & ClassInterfaceAttrib_callback)
            &&!(pClass->pDesc->ppInterfaces[n]->wAttribs
                                           & ClassInterfaceAttrib_filter)
            && (strcmp("IRegime", pCtx->m_pInterface->pszName) != 0)) {
            nRet = WithClsIntfMethod(pCtx, pDesc, pCtx->m_pInterface->pDesc);
            if (nRet < 0) break;
        }
    }
    s_cCheckedIntfs = 0;

    pCtx->m_pMethod = pOrigMethod;
    pCtx->m_pInterface = pOrigIntf;
    return nRet;
}

int WithAllClassMethod(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);
    return _WithAllClassMethod(pCtx, pDesc, pCtx->m_pClass);
}

int WithAllClassCtorMethod(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);

    int n, nRet = LUBE_OK;
    ClassDescriptor *pClsDesc;
    InterfaceDescriptor *pIntfDesc;

    pClsDesc = pCtx->m_pClass->pDesc;
    pIntfDesc = pCtx->m_pModule->mInterfaceDirs[pClsDesc->sCtorIndex]->pDesc;

    for (n = 0; n < pIntfDesc->cMethods; n++) {
        pCtx->m_pMethod = pIntfDesc->ppMethods[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }

    return nRet;
}

int _WithAllCoalescenceMethod(
    PLUBECTX pCtx, PSTATEDESC pDesc, ClassDirEntry *pClass)
{
    int m, n, nRet = LUBE_OK;
    InterfaceDirEntry *pOrigIntf = pCtx->m_pInterface;
    MethodDescriptor *pOrigMethod = pCtx->m_pMethod;

    for (n = 0; n < pClass->pDesc->mInterfaceCount; n++) {
        pCtx->m_pInterface = pCtx->m_pModule->mInterfaceDirs
                [pClass->pDesc->ppInterfaces[n]->sIndex];
        if (pClass->pDesc->ppInterfaces[n]->wAttribs
                                           & ClassInterfaceAttrib_callback) {
            for (m = 0; m < pCtx->m_pInterface->pDesc->cMethods; ++m) {
                pCtx->m_pMethod = pCtx->m_pInterface->pDesc->ppMethods[m];
                if (pCtx->m_pMethod->dwAttribs & MethodAttrib_WithCoalescence) {
                    nRet = pCtx->ExecStatements(pDesc->pBlockette);
                    if (nRet < 0) break;
                }
            }
        }
    }
    s_cCheckedIntfs = 0;

    pCtx->m_pMethod = pOrigMethod;
    pCtx->m_pInterface = pOrigIntf;
    return nRet;
}

int WithAllCoalescenceMethod(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);
    return _WithAllCoalescenceMethod(pCtx, pDesc, pCtx->m_pClass);
}

int WithAllCParentMtd(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClassParent);
    return _WithAllClassMethod(pCtx, pDesc, pCtx->m_pClassParent);
}

int WithAllParam(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pMethod);

    int n, nRet = LUBE_OK;
    ParamDescriptor *pOrigParam;

    pOrigParam = pCtx->m_pParam;

    for (n = 0; n < pCtx->m_pMethod->cParams; n++) {
        pCtx->m_pParam = pCtx->m_pMethod->ppParams[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pParam = pOrigParam;
    return nRet;
}

int WithAllStcMember(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pStruct);

    int n, nRet = LUBE_OK;
    StructElement *pOrigMember;

    pOrigMember = pCtx->m_pStructMember;

    for (n = 0; n < pCtx->m_pStruct->pDesc->cElems; n++) {
        pCtx->m_pStructMember = pCtx->m_pStruct->pDesc->ppElems[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pStructMember = pOrigMember;
    return nRet;
}

int WithAllEnumMember(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pEnum);

    int n, nRet = LUBE_OK;
    EnumElement *pOrigMember;

    pOrigMember = pCtx->m_pEnumMember;

    for (n = 0; n < pCtx->m_pEnum->pDesc->cElems; n++) {
        pCtx->m_pEnumMember = pCtx->m_pEnum->pDesc->ppElems[n];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pEnumMember = pOrigMember;
    return nRet;
}

int WithAllAspect(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);

    int n, nRet = LUBE_OK;
    ClassDirEntry *pOrigAspect;

    pOrigAspect = pCtx->m_pAspect;

    for (n = 0; n < pCtx->m_pClass->pDesc->cAspects; n++) {
        pCtx->m_pAspect = pCtx->m_pModule->mClassDirs
                        [pCtx->m_pClass->pDesc->pAspectIndexs[n]];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pAspect = pOrigAspect;
    return nRet;
}

int WithAllAggregate(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);

    int n, nRet = LUBE_OK;
    ClassDirEntry *pOrigAggregate;

    pOrigAggregate = pCtx->m_pAggregate;

    for (n = 0; n < pCtx->m_pClass->pDesc->cAggregates; n++) {
        pCtx->m_pAggregate = pCtx->m_pModule->mClassDirs
                    [pCtx->m_pClass->pDesc->pAggrIndexs[n]];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pAggregate = pOrigAggregate;
    return nRet;
}

int WithAllClassForAspect(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);

    int n, nRet = LUBE_OK;
    ClassDirEntry *pForClass;

    pForClass = pCtx->m_pForClass;

    for (n = 0; n < pCtx->m_pClass->pDesc->cClasses; n++) {
        pCtx->m_pForClass = pCtx->m_pModule->mClassDirs
                    [pCtx->m_pClass->pDesc->pClassIndexs[n]];
        nRet = pCtx->ExecStatements(pDesc->pBlockette);
        if (nRet < 0) break;
    }
    pCtx->m_pForClass = pForClass;
    return nRet;
}

int _WithAllClassParent(
    PLUBECTX pCtx, PSTATEDESC pDesc, ClassDirEntry *pClass)
{
    ClassDirEntry *pParent;

    if (pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
        pParent = pCtx->m_pModule->mClassDirs[pClass->pDesc->sParentIndex];
        if (_WithAllClassParent(pCtx, pDesc, pParent) < 0) return LUBE_FAIL;
        pCtx->m_pClassParent = pParent;
        return pCtx->ExecStatements(pDesc->pBlockette);
    }

    return LUBE_OK;
}

inline int WithAllClassParent(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int nRet;
    ClassDirEntry *pOrigParent;
    ClassDirEntry *pOrigFirst;
    ClassDirEntry *pOrigLast;
    ClassDirEntry *pOrigClass;

    pOrigFirst = pCtx->m_pFirstClass;
    pOrigLast = pCtx->m_pLastClass;
    pOrigClass = pCtx->m_pClass;
    pOrigParent = pCtx->m_pClassParent;
    pCtx->m_pOrigClass = pOrigClass;

    if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
        pCtx->m_pClass = pCtx->m_pModule->mClassDirs[pCtx->m_pClass->pDesc->sParentIndex];
        pCtx->m_pFirstClass = pCtx->m_pClass;
        pCtx->m_pLastClass = pCtx->m_pClass;
        while(pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
            pCtx->m_pClass = pCtx->m_pModule->mClassDirs[pCtx->m_pClass->pDesc->sParentIndex];
            if (pCtx->m_pClass) pCtx->m_pFirstClass = pCtx->m_pClass;
            else break;
        }
    }

    pCtx->m_pClass = pOrigClass;
    nRet = _WithAllClassParent(pCtx, pDesc, pCtx->m_pClass);

    pCtx->m_pClassParent = pOrigParent;
    pCtx->m_pClass = pOrigClass;
    pCtx->m_pFirstClass = pOrigFirst;
    pCtx->m_pLastClass = pOrigLast;
    pCtx->m_pOrigClass = NULL;
    return nRet;
}

int _WithAllIntfParent(
    PLUBECTX pCtx, PSTATEDESC pDesc, InterfaceDirEntry *pIntf)
{
    InterfaceDirEntry *pParent;

    if (0 != pIntf->pDesc->sParentIndex) {
        pParent = pCtx->m_pModule->mInterfaceDirs[pIntf->pDesc->sParentIndex];
        if (_WithAllIntfParent(pCtx, pDesc, pParent) < 0) return LUBE_FAIL;
        pCtx->m_pIntfChild = pCtx->m_pInterface;
        pCtx->m_pInterface = pParent;
        pCtx->m_pIntfParent = pCtx->m_pModule->
                        mInterfaceDirs[pParent->pDesc->sParentIndex];
        int nRet = pCtx->ExecStatements(pDesc->pBlockette);

        pCtx->m_pIntfParent = pCtx->m_pInterface;
        pCtx->m_pInterface = pCtx->m_pIntfChild;

        return nRet;
    }
    return LUBE_OK;
}

inline int WithAllIntfParent(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    int nRet;
    InterfaceDirEntry *pOrigParent, *pOrigInterface;

    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;
    nRet = _WithAllIntfParent(pCtx, pDesc, pCtx->m_pInterface);
    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;

    return nRet;
}

int _WithOneClsIntfAndAllItsParents(
    PLUBECTX pCtx, PSTATEDESC pDesc, InterfaceDirEntry *pIntf)
{
    InterfaceDirEntry *pParent;

    if (pIntf->pDesc->sParentIndex != 0) {
        pParent = pCtx->m_pModule->mInterfaceDirs[pIntf->pDesc->sParentIndex];
        if (_WithOneClsIntfAndAllItsParents(pCtx, pDesc, pParent) < 0) {
            return LUBE_FAIL;
        }
        pCtx->m_pInterface = pIntf;
        pCtx->m_pIntfParent = pParent;
        return pCtx->ExecStatements(pDesc->pBlockette);
    }
    else {
        pCtx->m_pIntfParent = pCtx->m_pModule->\
                mInterfaceDirs[pIntf->pDesc->sParentIndex];
        pCtx->m_pInterface = pIntf;
        return pCtx->ExecStatements(pDesc->pBlockette);
    }
    return LUBE_OK;
}

int WithOneClsIntfAndAllItsParents(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pCtx->m_pClass);
    int nRet = LUBE_OK;
    ClassInterface *pOrigClsIntf;
    InterfaceDirEntry *pOrigInterface, *pOrigParent;

    pOrigClsIntf = pCtx->m_pClsIntf;
    pOrigInterface = pCtx->m_pInterface;
    pOrigParent = pCtx->m_pIntfParent;

    if (strcmp(pCtx->m_pIntfParent->pszName, "IInterface")) {
        nRet = _WithOneClsIntfAndAllItsParents(pCtx, pDesc, pCtx->m_pInterface);
    }
    else {
        pCtx->ExecStatements(pDesc->pBlockette);
    }

    pCtx->m_pIntfParent = pOrigParent;
    pCtx->m_pInterface = pOrigInterface;
    pCtx->m_pClsIntf = pOrigClsIntf;
    return nRet;
}

IMPL_STATEMENT(WithAll)(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(Member_None == pDesc->member);

    if (NULL == pDesc->pBlockette) return LUBE_OK;

    switch (pDesc->object) {
        case Object_Library:
            return WithAllLibrary(pCtx, pDesc);
        case Object_Class:
            return WithAllClass(pCtx, pDesc);
        case Object_Interface:
            return WithAllInterface(pCtx, pDesc);
        case Object_ClsIntf:
            return WithAllClsIntf(pCtx, pDesc);
        case Object_Struct:
            return WithAllStruct(pCtx, pDesc);
        case Object_Enum:
            return WithAllEnum(pCtx, pDesc);
        case Object_Const:
            return WithAllConst(pCtx, pDesc);
        case Object_Typedef:
            return WithAllTypedef(pCtx, pDesc);
        case Object_ClassMethod:
            return WithAllClassMethod(pCtx, pDesc);
        case Object_ClassConstructor:
            return WithAllClassCtorMethod(pCtx, pDesc);
        case Object_IntfConst:
            return WithAllIntfConst(pCtx, pDesc);
        case Object_IntfMethod:
            return WithAllIntfMethod(pCtx, pDesc);
        case Object_Param:
            return WithAllParam(pCtx, pDesc);
        case Object_StcMember:
            return WithAllStcMember(pCtx, pDesc);
        case Object_EnumMember:
            return WithAllEnumMember(pCtx, pDesc);
        case Object_Aspect:
            return WithAllAspect(pCtx, pDesc);
        case Object_Aggregate:
            return WithAllAggregate(pCtx, pDesc);
        case Object_ClassForAspect:
            return WithAllClassForAspect(pCtx, pDesc);
        case Object_Super:
            return WithAllClassParent(pCtx, pDesc);
        case Object_Parent:
            return WithAllIntfParent(pCtx, pDesc);
        case Object_ClsIntfAndParents:
            return WithOneClsIntfAndAllItsParents(pCtx, pDesc);
        case Object_CParentMtd:
            return WithAllCParentMtd(pCtx, pDesc);
        case Object_IParentMtd:
            return WithAllIParentMtd(pCtx, pDesc);
        case Object_CbCoalescence:
            return WithAllCoalescenceMethod(pCtx, pDesc);
        default:
            assert(TRUE == FALSE);
            return LUBE_FAIL;
    }
}
