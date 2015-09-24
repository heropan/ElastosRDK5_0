
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace AndroidWebview {

//===============================================================
//          AwPrintDocumentAdapter::InnerValueCallback
//===============================================================

AwPrintDocumentAdapter::InnerValueCallback::InnerValueCallback(
    /* [in] */ AwPrintDocumentAdapter* owner,
    /* [in] */ WriteResultCallback* callback)
    : mOwner(owner)
    , mCallback(callback)
{
}

ECode AwPrintDocumentAdapter::InnerValueCallback::OnReceiveValue(
    /* [in] */ Boolean value)
{
    if (value) {
        AutoPtr< ArrayOf<IPageRange> > pageRange = ArrayOf<IPageRange>::Alloc(1);
        (*pageRange)[0] = IPageRange::ALL_PAGES;
        callback->OnWriteFinished(pageRange);
    }
    else {
        // TODO(sgurun) provide a localized error message
        callback->OnWriteFailed(NULL);
    }

    return NOERROR;
}

//===============================================================
//                   AwPrintDocumentAdapter
//===============================================================

/**
 * Constructor.
 * TODO(sgurun) remove in favor of constructor below once the AOSP changes are in.
 *
 * @param pdfExporter The PDF exporter to export the webview contents to a PDF file.
 */
AwPrintDocumentAdapter::AwPrintDocumentAdapter(
    /* [in] */ AwPdfExporter* pdfExporter)
    : mPdfExporter(pdfExporter)
    , mDocumentName("default")
{
}

/**
 * Constructor.
 *
 * @param pdfExporter The PDF exporter to export the webview contents to a PDF file.
 * @param documentName  The name of the pdf document.
 */
AwPrintDocumentAdapter::AwPrintDocumentAdapter(
    /* [in] */ AwPdfExporter* pdfExporter,
    /* [in] */ const String& documentName)
    : mPdfExporter(pdfExporter)
    , mDocumentName(documentName)
{
}


//@Override
void AwPrintDocumentAdapter::OnLayout(
    /* [in] */ IPrintAttributes* oldAttributes,
    /* [in] */ IPrintAttributes* newAttributes,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [in] */ LayoutResultCallback* callback,
    /* [in] */ IBundle* metadata)
{
    mAttributes = newAttributes;
    PrintDocumentInfo documentInfo = new PrintDocumentInfo
            .Builder(mDocumentName)
            .build();
    // TODO(sgurun) once componentization is done, do layout changes and
    // generate PDF here, set the page range information to documentinfo
    // and call onLayoutFinished with true/false depending on whether
    // layout actually changed.
    callback->OnLayoutFinished(documentInfo, TRUE);
}

//@Override
void AwPrintDocumentAdapter::OnWrite(
    /* [in] */ ArrayOf<IPageRange>* pages,
    /* [in] */ IParcelFileDescriptor* destination,
    /* [in] */ ICancellationSignal* cancellationSignal,
    /* [in] */ const WriteResultCallback* callback)
{
    AutoPtr<IValueCallback> valueCallback = new InnerValueCallback(this, callback);
    mPdfExporter->ExportToPdf(destination, mAttributes, valueCallback, cancellationSignal);
}
