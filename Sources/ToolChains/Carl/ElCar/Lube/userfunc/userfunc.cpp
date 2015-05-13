//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <lube.h>
#include <clsutil.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#ifdef _linux
#include <sys/io.h>
#else
#include <io.h>
#endif
#include <ctype.h>


#define HTIME_GT_DLLTIME -2
#define HTIME_LT_DLLTIME 2

#ifdef _linux
#define _MAX_PATH 256

#define _strdup strdup
#endif

extern const char *g_pszOutputPath;
extern char * ImplNamespaceType(const char * str);

DECL_USERFUNC(PrintMessage);
DECL_USERFUNC(ErrorMessage);
DECL_USERFUNC(Upper);
DECL_USERFUNC(Lower);
DECL_USERFUNC(Embed);
DECL_USERFUNC(Rewrite);
DECL_USERFUNC(MacroRewrite);
DECL_USERFUNC(FormatUuid);
DECL_USERFUNC(CLS2CAR);
DECL_USERFUNC(CLS2Abrg);
DECL_USERFUNC(CStyleParamType);
DECL_USERFUNC(TypeClass);
DECL_USERFUNC(ClassNameOfSink);
DECL_USERFUNC(ClassNameOfClassObj);
DECL_USERFUNC(NewOfGeneric);
DECL_USERFUNC(UsageNewOfCtor);
DECL_USERFUNC(UsageNewOfGeneric);
DECL_USERFUNC(NewHeaderOfGeneric);
DECL_USERFUNC(EmptyImplOfGeneric);
DECL_USERFUNC(NewHeaderOfGenericClassObjects);
DECL_USERFUNC(NewHeaderOfGenericInterfaces);
DECL_USERFUNC(ImplOfGenericClassObjects);
DECL_USERFUNC(GenericInfoQI);
DECL_USERFUNC(CreateHFiles);
DECL_USERFUNC(CreateCPPFiles);
DECL_USERFUNC(CStyleName);
DECL_USERFUNC(DefaultInterface);
DECL_USERFUNC(PrefixingName);
DECL_USERFUNC(PrefixingNameByName);
DECL_USERFUNC(InterfaceNameOfSink);
DECL_USERFUNC(ParamOrigType);
DECL_USERFUNC(ParamAddRef);
DECL_USERFUNC(ParamRelease);
DECL_USERFUNC(NamespaceType);
DECL_USERFUNC(ParamNamespaceType);
DECL_USERFUNC(MemberNamespaceType);
DECL_USERFUNC(ParamType2String);
DECL_USERFUNC(CopyOutParam);
DECL_USERFUNC(AndriodType);
DECL_USERFUNC(WriteToAndriodParcel);
DECL_USERFUNC(DelcareVariablesForSoMethod);
DECL_USERFUNC(MethodIndexInInterface);
DECL_USERFUNC(CallSoMethod);
DECL_USERFUNC(GenerateAndriodParameter);
DECL_USERFUNC(GenerateCBDataParameter);
DECL_USERFUNC(GenerateAndriodArgument);
DECL_USERFUNC(ReadOutParamFromParcel);
DECL_USERFUNC(ReadFromAndriodParcelOnTransact);
DECL_USERFUNC(PrepareParamForResponseCallback);
DECL_USERFUNC(IsFiltered);
DECL_USERFUNC(HasTrivialConstructor);
DECL_USERFUNC(HasDefaultConstructor);
DECL_USERFUNC(ParcelParameter);
DECL_USERFUNC(HasParameters);
DECL_USERFUNC(OrgClassIsAspect);
DECL_USERFUNC(ClassNamespaceBegin);
DECL_USERFUNC(ClassNamespaceEnd);
DECL_USERFUNC(InterfaceNamespaceBegin);
DECL_USERFUNC(InterfaceNamespaceEnd);
DECL_USERFUNC(EnumNamespaceBegin);
DECL_USERFUNC(EnumNamespaceEnd);

const UserFuncEntry g_userFuncs[] = {
    USERFUNC_(Embed, ARGTYPE_STRING, \
            "Embed string to target file"),
    USERFUNC_(PrintMessage, ARGTYPE_STRING, \
            "Put string to stdout"),
    USERFUNC_(ErrorMessage, ARGTYPE_STRING, \
            "Put string to stderr and stop lube"),
    USERFUNC_(Upper, ARGTYPE_STRING, \
            "Convert string to uppercase"),
    USERFUNC_(Lower, ARGTYPE_STRING, \
            "Convert string to lowercase"),
    USERFUNC_(FormatUuid, ARGTYPE_(Object_None, Member_Uuid), \
            "Generate an idl format uuid string"),
    USERFUNC_(Rewrite, ARGTYPE_STRING, \
            "Overwrite previous writed string"),
    USERFUNC_(MacroRewrite, ARGTYPE_STRING, \
            "Rewrite macro string"),
    USERFUNC_(CStyleParamType, ARGTYPE_(Object_None, Member_Type), \
            "Generate C style type string of method's parameters"),
    USERFUNC_(TypeClass, ARGTYPE_(Object_None, Member_Type), \
            "Generate type class string"),
    USERFUNC_(CLS2CAR, ARGTYPE_(Object_Module, Member_None), \
            "Generate CAR source code"),
    USERFUNC_(CLS2Abrg, ARGTYPE_(Object_Module, Member_None), \
            "Generate abridged class info C code"),
    USERFUNC_(ClassNameOfSink, ARGTYPE_STRING, \
            "Get class name from sink name"),
    USERFUNC_(ClassNameOfClassObj, ARGTYPE_STRING, \
            "Get class name from class object"),
    USERFUNC_(NewOfGeneric, ARGTYPE_(Object_Class, Member_None), \
            "Generate the function New source code for the generic with constructors"),
    USERFUNC_(UsageNewOfCtor, ARGTYPE_(Object_Class, Member_None), \
            "Generate usage for the classes with New methods"),
    USERFUNC_(UsageNewOfGeneric, ARGTYPE_(Object_Class, Member_None), \
            "Generate usage for the generics with New methods"),
    USERFUNC_(NewHeaderOfGeneric, ARGTYPE_(Object_Class, Member_None), \
            "Generate New hader for the generic with constructors"),
    USERFUNC_(EmptyImplOfGeneric, ARGTYPE_(Object_Class, Member_None), \
            "Generate empty New implementations New for generics with constructors"),
    USERFUNC_(NewHeaderOfGenericClassObjects, ARGTYPE_(Object_Class, Member_None), \
            "Generate GenericCreateClassObjectWith source code in _*ClassObject_.cpp for generics with constructors"),
    USERFUNC_(NewHeaderOfGenericInterfaces, ARGTYPE_(Object_Interface, Member_None), \
            "Generate GenericCreateClassObjectWith source code in _*.h for generics with constructors"),
    USERFUNC_(ImplOfGenericClassObjects, ARGTYPE_(Object_Class, Member_None), \
            "Generate GenericCreateClassObjectWith implementation for the generic with constructors"),
    USERFUNC_(GenericInfoQI, ARGTYPE_(Object_Class, Member_None), \
            "Insert EIID_GENERIC_INFO Probe"),
    USERFUNC_(CreateHFiles, ARGTYPE_STRING, \
            "Create .h files from all libraries"),
    USERFUNC_(CreateCPPFiles, ARGTYPE_STRING, \
            "Create .cpp files from all libraries"),
    USERFUNC_(CStyleName, ARGTYPE_(Object_Param, Member_None), \
            "Generate C style name string"),
    USERFUNC_(DefaultInterface, ARGTYPE_(Object_Class, Member_None), \
            "Get default interface name from class"),
    USERFUNC_(PrefixingName, ARGTYPE_(Object_Param, Member_None), \
            "Generate prefixing variable name string"),
    USERFUNC_(PrefixingNameByName, ARGTYPE_(Object_Param, Member_None), \
            "Generate prefixing variable name string"),
    USERFUNC_(InterfaceNameOfSink, ARGTYPE_STRING, \
            "Convert string to callback format"),
    USERFUNC_(ParamOrigType, ARGTYPE_(Object_None, Member_Type), \
            "Generate original C style parameter type string"),
    USERFUNC_(ParamAddRef, ARGTYPE_(Object_Param, Member_None), \
            "If the type of parameter is Interface then call its AddRef()."),
    USERFUNC_(ParamRelease, ARGTYPE_(Object_Param, Member_None), \
            "If the type of parameter is Interface then call its Release()."),
    USERFUNC_(NamespaceType, ARGTYPE_(Object_None, Member_Type), \
            "Generate elastos namepasce prefixing"),
    USERFUNC_(ParamNamespaceType, ARGTYPE_(Object_None, Member_Type), \
            "Generate elastos namepasce prefixing for method parameters"),
    USERFUNC_(MemberNamespaceType, ARGTYPE_(Object_None, Member_Type), \
            "Generate elastos namepasce prefixing"),
    USERFUNC_(ParamType2String, ARGTYPE_(Object_None, Member_None), \
            "Generate paramters type string"),
    USERFUNC_(CopyOutParam, ARGTYPE_(Object_Param, Member_None), \
            "Generate copying code for output-parameters"),
    USERFUNC_(AndriodType, ARGTYPE_(Object_Param, Member_Type), \
            "Generate andriod C style type string"),
    USERFUNC_(WriteToAndriodParcel, ARGTYPE_(Object_Param, Member_Type), \
            "Generate code for writing parameters to andriod parcel"),
    USERFUNC_(DelcareVariablesForSoMethod, ARGTYPE_(Object_IntfMethod, Member_None), \
            "Generate code for reading parameters from andriod parcel"),
    USERFUNC_(MethodIndexInInterface, ARGTYPE_(Object_IntfMethod, Member_None), \
            "Generate the index of the method in its interface"),
    USERFUNC_(CallSoMethod, ARGTYPE_(Object_IntfMethod, Member_None), \
            "Generate the index of the method in its interface"),
    USERFUNC_(GenerateAndriodParameter, ARGTYPE_(Object_Param, Member_None), \
            "Generate parameters according to andriod"),
    USERFUNC_(GenerateCBDataParameter, ARGTYPE_(Object_Param, Member_None), \
            "Generate parameters according to andriod"),
    USERFUNC_(GenerateAndriodArgument, ARGTYPE_(Object_Param, Member_None), \
            "Generate parameters according to andriod"),
    USERFUNC_(ReadOutParamFromParcel, ARGTYPE_(Object_IntfMethod, Member_None), \
            "Generate parameters according to andriod"),
    USERFUNC_(ReadFromAndriodParcelOnTransact, ARGTYPE_(Object_IntfMethod, Member_None), \
            "Generate the parameters of the method in its interface"),
    USERFUNC_(PrepareParamForResponseCallback, ARGTYPE_(Object_Param, Member_None), \
            "Prepare Android parameters according to Elastos"),
    USERFUNC_(IsFiltered, ARGTYPE_(Object_ClsIntf, Member_None), \
            "Judge whether the interface is filtered"),
    USERFUNC_(HasTrivialConstructor, ARGTYPE_(Object_Class, Member_None), \
            "Judge whether the contructor method in the class object is a constructor generated by car compiler automatically"),
    USERFUNC_(HasDefaultConstructor, ARGTYPE_(Object_Class, Member_None), \
            "Judge whether the contructor method in the class object is a default constructor"),
    USERFUNC_(ParcelParameter, ARGTYPE_(Object_Param, Member_None), \
            "Parcel parameters"),
    USERFUNC_(HasParameters, ARGTYPE_(Object_IntfMethod, Member_None), \
            "Judge if the method has some parameters"),
    USERFUNC_(OrgClassIsAspect, ARGTYPE_(Object_Class, Member_None), \
            "Judge whether the original class of this sink class is aspect"),
    USERFUNC_(ClassNamespaceBegin, ARGTYPE_(Object_Class, Member_None), \
            "Generate the namespace beginning of the class"),
    USERFUNC_(ClassNamespaceEnd, ARGTYPE_(Object_Class, Member_None), \
            "Generate the namespace end of the class"),
    USERFUNC_(InterfaceNamespaceBegin, ARGTYPE_(Object_Interface, Member_None), \
            "Generate the namespace beginning of the interface"),
    USERFUNC_(InterfaceNamespaceEnd, ARGTYPE_(Object_Interface, Member_None), \
            "Generate the namespace end of the interface"),
    USERFUNC_(EnumNamespaceBegin, ARGTYPE_(Object_Enum, Member_None), \
            "Generate the namespace beginning of the enum"),
    USERFUNC_(EnumNamespaceEnd, ARGTYPE_(Object_Enum, Member_None), \
            "Generate the namespace end of the enum"),
};
const int c_cUserFuncs = sizeof(g_userFuncs) / sizeof(UserFuncEntry);

IMPL_USERFUNC(PrintMessage)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    fputs((char *)pvArg, stdout);
    return LUBE_OK;
}

IMPL_USERFUNC(ErrorMessage)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    fputs((char *)pvArg, stderr);
    return LUBE_FAIL;
}

#ifdef _linux
static void _strupr(char* str)
{
    int i = 0;
    while(str[i] != '\0') {
        str[i] = toupper(str[i]);
        i++;
    }
}

static void _strlwr(char* str)
{
    int i = 0;
    while(str[i] != '\0') {
        str[i] = tolower(str[i]);
        i++;
    }
}
#endif

IMPL_USERFUNC(Upper)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    char *pString = _strdup((char *)pvArg);

    _strupr(pString);
    int n = strlen(pString);
    for(int i = 0; i < n; i++) {
        if (*(pString + i) == '.') {
            *(pString + i) = '_';
        }
    }
    pCtx->PutString(pString);
    free(pString);

    return LUBE_OK;
}

IMPL_USERFUNC(Lower)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    char *pString = _strdup((char *)pvArg);

    _strlwr(pString);
    pCtx->PutString(pString);
    free(pString);

    return LUBE_OK;
}

