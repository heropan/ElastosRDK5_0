
#include "os/SystemClock.h"
#include "os/FileUtils.h"
#include "os/Binder.h"
#include "connectivity/Vpn.h"
#include "connectivity/CVpnObserver.h"
#include "R.h"
#include "Manifest.h"
#include <elastos/core/StringUtils.h>
#include <elastos/utility/logging/Logger.h>
#include <netinet/in.h>
#include <linux/if.h>
#include <linux/if_tun.h>
#include <linux/route.h>
#include <linux/ipv6_route.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>

using Elastos::Core::StringUtils;
using Elastos::Core::ICharSequence;
using Elastos::Core::CObjectContainer;
using Elastos::Core::CStringWrapper;
using Elastos::IO::IFile;
using Elastos::IO::CFile;
using Elastos::IO::IIoUtils;
using Elastos::IO::CIoUtils;
using Elastos::IO::IOutputStream;
using Elastos::IO::IInputStream;
using Elastos::IO::IFlushable;
using Elastos::Net::IInet4Address;
using Elastos::Utility::Logging::Logger;
using Elastos::Droid::R;
using Elastos::Droid::Content::EIID_IServiceConnection;
using Elastos::Droid::Content::CIntent;
using Elastos::Droid::Content::Pm::IPackageManager;
using Elastos::Droid::Content::Pm::IApplicationInfo;
using Elastos::Droid::Content::Pm::IResolveInfo;
using Elastos::Droid::Content::Pm::IServiceInfo;
using Elastos::Droid::Content::Res::IResources;
using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IBitmapFactory;
using Elastos::Droid::Graphics::CBitmapFactory;
using Elastos::Droid::Graphics::Drawable::IDrawable;
using Elastos::Droid::Net::NetworkInfoDetailedState_DISCONNECTED;
using Elastos::Droid::Net::NetworkInfoDetailedState_CONNECTING;
using Elastos::Droid::Net::NetworkInfoDetailedState_FAILED;
using Elastos::Droid::Net::NetworkInfoDetailedState_CONNECTED;
using Elastos::Droid::Net::NetworkInfoDetailedState_IDLE;
using Elastos::Droid::Net::NetworkInfoDetailedState_AUTHENTICATING;
using Elastos::Droid::Net::CLocalSocket;
using Elastos::Droid::Net::ILocalSocketAddress;
using Elastos::Droid::Net::CLocalSocketAddress;
using Elastos::Droid::Net::LocalSocketAddressNamespace_RESERVED;
using Elastos::Droid::Net::IConnectivityManager;
using Elastos::Droid::Net::CNetworkInfo;
using Elastos::Droid::Os::SystemClock;
using Elastos::Droid::Os::FileUtils;
using Elastos::Droid::Os::Binder;
using Elastos::Droid::Internal::Net::CLegacyVpnInfo;
using Elastos::Droid::Internal::Net::IVpnConfigHelper;
using Elastos::Droid::Internal::Net::CVpnConfigHelper;

