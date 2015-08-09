//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <lube.h>
#include <ctype.h>

inline BOOL LibraryCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pLibrary == pCtx->m_pFirstLibrary;
    }
    return pCtx->m_pLibrary == pCtx->m_pLastLibrary;
}

inline BOOL ClassCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pClass == pCtx->m_pFirstClass;
    }
    return pCtx->m_pClass == pCtx->m_pLastClass;
}

inline BOOL SuperCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pClassParent== pCtx->m_pFirstClass;
    }
    return pCtx->m_pClassParent == pCtx->m_pLastClass;
}

inline BOOL InterfaceCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pInterface == pCtx->m_pFirstInterface;
    }
    return pCtx->m_pInterface == pCtx->m_pLastInterface;
}

inline BOOL StructCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pStruct == pCtx->m_pFirstStruct;
    }
    return pCtx->m_pStruct == pCtx->m_pLastStruct;
}

inline BOOL EnumCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pEnum == pCtx->m_pFirstEnum;
    }
    return pCtx->m_pEnum == pCtx->m_pLastEnum;
}

inline BOOL TypedefCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pTypedef == pCtx->m_pFirstTypedef;
    }
    return pCtx->m_pTypedef == pCtx->m_pLastTypedef;
}

inline BOOL InterfaceConstCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pInterfaceConst == pCtx->m_pInterface->pDesc->ppConsts[0];
    }
    return pCtx->m_pInterfaceConst == pCtx->m_pInterface->pDesc->ppConsts
                [pCtx->m_pInterface->pDesc->mConstCount - 1];
}

inline BOOL MethodCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pMethod == pCtx->m_pInterface->pDesc->ppMethods[0];
    }
    return pCtx->m_pMethod == pCtx->m_pInterface->pDesc->ppMethods
                [pCtx->m_pInterface->pDesc->cMethods - 1];
}

inline BOOL ParamCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pParam == pCtx->m_pMethod->ppParams[0];
    }
    return pCtx->m_pParam == pCtx->m_pMethod->ppParams
                [pCtx->m_pMethod->cParams - 1];
}

void FirstLastClsIntf(
    CLSModule *pModule, ClassDescriptor *pClass,
    ClassInterface **ppFirst, ClassInterface **ppLast)
{
    int n;
    ClassInterface *pFirst, *pLast;

    pFirst = pLast = NULL;

    for (n = 0; n < pClass->mInterfaceCount; n++) {
        if (!(pClass->ppInterfaces[n]->wAttribs
            & ClassInterfaceAttrib_callback)) {
            pLast = pClass->ppInterfaces[n];
            if (NULL == pFirst) pFirst = pLast;
        }
    }
    *ppLast = pLast;
    *ppFirst = pFirst;
}

inline BOOL ClsIntfCondition(PLUBECTX pCtx, BOOL bFirst)
{
    ClassInterface *pFirst, *pLast;

    FirstLastClsIntf(pCtx->m_pModule, pCtx->m_pClass->pDesc, &pFirst, &pLast);

    if (bFirst) {
        return pCtx->m_pClsIntf == pFirst;
    }
    return pCtx->m_pClsIntf == pLast;
}

inline BOOL AspectCondition(PLUBECTX pCtx, BOOL bFirst)
{
    ClassDescriptor *pDesc;

    pDesc = pCtx->m_pClass->pDesc;
    if (bFirst) {
        return pCtx->m_pAspect == pCtx->m_pModule->ppClassDir
                [pDesc->pAspectIndexs[0]];
    }
    return pCtx->m_pAspect == pCtx->m_pModule->ppClassDir
                [pDesc->pAspectIndexs[pDesc->cAspects - 1]];
}

inline BOOL AggregateCondition(PLUBECTX pCtx, BOOL bFirst)
{
    ClassDescriptor *pDesc;

    pDesc = pCtx->m_pClass->pDesc;
    if (bFirst) {
        return pCtx->m_pAggregate == pCtx->m_pModule->ppClassDir
                    [pDesc->pAggrIndexs[0]];
    }
    return pCtx->m_pAggregate == pCtx->m_pModule->ppClassDir
                    [pDesc->pAggrIndexs[pDesc->cAggregates - 1]];
}

inline BOOL ClassForAspectCondition(PLUBECTX pCtx, BOOL bFirst)
{
    ClassDescriptor *pDesc;

    pDesc = pCtx->m_pClass->pDesc;
    if (bFirst) {
        return pCtx->m_pForClass== pCtx->m_pModule->ppClassDir
                    [pDesc->pClassIndexs[0]];
    }
    return pCtx->m_pForClass == pCtx->m_pModule->ppClassDir
                    [pDesc->pClassIndexs[pDesc->cClasses - 1]];
}

