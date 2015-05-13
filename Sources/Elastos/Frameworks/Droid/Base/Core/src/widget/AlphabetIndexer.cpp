
#include "widget/AlphabetIndexer.h"
#include <elastos/Math.h>
//#include "text/Collator.h"

using Elastos::Core::CStringWrapper;
using Elastos::Text::CCollatorHelper;
using Elastos::Text::ICollatorHelper;

namespace Elastos {
namespace Droid {
namespace Widget {

AlphabetIndexer::AlphabetIndexer()
    : mColumnIndex(0)
    , mAlphabetLength(0)
    , mAlphaMap(NULL)
{
}

/**
 * Constructs the indexer.
 * @param cursor the cursor containing the data set
 * @param sortedColumnIndex the column number in the cursor that is sorted
 *        alphabetically
 * @param alphabet string containing the alphabet, with space as the first character.
 *        For example, use the string " ABCDEFGHIJKLMNOPQRSTUVWXYZ" for English indexing.
 *        The characters must be uppercase and be sorted in ascii/unicode order. Basically
 *        characters in the alphabet will show up as preview letters.
 */
AlphabetIndexer::AlphabetIndexer(
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 sortedColumnIndex,
    /* [in] */ ICharSequence* alphabet)
    : mColumnIndex(0)
    , mAlphabetLength(0)
    , mAlphaMap(NULL)
{
    InitForInstance(cursor, sortedColumnIndex, alphabet);
}

ECode AlphabetIndexer::Init(
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 sortedColumnIndex,
    /* [in] */ ICharSequence* alphabet)
{
    return InitForInstance(cursor, sortedColumnIndex, alphabet);
}

ECode AlphabetIndexer::InitForInstance(
    /* [in] */ ICursor* cursor,
    /* [in] */ Int32 sortedColumnIndex,
    /* [in] */ ICharSequence* alphabet)
{
    mDataCursor = cursor;
    mColumnIndex = sortedColumnIndex;
    mAlphabet = alphabet;
    alphabet->GetLength(&mAlphabetLength);
    mAlphabetArray = ArrayOf<String>::Alloc(mAlphabetLength);//new String[mAlphabetLength];
    for (Int32 i = 0; i < mAlphabetLength; i++) {
        AutoPtr<ICharSequence> temp;
        mAlphabet->SubSequence(i, i + 1, (ICharSequence**)&temp);
        temp->ToString(&(*mAlphabetArray)[i]);
    }
    mAlphaMap = new HashMap<Int32, Int32>(mAlphabetLength);
    if (cursor != NULL) {
        cursor->RegisterDataSetObserver(THIS_PROBE(IDataSetObserver));
    }

    // Get a Collator for the current locale for string comparisons.
   AutoPtr<ICollatorHelper> helper;
   CCollatorHelper::AcquireSingleton((ICollatorHelper**)&helper);
   helper->GetInstance((ICollator**)&mCollator);
   mCollator->SetStrength(ICollator::PRIMARY);
    return NOERROR;
}

AlphabetIndexer::~AlphabetIndexer()
{
    mAlphaMap = NULL;
}

/**
 * Returns the section array constructed from the alphabet provided in the constructor.
 * @return the section array
 */
AutoPtr<ArrayOf<IInterface*> > AlphabetIndexer::GetSections()
{
    Int32 length = mAlphabetArray->GetLength();
    AutoPtr<ArrayOf<IInterface*> > result = ArrayOf<IInterface*>::Alloc(length);
    for (Int32 i = 0; i < length; ++i) {
        AutoPtr<ICharSequence> temp;
        CStringWrapper::New((*mAlphabetArray)[i], (ICharSequence**)&temp);
        result->Set(i, temp);
    }

    return result;
}

/**
 * Sets a new cursor as the data set and resets the cache of indices.
 * @param cursor the new cursor to use as the data set
 */
ECode AlphabetIndexer::SetCursor(
    /* [in] */ ICursor* cursor)
{
    if (mDataCursor != NULL) {
        mDataCursor->UnregisterDataSetObserver((IDataSetObserver*)this->Probe(EIID_IDataSetObserver));
    }
    mDataCursor = cursor;
    if (cursor != NULL) {
        mDataCursor->RegisterDataSetObserver((IDataSetObserver*)this->Probe(EIID_IDataSetObserver));
    }
    mAlphaMap->Clear();
    return NOERROR;
}

/**
 * Default implementation compares the first character of word with letter.
 */
Int32 AlphabetIndexer::Compare(
    /* [in] */ const String& word,
    /* [in] */ const String& letter)
{
    String firstLetter;
    if (word.IsNullOrEmpty()) {
        firstLetter = String(" ");
    } else {
        firstLetter = word.Substring(0, 1);
    }

    Int32 result = 0;
    AutoPtr<ICharSequence> fcs;
    AutoPtr<ICharSequence> lcs;
    CStringWrapper::New(firstLetter, (ICharSequence**)&fcs);
    CStringWrapper::New(letter, (ICharSequence**)&lcs);
    mCollator->Compare(fcs, lcs, &result);
    return result;
}

/**
 * Performs a binary search or cache lookup to find the first row that
 * matches a given section's starting letter.
 * @param sectionIndex the section to search for
 * @return the row index of the first occurrence, or the nearest next letter.
 * For instance, if searching for "T" and no "T" is found, then the first
 * row starting with "U" or any higher letter is returned. If there is no
 * data following "T" at all, then the list size is returned.
 */
Int32 AlphabetIndexer::GetPositionForSection(
    /* [in] */ Int32 sectionIndex)
{
    AutoPtr<HashMap<Int32, Int32> > alphaMap = mAlphaMap;
    AutoPtr<ICursor> cursor = mDataCursor;

    if (cursor == NULL || mAlphabet == NULL) {
        return 0;
    }

    // Check bounds
    if (sectionIndex <= 0) {
        return 0;
    }
    if (sectionIndex >= mAlphabetLength) {
        sectionIndex = mAlphabetLength - 1;
    }

    Int32 savedCursorPos;
    cursor->GetPosition(&savedCursorPos);

    Int32 count;
    cursor->GetCount(&count);
    Int32 start = 0;
    Int32 end = count;
    Int32 pos;

    Char32 letter;
    mAlphabet->GetCharAt(sectionIndex, &letter);
    char cTemp = (char)letter;

    String targetLetter = String(&cTemp, 1);
    Int32 key = letter;
    // Check map
    AlphaMapIterator it = alphaMap->Find(key);
    if(it == alphaMap->End())
    {
        pos = Elastos::Core::Math::INT32_MIN_VALUE;
    }else {
        pos = it->mSecond;
    }
    if (Elastos::Core::Math::INT32_MIN_VALUE != pos) {
        // Is it approximate? Using negative value to indicate that it's
        // an approximation and positive value when it is the accurate
        // position.
        if (pos < 0) {
            pos = -pos;
            end = pos;
        } else {
            // Not approximate, this is the confirmed start of section, return it
            return pos;
        }
    }

    // Do we have the position of the previous section?
    if (sectionIndex > 0) {
        Int32 prevLetter;
        mAlphabet->GetCharAt(sectionIndex - 1, (Char32*)&prevLetter);
        Int32 prevLetterPos;
        it = alphaMap->Find(prevLetter);
        if(it == alphaMap->End())
        {
            prevLetterPos = Elastos::Core::Math::INT32_MIN_VALUE;
        }else {
            prevLetterPos = it->mSecond;
        }
        if (prevLetterPos != Elastos::Core::Math::INT32_MIN_VALUE) {
            start = Elastos::Core::Math::Abs(prevLetterPos);
        }
    }

    // Now that we have a possibly optimized start and end, let's binary search

    pos = (end + start) / 2;

    Boolean res;
    while (pos < end) {
        // Get letter at pos
        cursor->MoveToPosition(pos, &res);
        String curName;
        cursor->GetString(mColumnIndex, &curName);
        if (curName == NULL) {
            if (pos == 0) {
                break;
            } else {
                pos--;
                continue;
            }
        }
        Int32 diff = Compare(curName, targetLetter);
        if (diff != 0) {
            // TODO: Commenting out approximation code because it doesn't work for certain
            // lists with custom comparators
            // Enter approximation in hash if a better solution doesn't exist
            // String startingLetter = Character.toString(getFirstLetter(curName));
            // Int32 startingLetterKey = startingLetter.charAt(0);
            // Int32 curPos = alphaMap.get(startingLetterKey, Elastos::Core::Math::INT32_MIN_VALUE);
            // if (curPos == Elastos::Core::Math::INT32_MIN_VALUE || Elastos::Core::Math.abs(curPos) > pos) {
            //     Negative pos indicates that it is an approximation
            //     alphaMap.put(startingLetterKey, -pos);
            // }
            // if (mCollator.compare(startingLetter, targetLetter) < 0) {
            if (diff < 0) {
                start = pos + 1;
                if (start >= count) {
                    pos = count;
                    break;
                }
            } else {
                end = pos;
            }
        } else {
            // They're the same, but that doesn't mean it's the start
            if (start == pos) {
                // This is it
                break;
            } else {
                // Need to go further lower to find the starting row
                end = pos;
            }
        }
        pos = (start + end) / 2;
    }
    (*alphaMap)[key] = pos;
    cursor->MoveToPosition(savedCursorPos, &res);
    return pos;
}

/**
 * Returns the section index for a given position in the list by querying the item
 * and comparing it with all items in the section array.
 */
Int32 AlphabetIndexer::GetSectionForPosition(
    /* [in] */ Int32 position)
{
    Int32 savedCursorPos;
    Boolean res;
    mDataCursor->GetPosition(&savedCursorPos);
    mDataCursor->MoveToPosition(position, &res);
    String curName;
    mDataCursor->GetString(mColumnIndex, &curName);
    mDataCursor->MoveToPosition(savedCursorPos, &res);
    // Linear search, as there are only a few items in the section index
    // Could speed this up later if it actually gets used.
    for (Int32 i = 0; i < mAlphabetLength; i++) {
        Char32 letter;
        mAlphabet->GetCharAt(i, &letter);
        char cTemp = (char)letter;
        String targetLetter = String(&cTemp, 1);
        if (Compare(curName, targetLetter) == 0) {
            return i;
        }
    }
    return 0; // Don't recognize the letter - falls under zero'th section
}

/*
 * @hide
 */
ECode AlphabetIndexer::OnChanged()
{
    mAlphaMap->Clear();
    return NOERROR;
}

/*
 * @hide
 */
ECode AlphabetIndexer::OnInvalidated()
{
    mAlphaMap->Clear();
    return NOERROR;
}

}// namespace Widget
}// namespace Droid
}// namespace Elastos

