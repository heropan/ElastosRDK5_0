//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "assert.h"
//#include <wtypes.h>

typedef unsigned char  byte;
#include <elatypes.h>
#include <clsutil.h>

static char s_szStringBuf[128];

const char *ParamAttrib2String(DWORD dwAttribs)
{
    static char s_szAttrib[16];

    if (dwAttribs & ParamAttrib_in) {
        strcpy(s_szAttrib, "in");
    }
    else {
        strcpy(s_szAttrib, "out");
        if (dwAttribs & ParamAttrib_callee) {
            strcat(s_szAttrib, ", callee");
        }
    }

    return s_szAttrib;
}

void GenerateTypeString(
    const CLSModule *pModule,
    const TypeDescriptor *pType,
    char *pszBuf)
{
    char buf[32];

    switch (pType->type) {
        case Type_Char8:
            strcat(pszBuf, "Char8");
            break;
        case Type_Char16:
            strcat(pszBuf, "Char16");
            break;
        case Type_Char32:
            strcat(pszBuf, "Char32");
            break;
        case Type_Byte:
            strcat(pszBuf, "Byte");
            break;
        case Type_Boolean:
            strcat(pszBuf, "Boolean");
            break;
        case Type_Int8:
            strcat(pszBuf, "Int8");
            break;
        case Type_Int16:
            strcat(pszBuf, "Int16");
            break;
        case Type_Int32:
            strcat(pszBuf, "Int32");
            break;
        case Type_Int64:
            strcat(pszBuf, "Int64");
            break;
        case Type_UInt16:
            strcat(pszBuf, "UInt16");
            break;
        case Type_UInt32:
            strcat(pszBuf, "UInt32");
            break;
        case Type_UInt64:
            strcat(pszBuf, "UInt64");
            break;
        case Type_CString:
            strcat(pszBuf, "CString");
            break;
        case Type_String:
            if ((!pType->bNested) && (0 == pType->nPointer)) {
                strcat(pszBuf, "const String&");
            }
            else {
                strcat(pszBuf, "String");
            }
            break;
        case Type_Float:
            strcat(pszBuf, "Float");
            break;
        case Type_Double:
            strcat(pszBuf, "Double");
            break;
        case Type_PVoid:
            strcat(pszBuf, "PVoid");
            break;
        case Type_ECode:
            strcat(pszBuf, "ECode");
            break;
        case Type_EMuid:
            strcat(pszBuf, "EMuid");
            break;
        case Type_EGuid:
            strcat(pszBuf, "EGuid");
            break;
        case Type_ArrayOf:
            if (0 > pType->nSize) {
                strcat(pszBuf, "ArrayOf<");
                GenerateTypeString(pModule, pType->pNestedType, pszBuf);
                strcat(pszBuf, ">");
            }
            else {
                strcat(pszBuf, "ArrayOf_<");
                GenerateTypeString(pModule, pType->pNestedType, pszBuf);
                strcat(pszBuf, ",");
                sprintf(buf, "%d", pType->nSize);
                strcat(pszBuf, buf);
                strcat(pszBuf, ">");
            }
            break;
        case Type_EventHandler:
            strcat(pszBuf, "EventHandler");
            break;
        case Type_interface:
        {
            if (pModule->ppInterfaceDir[pType->sIndex]->pszNameSpace != NULL &&
                pModule->ppInterfaceDir[pType->sIndex]->pszNameSpace[0] != '\0' &&
                strcmp(pModule->ppInterfaceDir[pType->sIndex]->pszNameSpace, "systypes")) {
                char *pszNamespace = (char*)malloc(strlen(pModule->ppInterfaceDir[pType->sIndex]->pszNameSpace) + 1);
                strcpy(pszNamespace, pModule->ppInterfaceDir[pType->sIndex]->pszNameSpace);
                char buffer[1024];
                buffer[0] = '\0';
                char *begin = pszNamespace;
                while (begin != NULL) {
                    char *dot = strchr(begin, '.');
                    if (dot != NULL) *dot = '\0';
                    strcat(buffer, begin);
                    strcat(buffer, "::");
                    if (dot != NULL) begin = dot + 1;
                    else begin = NULL;
                }
                free(pszNamespace);
                strcat(pszBuf, buffer);
            }
            strcat(pszBuf, pModule->ppInterfaceDir[pType->sIndex]->pszName);
            break;
        }
        case Type_struct:
            strcat(pszBuf, "struct ");
            strcat(pszBuf, pModule->ppStructDir[pType->sIndex]->pszName);
            break;
        case Type_enum:
        {
            if (pModule->ppEnumDir[pType->sIndex]->pszNameSpace != NULL &&
                pModule->ppEnumDir[pType->sIndex]->pszNameSpace[0] != '\0' &&
                strcmp(pModule->ppEnumDir[pType->sIndex]->pszNameSpace, "systypes")) {
                char *pszNamespace = (char*)malloc(strlen(pModule->ppEnumDir[pType->sIndex]->pszNameSpace) + 1);
                strcpy(pszNamespace, pModule->ppEnumDir[pType->sIndex]->pszNameSpace);
                char buffer[1024];
                buffer[0] = '\0';
                char *begin = pszNamespace;
                while (begin != NULL) {
                    char *dot = strchr(begin, '.');
                    if (dot != NULL) *dot = '\0';
                    strcat(buffer, begin);
                    strcat(buffer, "::");
                    if (dot != NULL) begin = dot + 1;
                    else begin = NULL;
                }
                free(pszNamespace);
                strcat(pszBuf, buffer);
            }
            strcat(pszBuf, pModule->ppEnumDir[pType->sIndex]->pszName);
            break;
        }
        case Type_alias:
            strcat(pszBuf, pModule->ppAliasDir[pType->sIndex]->pszName);
            break;
        case Type_Array:
            break;
        case Type_const:
            break;
    }

    if (1 == pType->nPointer) {
        strcat(pszBuf, " *");
    }
    else if (2 == pType->nPointer) {
        strcat(pszBuf, " **");
    }
}

