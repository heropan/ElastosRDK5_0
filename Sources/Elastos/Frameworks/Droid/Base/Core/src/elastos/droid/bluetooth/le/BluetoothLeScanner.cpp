
#include "elastos/droid/bluetooth/le/BluetoothLeScanner.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//      BluetoothLeScanner::BleScanCallbackWrapper::ScanResultRunnable
//=====================================================================
BluetoothLeScanner::BleScanCallbackWrapper::ScanResultRunnable::ScanResultRunnable(
    /* [in] */ BleScanCallbackWrapper* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::ScanResultRunnable::Run()
{
    // ==================before translated======================
    // mScanCallback.onScanResult(ScanSettings.CALLBACK_TYPE_ALL_MATCHES, scanResult);
    assert(0);
    return NOERROR;
}

//=====================================================================
//      BluetoothLeScanner::BleScanCallbackWrapper::BatchScanResultsRunnable
//=====================================================================
BluetoothLeScanner::BleScanCallbackWrapper::BatchScanResultsRunnable::BatchScanResultsRunnable(
    /* [in] */ BleScanCallbackWrapper* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::BatchScanResultsRunnable::Run()
{
    // ==================before translated======================
    // mScanCallback.onBatchScanResults(results);
    assert(0);
    return NOERROR;
}

//=====================================================================
//      BluetoothLeScanner::BleScanCallbackWrapper::FoundOrLostRunnable
//=====================================================================
BluetoothLeScanner::BleScanCallbackWrapper::FoundOrLostRunnable::FoundOrLostRunnable(
    /* [in] */ BleScanCallbackWrapper* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::FoundOrLostRunnable::Run()
{
    // ==================before translated======================
    // if (onFound) {
    //     mScanCallback.onScanResult(ScanSettings.CALLBACK_TYPE_FIRST_MATCH,
    //             scanResult);
    // } else {
    //     mScanCallback.onScanResult(ScanSettings.CALLBACK_TYPE_MATCH_LOST,
    //             scanResult);
    // }
    assert(0);
    return NOERROR;
}

//=====================================================================
//              BluetoothLeScanner::BleScanCallbackWrapper
//=====================================================================
const Int32 BluetoothLeScanner::BleScanCallbackWrapper::REGISTRATION_CALLBACK_TIMEOUT_MILLIS;

BluetoothLeScanner::BleScanCallbackWrapper::BleScanCallbackWrapper(
    /* [in] */ IIBluetoothGatt* bluetoothGatt,
    /* [in] */ IList* filters,
    /* [in] */ IScanSettings* settings,
    /* [in] */ IScanCallback* scanCallback,
    /* [in] */ IList* resultStorages,
    /* [in] */ BluetoothLeScanner* owner)
{
    // ==================before translated======================
    // mBluetoothGatt = bluetoothGatt;
    // mFilters = filters;
    // mSettings = settings;
    // mScanCallback = scanCallback;
    // mClientIf = 0;
    // mResultStorages = resultStorages;
    mOwner = owner;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::StartRegisteration()
{
    // ==================before translated======================
    // synchronized (this) {
    //     // Scan stopped.
    //     if (mClientIf == -1) return;
    //     try {
    //         UUID uuid = UUID.randomUUID();
    //         mBluetoothGatt.registerClient(new ParcelUuid(uuid), this);
    //         wait(REGISTRATION_CALLBACK_TIMEOUT_MILLIS);
    //     } catch (InterruptedException | RemoteException e) {
    //         Log.e(TAG, "application registeration exception", e);
    //         postCallbackError(mScanCallback, ScanCallback.SCAN_FAILED_INTERNAL_ERROR);
    //     }
    //     if (mClientIf > 0) {
    //         mLeScanClients.put(mScanCallback, this);
    //     } else {
    //         postCallbackError(mScanCallback,
    //                 ScanCallback.SCAN_FAILED_APPLICATION_REGISTRATION_FAILED);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::StopLeScan()
{
    // ==================before translated======================
    // synchronized (this) {
    //     if (mClientIf <= 0) {
    //         Log.e(TAG, "Error state, mLeHandle: " + mClientIf);
    //         return;
    //     }
    //     try {
    //         mBluetoothGatt.stopScan(mClientIf, false);
    //         mBluetoothGatt.unregisterClient(mClientIf);
    //     } catch (RemoteException e) {
    //         Log.e(TAG, "Failed to stop scan and unregister", e);
    //     }
    //     mClientIf = -1;
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::FlushPendingBatchResults()
{
    // ==================before translated======================
    // synchronized (this) {
    //     if (mClientIf <= 0) {
    //         Log.e(TAG, "Error state, mLeHandle: " + mClientIf);
    //         return;
    //     }
    //     try {
    //         mBluetoothGatt.flushPendingBatchResults(mClientIf, false);
    //     } catch (RemoteException e) {
    //         Log.e(TAG, "Failed to get pending scan results", e);
    //     }
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::OnClientRegistered(
    /* [in] */ Int32 status,
    /* [in] */ Int32 clientIf)
{
    // ==================before translated======================
    // Log.d(TAG, "onClientRegistered() - status=" + status +
    //         " clientIf=" + clientIf);
    // synchronized (this) {
    //     if (mClientIf == -1) {
    //         if (DBG) Log.d(TAG, "onClientRegistered LE scan canceled");
    //     }
    //
    //     if (status == BluetoothGatt.GATT_SUCCESS) {
    //         mClientIf = clientIf;
    //         try {
    //             mBluetoothGatt.startScan(mClientIf, false, mSettings, mFilters,
    //                     mResultStorages);
    //         } catch (RemoteException e) {
    //             Log.e(TAG, "fail to start le scan: " + e);
    //             mClientIf = -1;
    //         }
    //     } else {
    //         // registration failed
    //         mClientIf = -1;
    //     }
    //     notifyAll();
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::OnScanResult(
    /* [in] */ IScanResult* scanResult)
{
    VALIDATE_NOT_NULL(scanResult);
    // ==================before translated======================
    // if (DBG) Log.d(TAG, "onScanResult() - " + scanResult.toString());
    //
    // // Check null in case the scan has been stopped
    // synchronized (this) {
    //     if (mClientIf <= 0) return;
    // }
    // Handler handler = new Handler(Looper.getMainLooper());
    // handler.post(new Runnable() {
    //     @Override
    //     public void run() {
    //         mScanCallback.onScanResult(ScanSettings.CALLBACK_TYPE_ALL_MATCHES, scanResult);
    //     }
    // });
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::OnBatchScanResults(
    /* [in] */ IList* results)
{
    VALIDATE_NOT_NULL(results);
    // ==================before translated======================
    // Handler handler = new Handler(Looper.getMainLooper());
    // handler.post(new Runnable() {
    //     @Override
    //     public void run() {
    //         mScanCallback.onBatchScanResults(results);
    //     }
    // });
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::BleScanCallbackWrapper::OnFoundOrLost(
    /* [in] */ Boolean onFound,
    /* [in] */ IScanResult* scanResult)
{
    VALIDATE_NOT_NULL(scanResult);
    // ==================before translated======================
    // if (DBG) {
    //     Log.d(TAG, "onFoundOrLost() - onFound = " + onFound +
    //             " " + scanResult.toString());
    // }
    //
    // // Check null in case the scan has been stopped
    // synchronized (this) {
    //     if (mClientIf <= 0)
    //         return;
    // }
    // Handler handler = new Handler(Looper.getMainLooper());
    // handler.post(new Runnable() {
    //         @Override
    //     public void run() {
    //         if (onFound) {
    //             mScanCallback.onScanResult(ScanSettings.CALLBACK_TYPE_FIRST_MATCH,
    //                     scanResult);
    //         } else {
    //             mScanCallback.onScanResult(ScanSettings.CALLBACK_TYPE_MATCH_LOST,
    //                     scanResult);
    //         }
    //     }
    // });
    assert(0);
    return NOERROR;
}

//=====================================================================
//                  BluetoothLeScanner::CallbackErrorRunnable
//=====================================================================
BluetoothLeScanner::CallbackErrorRunnable::CallbackErrorRunnable(
    /* [in] */ BluetoothLeScanner* owner)
    : mOwner(owner)
{
    // ==================before translated======================
    // mOwner = owner;
}

ECode BluetoothLeScanner::CallbackErrorRunnable::Run()
{
    // ==================before translated======================
    // callback.onScanFailed(errorCode);
    assert(0);
    return NOERROR;
}

//=====================================================================
//                          BluetoothLeScanner
//=====================================================================
const String BluetoothLeScanner::TAG("BluetoothLeScanner");
const Boolean BluetoothLeScanner::DBG = TRUE;

BluetoothLeScanner::BluetoothLeScanner(
    /* [in] */ IIBluetoothManager* bluetoothManager)
{
    // ==================before translated======================
    // mBluetoothManager = bluetoothManager;
    // mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
    // mHandler = new Handler(Looper.getMainLooper());
    // mLeScanClients = new HashMap<ScanCallback, BleScanCallbackWrapper>();
}

ECode BluetoothLeScanner::StartScan(
    /* [in] */ IScanCallback* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // if (callback == null) {
    //     throw new IllegalArgumentException("callback is null");
    // }
    // startScan(null, new ScanSettings.Builder().build(), callback);
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::StartScan(
    /* [in] */ IList* filters,
    /* [in] */ IScanSettings* settings,
    /* [in] */ IScanCallback* callback)
{
    VALIDATE_NOT_NULL(filters);
    VALIDATE_NOT_NULL(settings);
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // startScan(filters, settings, callback, null);
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::StopScan(
    /* [in] */ IScanCallback* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // BluetoothLeUtils.checkAdapterStateOn(mBluetoothAdapter);
    // synchronized (mLeScanClients) {
    //     BleScanCallbackWrapper wrapper = mLeScanClients.remove(callback);
    //     if (wrapper == null) {
    //         if (DBG) Log.d(TAG, "could not find callback wrapper");
    //         return;
    //     }
    //     wrapper.stopLeScan();
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::FlushPendingScanResults(
    /* [in] */ IScanCallback* callback)
{
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // BluetoothLeUtils.checkAdapterStateOn(mBluetoothAdapter);
    // if (callback == null) {
    //     throw new IllegalArgumentException("callback cannot be null!");
    // }
    // synchronized (mLeScanClients) {
    //     BleScanCallbackWrapper wrapper = mLeScanClients.get(callback);
    //     if (wrapper == null) {
    //         return;
    //     }
    //     wrapper.flushPendingBatchResults();
    // }
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::StartTruncatedScan(
    /* [in] */ IList* truncatedFilters,
    /* [in] */ IScanSettings* settings,
    /* [in] */ IScanCallback* callback)
{
    VALIDATE_NOT_NULL(truncatedFilters);
    VALIDATE_NOT_NULL(settings);
    VALIDATE_NOT_NULL(callback);
    // ==================before translated======================
    // int filterSize = truncatedFilters.size();
    // List<ScanFilter> scanFilters = new ArrayList<ScanFilter>(filterSize);
    // List<List<ResultStorageDescriptor>> scanStorages =
    //         new ArrayList<List<ResultStorageDescriptor>>(filterSize);
    // for (TruncatedFilter filter : truncatedFilters) {
    //     scanFilters.add(filter.getFilter());
    //     scanStorages.add(filter.getStorageDescriptors());
    // }
    // startScan(scanFilters, settings, callback, scanStorages);
    assert(0);
    return NOERROR;
}

ECode BluetoothLeScanner::Cleanup()
{
    // ==================before translated======================
    // mLeScanClients.clear();
    assert(0);
    return NOERROR;
}

void BluetoothLeScanner::StartScan(
    /* [in] */ IList* filters,
    /* [in] */ IScanSettings* settings,
    /* [in] */ IScanCallback* callback,
    /* [in] */ IList* resultStorages)
{
    // ==================before translated======================
    // BluetoothLeUtils.checkAdapterStateOn(mBluetoothAdapter);
    // if (settings == null || callback == null) {
    //     throw new IllegalArgumentException("settings or callback is null");
    // }
    // synchronized (mLeScanClients) {
    //     if (mLeScanClients.containsKey(callback)) {
    //         postCallbackError(callback, ScanCallback.SCAN_FAILED_ALREADY_STARTED);
    //         return;
    //     }
    //     IBluetoothGatt gatt;
    //     try {
    //         gatt = mBluetoothManager.getBluetoothGatt();
    //     } catch (RemoteException e) {
    //         gatt = null;
    //     }
    //     if (gatt == null) {
    //         postCallbackError(callback, ScanCallback.SCAN_FAILED_INTERNAL_ERROR);
    //         return;
    //     }
    //     if (!isSettingsConfigAllowedForScan(settings)) {
    //         postCallbackError(callback,
    //                 ScanCallback.SCAN_FAILED_FEATURE_UNSUPPORTED);
    //         return;
    //     }
    //     BleScanCallbackWrapper wrapper = new BleScanCallbackWrapper(gatt, filters,
    //             settings, callback, resultStorages);
    //     wrapper.startRegisteration();
    // }
    assert(0);
}

void BluetoothLeScanner::PostCallbackError(
    /* [in] */ IScanCallback* callback,
    /* [in] */ Int32 errorCode)
{
    // ==================before translated======================
    // mHandler.post(new Runnable() {
    //     @Override
    //     public void run() {
    //         callback.onScanFailed(errorCode);
    //     }
    // });
    assert(0);
}

Boolean BluetoothLeScanner::IsSettingsConfigAllowedForScan(
    /* [in] */ IScanSettings* settings)
{
    // ==================before translated======================
    // if (mBluetoothAdapter.isOffloadedFilteringSupported()) {
    //     return true;
    // }
    // final int callbackType = settings.getCallbackType();
    // // Only support regular scan if no offloaded filter support.
    // if (callbackType == ScanSettings.CALLBACK_TYPE_ALL_MATCHES
    //         && settings.getReportDelayMillis() == 0) {
    //     return true;
    // }
    // return false;
    assert(0);
    return FALSE;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos


