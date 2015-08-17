
#include "CStringEntity.h"
#include <elastos/Logger.h>

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

CAR_INTERFACE_IMPL(CStringEntity, AbstractHttpEntity, ICloneable)

CAR_OBJECT_IMPL(CStringEntity)

ECode CStringEntity::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    VALIDATE_NOT_NULL(isRepeatable)
    *isRepeatable = TRUE;
    return NOERROR;
}

ECode CStringEntity::GetContentLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length)
    *length = mContent->GetLength();
    return NOERROR;
}

ECode CStringEntity::GetContent(
    /* [out] */ IInputStream** inputStream)
{
    VALIDATE_NOT_NULL(inputStream)
    AutoPtr<IByteArrayInputStream> stream;
    CByteArrayInputStream::New(mContent, (IByteArrayInputStream**)&stream);
    *inputStream = IInputStream::Probe(stream);
    REFCOUNT_ADD(*inputStream)
    return NOERROR;
}

ECode CStringEntity::WriteTo(
    /* [in] */ IOutputStream* outstream)
{
    if (outstream == NULL) {
        Logger::E("CStringEntity", "Output stream may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    outstream->Write(mContent);
    IFlush::Probe(outstream)->Flush();
    return NOERROR;
}

ECode CStringEntity::IsStreaming(
    /* [out] */ Boolean* isStreaming)
{
    VALIDATE_NOT_NULL(isStreaming)
    *isStreaming = FALSE;
    return NOERROR;
}

ECode CStringEntity::Clone(
    /* [out] */ IInterface** o)
{
    VALIDATE_NOT_NULL(o)
    AutoPtr<IAbstractHttpEntity> entity;
    CStringEntity::New((IAbstractHttpEntity**)&entity);
    AutoPtr<CStringEntity> entityCls = (CStringEntity*)entity.Get();
    entityCls->mContentType = mContentType;
    entityCls->mContentEncoding = mContentEncoding;
    entityCls->mChunked = mChunked;
    entityCls->mContent = mContent;
    *o = entity->Probe(EIID_IInterface);
    REFCOUNT_ADD(*o)
    return NOERROR;
}

ECode CStringEntity::constructor(
    /* [in] */ const String& s,
    /* [in] */ const String& charset)
{
    if (s.IsNull()) {
        Logger::E("CStringEntity", "Source string may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (charset.IsNull()) {
        charset = IHTTP::DEFAULT_CONTENT_CHARSET;
    }
    mContent = s.GetBytes();// mContent = s.GetBytes(charset);
    SetContentType(IHTTP::PLAIN_TEXT_TYPE + IHTTP::CHARSET_PARAM + charset);
    return NOERROR;
}

ECode CStringEntity::constructor(
    /* [in] */ const String& s)
{
    return constructor(s, String(NULL));
}

ECode CStringEntity::constructor()
{
    return NOERROR;
}

} // namespace Entity
} // namespace Http
} // namespace Apache
} // namespace Org
