
#include "provider/CMediaStoreImagesMedia.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include "content/CContentUris.h"
#include "content/CContentValues.h"
#include "graphics/CBitmapFactory.h"
#include "graphics/CMatrix.h"
#include "provider/CMediaStoreImagesThumbnails.h"

using Elastos::IO::IInputStream;
using Elastos::IO::IOutputStream;
using Elastos::IO::IFileInputStream;
using Elastos::IO::CFileInputStream;
using Elastos::Core::CInteger32;
using Elastos::Core::ICharSequence;
using Elastos::Core::CStringWrapper;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Content::IContentValues;
using Elastos::Droid::Content::CContentValues;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::CContentUris;

namespace Elastos {
namespace Droid {
namespace Provider {

ECode CMediaStoreImagesMedia::GetINTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("internal"), uri);
}

ECode CMediaStoreImagesMedia::GetEXTERNAL_CONTENT_URI(
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);
    return GetContentUri(String("external"), uri);
}

ECode CMediaStoreImagesMedia::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [out] */ ICursor** cursor)
{
   return cr->Query(uri, projection, String(NULL), NULL, IMediaStoreImagesMedia::DEFAULT_SORT_ORDER, cursor);
}

ECode CMediaStoreImagesMedia::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& where,
    /* [in] */ const String& orderBy,
    /* [out] */ ICursor** cursor)
{
    return cr->Query(uri, projection, where,
         NULL, orderBy == String(NULL) ? IMediaStoreImagesMedia::DEFAULT_SORT_ORDER : orderBy, cursor);
}

ECode CMediaStoreImagesMedia::Query(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* uri,
    /* [in] */ ArrayOf<String>* projection,
    /* [in] */ const String& selection,
    /* [in] */ ArrayOf<String>* selectionArgs,
    /* [in] */ const String& orderBy,
    /* [out] */ ICursor** cursor)
{
    return cr->Query(uri, projection, selection,
            selectionArgs, orderBy == String(NULL) ? IMediaStoreImagesMedia::DEFAULT_SORT_ORDER : orderBy, cursor);
}

ECode CMediaStoreImagesMedia::GetBitmap(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IUri* url,
    /* [out] */ IBitmap** bitmap)
{
    AutoPtr<IInputStream> input;
    cr->OpenInputStream(url, (IInputStream**)&input);

    AutoPtr<IBitmapFactory> factory;
    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
        (IBitmapFactory**)&factory));

    factory->DecodeStream(input, (IBitmap**)&bitmap);

    input->Close();
    return NOERROR;
}

ECode CMediaStoreImagesMedia::InsertImage(
    /* [in] */ IContentResolver* cr,
    /* [in] */ const String& imagePath,
    /* [in] */ const String& name,
    /* [in] */ const String& description,
    /* [out] */ String* url)
{
    // Check if file exists with a FileInputStream
    AutoPtr<IFileInputStream> stream;
    CFileInputStream::New(imagePath, (IFileInputStream**)&stream);
    //try {

        AutoPtr<IBitmapFactory> factory;
        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
            (IBitmapFactory**)&factory));
        AutoPtr<IBitmap> bm;
        factory->DecodeFile(imagePath, (IBitmap**)&bm);

        InsertImage(cr, bm, name, description, url);
        bm->Recycle();

    //} finally {
        //try {
            stream->Close();
        /*} catch (IOException e) {
        }*/
    //}

    return NOERROR;
}

AutoPtr<IBitmap> CMediaStoreImagesMedia::StoreThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IBitmap* source,
    /* [in] */ Int64 id,
    /* [in] */ Float width,
    /* [in] */ Float height,
    /* [in] */ Int32 kind)
{
    // create the matrix to scale it
    AutoPtr<IMatrix> matrix;
    CMatrix::New((IMatrix**)&matrix);

    Int32 w, h;
    source->GetWidth(&w);
    source->GetHeight(&h);

    Float scaleX = width / w;
    Float scaleY = height / h;

    matrix->SetScale(scaleX, scaleY);

    AutoPtr<IBitmapFactory> factory;
    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory));

    AutoPtr<IBitmap> thumb;
    factory->CreateBitmap(source, 0, 0, w, h, matrix, TRUE, (IBitmap**)&thumb);

    AutoPtr<IContentValues> values;
    CContentValues::New(4, (IContentValues**)&values);
    AutoPtr<IInteger32> iKind, iId, iHeight, iWidth;
    CInteger32::New(kind, (IInteger32**)&iKind);
    values->PutInt32(IMediaStoreImagesThumbnails::KIND,     iKind);
    CInteger32::New((Int32)id, (IInteger32**)&iId);
    values->PutInt32(IMediaStoreImagesThumbnails::IMAGE_ID, iId);
    CInteger32::New((thumb->GetHeight(&h), h), (IInteger32**)&iHeight);
    values->PutInt32(IMediaStoreImagesThumbnails::HEIGHT,   iHeight);
    CInteger32::New((thumb->GetWidth(&w), w), (IInteger32**)&iWidth);
    values->PutInt32(IMediaStoreImagesThumbnails::WIDTH,    iWidth);

    AutoPtr<IUri> url;
    AutoPtr<IMediaStoreImagesThumbnails> helper;
    CMediaStoreImagesThumbnails::AcquireSingleton((IMediaStoreImagesThumbnails**)&helper);
    AutoPtr<IUri> uri;
    helper->GetEXTERNAL_CONTENT_URI((IUri**)&uri);
    cr->Insert(uri, values, (IUri**)&url);

    //try {
        AutoPtr<IOutputStream> thumbOut;
        cr->OpenOutputStream(url, (IOutputStream**)&thumbOut);

        Boolean res;
        thumb->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_JPEG, 100, thumbOut, &res);
        thumbOut->Close();
        return thumb;
    /*}
    catch (FileNotFoundException ex) {
        return NULL;
    }
    catch (IOException ex) {
        return NULL;
    }*/
}