IMPL_USERFUNC(InterfaceNameOfSink)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    char *pString = _strdup((char *)pvArg);
    int len = strlen(pString);
    if (len > 8) len -= 8;
    pString[len] = '\0'; // cut "Callback"
    pCtx->PutString(pString);
    free(pString);

    return LUBE_OK;
}

IMPL_USERFUNC(Embed)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    pCtx->PutString((char *)pvArg);
    return LUBE_OK;
}

IMPL_USERFUNC(Rewrite)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    if (pCtx->m_pFile) {
        int n = strlen((char *)pvArg);
        fseek(pCtx->m_pFile, -n, SEEK_CUR);
        fputs((char *)pvArg, pCtx->m_pFile);
    }
    return LUBE_OK;
}

IMPL_USERFUNC(MacroRewrite)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    char *pString = _strdup((char *)pvArg);

    int n = strlen(pString);
    for(int i = 0; i < n; i++) {
        if (*(pString + i) == '.') {
            *(pString + i) = '_';
        }
    }
    pCtx->PutString(pString);
    free(pString);

    return LUBE_OK;
}

IMPL_USERFUNC(FormatUuid)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    pCtx->PutString(Uuid2IString((GUID *)pvArg, TRUE));
    return LUBE_OK;
}

IMPL_USERFUNC(CLS2CAR)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    if (pCtx->m_pFile) {
        return CLS2CAR_(pCtx->m_pFile, (CLSModule *)pvArg);
    }
    return LUBE_OK;
}

IMPL_USERFUNC(CLS2Abrg)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    if (pCtx->m_pFile) {
        return CLS2AbrgCpp_(pCtx->m_pFile, (CLSModule *)pvArg);
    }
    return LUBE_OK;
}

IMPL_USERFUNC(CStyleParamType)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    assert(NULL != pCtx->m_pParam);

    const char *pszType = NULL;
    char szType[128];
    TypeDescriptor type, *pType = (TypeDescriptor *)pvArg;
    DWORD dwAttribs = pCtx->m_pParam->dwAttribs;

    switch (pType->type) {
        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &type);
            if ((Type_EMuid == type.type)
                    || (Type_EGuid == type.type)
                    || (Type_DateTime == type.type)
                    || (Type_struct == type.type)){
                if (dwAttribs & ParamAttrib_in) {
                    if (0 == type.nPointer) {
                        assert(0 == pType->nPointer);
                        sprintf(szType, "const %s *", Type2CString(pCtx->m_pModule, pType));
                        pszType = szType;
                    }
                    else pszType = Type2CString(pCtx->m_pModule, pType);
                }
                else pszType = Type2CString(pCtx->m_pModule, pType);
            }
            else if ((Type_ArrayOf == type.type)
                    || (Type_BufferOf == type.type)
                    || (Type_MemoryBuf == type.type)
                    || (Type_StringBuf == type.type)) {
                if (dwAttribs & ParamAttrib_in) {
                    if (0 == type.nPointer) {
                        assert(0 == pType->nPointer);
                        sprintf(szType, "const %s *", Type2CString(pCtx->m_pModule, pType));
                        pszType = szType;
                    }
                    else pszType = Type2CString(pCtx->m_pModule, pType);
                }
                else {
                    if (dwAttribs & ParamAttrib_callee) {
                        if (0 == pType->nPointer) {
                            assert(1 == type.nPointer);
                            sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                        }
                        else if (1 == pType->nPointer) {
                            assert(0 == type.nPointer);
                            sprintf(szType, "%s **", Type2CString(pCtx->m_pModule, pType));
                        }
                        pszType = szType;
                    }
                    else {
                        assert(0 == type.nPointer);
                        assert(0 == pType->nPointer);
                        sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                        pszType = szType;
                    }
                }
            }
            else pszType = Type2CString(pCtx->m_pModule, pType);
            break;

        case Type_EMuid:
        case Type_EGuid:
        case Type_DateTime:
        case Type_struct:
            if (dwAttribs & ParamAttrib_in) {
                if (0 == pType->nPointer) {
                    sprintf(szType, "const %s *", Type2CString(pCtx->m_pModule, pType));
                    pszType = szType;
                }
                else pszType = Type2CString(pCtx->m_pModule, pType);
            }
            else pszType = Type2CString(pCtx->m_pModule, pType);
            break;

        case Type_ArrayOf:
        case Type_BufferOf:
            if (dwAttribs & ParamAttrib_in) {
                pszType = "PCarQuintet";    //const
            }
            if (dwAttribs & ParamAttrib_out) {
                if (dwAttribs & ParamAttrib_callee) pszType = "PCarQuintet *";
                else pszType = "PCarQuintet";
            }
            break;

        case Type_MemoryBuf:
            if (dwAttribs & ParamAttrib_in) pszType = "const MemoryBuf *";
            else {
                if (dwAttribs & ParamAttrib_callee) pszType = "MemoryBuf **";
                else pszType = "MemoryBuf *";
            }
            break;

        case Type_StringBuf:
            if (dwAttribs & ParamAttrib_callee) pszType = "StringBuf **";
            else pszType = "StringBuf *";
            break;

        case Type_CString:
            assert(pType->nPointer <= 1);
            if (1 == pType->nPointer) {
                pszType = "CString*";
            }
            else if (0 == pType->nPointer) {
                pszType = "CString";
            }
            break;

        case Type_String:
            assert(pType->nPointer <= 1);
            if (1 == pType->nPointer) {
                pszType = "String*";
            }
            else if (0 == pType->nPointer) {
                pszType = "String";
            }
            break;

        default:
            pszType = Type2CString(pCtx->m_pModule, pType);
            break;
    }

    pCtx->PutString(pszType);

    return LUBE_OK;
}

IMPL_USERFUNC(TypeClass)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    const char *pszType;
    TypeDescriptor *pType = (TypeDescriptor *)pvArg;

    // TODO: need to improve
    TypeDescriptor orgType;

Restart:
    switch (pType->type) {
        case Type_Char8:
        case Type_Char16:
        case Type_Int8:
        case Type_Int16:
        case Type_Int32:
        case Type_Int64:
        case Type_UInt16:
        case Type_UInt32:
        case Type_UInt64:
        case Type_Byte:
        case Type_Boolean:
        case Type_Float:
        case Type_Double:
        case Type_PVoid:
        case Type_ECode:
        case Type_EventHandler:
        case Type_enum:
            pszType = "Intrinsic";
            break;

        case Type_EMuid:
            pszType = "EMuid";
            break;

        case Type_EGuid:
            pszType = "EGuid";
            break;

        case Type_ArrayOf:
            if (Type_CString == pType->pNestedType->type) {
                pszType = "ArrayOfCString";
            }
            else if (Type_String == pType->pNestedType->type) {
                pszType = "ArrayOfString";
            }
            else {
                pszType = "CarArray";
            }
            break;
        case Type_BufferOf:
            if (Type_CString == pType->pNestedType->type) {
                pszType = "BufferOfCString";
            }
            else if (Type_String == pType->pNestedType->type) {
                pszType = "BufferOfString";
            }
            else {
                pszType = "CarArray";
            }
            break;

        case Type_MemoryBuf:
        case Type_StringBuf:
        case Type_Array:
            pszType = "CarArray";
            break;

        case Type_interface:
            pszType = "Interface";
            break;

        case Type_struct:
            pszType = "Struct";
            break;

        case Type_CString:
            pszType = "CString";
            break;

        case Type_String:
            pszType = "String";
            break;

        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &orgType);
            pType = &orgType;
            goto Restart;

        case Type_const:
        default:
            pszType = "Unknown";
            break;
    }

    pCtx->PutString(pszType);

    return LUBE_OK;
}

IMPL_USERFUNC(ClassNameOfSink)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert (NULL != pvArg);

    char szName[c_nStrBufSize];

    strcpy(szName, (char *)pvArg);
    int len = strlen(szName);
    if (len > 4) len -= 4;
    szName[len] = 0; // cut "Sink"
    pCtx->PutString(szName);

    return LUBE_OK;
}

IMPL_USERFUNC(ClassNameOfClassObj)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert (NULL != pvArg);

    char szName[c_nStrBufSize];

    strcpy(szName, (char *)pvArg);
    szName[strlen(szName) - 11] = 0;
    pCtx->PutString(szName);

    return LUBE_OK;
}

#define CTOR_LOOP_BEGIN() \
    assert(pCtx->m_pClass->pDesc->sCtorIndex);\
    InterfaceDirEntry *pCtorInterface = \
                pCtx->m_pModule->ppInterfaceDir[pCtx->m_pClass->pDesc->sCtorIndex];\
    int m, p, i;\
    char szBuf[c_nStrBufSize];\
    int nLoopNum = 0;\
    int nLoopEnd = 3;\
    TypeDescriptor type, OrigType;\
    const char *string;\
    for (i = 0; i < pCtx->m_pClass->pDesc->cInterfaces; i++) {\
        if (pCtx->m_pClass->pDesc->ppInterfaces[i]->wAttribs & ClassInterfaceAttrib_callback) {\
            continue;\
        }\
        /*For all constructor methods*/ \
        for (m = 0; m < pCtorInterface->pDesc->cMethods; m++) {\
            nLoopNum = 0;\
            do {\
                ++nLoopNum;

#define CTOR_PARAMS() \
                pCtx->PutString("        ");\
                for (p = 0; p < pCtorInterface->pDesc->ppMethods[m]->cParams - 1; p++) {\
                    memset(szBuf, 0, c_nStrBufSize);\
                    type = pCtorInterface->pDesc->ppMethods[m]->ppParams[p]->type;\
                    if (Type_struct == type.type || Type_EMuid == type.type ||\
                        Type_EGuid == type.type || Type_BufferOf == type.type ||\
                        Type_ArrayOf == type.type) {\
                        string = StructType2CString(pCtx->m_pModule, &type);\
                    }\
                    else if (Type_alias == type.type) {\
                        GetOriginalType(pCtx->m_pModule, &type, &OrigType);\
                        if ((Type_struct == OrigType.type || Type_EMuid == OrigType.type ||\
                             Type_EGuid == OrigType.type || Type_BufferOf == OrigType.type ||\
                             Type_ArrayOf == type.type)\
                             && OrigType.nPointer == 0) {\
                            string = StructType2CString(pCtx->m_pModule, &type);\
                        }\
                        else {\
                            string = Type2CString(pCtx->m_pModule, &type);\
                        }\
                    }\
                    else {\
                        string = Type2CString(pCtx->m_pModule, &type);\
                    }\
                    sprintf(szBuf, "/*[in]*/  %s %s,\n", string, \
                            pCtorInterface->pDesc->ppMethods[m]->ppParams[p]->pszName);\
                    pCtx->PutString(ImplNamespaceType(szBuf));\
                    pCtx->PutString("        ");\
                }\
                memset(szBuf, 0, c_nStrBufSize);\
                sprintf(szBuf, "/*[out]*/ %s **pp%s", \
                  pCtx->m_pModule->ppInterfaceDir[pCtx->m_pClass->pDesc->ppInterfaces[i]->sIndex]->pszName, \
                  pCtx->m_pModule->ppInterfaceDir[pCtx->m_pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);\
                pCtx->PutString(ImplNamespaceType(szBuf));\

#define CTOR_LOOP_END() \
            } while (nLoopNum < nLoopEnd);\
        }\
    }\

IMPL_USERFUNC(UsageNewOfCtor)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    CTOR_LOOP_BEGIN()
                if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_singleton) {
                    if (1 == nLoopNum) {
                        //pCtx->PutString("static _ELASTOS ECode ");
                        pCtx->PutString(pCtx->m_pClass->pszName);
                        pCtx->PutString("::AcquireSingletonInDomain(\n");
                        pCtx->PutString("        /*[in]*/  PRegime pRegime,\n");
                    }
                    else {
                        //pCtx->PutString("static _ELASTOS ECode ");
                        pCtx->PutString(pCtx->m_pClass->pszName);
                        pCtx->PutString("::AcquireSingleton(\n");
                    }
                }
                else {
                    if (1 == nLoopNum) {
                        //pCtx->PutString("static _ELASTOS ECode ");
                        pCtx->PutString(pCtx->m_pClass->pszName);
                        pCtx->PutString("::NewInRegime(\n");
                        pCtx->PutString("        /*[in]*/  PRegime pRegime,\n");
                    }
                    else {
                        //pCtx->PutString("static _ELASTOS ECode ");
                        pCtx->PutString(pCtx->m_pClass->pszName);
                        pCtx->PutString("::New(\n");
                    }
                }
                CTOR_PARAMS()
                pCtx->PutString(");\n");
    CTOR_LOOP_END()
    return LUBE_OK;
}

/*
 * This template will prepare class from given class object interface!
 */
#define PREPARE_CLASS(pszName, before, after) \
    ClassDirEntry *pClass = NULL;\
    char szName[c_nStrBufSize];\
    strcpy(szName, (char *)pszName + before);\
    szName[strlen(szName) - after] = 0;\
    int r = SelectClassDirEntry(szName, NULL, pCtx->m_pModule);\
    if (r < 0) { /* Should never be in here! */ \
        /* ignore non existed generic class silently! */ \
        return LUBE_OK;\
    }\
    pClass = pCtx->m_pModule->ppClassDir[r];\
    if (0 == (pClass->pDesc->dwAttribs & ClassAttrib_t_generic)) {\
        return LUBE_OK;\
    }\

