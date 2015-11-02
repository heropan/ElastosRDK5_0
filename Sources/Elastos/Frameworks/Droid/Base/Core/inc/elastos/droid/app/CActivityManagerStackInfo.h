#ifndef __ELASTOS_DROID_APP_CACITONKEYINFO_H__
#define __ELASTOS_DROID_APP_CACITONKEYINFO_H__

#include "_Elastos_Droid_App_CActionKeyInfo.h"
#include <elastos/core/Object.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Utility::IAttributeSet;
namespace Elastos {
namespace Droid {
namespace App {

/**
 * Information you can retrieve about an ActivityStack in the system.
 * @hide
 */
CarClass(CActivityManagerStackInfo)
    , public Object
    , public IActivityManagerStackInfo
    , public IParcelable
{
public:
    CAR_INTERFACE_DECL()

    CAR_OBJECT_DECL()

    CActivityManagerStackInfo();

    CARAPI constructor();

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeInt(stackId);
        dest.writeInt(bounds.left);
        dest.writeInt(bounds.top);
        dest.writeInt(bounds.right);
        dest.writeInt(bounds.bottom);
        dest.writeIntArray(taskIds);
        dest.writeStringArray(taskNames);
        dest.writeInt(displayId);
    }

    public void readFromParcel(Parcel source) {
        stackId = source.readInt();
        bounds = new Rect(
                source.readInt(), source.readInt(), source.readInt(), source.readInt());
        taskIds = source.createIntArray();
        taskNames = source.createStringArray();
        displayId = source.readInt();
    }

    public static final Creator<StackInfo> CREATOR = new Creator<StackInfo>() {
        @Override
        public StackInfo createFromParcel(Parcel source) {
            return new StackInfo(source);
        }
        @Override
        public StackInfo[] newArray(int size) {
            return new StackInfo[size];
        }
    };

    public StackInfo() {
    }

    private StackInfo(Parcel source) {
        readFromParcel(source);
    }

    public String toString(String prefix) {
        StringBuilder sb = new StringBuilder(256);
        sb.append(prefix); sb.append("Stack id="); sb.append(stackId);
                sb.append(" bounds="); sb.append(bounds.toShortString());
                sb.append(" displayId="); sb.append(displayId);
                sb.append("\n");
        prefix = prefix + "  ";
        for (int i = 0; i < taskIds.length; ++i) {
            sb.append(prefix); sb.append("taskId="); sb.append(taskIds[i]);
                    sb.append(": "); sb.append(taskNames[i]); sb.append("\n");
        }
        return sb.toString();
    }

    CARAPI ToString(
        /* [out] */ String* str);

public:
    Int32 mStackId;
    AutoPtr<IRect> mBounds;// = new Rect();
    AutoPtr<ArrayOf<Int32> > mTaskIds;
    AutoPtr<ArrayOf<String> > mTaskNames;
    Int32 mDisplayId;

};

}// namespace App
}// namespace Droid
}// namespace Elastos

#endif
