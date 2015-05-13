/*
 * Copyright (C) 2006 Apple Computer, Inc.  All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef MIMETypeRegistry_h
#define MIMETypeRegistry_h

#include "PlatformString.h"
#include <wtf/HashSet.h>
#include <wtf/Vector.h>
#include <wtf/text/StringHash.h>

namespace WebCore {

class MIMETypeRegistry {
public:
    static WTF::String getMIMETypeForExtension(const WTF::String& extension);
#if ENABLE(FILE_SYSTEM) && ENABLE(WORKERS)
    static WTF::String getMIMETypeForExtensionThreadSafe(const WTF::String& extension);
#endif
    static WTF::Vector<WTF::String> getExtensionsForMIMEType(const WTF::String& type);
    static WTF::String getPreferredExtensionForMIMEType(const WTF::String& type);
    static WTF::String getMediaMIMETypeForExtension(const WTF::String& extension);
    static WTF::Vector<WTF::String> getMediaMIMETypesForExtension(const WTF::String& extension);

    static WTF::String getMIMETypeForPath(const WTF::String& path);

    // Check to see if a mime type is suitable for being loaded inline as an
    // image (e.g., <img> tags).
    static bool isSupportedImageMIMEType(const WTF::String& mimeType);

    // Check to see if a mime type is suitable for being loaded as an image
    // document in a frame.
    static bool isSupportedImageResourceMIMEType(const WTF::String& mimeType);

    // Check to see if a mime type is suitable for being encoded.
    static bool isSupportedImageMIMETypeForEncoding(const WTF::String& mimeType);

    // Check to see if a mime type is suitable for being loaded as a JavaScript
    // resource.
    static bool isSupportedJavaScriptMIMEType(const WTF::String& mimeType);

    // Check to see if a non-image mime type is suitable for being loaded as a
    // document in a frame.  Includes supported JavaScript MIME types.
    static bool isSupportedNonImageMIMEType(const WTF::String& mimeType);

    // Check to see if a mime type is suitable for being loaded using <video> and <audio>
    static bool isSupportedMediaMIMEType(const WTF::String& mimeType);

    // Check to see if the mime type is not suitable for being loaded as a text
    // document in a frame. Only valid for mime types begining with "text/".
    static bool isUnsupportedTextMIMEType(const WTF::String& mimeType);

    // Check to see if a mime type is a valid Java applet mime type
    static bool isJavaAppletMIMEType(const WTF::String& mimeType);

    // Check to see if a mime type is a plugin implemented by the
    // browser (e.g. a Qt Plugin).
    static bool isApplicationPluginMIMEType(const WTF::String& mimeType);

    static WTF::HashSet<WTF::String>& getSupportedImageMIMETypes();
    static WTF::HashSet<WTF::String>& getSupportedImageResourceMIMETypes();
    static WTF::HashSet<WTF::String>& getSupportedImageMIMETypesForEncoding();
    static WTF::HashSet<WTF::String>& getSupportedNonImageMIMETypes();
    static WTF::HashSet<WTF::String>& getSupportedMediaMIMETypes();
    static WTF::HashSet<WTF::String>& getUnsupportedTextMIMETypes();
};

const WTF::String& defaultMIMEType();

} // namespace WebCore

#endif // MIMETypeRegistry_h