inline BOOL StcMemberCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pStructMember == pCtx->m_pStruct->pDesc->ppElems[0];
    }
    return pCtx->m_pStructMember == pCtx->m_pStruct-> \
                pDesc->ppElems[pCtx->m_pStruct->pDesc->cElems - 1];
}

inline BOOL EnumMemberCondition(PLUBECTX pCtx, BOOL bFirst)
{
    if (bFirst) {
        return pCtx->m_pEnumMember == pCtx->m_pEnum->pDesc->ppElems[0];
    }
    return pCtx->m_pEnumMember == pCtx->m_pEnum-> \
                pDesc->ppElems[pCtx->m_pEnum->pDesc->cElems - 1];
}

BOOL ObjectConditionValue(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert((pDesc->dwCondition & Condition_First)
            || (pDesc->dwCondition & Condition_Last));

    BOOL bValue;
    BOOL bFirst = (pDesc->dwCondition & Condition_First);

    switch (pDesc->object) {
        case Object_Library:
            bValue = LibraryCondition(pCtx, bFirst);
            break;
        case Object_Class:
            bValue = ClassCondition(pCtx, bFirst);
            break;
        case Object_Super:
            bValue = SuperCondition(pCtx, bFirst);
            break;
        case Object_Aspect:
            bValue = AspectCondition(pCtx, bFirst);
            break;
        case Object_Aggregate:
            bValue = AggregateCondition(pCtx, bFirst);
            break;
        case Object_ClassForAspect:
            bValue = ClassForAspectCondition(pCtx, bFirst);
            break;
        case Object_Interface:
            bValue = InterfaceCondition(pCtx, bFirst);
            break;
        case Object_Struct:
            bValue = StructCondition(pCtx, bFirst);
            break;
        case Object_Enum:
            bValue = EnumCondition(pCtx, bFirst);
            break;
        case Object_Typedef:
            bValue = TypedefCondition(pCtx, bFirst);
            break;
        case Object_ClassMethod:
            bValue = FALSE;
            break;
        case Object_IntfConst:
            bValue = InterfaceConstCondition(pCtx, bFirst);
            break;
        case Object_IntfMethod:
            bValue = MethodCondition(pCtx, bFirst);
            break;
        case Object_Param:
            bValue = ParamCondition(pCtx, bFirst);
            break;
        case Object_ClsIntf:
            bValue = ClsIntfCondition(pCtx, bFirst);
            break;
        case Object_StcMember:
            bValue = StcMemberCondition(pCtx, bFirst);
            break;
        case Object_EnumMember:
            bValue = EnumMemberCondition(pCtx, bFirst);
            break;
        default:
            assert(TRUE == FALSE);
            return FALSE;
    }
    return bValue;
}

