
#include "CMultipartEntity.h"
#include "Part.h"
#include "CFilePartHelper.h"
#include "CStringPartHelper.h"
// #include "EncodingUtils.h"
#include <elastos/utility/logging/Logger.h>
#include <elastos/core/StringBuffer.h>
#include "elastos/droid/ext/frameworkext.h"

using namespace Elastos::Core;

using Elastos::Core::IRandom;
using Elastos::Utility::Logging::Logger;
using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CByteArrayOutputStream;
using Org::Apache::Commons::Logging::ILogFactory;
using Org::Apache::Http::Params::IHttpParams;
using Org::Apache::Http::IHeader;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

const AutoPtr<ILog> CMultipartEntity::LOG;
void CMultipartEntity::InitStaticLOG()
{
    AutoPtr<ILogFactory> factory;
    // TODO:
    // CLogFactory::AcquireSingleton((ILogFactory**)&factory);
    // factory->GetLog(String("CFilePart"), (ILog**)&LOG)
}

const String CMultipartEntity::MULTIPART_FORM_CONTENT_TYPE = String("multipart/form-data");
// TODO:
AutoPtr<ArrayOf<Byte> > CMultipartEntity::MULTIPART_CHARS/* =
    EncodingUtils::GetAsciiBytes(String("-_1234567890abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"))*/;

CMultipartEntity::CMultipartEntity()
    : mContentConsumed(FALSE)
{}

CMultipartEntity::~CMultipartEntity()
{}

