//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <assert.h>
#include "clsbase.h"
#include <stdio.h>
#include <clsdef.h>

static int s_cModules = 0;
static CLSModule *s_modules[c_nMaxLibNumber + 2];

int AddCLSLibrary(CLSModule *pModule)
{
    s_modules[s_cModules++] = pModule;
    return 0;
}

void DestroyAllLibraries()
{
    int n;

    for (n = 0; n < s_cModules; n++) {
        if (s_modules[n]->dwAttribs & CARAttrib_inheap)
            DisposeFlattedCLS(s_modules[n]);
        else
            DestroyCLS(s_modules[n]);
    }
    s_cModules = 0;
}

int RetrieveAlias(const char *pszName,
    CLSModule *pModule, BOOL bLibOnly)
{
    int n, m;

    assert(pszName && pModule);

    if (!bLibOnly) {
        n = SelectAliasDirEntry(pszName, pModule);
        if (n >= 0) return n;
    }
    for (n = s_cModules - 1; n >= 0; n--) {
        m = SelectAliasDirEntry(pszName, s_modules[n]);
        if (m >= 0)
            return AliasCopy(s_modules[n], m, pModule, TRUE);
    }

    return -1;
}

int RetrieveConst(const char *pszName,
    CLSModule *pModule, BOOL bLibOnly)
{
    int n, m;

    assert(pszName && pModule);

    if (!bLibOnly) {
        n = SelectConstDirEntry(pszName, pModule);
        if (n >= 0) return n;
    }
    for (n = s_cModules - 1; n >= 0; n--) {
        m = SelectConstDirEntry(pszName, s_modules[n]);
        if (m >= 0)
            return ConstCopy(s_modules[n], m, pModule, TRUE);
    }

    return -1;
}

int RetrieveEnum(const char *pszName, const char *pszNamespaces,
    CLSModule *pModule, BOOL bLibOnly)
{
    int n, m;

    assert(pszName && pModule);

    if (!bLibOnly) {
        n = SelectEnumDirEntry(pszName, pszNamespaces, pModule);
        if (n >= 0) return n;
    }
    for (n = s_cModules - 1; n >= 0; n--) {
        m = SelectEnumDirEntry(pszName, pszNamespaces, s_modules[n]);
        if (m >= 0)
            return EnumCopy(s_modules[n], m, pModule, TRUE);
    }

    return -1;
}

int RetrieveStruct(const char *pszName,
    CLSModule *pModule, BOOL bLibOnly)
{
    int n, m;

    assert(pszName && pModule);

    if (!bLibOnly) {
        n = SelectStructDirEntry(pszName, pModule);
        if (n >= 0) return n;
    }
    for (n = s_cModules - 1; n >= 0; n--) {
        m = SelectStructDirEntry(pszName, s_modules[n]);
        if (m >= 0)
            return StructCopy(s_modules[n], m, pModule, TRUE);
    }

    return -1;
}

int RetrieveInterface(const char *pszName, const char *pszNamespaces,
    CLSModule *pModule, BOOL bLibOnly)
{
    int n, m;

    assert(pszName && pModule);

    if (!bLibOnly) {
        n = SelectInterfaceDirEntry(pszName, pszNamespaces, pModule);
        if (n >= 0) return n;
    }
    for (n = s_cModules - 1; n >= 0; n--) {
        m = SelectInterfaceDirEntry(pszName, pszNamespaces, s_modules[n]);
        if (m >= 0)
            return InterfaceCopy(s_modules[n], m, pModule, TRUE);
    }

    return -1;
}

int RetrieveMethod(const char *pszName,
    CLSModule *pModule,
    const ClassInterface *pDesc)
{
    assert(pszName && pDesc);

    return SelectInterfaceMethod(pszName,
                                pModule->ppInterfaceDir[pDesc->sIndex]->pDesc);
}

int RetrieveIdentifyType(const char *pszName, const char *pszNamespaces,
    CLSModule *pModule, TypeDescriptor *pType, BOOL bLibOnly)
{
    int n;
    TypeDescriptor type;

    assert(pszName && pModule && pType);

    if (!bLibOnly) {
        if (GetIdentifyType(pszName, pszNamespaces, pModule, pType) == CLS_NoError)
            return 0;
    }
    memset(&type, 0, sizeof(TypeDescriptor));

    for (n = s_cModules - 1; n >= 0; n--) {
        if (GetIdentifyType(pszName, pszNamespaces, s_modules[n], &type) == CLS_NoError) {
            return TypeCopy(s_modules[n], &type, pModule, pType, TRUE);
        }
    }

    return -1;
}

