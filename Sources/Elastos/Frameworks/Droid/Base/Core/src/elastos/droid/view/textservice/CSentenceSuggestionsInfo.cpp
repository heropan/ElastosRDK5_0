
/**
 * Constructor.
 * @param suggestionsInfos from the text service
 * @param offsets the array of offsets of suggestions
 * @param lengths the array of lengths of suggestions
 */
ECode CSentenceSuggestionsInfo::constructor(
    /* [in] */ ArrayOf<ISuggestionsInfo*>* suggestionsInfos,
    /* [in] */ ArrayOf<Int32>* offsets,
    /* [in] */ ArrayOf<Int32>* lengths)
{
    assert(suggestionsInfos != NULL && offsets != NULL && lengths != NULL);

    assert(!(suggestionsInfos->GetLength() != offsets->GetLength()
        || offsets->GetLength() != lengths->GetLength()));

    Int32 infoSize = suggestionsInfos->GetLength();
    mSuggestionsInfos->CopyOf(suggestionsInfos, 0, infoSize);
    mOffsets->Copy(offsets, 0, infoSize);
    mLengths->CopyOf(lengths, 0, infoSize);

    return NOERROR;
}

ECode CSentenceSuggestionsInfo::constructor(
    /* [in] */ IParcel* source)
{
    assert(source != NULL);

    Int32 infoSize;
    source->ReadInt(infoSize);
    mSuggestionsInfos = ArrayOf<ISuggestionsInfo*>::Alloc(infoSize);
//    source.readTypedArray(mSuggestionsInfos, SuggestionsInfo.CREATOR);
    mOffsets = ArrayOf<Int32>::Alloc(mSuggestionsInfos->GetLength());
//    source.readIntArray(mOffsets);
    mLengths = ArrayOf<Int32>::Alloc(mSuggestionsInfos->GetLength());
//    source.readIntArray(mLengths);

    return NOERROR;
}

ECode CSentenceSuggestionsInfo::ReadFromParcel(
        /* [in] */ IParcel* parcel)
{
//    return new SentenceSuggestionsInfo(source);
}

/**
 * Used to package this object into a {@link Parcel}.
 *
 * @param dest The {@link Parcel} to be written.
 * @param flags The flags used for parceling.
 */
//@Override
ECode CSentenceSuggestionsInfo::WriteToParcel(
    /* [in] */ IParcel* dest)
{
    assert(dest != NULL);

    Int32 infoSize = mSuggestionsInfos->GetLength();
    dest->WriteInt32(infoSize);
//    dest.writeTypedArray(mSuggestionsInfos, 0);
//    dest.writeIntArray(mOffsets);
//    dest.writeIntArray(mLengths);

    return NOERROR;
}

/**
 * @return the count of {@link SuggestionsInfo}s this instance holds.
 */
ECode CSentenceSuggestionsInfo::GetSuggestionsCount(
    /* [out] */ Int32* count)
{
    VALIDATE_NOT_NULL(count);
    mSuggestionsInfos->GetLength();
    return NOERROR;
}

/**
 * @param i the id of {@link SuggestionsInfo}s this instance holds.
 * @return a {@link SuggestionsInfo} at the specified id
 */
ECode CSentenceSuggestionsInfo::GetSuggestionsInfoAt(
    /* [in] */ Int32 i,
    /* [out] */ ISuggestionsInfo* info)
{
    VALIDATE_NOT_NULL(info);

    if (i >= 0 && i < mSuggestionsInfos->GetLength()) {
        *info = mSuggestionsInfos[i];
        REFCOUNT_ADD(*info);
        return NOERROR;
    }

    *info = NULL;

    return NOERROR;
}

/**
 * @param i the id of {@link SuggestionsInfo}s this instance holds
 * @return the offset of the specified {@link SuggestionsInfo}
 */
ECode CSentenceSuggestionsInfo::GetOffsetAt(
    /* [in] */ Int32 i,
    /* [out] */ Int32* offset)
{
    VALIDATE_NOT_NULL(offset);

    if (i >= 0 && i < mOffsets->GetLength()) {
        *offset = mOffsets[i];
    }

    *offset = -1;

    return NOERROR;
}

/**
 * @param i the id of {@link SuggestionsInfo}s this instance holds
 * @return the length of the specified {@link SuggestionsInfo}
 */
ECode CSentenceSuggestionsInfo::GetLengthAt(
    /* [in] */ Int32 i,
    /* [out] */ Int32* length)
{
    VALIDATE_NOT_NULL(length);

    if (i >= 0 && i < mLengths->GetLength()) {
        *length = mLengths[i];
    }

    *length = -1;

    return NOERROR;
}
