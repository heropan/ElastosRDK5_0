//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <assert.h>
//#include <wtypes.h>

typedef unsigned char  byte;
#include <elatypes.h>
#include <clsutil.h>

void CGenerateCARAttribs(FILE *pFile, const CLSModule *pModule)
{
    BOOL bBracket;

    if (0 == CAR_ATTR(pModule->mAttribs)
        && !pModule->mUunm
        && 0 == pModule->mMajorVersion
        && 0 == pModule->mMinorVersion)
        return;

    bBracket = FALSE;

    if (0 != pModule->mMajorVersion || 0 != pModule->mMinorVersion) {
        if (!bBracket) {
            fputs("[\n", pFile);
            bBracket = TRUE;
        }
        fprintf(pFile, "    version(%d.%d)", pModule->mMajorVersion, pModule->mMinorVersion);
    }
    if (bBracket) {
        fputs("\n]\n", pFile);
    }
}

void CGenerateLibraries(FILE *pFile, const CLSModule *pModule)
{
    int n;

    for (n = 0; n < pModule->mLibraryCount; n++) {
        fprintf(pFile,
                "    importlib(\"%s\");\n",
                pModule->ppLibNames[n]);
    }
}

void CGenerateInterfaceDecls(FILE *pFile, const CLSModule *pModule)
{
    int n;
    InterfaceDirEntry *pEntry;
    BOOL bFirst = TRUE;

    for (n = 0; n < pModule->mInterfaceCount; n++) {
        pEntry = pModule->ppInterfaceDir[n];

        if (!pEntry->pszNameSpace \
            && !(pEntry->pDesc->dwAttribs & InterfaceAttrib_dual)
            && !(pEntry->pDesc->dwAttribs & InterfaceAttrib_clsobj)) {
            if (bFirst) {
                bFirst = FALSE;
                fputs("\n", pFile);
            }
            fprintf(pFile, "    interface %s;\n", pEntry->pszName);
        }
    }
}

void CGenerateAliases(FILE *pFile, const CLSModule *pModule)
{
    int n;
    AliasDirEntry *pEntry;
    BOOL bFirst = TRUE;

    for (n = 0; n < pModule->mAliasCount; n++) {
        pEntry = pModule->ppAliasDir[n];
        if (!pEntry->pszNameSpace) {
            if (bFirst) {
                bFirst = FALSE;
                fputs("\n", pFile);
            }
            fprintf(pFile,
                "    typedef %s%s %s;\n",
                (pEntry->bDummyType ? "[dummytype] ":""),
                Type2CString(pModule, &pEntry->type),
                pEntry->pszName);
        }
    }
}

void CGenerateEnums(FILE *pFile, const CLSModule *pModule)
{
    int n, m;
    EnumDescriptor *pDesc;

    for (n = 0; n < pModule->mEnumCount; n++) {
        if (!pModule->ppEnumDir[n]->pszNameSpace) {
            fprintf(pFile,
                        "\n"
                        "    enum %s {\n",
                        pModule->ppEnumDir[n]->pszName);

            pDesc = pModule->ppEnumDir[n]->pDesc;
            for (m = 0; m < pDesc->cElems; m++) {
                if (pDesc->ppElems[m]->bHexFormat) {
                    fprintf(pFile,
                            "        %s = 0x%08x,\n",
                            pDesc->ppElems[m]->pszName,
                            pDesc->ppElems[m]->nValue);
                }
                else {
                    fprintf(pFile,
                            "        %s = %d,\n",
                            pDesc->ppElems[m]->pszName,
                            pDesc->ppElems[m]->nValue);
                }
            }
            fputs("    }\n", pFile);
        }
    }
}

void CGenerateConsts(FILE *pFile, const CLSModule *pModule)
{
    int n;

    for (n = 0; n < pModule->mConstCount; n++) {
        if (!pModule->ppConstDir[n]->pszNameSpace) {
            fprintf(pFile,
                        "\n"
                        "    const %s = ",
                        pModule->ppConstDir[n]->pszName);
            if (pModule->ppConstDir[n]->type == TYPE_INTEGER32) {
                if (pModule->ppConstDir[n]->v.intValue.bHexFormat) {
                    fprintf(pFile,
                            "0x%08x;\n",
                            pModule->ppConstDir[n]->v.intValue.nValue);
                }
                else {
                    fprintf(pFile,
                            "%d;\n",
                            pModule->ppConstDir[n]->v.intValue.nValue);
                }
            }
            else {
                assert(pModule->ppConstDir[n]->type == TYPE_STRING);
                if (pModule->ppConstDir[n]->v.strValue.pszValue != NULL) {
                     fprintf(pFile,
                            "\"%s\";\n",
                            pModule->ppConstDir[n]->v.strValue.pszValue);
                }
            }
        }
    }
}

