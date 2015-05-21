
#include "CInputSource.h"

namespace Org {
namespace Xml {
namespace Sax {

CAR_INTERFACE_IMPL(CInputSource, Object, IInputSource)
CAR_OBJECT_IMPL(CInputSource)

ECode CInputSource::constructor()
{
    return NOERROR;
}

ECode CInputSource::constructor(
    /* [in] */ const String& systemId)
{
    return SetSystemId(systemId);
}

ECode CInputSource::constructor(
    /* [in] */ Elastos::IO::IInputStream* byteStream)
{
    return SetByteStream(byteStream);
}

ECode CInputSource::constructor(
    /* [in] */ Elastos::IO::IReader* characterStream)
{
    return SetCharacterStream(characterStream);;
}

ECode CInputSource::SetPublicId(
    /* [in] */ const String& publicId)
{
    mPublicId = publicId;
    return NOERROR;
}

ECode CInputSource::GetPublicId(
    /* [out] */ String* publicId)
{
    VALIDATE_NOT_NULL(publicId);

    *publicId = mPublicId;

    return NOERROR;
}

ECode CInputSource::SetSystemId(
    /* [in] */ const String& systemId)
{
    mSystemId = systemId;

    return NOERROR;
}

ECode CInputSource::GetSystemId(
    /* [out] */ String* systemId)
{
    VALIDATE_NOT_NULL(systemId);

    *systemId = mSystemId;

    return NOERROR;
}

ECode CInputSource::SetByteStream(
    /* [in] */ Elastos::IO::IInputStream* byteStream)
{
    mByteStream = byteStream;

    return NOERROR;
}

ECode CInputSource::GetByteStream(
    /* [out] */ Elastos::IO::IInputStream** byteStream)
{
    VALIDATE_NOT_NULL(byteStream);

    *byteStream = (mByteStream).Get();

    return NOERROR;
}

ECode CInputSource::SetEncoding(
    /* [in] */ const String& encoding)
{
    mEncoding = encoding;

    return NOERROR;
}

ECode CInputSource::GetEncoding(
    /* [out] */ String* encoding)
{
    VALIDATE_NOT_NULL(encoding);

    *encoding = mEncoding;

    return NOERROR;
}

ECode CInputSource::SetCharacterStream(
    /* [in] */ Elastos::IO::IReader* characterStream)
{
    mCharacterStream = characterStream;

    return NOERROR;
}

ECode CInputSource::GetCharacterStream(
    /* [out] */ Elastos::IO::IReader** characterStream)
{
    VALIDATE_NOT_NULL(characterStream);

    *characterStream = mCharacterStream.Get();

    return NOERROR;
}

} // namespace Sax
} // namespace Xml
} // namespace Org