ECode CMediaStoreImagesMedia::InsertImage(
    /* [in] */ IContentResolver* cr,
    /* [in] */ IBitmap* source,
    /* [in] */ const String& title,
    /* [in] */ const String& description,
    /* [out] */ String* urlvalue)
{
    AutoPtr<IContentValues> values;
    CContentValues::New((IContentValues**)&values);

    AutoPtr<ICharSequence> csTitle;
    CStringWrapper::New(title, (ICharSequence**)&csTitle);
    values->PutString(IMediaStoreImagesMedia::TITLE, csTitle);
    AutoPtr<ICharSequence> csDescription;
    CStringWrapper::New(description, (ICharSequence**)&csDescription);
    values->PutString(IMediaStoreImagesMedia::DESCRIPTION, csDescription);
    AutoPtr<ICharSequence> csImage;
    CStringWrapper::New(String("image/jpeg"), (ICharSequence**)&csImage);
    values->PutString(IMediaStoreImagesMedia::MIME_TYPE, csImage);

    AutoPtr<IUri> url;
    String stringUrl;    /* value to be returned */

    //try {
        AutoPtr<IUri> uri;
        GetEXTERNAL_CONTENT_URI((IUri**)&uri);
        cr->Insert(uri, values, (IUri**)&url);

        if (source != NULL) {
            AutoPtr<IOutputStream> imageOut;
            cr->OpenOutputStream(url, (IOutputStream**)&imageOut);
            //try {
                Boolean res;
                source->Compress(Elastos::Droid::Graphics::BitmapCompressFormat_JPEG, 50, imageOut, &res);
            //} finally {
                imageOut->Close();
            //}

                Int64 id;
                AutoPtr<IContentUris> contentUris;
                CContentUris::AcquireSingleton((IContentUris**)&contentUris);
                contentUris->ParseId(url, &id);

            // Wait until MINI_KIND thumbnail is generated.
                AutoPtr<IBitmap> miniThumb;
                AutoPtr<IMediaStoreImagesThumbnails> helper;
                CMediaStoreImagesThumbnails::AcquireSingleton((IMediaStoreImagesThumbnails**)&helper);
                helper->GetThumbnail(cr, id, IMediaStoreImagesThumbnails::MINI_KIND, NULL, (IBitmap**)&miniThumb);
            // This is for backward compatibility.
            AutoPtr<IBitmap> microThumb = StoreThumbnail(cr, miniThumb, id, 50.0, 50.0,
                IMediaStoreImagesThumbnails::MICRO_KIND);
        } else {
            //Log.e(TAG, "Failed to create thumbnail, removing original");
            Int32 val;
            cr->Delete(url, String(NULL), NULL, &val);
            url = NULL;
        }
    /*} catch (Exception e) {
        Log.e(TAG, "Failed to insert image", e);
        if (url != NULL) {
            cr.delete(url, NULL, NULL);
            url = NULL;
        }
    }*/

    if (url != NULL) {
        url->ToString(&stringUrl);
    }

    *urlvalue = stringUrl;
    return NOERROR;
}

ECode CMediaStoreImagesMedia::GetContentUri(
    /* [in] */ const String& volumeName,
    /* [out] */ IUri** uri)
{
    VALIDATE_NOT_NULL(uri);

    AutoPtr<IUriHelper> helper;
    CUriHelper::AcquireSingleton((IUriHelper**)&helper);
    return helper->Parse(IMediaStore::CONTENT_AUTHORITY_SLASH + volumeName +
            "/images/media", uri);
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
