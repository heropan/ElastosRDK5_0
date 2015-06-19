
#ifndef __AWPDFEXPORTER_H__
#define __AWPDFEXPORTER_H__

// import android.os.CancellationSignal;
// import android.os.ParcelFileDescriptor;
// import android.print.PrintAttributes;
// import android.util.Log;
// import android.view.ViewGroup;
// import android.webkit.ValueCallback;

// import org.chromium.base.CalledByNative;
// import org.chromium.base.JNINamespace;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Export the android webview as a PDF.
 * @TODO(sgurun) explain the ownership of this class and its native counterpart
 */
//@JNINamespace("android_webview")
class AwPdfExporter
{
public:
    AwPdfExporter(
        /* [in] */ IViewGroup* containerView);

    virtual CARAPI_(void) SetContainerView(
        /* [in] */ IViewGroup* containerView);

    virtual CARAPI_(void) ExportToPdf(
        /* [in] */ const IParcelFileDescriptor* fd,
        /* [in] */ IPrintAttributes* attributes,
        /* [in] */ IValueCallback* resultCallback,
        /* [in] */ ICancellationSignal* cancellationSignal);

private:
    //@CalledByNative
    CARAPI_(void) SetNativeAwPdfExporter(
        /* [in] */ Int64 nativePdfExporter);

    static CARAPI_(Int32) GetPrintDpi(
        /* [in] */ IPrintAttributes* attributes);

    //@CalledByNative
    CARAPI_(void) DidExportPdf(
        /* [in] */ Boolean success);

    //@CalledByNative
    CARAPI_(Int32) GetPageWidth();

    //@CalledByNative
    CARAPI_(Int32) GetPageHeight();

    //@CalledByNative
    CARAPI_(Int32) GetDpi();

    //@CalledByNative
    CARAPI_(Int32) GetLeftMargin();

    //@CalledByNative
    CARAPI_(Int32) GetRightMargin();

    //@CalledByNative
    CARAPI_(Int32) GetTopMargin();

    //@CalledByNative
    CARAPI_(Int32) GetBottomMargin();

    CARAPI_(void) NativeExportToPdf(
        /* [in] */ Int64 nativeAwPdfExporter,
        /* [in] */ Int32 fd,
        /* [in] */ ICancellationSignal* cancellationSignal);

private:
    static const String TAG;
    Int64 mNativeAwPdfExporter;
    // TODO(sgurun) result callback should return an int/object indicating errors.
    // potential errors: invalid print parameters, already pending, IO error
    AutoPtr<IValueCallback> mResultCallback;
    AutoPtr<IPrintAttributes> mAttributes;
    AutoPtr<IParcelFileDescriptor> mFd;
    // Maintain a reference to the top level object (i.e. WebView) since in a common
    // use case (offscreen webview) application may expect the framework's print manager
    // to own the Webview (via PrintDocumentAdapter).
    // NOTE: it looks unused, but please do not remove this reference.
    AutoPtr<IViewGroup> mContainerView;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__AWPDFEXPORTER_H__
