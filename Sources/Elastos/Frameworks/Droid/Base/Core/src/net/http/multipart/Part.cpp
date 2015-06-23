
#include "Part.h"
#include "ext/frameworkext.h"
// #include "EncodingUtils.h"
#include <elastos/utility/logging/Logger.h>

using Elastos::IO::IByteArrayOutputStream;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Commons::Logging::ILogFactory;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

const AutoPtr<ILog> Part::LOG;
void Part::InitStaticLOG()
{
    AutoPtr<ILogFactory> factory;
    // TODO:
    // CLogFactory::AcquireSingleton((ILogFactory**)&factory);
    // factory->GetLog(String("Part"), (ILog**)&LOG)
}

const String Part::BOUNDARY = String("----------------314159265358979323846");
// TODO:
const AutoPtr<ArrayOf<Byte> > Part::BOUNDARY_BYTES/* =
    EncodingUtils::GetAsciiBytes(BOUNDARY)*/;
const AutoPtr<ArrayOf<Byte> > Part::DEFAULT_BOUNDARY_BYTES = BOUNDARY_BYTES;
const String Part::CRLF = String("\r\n");
const AutoPtr<ArrayOf<Byte> > Part::CRLF_BYTES/* =
    EncodingUtils::GetAsciiBytes(CRLF)*/;
const String Part::QUOTE = String("\"");
const AutoPtr<ArrayOf<Byte> > Part::QUOTE_BYTES/* =
    EncodingUtils::GetAsciiBytes(QUOTE)*/;
const String Part::EXTRA = String("--");
const AutoPtr<ArrayOf<Byte> > Part::EXTRA_BYTES/* =
    EncodingUtils::GetAsciiBytes(EXTRA)*/;
const String Part::CONTENT_DISPOSITION = String("Content-Disposition: form-data; name=");
const AutoPtr<ArrayOf<Byte> > Part::CONTENT_DISPOSITION_BYTES/* =
    EncodingUtils::GetAsciiBytes(CONTENT_DISPOSITION)*/;
const String Part::CONTENT_TYPE = String("Content-Type: ");
const AutoPtr<ArrayOf<Byte> > Part::CONTENT_TYPE_BYTES/* =
    EncodingUtils::GetAsciiBytes(CONTENT_TYPE)*/;
const String Part::CHARSET = String("; charset=");
const AutoPtr<ArrayOf<Byte> > Part::CHARSET_BYTES/* =
    EncodingUtils::GetAsciiBytes(CHARSET)*/;
const String Part::CONTENT_TRANSFER_ENCODING = String("Content-Transfer-Encoding: ");
const AutoPtr<ArrayOf<Byte> > Part::CONTENT_TRANSFER_ENCODING_BYTES/* =
    EncodingUtils::GetAsciiBytes(CONTENT_TRANSFER_ENCODING)*/;

ECode Part::GetBoundary(
        /* [out] */ String* boundary)
{
    VALIDATE_NOT_NULL(boundary);
    *boundary = BOUNDARY;
    return NOERROR;
}

AutoPtr<ArrayOf<Byte> > Part::GetPartBoundary()
{
    if (mBoundaryBytes == NULL) {
        // custom boundary bytes have not been set, use the default.
        return DEFAULT_BOUNDARY_BYTES;
    }

    return mBoundaryBytes;
}

ECode Part::SetPartBoundary(
    /* [in] */ ArrayOf<Byte>* boundaryBytes)
{
    mBoundaryBytes = boundaryBytes;
    return NOERROR;
}

ECode Part::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    VALIDATE_NOT_NULL(isRepeatable);
    *isRepeatable = TRUE;
    return NOERROR;
}

ECode Part::SendStart(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendStart(OutputStream out)"));
    FAIL_RETURN(outStream->WriteBytes(*EXTRA_BYTES));
    FAIL_RETURN(outStream->WriteBytes(*(GetPartBoundary())));
    FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));

    return NOERROR;
}

ECode Part::SendDispositionHeader(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendDispositionHeader(OutputStream out)"));
    FAIL_RETURN(outStream->WriteBytes(*CONTENT_DISPOSITION_BYTES));
    FAIL_RETURN(outStream->WriteBytes(*QUOTE_BYTES));
    String name;
    GetName(&name);
    // TODO:
    // FAIL_RETURN(outStream->WriteBytes(EncodingUtils::GetAsciiBytes(name)));
    FAIL_RETURN(outStream->WriteBytes(*QUOTE_BYTES));

    return NOERROR;
}

ECode Part::SendContentTypeHeader(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendContentTypeHeader(OutputStream out)"));
    String contentType;
    GetContentType(&contentType);
    if (contentType != NULL) {
        FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));
        FAIL_RETURN(outStream->WriteBytes(*CONTENT_TYPE_BYTES));
        // TODO:
        // FAIL_RETURN(outStream->WriteBytes(EncodingUtils::GetAsciiBytes(contentType)));
        String charSet;
        GetCharSet(&charSet);
        if (charSet != NULL) {
            FAIL_RETURN(outStream->WriteBytes(*CHARSET_BYTES));
            // TODO:
            // FAIL_RETURN(outStream->WriteBytes(EncodingUtils::GetAsciiBytes(charSet)));
        }
    }

    return NOERROR;
}