#define START_FOR_LOOP(pClass) \
    int m, p, i;\
    int length;\
    char szBuf[c_nStrBufSize];\
    char *pszName;\
    InterfaceDirEntry *pCtorInterface = NULL;\
    pCtorInterface = pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->sCtorIndex];\
    for (i = 0; i < pClass->pDesc->cInterfaces; i++) {\
        if (pClass->pDesc->ppInterfaces[i]->wAttribs & ClassInterfaceAttrib_callback) {\
            continue;\
        }\
        pszName = pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName;\
        if (!strcmp(pszName, "IObject")) {\
            continue;\
        }\
        /*For all constructor methods*/ \
        length = pCtorInterface->pDesc->cMethods;\
        for (m = 0; m < length; m++) {\

/* generate method declaration sources */

#define DECLARE_PARAMS(pClass, spaces) \
            pCtx->PutString("\n");\
            pCtx->PutString(spaces);\
            for (p = 0; p < pCtorInterface->pDesc->ppMethods[m]->cParams - 1; p++) {\
                memset(szBuf, 0, c_nStrBufSize);\
                sprintf(szBuf, "/*[in]*/  %s %s,\n", \
                        Type2CString(pCtx->m_pModule, \
                        &(pCtorInterface->pDesc->ppMethods[m]->ppParams[p]->type)), \
                        pCtorInterface->pDesc->ppMethods[m]->ppParams[p]->pszName);\
                pCtx->PutString(ImplNamespaceType(szBuf));\
                pCtx->PutString(spaces);\
            }\
            memset(szBuf, 0, c_nStrBufSize);\
            sprintf(szBuf, "/*[out]*/ %s **pp%s", \
                pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName, \
                pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);\
            pCtx->PutString(ImplNamespaceType(szBuf));\

/* Here will be main method body codes or nothing */

#define END_FOR_LOOP \
        }\
    }\

#define INVOKE_PARAMS(pClass, spaces) \
            pCtx->PutString("\n");\
            pCtx->PutString(spaces);\
            for (p = 0; p < pCtorInterface->pDesc->ppMethods[m]->cParams - 1; p++) {\
                pCtx->PutString(pCtorInterface->pDesc->ppMethods[m]->ppParams[p]->pszName);\
                pCtx->PutString(",\n");\
                pCtx->PutString(spaces);\
            }\
            pCtx->PutString("pp");\
            pCtx->PutString(pCtx->m_pModule->\
                    ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);\

//Create the method name combined with paramter names
#define GENERIC_UPPER_CASE_PARAMS(pCtorInterface) \
    char szMethodName[c_nStrBufSize];\
    pCtx->PutString("GenericCreateClassObjectWith");\
    int nMeth;\
    for (nMeth = 0; nMeth < pCtorInterface->pDesc->ppMethods[m]->cParams - 1; nMeth++) {\
        if (pCtorInterface->pDesc->ppMethods[m]->ppParams[nMeth]->dwAttribs & ParamAttrib_out) {\
            continue;\
        }\
        memset(szMethodName, 0, c_nStrBufSize);\
        strcpy(szMethodName, pCtorInterface->pDesc->ppMethods[m]->ppParams[nMeth]->pszName);\
        if (szMethodName[0] >= 'a' && szMethodName[0] <= 'z') {\
            szMethodName[0] -= 'a' - 'A';\
        }\
        pCtx->PutString(szMethodName);\
    }\

IMPL_USERFUNC(ImplOfGenericClassObjects)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    PREPARE_CLASS(pCtx->m_pClass->pszName, 0, 11)
    START_FOR_LOOP(pClass)
            sprintf(szBuf, "ECode %s::", pCtx->m_pClass->pszName);
            pCtx->PutString(szBuf);
            GENERIC_UPPER_CASE_PARAMS(pCtorInterface)
            pCtx->PutString("(");
            DECLARE_PARAMS(pClass, "    ");
            pCtx->PutString(")\n{\n");

            pCtx->PutString("    EMuid *pClsid = NULL;\n");
            sprintf(szBuf, "    _ELASTOS ECode ec = %s::New(", szName);
            pCtx->PutString(szBuf);
            INVOKE_PARAMS(pClass, "        ")
            pCtx->PutString("\n    );\n");
            pCtx->PutString("    if (FAILED(ec)) return ec;\n");

            sprintf(szBuf, "    pClsid = (EMuid*)(*pp%s)->Probe(EIID_GENERIC_INFO);\n",
                    pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);
            pCtx->PutString(szBuf);
            pCtx->PutString("    if (!pClsid) {\n");
            sprintf(szBuf, "        (*pp%s)->Release();\n",
                    pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);
            pCtx->PutString(szBuf);
            pCtx->PutString("        return E_WRONG_GENERIC;\n");
            pCtx->PutString("    }\n");
            sprintf(szBuf, "    if (*pClsid != ECLSID_%s) {\n", szName);
            pCtx->PutString(szBuf);
            sprintf(szBuf, "        (*pp%s)->Release();\n",
                    pCtx->m_pModule->ppInterfaceDir[pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);
            pCtx->PutString(szBuf);
            pCtx->PutString("        return E_WRONG_GENERIC;\n");
            pCtx->PutString("    }\n");
            pCtx->PutString("    return ec;\n");
            pCtx->PutString("}\n\n");
    END_FOR_LOOP

    return LUBE_OK;
}

IMPL_USERFUNC(NewHeaderOfGenericClassObjects)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    PREPARE_CLASS(pCtx->m_pClass->pszName, 0, 11)
    START_FOR_LOOP(pClass)
            pCtx->PutString("    CARAPI ");
            GENERIC_UPPER_CASE_PARAMS(pCtorInterface)
            pCtx->PutString("(");
            DECLARE_PARAMS(pClass, "        ")
            pCtx->PutString(");\n\n");
    END_FOR_LOOP
    return LUBE_OK;
}

IMPL_USERFUNC(NewHeaderOfGenericInterfaces)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    PREPARE_CLASS(pCtx->m_pInterface->pszName, 1, 11)
    START_FOR_LOOP(pClass)
            pCtx->PutString("    virtual CARAPI ");
            GENERIC_UPPER_CASE_PARAMS(pCtorInterface)
            pCtx->PutString("(");
            DECLARE_PARAMS(pClass, "        ")
            pCtx->PutString(") = 0;\n\n");
    END_FOR_LOOP
    return LUBE_OK;
}

IMPL_USERFUNC(NewHeaderOfGeneric)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    START_FOR_LOOP(pCtx->m_pClass)
            pCtx->PutString("    static CARAPI New(");
            DECLARE_PARAMS(pCtx->m_pClass, "        ")
            pCtx->PutString(");\n");
    END_FOR_LOOP
    return LUBE_OK;
}

IMPL_USERFUNC(EmptyImplOfGeneric)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    START_FOR_LOOP(pCtx->m_pClass)
            sprintf(szBuf, "ECode %s::New(", pCtx->m_pClass->pszName);
            pCtx->PutString(szBuf);
            DECLARE_PARAMS(pCtx->m_pClass, "    ")
            pCtx->PutString(")\n{\n");
            pCtx->PutString("    // TODO: Add your code here\n");
            pCtx->PutString("    return E_NOT_IMPLEMENTED;\n");
            pCtx->PutString("}\n\n");
    END_FOR_LOOP
    return LUBE_OK;
}

IMPL_USERFUNC(NewOfGeneric)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    START_FOR_LOOP(pCtx->m_pClass)
            pCtx->PutString("    static CARAPI New(");
            DECLARE_PARAMS(pCtx->m_pClass, "        ")
            pCtx->PutString(")\n    {\n");
            pCtx->PutString("        _ELASTOS ECode _ecode = NOERROR;\n");
            memset(szBuf, 0, c_nStrBufSize);
            sprintf(szBuf, "        %s *_p%s = NULL;\n\n",
                    pCtorInterface->pszName, pCtorInterface->pszName);
            pCtx->PutString(szBuf);
            pCtx->PutString("        _ecode = _CObject_AcquireClassFactory(ECLSID_");
            pCtx->PutString(pCtx->m_pClass->pszName);
//            pCtx->PutString("ClassObject,\n");
            pCtx->PutString(",\n");
            pCtx->PutString("                ");
            pCtx->PutString("RGM_SAME_DOMAIN, ");
            pCtx->PutString("EIID_I");
            pCtx->PutString(pCtx->m_pClass->pszName);
            pCtx->PutString("ClassObject, ");
            pCtx->PutString("(IInterface**)&_p");
            pCtx->PutString(pCtorInterface->pszName);
            pCtx->PutString(");\n");
            pCtx->PutString("        if (FAILED(_ecode)) goto Exit;\n\n");
            pCtx->PutString("        _ecode = _p");
            pCtx->PutString(pCtorInterface->pszName);
            pCtx->PutString("->");
            GENERIC_UPPER_CASE_PARAMS(pCtorInterface)
            pCtx->PutString("(");
            INVOKE_PARAMS(pCtx->m_pClass, "            ")
            pCtx->PutString("\n        );\n");
            pCtx->PutString("        if (FAILED(_ecode)) goto Exit;\n\n");
            pCtx->PutString("    Exit:\n");
            pCtx->PutString("        if (_p");
            pCtx->PutString(pCtorInterface->pszName);
            pCtx->PutString(") _p");
            pCtx->PutString(pCtorInterface->pszName);
            pCtx->PutString("->Release();\n");
            pCtx->PutString("        return _ecode;\n");
            pCtx->PutString("    }\n\n");
    END_FOR_LOOP
    return LUBE_OK;
}

IMPL_USERFUNC(UsageNewOfGeneric)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    START_FOR_LOOP(pCtx->m_pClass)
            //pCtx->PutString("static _ELASTOS ECode ");
            pCtx->PutString(pCtx->m_pClass->pszName);
            pCtx->PutString("::New(");
            DECLARE_PARAMS(pCtx->m_pClass, "        ")
            pCtx->PutString(")\n");
    END_FOR_LOOP
    return LUBE_OK;
}

IMPL_USERFUNC(GenericInfoQI)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    ClassDirEntry *pClass = pCtx->m_pClass;

    /* generic class, if existed, will always be the root parent */
    while (pClass->pDesc->dwAttribs & ClassAttrib_hasparent) {
        pClass = pCtx->m_pModule->ppClassDir[pClass->pDesc->sParentIndex];
    }
    pCtx->PutString("        return ");
    if (pClass->pDesc->dwAttribs & ClassAttrib_t_generic) {
        pCtx->PutString("(IInterface *)&ECLSID_");
        pCtx->PutString(pClass->pszName);
        pCtx->PutString(";\n");
    }
    else {
        pCtx->PutString("NULL;\n");
    }
    return LUBE_OK;
}

//
//Compare the last modified time of XXX.h & XXX.dll files to avoid repeated building .h file
//
int CmpHModTime(char* szName)
{
    char szPath[_MAX_PATH];
    struct stat statHFileInfo;
    int nRet;
    char szBuf[_MAX_PATH];

    GetNakedFileName(szName, szBuf);
    char *szHName = (char *)alloca(strlen(szBuf) + 4);
    if (!szHName) return LUBE_FAIL;
    strcpy(szHName, szBuf);
    strcat(szHName, ".h");

    if (!strlen(g_pszOutputPath)) {
        if (getcwd(szPath, _MAX_PATH) == NULL) {
            fprintf(stderr, "Warning: _getcwd error!\n");
            return LUBE_FALSE;
        }
    }
    else
        strcpy(szPath, g_pszOutputPath);

#ifdef _win32
        strcat(szName, "\\");
#else
        strcat(szName, "/");
#endif
    strcat(szPath, szHName);

    if (-1 != access(szPath, 0)) {
        nRet = stat(szPath, &statHFileInfo);
        if (-1 == nRet) {
            return LUBE_FALSE;
        }
    }
    else
        return LUBE_OK;

    //Get DLL / cls file info
    struct stat statFileInfo;

    nRet = SearchFileFromPath(getenv("PATH"), szName, szPath);
    if (0 == nRet) {
        nRet = stat(szPath, &statFileInfo);
        if (-1 == nRet) {
            return LUBE_FALSE;
        }
    }
    else
        return LUBE_FAIL;

    //Compare  last modified time of .h & .dll files
    if (statHFileInfo.st_mtime > statFileInfo.st_mtime) {
        return HTIME_GT_DLLTIME;
    }
    else {
        return HTIME_LT_DLLTIME;
    }
}

IMPL_USERFUNC(CreateHFiles)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    char *szName = (char *)alloca(strlen((char *)pvArg) + 4);
    int nRet;
    CLSModule *pModule;

    strcpy(szName, (char *)pvArg);

    nRet = LoadCLS(szName, &pModule);
    if (nRet == CLSError_NotFound) {
        printf("Warning --Cannot find %s. Ignored.\n", szName);
        return LUBE_FALSE;
    }
    else if (nRet < 0)
        return LUBE_FAIL;

    pCtx->PutString("#include <");
    pCtx->PutString(pModule->pszName);
    pCtx->PutString(".h>\n");

    PLUBEHEADER pLube;
    if (LoadLube(NULL, &pLube) < 0) {
        return LUBE_FAIL;
    }

    LubeContext ctx(pLube, pModule, 2);

    //Compare .h file and .dll/.cls file to avoid repeated building problem
    nRet = CmpHModTime(szName);

    if (nRet >= 0) nRet = ctx.ExecTemplate("header");

    DisposeFlattedCLS(pModule);
    if (nRet < 0 && nRet != HTIME_GT_DLLTIME)
        return nRet;

    return LUBE_OK;
}

IMPL_USERFUNC(CreateCPPFiles)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    char *szName = (char *)alloca(strlen((char *)pvArg) + 4);
    int nRet;
    CLSModule *pModule;

    strcpy(szName, (char *)pvArg);

    nRet = LoadCLS(szName, &pModule);
    if (nRet == CLSError_NotFound) {
        printf("Warning --Cannot find %s. Ignored.\n", szName);
        return LUBE_FALSE;
    }
    else if (nRet < 0)
        return LUBE_FAIL;

    PLUBEHEADER pLube;
    if (LoadLube(NULL, &pLube) < 0) {
        return LUBE_FAIL;
    }

    LubeContext ctx(pLube, pModule, 2);

    //Compare .h file and .dll/.cls file to avoid repeated building problem
    nRet = CmpHModTime(szName);

    if (nRet >= 0) nRet = ctx.ExecTemplate("headercpp");

    DisposeFlattedCLS(pModule);
    if (nRet < 0 && nRet != HTIME_GT_DLLTIME)
        return nRet;

    return LUBE_OK;
}

