
#include "EntityDeserializer.h"
#include "CBasicHttpEntity.h"
#include "io/ContentLengthInputStream.h"
#include "io/ChunkedInputStream.h"
#include "io/IdentityInputStream.h"
#include "io/ChunkedInputStream.h"
#include "Logger.h"

using Elastos::IO::IInputStream;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Http::IHeader;
using Org::Apache::Http::Entity::IAbstractHttpEntity;
using Org::Apache::Http::Entity::CBasicHttpEntity;
using Org::Apache::Http::Impl::IO::ChunkedInputStream;
using Org::Apache::Http::Impl::IO::IdentityInputStream;
using Org::Apache::Http::Impl::IO::ChunkedInputStream;
using Org::Apache::Http::Impl::IO::ContentLengthInputStream;
using Org::Apache::Http::Protocol::IHTTP;

namespace Org {
namespace Apache {
namespace Http {
namespace Impl {
namespace Entity {

EntityDeserializer::EntityDeserializer(
    /* [in] */ IContentLengthStrategy* lenStrategy)
{
    if (lenStrategy == NULL) {
        Logger::E("EntityDeserializer", "Content length strategy may not be null");
        assert(0);
        // throw new IllegalArgumentException("Content length strategy may not be null");
    }
    mLenStrategy = lenStrategy;
}

ECode EntityDeserializer::DoDeserialize(
    /* [in] */ ISessionInputBuffer* inbuffer,
    /* [in] */ IHttpMessage* message,
    /* [out] */ IBasicHttpEntity** _entity)
{
    VALIDATE_NOT_NULL(_entity)
    *_entity = NULL;

    AutoPtr<IBasicHttpEntity> entity;
    CBasicHttpEntity::New((IBasicHttpEntity**)&entity);

    Int64 len;
    mLenStrategy->DetermineLength(message, &len);
    if (len == IContentLengthStrategy::CHUNKED) {
        IAbstractHttpEntity::Probe(entity)->SetChunked(TRUE);
        entity->SetContentLength(-1);
        AutoPtr<IInputStream> input = (IInputStream*)new ChunkedInputStream(inbuffer);
        entity->SetContent(input);
    }
    else if (len == IContentLengthStrategy::IDENTITY) {
        IAbstractHttpEntity::Probe(entity)->SetChunked(FALSE);
        entity->SetContentLength(-1);
        AutoPtr<IInputStream> input = (IInputStream*)new IdentityInputStream(inbuffer);
        entity->SetContent(input);
    }
    else {
        IAbstractHttpEntity::Probe(entity)->SetChunked(FALSE);
        entity->SetContentLength(len);
        AutoPtr<IInputStream> input = (IInputStream*)new ContentLengthInputStream(inbuffer, len);
        entity->SetContent(input);
    }

    AutoPtr<IHeader> contentTypeHeader;
    message->GetFirstHeader(IHTTP::CONTENT_TYPE, (IHeader**)&contentTypeHeader);
    if (contentTypeHeader != NULL) {
        IAbstractHttpEntity::Probe(entity)->SetContentType(contentTypeHeader);
    }
    AutoPtr<IHeader> contentEncodingHeader;
    message->GetFirstHeader(IHTTP::CONTENT_ENCODING, (IHeader**)&contentEncodingHeader);
    if (contentEncodingHeader != NULL) {
        IAbstractHttpEntity::Probe(entity)->SetContentEncoding(contentEncodingHeader);
    }
    *_entity = entity;
    REFCOUNT_ADD(*_entity)
    return NOERROR;
}

ECode EntityDeserializer::Deserialize(
    /* [in] */ ISessionInputBuffer* inbuffer,
    /* [in] */ IHttpMessage* message,
    /* [out] */ IHttpEntity** entity)
{
    VALIDATE_NOT_NULL(entity)
    *entity = NULL;
    if (inbuffer == NULL) {
        Logger::E("EntityDeserializer", "Session input buffer may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("Session input buffer may not be null");
    }
    if (message == NULL) {
        Logger::E("EntityDeserializer", "HTTP message may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
        // throw new IllegalArgumentException("Session input buffer may not be null");
    }
    AutoPtr<IBasicHttpEntity> basicEntity;
    DoDeserialize(inbuffer, message, (IBasicHttpEntity**)&basicEntity);
    *entity = IHttpEntity::Probe(basicEntity);
    REFCOUNT_ADD(*entity)
    return NOERROR;
}

} // namespace Entity
} // namespace Impl
} // namespace Http
} // namespace Apache
} // namespace Org