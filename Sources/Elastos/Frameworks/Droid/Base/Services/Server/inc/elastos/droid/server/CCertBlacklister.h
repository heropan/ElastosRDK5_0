
#ifndef __ELASTOS_DROID_SERVER_CCERTBLACKLISTER_H__
#define __ELASTOS_DROID_SERVER_CCERTBLACKLISTER_H__

#include "_Elastos_Droid_Server_CCertBlacklister.h"
#include <elastos/droid/database/ContentObserver.h>
#include <elastos/droid/os/Binder.h>
#include <elastos/core/Thread.h>
#include <Elastos.Droid.Content.h>

using Elastos::Droid::Content::IContext;
using Elastos::Droid::Content::IContentResolver;
using Elastos::Droid::Database::ContentObserver;
using Elastos::Droid::Database::IContentObserver;
using Elastos::Droid::Os::Binder;
using Elastos::Core::Thread;
using Elastos::IO::IFile;

namespace Elastos {
namespace Droid {
namespace Server {

CarClass(CCertBlacklister)
    , public Binder
{
private:
    class BlacklistObserver
        : public ContentObserver
    {
    private:
        class WriteBlacklistThread
            : public Thread
        {
        public:
            WriteBlacklistThread(
                /* [in] */ BlacklistObserver* host);

            CARAPI Run();
        private:
            BlacklistObserver* mHost;
        };

    public:
        BlacklistObserver(
            /* [in] */ const String& key,
            /* [in] */ const String& name,
            /* [in] */ const String& path,
            /* [in] */ IContentResolver* cr);

        CARAPI OnChange(
            /* [in] */ Boolean selfChange);

        CARAPI_(String) GetValue();

    private:
        CARAPI_(void) WriteBlacklist();

    private:
        String mKey;
        String mName;
        String mPath;
        AutoPtr<IFile> mTmpDir;
        AutoPtr<IContentResolver> mContentResolver;
    };

public:
    CAR_OBJECT_DECL()

    CARAPI constructor(
        /* [in] */ IContext* context);

private:
    AutoPtr<IContentObserver> BuildPubkeyObserver(
        /* [in] */ IContentResolver* cr);

    AutoPtr<IContentObserver> BuildSerialObserver(
        /* [in] */ IContentResolver* cr);

    void RegisterObservers(
        /* [in] */ IContentResolver* cr);

    CARAPI ToString(
        /* [out] */ String* info);

private:
    static const String TAG;
    static const String BLACKLIST_ROOT;

public:
    static const String PUBKEY_PATH;
    static const String SERIAL_PATH;

    static const String PUBKEY_BLACKLIST_KEY;
    static const String SERIAL_BLACKLIST_KEY;
};

} // namespace Server
} // namespace Droid
} // namespace Elastos

#endif //__ELASTOS_DROID_SERVER_CCERTBLACKLISTER_H__