int GetCStyleStructParamName(PLUBECTX pCtx, const ParamDescriptor *pParamDesc)
{
    assert(Type_struct == pParamDesc->type.type);

    char szName[c_nStrBufSize];
    szName[0] = 0;
    // If struct parameter type is't pointer, we have to change its type
    // to a pointer and prefix 'p' to its name.
    if (0 == pParamDesc->type.nPointer) {
        sprintf(szName, "p%s", pParamDesc->pszName);
        pCtx->PutString(szName);
    }
    else {
        pCtx->PutString(pParamDesc->pszName);
    }

    return LUBE_OK;
}

IMPL_USERFUNC(CStyleName)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    ParamDescriptor *pParamDesc;

    assert(pvArg);

    pParamDesc = (ParamDescriptor *)pvArg;

    switch (pParamDesc->type.type) {
        case Type_struct:
            return GetCStyleStructParamName(pCtx, pParamDesc);
        default:
            pCtx->PutString(pParamDesc->pszName);
            break;
    }

    return LUBE_OK;
}

IMPL_USERFUNC(DefaultInterface)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    int i;

    for (i = 0; i < pCtx->m_pClass->pDesc->cInterfaces; i++) {
        if (pCtx->m_pClass->pDesc->ppInterfaces[i]->wAttribs & ClassInterfaceAttrib_callback) {
            continue;
        }
        pCtx->PutString(pCtx->m_pModule->ppInterfaceDir[pCtx->m_pClass->pDesc->ppInterfaces[i]->sIndex]->pszName);
        return LUBE_OK;
    }

    return LUBE_FAIL;
}

#define TYPE_CASE(type, p0, p1, p2) \
        case type:\
            pszPrefix0 = p0;\
            pszPrefix1 = p1;\
            pszPrefix2 = p2;\
            break;

IMPL_USERFUNC(PrefixingNameByName)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    ParamDescriptor *pParamDesc = (ParamDescriptor *)pvArg;
    const char *pszPrefix0 = NULL;
    const char *pszPrefix1 = NULL;
    const char *pszPrefix2 = NULL;
    char *pszPrefix = NULL;

    TypeDescriptor paramType = pParamDesc->type;
    if (Type_alias == pParamDesc->type.type) {
        GetOriginalType(pCtx->m_pModule, &pParamDesc->type, &paramType);
    }

    switch (paramType.type) {

        TYPE_CASE(Type_Int8, NULL, "p", NULL)
        TYPE_CASE(Type_Byte, NULL, "p", NULL)
        TYPE_CASE(Type_Int16, NULL, "p", NULL)
        TYPE_CASE(Type_UInt16, NULL, "p", NULL)
        TYPE_CASE(Type_Int32, NULL, "p", NULL)
        TYPE_CASE(Type_UInt32, NULL, "p", NULL)
        TYPE_CASE(Type_Int64, NULL, "p", NULL)
        TYPE_CASE(Type_UInt64, NULL, "p", NULL)
        TYPE_CASE(Type_Float, NULL, "p", NULL)
        TYPE_CASE(Type_Double, NULL, "p", NULL)
        TYPE_CASE(Type_Boolean, NULL, "p", NULL)
        TYPE_CASE(Type_Char8, NULL, "p", NULL)
        TYPE_CASE(Type_Char16, NULL, "p", NULL)
        TYPE_CASE(Type_PVoid, "p", "pp", NULL)
        TYPE_CASE(Type_CString, NULL, "p", NULL)
        TYPE_CASE(Type_String, NULL, "p", NULL)
        TYPE_CASE(Type_StringBuf, "p", "pp", NULL)
        TYPE_CASE(Type_ArrayOf, "p", "pp", NULL)
        TYPE_CASE(Type_BufferOf, "p", "pp", NULL)
        TYPE_CASE(Type_MemoryBuf, "p", "pp", NULL)
        case Type_enum:
        case Type_struct:
            pszPrefix1 = "p";
            pszPrefix2 = NULL;
            break;
        default:
            pszPrefix1 = "p";
            pszPrefix2 = "pp";
            break;
    }

    char* pszVarName;

    pszVarName = pParamDesc->pszName;
    if (pParamDesc->dwAttribs & ParamAttrib_in) {
        if ((Type_PVoid == paramType.type)
                || (1 == paramType.nPointer)) {
            pszPrefix = GeneratePrefixalVarName("p", pszVarName);
        }
    }
    else if (pParamDesc->dwAttribs & ParamAttrib_out) {
        if (1 == paramType.nPointer) { // Considered as caller
            pszPrefix = GeneratePrefixalVarName(pszPrefix1, pszVarName);
        }
        else if (2 == paramType.nPointer) { // Considered as callee
            pszPrefix = GeneratePrefixalVarName(pszPrefix2, pszVarName);
        }
        else {
            pszPrefix = GeneratePrefixalVarName(pszPrefix0, pszVarName);
        }
    }
    // do nothing to those variables with neither [in] nor [out] attributes!
    if (pszPrefix != NULL) {
        pCtx->PutString(pszPrefix);
        delete pszPrefix;
    }
    else {
        pCtx->PutString(pszVarName);
    }
    return LUBE_OK;
}
IMPL_USERFUNC(PrefixingName)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    ParamDescriptor *pParamDesc = (ParamDescriptor *)pvArg;
    const char *pszPrefix0 = NULL;
    const char *pszPrefix1 = NULL;
    const char *pszPrefix2 = NULL;
    char *pszPrefix = NULL;

    TypeDescriptor paramType = pParamDesc->type;
    if (Type_alias == pParamDesc->type.type) {
        GetOriginalType(pCtx->m_pModule, &pParamDesc->type, &paramType);
    }

    switch (paramType.type) {

        TYPE_CASE(Type_Int8, NULL, "p", NULL)
        TYPE_CASE(Type_Byte, NULL, "p", NULL)
        TYPE_CASE(Type_Int16, NULL, "p", NULL)
        TYPE_CASE(Type_UInt16, NULL, "p", NULL)
        TYPE_CASE(Type_Int32, NULL, "p", NULL)
        TYPE_CASE(Type_UInt32, NULL, "p", NULL)
        TYPE_CASE(Type_Int64, NULL, "p", NULL)
        TYPE_CASE(Type_UInt64, NULL, "p", NULL)
        TYPE_CASE(Type_Float, NULL, "p", NULL)
        TYPE_CASE(Type_Double, NULL, "p", NULL)
        TYPE_CASE(Type_Boolean, NULL, "p", NULL)
        TYPE_CASE(Type_Char8, NULL, "p", NULL)
        TYPE_CASE(Type_Char16, NULL, "p", NULL)
        TYPE_CASE(Type_PVoid, "p", "pp", NULL)
        TYPE_CASE(Type_CString, NULL, "p", NULL)
        TYPE_CASE(Type_String, NULL, "p", NULL)
        TYPE_CASE(Type_ArrayOf, "p", "pp", NULL)
        TYPE_CASE(Type_BufferOf, "p", "pp", NULL)
        TYPE_CASE(Type_MemoryBuf, "p", "pp", NULL)
        TYPE_CASE(Type_StringBuf, "p", "pp", NULL)
        case Type_enum:
        case Type_struct:
            pszPrefix1 = "p";
            pszPrefix2 = NULL;
            break;
        default:
            pszPrefix1 = "p";
            pszPrefix2 = "pp";
            break;
    }

    char* pszVarName;
    char szValName[12];
    int nParam = 0;

    if (pCtx->m_pClass && pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) nParam = 1;

    for (; nParam < pCtx->m_pMethod->cParams; ++nParam) {
        if (pParamDesc == pCtx->m_pMethod->ppParams[nParam]) {
            if (pCtx->m_pClass && pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) nParam -= 1;
            break;
        }
    }
    sprintf(szValName, "Param%d", nParam);

    pszVarName = szValName;

    if (pParamDesc->dwAttribs & ParamAttrib_in) {
        if ((Type_PVoid == paramType.type)
                || (1 == paramType.nPointer)) {
            pszPrefix = GeneratePrefixalVarName("p", pszVarName);
        }
    }
    else if (pParamDesc->dwAttribs & ParamAttrib_out) {
        if (1 == paramType.nPointer) {
            pszPrefix = GeneratePrefixalVarName(pszPrefix1, pszVarName);
        }
        else if (2 == paramType.nPointer) {
            pszPrefix = GeneratePrefixalVarName(pszPrefix2, pszVarName);
        }
        else {
            pszPrefix = GeneratePrefixalVarName(pszPrefix0, pszVarName);
        }
    }

    // do nothing to those variables with neither [in] nor [out] attributes!
    if (pszPrefix != NULL) {
        pCtx->PutString(pszPrefix);
        delete pszPrefix;
    }
    else {
        pCtx->PutString(pszVarName);
    }
    return LUBE_OK;
}

IMPL_USERFUNC(ParamAddRef)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    ParamDescriptor *pParamDesc = (ParamDescriptor *)pvArg;
    char szExpression[128];
    char* pszPrefix = NULL;
    TypeDescriptor type, *pType = &pParamDesc->type;

    if (Type_alias == pParamDesc->type.type) {
        GetOriginalType(pCtx->m_pModule, pType, &type);
        pType = &type;
    }

    if (Type_interface == pType->type &&
        pParamDesc->dwAttribs & ParamAttrib_in &&
        1 == pType->nPointer) {

        char* pszVarName;
        char szValName[12];
        int nParam = 0;

        if (pCtx->m_pClass && pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) nParam = 1;

        for (; nParam < pCtx->m_pMethod->cParams; ++nParam) {
            if (pParamDesc == pCtx->m_pMethod->ppParams[nParam]) {
                if (pCtx->m_pClass && pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) nParam -= 1;
                break;
            }
        }
        sprintf(szValName, "Param%d", nParam);
        pszVarName = szValName;

        pszPrefix = GeneratePrefixalVarName("p", pszVarName);
        if (NULL == pszPrefix) return LUBE_FALSE;
        sprintf(szExpression, "if (m_pUserParams->m_%s) m_pUserParams->m_%s->AddRef();", pszPrefix, pszPrefix);
        pCtx->PutString(szExpression);
        delete [] pszPrefix;
    }

    return LUBE_OK;
}

IMPL_USERFUNC(ParamRelease)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    ParamDescriptor *pParamDesc = (ParamDescriptor *)pvArg;
    char szExpression[128];
    char* pszPrefix = NULL;
    TypeDescriptor type, *pType = &pParamDesc->type;

    if (Type_alias == pType->type) {
        GetOriginalType(pCtx->m_pModule, pType, &type);
        pType = &type;
    }

    if (Type_interface == pType->type &&
        pParamDesc->dwAttribs & ParamAttrib_in &&
        1 == pType->nPointer) {
        char* pszVarName;
        char szValName[12];
        int nParam = 0;

        if (pCtx->m_pClass && pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) nParam = 1;

        for (; nParam < pCtx->m_pMethod->cParams; ++nParam) {
            if (pParamDesc == pCtx->m_pMethod->ppParams[nParam]) {
                if (pCtx->m_pClass && pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) nParam -= 1;
                break;
            }
        }
        sprintf(szValName, "Param%d", nParam);
        pszVarName = szValName;

        pszPrefix = GeneratePrefixalVarName("p", pszVarName);
        if (NULL == pszPrefix) return LUBE_FALSE;
        sprintf(szExpression, "if (m_pUserParams->m_%s) m_pUserParams->m_%s->Release();", pszPrefix, pszPrefix);
        pCtx->PutString(szExpression);
        delete [] pszPrefix;
    }

    return LUBE_OK;
}

IMPL_USERFUNC(ParamOrigType)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    const char *pszType;
    char szType[32];
    TypeDescriptor type;
    TypeDescriptor *pType = (TypeDescriptor *)pvArg;

    assert(NULL != pCtx->m_pParam);
    DWORD dwAttribs = pCtx->m_pParam->dwAttribs;

    switch (pType->type) {
        case Type_ArrayOf:
            assert(pType->pNestedType);
            if (dwAttribs & ParamAttrib_in) {
                sprintf(szType, "ArrayOf<%s>",
                    Type2CString(pCtx->m_pModule, pType->pNestedType));
            }
            else {
                sprintf(szType, "ArrayOf<%s>*",
                    Type2CString(pCtx->m_pModule, pType->pNestedType));
            }
            pszType = szType;
            break;

        case Type_BufferOf:
            assert(pType->pNestedType);
            if (dwAttribs & ParamAttrib_in) {
                sprintf(szType, "BufferOf<%s>",
                    Type2CString(pCtx->m_pModule, pType->pNestedType));
            }
            else {
                sprintf(szType, "BufferOf<%s>*",
                    Type2CString(pCtx->m_pModule, pType->pNestedType));
            }
            pszType = szType;
            break;

        case Type_MemoryBuf:
            if (dwAttribs & ParamAttrib_in) strcpy(szType, "MemoryBuf");
            else strcpy(szType, "MemoryBuf*");
            pszType = szType;
            break;

        case Type_StringBuf:
            strcpy(szType, "StringBuf*");
            pszType = szType;
            break;

        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &type);
            if ((type.type == Type_EGuid ||
                type.type == Type_EMuid) &&
                1 == type.nPointer) {
                if (type.type == Type_EGuid) strcpy(szType, "EGuid");
                else if (type.type == Type_EMuid) strcpy(szType, "EMuid");
                pszType = szType;
                break;
            }
        default:
            pszType = Type2CString(pCtx->m_pModule, pType);
            break;
    }

    pCtx->PutString(pszType);

    return LUBE_OK;
}

