
#include "Math.h"
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
using Elastos::IO::ICloseable;

namespace Libcore {
namespace Net {
namespace Url {

CAR_INTERFACE_IMPL(FileURLConnection, URLConnection, IFileURLConnection)

ECode FileURLConnection::constructor(
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
    //TODO
    assert(0);
    // File f = new File(filename);
    // IOException error = null;
    // if (f.isDirectory()) {
    //     isDir = true;
    //     is = getDirectoryListing(f);
    //     // use -1 for the contentLength
    //     lastModified = f.lastModified();
    //     headerKeysAndValues[CONTENT_TYPE_VALUE_IDX] = "text/html";
    // } else {
    //     try {
    //         is = new BufferedInputStream(new FileInputStream(f));
    //     } catch (IOException ioe) {
    //         error = ioe;
    //     }

    //     if (error == null) {
    //         length = f.length();
    //         lastModified = f.lastModified();
    //         headerKeysAndValues[CONTENT_TYPE_VALUE_IDX] = getContentTypeForPlainFiles();
    //     } else {
    //         headerKeysAndValues[CONTENT_TYPE_VALUE_IDX] = "content/unknown";
    //     }
    // }

    // headerKeysAndValues[CONTENT_LENGTH_VALUE_IDX] = String.valueOf(length);
    // headerKeysAndValues[LAST_MODIFIED_VALUE_IDX] = String.valueOf(lastModified);

    // connected = true;
    // if (error != null) {
    //     throw error;
    // }
    return NOERROR;
}

ECode FileURLConnection::GetHeaderField(
    /* [in] */ const String& key,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    //TODO
    assert(0);
    // if (!connected) {
    //     try {
    //         connect();
    //     } catch (IOException ioe) {
    //         return null;
    //     }
    // }

    // for (int i = 0; i < headerKeysAndValues.length; i += 2) {
    //     if (headerKeysAndValues[i].equalsIgnoreCase(key)) {
    //         return headerKeysAndValues[i + 1];
    //     }
    // }

    // return null;

    return NOERROR;
}

ECode FileURLConnection::GetHeaderFieldKey(
    /* [in] */ Int32 posn,
    /* [out] */ String* key)
{
    VALIDATE_NOT_NULL(key);
    //TODO
    assert(0);
    // if (!connected) {
    //     try {
    //         connect();
    //     } catch (IOException ioe) {
    //         return null;
    //     }
    // }

    // if (position < 0 || position > headerKeysAndValues.length / 2) {
    //     return null;
    // }

    // return headerKeysAndValues[position * 2];

    return NOERROR;
}

ECode FileURLConnection::GetHeaderField(
    /* [in] */ Int32 pos,
    /* [out] */ String* value)
{
    VALIDATE_NOT_NULL(value);
    //TODO
    assert(0);

    // if (!connected) {
    //     try {
    //         connect();
    //     } catch (IOException ioe) {
    //         return null;
    //     }
    // }

    // if (position < 0 || position > headerKeysAndValues.length / 2) {
    //     return null;
    // }

    // return headerKeysAndValues[(position * 2) + 1];

    return NOERROR;
}

ECode FileURLConnection::GetHeaderFields(
    /* [out] */ IMap** headerFields)
{
    VALIDATE_NOT_NULL(headerFields);
    //TODO
    assert(0);

    // if (headerFields == null) {
    //     final TreeMap<String, List<String>> headerFieldsMap = new TreeMap<>(HEADER_COMPARATOR);

    //     for (int i = 0; i < headerKeysAndValues.length; i+=2) {
    //         headerFieldsMap.put(headerKeysAndValues[i],
    //                 Collections.singletonList(headerKeysAndValues[i + 1]));
    //     }

    //     headerFields = Collections.unmodifiableMap(headerFieldsMap);
    // }

    // return headerFields;

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
    REFCOUNT_ADD(*is)
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
    CPrintStream::New(IOutputStream::Probe(bytes), (IPrintStream**)&out);
    out->Print(String("<title>Directory Listing</title>\n"));
    out->Print(String("<base href=\"file:"));
    String pathname;
    out->Print((f->GetPath(&pathname), pathname).Replace('\\', '/')
                        + String("/\"><h1>")
                        + (f->GetPath(&pathname), pathname)
                        + String("</h1>\n<hr>\n"));
    Int32 i = 0;
    for (i = 0; i < fileList->GetLength(); i++) {
        out->Print((*fileList)[i] + String("<br>\n"));
    }
    ICloseable::Probe(out)->Close();
    AutoPtr<IInputStream> outinput;
    AutoPtr< ArrayOf<Byte> > outbytes;
    bytes->ToByteArray((ArrayOf<Byte>**)&outbytes);
    CByteArrayInputStream::New(outbytes, (IByteArrayInputStream**)&outinput);

    return outinput;
}

String FileURLConnection::GetContentTypeForPlainFiles()
{
    //TODO
    assert(0);
    // String result = guessContentTypeFromName(url.getFile());
    // if (result != null) {
    //     return result;
    // }

    // try {
    //     result = guessContentTypeFromStream(is);
    // } catch (IOException e) {
    //     // Ignore
    // }
    // if (result != null) {
    //     return result;
    // }

    // return "content/unknown";
    return String(NULL);
}

} // namespace Url
} // namespace Net
} // namespace Libcore
