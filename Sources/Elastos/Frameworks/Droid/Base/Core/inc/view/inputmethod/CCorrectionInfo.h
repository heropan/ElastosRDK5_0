
#ifndef  __CCORRECTIONINFO_H__
#define  __CCORRECTIONINFO_H__

#include "_CCorrectionInfo.h"

using Elastos::Core::ICharSequence;

namespace Elastos {
namespace Droid {
namespace View {
namespace InputMethod {


CarClass(CCorrectionInfo)
{
public:
    CCorrectionInfo();

    CARAPI constructor();

    /**
     * @param offset The offset in the edited text where the old and new text start.
     * @param oldText The old text that has been replaced.
     * @param newText The replacement text.
     */
    CARAPI constructor(
        /* [in] */ Int32 offset,
        /* [in] */ ICharSequence* oldText,
        /* [in] */ ICharSequence* newText);

    // private CorrectionInfo(Parcel source) {
    //     mOffset = source.readInt();
    //     mOldText = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    //     mNewText = TextUtils.CHAR_SEQUENCE_CREATOR.createFromParcel(source);
    // }

    /**
     * Return the offset position of this correction in the text. Both the {@link #getOldText()} and
     * {@link #getNewText()} start at this offset.
     */
    CARAPI GetOffset(
        /* [out] */ Int32* offset);

    /**
     * Return the text that has actually been typed by the user, and which has been corrected.
     */
    CARAPI GetOldText(
        /* [out] */ ICharSequence** text);

    /**
     * Return the new text that corrects what was typed by the user.
     */
    CARAPI GetNewText(
        /* [out] */ ICharSequence** text);

    // @Override
    // public String ToString() {
    //     return "CorrectionInfo{#" + mOffset + " \"" + mOldText + "\" -> \"" + mNewText + "\"}";
    // }

    /**
     * Used to package this object into a {@link Parcel}.
     *
     * @param dest The {@link Parcel} to be written.
     * @param flags The flags used for parceling.
     */
    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);


    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    /**
     * Used to make this class parcelable.
     */
    // public static final Parcelable.Creator<CorrectionInfo> CREATOR
    //         = new Parcelable.Creator<CorrectionInfo>() {
    //     public CorrectionInfo createFromParcel(Parcel source) {
    //         return new CorrectionInfo(source);
    //     }

    //     public CorrectionInfo[] newArray(int size) {
    //         return new CorrectionInfo[size];
    //     }
    // };

    // public int DescribeContents() {
    //     return 0;
    // }

private:
    Int32 mOffset;
    AutoPtr<ICharSequence> mOldText;
    AutoPtr<ICharSequence> mNewText;
};

} // namespace InputMethod
} // namespace View
} // namespace Droid
} // namespace Elastos

#endif  //__CCORRECTIONINFO_H__