BOOL NameConditionValue(PLUBECTX pCtx, ObjectType object, const char *pszDest)
{
    const char *pszSource;
    BOOL retVal;

    switch (object) {
        case Object_Module:
            pszSource = pCtx->m_pModule->mName;
            break;
        case Object_Library:
            pszSource = pCtx->m_pLibrary;
            while (*pszSource) {
                if (toupper(*pszSource) != toupper(*pszDest)) break;
                pszSource++;
                pszDest++;
            }
            return ((0 == *pszSource || '.' == *pszSource) && 0 == *pszDest);
        case Object_Class:
            pszSource = pCtx->m_pClass->pszName;
            break;
        case Object_Aspect:
            pszSource = pCtx->m_pAspect->pszName;
            break;
        case Object_Aggregate:
            pszSource = pCtx->m_pAggregate->pszName;
            break;
        case Object_ClassForAspect:
            pszSource = pCtx->m_pForClass->pszName;
            break;
        case Object_ClsIntf:
        case Object_Interface:
            pszSource = pCtx->m_pInterface->pszName;
            break;
        case Object_Struct:
            pszSource = pCtx->m_pStruct->pszName;
            break;
        case Object_Enum:
            pszSource = pCtx->m_pEnum->pszName;
            break;
        case Object_Typedef:
            pszSource = pCtx->m_pTypedef->pszName;
            break;
        case Object_IntfConst:
            pszSource = pCtx->m_pInterfaceConst->pszName;
            break;
        case Object_ClassMethod:
        case Object_IntfMethod:
            pszSource = pCtx->m_pMethod->pszName;
            break;
        case Object_Param:
            pszSource = pCtx->m_pParam->pszName;
            break;
        case Object_StcMember:
            pszSource = pCtx->m_pStructMember->pszName;
            break;
        case Object_EnumMember:
            pszSource = pCtx->m_pEnumMember->pszName;
            break;
        case Object_Parent:
            pszSource = pCtx->m_pIntfParent->pszName;
            break;
        case Object_Super:
            pszSource = pCtx->m_pClassParent->pszName;
            break;
        case Object_LubeOption:
            retVal = false;
            if (!retVal && (pCtx->m_dwOptions & LubeOpt_InKernel)) {
                retVal = !_stricmp("InKernel", pszDest);
            }
            if (!retVal && (pCtx->m_dwOptions & LubeOpt_UseCustomClassObject)) {
                retVal = !_stricmp("UseCustomClassObject", pszDest);
            }
            if (!retVal && (pCtx->m_dwOptions & LubeOpt_HaveSandwichMethod)) {
                retVal = !_stricmp("HaveSandwichMethod", pszDest);
            }
            if (!retVal && (pCtx->m_dwOptions & LubeOpt_UseNakedMode)) {
                retVal = !_stricmp("UseNakedMode", pszDest);
            }
            if (!retVal && (pCtx->m_dwOptions & LubeOpt_WeakRef)) {
                retVal = !_stricmp("WeakReference", pszDest);
            }
            if (!retVal) {
                retVal = !_stricmp("", pszDest);
            }
            return retVal;
        case Object_InuptType:
            if (pCtx->m_dwOptions & LubeOpt_FromExCls) {
                pszSource = "ExCls";
            }
            else pszSource = "Dll";
            break;
        case Object_CompilingEnv:
            if (pCtx->m_dwOptions & LubeOpt_InSDK) {
                pszSource = "InSDK";
            }
            else pszSource = "";
            break;
        default:
            assert(TRUE == FALSE);
            return FALSE;
    }
    return !_stricmp(pszSource, pszDest);
}

BOOL TypeConditionValue(PLUBECTX pCtx, ObjectType object, const char *pszDest)
{
    TypeDescriptor *pType, type;
    const char *pszSource;

    switch (object) {
        case Object_Library:
            pszSource = pCtx->m_pLibrary + strlen(pCtx->m_pLibrary);
            while (pszSource != pCtx->m_pLibrary && *(pszSource - 1) != '.') {
                pszSource--;
            }
            return !_stricmp(pszSource, pszDest);
        case Object_Typedef:
            pType = &pCtx->m_pTypedef->type;
            break;
        case Object_IntfConst:
        {
            if (pCtx->m_pInterfaceConst->type == TYPE_BOOLEAN)
                return !_stricmp("Boolean", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_CHAR32)
                return !_stricmp("Char32", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_BYTE)
                return !_stricmp("Byte", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_INTEGER16)
                return !_stricmp("Int16", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_INTEGER32)
                return !_stricmp("Int32", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_INTEGER64)
                return !_stricmp("Int64", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_FLOAT)
                return !_stricmp("Float", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_DOUBLE)
                return !_stricmp("Double", pszDest);
            else if (pCtx->m_pInterfaceConst->type == TYPE_STRING)
                return !_stricmp("String", pszDest);
            assert(TRUE == FALSE);
            return FALSE;
        }
        case Object_ClassMethod:
        case Object_IntfMethod:
            pType = &pCtx->m_pMethod->type;
            break;
        case Object_Param:
            pType = &pCtx->m_pParam->type;
            break;
        case Object_StcMember:
            pType = &pCtx->m_pStructMember->type;
            break;
        default:
            assert(TRUE == FALSE);
            return FALSE;
    }

    GetOriginalType(pCtx->m_pModule, pType, &type);
    pszSource = Type2CString(pCtx->m_pModule, &type);

    return !_stricmp(pszSource, pszDest);
}

BOOL StringConditionValue(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    char szBuf[c_nStrBufSize];

    if (pCtx->ParseStringEmbed((char *)pDesc->pvData, szBuf) < 0) {
        return FALSE;
    }

    switch (pDesc->member) {
        case Member_Name:
            return NameConditionValue(pCtx, pDesc->object, szBuf);
        case Member_Type:
            return TypeConditionValue(pCtx, pDesc->object, szBuf);
        default:
            assert(TRUE == FALSE);
            return FALSE;
    }
}

