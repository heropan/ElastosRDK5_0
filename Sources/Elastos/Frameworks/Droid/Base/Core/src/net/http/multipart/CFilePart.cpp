
#include "CFilePart.h"
#include "elastos/Logger.h"
#include "ext/frameworkext.h"
// #include "EncodingUtils.h"
#include "CFilePartSource.h"

using Elastos::IO::IInputStream;
using Elastos::Utility::Logging::Logger;
using Org::Apache::Commons::Logging::ILogFactory;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

const AutoPtr<ILog> CFilePart::LOG;
void CFilePart::InitStaticLOG()
{
    AutoPtr<ILogFactory> factory;
    // TODO:
    // CLogFactory::AcquireSingleton((ILogFactory**)&factory);
    // factory->GetLog(String("CFilePart"), (ILog**)&LOG)
}

const String CFilePart::FILE_NAME = String("; filename=");
// TODO:
const AutoPtr<ArrayOf<Byte> > CFilePart::FILE_NAME_BYTES/* =
    EncodingUtils::GetAsciiBytes(FILE_NAME)*/;

ECode CFilePart::constructor(
    /* [in] */ const String& name,
    /* [in] */ IPartSource* partSource,
    /* [in] */ const String& contentType,
    /* [in] */ const String& charset)
{
    return Init(name, partSource, contentType, charset);
}

ECode CFilePart::constructor(
    /* [in] */ const String& name,
    /* [in] */ IPartSource* partSource)
{
    return Init(name, partSource, String(NULL), String(NULL));
}

ECode CFilePart::constructor(
    /* [in] */ const String& name,
    /* [in] */ Elastos::IO::IFile* file)
{
    AutoPtr<IPartSource> partSource;
    FAIL_RETURN(CFilePartSource::New(file, (IPartSource**)&partSource));
    return Init(name, partSource, String(NULL), String(NULL));
}

ECode CFilePart::constructor(
    /* [in] */ const String& name,
    /* [in] */ Elastos::IO::IFile* file,
    /* [in] */ const String& contentType,
    /* [in] */ const String& charset)
{
    AutoPtr<IPartSource> partSource;
    FAIL_RETURN(CFilePartSource::New(file, (IPartSource**)&partSource));
    return Init(name, partSource, contentType, charset);
}

ECode CFilePart::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& fileName,
    /* [in] */ Elastos::IO::IFile* file)
{
    AutoPtr<IPartSource> partSource;
    FAIL_RETURN(CFilePartSource::New(fileName, file, (IPartSource**)&partSource));
    return Init(name, partSource, String(NULL), String(NULL));
}

ECode CFilePart::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& fileName,
    /* [in] */ Elastos::IO::IFile* file,
    /* [in] */ const String& contentType,
    /* [in] */ const String& charset)
{
    AutoPtr<IPartSource> partSource;
    FAIL_RETURN(CFilePartSource::New(fileName, file, (IPartSource**)&partSource));
    return Init(name, partSource, contentType, charset);
}

ECode CFilePart::Init(
    /* [in] */ const String& name,
    /* [in] */ IPartSource* partSource,
    /* [in] */ const String& contentType,
    /* [in] */ const String& charset)
{
    PartBase::Init(name,
        contentType.IsNullOrEmpty() ? DEFAULT_CONTENT_TYPE : contentType,
        charset.IsNullOrEmpty() ? String("ISO-8859-1") : charset,
        DEFAULT_TRANSFER_ENCODING);

    if (partSource == NULL) {
        Logger::E(String("CFilePart"), String("partSource may not be null"));
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mSource = partSource;
    return NOERROR;
}

ECode CFilePart::SendData(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendData(OutputStream out)"));
    Int64 length;
    LengthOfData(&length);
    if (length == 0) {

        // this file contains no data, so there is nothing to send.
        // we don't want to create a zero length buffer as this will
        // cause an infinite loop when reading.
        // LOG->Debug(String("No data to send."));
        return NOERROR;
    }

    AutoPtr<ArrayOf<Byte> > tmp = ArrayOf<Byte>::Alloc(4096);
    AutoPtr<IInputStream> inStream;
    mSource->CreateInputStream((IInputStream**)&inStream);

    Int32 len;
    ECode ec;
    while ((ec = inStream->ReadBytes(tmp, &len)) == NOERROR && len >= 0) {
        ec = outStream->WriteBytesEx(*tmp, 0, len);
        if(ec != NOERROR) {
            break;
        }
    }

    // we're done with the stream, close it
    inStream->Close();

    return NOERROR;
}

ECode CFilePart::GetSource(
    /* [out] */ IPartSource** partSource)
{
    // LOG->Trace(String("enter getSource()"));

    VALIDATE_NOT_NULL(partSource);
    *partSource = mSource;
    INTERFACE_ADDREF(*partSource);
    return NOERROR;
}

ECode CFilePart::LengthOfData(
    /* [out] */ Int64* length)
{
    // LOG->Trace(String("enter lengthOfData()"));

    VALIDATE_NOT_NULL(length);
    return mSource->GetLength(length);
}

ECode CFilePart::GetName(
    /* [out] */ String* name)
{
    return PartBase::GetName(name);
}

ECode CFilePart::GetContentType(
    /* [out] */ String* type)
{
    return PartBase::GetContentType(type);
}

ECode CFilePart::GetCharSet(
    /* [out] */ String* charset)
{
    return PartBase::GetCharSet(charset);
}

ECode CFilePart::GetTransferEncoding(
    /* [out] */ String* transferEncoding)
{
    return PartBase::GetTransferEncoding(transferEncoding);
}

ECode CFilePart::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    return PartBase::IsRepeatable(isRepeatable);
}

ECode CFilePart::Send(
    /* [in] */ Elastos::IO::IOutputStream* outStream)
{
    return PartBase::Send(outStream);
}

ECode CFilePart::Length(
    /* [out] */ Int64* length)
{
    return PartBase::Length(length);
}

ECode CFilePart::ToString(
    /* [out] */ String* str)
{
    return PartBase::ToString(str);
}

ECode CFilePart::SetCharSet(
    /* [in] */ const String& charSet)
{
    return PartBase::SetCharSet(charSet);
}

ECode CFilePart::SetContentType(
    /* [in] */ const String& contentType)
{
    return PartBase::SetContentType(contentType);
}

ECode CFilePart::SetName(
    /* [in] */ const String& name)
{
    return PartBase::SetName(name);
}

ECode CFilePart::SetTransferEncoding(
    /* [in] */ const String& transferEncoding)
{
    return PartBase::SetTransferEncoding(transferEncoding);
}

}
}
}
}
}
}

