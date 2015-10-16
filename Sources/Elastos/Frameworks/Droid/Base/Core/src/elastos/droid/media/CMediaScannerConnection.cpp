
#include "elastos/droid/media/CMediaScannerConnection.h"

#include <elastos/utility/logging/Logger.h>
#include "elastos/droid/content/CIntent.h"

using Elastos::Utility::Logging::Logger;
using Elastos::Droid::Content::IIntent;
using Elastos::Droid::Content::CIntent;

namespace Elastos {
namespace Droid {
namespace Media {

const String CMediaScannerConnection::TAG("MediaScannerConnection");

//---------------------------------------------------
//    CMediaScannerConnection::MyMediaScannerListener
//---------------------------------------------------

CAR_INTERFACE_IMPL(CMediaScannerConnection::MyMediaScannerListener, IIMediaScannerListener);

CMediaScannerConnection::MyMediaScannerListener::MyMediaScannerListener(
    /* [in] */ IWeakReference* host)
    : mWeakHost(host)
{}

ECode CMediaScannerConnection::MyMediaScannerListener::ScanCompleted(
    /* [in] */ const String& path,
    /* [in] */ IUri* uri)
{
    AutoPtr<IMediaScannerConnection> connection;
    mWeakHost->Resolve(EIID_IMediaScannerConnection, (IInterface**)&connection);
    if (connection) {
        CMediaScannerConnection* msc = (CMediaScannerConnection*)connection.Get();
        AutoPtr<IMediaScannerConnectionClient> client = msc->mClient;
        if (client != NULL) {
            client->OnScanCompleted(path, uri);
        }
    }
    return NOERROR;
}

//----------------------------------------
//    CMediaScannerConnection::ClientProxy
//----------------------------------------

CAR_INTERFACE_IMPL(CMediaScannerConnection::ClientProxy, IMediaScannerConnectionClient);

CMediaScannerConnection::ClientProxy::ClientProxy(
    /* [in] */ ArrayOf<String>* paths,
    /* [in] */ ArrayOf<String>* mimeTypes,
    /* [in] */ IOnScanCompletedListener* client)
    : mPaths(paths)
    , mMimeTypes(mimeTypes)
    , mClient(client)
    , mNextPath(0)
{}

ECode CMediaScannerConnection::ClientProxy::OnMediaScannerConnected()
{
    ScanNextPath();
    return NOERROR;
}

ECode CMediaScannerConnection::ClientProxy::OnScanCompleted(
    /* [in] */ const String& path,
    /* [in] */ IUri* uri)
{
    if (mClient != NULL) {
        mClient->OnScanCompleted(path, uri);
    }
    ScanNextPath();
    return NOERROR;
}

void CMediaScannerConnection::ClientProxy::ScanNextPath()
{
    if (mNextPath >= mPaths->GetLength()) {
        mConnection->Disconnect();
        return;
    }

    String mimeType;
    if (mMimeTypes != NULL && mNextPath < mMimeTypes->GetLength()) {
        mimeType = (*mMimeTypes)[mNextPath];
    }

    mConnection->ScanFile((*mPaths)[mNextPath], mimeType);
    mNextPath++;
}

//----------------------------------------
//    CMediaScannerConnection
//----------------------------------------

CMediaScannerConnection::CMediaScannerConnection()
    : mConnected(FALSE)
{
}

ECode CMediaScannerConnection::constructor(
    /* [in] */ IContext * context,
    /* [in] */ IMediaScannerConnectionClient* client)
{
    mContext = context;
    mClient = client;
    AutoPtr<IWeakReference> wr;
    GetWeakReference((IWeakReference**)&wr);
    mListener = new MyMediaScannerListener(wr);
    return NOERROR;
}

ECode CMediaScannerConnection::Connect()
{
   AutoLock lock(mThisLock);

   if (!mConnected) {
       AutoPtr<IIntent> intent;
       // CIntent::New(MediaScannerService.class.getName(), (IIntent**)&intent);
       CIntent::New(String("IMediaScannerService"), (IIntent**)&intent);
       Boolean tempState;
       mContext->BindService(intent, this, IContext::BIND_AUTO_CREATE, &tempState);
       mConnected = TRUE;
   }

    return NOERROR;
}

ECode CMediaScannerConnection::Disconnect()
{
    AutoLock lock(mThisLock);

    ECode ec = NOERROR;
    if (mConnected) {
        if (FALSE) {
            Logger::V(TAG, "Disconnecting from Media Scanner");
        }

//      try {
        ec = mContext->UnbindService(this);
    //      } catch (IllegalArgumentException ex) {
        if (ec == (ECode)E_ILLEGAL_ARGUMENT_EXCEPTION) {
            if (FALSE) {
                Logger::V(TAG, "disconnect failed: " /*+ ex*/);
            }
            ec = NOERROR;
        }
    //      }
        mConnected = FALSE;
    }

    return ec;
}

/*synchronized*/
ECode CMediaScannerConnection::IsConnected(
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    AutoLock lock(mThisLock);

    *result = mService != NULL && mConnected;
    return NOERROR;
}

ECode CMediaScannerConnection::ScanFile(
    /* [in] */ const String& path,
    /* [in] */ const String& mimeType)
{
    AutoLock lock(mThisLock);

    if (mService == NULL || !mConnected) {
        Logger::E(TAG, "not connected to MediaScannerService");
        return E_ILLEGAL_STATE_EXCEPTION;
    }

//        try {
    if (FALSE) {
        Logger::V(TAG, "Scanning file %s", path.string());
    }
    ECode ec = mService->RequestScanFile(path, mimeType, mListener);
//        } catch (RemoteException e) {
    if (ec == (ECode)E_REMOTE_EXCEPTION) {
        if (FALSE) {
            Logger::D(TAG, "Failed to scan file %s", path.string());
        }
        ec = NOERROR;
    }
//        }

    return ec;
}

ECode CMediaScannerConnection::OnServiceConnected(
    /* [in] */ IComponentName* className,
    /* [in] */ IBinder* service)
{
    if (FALSE) {
        Logger::V(TAG, "Connected to Media Scanner");
    }

    AutoLock lock(mThisLock);

    mService = IIMediaScannerService::Probe(service);
    if (mService != NULL && mClient != NULL) {
        mClient->OnMediaScannerConnected();
    }

    return NOERROR;
}

ECode CMediaScannerConnection::OnServiceDisconnected(
    /* [in] */ IComponentName* className)
{
    if (FALSE) {
        Logger::V(TAG, "Disconnected from Media Scanner");
    }

    AutoLock lock(mThisLock);

    mService = NULL;

    return NOERROR;
}

/*static */
ECode CMediaScannerConnection::ScanFile(
    /* [in] */ IContext* context,
    /* [in] */ ArrayOf<String>* paths,
    /* [in] */ ArrayOf<String>* mimeTypes,
    /* [in] */ IOnScanCompletedListener* cb)
{
    AutoPtr<ClientProxy> client = new ClientProxy(paths, mimeTypes, cb);
    AutoPtr<CMediaScannerConnection> connection;
    CMediaScannerConnection::NewByFriend(context, client, (CMediaScannerConnection**)&connection);
    client->mConnection = (IMediaScannerConnection*)connection.Get();
    connection->Connect();
    return NOERROR;
}

} // namespace Media
} // namepsace Droid
} // namespace Elastos