BOOL AttribConditionValue(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(pDesc->dwExtra != 0);
    char* pszName;
    int nLength;
    switch (pDesc->object) {
        case Object_Module:
            return pCtx->m_pModule->mAttribs & pDesc->dwExtra;
        case Object_Class:
            return pCtx->m_pClass->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_Aspect:
            return pCtx->m_pAspect->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_Aggregate:
            return pCtx->m_pAggregate->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_ClassForAspect:
            return pCtx->m_pClass->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_ClsIntf:
        case Object_ClsIntfAndParents:
            return pCtx->m_pClsIntf->wAttribs & pDesc->dwExtra;
        case Object_Interface:
            return pCtx->m_pInterface->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_Param:
            return pCtx->m_pParam->dwAttribs & pDesc->dwExtra;
        case Object_Super:
            return pCtx->m_pClassParent->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_Parent:
            return pCtx->m_pIntfParent->pDesc->dwAttribs & pDesc->dwExtra;
        case Object_ClassMethod:
        case Object_IntfMethod:
            return pCtx->m_pMethod->dwAttribs & pDesc->dwExtra;
        case Object_Typedef:
            return !pCtx->m_pTypedef->bDummyType == !pDesc->dwExtra;
        case Object_Enum: // callback enums
            pszName = pCtx->m_pEnum->pDesc->ppElems[pCtx->m_pEnum->pDesc->cElems - 1]->pszName;
            nLength = strlen(pszName);
            if (nLength > 14 && !strcmp(pszName + (nLength - 14), "Sink_MaxEvents")) {
                return TRUE;
            }
            return FALSE;
        default:
            assert(TRUE == FALSE);
            return FALSE;
    }
}

inline BOOL FuncConditionValue(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    BOOL bRet;

    DECL_STATEMENT(Func);
    bRet = (0 != CALL_STATEMENT(Func)(pCtx, pDesc));

    return (pDesc->dwCondition & Condition_True) ? bRet:!bRet;
}

BOOL ConditionValue(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    BOOL bValue;

    if (Condition_Function & pDesc->dwCondition) {
        bValue = FuncConditionValue(pCtx, pDesc);
    }
    else if (Member_None == pDesc->member) {
        bValue = ObjectConditionValue(pCtx, pDesc);
    }
    else if (pDesc->pvData) {
        bValue = StringConditionValue(pCtx, pDesc);
    }
    else {
        bValue = AttribConditionValue(pCtx, pDesc);
    }
    return (pDesc->dwCondition & Condition_Not) ? !bValue:bValue;
}

BOOL DecideCondition(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    assert(NULL != pDesc && State_Condition == pDesc->type);

    BOOL bTrue;

    bTrue = ConditionValue(pCtx, pDesc);
    if (bTrue) {
        if (pDesc->dwCondition & Condition_And) {
            bTrue = DecideCondition(pCtx, pDesc->pNext);
        }
    }
    else {
        while (pDesc->dwCondition & Condition_And) {
            pDesc = pDesc->pNext;
        }
        if (pDesc->dwCondition & Condition_Or) {
            bTrue = DecideCondition(pCtx, pDesc->pNext);
        }
    }
    return bTrue;
}

IMPL_STATEMENT(If)(PLUBECTX pCtx, PSTATEDESC pDesc)
{
    BOOL bTrue;
    StatementFn_t fn;

    pDesc = pDesc->pBlockette;
    bTrue = DecideCondition(pCtx, pDesc);

    // Skip conditions.
    //
    while (pDesc && State_Condition == pDesc->type) {
        pDesc = pDesc->pNext;
    }

    if (bTrue) {
        //
        // Exec to 'ELSE' or 'END IF' for true condition.
        //
        while (pDesc && State_Else != pDesc->type) {
            fn = GetStatementFunc(pDesc->uStateIndex);
            if ((*fn)(pCtx, pDesc) < 0) return LUBE_FAIL;
            pDesc = pDesc->pNext;
        }
    }
    else {
        //
        // Skip to 'ELSE' or 'END IF' for false condition.
        //
        while (pDesc && State_Else != pDesc->type) {
            pDesc = pDesc->pNext;
        }
        if (pDesc) pDesc = pDesc->pNext; // skip 'ELSE'

        while (pDesc) {
            fn = GetStatementFunc(pDesc->uStateIndex);
            if ((*fn)(pCtx, pDesc) < 0) return LUBE_FAIL;
            pDesc = pDesc->pNext;
        }
    }
    return LUBE_OK;
}
