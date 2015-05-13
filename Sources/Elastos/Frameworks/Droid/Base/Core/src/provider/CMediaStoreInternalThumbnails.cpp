
#include "provider/CMediaStoreInternalThumbnails.h"
#include "ext/frameworkext.h"
#include "net/CUriHelper.h"
#include "content/CContentUris.h"
#include "graphics/CBitmapFactory.h"
#include <elastos/Logger.h>
#include <elastos/StringUtils.h>
#include "provider/CMediaStoreVideoMedia.h"
#include "provider/CMediaStoreImagesMedia.h"
#include "media/media/CMiniThumbFile.h"

using Elastos::Core::StringUtils;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Net::IUriHelper;
using Elastos::Droid::Net::CUriHelper;
using Elastos::Droid::Content::IContentUris;
using Elastos::Droid::Content::CContentUris;
using Elastos::Droid::Os::IParcelFileDescriptor;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Media::IMiniThumbFile;
using Elastos::Droid::Media::CMiniThumbFile;

namespace Elastos {
namespace Droid {
namespace Provider {

CMediaStoreInternalThumbnails::CMediaStoreInternalThumbnails()
{
    PROJECTION = ArrayOf<String>::Alloc(2);
    PROJECTION->Set(0, IMediaStoreInternalThumbnails::ID);
    PROJECTION->Set(1, IMediaStoreMediaColumns::DATA);
}

AutoPtr<IBitmap> CMediaStoreInternalThumbnails::GetMiniThumbFromFile(
    /* [in] */ ICursor* c,
    /* [in] */ IUri* baseUri,
    /* [in] */ IContentResolver* cr,
    /* [in] */ IBitmapFactoryOptions* options)
{
    AutoPtr<IBitmap> bitmap;
    AutoPtr<IUri> thumbUri;
    //try {
        Int64 thumbId;
        c->GetInt64(0, &thumbId);

        String filePath;
        c->GetString(1, &filePath);

        AutoPtr<IContentUris> contentUris;
        CContentUris::AcquireSingleton((IContentUris**)&contentUris);
        contentUris->WithAppendedId(baseUri, thumbId, (IUri**)&thumbUri);

        AutoPtr<IParcelFileDescriptor> pfdInput;
        cr->OpenFileDescriptor(thumbUri, String("r"), (IParcelFileDescriptor**)&pfdInput);

        AutoPtr<IBitmapFactory> factory;
        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
            (IBitmapFactory**)&factory));

        AutoPtr<IFileDescriptor> fd;
        pfdInput->GetFileDescriptor((IFileDescriptor**)&fd);

        factory->DecodeFileDescriptor(
                fd, NULL, options, (IBitmap**)&bitmap);

        pfdInput->Close();

    /*} catch (FileNotFoundException ex) {
        Log.e(TAG, "couldn't open thumbnail " + thumbUri + "; " + ex);
    } catch (IOException ex) {
        Log.e(TAG, "couldn't open thumbnail " + thumbUri + "; " + ex);
    } catch (OutOfMemoryError ex) {
        Log.e(TAG, "failed to allocate memory for thumbnail "
                + thumbUri + "; " + ex);
    }*/

