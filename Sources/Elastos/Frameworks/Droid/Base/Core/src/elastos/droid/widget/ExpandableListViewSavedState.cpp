
#include "widget/ExpandableListViewSavedState.h"
    
/**
 * Constructor called from {@link ExpandableListView#onSaveInstanceState()}
 */
ExpandableListViewSavedState::ExpandableListViewSavedState(
    /* [in] */ IParcelable* superState,
    /* [in] */ ArrayOf<IGroupMetadata*>* expandedGroupMetadataList) : ViewBaseSavedState(superState)
{
    this->expandedGroupMetadataList = expandedGroupMetadataList;
}

/**
 * Constructor called from {@link #CREATOR}
 */
ExpandableListViewSavedState::ExpandableListViewSavedState(
    /* [in] */ IParcel* in) : ViewBaseSavedState(in)
{
    //expandedGroupMetadataList = new ArrayList<ExpandableListConnector.GroupMetadata>();
    //in->ReadList(expandedGroupMetadataList, ExpandableListConnector.class.getClassLoader());
}

ECode ExpandableListViewSavedState::WriteToParcel(
    /* [in] */ IParcel* out, 
    /* [in] */ Int32 flags)
{
    //ViewBaseSavedState::WriteToParcel(out, flags);
    //out->WriteList(expandedGroupMetadataList);
}

//public static final Parcelable.Creator<SavedState> CREATOR
//        = new Parcelable.Creator<SavedState>() {
//    public SavedState createFromParcel(Parcel in) {
//        return new SavedState(in);
//    }
//
//    public SavedState[] newArray(int size) {
//        return new SavedState[size];
//    }
//};
