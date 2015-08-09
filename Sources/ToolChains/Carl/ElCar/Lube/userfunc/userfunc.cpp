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

    switch (pType->mType) {
        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &type);
            if ((Type_EMuid == type.mType)
                    || (Type_EGuid == type.mType)
                    || (Type_struct == type.mType)){
                if (dwAttribs & ParamAttrib_in) {
                    if (0 == type.mPointer) {
                        assert(0 == pType->mPointer);
                        sprintf(szType, "const %s *", Type2CString(pCtx->m_pModule, pType));
                        pszType = szType;
                    }
                    else pszType = Type2CString(pCtx->m_pModule, pType);
                }
                else pszType = Type2CString(pCtx->m_pModule, pType);
            }
            else if (Type_ArrayOf == type.mType) {
                if (dwAttribs & ParamAttrib_in) {
                    if (0 == type.mPointer) {
                        assert(0 == pType->mPointer);
                        sprintf(szType, "const %s *", Type2CString(pCtx->m_pModule, pType));
                        pszType = szType;
                    }
                    else pszType = Type2CString(pCtx->m_pModule, pType);
                }
                else {
                    if (dwAttribs & ParamAttrib_callee) {
                        if (0 == pType->mPointer) {
                            assert(1 == type.mPointer);
                            sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                        }
                        else if (1 == pType->mPointer) {
                            assert(0 == type.mPointer);
                            sprintf(szType, "%s **", Type2CString(pCtx->m_pModule, pType));
                        }
                        pszType = szType;
                    }
                    else {
                        assert(0 == type.mPointer);
                        assert(0 == pType->mPointer);
                        sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                        pszType = szType;
                    }
                }
            }
            else pszType = Type2CString(pCtx->m_pModule, pType);
            break;

        case Type_EMuid:
        case Type_EGuid:
        case Type_struct:
            if (dwAttribs & ParamAttrib_in) {
                if (0 == pType->mPointer) {
                    sprintf(szType, "const %s *", Type2CString(pCtx->m_pModule, pType));
                    pszType = szType;
                }
                else pszType = Type2CString(pCtx->m_pModule, pType);
            }
            else pszType = Type2CString(pCtx->m_pModule, pType);
            break;

        case Type_ArrayOf:
            if (dwAttribs & ParamAttrib_in) {
                pszType = "PCarQuintet";    //const
            }
            if (dwAttribs & ParamAttrib_out) {
                if (dwAttribs & ParamAttrib_callee) pszType = "PCarQuintet *";
                else pszType = "PCarQuintet";
            }
            break;

        case Type_String:
            assert(pType->mPointer <= 1);
            if (1 == pType->mPointer) {
                pszType = "String*";
            }
            else if (0 == pType->mPointer) {
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
    switch (pType->mType) {
        case Type_Char16:
        case Type_Char32:
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
            if (Type_String == pType->mNestedType->mType) {
                pszType = "ArrayOfString";
            }
            else {
                pszType = "CarArray";
            }
            break;

        case Type_Array:
            pszType = "CarArray";
            break;

        case Type_interface:
            pszType = "Interface";
            break;

        case Type_struct:
            pszType = "Struct";
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
                pCtx->m_pModule->mInterfaceDirs[pCtx->m_pClass->pDesc->sCtorIndex];\
    int m, p, i;\
    char szBuf[c_nStrBufSize];\
    int nLoopNum = 0;\
    int nLoopEnd = 3;\
    TypeDescriptor type, OrigType;\
    const char *string;\
    for (i = 0; i < pCtx->m_pClass->pDesc->mInterfaceCount; i++) {\
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
                    if (Type_struct == type.mType || Type_EMuid == type.mType ||\
                        Type_EGuid == type.mType || Type_ArrayOf == type.mType) {\
                        string = StructType2CString(pCtx->m_pModule, &type);\
                    }\
                    else if (Type_alias == type.mType) {\
                        GetOriginalType(pCtx->m_pModule, &type, &OrigType);\
                        if ((Type_struct == OrigType.mType || Type_EMuid == OrigType.mType ||\
                             Type_EGuid == OrigType.mType || Type_ArrayOf == type.mType)\
                             && OrigType.mPointer == 0) {\
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
                  pCtx->m_pModule->mInterfaceDirs[pCtx->m_pClass->pDesc->ppInterfaces[i]->mIndex]->pszName, \
                  pCtx->m_pModule->mInterfaceDirs[pCtx->m_pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);\
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
    pClass = pCtx->m_pModule->mClassDirs[r];\
    if (0 == (pClass->pDesc->dwAttribs & ClassAttrib_t_generic)) {\
        return LUBE_OK;\
    }\

#define START_FOR_LOOP(pClass) \
    int m, p, i;\
    int length;\
    char szBuf[c_nStrBufSize];\
    char *pszName;\
    InterfaceDirEntry *pCtorInterface = NULL;\
    pCtorInterface = pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->sCtorIndex];\
    for (i = 0; i < pClass->pDesc->mInterfaceCount; i++) {\
        if (pClass->pDesc->ppInterfaces[i]->wAttribs & ClassInterfaceAttrib_callback) {\
            continue;\
        }\
        pszName = pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName;\
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
                pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName, \
                pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);\
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
                    mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);\

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
                    pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);
            pCtx->PutString(szBuf);
            pCtx->PutString("    if (!pClsid) {\n");
            sprintf(szBuf, "        (*pp%s)->Release();\n",
                    pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);
            pCtx->PutString(szBuf);
            pCtx->PutString("        return E_WRONG_GENERIC;\n");
            pCtx->PutString("    }\n");
            sprintf(szBuf, "    if (*pClsid != ECLSID_%s) {\n", szName);
            pCtx->PutString(szBuf);
            sprintf(szBuf, "        (*pp%s)->Release();\n",
                    pCtx->m_pModule->mInterfaceDirs[pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);
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
        pClass = pCtx->m_pModule->mClassDirs[pClass->pDesc->sParentIndex];
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
    pCtx->PutString(pModule->mName);
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
    assert(Type_struct == pParamDesc->type.mType);

    char szName[c_nStrBufSize];
    szName[0] = 0;
    // If struct parameter type is't pointer, we have to change its type
    // to a pointer and prefix 'p' to its name.
    if (0 == pParamDesc->type.mPointer) {
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

    switch (pParamDesc->type.mType) {
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

    for (i = 0; i < pCtx->m_pClass->pDesc->mInterfaceCount; i++) {
        if (pCtx->m_pClass->pDesc->ppInterfaces[i]->wAttribs & ClassInterfaceAttrib_callback) {
            continue;
        }
        pCtx->PutString(pCtx->m_pModule->mInterfaceDirs[pCtx->m_pClass->pDesc->ppInterfaces[i]->mIndex]->pszName);
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
    if (Type_alias == pParamDesc->type.mType) {
        GetOriginalType(pCtx->m_pModule, &pParamDesc->type, &paramType);
    }

    switch (paramType.mType) {

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
        TYPE_CASE(Type_Char16, NULL, "p", NULL)
        TYPE_CASE(Type_Char32, NULL, "p", NULL)
        TYPE_CASE(Type_PVoid, "p", "pp", NULL)
        TYPE_CASE(Type_String, NULL, "p", NULL)
        TYPE_CASE(Type_ArrayOf, "p", "pp", NULL)
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
        if ((Type_PVoid == paramType.mType)
                || (1 == paramType.mPointer)) {
            pszPrefix = GeneratePrefixalVarName("p", pszVarName);
        }
    }
    else if (pParamDesc->dwAttribs & ParamAttrib_out) {
        if (1 == paramType.mPointer) { // Considered as caller
            pszPrefix = GeneratePrefixalVarName(pszPrefix1, pszVarName);
        }
        else if (2 == paramType.mPointer) { // Considered as callee
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
    if (Type_alias == pParamDesc->type.mType) {
        GetOriginalType(pCtx->m_pModule, &pParamDesc->type, &paramType);
    }

    switch (paramType.mType) {

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
        TYPE_CASE(Type_Char16, NULL, "p", NULL)
        TYPE_CASE(Type_Char32, NULL, "p", NULL)
        TYPE_CASE(Type_PVoid, "p", "pp", NULL)
        TYPE_CASE(Type_String, NULL, "p", NULL)
        TYPE_CASE(Type_ArrayOf, "p", "pp", NULL)
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
        if ((Type_PVoid == paramType.mType)
                || (1 == paramType.mPointer)) {
            pszPrefix = GeneratePrefixalVarName("p", pszVarName);
        }
    }
    else if (pParamDesc->dwAttribs & ParamAttrib_out) {
        if (1 == paramType.mPointer) {
            pszPrefix = GeneratePrefixalVarName(pszPrefix1, pszVarName);
        }
        else if (2 == paramType.mPointer) {
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

    if (Type_alias == pParamDesc->type.mType) {
        GetOriginalType(pCtx->m_pModule, pType, &type);
        pType = &type;
    }

    if (Type_interface == pType->mType &&
        pParamDesc->dwAttribs & ParamAttrib_in &&
        1 == pType->mPointer) {

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

    if (Type_alias == pType->mType) {
        GetOriginalType(pCtx->m_pModule, pType, &type);
        pType = &type;
    }

    if (Type_interface == pType->mType &&
        pParamDesc->dwAttribs & ParamAttrib_in &&
        1 == pType->mPointer) {
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

    switch (pType->mType) {
        case Type_ArrayOf:
            assert(pType->mNestedType);
            if (dwAttribs & ParamAttrib_in) {
                sprintf(szType, "ArrayOf<%s>",
                    Type2CString(pCtx->m_pModule, pType->mNestedType));
            }
            else {
                sprintf(szType, "ArrayOf<%s>*",
                    Type2CString(pCtx->m_pModule, pType->mNestedType));
            }
            pszType = szType;
            break;

        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &type);
            if ((type.mType == Type_EGuid ||
                type.mType == Type_EMuid) &&
                1 == type.mPointer) {
                if (type.mType == Type_EGuid) strcpy(szType, "EGuid");
                else if (type.mType == Type_EMuid) strcpy(szType, "EMuid");
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

    if (Type_Array == pType->mType) {
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

    if ((Type_struct == pType->mType)
            || (Type_EMuid == pType->mType)
            || (Type_EGuid == pType->mType)) {
        if (0 == pType->mPointer) {
            sprintf(szType, "const %s &",
                    Type2CString(pCtx->m_pModule, pType));
            pszType = szType;
        }
        else {
            pszType = Type2CString(pCtx->m_pModule, pType);
        }
    }
    else if (Type_alias == pType->mType) {
        GetOriginalType(pCtx->m_pModule, pType, &type);
        if ((Type_EMuid == type.mType)
                || (Type_EGuid == type.mType)
                || (Type_struct == type.mType)) {
            if (dwAttribs & ParamAttrib_in) {
                if (0 == type.mPointer) {
                    assert(0 == pType->mPointer);
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
        else if (Type_ArrayOf == type.mType) {
            if (dwAttribs & ParamAttrib_in) {
                if (0 == type.mPointer) {
                    assert(0 == pType->mPointer);
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
                    if (0 == pType->mPointer) {
                        assert(1 == type.mPointer);
                        sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                    }
                    else if (1 == pType->mPointer) {
                        assert(0 == type.mPointer);
                        sprintf(szType, "%s **", Type2CString(pCtx->m_pModule, pType));
                    }
                    pszType = szType;
                }
                else {
                    assert(0 == pType->mPointer);
                    assert(0 == type.mPointer);
                    sprintf(szType, "%s *", Type2CString(pCtx->m_pModule, pType));
                    pszType = szType;
                }
            }
        }
        else pszType = Type2CString(pCtx->m_pModule, pType);
    }
    else if (Type_ArrayOf == pType->mType) {
        if (dwAttribs & ParamAttrib_in) {
            if (0 == pType->mPointer) {
                sprintf(szType, "const ArrayOf<%s> &",
                        Type2CString(pCtx->m_pModule, pType->mNestedType));
            }
            else sprintf(szType, "%s", Type2CString(pCtx->m_pModule, pType));
        }
        else {
            //dwAttribs == ParamAttrib_out
            sprintf(szType, "ArrayOf<%s>",
                    Type2CString(pCtx->m_pModule, pType->mNestedType));
            if (0 == pType->mPointer) strcat(szType, " *");
            else strcat(szType, " **");
        }
        pszType = szType;
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

    switch(pType->mType) {
        case Type_ArrayOf:
            strcpy(szType, "ArrayOf<");
            strcat(szType, Type2CString(pCtx->m_pModule, pType->mNestedType));
            strcat(szType, ">");
            if (1 == pType->mPointer) strcat(szType, " *");
            else if (2 == pType->mPointer) strcat(szType, " **");
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

IMPL_USERFUNC(IsFiltered)(PLUBECTX pCtx, PSTATEDESC pDesc, PVOID pvArg)
{
    assert(NULL != pCtx->m_pClsIntf && pvArg == pCtx->m_pClsIntf);
    assert(NULL != pCtx->m_pOrigClass);

    ClassInterface *pClsIntf = pCtx->m_pClsIntf;
    ClassDescriptor *pClsDesc = pCtx->m_pOrigClass->pDesc;

    for(int i = 0; i < pClsDesc->mInterfaceCount; i++) {
        if ((pClsDesc->ppInterfaces[i]->mIndex == pClsIntf->mIndex)
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

    for(int i = 0; i < pClsDesc->mInterfaceCount; i++) {
        pClsIntf = pClsDesc->ppInterfaces[i];
        pIntfDesc = pCtx->m_pModule->mInterfaceDirs[pClsIntf->mIndex]->pDesc;
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

    for(int i = 0; i < pClsDesc->mInterfaceCount; i++) {
        pClsIntf = pClsDesc->ppInterfaces[i];
        pIntfDesc = pCtx->m_pModule->mInterfaceDirs[pClsIntf->mIndex]->pDesc;
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
    switch(pType->mType) {
        case Type_alias:
            GetOriginalType(pCtx->m_pModule, pType, &orgType);
            pType = &orgType;
            goto Restart;

        case Type_Byte:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteByte(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Char16:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteChar16(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Char32:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteChar32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                pCtx->PutString("pParams->WriteChar32Ptr((Handle32)");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            break;

        case Type_String:
            assert(0 == pType->mPointer);

            pCtx->PutString("pParams->WriteString(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_Boolean:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteBoolean(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_ECode:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteInt32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Int16:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteInt16(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Int32:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteInt32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Int64:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteInt64(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Float:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteFloat(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_Double:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteDouble(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
            }
            break;

        case Type_enum:
            assert(1 >= pType->mPointer);

            if (0 == pType->mPointer) {
                pCtx->PutString("pParams->WriteInt32(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else {
                assert(0);
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
            assert(0 == pType->mPointer);

            pCtx->PutString("pParams->WriteEMuid(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_EGuid:
            assert(0 == pType->mPointer);

            pCtx->PutString("pParams->WriteEGuid(");
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            pCtx->PutString(");");
            break;

        case Type_interface:
            if (1 == pType->mPointer) {
                pCtx->PutString("pParams->WriteInterfacePtr(");
                UserFunc_PrefixingName(pCtx, pDesc, pvArg);
                pCtx->PutString(");");
            }
            else if (2 == pType->mPointer) {
                assert(0);
            }
            else assert(0);

            break;

        case Type_ArrayOf:
            assert(0 == pType->mPointer);

            if (Type_String == pType->mNestedType->mType) {
                pCtx->PutString("pParams->WriteArrayOfString(const_cast<ArrayOf<String>*>(&");
            }
            else {
                pCtx->PutString("pParams->WriteArrayOf((Handle32)&");
            }
            UserFunc_PrefixingName(pCtx, pDesc, pvArg);
            if (Type_String == pType->mNestedType->mType) {
                pCtx->PutString("));");
            }
            else {
                pCtx->PutString(");");
            }
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
