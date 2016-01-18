
#ifndef __ELASTOS_DROID_SERVER_CCERTBLACKLISTER_H__
#define __ELASTOS_DROID_SERVER_CCERTBLACKLISTER_H__

#include "_Elastos_Droid_Server_CCertBlacklister.h"
#include "elastos/droid/database/ContentObserver.h"
#include "elastos/droid/os/Binder.h"
#include <elastos/core/Thread.h>

using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Os::Binder;
using Elastos::Core::Thread;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CCertBlacklister)
    , public Binder
{
private:
    class BlacklistObserver : public ContentObserver
    {
    private:
        class WriteBlacklistThread : public Thread
        {
        public:
            CARAPI Run();
        };

    public:
        BlacklistObserver(
            /* [in] */ const String& key,
            /* [in] */ const String& name,
            /* [in] */ const String& path,
            /* [in] */ IContentResolver* cr);

        CARAPI OnChange(boolean selfChange);

        CARAPI_(String) GetValue();

    private:
        CARAPI_(void) WriteBlacklist();

    private:
        String mKey;
        String mName;
        String mPath;
        File mTmpDir;
        AutoPtr<IContentResolver> mContentResolver;
    };

public:
    CARAPI OnReceive(
        /* [in] */ IContext* context,
        /* [in] */ IIntent* intent);

    CARAPI ToString(
        /* [out] */ String* info);

public:
    static const String PUBKEY_PATH = BLACKLIST_ROOT + "pubkey_blacklist.txt";
    static const String SERIAL_PATH = BLACKLIST_ROOT + "serial_blacklist.txt";

    static const String PUBKEY_BLACKLIST_KEY = "pubkey_blacklist";
    static const String SERIAL_BLACKLIST_KEY = "serial_blacklist";

private:
    static const String TAG = "CertBlacklister";

    static const String BLACKLIST_ROOT = System.getenv("ANDROID_DATA") + "/misc/keychain/";
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_CCERTBLACKLISTER_H__
