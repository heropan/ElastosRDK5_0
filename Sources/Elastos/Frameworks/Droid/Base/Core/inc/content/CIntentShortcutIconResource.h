
#ifndef __CINTENTSHORTCUTICONRESOURCE_H__
#define __CINTENTSHORTCUTICONRESOURCE_H__

#include "_CIntentShortcutIconResource.h"

namespace Elastos {
namespace Droid {
namespace Content {

CarClass(CIntentShortcutIconResource)
{
public:
    CARAPI constructor();

    CARAPI GetPackageName(
        /* [out] */ String* packageName);

    CARAPI SetPackageName(
        /* [in] */ const String& packageName);

    CARAPI GetResourceName(
        /* [out] */ String* resourceName);

    CARAPI SetResourceName(
        /* [in] */ const String& resourceName);

    CARAPI ToString(
        /* [out] */ String* str);

    CARAPI ReadFromParcel(
        /* [in] */ IParcel* source);

    CARAPI WriteToParcel(
        /* [in] */ IParcel* dest);

private:
    /**
     * The package name of the application containing the icon.
     */
    String mPackageName;

    /**
     * The resource name of the icon, including package, name and type.
     */
    String mResourceName;

};

}
}
}

#endif // __CINTENTSHORTCUTICONRESOURCE_H__
