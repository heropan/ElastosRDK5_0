
#include "CStringPart.h"
// #include "EncodingUtils.h"
#include <elastos/utility/logging/Logger.h>
#include "ext/frameworkext.h"

using Elastos::Utility::Logging::Logger;
using Org::Apache::Commons::Logging::ILogFactory;

namespace Elastos {
namespace Droid {
namespace Net {
namespace Internal {
namespace Http {
namespace Multipart {

const AutoPtr<ILog> CStringPart::LOG;
void CStringPart::InitStaticLOG()
{
    AutoPtr<ILogFactory> factory;
    // TODO:
    // CLogFactory::AcquireSingleton((ILogFactory**)&factory);
    // factory->GetLog(String("CFilePart"), (ILog**)&LOG)
}

ECode CStringPart::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& value,
    /* [in] */ const String& charset)
{
    FAIL_RETURN(PartBase::Init(name,
        DEFAULT_CONTENT_TYPE,
        charset.IsNullOrEmpty() ? IStringPart::DEFAULT_CHARSET : charset,
        IStringPart::DEFAULT_TRANSFER_ENCODING));

    if (value == NULL) {
        Logger::E("CStringPart", "Value may not be null");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }
    if (value.ToByteIndex(0) != -1) {
        // See RFC 2048, 2.8. "8bit Data"
        Logger::E("CStringPart", "NULs may not be present in string parts");
        return E_ILLEGAL_ARGUMENT_EXCEPTION;
    }

    mValue = value;
    return NOERROR;
}

ECode CStringPart::constructor(
    /* [in] */ const String& name,
    /* [in] */ const String& value)
{
    return constructor(name, value, String(NULL));
}

AutoPtr<ArrayOf<Byte> > CStringPart::GetContent()
{
    if (mContent == NULL) {
        String charset;
        GetCharSet(&charset);
        // TODO:
        // mContent = EncodingUtils::GetBytes(value, charset);
    }
    return mContent;
}

ECode CStringPart::SendData(
    /* [in] */ IOutputStream* outStream)
{
    // LOG->Trace(String("enter sendData(OutputStream)"));
    AutoPtr<ArrayOf<Byte> > bytes = GetContent();
    return outStream->WriteBytes(*bytes);
}

ECode CStringPart::LengthOfData(
    /* [out] */ Int64* length)
{
    // LOG->Trace(String("enter lengthOfData()"));
    return GetContent()->GetLength();
}

ECode CStringPart::SetCharSet(
    /* [in] */ const String& charSet)
{
    PartBase::SetCharSet(charSet);
    mContent = NULL;
    return NOERROR;
}

ECode CStringPart::GetName(
    /* [out] */ String* name)
{
    return PartBase::GetName(name);
}

ECode CStringPart::GetContentType(
    /* [out] */ String* type)
{
    return PartBase::GetContentType(type);
}

ECode CStringPart::GetCharSet(
    /* [out] */ String* charset)
{
    return PartBase::GetCharSet(charset);
}

ECode CStringPart::GetTransferEncoding(
    /* [out] */ String* transferEncoding)
{
    return PartBase::GetTransferEncoding(transferEncoding);
}

ECode CStringPart::IsRepeatable(
    /* [out] */ Boolean* isRepeatable)
{
    return PartBase::IsRepeatable(isRepeatable);
}

ECode CStringPart::Send(
    /* [in] */ Elastos::IO::IOutputStream* outStream)
{
    return PartBase::Send(outStream);
}

ECode CStringPart::Length(
    /* [out] */ Int64* length)
{
    return PartBase::Length(length);
}

ECode CStringPart::ToString(
    /* [out] */ String* str)
{
    return PartBase::ToString(str);
}

ECode CStringPart::SetContentType(
    /* [in] */ const String& contentType)
{
    return PartBase::SetContentType(contentType);
}

ECode CStringPart::SetName(
    /* [in] */ const String& name)
{
    return PartBase::SetName(name);
}

ECode CStringPart::SetTransferEncoding(
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