namespace Elastos {
namespace Droid {
namespace Server {
namespace Connectivity {

static Int32 inet4 = -1;
static Int32 inet6 = -1;

static inline in_addr_t *as_in_addr(sockaddr *sa) {
    return &((sockaddr_in *)sa)->sin_addr.s_addr;
}

//=============================================================================
// Vpn::Connection
//=============================================================================
CAR_INTERFACE_IMPL(Vpn::Connection, IServiceConnection);

ECode Vpn::Connection::OnServiceConnected(
    /* [in] */ IComponentName* name,
    /* [in] */ IBinder* service)
{
    mService = service;
    return NOERROR;
}

ECode Vpn::Connection::OnServiceDisconnected(
    /* [in] */ IComponentName* name)
{
    mService = NULL;
    return NOERROR;
}


//=============================================================================
// Vpn::LegacyVpnRunner
//=============================================================================
const String Vpn::LegacyVpnRunner::TAG("LegacyVpnRunner");
Mutex Vpn::LegacyVpnRunner::sTAGLock;

Vpn::LegacyVpnRunner::LegacyVpnRunner(
    /* [in] */ IVpnConfig* config,
    /* [in] */ ArrayOf<String>* racoon,
    /* [in] */ ArrayOf<String>* mtpd,
    /* [in] */ Vpn* owner)
    : mTimer(-1)
{
    Thread::Init(TAG);

    mConfig = config;
    mDaemons = ArrayOf<String>::Alloc(2);
    (*mDaemons)[0] = "racoon";
    (*mDaemons)[1] = "mtpd";
    // TODO: clear arguments from memory once launched
    mArguments = ArrayOf<StringArray>::Alloc(2);
    mArguments->Set(0, racoon);
    mArguments->Set(1, mtpd);
    mSockets = ArrayOf<ILocalSocket*>::Alloc(mDaemons->GetLength());

    // This is the interface which VPN is running on,
    // mConfig.interfaze will change to point to OUR
    // internal interface soon. TODO - add inner/outer to mconfig
    mConfig->GetInterfaze(&mOuterInterface);
}

ECode Vpn::LegacyVpnRunner::Check(
    /* [in] */ const String& interfaze)
{
    if (interfaze.Equals(mOuterInterface)){
        Logger::I(TAG, "Legacy VPN is going down with %s", interfaze.string());
        Exit();
    }
    return NOERROR;
}

ECode Vpn::LegacyVpnRunner::Exit()
{
    // We assume that everything is reset after stopping the daemons.
    Interrupt();
    AutoPtr<IIoUtils> ioUtils;
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    for (Int32 i = 0; i < mSockets->GetLength(); i++) {
        ioUtils->CloseQuietly((*mSockets)[i]);
    }
    mOwner->UpdateState(NetworkInfoDetailedState_DISCONNECTED, String("exit"));
    return NOERROR;
}

ECode Vpn::LegacyVpnRunner::Run()
{
    // Wait for the previous thread since it has been interrupted.
    Logger::V(TAG, "Waiting");
    {
        Mutex::Autolock lock(sTAGLock);
        Logger::V(TAG, "Executing");
        Execute();
        MonitorDaemons();
    }
    return NOERROR;
}

ECode Vpn::LegacyVpnRunner::Checkpoint(
    /* [in] */ Boolean yield)
{
    Int64 now = SystemClock::GetElapsedRealtime();
    if (mTimer == -1) {
        mTimer = now;
        Thread::Sleep(1);
    }
    else if (now - mTimer <= 60000) {
        Thread::Sleep(yield ? 200 : 1);
    }
    else {
        mOwner->UpdateState(NetworkInfoDetailedState_FAILED, String("checkpoint"));
        // throw new IllegalStateException("Time is up");
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode Vpn::LegacyVpnRunner::Execute()
{
    AutoPtr<ISystemService> systemService;
    AutoPtr<IFile> state, state2;
    AutoPtr< ArrayOf<String> > parameters;
    String readfile, interfaze, routes;
    AutoPtr<IObjectContainer> dnsServers, searchDomains;
    // Catch all exceptions so we can clean up few things.
    Boolean initFinished = FALSE;
    Boolean restart = FALSE, result;
    // try {
    // Initialize the timer.
    ECode ec = Checkpoint(FALSE);
    if(FAILED(ec)) goto ERROR;

    CSystemService::AcquireSingleton((ISystemService**)&systemService);
    // Wait for the daemons to stop.
    for (Int32 i = 0; i < mDaemons->GetLength(); i++){
        String daemon = (*mDaemons)[i];
        Boolean isStopped = FALSE;
        systemService->IsStopped(daemon, &isStopped);
        while(!isStopped){
            ec = Checkpoint(TRUE);
            if(FAILED(ec)) goto ERROR;
        }
    }

    // Clear the previous state.
    CFile::New(String("/data/misc/vpn/state"), (IFile**)&state);
    state->Delete(&result);
    if(state->Exists(&result), result) {
        //throw new IllegalStateException("Cannot delete the state");
        ec = E_ILLEGAL_STATE_EXCEPTION;
        goto ERROR;
    }

    CFile::New(String("/data/misc/vpn/abort"), (IFile**)&state2);
    state2->Delete(&result);
    initFinished = TRUE;

    // Check if we need to restart any of the daemons.
    for (Int32 i = 0; i < mArguments->GetLength(); i++){
        restart = restart || ((*mArguments)[i] != NULL);
    }
    if (!restart) {
        mOwner->UpdateState(NetworkInfoDetailedState_DISCONNECTED, String("execute"));
        ec = NOERROR;
        goto RETURN;
    }
    mOwner->UpdateState(NetworkInfoDetailedState_CONNECTING, String("execute"));

    // Start the daemon with arguments.
    for (Int32 i = 0; i < mDaemons->GetLength(); ++i) {
        AutoPtr<ArrayOf<String> > arguments = (*mArguments)[i];
        if (arguments == NULL) {
            continue;
        }

        // Start the daemon.
        String daemon = (*mDaemons)[i];
        systemService->Start(daemon);

        // Wait for the daemon to start.
        systemService->IsRunning(daemon, &result);
        while (!result) {
            ec = Checkpoint(TRUE);
            if(FAILED(ec)) goto ERROR;
        }

        // Create the control socket.
        AutoPtr<ILocalSocket> socket;
        CLocalSocket::New((ILocalSocket**)&socket);
        mSockets->Set(i, socket);
        AutoPtr<ILocalSocketAddress> address;
        CLocalSocketAddress::New(daemon, LocalSocketAddressNamespace_RESERVED, (ILocalSocketAddress**)&address);

        // Wait for the socket to connect.
        while (TRUE) {
            ec = (*mSockets)[i]->Connect(address);
            if(FAILED(ec)){
                ec = Checkpoint(TRUE);
                if(FAILED(ec)) goto ERROR;
            }
            else {
                break;
            }
        }
        (*mSockets)[i]->SetSoTimeout(500);

        // Send over the arguments.
        AutoPtr<IOutputStream> out;
        (*mSockets)[i]->GetOutputStream((IOutputStream**)&out);
        for (Int32 j = 0; j < arguments->GetLength(); ++j){
            String argument = (*arguments)[j];
            AutoPtr< ArrayOf<Byte> > bytes = ArrayOf<Byte>::Alloc(argument.GetLength());
            bytes->Copy((Byte*)argument.string(), bytes->GetLength());
            if (bytes->GetLength() >= 0xFFFF) {
                //throw new IllegalArgumentException("Argument is too large");
                ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
                goto ERROR;
            }
            out->Write(bytes->GetLength() >> 8);
            out->Write(bytes->GetLength());
            out->WriteBytes(*bytes);
            Checkpoint(FALSE);
        }
        out->Write(0xFF);
        out->Write(0xFF);
        IFlushable::Probe(out)->Flush();

        // Wait for End-of-File.
        AutoPtr<IInputStream> in;
        (*mSockets)[i]->GetInputStream((IInputStream**)&in);
        while (TRUE) {
            Int32 value;
            ec = in->Read(&value);
            if (SUCCEEDED(ec) && value == -1) {
                break;
            }
            ec = Checkpoint(TRUE);
            if(FAILED(ec)) goto ERROR;
        }
    }

    // Wait for the daemons to create the new state.
    while (state->Exists(&result), !result) {
        // Check if a running daemon is dead.
        for (Int32 i = 0; i < mDaemons->GetLength(); ++i) {
            String daemon = (*mDaemons)[i];
            Boolean isRunning;
            if ((*mArguments)[i] != NULL &&
                    (systemService->IsRunning(daemon, &isRunning), !isRunning)) {
                //throw new IllegalStateException(daemon + " is dead");
                ec = E_ILLEGAL_STATE_EXCEPTION;
                goto ERROR;
            }
        }
        ec = Checkpoint(TRUE);
        if(FAILED(ec)) goto ERROR;
    }

    // Now we are connected. Read and parse the new state.
    FileUtils::ReadTextFile(state, 0, String(NULL), &readfile);
    StringUtils::Split(readfile, String("\n"), -1, (ArrayOf<String>**)&parameters);
    if (parameters->GetLength() != 6) {
        //throw new IllegalStateException("Cannot parse the state");
        ec = E_ILLEGAL_STATE_EXCEPTION;
        goto ERROR;
    }

    // Set the interface and the addresses in the config.
    mConfig->SetInterfaze((*parameters)[0].Trim());
    mConfig->SetAddresses((*parameters)[1].Trim());

    mConfig->GetRoutes(&routes);
    // Set the routes if they are not set in the config.
    if (routes.IsNullOrEmpty()) {
        mConfig->SetRoutes((*parameters)[2].Trim());
    }

    // Set the DNS servers if they are not set in the config.
    mConfig->GetDnsServers((IObjectContainer**)&dnsServers);
    Int32 count;
    if (dnsServers == NULL || (dnsServers->GetObjectCount(&count), count == 0)) {
        String dnsServersStr = (*parameters)[3].Trim();
        if (!dnsServersStr.IsEmpty()) {
            AutoPtr< ArrayOf<String> > servers;
            StringUtils::Split(dnsServersStr, String(" "), (ArrayOf<String>**)&servers);
            dnsServers = NULL;
            CObjectContainer::New((IObjectContainer**)&dnsServers);
            for (Int32 i = 0; i < servers->GetLength(); i++) {
                AutoPtr<ICharSequence> server;
                CStringWrapper::New((*servers)[i], (ICharSequence**)&server);
                dnsServers->Add(server);
            }
            mConfig->SetDnsServers(dnsServers);
        }
    }

    // Set the search domains if they are not set in the config.
    mConfig->GetSearchDomains((IObjectContainer**)&searchDomains);
    if (searchDomains == NULL || (searchDomains->GetObjectCount(&count), count == 0)) {
        String searchDomainsStr = (*parameters)[4].Trim();
        if (!searchDomainsStr.IsEmpty()) {
            AutoPtr< ArrayOf<String> > domains;
            StringUtils::Split(searchDomainsStr, String(" "), (ArrayOf<String>**)&domains);
            searchDomains = NULL;
            CObjectContainer::New((IObjectContainer**)&searchDomains);
            for (Int32 i = 0; i < domains->GetLength(); i++) {
                AutoPtr<ICharSequence> domain;
                CStringWrapper::New((*domains)[i], (ICharSequence**)&domain);
                searchDomains->Add(domain);
            }
            mConfig->SetSearchDomains(searchDomains);
        }
    }

    // Set the routes.
    mConfig->GetInterfaze(&interfaze);
    mConfig->GetRoutes(&routes);
    if (mOwner->NativeSetRoutes(interfaze, routes) < 0) {
        ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
        goto ERROR;
    }

    {
        Mutex::Autolock lock(mOwner->mLock);
        // Check if the thread is interrupted while we are waiting.
        ec = Checkpoint(FALSE);
        if(FAILED(ec)) goto ERROR;

        // Check if the interface is gone while we are waiting.
        if (Check(interfaze) == 0) {
            //throw new IllegalStateException(mConfig.interfaze + " is gone");
            ec = E_ILLEGAL_STATE_EXCEPTION;
            goto ERROR;
        }

        // Now NetworkManagementEventObserver is watching our back.
        mConfig->GetInterfaze(&mOwner->mInterface);
        mOwner->mCallback->Override(dnsServers, searchDomains);
        mOwner->ShowNotification(mConfig, String(NULL), NULL);

        Logger::I(TAG, "Connected!");
        mOwner->UpdateState(NetworkInfoDetailedState_CONNECTED, String("execute"));
    }
    // } catch (Exception e) {
    //     Log.i(TAG, "Aborting", e);
    //     exit();
    // }
ERROR:
    // Log.i(TAG, "Aborting", e);
    Exit();
    // } finally {
RETURN:
    if (!initFinished) {
        for(Int32 i = 0; i < mDaemons->GetLength(); ++i){
            String daemon = (*mDaemons)[i];
            systemService->Stop(daemon);
        }
    }

    // Do not leave an unstable state.
    NetworkInfoDetailedState niState;
    if (!initFinished ||
            (mOwner->mNetworkInfo->GetDetailedState(&niState), niState == NetworkInfoDetailedState_CONNECTING)) {
        mOwner->UpdateState(NetworkInfoDetailedState_FAILED, String("execute"));
    }
    // }
    return ec;
}

/**
 * Monitor the daemons we started, moving to disconnected state if the
 * underlying services fail.
 */
void Vpn::LegacyVpnRunner::MonitorDaemons()
{
	Boolean result;
    if (mOwner->mNetworkInfo->IsConnected(&result), !result) {
        return;
    }

    AutoPtr<ISystemService> systemService;
    CSystemService::AcquireSingleton((ISystemService**)&systemService);
    // try {
    while (TRUE) {
        Thread::Sleep(2000);
        for (Int32 i = 0; i < mDaemons->GetLength(); i++) {
            Boolean result;
            if ((*mArguments)[i] != NULL &&
                    (systemService->IsStopped((*mDaemons)[i], &result), result)) {
                goto RETURN;
            }
        }
    }
    // } catch (InterruptedException e) {
    //     Log.d(TAG, "interrupted during monitorDaemons(); stopping services");
    // } finally {
RETURN:
    for (Int32 i = 0; i < mDaemons->GetLength(); i++) {
        String daemon = (*mDaemons)[i];
        systemService->Stop(daemon);
    }

    mOwner->UpdateState(NetworkInfoDetailedState_DISCONNECTED, String("babysit"));
    // }
}


////////////////////////////////////////////////////////////////////////////////
// Vpn
const String Vpn::TAG("Vpn");
const Boolean Vpn::LOGD = TRUE;

Vpn::Vpn(
    /* [in] */ IContext* context,
    /* [in] */ CConnectivityService::VpnCallback* callback,
    /* [in] */ INetworkManagementService* netService)
    : BaseNetworkStateTracker(IConnectivityManager::TYPE_DUMMY)
    , mCallback(callback)
    , mPackage(IVpnConfig::LEGACY_VPN)
    , mEnableNotif(TRUE)
{
    mContext = context;
    CVpnObserver::New((Handle32)this, (INetworkManagementEventObserver**)&mObserver);
    ECode ec = netService->RegisterObserver(mObserver);
    if (FAILED(ec)) {
        //Log.wtf(TAG, "Problem registering observer", e);
    }
}

ECode Vpn::SetEnableNotifications(
    /* [in] */ Boolean enableNotif)
{
    mEnableNotif = enableNotif;
    return NOERROR;
}

void Vpn::StartMonitoringInternal()
{
    // Ignored; events are sent through callbacks for now
}

ECode Vpn::Teardown(
    /* [out] */ Boolean* result)
{
    // TODO: finish migration to unique tracker for each VPN
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Vpn::Reconnect(
    /* [out] */ Boolean* result)
{
    // TODO: finish migration to unique tracker for each VPN
    return E_UNSUPPORTED_OPERATION_EXCEPTION;
}

ECode Vpn::GetTcpBufferSizesPropName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = PROP_TCP_BUFFER_UNKNOWN;
    return NOERROR;
}

/**
* Update current state, dispaching event to listeners.
*/
void Vpn::UpdateState(
    /* [in] */ NetworkInfoDetailedState detailedState,
    /* [in] */ const String& reason)
{
    if (LOGD) {
        Logger::D(TAG, "setting state=%d, reason=%s", detailedState, reason.string());
    }
    mNetworkInfo->SetDetailedState(detailedState, reason, String(NULL));
    AutoPtr<INetworkInfo> network;
    CNetworkInfo::New(mNetworkInfo, (INetworkInfo**)&network);
    mCallback->OnStateChanged(network);
}

/**
 * Prepare for a VPN application. This method is designed to solve
 * race conditions. It first compares the current prepared package
 * with {@code oldPackage}. If they are the same, the prepared
 * package is revoked and replaced with {@code newPackage}. If
 * {@code oldPackage} is {@code null}, the comparison is omitted.
 * If {@code newPackage} is the same package or {@code null}, the
 * revocation is omitted. This method returns {@code true} if the
 * operation is succeeded.
 *
 * Legacy VPN is handled specially since it is not a real package.
 * It uses {@link VpnConfig#LEGACY_VPN} as its package name, and
 * it can be revoked by itself.
 *
 * @param oldPackage The package name of the old VPN application.
 * @param newPackage The package name of the new VPN application.
 * @return true if the operation is succeeded.
 */
ECode Vpn::Prepare(
    /* [in] */ const String& oldPackage,
    /* [in] */ const String& newPackage,
    /* [out] */ Boolean* result)
{
    VALIDATE_NOT_NULL(result);

    Mutex::Autolock lock(mLock);
    // Return false if the package does not match.
    if (!oldPackage.IsNull() && !oldPackage.Equals(mPackage)) {
        *result = FALSE;
        return NOERROR;
    }

    // Return true if we do not need to revoke.
    if (newPackage.IsNull() ||
            (newPackage.Equals(mPackage) && !newPackage.Equals(IVpnConfig::LEGACY_VPN))) {
        *result = TRUE;
        return NOERROR;
    }

    // Check if the caller is authorized.
    FAIL_RETURN(EnforceControlPermission());

    // Reset the interface and hide the notification.
    if (!mInterface.IsNull()) {
        NativeReset(mInterface);
        Int64 token = Binder::ClearCallingIdentity();
        mCallback->Restore();
        HideNotification();
        Binder::RestoreCallingIdentity(token);
        mInterface = NULL;
    }

    // Revoke the connection or stop LegacyVpnRunner.
    //alex maybe have return value:Parcel::Obtain()
    if (mConnection != NULL) {
        assert(0);
//        mConnection->mService->Transact(IBinder::LAST_CALL_TRANSACTION,
//                            Parcel::Obtain(), NULL, IBinder::FLAG_ONEWAY);
        mContext->UnbindService(mConnection);
        mConnection = NULL;
    }
    else if (mLegacyVpnRunner != NULL) {
        mLegacyVpnRunner->Exit();
        mLegacyVpnRunner = NULL;
    }

//    Logger::I(TAG, StringBuffer("Switched from ") + mPackage + " to " + newPackage);
    mPackage = newPackage;
    UpdateState(NetworkInfoDetailedState_IDLE, String("prepare"));
    *result = TRUE;
    return NOERROR;
}

/**
 * Protect a socket from routing changes by binding it to the given
 * interface. The socket is NOT closed by this method.
 *
 * @param socket The socket to be bound.
 * @param interfaze The name of the interface.
 */
ECode Vpn::Protect(
    /* [in] */ IParcelFileDescriptor *socket,
    /* [in] */ const String& interfaze)
{
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IApplicationInfo> app;
    pm->GetApplicationInfo(mPackage, 0, (IApplicationInfo**)&app);

    Int32 uid;
    app->GetUid(&uid);
    if (Binder::GetCallingUid() != uid) {
        // throw new SecurityException("Unauthorized Caller");
        return E_SECURITY_EXCEPTION;
    }
    Int32 fd;
    socket->GetFd(&fd);
    return NativeProtect(fd, interfaze);
}

/**
 * Establish a VPN network and return the file descriptor of the VPN
 * interface. This methods returns {@code null} if the application is
 * revoked or not prepared.
 *
 * @param config The parameters to configure the network.
 * @return The file descriptor of the VPN interface.
 */
ECode Vpn::Establish(
   /* [in] */ IVpnConfig* config,
   /* [out] */ IParcelFileDescriptor** fd)
{
    VALIDATE_NOT_NULL(fd);
    *fd = NULL;

    Mutex::Autolock lock(mLock);

    // Check if the caller is already prepared.
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IApplicationInfo> app;
    ECode ec = pm->GetApplicationInfo(mPackage, 0, (IApplicationInfo**)&app);
    if(FAILED(ec)){
        return NOERROR;
    }
    Int32 uid;
    app->GetUid(&uid);
    if (Binder::GetCallingUid() != uid) {
        return NOERROR;
    }

    // Check if the service is properly declared.
    AutoPtr<IIntent> intent;
    CIntent::New(IVpnConfig::SERVICE_INTERFACE, (IIntent**)&intent);
    String user;
    config->GetUser(&user);
    intent->SetClassName(mPackage, user);
    AutoPtr<IResolveInfo> info;
    pm->ResolveService(intent, 0, (IResolveInfo**)&info);
    if (info == NULL) {
        //throw new SecurityException("Cannot find " + config.user);
        return E_SECURITY_EXCEPTION;
    }
    AutoPtr<IServiceInfo> sinfo;
    info->GetServiceInfo((IServiceInfo**)&sinfo);
    String permission;
    sinfo->GetPermission(&permission);
    if (!Elastos::Droid::Manifest::Permission::BIND_VPN_SERVICE.Equals(permission)) {
        // throw new SecurityException(config.user + " does not require " + BIND_VPN_SERVICE);
        return E_SECURITY_EXCEPTION;
    }

    // Load the label.
    String label;
    AutoPtr<ICharSequence> csLabel;
    app->LoadLabel(pm, (ICharSequence**)&csLabel);
    csLabel->ToString((String*)&label);

    // Load the icon and convert it into a bitmap.
    AutoPtr<IDrawable> icon;
    app->LoadIcon(pm, (IDrawable**)&icon);
    AutoPtr<IBitmap> bitmap;
    Int32 iconwidth, iconheight;
    icon->GetIntrinsicWidth(&iconwidth);
    icon->GetIntrinsicHeight(&iconheight);
    if (iconwidth > 0 && iconheight > 0) {
        AutoPtr<IResources> resource;
        mContext->GetResources((IResources**)&resource);
        Int32 width;
        resource->GetDimensionPixelSize(
                R::dimen::notification_large_icon_width, &width);
        Int32 height;
        resource->GetDimensionPixelSize(
                R::dimen::notification_large_icon_height, &height);
        icon->SetBounds(0, 0, width, height);

        AutoPtr<IBitmapFactory> factory;
        CBitmapFactory::AcquireSingleton((IBitmapFactory**)&factory);
        factory->CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&bitmap);
        AutoPtr<ICanvas> c;
        CCanvas::New(bitmap, (ICanvas**)&c);
        icon->Draw(c);
        c->SetBitmap(NULL);
    }

    // Configure the interface. Abort if any of these steps fails.
    Int32 mtu, nfd;
    config->GetMtu(&mtu);
    FAIL_RETURN(NativeCreate(mtu, &nfd));
    AutoPtr<IParcelFileDescriptorHelper> helper;
    CParcelFileDescriptorHelper::AcquireSingleton((IParcelFileDescriptorHelper**)&helper);
    AutoPtr<IParcelFileDescriptor> tun;
    helper->AdoptFd(nfd, (IParcelFileDescriptor**)&tun);
    // try {
    String addresses, routes;
    AutoPtr<Connection> connection;
    AutoPtr<IIoUtils> ioUtils;
    UpdateState(NetworkInfoDetailedState_CONNECTING, String("establish"));
    tun->GetFd(&nfd);
    String interfaze;
    ec = NativeGetName(nfd, &interfaze);
    if (FAILED(ec)) goto ERROR;
    config->GetAddresses(&addresses);
    if (NativeSetAddresses(interfaze, addresses) < 1) {
        //throw new IllegalArgumentException("At least one address must be specified");
        ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
        goto ERROR;
    }
    config->GetRoutes(&routes);
    if (!routes.IsNull()) {
        if (NativeSetRoutes(interfaze, routes) < 0) {
            ec = E_ILLEGAL_ARGUMENT_EXCEPTION;
            goto ERROR;
        }
    }
    connection = new Connection(this);
    Boolean result;
    if (FAILED(mContext->BindService(intent, connection, IContext::BIND_AUTO_CREATE, &result)), !result) {
        //throw new IllegalStateException("Cannot bind " + config.user);
        ec = E_ILLEGAL_STATE_EXCEPTION;
        goto ERROR;
    }
    if (mConnection != NULL) {
        mContext->UnbindService(mConnection);
    }
    if (!mInterface.IsNull() && !mInterface.Equals(interfaze)) {
        ec = NativeReset(mInterface);
        if (FAILED(ec)) goto ERROR;
    }
    mConnection = connection;
    mInterface = interfaze;
    goto NEXT;
    // } catch (RuntimeException e) {
ERROR:
    UpdateState(NetworkInfoDetailedState_FAILED, String("establish"));
    CIoUtils::AcquireSingleton((IIoUtils**)&ioUtils);
    ioUtils->CloseQuietly(tun);
    return ec;
    // }
NEXT:
    // Logger::I(TAG, "Established by " + config->user + " on " + mInterface);

    // Fill more values.
    config->SetUser(mPackage);
    config->SetInterfaze(mInterface);

    // Override DNS servers and show the notification.
    const Int64 token = Binder::ClearCallingIdentity();
    AutoPtr<IObjectContainer> dnsServers, searchDomains;
    config->GetDnsServers((IObjectContainer**)&dnsServers);
    config->GetSearchDomains((IObjectContainer**)&searchDomains);
    mCallback->Override(dnsServers, searchDomains);
    ShowNotification(config, label, bitmap);
    Binder::RestoreCallingIdentity(token);
    // TODO: ensure that contract class eventually marks as connected
    UpdateState(NetworkInfoDetailedState_AUTHENTICATING, String("establish"));
    *fd = tun;
    INTERFACE_ADDREF(*fd);
    return NOERROR;
}

ECode Vpn::InterfaceStatusChanged(
    /* [in] */ const String& iface,
    /* [in] */ Boolean up)
{
    Mutex::Autolock lock(mLock);
    return mObserver->InterfaceStatusChanged(iface, up);
}

ECode Vpn::EnforceControlPermission()
{
    // System user is allowed to control VPN.
    if (Binder::GetCallingUid() == IProcess::SYSTEM_UID) {
       return NOERROR;
    }

    // System dialogs are also allowed to control VPN.
    AutoPtr<IPackageManager> pm;
    mContext->GetPackageManager((IPackageManager**)&pm);
    AutoPtr<IApplicationInfo> app;
    pm->GetApplicationInfo(IVpnConfig::DIALOGS_PACKAGE, 0, (IApplicationInfo**)&app);
    Int32 uid;
    app->GetUid(&uid);
    if (Binder::GetCallingUid() == uid) {
        return NOERROR;
    }

    //throw new SecurityException("Unauthorized Caller");
    return E_SECURITY_EXCEPTION;
}

void Vpn::ShowNotification(
    /* [in] */ IVpnConfig* config,
    /* [in] */ const String& label,
    /* [in] */ IBitmap* icon)
{
    if (!mEnableNotif) return;
    AutoPtr<IVpnConfigHelper> helper;
    CVpnConfigHelper::AcquireSingleton((IVpnConfigHelper**)&helper);
    mStatusIntent = NULL;
    helper->GetIntentForStatusPanel(mContext, config, (IPendingIntent**)&mStatusIntent);

    AutoPtr<IInterface> service;
    mContext->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&service);
    AutoPtr<INotificationManager> nm = INotificationManager::Probe(service);

    if (nm != NULL) {
        String title;
        if (label.IsNull()) {
            mContext->GetString(R::string::vpn_title, &title);
        }
        else {
            AutoPtr<ICharSequence> csLabel;
            CStringWrapper::New(label, (ICharSequence**)&csLabel);
            AutoPtr< ArrayOf<IInterface*> > format = ArrayOf<IInterface*>::Alloc(1);
            format->Set(0, csLabel.Get());
            mContext->GetString(R::string::vpn_title_long, format, &title);
        }
        String text;
        String session;
        config->GetSession(&session);
        if (session.IsNull()) {
            mContext->GetString(R::string::vpn_text, &text);
        }
        else {
            AutoPtr<ICharSequence> csSession;
            CStringWrapper::New(session, (ICharSequence**)&csSession);
            AutoPtr< ArrayOf<IInterface*> > format = ArrayOf<IInterface*>::Alloc(1);
            format->Set(0, csSession.Get());
            mContext->GetString(R::string::vpn_text_long, format, &text);
        }
        config->SetStartTime(SystemClock::GetElapsedRealtime());

        AutoPtr<INotificationBuilder> builder;
        CNotificationBuilder::New(mContext, (INotificationBuilder**)&builder);
        builder->SetSmallIcon(R::drawable::vpn_connected);
        builder->SetLargeIcon(icon);
        AutoPtr<ICharSequence> csTitle;
        CStringWrapper::New(title, (ICharSequence**)&csTitle);
        builder->SetContentTitle(csTitle);
        AutoPtr<ICharSequence> csText;
        CStringWrapper::New(text, (ICharSequence**)&csText);
        builder->SetContentText(csText);
        builder->SetContentIntent(mStatusIntent);
        builder->SetDefaults(0);
        builder->SetOngoing(TRUE);
        AutoPtr<INotification> notification;
        builder->Build((INotification**)&notification);
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        AutoPtr<IUserHandle> ALL;
        helper->GetALL((IUserHandle**)&ALL);
        nm->NotifyAsUser(String(NULL), R::drawable::vpn_connected, notification, ALL);
    }
}

void Vpn::HideNotification()
{
    if (!mEnableNotif) return;
    mStatusIntent = NULL;

    AutoPtr<IInterface> service;
    mContext->GetSystemService(IContext::NOTIFICATION_SERVICE, (IInterface**)&service);
    AutoPtr<INotificationManager> nm = INotificationManager::Probe(service);

    if (nm != NULL) {
        AutoPtr<IUserHandleHelper> helper;
        CUserHandleHelper::AcquireSingleton((IUserHandleHelper**)&helper);
        AutoPtr<IUserHandle> ALL;
        helper->GetALL((IUserHandle**)&ALL);
        nm->CancelAsUser(String(NULL), R::drawable::vpn_connected, ALL);
    }
}

#define SYSTEM_ERROR -1
#define BAD_ARGUMENT -2

static int create_interface(int mtu)
{
    int tun = open("/dev/tun", O_RDWR | O_NONBLOCK);

    ifreq ifr4;
    memset(&ifr4, 0, sizeof(ifr4));

    // Allocate interface.
    ifr4.ifr_flags = IFF_TUN | IFF_NO_PI;
    if (ioctl(tun, TUNSETIFF, &ifr4)) {
        Logger::E("Vpn", "Cannot allocate TUN: %s", strerror(errno));
        goto error;
    }

    // Activate interface.
    ifr4.ifr_flags = IFF_UP;
    if (ioctl(inet4, SIOCSIFFLAGS, &ifr4)) {
        Logger::E("Vpn", "Cannot activate %s: %s", ifr4.ifr_name, strerror(errno));
        goto error;
    }

    // Set MTU if it is specified.
    ifr4.ifr_mtu = mtu;
    if (mtu > 0 && ioctl(inet4, SIOCSIFMTU, &ifr4)) {
        Logger::E("Vpn", "Cannot set MTU on %s: %s", ifr4.ifr_name, strerror(errno));
        goto error;
    }

    return tun;

error:
    close(tun);
    return SYSTEM_ERROR;
}

static int get_interface_name(char *name, int tun)
{
    ifreq ifr4;
    if (ioctl(tun, TUNGETIFF, &ifr4)) {
        Logger::E("Vpn", "Cannot get interface name: %s", strerror(errno));
        return SYSTEM_ERROR;
    }
    strncpy(name, ifr4.ifr_name, IFNAMSIZ);
    return 0;
}

static int get_interface_index(const char *name)
{
    ifreq ifr4;
    strncpy(ifr4.ifr_name, name, IFNAMSIZ);
    if (ioctl(inet4, SIOGIFINDEX, &ifr4)) {
        Logger::E("Vpn", "Cannot get index of %s: %s", name, strerror(errno));
        return SYSTEM_ERROR;
    }
    return ifr4.ifr_ifindex;
}

static int set_addresses(const char *name, const char *addresses)
{
    int index = get_interface_index(name);
    if (index < 0) {
        return index;
    }

    ifreq ifr4;
    memset(&ifr4, 0, sizeof(ifr4));
    strncpy(ifr4.ifr_name, name, IFNAMSIZ);
    ifr4.ifr_addr.sa_family = AF_INET;
    ifr4.ifr_netmask.sa_family = AF_INET;

    in6_ifreq ifr6;
    memset(&ifr6, 0, sizeof(ifr6));
    ifr6.ifr6_ifindex = index;

    char address[65];
    int prefix;
    int chars;
    int count = 0;

    while (sscanf(addresses, " %64[^/]/%d %n", address, &prefix, &chars) == 2) {
        addresses += chars;

        if (strchr(address, ':')) {
            // Add an IPv6 address.
            if (inet_pton(AF_INET6, address, &ifr6.ifr6_addr) != 1 ||
                    prefix < 0 || prefix > 128) {
                count = BAD_ARGUMENT;
                break;
            }

            ifr6.ifr6_prefixlen = prefix;
            if (ioctl(inet6, SIOCSIFADDR, &ifr6)) {
                count = (errno == EINVAL) ? BAD_ARGUMENT : SYSTEM_ERROR;
                break;
            }
        } else {
            // Add an IPv4 address.
            if (inet_pton(AF_INET, address, as_in_addr(&ifr4.ifr_addr)) != 1 ||
                    prefix < 0 || prefix > 32) {
                count = BAD_ARGUMENT;
                break;
            }

            if (count) {
                sprintf(ifr4.ifr_name, "%s:%d", name, count);
            }
            if (ioctl(inet4, SIOCSIFADDR, &ifr4)) {
                count = (errno == EINVAL) ? BAD_ARGUMENT : SYSTEM_ERROR;
                break;
            }

            in_addr_t mask = prefix ? (~0 << (32 - prefix)) : 0;
            *as_in_addr(&ifr4.ifr_netmask) = htonl(mask);
            if (ioctl(inet4, SIOCSIFNETMASK, &ifr4)) {
                count = (errno == EINVAL) ? BAD_ARGUMENT : SYSTEM_ERROR;
                break;
            }
        }
        Logger::D("Vpn", "Address added on %s: %s/%d", name, address, prefix);
        ++count;
    }

    if (count == BAD_ARGUMENT) {
        Logger::E("Vpn", "Invalid address: %s/%d", address, prefix);
    } else if (count == SYSTEM_ERROR) {
        Logger::E("Vpn", "Cannot add address: %s/%d: %s", address, prefix, strerror(errno));
    } else if (*addresses) {
        Logger::E("Vpn", "Invalid address: %s", addresses);
        count = BAD_ARGUMENT;
    }

    return count;
}

static int set_routes(const char *name, const char *routes)
{
    int index = get_interface_index(name);
    if (index < 0) {
        return index;
    }

    rtentry rt4;
    memset(&rt4, 0, sizeof(rt4));
    rt4.rt_dev = (char *)name;
    rt4.rt_flags = RTF_UP;
    rt4.rt_dst.sa_family = AF_INET;
    rt4.rt_genmask.sa_family = AF_INET;

    in6_rtmsg rt6;
    memset(&rt6, 0, sizeof(rt6));
    rt6.rtmsg_ifindex = index;
    rt6.rtmsg_flags = RTF_UP;

    char address[65];
    int prefix;
    int chars;
    int count = 0;

    while (sscanf(routes, " %64[^/]/%d %n", address, &prefix, &chars) == 2) {
        routes += chars;

        if (strchr(address, ':')) {
            // Add an IPv6 route.
            if (inet_pton(AF_INET6, address, &rt6.rtmsg_dst) != 1 ||
                    prefix < 0 || prefix > 128) {
                count = BAD_ARGUMENT;
                break;
            }

            rt6.rtmsg_dst_len = prefix ? prefix : 1;
            if (ioctl(inet6, SIOCADDRT, &rt6) && errno != EEXIST) {
                count = (errno == EINVAL) ? BAD_ARGUMENT : SYSTEM_ERROR;
                break;
            }

            if (!prefix) {
                // Split the route instead of replacing the default route.
                rt6.rtmsg_dst.s6_addr[0] ^= 0x80;
                if (ioctl(inet6, SIOCADDRT, &rt6) && errno != EEXIST) {
                    count = SYSTEM_ERROR;
                    break;
                }
            }
        } else {
            // Add an IPv4 route.
            if (inet_pton(AF_INET, address, as_in_addr(&rt4.rt_dst)) != 1 ||
                    prefix < 0 || prefix > 32) {
                count = BAD_ARGUMENT;
                break;
            }

            in_addr_t mask = prefix ? (~0 << (32 - prefix)) : 0x80000000;
            *as_in_addr(&rt4.rt_genmask) = htonl(mask);
            if (ioctl(inet4, SIOCADDRT, &rt4) && errno != EEXIST) {
                count = (errno == EINVAL) ? BAD_ARGUMENT : SYSTEM_ERROR;
                break;
            }

            if (!prefix) {
                // Split the route instead of replacing the default route.
                *as_in_addr(&rt4.rt_dst) ^= htonl(0x80000000);
                if (ioctl(inet4, SIOCADDRT, &rt4) && errno != EEXIST) {
                    count = SYSTEM_ERROR;
                    break;
                }
            }
        }
        Logger::D("Vpn", "Route added on %s: %s/%d", name, address, prefix);
        ++count;
    }

    if (count == BAD_ARGUMENT) {
        Logger::E("Vpn", "Invalid route: %s/%d", address, prefix);
    } else if (count == SYSTEM_ERROR) {
        Logger::E("Vpn", "Cannot add route: %s/%d: %s",
                address, prefix, strerror(errno));
    } else if (*routes) {
        Logger::E("Vpn", "Invalid route: %s", routes);
        count = BAD_ARGUMENT;
    }

    return count;
}

static int reset_interface(const char *name)
{
    ifreq ifr4;
    strncpy(ifr4.ifr_name, name, IFNAMSIZ);
    ifr4.ifr_flags = 0;

    if (ioctl(inet4, SIOCSIFFLAGS, &ifr4) && errno != ENODEV) {
        Logger::E("Vpn", "Cannot reset %s: %s", name, strerror(errno));
        return SYSTEM_ERROR;
    }
    return 0;
}

static int check_interface(const char *name)
{
    ifreq ifr4;
    strncpy(ifr4.ifr_name, name, IFNAMSIZ);
    ifr4.ifr_flags = 0;

    if (ioctl(inet4, SIOCGIFFLAGS, &ifr4) && errno != ENODEV) {
        Logger::E("Vpn", "Cannot check %s: %s", name, strerror(errno));
    }
    return ifr4.ifr_flags;
}

static int bind_to_interface(int socket, const char *name)
{
    if (setsockopt(socket, SOL_SOCKET, SO_BINDTODEVICE, name, strlen(name))) {
        Logger::E("Vpn", "Cannot bind socket to %s: %s", name, strerror(errno));
        return SYSTEM_ERROR;
    }
    return 0;
}

ECode Vpn::NativeCreate(
    /* [in] */ Int32 mtu,
    /* [out] */ Int32* fd)
{
    Int32 tun = create_interface(mtu);
    if (tun < 0) {
        // throwException(env, tun, "Cannot create interface");
        *fd = -1;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *fd = tun;
    return NOERROR;
}

ECode Vpn::NativeGetName(
    /* [in] */ Int32 tun,
    /* [out] */ String* vpnName)
{
    char name[IFNAMSIZ];
    if (get_interface_name(name, tun) < 0) {
        // throwException(env, SYSTEM_ERROR, "Cannot get interface name");
        *vpnName = NULL;
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    *vpnName = name;
    return NOERROR;
}

Int32 Vpn::NativeSetAddresses(
    /* [in] */ const String& interfaze,
    /* [in] */ const String& addresses)
{
    Int32 count = -1;

    if (interfaze.IsNull() || addresses.IsNull()) return count;

    count = set_addresses(interfaze.string(), addresses.string());
    if (count < 0) count = -1;
    return count;
}

Int32 Vpn::NativeSetRoutes(
    /* [in] */ const String& interfaze,
    /* [in] */ const String& routes)
{
    Int32 count = -1;

    if (interfaze.IsNull() || routes.IsNull()) return count;

    count = set_routes(interfaze.string(), routes.string());
    if (count < 0) count = -1;
    return count;
}

ECode Vpn::NativeReset(
    /* [in] */ const String& interfaze)
{
    if (interfaze.IsNull()) return E_NULL_POINTER_EXCEPTION;
    if (reset_interface(interfaze.string()) < 0) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

Int32 Vpn::NativeCheck(
    /* [in] */ const String& interfaze)
{
    const char *name = interfaze.string();
    Int32 flags = check_interface(name);
    return flags;
}

ECode Vpn::NativeProtect(
    /* [in] */ Int32 socket,
    /* [in] */ const String& interfaze)
{
    const char *name = interfaze.string();
    if (!name) {
        return E_NULL_POINTER_EXCEPTION;
    }
    if (bind_to_interface(socket, name) < 0) {
        return E_ILLEGAL_STATE_EXCEPTION;
    }
    return NOERROR;
}

ECode Vpn::FindLegacyVpnGateway(
    /* [in] */ ILinkProperties* prop,
    /* [out] */ String* address)
{
    VALIDATE_NOT_NULL(address);

    AutoPtr<IObjectContainer> propRoute;
    prop->GetRoutes((IObjectContainer**)&propRoute);
    AutoPtr<IObjectEnumerator> it;
    propRoute->GetObjectEnumerator((IObjectEnumerator**)&it);
    Boolean succeeded;
    while (it->MoveNext(&succeeded), succeeded) {
        // Currently legacy VPN only works on IPv4.
        AutoPtr<IInterface> object;
        it->Current((IInterface**)&object);
        AutoPtr<IRouteInfo> route = IRouteInfo::Probe(object);
        Boolean isDefault;
        route->IsDefaultRoute(&isDefault);
        AutoPtr<IInetAddress> inet;
        route->GetGateway((IInetAddress**)&inet);
        if (isDefault && IInet4Address::Probe(inet)) {
            return inet->GetHostAddress(address);
        }
    }

    return E_ILLEGAL_STATE_EXCEPTION;
}

/**
* Start legacy VPN, controlling native daemons as needed. Creates a
* secondary thread to perform connection work, returning quickly.
*/
ECode Vpn::StartLegacyVpn(
    /* [in] */ IVpnProfile* socket,
    /* [in] */ IKeyStore* interfaze,
    /* [in] */ ILinkProperties* egress)
{
//    if (keyStore.state() != KeyStore.State.UNLOCKED) {
//        throw new IllegalStateException("KeyStore isn't unlocked");
//    }
//
//    final String iface = egress.getInterfaceName();
//    final String gateway = findLegacyVpnGateway(egress);
//
//    // Load certificates.
//    String privateKey = "";
//    String userCert = "";
//    String caCert = "";
//    String serverCert = "";
//    if (!profile.ipsecUserCert.isEmpty()) {
//        privateKey = Credentials.USER_PRIVATE_KEY + profile.ipsecUserCert;
//        byte[] value = keyStore.get(Credentials.USER_CERTIFICATE + profile.ipsecUserCert);
//        userCert = (value == null) ? null : new String(value, Charsets.UTF_8);
//    }
//    if (!profile.ipsecCaCert.isEmpty()) {
//        byte[] value = keyStore.get(Credentials.CA_CERTIFICATE + profile.ipsecCaCert);
//        caCert = (value == null) ? null : new String(value, Charsets.UTF_8);
//    }
//    if (!profile.ipsecServerCert.isEmpty()) {
//        byte[] value = keyStore.get(Credentials.USER_CERTIFICATE + profile.ipsecServerCert);
//        serverCert = (value == null) ? null : new String(value, Charsets.UTF_8);
//    }
//    if (privateKey == null || userCert == null || caCert == null || serverCert == null) {
//        throw new IllegalStateException("Cannot load credentials");
//    }
//
//    // Prepare arguments for racoon.
//    String[] racoon = null;
//    switch (profile.type) {
//        case VpnProfile.TYPE_L2TP_IPSEC_PSK:
//            racoon = new String[] {
//                iface, profile.server, "udppsk", profile.ipsecIdentifier,
//                profile.ipsecSecret, "1701",
//            };
//            break;
//        case VpnProfile.TYPE_L2TP_IPSEC_RSA:
//            racoon = new String[] {
//                iface, profile.server, "udprsa", privateKey, userCert,
//                caCert, serverCert, "1701",
//            };
//            break;
//        case VpnProfile.TYPE_IPSEC_XAUTH_PSK:
//            racoon = new String[] {
//                iface, profile.server, "xauthpsk", profile.ipsecIdentifier,
//                profile.ipsecSecret, profile.username, profile.password, "", gateway,
//            };
//            break;
//        case VpnProfile.TYPE_IPSEC_XAUTH_RSA:
//            racoon = new String[] {
//                iface, profile.server, "xauthrsa", privateKey, userCert,
//                caCert, serverCert, profile.username, profile.password, "", gateway,
//            };
//            break;
//        case VpnProfile.TYPE_IPSEC_HYBRID_RSA:
//            racoon = new String[] {
//                iface, profile.server, "hybridrsa",
//                caCert, serverCert, profile.username, profile.password, "", gateway,
//            };
//            break;
//    }
//
//    // Prepare arguments for mtpd.
//    String[] mtpd = null;
//    switch (profile.type) {
//        case VpnProfile.TYPE_PPTP:
//            mtpd = new String[] {
//                iface, "pptp", profile.server, "1723",
//                "name", profile.username, "password", profile.password,
//                "linkname", "vpn", "refuse-eap", "nodefaultroute",
//                "usepeerdns", "idle", "1800", "mtu", "1400", "mru", "1400",
//                (profile.mppe ? "+mppe" : "nomppe"),
//            };
//            break;
//        case VpnProfile.TYPE_L2TP_IPSEC_PSK:
//        case VpnProfile.TYPE_L2TP_IPSEC_RSA:
//            mtpd = new String[] {
//                iface, "l2tp", profile.server, "1701", profile.l2tpSecret,
//                "name", profile.username, "password", profile.password,
//                "linkname", "vpn", "refuse-eap", "nodefaultroute",
//                "usepeerdns", "idle", "1800", "mtu", "1400", "mru", "1400",
//            };
//            break;
//    }
//
//    VpnConfig config = new VpnConfig();
//    config.legacy = true;
//    config.user = profile.key;
//    config.interfaze = iface;
//    config.session = profile.name;
//    config.routes = profile.routes;
//    if (!profile.dnsServers.isEmpty()) {
//        config.dnsServers = Arrays.asList(profile.dnsServers.split(" +"));
//    }
//    if (!profile.searchDomains.isEmpty()) {
//        config.searchDomains = Arrays.asList(profile.searchDomains.split(" +"));
//    }
//
//    startLegacyVpn(config, racoon, mtpd);
    assert(0);
    return E_NOT_IMPLEMENTED;
}

void Vpn::StartLegacyVpn(
    /* [in] */ IVpnConfig* config,
    /* [in] */ ArrayOf<String>* racoon,
    /* [in] */ ArrayOf<String>* mtpd)
{
    Mutex::Autolock lock(mLock);
    StopLegacyVpn();

    // Prepare for the new request. This also checks the caller.
    Boolean result;
    Prepare(String(NULL), IVpnConfig::LEGACY_VPN, &result);
    UpdateState(NetworkInfoDetailedState_CONNECTING, String("startLegacyVpn"));

    // Start a new LegacyVpnRunner and we are done!
    mLegacyVpnRunner = new LegacyVpnRunner(config, racoon, mtpd, this);
    mLegacyVpnRunner->Start();
}

ECode Vpn::StopLegacyVpn()
{
    Mutex::Autolock lock(mLock);
    if (mLegacyVpnRunner != NULL) {
        mLegacyVpnRunner->Exit();
        mLegacyVpnRunner = NULL;

        {
            Mutex::Autolock lock(LegacyVpnRunner::sTAGLock);
            // wait for old thread to completely finish before spinning up
            // new instance, otherwise state updates can be out of order.
        }
    }
    return NOERROR;
}

/**
* Return the information of the current ongoing legacy VPN.
*/
ECode Vpn::GetLegacyVpnInfo(
    /* [out] */ ILegacyVpnInfo** info)
{
    VALIDATE_NOT_NULL(info);
    *info = NULL;

    Mutex::Autolock lock(mLock);
    // Check if the caller is authorized.
    FAIL_RETURN(EnforceControlPermission());
    if (mLegacyVpnRunner == NULL) {
        return NOERROR;
    }

    CLegacyVpnInfo::New(info);
    String user;
    mLegacyVpnRunner->mConfig->GetUser(&user);
    (*info)->SetKey(user);
    // TODO:
    // ILegacyVpnInfo::StateFromNetworkInfo(mNetworkInfo, &(*info)->state);

    Boolean result;
    if (mNetworkInfo->IsConnected(&result), result) {
       (*info)->SetIntent(mStatusIntent);
    }
    return NOERROR;
}

ECode Vpn::GetLegacyVpnConfig(
    /* [out] */ IVpnConfig** mConfig)
{
    VALIDATE_NOT_NULL(mConfig);
    *mConfig = NULL;

    if (mLegacyVpnRunner != NULL) {
        *mConfig = mLegacyVpnRunner->mConfig;
        INTERFACE_ADDREF(*mConfig);
    }

    return NOERROR;
}

} // namespace Connectivity
} // namespace Server
} // namespace Droid
} // namespace Elastos
