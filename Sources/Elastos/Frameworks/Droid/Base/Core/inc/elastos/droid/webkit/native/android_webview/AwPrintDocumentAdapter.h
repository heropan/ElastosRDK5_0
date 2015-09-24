
#ifndef __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWPRINTDOCUMENTADAPTER_H__
#define __ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWPRINTDOCUMENTADAPTER_H__

// import android.os.Bundle;
// import android.os.CancellationSignal;
// import android.os.ParcelFileDescriptor;
// import android.print.PageRange;
// import android.print.PrintAttributes;
// import android.print.PrintDocumentAdapter;
// import android.print.PrintDocumentInfo;
// import android.webkit.ValueCallback;

namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

/**
 * Adapter for printing Webview. This class implements the abstract
 * system class PrintDocumentAdapter and hides all printing details from
 * the developer.
 */
class AwPrintDocumentAdapter : public PrintDocumentAdapter
{
private:
    class InnerValueCallback
        : public Object
        , public IValueCallback
    {
    public:
        InnerValueCallback(
            /* [in] */ AwPrintDocumentAdapter* owner,
            /* [in] */ WriteResultCallback* callback);

        CARAPI OnReceiveValue(
            /* [in] */ Boolean value);

    private:
        AwPrintDocumentAdapter* mOwner;
        WriteResultCallback* mCallback;
    };

public:
    /**
     * Constructor.
     * TODO(sgurun) remove in favor of constructor below once the AOSP changes are in.
     *
     * @param pdfExporter The PDF exporter to export the webview contents to a PDF file.
     */
    AwPrintDocumentAdapter(
        /* [in] */ AwPdfExporter* pdfExporter);

    /**
     * Constructor.
     *
     * @param pdfExporter The PDF exporter to export the webview contents to a PDF file.
     * @param documentName  The name of the pdf document.
     */
    AwPrintDocumentAdapter(
        /* [in] */ AwPdfExporter* pdfExporter,
        /* [in] */ const String& documentName);

    //@Override
    CARAPI_(void) OnLayout(
        /* [in] */ IPrintAttributes* oldAttributes,
        /* [in] */ IPrintAttributes* newAttributes,
        /* [in] */ ICancellationSignal* cancellationSignal,
        /* [in] */ LayoutResultCallback* callback,
        /* [in] */ IBundle* metadata);

    //@Override
    CARAPI_(void) OnWrite(
        /* [in] */ ArrayOf<IPageRange>* pages,
        /* [in] */ IParcelFileDescriptor* destination,
        /* [in] */ ICancellationSignal* cancellationSignal,
        /* [in] */ const WriteResultCallback* callback);

private:
    AutoPtr<AwPdfExporter> mPdfExporter;
    AutoPtr<IPrintAttributes> mAttributes;
    String mDocumentName;
};

} // namespace AndroidWebview
} // namespace Webkit
} // namespace Droid
} // namespace Elastos

#endif//__ELASTOS_DROID_WEBKIT_ANDROIDWEBVIEW_AWPRINTDOCUMENTADAPTER_H__
