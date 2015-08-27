// wuweizuo automatic build .cpp file from .java file.

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Content {
namespace Browser {

//=====================================================================
//                          NavigationHistory
//=====================================================================
ECode NavigationHistory::SetCurrentEntryIndex(
    /* [in] */ Int32 currentEntryIndex)
{
    /*
    mCurrentEntryIndex = currentEntryIndex;
    */
    return NOERROR;
}

Int32 NavigationHistory::GetEntryCount()
{
    /*
    return mEntries.size();
    */
}

AutoPtr<NavigationEntry> NavigationHistory::GetEntryAtIndex(
    /* [in] */ Int32 index)
{
    /*
    return mEntries.get(index);
    */
}

Int32 NavigationHistory::GetCurrentEntryIndex()
{
    /*
    return mCurrentEntryIndex;
    */
}

ECode NavigationHistory::AddEntry(
    /* [in] */ NavigationEntry* entry)
{
    VALIDATE_NOT_NULL(entry);
    /*
    mEntries.add(entry);
    */
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


