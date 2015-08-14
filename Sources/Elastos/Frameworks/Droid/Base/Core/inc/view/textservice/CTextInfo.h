
#ifndef __ELASTOS_DROID_VIEW_TEXTSERVICE_CTEXTINFO_H__
#define __ELASTOS_DROID_VIEW_TEXTSERVICE_CTEXTINFO_H__

namespace Elastos {
namespace Droid {
namespace View {
namespace Textservice {

/**
 * This class contains a metadata of the input of TextService
 */
CarClass(CTextInfo)
{
public:
    /**
     * Constructor.
     * @param text the text which will be input to TextService
     */
    CARAPI constructor(
        /* [in] */ const String& text);

    /**
     * Constructor.
     * @param text the text which will be input to TextService
     * @param cookie the cookie for this TextInfo
     * @param sequence the sequence number for this TextInfo
     */
    CARAPI constructor(
        /* [in] */ const String& text,
        /* [in] */ Int32 cookie,
        /* [in] */ Int32 sequence);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* parcel);

    /**
     * Used to package this object into a {@link Parcel}.
     *
     * @param dest The {@link Parcel} to be written.
     * @param flags The flags used for parceling.
     */
    //@Override
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

    /**
     * @return the text which is an input of a text service
     */
    CARAPI GetText(
        /* [out] */ String* text);

    /**
     * @return the cookie of TextInfo
     */
    CARAPI GetCookie(
        /* [out] */ Int32* cookie);

    /**
     * @return the sequence of TextInfo
     */
    CARAPI GetSequence(
        /* [out] */ Int32* sequence);

private:
    String mText;
    Int32 mCookie;
    Int32 mSequence;
};

}   //namespace Textservice
}   //namespace View
}   //namespace Droid
}   //namespace Elastos

#endif //__ELASTOS_DROID_VIEW_TEXTSERVICE_CTEXTINFO_H__
