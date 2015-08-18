
#ifndef __ELASTOS_DROID_CONTENT_CCLIPDATA_H__
#define __ELASTOS_DROID_CONTENT_CCLIPDATA_H__

#include "_Elastos_Droid_Content_CClipData.h"
#include "ext/frameworkext.h"
#include <elastos/utility/etl/List.h>

using Elastos::Utility::Etl::List;
using Elastos::Core::ICharSequence;
using Elastos::Core::IStringBuilder;
using Elastos::Droid::Net::IUri;
using Elastos::Droid::Graphics::IBitmap;

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CClipData)
{
public:
    ~CClipData();

    /**
     * Create a new ClipData holding data of the type
     * {@link ClipDescription#MIMETYPE_TEXT_PLAIN}.
     *
     * @param label User-visible label for the clip data.
     * @param text The actual text in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    static CARAPI NewPlainText(
        /* [in] */ ICharSequence* label,
        /* [in] */ ICharSequence* text,
        /* [out] */ IClipData** data);

    /**
     * Create a new ClipData holding data of the type
     * {@link ClipDescription#MIMETYPE_TEXT_HTML}.
     *
     * @param label User-visible label for the clip data.
     * @param text The text of clip as plain text, for receivers that don't
     * handle HTML.  This is required.
     * @param htmlText The actual HTML text in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    static CARAPI NewHtmlText(
        /* [in] */ ICharSequence* label,
        /* [in] */ ICharSequence* text,
        /* [in] */ const String& htmlText,
        /* [out] */ IClipData** data);

    /**
     * Create a new ClipData holding an Intent with MIME type
     * {@link ClipDescription#MIMETYPE_TEXT_INTENT}.
     *
     * @param label User-visible label for the clip data.
     * @param intent The actual Intent in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    static CARAPI NewIntent(
        /* [in] */ ICharSequence* label,
        /* [in] */ IIntent* intent,
        /* [out] */ IClipData** data);

    /**
     * Create a new ClipData holding a URI.  If the URI is a content: URI,
     * this will query the content provider for the MIME type of its data and
     * use that as the MIME type.  Otherwise, it will use the MIME type
     * {@link ClipDescription#MIMETYPE_TEXT_URILIST}.
     *
     * @param resolver ContentResolver used to get information about the URI.
     * @param label User-visible label for the clip data.
     * @param uri The URI in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    static CARAPI NewUri(
        /* [in] */ IContentResolver* resolver,
        /* [in] */ ICharSequence* label,
        /* [in] */ IUri* uri,
        /* [out] */ IClipData** data);

    /**
     * Create a new ClipData holding an URI with MIME type
     * {@link ClipDescription#MIMETYPE_TEXT_URILIST}.
     * Unlike {@link #newUri(ContentResolver, CharSequence, Uri)}, nothing
     * is inferred about the URI -- if it is a content: URI holding a bitmap,
     * the reported type will still be uri-list.  Use this with care!
     *
     * @param label User-visible label for the clip data.
     * @param uri The URI in the clip.
     * @return Returns a new ClipData containing the specified data.
     */
    static CARAPI NewRawUri(
        /* [in] */ ICharSequence* label,
        /* [in] */ IUri* uri,
        /* [out] */ IClipData** data);

    /**
    * Return the {@link ClipDescription} associated with this data, describing
    * what it contains.
    */
    CARAPI GetDescription(
        /* [out] */ IClipDescription** description);

    /**
     * Add a new Item to the overall ClipData container.
     */
    CARAPI AddItem(
        /* [in] */ IClipDataItem* item);

    /** @hide */
    CARAPI GetIcon(
        /* [out] */ IBitmap** icon);

    /**
     * Return the number of items in the clip data.
     */
    CARAPI GetItemCount(
        /* [out] */ Int32* itemCount);

    /**
     * Return a single item inside of the clip data.  The index can range
     * from 0 to {@link #getItemCount()}-1.
     */
    CARAPI GetItemAt(
        /* [in] */ Int32 index,
        /* [out] */ IClipDataItem** item);

    CARAPI ToString(
        /* [out] */ String* str);

    /** @hide */
    CARAPI ToShortString(
        /* [in] */ IStringBuilder* sb);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Create a new clip.
     *
     * @param label Label to show to the user describing this clip.
     * @param mimeTypes An array of MIME types this data is available as.
     * @param item The contents of the first item in the clip.
     */
    CARAPI constructor(
        /* [in] */ ICharSequence* label,
        /* [in] */ ArrayOf<String>* mimeTypes,
        /* [in] */ IClipDataItem* item);

    /**
     * Create a new clip.
     *
     * @param description The ClipDescription describing the clip contents.
     * @param item The contents of the first item in the clip.
     */
    CARAPI constructor(
        /* [in] */ IClipDescription* description,
        /* [in] */ IClipDataItem* item);

    /**
     * Create a new clip that is a copy of another clip.  This does a deep-copy
     * of all items in the clip.
     *
     * @param other The existing ClipData that is to be copied.
     */
    CARAPI constructor(
        /* [in] */ IClipData* other);

    CARAPI constructor();

public:
    static const AutoPtr< ArrayOf<String> > MIMETYPES_TEXT_PLAIN;
    static const AutoPtr< ArrayOf<String> > MIMETYPES_TEXT_HTML;
    static const AutoPtr< ArrayOf<String> > MIMETYPES_TEXT_URILIST;
    static const AutoPtr< ArrayOf<String> > MIMETYPES_TEXT_INTENT;

    AutoPtr<IClipDescription> mClipDescription;
    AutoPtr<IBitmap> mIcon;
    List< AutoPtr<IClipDataItem> > mItems;

private:
    static const String TAG;
};

} // Content
} // Droid
} // Elastos

#endif // __ELASTOS_DROID_CONTENT_CCLIPDATA_H__