IMPL_USERFUNC(MemberNamespaceType)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    TypeDescriptor *pType = (TypeDescriptor *)pvArg;
    const char *pszType;
    TypeDescriptor type;

    memset(&type, 0, sizeof(type));

    if (Type_Array == pType->type) {
        GetArrayBaseType(pCtx->m_pModule, pType, &type);
        pType = &type;
    }

    pszType = Type2CString(pCtx->m_pModule, pType);

    pCtx->PutString(ImplNamespaceType(pszType));
    return LUBE_OK;
}

IMPL_USERFUNC(ParamNamespaceType)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    TypeDescriptor *pType = (TypeDescriptor *)pvArg;
    const char *pszType;
    char szType[128];
    TypeDescriptor type;

    memset(&type, 0, sizeof(type));

    assert(NULL != pCtx->m_pParam);
    DWORD dwAttribs = pCtx->m_pParam->dwAttribs;

    if ((Type_struct == pType->type)
            || (Type_EMuid == pType->type)
            || (Type_EGuid == pType->type)
            || (Type_DateTime == pType->type)) {
        if (0 == pType->nPointer) {
            sprintf(szType, "const %s &",
                    Type2CString(pCtx->m_pModule, pType));
            pszType = szType;
        }
        else {
            pszType = Type2CString(pCtx->m_pModule, pType);
        }
    }
    else if (Type_alias == pType->type) {
        GetOriginalType(pCtx->m_pModule, pType, &type);
        if ((Type_EMuid == type.type)
                || (Type_EGuid == type.type)
                || (Type_DateTime == type.type)
                || (Type_struct == type.type)) {
            if (dwAttribs & ParamAttrib_in) {
                if (0 == type.nPointer) {
                    assert(0 == pType->nPointer);
                    sprintf(szType, "const %s &",
                            Type2CString(pCtx->m_pModule, pType));
                    pszType = szType;
                }
                else {
                    pszType = Type2CString(pCtx->m_pModule, pType);
                }
            }
            else pszType = Type2CString(pCtx->m_pModule, pType);
        }
        else if ((Type_ArrayOf == type.type)
                || (Type_BufferOf == type.type)
                || (Type_MemoryBuf == type.type)
                || (Type_StringBuf == type.type)) {
            if (dwAttribs & ParamAttrib_in) {
                if (0 == type.nPointer) {
                    assert(0 == pType->nPointer);
                    sprintf(szType, "const %s &",
                            Type2CString(pCtx->m_pModule, pType));
                    pszType = szType;
                }
                else {
                    pszType = Type2CString(pCtx->m_pModule, pType);
                }
            }
            else {
                if (dwAttribs & ParamAttrib_callee) {
                    if (0 == pType->nPointer) {
                        assert(1 == type.nPointer);
                        sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                    }
                    else if (1 == pType->nPointer) {
                        assert(0 == type.nPointer);
                        sprintf(szType, "%s **", Type2CString(pCtx->m_pModule, pType));
                    }
                    pszType = szType;
                }
                else {
                    assert(0 == pType->nPointer);
                    assert(0 == type.nPointer);
                    sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                    pszType = szType;
                }
            }
        }
        else pszType = Type2CString(pCtx->m_pModule, pType);
    }
    else if (Type_ArrayOf == pType->type) {
        if (dwAttribs & ParamAttrib_in) {
            if (0 == pType->nPointer) {
                sprintf(szType, "const ArrayOf<%s> &",
                        Type2CString(pCtx->m_pModule, pType->pNestedType));
            }
            else sprintf(szType, "%s", Type2CString(pCtx->m_pModule, pType));
        }
        else {
            //dwAttribs == ParamAttrib_out
            sprintf(szType, "ArrayOf<%s>",
                    Type2CString(pCtx->m_pModule, pType->pNestedType));
            if (0 == pType->nPointer) strcat(szType, " *");
            else strcat(szType, " **");
        }
        pszType = szType;
    }
    else if (Type_BufferOf == pType->type) {
        if (dwAttribs & ParamAttrib_in) {
            if (0 == pType->nPointer) {
                sprintf(szType, "const BufferOf<%s> &",
                        Type2CString(pCtx->m_pModule, pType->pNestedType));
            }
            else sprintf(szType, "%s", Type2CString(pCtx->m_pModule, pType));
        }
        else {
            //dwAttribs == ParamAttrib_out
            sprintf(szType, "BufferOf<%s>",
                    Type2CString(pCtx->m_pModule, pType->pNestedType));
            if (0 == pType->nPointer) strcat(szType, " *");
            else strcat(szType, " **");
        }
        pszType = szType;
    }
    else if (Type_MemoryBuf == pType->type) {
        if (dwAttribs & ParamAttrib_in) {
            if (0 == pType->nPointer) pszType = "const MemoryBuf &";
            else pszType = Type2CString(pCtx->m_pModule, pType);
        }
        else if ((dwAttribs & ParamAttrib_out)
                && (0 == pType->nPointer)) pszType = "MemoryBuf *";
        else pszType = "MemoryBuf **";
    }
    else if (Type_StringBuf == pType->type) {
        if (0 == pType->nPointer) pszType = "StringBuf *";
        else pszType = "StringBuf **";
    }
    else {
        pszType = Type2CString(pCtx->m_pModule, pType);
    }

    pCtx->PutString(ImplNamespaceType(pszType));
    return LUBE_OK;
}

IMPL_USERFUNC(NamespaceType)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    TypeDescriptor *pType = (TypeDescriptor *)pvArg;
    const char *pszType;
    char szType[128];
    TypeDescriptor type;

    memset(&type, 0, sizeof(type));

    switch(pType->type) {
        case Type_ArrayOf:
            strcpy(szType, "ArrayOf<");
            strcat(szType, Type2CString(pCtx->m_pModule, pType->pNestedType));
            strcat(szType, ">");
            if (1 == pType->nPointer) strcat(szType, " *");
            else if (2 == pType->nPointer) strcat(szType, " **");
            pszType = szType;
            break;

        case Type_BufferOf:
            strcpy(szType, "BufferOf<");
            strcat(szType, Type2CString(pCtx->m_pModule, pType->pNestedType));
            strcat(szType, ">");
            if (1 == pType->nPointer) strcat(szType, " *");
            else if (2 == pType->nPointer) strcat(szType, " **");
            pszType = szType;
            break;

        case Type_MemoryBuf:
            strcpy(szType, "MemoryBuf");
            if (1 == pType->nPointer) strcat(szType, " *");
            else if (2 == pType->nPointer) strcat(szType, " **");
            pszType = szType;
            break;

        case Type_StringBuf:
            strcpy(szType, "StringBuf");
            if (1 == pType->nPointer) strcat(szType, " *");
            else if (2 == pType->nPointer) strcat(szType, " **");
            pszType = szType;
            break;

        default:
            pszType = Type2CString(pCtx->m_pModule, pType);
            break;
    }

    pCtx->PutString(ImplNamespaceType(pszType));

    return LUBE_OK;
}

IMPL_USERFUNC(ParamType2String)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    char szOutput[512] = {0};

    MethodDescriptor *pMethod = (MethodDescriptor *)pvArg;

    strcpy(szOutput, "ParamType_");

    int i = 0;
    if (pCtx->m_pClass->pDesc->dwAttribs & ClassAttrib_t_sink) i = 1;
    else i = 0;

    for (; i < pMethod->cParams; ++i) {
        TypeDescriptor *pType = &pMethod->ppParams[i]->type;
        GenerateTypeStringForParam(pCtx->m_pModule, pType, szOutput);
        strcat(szOutput, "_");
    }

    pCtx->PutString(szOutput);

    return LUBE_OK;
}

IMPL_USERFUNC(CopyOutParam)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    TypeDescriptor type;
    TypeDescriptor *pType = &((ParamDescriptor *)pvArg)->type;

Begin:
    switch(pType->type) {
        case Type_ArrayOf:
        case Type_BufferOf:
        case Type_MemoryBuf:
        case Type_StringBuf:
            pCtx->PutString("_BufferOf_Copy((PCarQuintet)");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(", (PCarQuintet)_pCallbackEvent_->m_pUserParams->m_");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;
        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &type);
            pType = &type;
            goto Begin;
        default:
            pCtx->PutString("*");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(" = *_pCallbackEvent_->m_pUserParams->m_");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            break;
    }

    return LUBE_OK;
}

/* ������aidl2car�����õ���USERFUNC������Ӧ���Ƽ�ʹ��UTF8�ַ���������
 * �����У�switch case����漰String��StringBuf�ķ�֧������ִ�У�������
 * Щ��֧�ڱ��ļ�����ʱ����
 */

char* AndroidType2CString(TypeDescriptor* pTypeDesc)
{
    switch(pTypeDesc->type) {
        case Type_Boolean:
            return "int";
        case Type_Int32:
            return "int";
        case Type_Int64:
            return "jlong";
        case Type_Byte:
            return "int";
        case Type_Double:
            return "double";
        case Type_Float:
            return "float";

        default:
            return "default";
    }
}

char* CAR2AndriodRWType(TypeDescriptor* pTypeDesc)
{
    switch(pTypeDesc->type) {
        case Type_Boolean:
        case Type_Int32:
        case Type_Byte:
            return "Int32";
        case Type_Int64:
            return "Int64";
        case Type_Double:
            return "Double";
        case Type_Float:
            return "Float";

        default:
            return "default";
    }
}

IMPL_USERFUNC(AndriodType)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    const char *pszType;
    char szType[32];
    TypeDescriptor type;
    TypeDescriptor *pType = (TypeDescriptor *)pvArg;

     switch (pType->type) {
        case Type_CString:
        case Type_String:
            assert(0 == pType->nPointer);
            sprintf(szType, "const char*");
            pszType = szType;
            break;
        case Type_Boolean:
            if (0 == pType->nPointer) sprintf(szType, "int");
            else if (1 == pType->nPointer) sprintf(szType, "int *");
            pszType = szType;
            break;
        case Type_Byte:
            if (0 == pType->nPointer) sprintf(szType, "int");
            else if (1 == pType->nPointer) sprintf(szType, "int *");
            pszType = szType;
            break;
        case Type_Int32:
            if (0 == pType->nPointer) sprintf(szType, "int");
            else if (1 == pType->nPointer) sprintf(szType, "int *");
            pszType = szType;
            break;
        case Type_Int64:
            if (0 == pType->nPointer) sprintf(szType, "jlong");
            else if (1 == pType->nPointer) sprintf(szType, "jlong *");
            pszType = szType;
            break;
        case Type_Double:
            if (0 == pType->nPointer) sprintf(szType, "bouble");
            else if (1 == pType->nPointer) sprintf(szType, "bouble *");
            pszType = szType;
            break;
        case Type_Float:
            if (0 == pType->nPointer) sprintf(szType, "float");
            else if (1 == pType->nPointer) sprintf(szType, "float *");
            pszType = szType;
            break;
        case Type_ArrayOf:
            assert(pType->pNestedType);
            sprintf(szType, "ArrayOf<%s>",
                Type2CString(pCtx->m_pModule, pType->pNestedType));
            pszType = szType;
            break;
        case Type_BufferOf:
            assert(pType->pNestedType);
            sprintf(szType, "BufferOf<%s>",
                Type2CString(pCtx->m_pModule, pType->pNestedType));
            pszType = szType;
            break;
        case Type_MemoryBuf:
            strcpy(szType, "MemoryBuf*");
            pszType = szType;
            break;
        case Type_StringBuf:
            strcpy(szType, "StringBuf*");
            pszType = szType;
            break;
        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &type);
            if ((type.type == Type_EGuid ||
                type.type == Type_EMuid) &&
                1 == type.nPointer) {
                if (type.type == Type_EGuid) strcpy(szType, "EGuid");
                else if (type.type == Type_EMuid) strcpy(szType, "EMuid");
                pszType = szType;
                break;
            }
        default:
            pszType = Type2CString(pCtx->m_pModule, pType);
            break;
    }

    pCtx->PutString(pszType);

    return LUBE_OK;
}

IMPL_USERFUNC(DelcareVariablesForSoMethod)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    assert(pvArg == pCtx->m_pMethod);

    int pn;
    bool rtFlag = false;
    ParamDescriptor* pParamDesc;

    const char *pszType;
    char szType[256];

    pCtx->PutString("    Parcel data, reply;\n");

    pn = pCtx->m_pMethod->cParams;
    for(int i = 0; i < pn; i++) {
        pParamDesc = pCtx->m_pMethod->ppParams[i];
        if (ParamAttrib_out & pParamDesc->dwAttribs) {
            switch (pParamDesc->type.type) {
                case Type_StringBuf:
                    sprintf(szType, "    size_t %sStrlen;\n"
                                    "    const char16_t* %sStr16;\n",
                                    pParamDesc->pszName,
                                    pParamDesc->pszName);
                    pszType = szType;
                    pCtx->PutString(pszType);
                    break;

                default:
                    if (!rtFlag){
                        sprintf(szType, "    int rt;\n");
                        pszType = szType;
                        pCtx->PutString(pszType);
                        rtFlag = true;
                    }
                    break;
            }
        }
    }

    return LUBE_OK;
}

