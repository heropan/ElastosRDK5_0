
#ifndef __EXPANDABLELISTVIEWSAVEDSTATE_H__
#define __EXPANDABLELISTVIEWSAVEDSTATE_H__

#include "elastos/droid/ext/frameworkext.h"
#include "view/ViewBaseSavedState.h"

class ExpandableListViewSavedState : public ViewBaseSavedState
{
public:
    /**
     * Constructor called from {@link ExpandableListView#onSaveInstanceState()}
     */
    ExpandableListViewSavedState(
        /* [in] */ IParcelable* superState,
        /* [in] */ ArrayOf<IGroupMetadata*>* expandedGroupMetadataList);

private:
    /**
     * Constructor called from {@link #CREATOR}
     */
    ExpandableListViewSavedState(
        /* [in] */ IParcel* in);

public:
    virtual CARAPI WriteToParcel(
        /* [in] */ IParcel* out,
        /* [in] */ Int32 flags);

    /*public static final Parcelable.Creator<SavedState> CREATOR
            = new Parcelable.Creator<SavedState>() {
        public SavedState createFromParcel(Parcel in) {
            return new SavedState(in);
        }

        public SavedState[] newArray(Int32 size) {
            return new SavedState[size];
        }
    };*/

public:
    AutoPtr<ArrayOf<IGroupMetadata*> > expandedGroupMetadataList;

};

#endif
