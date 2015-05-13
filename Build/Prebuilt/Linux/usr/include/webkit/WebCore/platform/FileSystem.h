/*
 * Copyright (C) 2007, 2008, 2011 Apple Inc. All rights reserved.
 * Copyright (C) 2008 Collabora, Ltd. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1.  Redistributions of source code must retain the above copyright
 *     notice, this list of conditions and the following disclaimer.
 * 2.  Redistributions in binary form must reproduce the above copyright
 *     notice, this list of conditions and the following disclaimer in the
 *     documentation and/or other materials provided with the distribution.
 * 3.  Neither the name of Apple Computer, Inc. ("Apple") nor the names of
 *     its contributors may be used to endorse or promote products derived
 *     from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE AND ITS CONTRIBUTORS "AS IS" AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL APPLE OR ITS CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef FileSystem_h
#define FileSystem_h

#include "PlatformString.h"
#include <time.h>
#include <wtf/Forward.h>
#include <wtf/Vector.h>

#if USE(CF)
#include <wtf/RetainPtr.h>
#endif

#if PLATFORM(QT)
#include <QFile>
#include <QLibrary>
#if defined(Q_OS_WIN32)
#include <windows.h>
#endif
#endif

#if PLATFORM(WX)
#include <wx/defs.h>
#include <wx/file.h>
#endif

#if USE(CF) || (PLATFORM(QT) && defined(Q_WS_MAC))
typedef struct __CFBundle* CFBundleRef;
typedef const struct __CFData* CFDataRef;
#endif

#if OS(WINDOWS)
// These are to avoid including <winbase.h> in a header for Chromium
typedef void *HANDLE;
// Assuming STRICT
typedef struct HINSTANCE__* HINSTANCE;
typedef HINSTANCE HMODULE;
#endif

#if PLATFORM(BREWMP)
typedef struct _IFile IFile;
#endif

#if PLATFORM(GTK)
typedef struct _GFileIOStream GFileIOStream;
typedef struct _GModule GModule;
#endif

namespace WebCore {

// PlatformModule
#if PLATFORM(GTK)
typedef GModule* PlatformModule;
#elif OS(WINDOWS)
typedef HMODULE PlatformModule;
#elif PLATFORM(QT)
#if defined(Q_WS_MAC)
typedef CFBundleRef PlatformModule;
#elif !defined(QT_NO_LIBRARY)
typedef QLibrary* PlatformModule;
#else
typedef void* PlatformModule;
#endif
#elif USE(CF)
typedef CFBundleRef PlatformModule;
#else
typedef void* PlatformModule;
#endif

// PlatformModuleVersion
#if OS(WINDOWS)
struct PlatformModuleVersion {
    unsigned leastSig;
    unsigned mostSig;

    PlatformModuleVersion(unsigned)
        : leastSig(0)
        , mostSig(0)
    {
    }

    PlatformModuleVersion(unsigned lsb, unsigned msb)
        : leastSig(lsb)
        , mostSig(msb)
    {
    }

};
#else
typedef unsigned PlatformModuleVersion;
#endif

// PlatformFileHandle
#if PLATFORM(QT)
typedef QFile* PlatformFileHandle;
const PlatformFileHandle invalidPlatformFileHandle = 0;
#elif PLATFORM(GTK)
typedef GFileIOStream* PlatformFileHandle;
const PlatformFileHandle invalidPlatformFileHandle = 0;
#elif OS(WINDOWS)
typedef HANDLE PlatformFileHandle;
// FIXME: -1 is INVALID_HANDLE_VALUE, defined in <winbase.h>. Chromium tries to
// avoid using Windows headers in headers.  We'd rather move this into the .cpp.
const PlatformFileHandle invalidPlatformFileHandle = reinterpret_cast<HANDLE>(-1);
#elif PLATFORM(BREWMP)
typedef IFile* PlatformFileHandle;
const PlatformFileHandle invalidPlatformFileHandle = 0;
#elif PLATFORM(WX)
typedef wxFile* PlatformFileHandle;
const PlatformFileHandle invalidPlatformFileHandle = 0;
#else
typedef int PlatformFileHandle;
const PlatformFileHandle invalidPlatformFileHandle = -1;
#endif

enum FileOpenMode {
    OpenForRead = 0,
    OpenForWrite
};

enum FileSeekOrigin {
    SeekFromBeginning = 0,
    SeekFromCurrent,
    SeekFromEnd
};

#if OS(WINDOWS)
static const char PlatformFilePathSeparator = '\\';
#else
static const char PlatformFilePathSeparator = '/';
#endif

void revealFolderInOS(const WTF::String&);
bool fileExists(const WTF::String&);
bool deleteFile(const WTF::String&);
bool deleteEmptyDirectory(const WTF::String&);
bool getFileSize(const WTF::String&, long long& result);
bool getFileModificationTime(const WTF::String&, time_t& result);
WTF::String pathByAppendingComponent(const WTF::String& path, const WTF::String& component);
bool makeAllDirectories(const WTF::String& path);
WTF::String homeDirectoryPath();
WTF::String pathGetFileName(const WTF::String&);
WTF::String directoryName(const WTF::String&);

bool canExcludeFromBackup(); // Returns true if any file can ever be excluded from backup.
bool excludeFromBackup(const WTF::String&); // Returns true if successful.

WTF::Vector<WTF::String> listDirectory(const WTF::String& path, const WTF::String& filter = WTF::String());

WTF::CString fileSystemRepresentation(const WTF::String&);

inline bool isHandleValid(const PlatformFileHandle& handle) { return handle != invalidPlatformFileHandle; }

// Prefix is what the filename should be prefixed with, not the full path.
WTF::String openTemporaryFile(const WTF::String& prefix, PlatformFileHandle&);
PlatformFileHandle openFile(const WTF::String& path, FileOpenMode);
void closeFile(PlatformFileHandle&);
// Returns the resulting offset from the beginning of the file if successful, -1 otherwise.
long long seekFile(PlatformFileHandle, long long offset, FileSeekOrigin);
bool truncateFile(PlatformFileHandle, long long offset);
// Returns number of bytes actually read if successful, -1 otherwise.
int writeToFile(PlatformFileHandle, const char* data, int length);
// Returns number of bytes actually written if successful, -1 otherwise.
int readFromFile(PlatformFileHandle, char* data, int length);

// Functions for working with loadable modules.
bool unloadModule(PlatformModule);

// Encode a string for use within a file name.
WTF::String encodeForFileName(const WTF::String&);

#if USE(CF)
RetainPtr<CFURLRef> pathAsURL(const WTF::String&);
#endif

#if PLATFORM(CHROMIUM)
WTF::String pathGetDisplayFileName(const WTF::String&);
#endif

#if PLATFORM(GTK)
WTF::String filenameToString(const char*);
WTF::String filenameForDisplay(const WTF::String&);
WTF::CString applicationDirectoryPath();
#endif

#if PLATFORM(WIN) && !OS(WINCE)
WTF::String localUserSpecificStorageDirectory();
WTF::String roamingUserSpecificStorageDirectory();
bool safeCreateFile(const WTF::String&, CFDataRef);
#endif

} // namespace WebCore

#endif // FileSystem_h