ECode Part::SendTransferEncodingHeader(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendTransferEncodingHeader(OutputStream out)"));
    String transferEncoding;
    GetTransferEncoding(&transferEncoding);
    if (transferEncoding != NULL) {
        FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));
        FAIL_RETURN(outStream->WriteBytes(*CONTENT_TRANSFER_ENCODING_BYTES));
        // TODO:
        // FAIL_RETURN(outStream->WriteBytes(EncodingUtils::GetAsciiBytes(transferEncoding)));
    }

    return NOERROR;
}

ECode Part::SendEndOfHeader(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendEndOfHeader(OutputStream out)"));
    FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));
    FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));

    return NOERROR;
}

ECode Part::SendEnd(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Tace(String("enter sendEnd(OutputStream out)"));
    FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));

    return NOERROR;
}

ECode Part::Send(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter send(OutputStream out)"));
    FAIL_RETURN(SendStart(outStream));
    FAIL_RETURN(SendDispositionHeader(outStream));
    FAIL_RETURN(SendContentTypeHeader(outStream));
    FAIL_RETURN(SendTransferEncodingHeader(outStream));
    FAIL_RETURN(SendEndOfHeader(outStream));
    FAIL_RETURN(SendData(outStream));
    FAIL_RETURN(SendEnd(outStream));

    return NOERROR;
}

ECode Part::Length(
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);

    // LOG->Trace(String("enter length()"));
    FAIL_RETURN(LengthOfData(length));
    if (length < 0) {
        *length = -1;
        return NOERROR;
    }

    AutoPtr<IByteArrayOutputStream> overhead;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&overhead);
    FAIL_RETURN(SendStart(overhead));
    FAIL_RETURN(SendDispositionHeader(overhead));
    FAIL_RETURN(SendContentTypeHeader(overhead));
    FAIL_RETURN(SendTransferEncodingHeader(overhead));
    FAIL_RETURN(SendEndOfHeader(overhead));
    FAIL_RETURN(SendEnd(overhead));

    FAIL_RETURN(LengthOfData(length));
    Int32 ovSize;
    overhead->GetSize(&ovSize);
    *length += ovSize;

    return NOERROR;
}

ECode Part::ToString(
     /* [out] */ String* str)
{
    return GetName(str);
}

ECode Part::SendParts(
    /* [in] */ IOutputStream* outStream,
    /* [in] */ ArrayOf<Part*>* parts)
{
    return SendParts(outStream, parts, DEFAULT_BOUNDARY_BYTES);
}

ECode Part::SendParts(
    /* [in] */ IOutputStream* outStream,
    /* [in] */ ArrayOf<Part*>* parts,
    /* [in] */ ArrayOf<Byte>* partBoundary)
{
    if (parts == NULL) {
        Logger::E(String("Part"), String("Parts may not be null"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (partBoundary == NULL || partBoundary->GetLength() == 0) {
        Logger::E(String("Part"), String("partBoundary may not be empty"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    for (Int32 i = 0; i < parts->GetLength(); i++) {
        // set the part boundary before the part is sent
        FAIL_RETURN(((*parts)[i])->SetPartBoundary(partBoundary));
        FAIL_RETURN(((*parts)[i])->Send(outStream));
    }
    FAIL_RETURN(outStream->WriteBytes(*EXTRA_BYTES));
    FAIL_RETURN(outStream->WriteBytes(*partBoundary));
    FAIL_RETURN(outStream->WriteBytes(*EXTRA_BYTES));
    FAIL_RETURN(outStream->WriteBytes(*CRLF_BYTES));

    return NOERROR;
}

ECode Part::GetLengthOfParts(
    /* [in] */ ArrayOf<Part*>* parts,
    /* [out] */ Int64* length)
{
    return GetLengthOfParts(parts, DEFAULT_BOUNDARY_BYTES, length);
}

ECode Part::GetLengthOfParts(
    /* [in] */ ArrayOf<Part*>* parts,
    /* [in] */ ArrayOf<Byte>* partBoundary,
    /* [out] */ Int64* length)
{
    VALIDATE_NOT_NULL(length);

    // LOG->Trace(String("getLengthOfParts(Parts[])"));
    if (parts == NULL) {
        Logger::E(String("Part"), String("Parts may not be null"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    Int64 total = 0;
    for (Int32 i = 0; i < parts->GetLength(); i++) {
        // set the part boundary before we calculate the part's length
        FAIL_RETURN(((*parts)[i])->SetPartBoundary(partBoundary));
        Int64 l;
        FAIL_RETURN(((*parts)[i])->Length(&l));
        if (l < 0) {
            *length = -1;
            return NOERROR;
        }
        total += l;
    }
    total += EXTRA_BYTES->GetLength();
    total += partBoundary->GetLength();
    total += EXTRA_BYTES->GetLength();
    total += CRLF_BYTES->GetLength();
    *length = total;

    return NOERROR;
}

}
}
}
}
}
}