    return bitmap;
}

ECode CMediaStoreInternalThumbnails::CancelThumbnailRequest(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ IUri* baseUri,
    /* [in] */ Int64 groupId)
{
    AutoPtr<IUri> cancelUri;

    AutoPtr<IUriBuilder> ub;
    baseUri->BuildUpon((IUriBuilder**)&ub);

    ub->AppendQueryParameter(String("cancel"), String("1"));
    ub->AppendQueryParameter(String("orig_id"), StringUtils::Int64ToString(origId));
    ub->AppendQueryParameter(String("group_id"), StringUtils::Int64ToString(groupId));
    ub->Build((IUri**)&cancelUri);

    AutoPtr<ICursor> c = NULL;
    //try {
        cr->Query(cancelUri, PROJECTION.Get(), String(NULL), NULL, String(NULL), (ICursor**)&c);
    //}
    //finally {
        if (c != NULL) c->Close();
    //}
    return NOERROR;
}

ECode CMediaStoreInternalThumbnails::GetThumbnail(
    /* [in] */ IContentResolver* cr,
    /* [in] */ Int64 origId,
    /* [in] */ Int64 groupId,
    /* [in] */ Int32 kind,
    /* [in] */ IBitmapFactoryOptions* options,
    /* [in] */ IUri* baseUri,
    /* [in] */ Boolean isVideo,
    /* [out] */ IBitmap** outBitmap)
{
    VALIDATE_NOT_NULL(outBitmap);

    AutoPtr<IBitmap> bitmap;
    String filePath;
    // Log.v(TAG, "getThumbnail: origId="+origId+", kind="+kind+", isVideo="+isVideo);
    // If the magic is non-zero, we simply return thumbnail if it does exist.
    // querying MediaProvider and simply return thumbnail.
    AutoPtr<IMiniThumbFile> thumbFile;
    AutoPtr<IUri> uri;
    if(isVideo) {
        AutoPtr<IMediaStoreVideoMedia> media;
        CMediaStoreVideoMedia::AcquireSingleton((IMediaStoreVideoMedia**)&media);
        media->GetEXTERNALCONTENTURI((IUri**)&uri);
    } else {
        AutoPtr<IMediaStoreImagesMedia> media;
        CMediaStoreImagesMedia::AcquireSingleton((IMediaStoreImagesMedia**)&media);
        media->GetEXTERNALCONTENTURI((IUri**)&uri);
    }
    CMiniThumbFile::New(uri, (IMiniThumbFile**)&thumbFile);

    AutoPtr<ICursor> c;
    // try {
        Int64 magic;
        thumbFile->GetMagic(origId, &magic);
        if (magic != 0) {
            if (kind == MICRO_KIND) {
                {
                    Mutex::Autolock lock(sThumbBufLock);
                    if (sThumbBuf == NULL) {
                        sThumbBuf = ArrayOf<Byte>::Alloc(IMiniThumbFile::BYTES_PER_MINTHUMB);
                    }
                    AutoPtr<ArrayOf<Byte> > miniThumb;
                    thumbFile->GetMiniThumbFromFile(origId, sThumbBuf.Get(), (ArrayOf<Byte>**)&miniThumb);
                    if (miniThumb != NULL) {
                        AutoPtr<IBitmapFactory> factory;
                        ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
                            (IBitmapFactory**)&factory));

                        factory->DecodeByteArrayEx(*sThumbBuf, 0, sThumbBuf->GetLength(), (IBitmap**)&bitmap);
                        if (bitmap == NULL) {
                            // Logger::W(TAG, "couldn't decode byte array.");
                        }
                    }
                }
                *outBitmap = bitmap;
                INTERFACE_ADDREF(*outBitmap);
                return NOERROR;
            } else if (kind == MINI_KIND) {
                String column = isVideo ? String("video_id=") : String("image_id=");
                cr->Query(baseUri, PROJECTION.Get(), column + origId, NULL, String(NULL), (ICursor**)&c);

                Boolean bSucceeded;
                if (c != NULL && (c->MoveToFirst(&bSucceeded), bSucceeded)) {
                    bitmap = GetMiniThumbFromFile(c, baseUri, cr, options);
                    if (bitmap != NULL) {
                        *outBitmap = bitmap;
                        INTERFACE_ADDREF(*outBitmap);
                        return NOERROR;
                    }
                }
            }
        }

        AutoPtr<IUri> blockingUri;

        AutoPtr<IUriBuilder> ub;
        baseUri->BuildUpon((IUriBuilder**)&ub);

        ub->AppendQueryParameter(String("blocking"), String("1"));
        ub->AppendQueryParameter(String("orig_id"), StringUtils::Int64ToString(origId));
        ub->AppendQueryParameter(String("group_id"), StringUtils::Int64ToString(groupId));
        ub->Build((IUri**)&blockingUri);

        if (c != NULL) {
            c->Close();
            c = NULL;
        }
        cr->Query(blockingUri, PROJECTION.Get(), String(NULL), NULL, String(NULL), (ICursor**)&c);
        // This happens when original image/video doesn't exist.
        if (c == NULL) {
            *outBitmap = NULL;
            INTERFACE_ADDREF(*outBitmap);
            return NOERROR;
        }

        // Assuming thumbnail has been generated, at least original image exists.
        if (kind == MICRO_KIND) {
            {
                Mutex::Autolock lock(sThumbBufLock);
                if (sThumbBuf == NULL) {
                    sThumbBuf = ArrayOf<Byte>::Alloc(IMiniThumbFile::BYTES_PER_MINTHUMB);
                }

                AutoPtr<ArrayOf<Byte> > miniThumb;
                thumbFile->GetMiniThumbFromFile(origId, sThumbBuf.Get(), (ArrayOf<Byte>**)&miniThumb);

                if (miniThumb != NULL) {

                    AutoPtr<IBitmapFactory> factory;
                    ASSERT_SUCCEEDED(CBitmapFactory::AcquireSingleton(
                        (IBitmapFactory**)&factory));

                    bitmap = NULL;
                    factory->DecodeByteArrayEx(*sThumbBuf, 0, sThumbBuf->GetLength(), (IBitmap**)&bitmap);

                    if (bitmap == NULL) {
                        //Log.w(TAG, "couldn't decode byte array.");
                    }
                }
            }
        } else if (kind == MINI_KIND) {
            Boolean bSucceeded;
            if ((c->MoveToFirst(&bSucceeded), bSucceeded)) {
                bitmap = GetMiniThumbFromFile(c, baseUri, cr, options);
            }
        } else {
            //throw new IllegalArgumentException("Unsupported kind: " + kind);
        }

        // We probably run out of space, so create the thumbnail in memory.
        if (bitmap == NULL) {
            /*Log.v(TAG, "Create the thumbnail in memory: origId=" + origId
                    + ", kind=" + kind + ", isVideo="+isVideo);*/
            AutoPtr<IUri> uri;

            AutoPtr<IUriBuilder> ub;
            baseUri->BuildUpon((IUriBuilder**)&ub);

            ub->AppendPath(StringUtils::Int64ToString(origId));

            String str;
            ub->ToString(&str);

            StringUtils::ReplaceFirst(str, String("thumbnails"), String("media"),&str);
            AutoPtr<IUriHelper> helper;
            CUriHelper::AcquireSingleton((IUriHelper**)&helper);
            helper->Parse(str, (IUri**)&uri);

            if (filePath == NULL) {
                if (c != NULL) {
                    c->Close();
                    c = NULL;
                }
                cr->Query(uri, PROJECTION.Get(), String(NULL), NULL, String(NULL), (ICursor**)&c);

                Boolean bSucceeded;
                if (c == NULL || !(c->MoveToFirst(&bSucceeded), bSucceeded)) {
                    *outBitmap = NULL;
                    INTERFACE_ADDREF(*outBitmap);
                    return NOERROR;
                }
                c->GetString(1, &filePath);
            }
            if (isVideo) {
//TODO: Need ThumbnailUtils
                // bitmap = ThumbnailUtils::CreateVideoThumbnail(filePath, kind);
            } else {
                // bitmap = ThumbnailUtils::CreateImageThumbnail(filePath, kind);
            }
        }
    //} catch (SQLiteException ex) {
        //Log.w(TAG, ex);
    //} finally {
        if (c != NULL) c->Close();
        // To avoid file descriptor leak in application process.
        thumbFile->Deactivate();
    //}
    *outBitmap = bitmap;
    INTERFACE_ADDREF(*outBitmap);
    return NOERROR;
}

} //namespace Provider
} //namespace Droid
} //namespace Elastos