void CGenerateStructs(FILE *pFile, const CLSModule *pModule)
{
    int n, m;
    StructDescriptor *pDesc;
    TypeDescriptor elemType;

    for (n = 0; n < pModule->mStructCount; n++) {
        if (!pModule->ppStructDir[n]->pszNameSpace) {
            fprintf(pFile,
                    "\n"
                    "    struct %s {\n",
                    pModule->ppStructDir[n]->pszName);

            pDesc = pModule->ppStructDir[n]->pDesc;
            for (m = 0; m < pDesc->cElems; m++) {
                elemType = pDesc->ppElems[m]->type;
                if (Type_Array == elemType.type) { //Handle Array
                    fprintf(pFile,
                            "        %s;\n",
                            Array2CString(pModule, &elemType, pDesc->ppElems[m]->pszName));
                }
                else {
                    fprintf(pFile,
                            "        %s %s;\n",
                            Type2CString(pModule, &pDesc->ppElems[m]->type),
                            pDesc->ppElems[m]->pszName);
                }
            }
            fputs("    }\n", pFile);
        }
    }
}

void CGenerateInterfaceAttribs(
    FILE *pFile, const InterfaceDescriptor *pDesc)
{
    if (0 == INTERFACE_ATTR(pDesc->dwAttribs))
        return;

    fputs("    [\n", pFile);
    if (pDesc->dwAttribs & InterfaceAttrib_local) {
        fputs("        local", pFile);
    }
    if (pDesc->dwAttribs & InterfaceAttrib_oneway) {
        fputs("        oneway", pFile);
    }
    if (pDesc->dwAttribs & InterfaceAttrib_parcelable) {
        fputs("        parcelable", pFile);
    }

    fputs("\n    ]\n", pFile);
}

void CGenerateInterfaceConst(FILE *pFile,
    const CLSModule *pModule, InterfaceConstDescriptor *pConst)
{
    fprintf(pFile,
            "        const ");
    if (pConst->type == TYPE_BOOLEAN) {
        fprintf(pFile, "Boolean %s = %s;\n", pConst->pszName, pConst->v.bValue ? "TRUE" : "FALSE");
    }
    else if (pConst->type == TYPE_CHAR32) {
        fprintf(pFile, "Char32 %s = \'%c\'\n", pConst->pszName, pConst->v.int32Value.nValue);
    }
    else if (pConst->type == TYPE_BYTE) {
        fprintf(pFile, "Byte %s = ", pConst->pszName);
        if (pConst->v.int32Value.format == FORMAT_HEX) {
            fprintf(pFile, "0x%08x;\n", (unsigned char)pConst->v.int32Value.nValue);
        }
        else {
            fprintf(pFile, "%d;\n", (unsigned char)pConst->v.int32Value.nValue);
        }
    }
    else if (pConst->type == TYPE_INTEGER16) {
        fprintf(pFile, "Int16 %s = ", pConst->pszName);
        if (pConst->v.int32Value.format == FORMAT_HEX) {
            fprintf(pFile, "0x%08x;\n", (short)pConst->v.int32Value.nValue);
        }
        else {
            fprintf(pFile, "%d;\n", (short)pConst->v.int32Value.nValue);
        }
    }
    else if (pConst->type == TYPE_INTEGER32) {
        fprintf(pFile, "Int32 %s = ", pConst->pszName);
        if (pConst->v.int32Value.format == FORMAT_HEX) {
            fprintf(pFile, "0x%08x;\n", pConst->v.int32Value.nValue);
        }
        else {
            fprintf(pFile, "%d;\n", pConst->v.int32Value.nValue);
        }
    }
    else if (pConst->type == TYPE_INTEGER64) {
        fprintf(pFile, "Int64 ");
        fprintf(pFile, "%s = ", pConst->pszName);
        if (pConst->v.int64Value.format == FORMAT_HEX) {
            fprintf(pFile, "0x%llx;\n", pConst->v.int64Value.nValue);
        }
        else {
            fprintf(pFile, "%lld;\n", pConst->v.int64Value.nValue);
        }
    }
    else if (pConst->type == TYPE_FLOAT) {
        fprintf(pFile, "Float %s = %f;\n", pConst->pszName, pConst->v.dValue);
    }
    else if (pConst->type == TYPE_DOUBLE) {
        fprintf(pFile, "Double %s = %f;\n", pConst->pszName, pConst->v.dValue);
    }
    else {
        assert(pConst->type == TYPE_STRING);

        if (pConst->v.pStrValue != NULL) {
             fprintf(pFile, "String %s = \"%s\";\n", pConst->pszName, pConst->v.pStrValue);
        }
    }
}

