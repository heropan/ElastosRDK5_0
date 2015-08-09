
#include <stdint.h>
#include <elatypes.h>
#include "clsdef.h"
#include <stdlib.h>
#include <stdio.h>

typedef
struct ModuleRsc {
    unsigned int    uSize;
    const char      uClsinfo[0];
} ModuleRsc;

static const char* OBJCOPY = "arm-linux-androideabi-objcopy -j .clsresource -O binary ";
static const char* CLSFILE = "cls.dump";

static CLSModule* sModule = NULL;

void dump_param_type(ParamDescriptor* paramDesc)
{
    switch(paramDesc->type.type) {
    case Type_Char16:
        printf("Char16");
        break;
    case Type_Char32:
        printf("Char32");
        break;
    case Type_Boolean:
        printf("Boolean");
        break;
    case Type_Float:
        printf("Float");
        break;
    case Type_Double:
        printf("Double");
        break;
    case Type_Int8:
        printf("Int8");
        break;
    case Type_Int16:
        printf("Int16");
        break;
    case Type_Int32:
        printf("Int32");
        break;
    case Type_Int64:
        printf("Int64");
        break;
    case Type_Byte:
        printf("Byte");
        break;
    case Type_UInt16:
        printf("UInt16");
        break;
    case Type_UInt32:
        printf("UInt32");
        break;
    case Type_UInt64:
        printf("UInt64");
        break;
    case Type_EMuid:
        printf("EMuid");
        break;
    case Type_ECode:
        printf("ECode");
        break;
    case Type_EGuid:
        printf("EGuid");
        break;
    case Type_PVoid:
        printf("PVoid");
        break;
    case Type_String:
        printf("String");
        break;
    case Type_ArrayOf:
        printf("ArrayOf");
        break;
    default:
        printf("Unknow type");
        break;
    }

    switch(paramDesc->type.nPointer) {
    case 0:
        printf("\n");
        break;
    case 1:
        printf("*\n");
        break;
    case 2:
        printf("**\n");
        break;
    default:
        printf("invalid pointer\n");
        break;
    }
}

void dump_method_params(MethodDescriptor* mtdDesc)
{
    printf("[%d params]\n", mtdDesc->cParams);
    for (int i = 0; i < mtdDesc->cParams; ++i) {
        ParamDescriptor* paramDesc = mtdDesc->ppParams[i];
        printf("%s:", paramDesc->pszName);
        dump_param_type(paramDesc);
    }
    printf("[end]\n");
}

void dump_interface_method(InterfaceDescriptor* itfDesc)
{
    printf("[%d methods]\n", itfDesc->cMethods);
    for (int i = 0; i < itfDesc->cMethods; ++i) {
        MethodDescriptor* mtdDesc = itfDesc->ppMethods[i];
        printf("%s\n", mtdDesc->pszName);
        dump_method_params(mtdDesc);
    }
    printf("[end]\n");
}

void dump_interface(CLSModule* clsmod)
{
    printf("[%d interfaces]\n", clsmod->mDefinedInterfaceCount);
    for (int i = 0; i < clsmod->mDefinedInterfaceCount; ++i) {
        int itfIndex = clsmod->pDefinedInterfaceIndex[i];
        InterfaceDirEntry* itfDirEntry = clsmod->ppInterfaceDir[itfIndex];
        printf("%s\n", itfDirEntry->pszName);
        dump_interface_method(itfDirEntry->pDesc);
    }
    printf("[end]\n");
}

void dump_class_interface(ClassDescriptor* clsDesc)
{
    printf("[%d interfaces]\n", clsDesc->mInterfaceCount);
    for (int i = 0; i < clsDesc->mInterfaceCount; ++i) {
        ClassInterface* clsItf = clsDesc->ppInterfaces[i];
        printf("%s\n", sModule->ppInterfaceDir[clsItf->sIndex]->pszName);
    }
    printf("[end]\n");
}

void dump_class(CLSModule* clsmod)
{
    printf("[%d classes]\n", clsmod->cClasses);
    for (int i = 0; i < clsmod->cClasses; ++i) {
        ClassDirEntry* clsDirEntry = clsmod->ppClassDir[i];
        printf("%s\n", clsDirEntry->pszName);
        dump_class_interface(clsDirEntry->pDesc);
    }
    printf("[end]\n");
}

void dump_eco(CLSModule* clsmod)
{
    printf("[name]\n");
    printf("%s.eco\n", clsmod->pszName);
    dump_interface(clsmod);
    dump_class(clsmod);
    printf("[end]\n");
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        printf("dumpeco <eco name> \n");
        return 0;
    }

    char* command = (char*)malloc(strlen(OBJCOPY) + strlen(argv[1]) + strlen(CLSFILE) + 2);
    strcpy(command, OBJCOPY);
    strcat(command, argv[1]);
    strcat(command, " ");
    strcat(command, CLSFILE);

    int ret = system(command);

    FILE* file = fopen(CLSFILE, "rb");
    if(!file) {
        printf("fopen %s's cls.dump failed!\n", argv[1]);
        return 0;
    }

    fseek(file, 0, SEEK_END);
    int len = ftell(file);
    if (len <= 0) {
        printf("%s's cls.dump length should not be 0!\n", argv[1]);
        fclose(file);
        return 0;
    }

    fseek(file, 0, SEEK_SET);

    void* buffer = malloc(len);
    fread(buffer, len, 1, file);
    ModuleRsc* rsc = (ModuleRsc*)buffer;
    void* address = (void*)&(rsc->uClsinfo[0]);

    ret = RelocFlattedCLS((CLSModule*)address, len, &sModule);

    dump_eco(sModule);

    fclose(file);
    remove(CLSFILE);
    return 0;
}