IMPL_USERFUNC(WriteToAndriodParcel)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pParam);

    const char *pszType;
    char szType[256];

    switch (pCtx->m_pParam->type.type) {
        case Type_CString:
        case Type_String:
            sprintf(szType, "data.writeString16(String16(%s));\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;

        case Type_Boolean:
            sprintf(szType, "data.writeInt32(%s);\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;
        case Type_Int64:
            sprintf(szType, "data.writeInt64(%s);\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;
        case Type_Int32:
            sprintf(szType, "data.writeInt32(%s);\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;
        case Type_Double:
            sprintf(szType, "data.writeDouble(%s);\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;
        case Type_Float:
            sprintf(szType, "data.writeFloat(%s);\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;
        case Type_Byte:
            sprintf(szType, "data.writeInt32(%s);\n", pCtx->m_pParam->pszName);
            pszType = szType;
            break;

        case Type_MemoryBuf:
            sprintf(szType, "data.writeInt32(%sCount);\n"
                            "    for (int i = 0; i < %sCount; i++) data.writeInt32(%s[i]);\n",
                            pCtx->m_pParam->pszName,
                            pCtx->m_pParam->pszName,
                            pCtx->m_pParam->pszName);
            pszType = szType;
            break;
        case Type_BufferOf:
        case Type_ArrayOf:
            sprintf(szType, "data.writeInt32(%sCount);\n"
                            "    for (int i = 0; i < %sCount; i++) data.write%s(%s[i]);\n",
                            pCtx->m_pParam->pszName,
                            pCtx->m_pParam->pszName,
                            CAR2AndriodRWType(pCtx->m_pParam->type.pNestedType),
                            pCtx->m_pParam->pszName);
            pszType = szType;
            break;

        default:
            sprintf(szType, "default\n");
            pszType = szType;
            break;
    }

    pCtx->PutString(pszType);

    return LUBE_OK;
}

void ReadFromParcel(PLUBECTX pCtx, ParamDescriptor* pParamDesc)
{
    const char *pszType = NULL;
    char szType[512];

    switch (pParamDesc->type.type) {
        case Type_CString:
        case Type_String:
            assert(0);
            break;
        case Type_StringBuf:
            sprintf(szType,
                    "    %sStr16 = reply.readString16Inplace(&%sStrlen);\n"
                    "    if (%sStr16 != NULL && %sStr16[0] != '\\0'){\n"
                    "        *%s = strndup16to8(%sStr16, %sStrlen);\n"
                    "    } \n"
                    "    else {\n"
                    "        *%s = NULL;\n"
                    "    }\n",
                    pParamDesc->pszName,pParamDesc->pszName,
                    pParamDesc->pszName,pParamDesc->pszName,
                    pParamDesc->pszName,pParamDesc->pszName,
                    pParamDesc->pszName,pParamDesc->pszName);
            pszType = szType;
            break;

        case Type_Boolean:
        case Type_Int32:
            assert(1 == pParamDesc->type.nPointer);
            sprintf(szType, "    rt = reply.readInt32(%s);\n"
                            "    if (rt != 0) return E_FAIL;\n\n",
                    pParamDesc->pszName);
            pszType = szType;
            break;
        case Type_Int64:
            assert(1 == pParamDesc->type.nPointer);
            sprintf(szType, "    rt = reply.readInt64(%s);\n"
                            "    if (rt != 0) return E_FAIL;\n\n",
                    pParamDesc->pszName);
            pszType = szType;
            break;
        case Type_Byte:
            assert(1 == pParamDesc->type.nPointer);
            sprintf(szType, "    rt = reply.readInt32(%s);\n"
                            "    if (rt != 0) return E_FAIL;\n\n",
                    pParamDesc->pszName);
            pszType = szType;
            break;
        case Type_Double:
            assert(1 == pParamDesc->type.nPointer);
            sprintf(szType, "    rt = reply.readDouble(%s);\n"
                            "    if (rt != 0) return E_FAIL;\n\n",
                    pParamDesc->pszName);
            pszType = szType;
            break;
        case Type_Float:
            assert(1 == pParamDesc->type.nPointer);
            sprintf(szType, "    rt = reply.readFloat(%s);\n"
                            "    if (rt != 0) return E_FAIL;\n\n",
                    pParamDesc->pszName);
            pszType = szType;
            break;

        case Type_MemoryBuf:
            sprintf(szType, "    rt = reply.readInt32(%sCount);\n"
                            "    if (rt != 0) {\n"
                            "        return E_FAIL;\n"
                            "    } else {\n"
                            "        *%s = (int*)malloc((*%sCount)*sizeof(int));\n"
                            "        if (*%s)\n"
                            "            for (int i = 0; i < *%sCount; i++)\n"
                            "                (*%s)[i] = reply.readInt32();\n"
                            "    }\n\n",
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            pParamDesc->pszName);
            pszType = szType;
            break;
        case Type_BufferOf:
        case Type_ArrayOf:
            sprintf(szType, "    rt = reply.readInt32(%sCount);\n"
                            "    if (rt != 0) {\n"
                            "        return E_FAIL;\n"
                            "    } else {\n"
                            "        *%s = (%s*)malloc((*%sCount)*sizeof(%s));\n"
                            "        if (*%s)\n"
                            "            for (int i = 0; i < *%sCount; i++)\n"
                            "                (*%s)[i] = (%s)reply.read%s();\n"
                            "    }\n\n",
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            AndroidType2CString(pParamDesc->type.pNestedType),
                            pParamDesc->pszName,
                            AndroidType2CString(pParamDesc->type.pNestedType),
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            pParamDesc->pszName,
                            AndroidType2CString(pParamDesc->type.pNestedType),
                            CAR2AndriodRWType(pParamDesc->type.pNestedType));
            pszType = szType;
            break;

        default:
            sprintf(szType,
                    "           default\n");
            pszType = szType;
            break;
    }

    pCtx->PutString(pszType);

}

IMPL_USERFUNC(ReadOutParamFromParcel)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pMethod);
    assert(pvArg == pCtx->m_pMethod);

    char szType[512];
    bool isFirstOutParam = true;

    for (int i = 0; i < pCtx->m_pMethod->cParams; i++) {
        if (ParamAttrib_out & pCtx->m_pMethod->ppParams[i]->dwAttribs) {
            if (isFirstOutParam) {
                sprintf(szType, "    //readException\n"
                                "    int exceptionCode = reply.readInt32();\n"
                                "    if (0 != exceptionCode) {\n"
                                "        ReportException(TRANSACTION_%s, reply);\n"
                                "        return E_FAIL;\n"
                                "    }\n\n",
                                pCtx->m_pMethod->pszName);
                pCtx->PutString(szType);

                isFirstOutParam = false;
            }
            ReadFromParcel(pCtx, pCtx->m_pMethod->ppParams[i]);

        }
    }

    return LUBE_OK;
}

IMPL_USERFUNC(MethodIndexInInterface)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    assert(pvArg == pCtx->m_pMethod);

    char szType[5];

    InterfaceDescriptor* pInterfaceDesc = pCtx->m_pModule->ppInterfaceDir[pCtx->m_pClsIntf->sIndex]->pDesc;
    int mn = pInterfaceDesc->cMethods;
    for (int i = 0; i < mn; i++) {
        if (!strcmp(pCtx->m_pMethod->pszName, pInterfaceDesc->ppMethods[i]->pszName)) {
            sprintf(szType, "%d", i + 1);
            pCtx->PutString(szType);
        }
    }

    return LUBE_OK;
}

IMPL_USERFUNC(CallSoMethod)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    assert(pvArg == pCtx->m_pMethod);

    char szBuffer[256];
    char szTemp[256];

    int pn;
    ParamDescriptor* pParamDesc;

    pn = pCtx->m_pMethod->cParams;
    for(int i = 0; i < pn; i++) {
        pParamDesc = pCtx->m_pMethod->ppParams[i];

        switch (pParamDesc->type.type) {
            case Type_CString:
            case Type_String:
                sprintf(szBuffer, "    const char* p%sTmp = (const char*)%s;\n",
                        pParamDesc->pszName, pParamDesc->pszName);
                pCtx->PutString(szBuffer);
                break;
            case Type_Byte:
            case Type_Boolean:
                if (ParamAttrib_out & pParamDesc->dwAttribs){
                    sprintf(szBuffer, "    Int32 retValue;\n");
                    pCtx->PutString(szBuffer);
                }
                break;

            case Type_MemoryBuf:
                if (ParamAttrib_in & pParamDesc->dwAttribs) {
                    sprintf(szBuffer, "    Byte* %sBuf = %s.GetPayload();\n"
                                  "    int %sCount = %s.GetUsed();\n"
                                  "    int* tmpBuf;\n\n"
                                  "    tmpBuf = (int*)malloc(%sCount*sizeof(int));\n\n"
                                  "    for (int i=0;i<%sCount;i++)\n"
                                  "        tmpBuf[i] = %sBuf[i];\n",
                        pParamDesc->pszName, pParamDesc->pszName,
                        pParamDesc->pszName, pParamDesc->pszName,
                        pParamDesc->pszName, pParamDesc->pszName,
                        pParamDesc->pszName);
                }
                else if (ParamAttrib_out & pParamDesc->dwAttribs) {
                    sprintf(szBuffer, "    int %sCount;\n"
                                      "    int* %sBuf;\n",
                            pParamDesc->pszName, pParamDesc->pszName);
                  }
                pCtx->PutString(szBuffer);
                break;
            case Type_BufferOf:
                if (ParamAttrib_in & pParamDesc->dwAttribs) {
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type) {
                        sprintf(szBuffer, "    int %sCount = %s.GetUsed();\n"
                                      "    %s* %sBuf = %s.GetPayload();\n"
                                      "    int* tmpBuf;\n\n"
                                      "    tmpBuf = (int*)malloc(%sCount*sizeof(int));\n\n"
                                      "    for (int i=0;i<%sCount;i++)\n"
                                      "        tmpBuf[i] = %sBuf[i];\n",
                            pParamDesc->pszName, pParamDesc->pszName,
                            Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                            pParamDesc->pszName, pParamDesc->pszName,
                            pParamDesc->pszName, pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                    else {
                        sprintf(szBuffer, "    int %sCount = %s.GetUsed();\n"
                                      "    %s* %sBuf = %s.GetPayload();\n",
                            pParamDesc->pszName, pParamDesc->pszName,
                            Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                            pParamDesc->pszName, pParamDesc->pszName);
                    }
                }
                else if (ParamAttrib_out & pParamDesc->dwAttribs) {
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type) {
                        sprintf(szBuffer, "    int %sCount;\n"
                                      "    int* %sBuf;\n",
                            pParamDesc->pszName, pParamDesc->pszName);
                    }
                    else {
                        sprintf(szBuffer, "    int %sCount;\n"
                                      "    %s* %sBuf;\n",
                            pParamDesc->pszName,
                            AndroidType2CString(pParamDesc->type.pNestedType),
                            pParamDesc->pszName);
                    }
                  }
                pCtx->PutString(szBuffer);
                break;
            case Type_ArrayOf:
                if (ParamAttrib_in & pParamDesc->dwAttribs) {
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type) {
                        sprintf(szBuffer, "    int %sCount = %s.GetLength();\n"
                                      "    %s* %sBuf = %s.GetPayload();\n"
                                      "    int* tmpBuf;\n\n"
                                      "    tmpBuf = (int*)malloc(%sCount*sizeof(int));\n\n"
                                      "    for (int i=0;i<%sCount;i++)\n"
                                      "        tmpBuf[i] = %sBuf[i];\n",
                            pParamDesc->pszName, pParamDesc->pszName,
                            Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                            pParamDesc->pszName, pParamDesc->pszName,
                            pParamDesc->pszName, pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                    else {
                        sprintf(szBuffer, "    int %sCount = %s.GetLength();\n"
                                      "    %s* %sBuf = %s.GetPayload();\n",
                            pParamDesc->pszName, pParamDesc->pszName,
                            Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                            pParamDesc->pszName, pParamDesc->pszName);
                    }
                }
                else if (ParamAttrib_out & pParamDesc->dwAttribs) {
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type) {
                        sprintf(szBuffer, "    int %sCount;\n"
                                      "    int* %sBuf;\n",
                            pParamDesc->pszName, pParamDesc->pszName);
                    }
                    else {
                        sprintf(szBuffer, "    int %sCount;\n"
                                      "    %s* %sBuf;\n",
                            pParamDesc->pszName,
                            AndroidType2CString(pParamDesc->type.pNestedType),
                            pParamDesc->pszName);
                    }
                  }
                pCtx->PutString(szBuffer);
                break;
            case Type_StringBuf:
                sprintf(szBuffer, "    const char* p%sTmp = NULL;\n",
                        pParamDesc->pszName);
                pCtx->PutString(szBuffer);
                break;
            default:
                break;
        }
    }

    sprintf(szBuffer, "\n    ec = %sSo%s(", pCtx->m_pClass->pszName, pCtx->m_pMethod->pszName);
    pCtx->PutString(szBuffer);
    for(int i = 0; i < pn; i++) {
        pParamDesc = pCtx->m_pMethod->ppParams[i];

        switch (pParamDesc->type.type) {
            case Type_CString:
            case Type_String:
                assert(0 == pParamDesc->type.nPointer);
                sprintf(szBuffer, "p%sTmp", pParamDesc->pszName);
                pCtx->PutString(szBuffer);
                break;

            case Type_Boolean:
            case Type_Byte:
                if (0 == pParamDesc->type.nPointer) {
                    sprintf(szBuffer, "%s", pParamDesc->pszName);
                }
                else {
                    sprintf(szBuffer, "&retValue");
                }
                pCtx->PutString(szBuffer);
                break;

            case Type_Int32:
                if (0 == pParamDesc->type.nPointer) {
                    sprintf(szBuffer, "%s", pParamDesc->pszName);
                }
                else {
                    sprintf(szBuffer, "p%s", pParamDesc->pszName);
                    szBuffer[1] -= 32;
                }
                pCtx->PutString(szBuffer);
                break;
            case Type_Int64:
                if (0 == pParamDesc->type.nPointer) {
                    sprintf(szBuffer, "(jlong)%s", pParamDesc->pszName);
                }
                else {
                    sprintf(szBuffer, "(jlong*)p%s", pParamDesc->pszName);
                    szBuffer[9] -= 32;
                }
                pCtx->PutString(szBuffer);
                break;

            case Type_MemoryBuf:
                if (0 == pParamDesc->type.nPointer) {
                    sprintf(szBuffer, "%sCount, tmpBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                }
                else {
                    sprintf(szBuffer, "&%sCount, (int**)&%sBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                pCtx->PutString(szBuffer);
                break;
            case Type_BufferOf:
            case Type_ArrayOf:
                if (0 == pParamDesc->type.nPointer) {
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer, "%sCount, tmpBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                    else if (Type_Int64 == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer, "%sCount, (jlong*)%sBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                    else {
                        sprintf(szBuffer, "%sCount, %sBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                }
                else {
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer, "&%sCount, (int**)&%sBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                    else if (Type_Int64 == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer, "&%sCount, (jlong**)&%sBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                    else {
                        sprintf(szBuffer, "&%sCount, &%sBuf",
                            pParamDesc->pszName,
                            pParamDesc->pszName);
                    }
                }
                pCtx->PutString(szBuffer);
                break;
            case Type_StringBuf:
                sprintf(szBuffer, "&p%sTmp", pParamDesc->pszName);
                pCtx->PutString(szBuffer);
                break;
            default:
                if (0 == pParamDesc->type.nPointer) {
                    sprintf(szBuffer, "%s", pParamDesc->pszName);
                }
                else {
                    sprintf(szBuffer, "p%s", pParamDesc->pszName);
                    szBuffer[1] -= 32;
                }
                pCtx->PutString(szBuffer);
                break;

        }

        if (i < pn - 1) pCtx->PutString(", ");
    }
    pCtx->PutString(");\n");

    for(int i = 0; i < pn; i++) {
        pParamDesc = pCtx->m_pMethod->ppParams[i];

        if (ParamAttrib_out & pParamDesc->dwAttribs) {
            switch (pParamDesc->type.type) {
                case Type_MemoryBuf:
                    strcpy(szTemp, pParamDesc->pszName);
                    szTemp[0] -= 32;
                    sprintf(szBuffer,"\n    if (%sBuf) {\n"
                                     "       *pp%s = MemoryBuf::Alloc(%sCount);\n"
                                     "        if (*pp%s)\n"
                                     "            for (int i=0;i<%sCount;i++)\n",
                            pParamDesc->pszName,
                            szTemp, pParamDesc->pszName,
                            szTemp, pParamDesc->pszName);
                    pCtx->PutString(szBuffer);

                    sprintf(szBuffer,"                (**pp%s)[i] = (Int8)(%sBuf[i] & 0xff);\n"
                                     "        free(%sBuf);\n"
                                     "    }\n"
                                     "    else\n"
                                     "        *pp%s = NULL;",
                            szTemp, pParamDesc->pszName,
                            pParamDesc->pszName, szTemp);
                    pCtx->PutString(szBuffer);
                    break;
                case Type_BufferOf:
                    strcpy(szTemp, pParamDesc->pszName);
                    szTemp[0] -= 32;
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer,"\n    if (%sBuf) {\n"
                                         "        *pp%s = BufferOf<%s>::Alloc(%sCount);\n"
                                         "        if (*pp%s)\n"
                                         "            for (int i=0;i<%sCount;i++)\n",
                            pParamDesc->pszName,
                            szTemp,
                            Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                            pParamDesc->pszName,
                            szTemp, pParamDesc->pszName);
                        pCtx->PutString(szBuffer);

                        sprintf(szBuffer,"                (**pp%s)[i] = (Int8)(%sBuf[i] & 0xff);\n"
                                         "        free(%sBuf);\n"
                                         "    }\n"
                                         "    else\n"
                                         "        *pp%s = NULL;\n",
                            szTemp, pParamDesc->pszName,
                            pParamDesc->pszName, szTemp);
                        pCtx->PutString(szBuffer);
                    }
                    else{
                        sprintf(szBuffer, "\n    if (%sBuf) {\n"
                                          "        *pp%s = BufferOf<%s>::Alloc(%sCount);\n"
                                          "        if (*pp%s) (*pp%s)->Copy(%sBuf, %sCount);\n"
                                          "        free(%sBuf);\n"
                                          "    }\n"
                                          "    else\n"
                                          "        *pp%s = NULL;\n",
                                pParamDesc->pszName,
                                szTemp, Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                                pParamDesc->pszName, szTemp, szTemp,
                                pParamDesc->pszName, pParamDesc->pszName,
                                pParamDesc->pszName, szTemp);
                        pCtx->PutString(szBuffer);
                    }
                    break;
                case Type_ArrayOf:
                    strcpy(szTemp, pParamDesc->pszName);
                    szTemp[0] -= 32;
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                        Type_Boolean == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer,"\n    if (%sBuf) {\n"
                                         "        *pp%s = ArrayOf<%s>::Alloc(%sCount);\n"
                                         "        if (*pp%s)\n"
                                         "            for (int i=0;i<%sCount;i++)\n",
                            pParamDesc->pszName,
                            szTemp,
                            Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                            pParamDesc->pszName,
                            szTemp, pParamDesc->pszName);
                        pCtx->PutString(szBuffer);

                        sprintf(szBuffer,"                (**pp%s)[i] = (Int8)(%sBuf[i] & 0xff);\n"
                                         "        free(%sBuf);\n"
                                         "    }\n"
                                         "    else\n"
                                         "        *pp%s = NULL;\n",
                            szTemp, pParamDesc->pszName,
                            pParamDesc->pszName, szTemp);
                        pCtx->PutString(szBuffer);
                    }
                    else{
                        sprintf(szBuffer, "\n    if (%sBuf) {\n"
                                          "        *pp%s = ArrayOf<%s>::Alloc(%sCount);\n"
                                          "        if (*pp%s) (*pp%s)->Copy(%sBuf, %sCount);\n"
                                          "        free(%sBuf);\n"
                                          "    }\n"
                                          "    else\n"
                                          "        *pp%s = NULL;\n",
                                pParamDesc->pszName,
                                szTemp, Type2CString(pCtx->m_pModule, pParamDesc->type.pNestedType),
                                pParamDesc->pszName, szTemp, szTemp,
                                pParamDesc->pszName, pParamDesc->pszName,
                                pParamDesc->pszName, szTemp);
                        pCtx->PutString(szBuffer);
                    }
                    break;
                case Type_StringBuf:
                    strcpy(szTemp, pParamDesc->pszName);
                    szTemp[0] -= 32;

                    sprintf(szBuffer, "\n    if (p%sTmp) {\n",
                            pParamDesc->pszName);
                    pCtx->PutString(szBuffer);

                    sprintf(szBuffer,
                                "        *pp%s = StringBuf::Alloc(strlen(p%sTmp)+1);\n",
                                szTemp, pParamDesc->pszName);
                    pCtx->PutString(szBuffer);

                    sprintf(szBuffer,
                                "        if (*pp%s) (*pp%s)->Copy(p%sTmp);\n"
                                "        free((char *)p%sTmp);\n"
                                "    }\n"
                                "    else\n",
                            szTemp, szTemp,
                            pParamDesc->pszName, pParamDesc->pszName);
                    pCtx->PutString(szBuffer);

                    sprintf(szBuffer,
                                "        *pp%s = StringBuf::Alloc(0);\n",
                            szTemp);
                    pCtx->PutString(szBuffer);
                    break;
                case Type_Boolean:
                    sprintf(szBuffer, "\n    *p%s = (Boolean)retValue;\n",
                            pParamDesc->pszName);
                    szBuffer[7] -= 32;
                    pCtx->PutString(szBuffer);
                    break;
                case Type_Byte:
                    sprintf(szBuffer, "\n    *p%s = (Byte)(retValue & 0xff);\n",
                            pParamDesc->pszName);
                    szBuffer[7] -= 32;
                    pCtx->PutString(szBuffer);
                    break;

                default:
                    break;
            }
        }
        else if (ParamAttrib_in & pParamDesc->dwAttribs){
            switch (pParamDesc->type.type) {
                case Type_MemoryBuf:
                case Type_BufferOf:
                case Type_ArrayOf:
                    if (Type_Byte == pParamDesc->type.pNestedType->type ||
                            Type_Boolean == pParamDesc->type.pNestedType->type){
                        sprintf(szBuffer, "\n    free(tmpBuf);\n");
                        pCtx->PutString(szBuffer);
                    }
                    break;
                default:
                    break;
            }
        }
    }

    return LUBE_OK;
}

IMPL_USERFUNC(GenerateAndriodParameter)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pParam);
    assert(pvArg == pCtx->m_pParam);

    char szBuffer[256];
    ParamDescriptor* pParamDesc = pCtx->m_pParam;

    switch(pParamDesc->type.type) {
        case Type_CString:
            assert(0 == pParamDesc->type.nPointer);
            sprintf(szBuffer, "const char* %s", pParamDesc->pszName);
            break;

        case Type_String:
            assert(0 == pParamDesc->type.nPointer);
            sprintf(szBuffer, "const char* %s", pParamDesc->pszName);
            break;

        case Type_Boolean:
            if (0 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "int %s", pParamDesc->pszName);
            }
            else if (1 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "int* %s", pParamDesc->pszName);
            }
            break;

        case Type_Int32:
            if (0 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "int %s", pParamDesc->pszName);
            }
            else if (1 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "int* %s", pParamDesc->pszName);
            }
            break;
        case Type_Int64:
            if (0 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "jlong %s", pParamDesc->pszName);
            }
            else if (1 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "jlong* %s", pParamDesc->pszName);
            }
            break;
        case Type_Double:
            if (0 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "double %s", pParamDesc->pszName);
            }
            else if (1 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "double* %s", pParamDesc->pszName);
            }
            break;
        case Type_Float:
            if (0 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "float %s", pParamDesc->pszName);
            }
            else if (1 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "float* %s", pParamDesc->pszName);
            }
            break;
        case Type_Byte:
            if (0 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "int %s", pParamDesc->pszName);
            }
            else if (1 == pParamDesc->type.nPointer) {
                sprintf(szBuffer, "int* %s", pParamDesc->pszName);
            }
            break;

        case Type_MemoryBuf:
            if (ParamAttrib_in & pParamDesc->dwAttribs) {
                sprintf(szBuffer, "int %sCount, int* %s",
                        pParamDesc->pszName,
                        pParamDesc->pszName);
            }
            else if (ParamAttrib_out & pParamDesc->dwAttribs) {
                sprintf(szBuffer, "int* %sCount, int** %s",
                        pParamDesc->pszName,
                        pParamDesc->pszName);
            }
            break;
        case Type_BufferOf:
        case Type_ArrayOf:
            if (ParamAttrib_in & pParamDesc->dwAttribs) {
                assert(pParamDesc->type.pNestedType);
                sprintf(szBuffer, "int %sCount, %s* %s",
                        pParamDesc->pszName,
                        AndroidType2CString(pParamDesc->type.pNestedType),
                        pParamDesc->pszName);
            }
            else if (ParamAttrib_out & pParamDesc->dwAttribs) {
                assert(pParamDesc->type.pNestedType);
                sprintf(szBuffer, "int* %sCount, %s** %s",
                        pParamDesc->pszName,
                        AndroidType2CString(pParamDesc->type.pNestedType),
                        pParamDesc->pszName);
            }
            break;

        case Type_StringBuf:
            sprintf(szBuffer, "const char** %s", pParamDesc->pszName);
            break;

        default:
            sprintf(szBuffer, "default");
            break;
    }

    pCtx->PutString(szBuffer);

    return LUBE_OK;
}