void GenerateTypeStringForParam(
    const CLSModule *pModule,
    const TypeDescriptor *pType,
    char *pszBuf)
{
    if (1 == pType->nPointer) {
        strcat(pszBuf, "P");
    }
    else if (2 == pType->nPointer) {
        strcat(pszBuf, "PP");
    }

    switch (pType->type) {
        case Type_Char8:
            strcat(pszBuf, "Char8");
            break;
        case Type_Char16:
            strcat(pszBuf, "Char16");
            break;
        case Type_Char32:
            strcat(pszBuf, "Char32");
            break;
        case Type_Byte:
            strcat(pszBuf, "Byte");
            break;
        case Type_Boolean:
            strcat(pszBuf, "Boolean");
            break;
        case Type_Int8:
            strcat(pszBuf, "Int8");
            break;
        case Type_Int16:
            strcat(pszBuf, "Int16");
            break;
        case Type_Int32:
            strcat(pszBuf, "Int32");
            break;
        case Type_Int64:
            strcat(pszBuf, "Int64");
            break;
        case Type_UInt16:
            strcat(pszBuf, "UInt16");
            break;
        case Type_UInt32:
            strcat(pszBuf, "UInt32");
            break;
        case Type_UInt64:
            strcat(pszBuf, "UInt64");
            break;
        case Type_CString:
            strcat(pszBuf, "CString");
            break;
        case Type_String:
            strcat(pszBuf, "String");
            break;
        case Type_Float:
            strcat(pszBuf, "Float");
            break;
        case Type_Double:
            strcat(pszBuf, "Double");
            break;
        case Type_PVoid:
            strcat(pszBuf, "PVoid");
            break;
        case Type_ECode:
            strcat(pszBuf, "ECode");
            break;
        case Type_EMuid:
            strcat(pszBuf, "EMuid");
            break;
        case Type_EGuid:
            strcat(pszBuf, "EGuid");
            break;
        case Type_ArrayOf:
            strcat(pszBuf, "ArrayOf");
            GenerateTypeStringForParam(pModule, pType->pNestedType, pszBuf);
            break;
        case Type_EventHandler:
            strcat(pszBuf, "EventHandler");
            break;
        case Type_interface:
            strcat(pszBuf, pModule->ppInterfaceDir[pType->sIndex]->pszName);
            break;
        case Type_struct:
            strcat(pszBuf, pModule->ppStructDir[pType->sIndex]->pszName);
            break;
        case Type_enum:
            strcat(pszBuf, pModule->ppEnumDir[pType->sIndex]->pszName);
            break;
        case Type_alias:
            strcat(pszBuf, pModule->ppAliasDir[pType->sIndex]->pszName);
            break;
        case Type_Array:
            {
                TypeDescriptor type;
                GetArrayBaseType(pModule, pType, &type);
                GenerateTypeStringForParam(pModule, &type, pszBuf);
            }
            break;
        case Type_const:
            break;
    }

}

// Generate [n][m]...
inline void GenerateArrayDims(
    const CLSModule *pModule,
    const TypeDescriptor *pArrayType,
    char *pBuf)
{
    const TypeDescriptor *pType;
    char szNum[128];

    assert(Type_Array == pArrayType->type);

    pType = pArrayType;

    do {
        strcat(pBuf, "[");
        sprintf(szNum, "%d", pModule->ppArrayDir[pType->sIndex]->nElements);
        strcat(pBuf, szNum);
        strcat(pBuf, "]");
        pType = &(pModule->ppArrayDir[pType->sIndex]->type);
    } while (Type_Array == pType->type);
}

