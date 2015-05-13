
#ifndef __CURIHELPER_H__
#define __CURIHELPER_H__

#include "_CUriHelper.h"
#include <Elastos.Core.h>

using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Net {

CarClass(CUriHelper)
{
public:
    CARAPI GetEMPTY(
        /* [out] */ IUri** uri);

    CARAPI Parse(
        /* [in] */ const String& str,
        /* [out] */ IUri** uri);

    CARAPI FromFile(
        /* [in] */ IFile* file,
        /* [out] */ IUri** uri);

    CARAPI FromParts(
        /* [in] */ const String& scheme,
        /* [in] */ const String& ssp,
        /* [in] */ const String& fragment,
         /* [out] */ IUri** uri);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* p,
        /* [in] */ IUri* uri);

    CARAPI Decode(
        /* [in] */ const String& s,
        /* [out] */ String* result);

    CARAPI Encode(
        /* [in] */ const String& s,
        /* [out] */ String* encoded);

    CARAPI EncodeEx(
        /* [in] */ const String& s,
        /* [in] */ const String& allow,
        /* [out] */ String* result);

    CARAPI WithAppendedPath(
        /* [in] */ IUri* baseUri,
        /* [in] */ const String& pathSegment,
        /* [out] */ IUri** result);
};

} // namespace Net
} // namespace Droid
} // namespace Elastos

#endif //__CURIHELPER_H__