int RetrieveClass(const char *pszName, const char *pszNamespace,
    CLSModule *pModule, BOOL bLibOnly)
{
    int n, m;

    assert(pszName && pModule);

    if (!bLibOnly) {
        n = SelectClassDirEntry(pszName, pszNamespace, pModule);
        if (n >= 0) return n;
    }
    for (n = s_cModules - 1; n >= 0; n--) {
        m = SelectClassDirEntry(pszName, pszNamespace, s_modules[n]);
        if (m >= 0)
            return ClassCopy(s_modules[n], m, pModule, TRUE);
    }

    return -1;
}

int MergeCLS(const CLSModule *pSrc, CLSModule *pDest)
{
    int n;

    for (n = 0; n < pSrc->cEnums; n++) {
        if (EnumCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    for (n = 0; n < pSrc->cStructs; n++) {
        if (StructCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    for (n = 0; n < pSrc->cAliases; n++) {
        if (AliasCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    for (n = 0; n < pSrc->cInterfaces; n++) {
        if (InterfaceCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    for (n = 0; n < pSrc->cClasses; n++) {
        if (ClassCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    return 0;
}

int CopyCLS(const CLSModule *pSrc, CLSModule *pDest)
{
    int n;

    memcpy(pDest->szMagic, pSrc->szMagic, c_nMagicSize);
    pDest->cMajorVersion = pSrc->cMajorVersion;
    pDest->cMinorVersion = pSrc->cMinorVersion;
    pDest->nCLSModuleVersion = pSrc->nCLSModuleVersion;
    pDest->nSize = pSrc->nSize;

    if (pSrc->pszUunm) {
        pDest->pszUunm = new char[strlen(pSrc->pszUunm)+1];
        strcpy(pDest->pszUunm, pSrc->pszUunm);
    }
    else {
        pDest->pszUunm = NULL;
    }

    memcpy(&pDest->uuid, &pSrc->uuid, sizeof(pSrc->uuid));
    pDest->dwAttribs = pSrc->dwAttribs;
    if (pSrc->pszName) {
        pDest->pszName = new char[strlen(pSrc->pszName)+1];
        strcpy(pDest->pszName, pSrc->pszName);
    }
    else {
        pDest->pszName = NULL;
    }

    if (pSrc->pszServiceName) {
        pDest->pszServiceName = new char[strlen(pSrc->pszServiceName)+1];
        strcpy(pDest->pszServiceName, pSrc->pszServiceName);
    }
    else {
        pDest->pszServiceName = NULL;
    }

    pDest->cDefinedInterfaces = 0;
    for (n = 0; n < pSrc->cDefinedInterfaces; n++) {
        pDest->pDefinedInterfaceIndex[pDest->cDefinedInterfaces++] = pSrc->pDefinedInterfaceIndex[n];
    }

    pDest->cLibraries = 0;
    for (n = 0; n < pSrc->cLibraries; n++) {
        pDest->ppLibNames[n] = new char[strlen(pSrc->ppLibNames[n])+1];
        strcpy(pDest->ppLibNames[n], pSrc->ppLibNames[n]);
        pDest->cLibraries++;
    }

    pDest->cArrays = 0;
    for (n = 0; n < pSrc->cArrays; n++) {
        if (ArrayXCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    pDest->cConsts = 0;
    for (n = 0; n < pSrc->cConsts; n++) {
        if (ConstCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    pDest->cEnums = 0;
    for (n = 0; n < pSrc->cEnums; n++) {
        if (EnumCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    pDest->cStructs = 0;
    for (n = 0; n < pSrc->cStructs; n++) {
        if (StructXCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    pDest->cAliases = 0;
    for (n = 0; n < pSrc->cAliases; n++) {
        if (AliasXCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    pDest->cInterfaces = 0;
    for (n = 0; n < pSrc->cInterfaces; n++) {
        if (InterfaceXCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    pDest->cClasses = 0;
    for (n = 0; n < pSrc->cClasses; n++) {
        if (ClassXCopy(pSrc, n, pDest, FALSE) < 0) return -1;
    }

    return 0;
}
