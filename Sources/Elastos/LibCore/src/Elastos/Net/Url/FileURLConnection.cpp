
#include "cmdef.h"
#include "elastos/Math.h"
#include "FileURLConnection.h"
#include "UriCodec.h"
#include "CFile.h"
#include "CBufferedInputStream.h"
#include "CFileInputStream.h"
#include "CPrintStream.h"
#include "CByteArrayInputStream.h"
#include "CByteArrayOutputStream.h"

using Elastos::IO::CFile;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::IO::IBufferedInputStream;
using Elastos::IO::CBufferedInputStream;
using Elastos::IO::IPrintStream;
using Elastos::IO::CPrintStream;
using Elastos::IO::IByteArrayInputStream;
using Elastos::IO::CByteArrayInputStream;
using Elastos::IO::IByteArrayOutputStream;
using Elastos::IO::CByteArrayOutputStream;

namespace Elastos {
namespace Net {
namespace Url {

ECode FileURLConnection::Init(
    /* [in] */ IURL* url)
{
    mLength = -1;
    mIsDir = FALSE;

    URLConnection::Init(url);
    url->GetFile(&mFilename);
    if (mFilename.IsNull()) {
        mFilename = String("");
    }
    return UriCodec::Decode(mFilename, &mFilename);
}

ECode FileURLConnection::Connect()
{
    AutoPtr<IFile> f;
    FAIL_RETURN(CFile::New(mFilename, (IFile**)&f));
    Boolean isflag = FALSE;
    if (f->IsDirectory(&isflag), isflag) {
        mIsDir = TRUE;
        mIs = GetDirectoryListing(f);
        // use -1 for the contentLength
    }
    else {
        AutoPtr<IFileInputStream> fis;
        FAIL_RETURN(CFileInputStream::New(f, (IFileInputStream**)&fis));
        mIs = NULL;
        FAIL_RETURN(CBufferedInputStream::New(fis, (IBufferedInputStream**)&mIs));
        Int64 lengthAsLong = 0;
        f->GetLength(&lengthAsLong);
        mLength = lengthAsLong <= Elastos::Core::Math::INT32_MAX_VALUE ? (Int32) lengthAsLong : Elastos::Core::Math::INT32_MAX_VALUE;
    }
    mConnected = TRUE;
    return NOERROR;
}

ECode FileURLConnection::GetContentLength(
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length)

    // try {
    ECode ec = NOERROR;
    if (!mConnected) {
        ec = Connect();
    }

    if (ec != NOERROR)
    {
        *length = -1;
        return ec;
    }
    // } catch (IOException e) {
    //     // default is -1
    // }
    *length = mLength;
    return NOERROR;
}

ECode FileURLConnection::GetContentType(
    /* [out] */ String* type)
{
    VALIDATE_NOT_NULL(type)

    // try {
    ECode ec = NOERROR;
    if (!mConnected) {
        ec = Connect();
    }

    if (ec != NOERROR)
    {
        *type = String("content/unknown");
        return ec;
    }
    // } catch (IOException e) {
    //     return "content/unknown";
    // }
    if (mIsDir) {
        *type = String("text/plain");
    }
    String filename;
    mUrl->GetFile(&filename);
    String result = GuessContentTypeFromName(filename);
    if (result.IsNull()) {
        *type = result;
        return NOERROR;
    }

    // try {
    result = GuessContentTypeFromStream(mIs);
    // } catch (IOException e) {
    //     // Ignore
    // }
    if (result.IsNull()) {
        *type = result;
        return NOERROR;
    }

    *type = String("content/unknown");
    return NOERROR;
}

ECode FileURLConnection::GetInputStream(
    /* [out] */ IInputStream** is)
{
    VALIDATE_NOT_NULL(is)

    if (!mConnected) {
        Connect();
    }
    *is = mIs;
    INTERFACE_ADDREF(*is)
    return NOERROR;
}

// public java.security.Permission getPermission() {
//     if (permission == null) {
//         String path = filename;
//         if (File.separatorChar != '/') {
//             path = path.replace('/', File.separatorChar);
//         }
//         permission = new FilePermission(path, "read");
//     }
//     return permission;
// }

AutoPtr<IInputStream> FileURLConnection::GetDirectoryListing(
    /* [in] */ IFile* f)
{
    AutoPtr< ArrayOf<String> > fileList;
    f->List((ArrayOf<String>**)&fileList);
    AutoPtr<IByteArrayOutputStream> bytes;
    CByteArrayOutputStream::New((IByteArrayOutputStream**)&bytes);
    AutoPtr<IPrintStream> out;
    CPrintStream::New(bytes, (IPrintStream**)&out);
    out->PrintString(String("<title>Directory Listing</title>\n"));
    out->PrintString(String("<base href=\"file:"));
    String pathname;
    out->PrintString((f->GetPath(&pathname), pathname).Replace('\\', '/')
                        + String("/\"><h1>")
                        + (f->GetPath(&pathname), pathname)
                        + String("</h1>\n<hr>\n"));
    Int32 i = 0;
    for (i = 0; i < fileList->GetLength(); i++) {
        out->PrintString((*fileList)[i] + String("<br>\n"));
    }
    out->Close();
    AutoPtr<IInputStream> outinput;
    AutoPtr< ArrayOf<Byte> > outbytes;
    bytes->ToByteArray((ArrayOf<Byte>**)&outbytes);
    CByteArrayInputStream::New(outbytes, (IByteArrayInputStream**)&outinput);

    return outinput;
}

} // namespace Url
} // namespace Net
} // namespace Elastos
