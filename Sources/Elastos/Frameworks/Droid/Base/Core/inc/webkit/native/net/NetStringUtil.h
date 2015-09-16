// wuweizuo automatic build .h file from .java file.
// Copyright 2014 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef _ELASTOS_DROID_WEBKIT_NET_NETSTRINGUTIL_H_
#define _ELASTOS_DROID_WEBKIT_NET_NETSTRINGUTIL_H_

#include "elatypes.h"
#include "elautoptr.h"
#include "ext/frameworkext.h"
#include "elastos/text/Normalizer.h"

// package org.chromium.net;
// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;
// import java.nio.ByteBuffer;
// import java.nio.charset.Charset;
// import java.nio.charset.CharsetDecoder;
// import java.nio.charset.CodingErrorAction;
// import java.text.Normalizer;

using Elastos::Text::INormalizer;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

/**
  * Utility functions for converting strings between formats when not built with
  * icu.
  */
// @JNINamespace("net::android")
class NetStringUtil
{
private:
    /**
      * Attempts to convert text in a given character set to a Unicode string.
      * Returns null on failure.
      * @param text ByteBuffer containing the character array to convert.
      * @param charset Character set it's in encoded in.
      * @return: Unicode string on success, null on failure.
      */
    // @CalledByNative
    static CARAPI_(String) ConvertToUnicode(
        /* [in] */ IByteBuffer* text,
        /* [in] */ String charset_name);

    /**
      * Attempts to convert text in a given character set to a Unicode string,
      * and normalize it.  Returns null on failure.
      * @param text ByteBuffer containing the character array to convert.
      * @param charset Character set it's in encoded in.
      * @return: Unicode string on success, null on failure.
      */
    // @CalledByNative
    static CARAPI_(String) ConvertToUnicodeAndNormalize(
        /* [in] */ IByteBuffer* text,
        /* [in] */ String charset_name);

    /**
      * Convert text in a given character set to a Unicode string.  Any invalid
      * characters are replaced with U+FFFD.  Returns null if the character set
      * is not recognized.
      * @param text ByteBuffer containing the character array to convert.
      * @param charset Character set it's in encoded in.
      * @return: Unicode string on success, null on failure.
      */
    // @CalledByNative
    static CARAPI_(String) ConvertToUnicodeWithSubstitutions(
        /* [in] */ IByteBuffer* text,
        /* [in] */ String charset_name);
};

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif // _ELASTOS_DROID_WEBKIT_NET_NETSTRINGUTIL_H_