IMPL_USERFUNC(GenerateCBDataParameter)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pParam);
    assert(pvArg == pCtx->m_pParam);

    char szBuffer[256];
    ParamDescriptor* pParamDesc = pCtx->m_pParam;

    switch(pParamDesc->type.type) {
        case Type_CString:
            assert(0 == pParamDesc->type.nPointer);
            sprintf(szBuffer, "char* %s", pParamDesc->pszName);
            break;
        case Type_String:
            assert(0 == pParamDesc->type.nPointer);
            sprintf(szBuffer, "char* %s", pParamDesc->pszName);
            break;
        case Type_Boolean:
            sprintf(szBuffer, "int %s", pParamDesc->pszName);
        case Type_Int32:
            sprintf(szBuffer, "int %s", pParamDesc->pszName);
            break;
        case Type_Int64:
            sprintf(szBuffer, "jlong %s", pParamDesc->pszName);
            break;
        case Type_Double:
            sprintf(szBuffer, "double %s", pParamDesc->pszName);
            break;
        case Type_Float:
            sprintf(szBuffer, "float %s", pParamDesc->pszName);
            break;
        case Type_Byte:
            sprintf(szBuffer, "int %s", pParamDesc->pszName);
            break;

        default:
            sprintf(szBuffer, "default");
            break;
    }

    pCtx->PutString(szBuffer);

    return LUBE_OK;
}

IMPL_USERFUNC(GenerateAndriodArgument)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pParam);
    assert(pvArg == pCtx->m_pParam);

    char szBuffer[256];
    ParamDescriptor* pParamDesc = pCtx->m_pParam;

    switch(pParamDesc->type.type) {
        case Type_MemoryBuf:
        case Type_BufferOf:
        case Type_ArrayOf:
            assert(pParamDesc->type.pNestedType);
            sprintf(szBuffer, "%sCount, %s",
                    pParamDesc->pszName,
                    pParamDesc->pszName);
            break;

        default:
            sprintf(szBuffer, "%s", pParamDesc->pszName);
            break;
    }

    pCtx->PutString(szBuffer);

    return LUBE_OK;
}

