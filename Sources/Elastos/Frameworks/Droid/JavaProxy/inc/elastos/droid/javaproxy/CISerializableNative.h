
#ifndef __ELASTOS_DROID_JAVAPROXY_CISERIALIZABLENATIVE_H__
#define __ELASTOS_DROID_JAVAPROXY_CISERIALIZABLENATIVE_H__

#include "_Elastos_Droid_JavaProxy_CISerializableNative.h"
#include <jni.h>

namespace Elastos {
namespace Droid {
namespace JavaProxy {

CarClass(CISerializableNative)
{
public:
    CARAPI constructor();

    CARAPI constructor(
        /* [in] */ const String& pkgPath,
        /* [in] */ ArrayOf<Byte>* obj);

    CARAPI_(String) GetPackagePath();

    CARAPI GetObject(
        /* [out, callee] */ ArrayOf<Byte>** obj);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel * pSource);

    CARAPI WriteToParcel(
        /* [in] */ IParcel * pDest);

private:
    static const String TAG;

    String mPkgPath;

    AutoPtr<ArrayOf<Byte> > mObject;
};

}
}
}

#endif // __ELASTOS_DROID_JAVAPROXY_CISERIALIZABLENATIVE_H__