void CGenerateMethodAttribs(
    FILE *pFile, const MethodDescriptor *pMethod)
{
    if (pMethod->dwAttribs & MethodAttrib_Oneway) {
        fprintf(pFile, "        [oneway] ");
    }
    else {
        fprintf(pFile, "        ");
    }
}

void CGenerateMethod(FILE *pFile,
    const CLSModule *pModule, MethodDescriptor *pMethod)
{
    int n;
    ParamDescriptor *pParam;

    CGenerateMethodAttribs(pFile, pMethod);
    fprintf(pFile,
            "%s(",
            pMethod->pszName);
    for (n = 0; n < pMethod->cParams; n++) {
        if (0 != n) fputs(", ", pFile);
        pParam = pMethod->ppParams[n];

        fprintf(pFile,
                "\n            [%s] %s %s",
                ParamAttrib2String(pParam->dwAttribs),
                Type2CString(pModule, &pParam->type),
                pParam->pszName);
    }
    fputs(");\n", pFile);
}

void CGenerateInterfaces(FILE *pFile, const CLSModule *pModule)
{
    int n, m;
    InterfaceDescriptor *pDesc;

    for (n = 0; n < pModule->mInterfaceCount; n++) {
        pDesc = pModule->ppInterfaceDir[n]->pDesc;

        if (!pModule->ppInterfaceDir[n]->pszNameSpace
            && !(pDesc->dwAttribs & InterfaceAttrib_dual)
            && !(pDesc->dwAttribs & InterfaceAttrib_clsobj)) {
            fputs("\n", pFile);
            CGenerateInterfaceAttribs(pFile, pDesc);

            fprintf(pFile, "    interface %s",
                    pModule->ppInterfaceDir[n]->pszName);

            if (0 != pDesc->sParentIndex) {
                fprintf(pFile, " : %s",
                        pModule->ppInterfaceDir[pDesc->sParentIndex]->pszName);
            }
            fputs(" {\n", pFile);

            for (m = 0; m < pDesc->mConstCount; m++) {
                if (0 != m) fputs("\n", pFile);
                CGenerateInterfaceConst(pFile, pModule, pDesc->ppConsts[m]);
            }

            for (m = 0; m < pDesc->cMethods; m++) {
                if (0 != m) fputs("\n", pFile);
                CGenerateMethod(pFile, pModule, pDesc->ppMethods[m]);
            }
            fputs("    }\n", pFile);
        }
    }
}

void CGenerateClassAttribs(
    FILE *pFile, const CLSModule *pModule, const ClassDescriptor *pDesc)
{
    int n;
    BOOL bComma;

    if (0 == CLASS_ATTR(pDesc->dwAttribs))
        return;

    bComma = FALSE;
    fputs("    [\n", pFile);

    if (pDesc->dwAttribs & ClassAttrib_singleton) {
        if (bComma) fputs(",\n", pFile);
        fputs("        singleton", pFile);
        bComma = TRUE;
    }
    if (pDesc->dwAttribs & ClassAttrib_private) {
        if (bComma) fputs(",\n", pFile);
        fputs("        private", pFile);
        bComma = TRUE;
    }
    if (pDesc->dwAttribs & ClassAttrib_aggregate) {
        if (bComma) fputs(",\n", pFile);
        fprintf(pFile,
                "        aggregate(%s",
                pModule->ppClassDir[pDesc->pAggrIndexs[0]]->pszName);
        for (n = 1; n < pDesc->cAggregates; n++) {
            fprintf(pFile, ", %s",
                    pModule->ppClassDir[pDesc->pAggrIndexs[n]]->pszName);
        }
        fputs(")", pFile);
    }

    if (pDesc->dwAttribs & ClassAttrib_aspect) {
        if (bComma) fputs(",\n", pFile);
        fprintf(pFile,
                "        aspect(%s",
                pModule->ppClassDir[pDesc->pAspectIndexs[0]]->pszName);
        for (n = 1; n < pDesc->cAspects; n++) {
            fprintf(pFile, ", %s",
                    pModule->ppClassDir[pDesc->pAspectIndexs[n]]->pszName);
        }
        fputs(")", pFile);
    }
    fputs("\n    ]\n", pFile);
}

