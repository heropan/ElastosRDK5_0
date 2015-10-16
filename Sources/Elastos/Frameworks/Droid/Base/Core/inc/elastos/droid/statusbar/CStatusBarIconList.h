#ifndef __ELASTOS_DROID_STATUSBAR_CSTATUSBARICONLIST_H__
#define __ELASTOS_DROID_STATUSBAR_CSTATUSBARICONLIST_H__

#include "_Elastos_Droid_StatusBar_CStatusBarIconList.h"
#include "elastos/droid/ext/frameworkdef.h"
#include <elastos/core/StringBuilder.h>
#include "elastos/droid/statusbar/CStatusBarIcon.h"

using Elastos::Core::StringBuilder;
using Elastos::Droid::StatusBar::IStatusBarIcon;
using Elastos::Droid::StatusBar::CStatusBarIcon;
using Elastos::IO::IPrintWriter;

namespace Elastos {
namespace Droid {
namespace StatusBar {

CarClass(CStatusBarIconList)
{
public:
    CARAPI constructor();

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* in);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* out);

    CARAPI DescribeContents(
        /* [out] */ Int32* value);

    /**
     * Parcelable.Creator that instantiates StatusBarIconList objects
     */
/*
    public static final Parcelable.Creator<StatusBarIconList> CREATOR
            = new Parcelable.Creator<StatusBarIconList>()
    {
        public StatusBarIconList createFromParcel(Parcel parcel)
        {
            return new StatusBarIconList(parcel);
        }

        public StatusBarIconList[] newArray(int size)
        {
            return new StatusBarIconList[size];
        }
    };
*/
    CARAPI DefineSlots(
        /* [in] */ const ArrayOf<String>& slots);

    CARAPI GetSlotIndex(
        /* [in] */ const String& slot,
        /* [out] */ Int32* slotIndex);

    CARAPI Size(
        /* [out] */ Int32* size);

    CARAPI SetIcon(
        /* [in] */ Int32 index,
        /* [in] */ IStatusBarIcon* icon);

    CARAPI RemoveIcon(
        /* [in] */ Int32 index);

    CARAPI GetSlot(
        /* [in] */ Int32 index,
        /* [out] */ String* slot);

    CARAPI GetIcon(
        /* [in] */ Int32 index,
        /* [out] */ IStatusBarIcon** icon);

    CARAPI GetViewIndex(
        /* [in] */ Int32 index,
        /* [out] */ Int32* viewIndex);

    CARAPI CopyFrom(
        /* [in] */ IStatusBarIconList* that);

    CARAPI Dump(
        /* [in] */ IPrintWriter* pw);

    CARAPI GetSlots(
        /* [out, callee] */ ArrayOf<String>** slots);

    CARAPI GetIcons(
        /* [out, callee] */ ArrayOf<IStatusBarIcon*>** icons);
private:
    AutoPtr<ArrayOf<String> > mSlots;
    AutoPtr<ArrayOf<IStatusBarIcon*> > mIcons;
};

}//namespace StatusBar
}//namespace Droid
}//namespace Elastos

#endif //__ELASTOS_DROID_STATUSBAR_CSTATUSBARICONLIST_H__
