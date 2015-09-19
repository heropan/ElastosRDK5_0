// wuweizuo automatic build .cpp file from .java file.

#include "NavigationHistory.h"

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
    // ==================before translated======================
    // mCurrentEntryIndex = currentEntryIndex;
    assert(0);
    return NOERROR;
}

Int32 NavigationHistory::GetEntryCount()
{
    // ==================before translated======================
    // return mEntries.size();
    assert(0);
    return 0;
}

AutoPtr<NavigationEntry> NavigationHistory::GetEntryAtIndex(
    /* [in] */ Int32 index)
{
    // ==================before translated======================
    // return mEntries.get(index);
    assert(0);
    AutoPtr<NavigationEntry> empty;
    return empty;
}

Int32 NavigationHistory::GetCurrentEntryIndex()
{
    // ==================before translated======================
    // return mCurrentEntryIndex;
    assert(0);
    return 0;
}

ECode NavigationHistory::AddEntry(
    /* [in] */ NavigationEntry* entry)
{
    VALIDATE_NOT_NULL(entry);
    // ==================before translated======================
    // mEntries.add(entry);
    assert(0);
    return NOERROR;
}

} // namespace Browser
} // namespace Content
} // namespace Webkit
} // namespace Droid
} // namespace Elastos


