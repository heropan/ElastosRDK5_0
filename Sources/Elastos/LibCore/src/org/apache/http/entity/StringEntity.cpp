
#include "StringEntity.h"
#include "Logger.h"

using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IFlush;
using Elastos::IO::ICloseable;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::Protocol::IHTTP;

namespace Org {
namespace Apache {
namespace Http {
namespace Entity {

CAR_INTERFACE_IMPL(StringEntity, AbstractHttpEntity, ICloneable)

ECode StringEntity::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    VALIDATE_NOT_NULL(isRepeatable)
    *isRepeatable = TRUE;
    return NOERROR;
}

ECode StringEntity::GetContentLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mContent->GetLength();
    return NOERROR;
}

ECode StringEntity::GetContent(
    /* [out] */ IInputStream** inputStream)
{
    VALIDATE_NOT_NULL(inputStream)
    AutoPtr<IByteArrayInputStream> stream;
    CByteArrayInputStream::New(mContent, (IByteArrayInputStream**)&stream);
    *inputStream = IInputStream::Probe(stream);
    REFCOUNT_ADD(*inputStream)
    return NOERROR;
}

ECode StringEntity::WriteTo(
    /* [in] */ IOutputStream* outstream)
{
    if (outstream == NULL) {
        Logger::E("StringEntity", "Output stream may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    outstream->Write(mContent);
    IFlush::Probe(outstream)->Flush();
    return NOERROR;
}

ECode StringEntity::IsStreaming(
    /* [out] */ Boolean* isStreaming)
{
    VALIDATE_NOT_NULL(isStreaming)
    *isStreaming = FALSE;
    return NOERROR;
}

void StringEntity::CloneImpl(
    /* [in] */ StringEntity* o)
{
    o->mContentType = mContentType;
    o->mContentEncoding = mContentEncoding;
    o->mChunked = mChunked;
    o->mContent = mContent->Clone();
}

ECode StringEntity::Init(
    /* [in] */ const String& s,
    /* [in] */ const String& charset)
{
    if (s.IsNull()) {
        Logger::E("StringEntity", "Source string may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (charset.IsNull()) {
        charset = IHTTP::DEFAULT_CONTENT_CHARSET;
    }
    mContent = s.GetBytes();// mContent = s.GetBytes(charset);
    SetContentType(IHTTP::PLAIN_TEXT_TYPE + IHTTP::CHARSET_PARAM + charset);
    return NOERROR;
}

ECode StringEntity::Init(
    /* [in] */ const String& s)
{
    return Init(s, String(NULL));
}

} // namespace Entity
} // namespace Http
} // namespace Apache
} // namespace Org
