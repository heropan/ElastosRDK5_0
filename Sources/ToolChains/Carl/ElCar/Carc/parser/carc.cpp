//==========================================================================
// Copyright (c) 2000-2009,  Elastos, Inc.  All Rights Reserved.
//==========================================================================

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "carc.h"
#include "clsutil.h"
#include <chgpath.h>

#ifdef _linux
#define _MAX_PATH 256
#endif

static bool s_bMakeDependence = false;
static char s_szCarDepName[_MAX_PATH] = "";

extern bool s_bLenientNaming;
extern bool s_bInKernel;
extern bool s_bSupportWeakRef;
extern bool s_bInNakedMode;

extern void SetDefaultThreadModel(ClassAttrib attrib);

void GenCarDependences(const char *pszden, int nRes)
{
    if(!pszden || !s_bMakeDependence) return;
    if (0 == nRes) {
        strcpy(s_szCarDepName, pszden);
    }
    else {
        static bool bdep = false;
        if (!bdep) {
            fprintf(stdout, "%s:", s_szCarDepName);
            bdep = TRUE;
        }
        fprintf(stdout, "\\\n");
//        GetUnixpath();
//        if(HasCygpath()){
//            char *pszDependPath;
//            pszDependPath = Turn2UnixPath(pszden);
//            fprintf(stdout, "%s", pszDependPath);
//        }
//        else {
            fprintf(stdout, "%s", pszden);
//        }
    }
}

bool GenCarChecksum(const char *pszName, CLSModule *pModule)
{
    FILE *pFile;
    int len;
    char *pBuf;
    unsigned int checksum;

    pFile = fopen(pszName, "r");
    if (!pFile) {
        return false;
    }

    fseek(pFile, 0, SEEK_END);
    len=ftell(pFile);
    if (len <= 0) {
        return false;
    }

    pBuf = (char*)malloc(len + 1);
    if (!pBuf) {
        return false;
    }
    fseek(pFile, 0, SEEK_SET);
    fread(pBuf, sizeof(char), len + 1, pFile);
    if (!feof(pFile)) {
        free(pBuf);
        return false;
    }

    fclose(pFile);

    checksum = rabin(pBuf, len);
    pModule->nChecksum = checksum;

    free(pBuf);

    return true;
}

bool GenCarBarcode(CLSModule *pModule)
{
    unsigned int barcode;
    time_t nowtime;

    time(&nowtime);
    tm tmDate = {0};
    tmDate.tm_mday=1;
    tmDate.tm_mon=0;
    tmDate.tm_year=2010-1900;

    time_t eltime=mktime(&tmDate);

    barcode = (unsigned long)(difftime(nowtime,eltime) / 60);

    pModule->nBarcode = barcode;

    return true;
}

extern void InitNamespace();
extern void UninitNamespace();

CLSModule * CompileCAR(const char *pszName, DWORD dwAttribs)
{
    CLSModule *pModule;
    char *psztmp;
    int len;

    if (!(dwAttribs & Command_s_NoSys)) {
        if (LoadSystemLibrary(dwAttribs & Command_e_NoElastos) < 0) {
            return NULL;
        }
    }

    pModule = CreateCLS();
    if (!pModule) {
        fprintf(stderr, "[ERROR] carc (0x1001) : Out of memory.\n");
        return NULL;
    }

    if (dwAttribs & Command_w_SuppWarn)
        SuppressWarning();
    if (dwAttribs & Command_t_Warn2Err)
        TreatWarningAsError();
    if (dwAttribs & Command_e_NoElastos)
        DisableWarning(0x000f);
    if (dwAttribs & Command_a_Compress)
        pModule->dwAttribs |= CARAttrib_compress;
    if (dwAttribs & Command_A_FreeModel)
        SetDefaultThreadModel(ClassAttrib_naked);
    if (dwAttribs & Command_i_IgrName)
        s_bLenientNaming = true;
    if (dwAttribs & Command_k_InKernel)
        s_bInKernel = true;
    if (dwAttribs & Command_d_Depend) {
        s_bMakeDependence = true;
        len = strlen(pszName);
        psztmp = new char[len + 1];
        if (!psztmp) {
            fprintf(stderr, "[ERROR] carc (0x1002) : Out of memory.\n");
            return NULL;
        }
        GetNakedFileName(pszName, psztmp);
        psztmp[len - 4] = '\0';
        strcat(psztmp, ".rsc");
        GenCarDependences(psztmp, 0);
        delete psztmp;
    }
    if (dwAttribs & Command_u_WeakRef)
        s_bSupportWeakRef = true;
    if (dwAttribs & Command_n_NakedMode)
        s_bInNakedMode = true;

    InitNamespace();

    // compiling
    //
    DoCompiling(pszName, pModule);

    UninitNamespace();

    if (0 != g_nErrorNumber) {
        fprintf(stderr, "[INFO] carc (0x1003) : Aborting compilation.\n");
        DestroyCLS(pModule);
        return NULL;
    }

    len = strlen(pszName);
    psztmp = new char[len + 1];
    if (!psztmp) {
        fprintf(stderr, "[ERROR] carc (0x1002) : Out of memory.\n");
        return NULL;
    }
    GetNakedFileName(pszName, psztmp);
    psztmp[len - 4] = '\0';
    strcat(psztmp, ".tmp");
    if (CLS2CAR(psztmp, pModule) < 0) {
        delete[] psztmp;
        fprintf(stderr, "[ERROR] carc (0x1002) : Generate temp file for generate checksum code error.\n");
        return NULL;
    }

    if (!GenCarChecksum(psztmp, pModule)) {
        delete[] psztmp;
        fprintf(stderr, "[ERROR] carc (0x1002) : Generate Rabin checksum code error.\n");
        return NULL;
    }

    if (!GenCarBarcode(pModule)) {
        fprintf(stderr, "[ERROR] carc (0x1002) : Generate barcode error.\n");
        return NULL;
    }

    delete[] psztmp;
    return pModule;
}

extern int ExtendCLS(CLSModule *pModule, BOOL bNoElastos);
int main(int nArgc, char **ppArgv)
{
    CommandArgs args;
    CLSModule *pModule;

    if (ParseArgs(nArgc, ppArgv, &args) < 0) exit(1);

    if (args.pszFactoryUrl) SetFactoryUrl(args.pszFactoryUrl);
    if (args.pszSourcePath) SetSourcePath(args.pszSourcePath);
    if (args.pszLibraryPath) SetLibraryPath(args.pszLibraryPath);

    pModule = CompileCAR(args.pszSource, args.dwAttribs);
    if (!pModule) exit(1);

    // generate file(s) specified by command line
    //
    if (args.pszCAR) {
        if (CLS2CAR(args.pszCAR, pModule) < 0) goto ErrorExit;
    }
    if (args.pszCLS) {
        if (SaveCLS(args.pszCLS, pModule) < 0) goto ErrorExit;
    }
    if (args.dwAttribs & Command_E_GenExCLS) {
        if (ExtendCLS(pModule, TRUE) < 0) goto ErrorExit;
        if (SaveCLS(args.pszExCLS, pModule) < 0) goto ErrorExit;
    }

    DestroyAllLibraries();
    DestroyCLS(pModule);
    return 0;

ErrorExit:
    fprintf(stderr, "[ERROR] carc (0x1004) : Writing file failed, aborting.\n");
    DestroyCLS(pModule);
    exit(1);
}
