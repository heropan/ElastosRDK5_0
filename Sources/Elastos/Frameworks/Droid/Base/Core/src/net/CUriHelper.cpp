
#include "net/CUriHelper.h"
#include "net/Uri.h"

namespace Elastos {
namespace Droid {
namespace Net {

ECode CUriHelper::GetEMPTY(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    AutoPtr<IUri> empty = Uri::GetEMPTY();
    *uri = empty;
    INTERFACE_ADDREF(*uri);
    return NOERROR;
}

ECode CUriHelper::Parse(
    /* [in] */ const String& str,
    /* [out] */ IUri** uri)
{
    return Uri::Parse(str, uri);
}

ECode CUriHelper::FromFile(
    /* [in] */ IFile* file,
    /* [out] */ IUri** uri)
{
    return Uri::FromFile(file, uri);
}

ECode CUriHelper::FromParts(
    /* [in] */ const String& scheme,
    /* [in] */ const String& ssp,
    /* [in] */ const String& fragment,
    /* [out] */ IUri** uri)
{
    return Uri::FromParts(scheme, ssp, fragment, uri);
}

ECode CUriHelper::WriteToParcel(
    /* [in] */ IParcel* p,
    /* [in] */ IUri* uri)
{
    return Uri::WriteToParcel(p, uri);
}

ECode CUriHelper::Decode(
    /* [in] */ const String& s,
    /* [out] */ String* result)
{
    return Uri::Decode(s, result);
}

ECode CUriHelper::Encode(
    /* [in] */ const String& s,
    /* [out] */ String* encoded)
{
    return Uri::Encode(s, encoded);
}

ECode CUriHelper::EncodeEx(
    /* [in] */ const String& s,
    /* [in] */ const String& allow,
    /* [out] */ String* result)
{
    return Uri::Encode(s, allow, result);
}

ECode CUriHelper::WithAppendedPath(
    /* [in] */ IUri* baseUri,
    /* [in] */ const String& pathSegment,
    /* [out] */ IUri** result)
{
    return Uri::WithAppendedPath(baseUri, pathSegment, result);
}


} // namespace Net
} // namespace Droid
} // namespace Elastos