//Generate "TYPE [*[*]] NAME[m][n].."
const char *Array2CString(
    const CLSModule *pModule,
    const TypeDescriptor *pType,
    const char *pszElemName)
{
    s_szStringBuf[0] = 0;
    TypeDescriptor type;

    assert(Type_Array == pType->type);

    GetArrayBaseType(pModule, pType, &type);

    GenerateTypeString(pModule, &type, s_szStringBuf);

    strcat(s_szStringBuf, " ");
    strcat(s_szStringBuf, pszElemName);

    GenerateArrayDims(pModule, pType, s_szStringBuf);

    return s_szStringBuf;
}

const char *Dims2CString(
    const CLSModule *pModule,
    const TypeDescriptor *pType)
{
    s_szStringBuf[0] = 0;

    assert(Type_Array == pType->type);

    GenerateArrayDims(pModule, pType, s_szStringBuf);

    return s_szStringBuf;
}

const char *CStyleStructParamType2CString(
    const CLSModule *pModule,
    const TypeDescriptor *pType)
{
    assert(Type_struct == pType->type || Type_EMuid == pType->type
           || Type_EGuid == pType->type || Type_alias == pType->type);

    s_szStringBuf[0] = 0;
    // If struct parameter type is't pointer, we have to change its type
    // to a pointer of a const struct object.
    if (0 == pType->nPointer) {
        strcat(s_szStringBuf, "const ");
        GenerateTypeString(pModule, pType, s_szStringBuf);
        strcat(s_szStringBuf, " *");
    }
    else {
        GenerateTypeString(pModule, pType, s_szStringBuf);
    }

    return s_szStringBuf;
}

const char *StructType2CString(
    const CLSModule *pModule,
    const TypeDescriptor *pType)
{
    assert(Type_struct == pType->type
            || Type_EMuid == pType->type
            || Type_EGuid == pType->type
            || Type_alias == pType->type);

    TypeDescriptor type;

    if (Type_alias == pType->type) {
        GetOriginalType(pModule, pType, &type);
    }

    s_szStringBuf[0] = 0;
    // If struct parameter type is't pointer, we have to change its type
    // to a reference of a const struct object.
    if (((Type_alias == pType->type) && (0 == type.nPointer))
        || (!(Type_alias == pType->type) && (0 == pType->nPointer))) {
        strcat(s_szStringBuf, "const ");
        GenerateTypeString(pModule, pType, s_szStringBuf);
        strcat(s_szStringBuf, " &");
    }
    else {
        GenerateTypeString(pModule, pType, s_szStringBuf);
    }

    return s_szStringBuf;
}

const char *Type2CString(
    const CLSModule *pModule,
    const TypeDescriptor *pType)
{
    s_szStringBuf[0] = 0;
    GenerateTypeString(pModule, pType, s_szStringBuf);

    return s_szStringBuf;
}

void GenerateITypeString(
    const CLSModule *pModule, const TypeDescriptor *pType, char *pszBuf)
{
    switch (pType->type) {
        default:
            GenerateTypeString(pModule, pType, pszBuf);
            return;
    }

    if (1 == pType->nPointer) {
        strcat(pszBuf, " *");
    }
    else if (2 == pType->nPointer) {
        strcat(pszBuf, " **");
    }
}

const char *Type2IString(
    const CLSModule *pModule, const TypeDescriptor *pType)
{
    s_szStringBuf[0] = 0;
    GenerateITypeString(pModule, pType, s_szStringBuf);

    return s_szStringBuf;
}

const char *c_szIUpperFmt = "%08X-%04X-%04X-%02X%02X-%02X%02X%02X%02X%02X%02X";
const char *c_szILowerFmt = "%08x-%04x-%04x-%02x%02x-%02x%02x%02x%02x%02x%02x";

const char *Uuid2IString(const GUID *pUuid, BOOL bUpper)
{
    const char *pszFormat = bUpper ? c_szIUpperFmt:c_szILowerFmt;

    sprintf(s_szStringBuf, pszFormat,
            pUuid->Data1, pUuid->Data2, pUuid->Data3,
            pUuid->Data4[0], pUuid->Data4[1], pUuid->Data4[2],
            pUuid->Data4[3], pUuid->Data4[4], pUuid->Data4[5],
            pUuid->Data4[6], pUuid->Data4[7]);

    return s_szStringBuf;
}

const char *c_szCUpperFmt = "{0x%08X,0x%04X,0x%04X,"
    "{0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X,0x%02X}}";
const char *c_szCLowerFmt = "{0x%08x,0x%04x,0x%04x,"
    "{0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x,0x%02x}}";

const char *Uuid2CString(const GUID *pUuid, BOOL bUpper)
{
    const char *pszFormat = bUpper ? c_szCUpperFmt:c_szCLowerFmt;

    sprintf(s_szStringBuf, pszFormat,
            pUuid->Data1, pUuid->Data2, pUuid->Data3,
            pUuid->Data4[0], pUuid->Data4[1], pUuid->Data4[2],
            pUuid->Data4[3], pUuid->Data4[4], pUuid->Data4[5],
            pUuid->Data4[6], pUuid->Data4[7]);

    return s_szStringBuf;
}
