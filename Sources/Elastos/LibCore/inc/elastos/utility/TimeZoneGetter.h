#ifndef __UTILITY_TIMEZONEGETTER_H__
#define __UTILITY_TIMEZONEGETTER_H__

#include "cmdef.h"
#include "Elastos.Core_server.h"

namespace Elastos {
namespace Utility {

class TimeZoneGetter
{
private:
    class DefaultTimeZoneGetter
        : public ITimeZoneGetter
        , public ElRefBase
    {
    public:
        CAR_INTERFACE_DECL();

        CARAPI GetId(
            /* [out] */ String* string);

    private:
        String mId;
    };

public:
    static CARAPI_(AutoPtr<ITimeZoneGetter>) GetInstance();

    static CARAPI SetInstance(
        /* [in] */ ITimeZoneGetter* getter);

private:
    static AutoPtr<ITimeZoneGetter> sInstance;
};

}// namespace Utility
}// namespace Utility

#endif // __UTILITY_TIMEZONEGETTER_H__
