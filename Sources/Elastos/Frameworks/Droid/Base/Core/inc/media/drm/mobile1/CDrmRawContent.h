
#ifndef __CDRMRAWCONTENT_H__
#define __CDRMRAWCONTENT_H__

#include "_CDrmRawContent.h"
#include <Elastos.CoreLibrary.h>
#include <ext/frameworkext.h>
#include <elastos/io/InputStream.h>

using Elastos::IO::InputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IBufferedInputStream;

namespace Elastos {
namespace Droid {
namespace Drm {
namespace Mobile1 {

CarClass(CDrmRawContent)
{
public:
    class DrmInputStream
        : public ElRefBase
        , public IInputStream
        , public InputStream
    {
    public:
        DrmInputStream(
            /* [in] */ IDrmRights* rights,
            /* [in] */ CDrmRawContent* drmRawContent);

        ~DrmInputStream();

        CARAPI_(PInterface) Probe(
            /* [in] */ REIID riid);

        CARAPI_(UInt32) AddRef();

        CARAPI_(UInt32) Release();

        CARAPI GetInterfaceID(
            /* [in] */ IInterface* pObject,
            /* [in] */ InterfaceID* pIID);

        CARAPI Available(
            /* [out] */ Int32* number);

        CARAPI Close();

        CARAPI Mark(
            /* [in] */ Int32 readLimit);

        CARAPI IsMarkSupported(
            /* [out] */ Boolean* supported);

        CARAPI Read(
            /* [out] */ Int32* value);

        CARAPI ReadBytes(
            /* [in] */ ArrayOf<Byte>* b,
            /* [out] */ Int32* number);

        CARAPI ReadBytes(
            /* [in] */ ArrayOf<Byte>* b,
            /* [in] */ Int32 off,
            /* [in] */ Int32 len,
            /* [out] */ Int32 * number);

        CARAPI Reset();

        CARAPI Skip(
            /* [in] */ Int64 count,
            /* [out] */ Int64 * number);

        CARAPI GetLock(
            /* [out] */ IInterface** lockobj);
    private:
        Boolean mIsClosed;
        Int32 mOffset;
        AutoPtr<ArrayOf<Byte> > mB;
        CDrmRawContent* mDrmRawContent;
    };

public:
    CDrmRawContent();

    ~CDrmRawContent();

    CARAPI constructor(
    /*[in] */ IInputStream* inRawdata,
    /*[in] */ Int32 len,
    /*[in] */ const String& mimeTypeStr);

    CARAPI GetRightsAddress(
        /* [out] */ String* rightsIssuer);

    CARAPI GetRawType(
        /* [out] */ Int32* rawType);

    CARAPI GetContentInputStream(
        /* [in] */ IDrmRights* mRights,
        /* [out] */ IInputStream** rights);

    CARAPI GetContentType(
        /* [out] */ String* mediaType);

    CARAPI GetContentLength(
        /* [in] */ IDrmRights* mRights,
        /* [out] */ Int32* len);

    CARAPI GetId(
        /* [out] */ Int32* id);

private:
    /**
     * native method: construct a DRM content according the mime type.
     *
     * @param data      input DRM content data to be parsed.
     * @param len       the length of the data.
     * @param mimeType  the mime type of this DRM content. the value of this field includes:
     *                      #DRM_MIMETYPE_MESSAGE
     *                      #DRM_MIMETYPE_CONTENT
     *
     * @return #the id of the DRM content if succeed.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeConstructDrmContent(
         /* [in] */ IInputStream *data,
         /* [in] */ Int32 len,
         /* [in] */ Int32 mimeType);

    /**
     * native method: get this DRM content rights issuer.
     *
     * @return the address of rights issuer if in case of separate delivery.
     *         null if not separete delivery, or otherwise.
     */
    CARAPI_(String) NativeGetRightsAddress();

    /**
     * native method: get this DRM content delivery type.
     *
     * @return the delivery method, the value may be one of the following:
     *              #DRM_FORWARD_LOCK
     *              #DRM_COMBINED_DELIVERY
     *              #DRM_SEPARATE_DELIVERY
     *              #DRM_SEPARATE_DELIVERY_DM
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeGetDeliveryMethod();


    /**
     * native method: get a piece of media content data.
     *
     * @param buf       the buffer to save DRM media content data.
     * @param bufOff    the offset of the buffer to start to save data.
     * @param len       the number of byte to read.
     * @param mediaOff  the offset of the media content data to start to read.
     *
     * @return the length of the media content data has been read.
     *         #JNI_DRM_EOF if reach to end of the media content.
     *         #JNI_DRM_FAILURE if fail.
     */
    CARAPI_(Int32) NativeReadContent(
        /* [in] */ ArrayOf<Byte>* buf,
        /* [in] */ Int32 bufOff,
        /* [in] */ Int32 len,
        /* [in] */ Int32 mediaOff);

    /**
     * native method: get this DRM content type.
     *
     * @return the decrypted media content type.
     *         null if fail.
     */
    CARAPI_(String) NativeGetContentType();

    /**
     * native method: get this DRM decrypted media content length.
     *
     * @return the length of decrypted media content.
     *         #JNI_DRM_FAILURE if fail.
     *         #JNI_DRM_UNKNOWN_DATA_LEN if the length is unknown currently.
     */
    CARAPI_(Int32) NativeGetContentLength();

public:
    /**
     * Successful operation.
     */
    static const Int32 JNI_DRM_SUCCESS = 0;

    /**
     * General failure.
     */
    static const Int32 JNI_DRM_FAILURE = -1;

    /**
     * The id of "application/vnd.oma.drm.message" mime type.
     */
    static const Int32 DRM_MIMETYPE_MESSAGE = 1;

    /**
     * The id of "application/vnd.oma.drm.content" mime type.
     */
    static const Int32 DRM_MIMETYPE_CONTENT = 2;

    /**
     * Indicates the end of the DRM content is reached.
     */
    static const Int32 JNI_DRM_EOF = -2;

    /**
     * The media content length is unknown from native method
     */
    static const Int32 JNI_DRM_UNKNOWN_DATA_LEN = -3;

private:
    /**
     * The member to save the original InputStream data.
     */
    AutoPtr<IBufferedInputStream> mInData;

    /**
     * The member to save the original InputStream data length.
     */
    Int32 mInDataLen;

    /**
     * The unique id to this DRM content. It will be initialized
     * in constructor by native method. And it will not be changed
     * after initialization.
     */
    Int32 mId;

    /**
     * The rights issuer address of this DRM object.
     */
    String mRightsIssuer;

    /**
     * The media content type of this DRM object.
     */
    String mMediaType;

    /**
     * The delivery method type of this DRM object.
     */
    Int32 mRawType;

};

} // namespace mobile1
} // namespace drm
} // namepsace Droid
} // namespace Elastos


#endif // __CDRMRAWCONTENT_H__