void CGenerateClasses(FILE *pFile, const CLSModule *pModule)
{
    int n, m;
    ClassDescriptor *pClass;
    InterfaceDirEntry *pIEntry;
    ClassInterface *pClsIntf;

    for (n = 0; n < pModule->mClassCount; n++) {
        pClass = pModule->ppClassDir[n]->pDesc;
        if (!pModule->ppClassDir[n]->pszNameSpace
            && !(pClass->dwAttribs & ClassAttrib_t_sink)
            && !(pClass->dwAttribs & ClassAttrib_t_clsobj)
            && !(pClass->dwAttribs & ClassAttrib_t_external)) {
            fputs("\n", pFile);
            CGenerateClassAttribs(pFile, pModule, pClass);

            switch (CLASS_TYPE(pClass->dwAttribs)) {
                case ClassAttrib_t_aspect:
                    fputs("    aspect ", pFile);
                    break;
                case ClassAttrib_t_generic:
                    fputs("    generic ", pFile);
                    break;
                case ClassAttrib_t_regime:
                    fputs("    regime ", pFile);
                    break;
                case ClassAttrib_t_normalClass:
                    fputs("    class ", pFile);
                    break;
                default:
                    assert(TRUE == FALSE);
                    break;
            }
            fputs(pModule->ppClassDir[n]->pszName, pFile);

            if (pClass->dwAttribs & ClassAttrib_hasparent) {
                if (pModule->ppClassDir[pClass->sParentIndex]->pDesc->dwAttribs
                    & ClassAttrib_t_generic) {
                    fprintf(pFile, " :: %s",
                        pModule->ppClassDir[pClass->sParentIndex]->pszName);
                }
                else {
                    fprintf(pFile, " : %s",
                        pModule->ppClassDir[pClass->sParentIndex]->pszName);
                }
            }

            fputs(" {\n", pFile);

// TODO: ctors

            for (m = 0; m < pClass->mInterfaceCount; m++) {
                pClsIntf = pClass->ppInterfaces[m];
                pIEntry = pModule->ppInterfaceDir[pClsIntf->sIndex];

                if (pClsIntf->wAttribs & ClassInterfaceAttrib_outer)
                    continue;

                fputs("        ", pFile);
                if (pClsIntf->wAttribs & ClassInterfaceAttrib_virtual) {
                    fputs("virtual ", pFile);
                }
                else if (pClsIntf->wAttribs & ClassInterfaceAttrib_hidden) {
                    fputs("hidden ", pFile);
                }
                else if (pClsIntf->wAttribs & ClassInterfaceAttrib_callback) {
                    fputs("callback ", pFile);
                }

                fprintf(pFile, "interface %s;\n", pIEntry->pszName);
            }
            fputs("    }\n", pFile);
        }
    }
}

int CLS2CAR_(FILE *pFile, const CLSModule *pModule)
{
    CGenerateCARAttribs(pFile, pModule);

    if (pModule->mAttribs & CARAttrib_library) {
        fprintf(pFile, "library");
    }
    else {
        fprintf(pFile, "module");
    }

    if (pModule->mUunm) {
        fprintf(pFile, " %s\n{\n", pModule->mUunm);
    }
    else {
        fprintf(pFile, "\n{\n");
    }

    CGenerateLibraries(pFile, pModule);
    CGenerateInterfaceDecls(pFile, pModule);
    CGenerateEnums(pFile, pModule);
    CGenerateConsts(pFile, pModule);
    CGenerateAliases(pFile, pModule);
    CGenerateStructs(pFile, pModule);
    CGenerateInterfaces(pFile, pModule);
    CGenerateClasses(pFile, pModule);

    fputs("}\n", pFile);

    return CLS_NoError;
}

int CLS2CAR(const char *pszName, const CLSModule *pModule)
{
    int nRet;
    FILE *pFile;

    pFile = fopen(pszName, "w+t");
    if (!pFile) return CLSError_OpenFile;

    nRet = CLS2CAR_(pFile, pModule);

    fclose(pFile);
    return nRet;
}