ECode CMultipartEntity::constructor(
    /* [in] */ ArrayOf<IPart *>* parts,
    /* [in] */ IHttpParams* params)
{
    if (parts == NULL) {
      Logger::E(String("CMultipartEntity"), String("parts cannot be null"));
      return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (params == NULL) {
      Logger::E(String("CMultipartEntity"), String("params cannot be null"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mParts = parts;
    mParams = params;
    return NOERROR;
}

ECode CMultipartEntity::constructor(
    /* [in] */ ArrayOf<IPart *>* parts)
{
    SetContentType(MULTIPART_FORM_CONTENT_TYPE);
    if (parts == NULL) {
      Logger::E(String("CMultipartEntity"), String("parts cannot be null"));
      return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mParts = parts;
    mParams = NULL;
    return NOERROR;
}

AutoPtr<ArrayOf<Byte> > CMultipartEntity::GenerateMultipartBoundary()
{
    AutoPtr<IRandom> rand;
    CRandom::New((IRandom**)&rand);
    Int32 size;
    rand->NextInt(11, &size);
    AutoPtr<ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(size + 30); // a random size from 30 to 40
    Int32 randSize;
    for (Int32 i = 0; i < bytes->GetLength(); i++) {
        rand->NextInt(MULTIPART_CHARS->GetLength(), &randSize);
        (*bytes)[i] = (*MULTIPART_CHARS)[randSize];
    }
    return bytes;
}

AutoPtr<ArrayOf<Byte> > CMultipartEntity::GetMultipartBoundary()
{
    if (mMultipartBoundary == NULL) {
        String temp;
        if (mParams != NULL) {
            mParams->GetParameter(IMultipartEntity::MULTIPART_BOUNDARY, &temp);
        }
        if (!temp.IsNullOrEmpty()) {
            // TODO:
            // mMultipartBoundary = EncodingUtils::GetAsciiBytes(temp);
        } else {
            mMultipartBoundary = GenerateMultipartBoundary();
        }
    }
    return mMultipartBoundary;
}

ECode CMultipartEntity::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    VALIDATE_NOT_NULL(isRepeatable);

    for (Int32 i = 0; i < mParts->GetLength(); i++) {
        Boolean repeatable;
        ((*mParts)[i])->IsRepeatable(&repeatable);
        if (!repeatable) {
            *isRepeatable = FALSE;
            return NOERROR;
        }
    }

    *isRepeatable = TRUE;
    return NOERROR;
}

ECode CMultipartEntity::WriteTo(
    /* [in] */ IOutputStream* outStream)
{
    if (mParts != NULL) {
        AutoPtr<IPart> part = (*mParts)[0];
        AutoPtr<IFilePart> filePart = (IFilePart*)part->Probe(EIID_IFilePart);
        if (filePart != NULL) {
            AutoPtr<IFilePartHelper> helper;
            CFilePartHelper::AcquireSingleton((IFilePartHelper**)&helper);
            return helper->SendParts(outStream, mParts, GetMultipartBoundary());
        } else {
            AutoPtr<IStringPartHelper> helper;
            CStringPartHelper::AcquireSingleton((IStringPartHelper**)&helper);
            return helper->SendParts(outStream, mParts, GetMultipartBoundary());
        }
    }

    return NOERROR;
}

ECode CMultipartEntity::GetContentType(
    /* [out] */ IHeader** header)
{
    VALIDATE_NOT_NULL(header);

    StringBuffer* buffer = new StringBuffer(MULTIPART_FORM_CONTENT_TYPE);
    buffer->AppendCStr("; boundary=");
    buffer->AppendString(EncodingUtils::GetAsciiString(GetMultipartBoundary()));
    String bufferStr;
    buffer->ToString(&bufferStr);
    // return new CBasicHeader::New(IHTTP::CONTENT_TYPE, bufferStr, header);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::GetContentLength(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);

    ECode ec = NOERROR;
    *length = 0;

    if (mParts != NULL) {
        AutoPtr<IPart> part = (*mParts)[0];
        AutoPtr<IFilePart> filePart = (IFilePart*)part->Probe(EIID_IFilePart);
        if (filePart != NULL) {
            AutoPtr<IFilePartHelper> helper;
            CFilePartHelper::AcquireSingleton((IFilePartHelper**)&helper);
            return helper->GetLengthOfParts(mParts, GetMultipartBoundary(), length);
        } else {
            AutoPtr<IStringPartHelper> helper;
            CStringPartHelper::AcquireSingleton((IStringPartHelper**)&helper);
            return helper->GetLengthOfParts(mParts, GetMultipartBoundary(), length);
        }
    }

    if (FAILED(ec)) {
        Logger::E(String("CMultipartEntity"), String("An exception occurred while getting the length of the parts"));
    }

    return ec;
}

ECode CMultipartEntity::GetContent(
    /* [out] */ IInputStream** stream)
{
    Boolean repeatable;
    IsRepeatable(&repeatable);
    if(!repeatable && mContentConsumed ) {
        Logger::E(String("CMultipartEntity"), String("Content has been consumed"));
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    mContentConsumed = TRUE;

    AutoPtr<IByteArrayOutputStream> baos;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&baos);
    if (mParts != NULL) {
        AutoPtr<IPart> part = (*mParts)[0];
        AutoPtr<IFilePart> filePart = (IFilePart*)part->Probe(EIID_IFilePart);
        if (filePart != NULL) {
            AutoPtr<IFilePartHelper> helper;
            CFilePartHelper::AcquireSingleton((IFilePartHelper**)&helper);
            return helper->SendParts(baos, mParts, mMultipartBoundary);
        } else {
            AutoPtr<IStringPartHelper> helper;
            CStringPartHelper::AcquireSingleton((IStringPartHelper**)&helper);
            return helper->SendParts(baos, mParts, mMultipartBoundary);
        }
    }

    AutoPtr<ArrayOf<Byte> > bytes;
    baos->ToByteArray((ArrayOf<Byte>**)&bytes);
    return CByteArrayInputStream::New(bytes, (IByteArrayOutputStream**)stream);
}


ECode CMultipartEntity::IsStreaming(
    /* [out] */ Boolean* isStreaming)
{
    VALIDATE_NOT_NULL(isStreaming);
    *isStreaming = FALSE;
    return NOERROR;
}

ECode CMultipartEntity::GetContentEncoding(
    /* [out] */ IHeader** header)
{
    // return AbstractHttpEntity::GetContentEncoding(header);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::IsChunked(
    /* [out] */ Boolean* isChunked)
{
    // return AbstractHttpEntity::IsChunked(isChunked);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::SetContentType(
    /* [in] */ IHeader* contentType)
{
    // return AbstractHttpEntity::SetContentType(contentType);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::SetContentType(
    /* [in] */ const String& ctString)
{
    // return AbstractHttpEntity::SetContentType(ctString);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::SetContentEncoding(
    /* [in] */ IHeader* contentEncoding)
{
    // return AbstractHttpEntity::SetContentEncoding(contentEncoding);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::SetContentEncoding(
    /* [in] */ const String& ceString)
{
    // return AbstractHttpEntity::SetContentEncoding(ceString);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::SetChunked(
    /* [in] */ Boolean chunked)
{
    // return AbstractHttpEntity::SetChunked(chunked);
    return E_NOT_IMPLEMENTED;
}

ECode CMultipartEntity::ConsumeContent()
{
    // return AbstractHttpEntity::ConsumeContent();
    return E_NOT_IMPLEMENTED;
}

}
}
}
}
}
}