IMPL_USERFUNC(ReadFromAndriodParcelOnTransact)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    assert(pvArg == pCtx->m_pMethod);

    char szBuffer[512];

    int pn;
    ParamDescriptor* pParamDesc;

    pn = pCtx->m_pMethod->cParams;
    for(int i = 0; i < pn; i++) {
        pParamDesc = pCtx->m_pMethod->ppParams[i];

        switch (pParamDesc->type.type) {
            case Type_CString:
                sprintf(szBuffer,
                    "            str16 = data.readString16Inplace(&strlen);\n"
                    "            if (str16 != NULL && str16[0] != '\\0'){\n"
                    "                m_p%sData->%s = strndup16to8(str16, strlen);\n"
                    "            } \n"
                    "            else {\n"
                    "                m_p%sData->%s = NULL;\n"
                    "            }\n",
                    pCtx->m_pMethod->pszName, pParamDesc->pszName,
                    pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            case Type_String:
                sprintf(szBuffer,
                    "            str16 = data.readString16Inplace(&strlen);\n"
                    "            if (str16 != NULL && str16[0] != '\\0'){\n"
                    "                m_p%sData->%s = strndup16to8(str16, strlen);\n"
                    "            } \n"
                    "            else {\n"
                    "                m_p%sData->%s = NULL;\n"
                    "            }\n",
                    pCtx->m_pMethod->pszName, pParamDesc->pszName,
                    pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            case Type_Boolean:
            case Type_Int32:
                sprintf(szBuffer, "            m_p%sData->%s = data.readInt32();\n\n",
                        pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            case Type_Int64:
                sprintf(szBuffer, "            m_p%sData->%s = data.readInt64();\n\n",
                        pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            case Type_Double:
                sprintf(szBuffer, "            m_p%sData->%s = data.readDouble();\n\n",
                        pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            case Type_Float:
                sprintf(szBuffer, "            m_p%sData->%s = data.readFloat();\n\n",
                        pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            case Type_Byte:
                sprintf(szBuffer, "            m_p%sData->%s = data.readInt32();\n\n",
                        pCtx->m_pMethod->pszName, pParamDesc->pszName);
                break;
            default:
                sprintf(szBuffer, "default");
                break;
        }
        pCtx->PutString(szBuffer);
    }
    return LUBE_OK;
}

IMPL_USERFUNC(PrepareParamForResponseCallback)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pParam);
    assert(pvArg == pCtx->m_pParam);

    char szBuffer[256];
    ParamDescriptor* pParamDesc = pCtx->m_pParam;

    switch (pParamDesc->type.type) {
        case Type_Boolean:
            sprintf(szBuffer, "    Boolean %s = (Boolean)_%s;\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_Byte:
            sprintf(szBuffer, "    Byte %s = (Byte)(_%s & 0xff);\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_CString:
            sprintf(szBuffer, "    CString %s(_%s);\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_String:
            sprintf(szBuffer, "    String %s(_%s);\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_Int32:
            sprintf(szBuffer, "    Int32 %s = _%s;\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_Int64:
            sprintf(szBuffer, "    Int64 %s = _%s;\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_Double:
            sprintf(szBuffer, "    Double %s = _%s;\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        case Type_Float:
            sprintf(szBuffer, "    Float %s = _%s;\n",
                    pParamDesc->pszName, pParamDesc->pszName);
            pCtx->PutString(szBuffer);
            break;
        default:
            break;
    }

    return LUBE_OK;
}

IMPL_USERFUNC(IsFiltered)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClsIntf && pvArg == pCtx->m_pClsIntf);
    assert(NULL != pCtx->m_pOrigClass);

    ClassInterface *pClsIntf = pCtx->m_pClsIntf;
    ClassDescriptor *pClsDesc = pCtx->m_pOrigClass->pDesc;

    for(int i = 0; i < pClsDesc->cInterfaces; i++) {
        if ((pClsDesc->ppInterfaces[i]->sIndex == pClsIntf->sIndex)
                || (pClsDesc->ppInterfaces[i]->wAttribs & ClassInterfaceAttrib_filter)) {
            return true;
        }
    }

    return false;
}

IMPL_USERFUNC(HasTrivialConstructor)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClass && pvArg == pCtx->m_pClass);

    ClassDescriptor *pClsDesc = pCtx->m_pClass->pDesc;
    assert(pClsDesc->dwAttribs & ClassAttrib_t_clsobj);

    ClassInterface *pClsIntf;
    InterfaceDescriptor *pIntfDesc;

    for(int i = 0; i < pClsDesc->cInterfaces; i++) {
        pClsIntf = pClsDesc->ppInterfaces[i];
        pIntfDesc = pCtx->m_pModule->ppInterfaceDir[pClsIntf->sIndex]->pDesc;
        for (int j = 0; j < pIntfDesc->cMethods; j++) {
            if (pIntfDesc->ppMethods[j]->dwAttribs & MethodAttrib_TrivialCtor) {
                return true;
            }
        }
    }

    return false;
}

IMPL_USERFUNC(HasDefaultConstructor)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClass && pvArg == pCtx->m_pClass);

    ClassDescriptor *pClsDesc = pCtx->m_pClass->pDesc;
    assert(pClsDesc->dwAttribs & ClassAttrib_t_clsobj);

    ClassInterface *pClsIntf;
    InterfaceDescriptor *pIntfDesc;

    for(int i = 0; i < pClsDesc->cInterfaces; i++) {
        pClsIntf = pClsDesc->ppInterfaces[i];
        pIntfDesc = pCtx->m_pModule->ppInterfaceDir[pClsIntf->sIndex]->pDesc;
        for (int j = 0; j < pIntfDesc->cMethods; j++) {
            if (pIntfDesc->ppMethods[j]->dwAttribs & MethodAttrib_DefaultCtor) {
                return true;
            }
        }
    }

    return false;
}

IMPL_USERFUNC(OrgClassIsAspect)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClass && pvArg == pCtx->m_pClass);

    ClassDirEntry *pClsDir = pCtx->m_pClass;
    assert(pClsDir->pDesc->dwAttribs & ClassAttrib_t_sink);

    if (pClsDir->pszName[0] == 'A') return true;
    else return false;
}

IMPL_USERFUNC(ParcelParameter)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);

    ParamDescriptor* pParamDesc = (ParamDescriptor*)pvArg;
    TypeDescriptor *pType = &(pParamDesc->type);
    TypeDescriptor orgType;

Restart:
    switch(pType->type) {
        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &orgType);
            pType = &orgType;
            goto Restart;

        case Type_Byte:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteByte(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteBytePtr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Char8:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteChar8(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteChar8Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Char16:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteChar16(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteChar16Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_CString:
            assert(0 == pType->nPointer);

            pCtx->PutString("pParams->WriteCString(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;


        case Type_String:
            assert(0 == pType->nPointer);

            pCtx->PutString("pParams->WriteString(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_Boolean:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteBoolean(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteBooleanPtr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_ECode:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInt32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteInt32Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Int16:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInt16(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteInt16Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Int32:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInt32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteInt32Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Int64:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInt64(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteInt64Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Float:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteFloat(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteFloatPtr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_Double:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteDouble(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteDoublePtr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_enum:
            assert(1 >= pType->nPointer);

            if (0 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInt32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteInt32Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_struct:
            pCtx->PutString("pParams->WriteStruct((Handle32)&");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(", sizeof(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString("));");
            break;

        case Type_EMuid:
            assert(0 == pType->nPointer);

            pCtx->PutString("pParams->WriteEMuid(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_EGuid:
            assert(0 == pType->nPointer);

            pCtx->PutString("pParams->WriteEGuid(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_interface:
            if (1 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInterfacePtr(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else if (2 == pType->nPointer) {
                pCtx->PutString("pParams->WriteInterfacePtrPtr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else assert(0);

            break;

        case Type_StringBuf:
            assert(0 == pType->nPointer);

            pCtx->PutString("pParams->WriteStringBuf((Handle32)&");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_ArrayOf:
            assert(0 == pType->nPointer);

            if (Type_CString == pType->pNestedType->type) {
                pCtx->PutString("pParams->WriteArrayOfCString(const_cast<ArrayOf<CString>*>(&");
            }
            else if (Type_String == pType->pNestedType->type) {
                pCtx->PutString("pParams->WriteArrayOfString(const_cast<ArrayOf<String>*>(&");
            }
            else {
                pCtx->PutString("pParams->WriteArrayOf((Handle32)&");
            }
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            if (Type_CString == pType->pNestedType->type || Type_String == pType->pNestedType->type) {
                pCtx->PutString("));");
            }
            else {
                pCtx->PutString(");");
            }
            break;

        case Type_BufferOf:
            assert(0 == pType->nPointer);

            if (Type_CString == pType->pNestedType->type) {
                pCtx->PutString("pParams->WriteBufferOfCString(");
            }
            else if (Type_String == pType->pNestedType->type) {
                pCtx->PutString("pParams->WriteBufferOfString(");
            }
            else {
                pCtx->PutString("pParams->WriteBufferOf((Handle32)&");
            }
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_MemoryBuf:
            assert(0 == pType->nPointer);
            pCtx->PutString("pParams->WriteMemoryBuf((Handle32)&");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        default:
            assert(0);
            break;
    }

    return LUBE_OK;
}

IMPL_USERFUNC(HasParameters)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pvArg);
    assert(NULL != pCtx->m_pMethod);

    if (pCtx->m_pMethod->cParams > 1) return true;

    return false;
}

IMPL_USERFUNC(ClassNamespaceBegin)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClass && pvArg == pCtx->m_pClass);

    ClassDirEntry *pClsDir = pCtx->m_pClass;
    if (pClsDir->pszNameSpace != NULL && pClsDir->pszNameSpace[0] != '\0') {
        char *pszNamespace = (char*)malloc(strlen(pClsDir->pszNameSpace) + 1);
        strcpy(pszNamespace, pClsDir->pszNameSpace);
        char *begin = pszNamespace;
        while (begin != NULL) {
            char *dot = strchr(begin, '.');
            if (dot != NULL) *dot = '\0';
            pCtx->PutString("namespace ");
            pCtx->PutString(begin);
            pCtx->PutString(" {\n");
            if (dot != NULL) begin = dot + 1;
            else begin = NULL;
        }
        free(pszNamespace);
    }

    return LUBE_OK;
}

IMPL_USERFUNC(ClassNamespaceEnd)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClass && pvArg == pCtx->m_pClass);

    ClassDirEntry *pClsDir = pCtx->m_pClass;
    if (pClsDir->pszNameSpace != NULL && pClsDir->pszNameSpace[0] != '\0') {
        char *pszNamespace = (char*)malloc(strlen(pClsDir->pszNameSpace) + 1);
        strcpy(pszNamespace, pClsDir->pszNameSpace);
        char *begin = pszNamespace;
        while (begin != NULL) {
            char *dot = strchr(begin, '.');
            if (dot != NULL) *dot = '\0';
            pCtx->PutString("}\n");
            if (dot != NULL) begin = dot + 1;
            else begin = NULL;
        }
        free(pszNamespace);
    }

    return LUBE_OK;
}

IMPL_USERFUNC(InterfaceNamespaceBegin)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pInterface && pvArg == pCtx->m_pInterface);

    InterfaceDirEntry *pItfDir = pCtx->m_pInterface;
    if (pItfDir->pszNameSpace != NULL && pItfDir->pszNameSpace[0] != '\0') {
        char *pszNamespace = (char*)malloc(strlen(pItfDir->pszNameSpace) + 1);
        strcpy(pszNamespace, pItfDir->pszNameSpace);
        char *begin = pszNamespace;
        while (begin != NULL) {
            char *dot = strchr(begin, '.');
            if (dot != NULL) *dot = '\0';
            pCtx->PutString("namespace ");
            pCtx->PutString(begin);
            pCtx->PutString(" {\n");
            if (dot != NULL) begin = dot + 1;
            else begin = NULL;
        }
        free(pszNamespace);
    }

    return LUBE_OK;
}

IMPL_USERFUNC(InterfaceNamespaceEnd)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pInterface && pvArg == pCtx->m_pInterface);

    InterfaceDirEntry *pItfDir = pCtx->m_pInterface;
    if (pItfDir->pszNameSpace != NULL && pItfDir->pszNameSpace[0] != '\0') {
        char *pszNamespace = (char*)malloc(strlen(pItfDir->pszNameSpace) + 1);
        strcpy(pszNamespace, pItfDir->pszNameSpace);
        char *begin = pszNamespace;
        while (begin != NULL) {
            char *dot = strchr(begin, '.');
            if (dot != NULL) *dot = '\0';
            pCtx->PutString("}\n");
            if (dot != NULL) begin = dot + 1;
            else begin = NULL;
        }
        free(pszNamespace);
    }

    return LUBE_OK;
}

IMPL_USERFUNC(EnumNamespaceBegin)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pEnum && pvArg == pCtx->m_pEnum);

    EnumDirEntry *pEnumDir = pCtx->m_pEnum;
    if (pEnumDir->pszNameSpace != NULL && pEnumDir->pszNameSpace[0] != '\0') {
        char *pszNamespace = (char*)malloc(strlen(pEnumDir->pszNameSpace) + 1);
        strcpy(pszNamespace, pEnumDir->pszNameSpace);
        char *begin = pszNamespace;
        while (begin != NULL) {
            char *dot = strchr(begin, '.');
            if (dot != NULL) *dot = '\0';
            pCtx->PutString("namespace ");
            pCtx->PutString(begin);
            pCtx->PutString(" {\n");
            if (dot != NULL) begin = dot + 1;
            else begin = NULL;
        }
        free(pszNamespace);
    }

    return LUBE_OK;
}

IMPL_USERFUNC(EnumNamespaceEnd)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pEnum && pvArg == pCtx->m_pEnum);

    EnumDirEntry *pEnumDir = pCtx->m_pEnum;
    if (pEnumDir->pszNameSpace != NULL && pEnumDir->pszNameSpace[0] != '\0') {
        char *pszNamespace = (char*)malloc(strlen(pEnumDir->pszNameSpace) + 1);
        strcpy(pszNamespace, pEnumDir->pszNameSpace);
        char *begin = pszNamespace;
        while (begin != NULL) {
            char *dot = strchr(begin, '.');
            if (dot != NULL) *dot = '\0';
            pCtx->PutString("}\n");
            if (dot != NULL) begin = dot + 1;
            else begin = NULL;
        }
        free(pszNamespace);
    }

    return LUBE_OK;
}
