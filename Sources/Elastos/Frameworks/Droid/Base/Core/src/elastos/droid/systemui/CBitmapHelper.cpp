
#include "elastos/droid/systemui/CBitmapHelper.h"
#include "elastos/droid/graphics/CBitmap.h"
#include "elastos/droid/graphics/CCanvas.h"
#include "elastos/droid/graphics/CPaint.h"
#include "elastos/droid/graphics/CBitmapShader.h"
#include "elastos/droid/graphics/CRectF.h"
#include "elastos/droid/graphics/CMatrix.h"

using Elastos::Droid::Graphics::BitmapConfig_ARGB_8888;
using Elastos::Droid::Graphics::MatrixScaleToFit_CENTER;
using Elastos::Droid::Graphics::ShaderTileMode_CLAMP;
using Elastos::Droid::Graphics::CBitmap;
using Elastos::Droid::Graphics::ICanvas;
using Elastos::Droid::Graphics::CCanvas;
using Elastos::Droid::Graphics::IPaint;
using Elastos::Droid::Graphics::CPaint;
using Elastos::Droid::Graphics::IBitmapShader;
using Elastos::Droid::Graphics::CBitmapShader;
using Elastos::Droid::Graphics::IRectF;
using Elastos::Droid::Graphics::CRectF;
using Elastos::Droid::Graphics::IMatrix;
using Elastos::Droid::Graphics::CMatrix;
using Elastos::Droid::Graphics::IShader;

namespace Elastos {
namespace Droid {
namespace SystemUI {

//===============================================================
// CBitmapHelper::
//===============================================================
CAR_SINGLETON_IMPL(CBitmapHelper)

CAR_INTERFACE_IMPL(CBitmapHelper, Singleton, IBitmapHelper)

ECode CBitmapHelper::CreateCircularClip(
    /* [in] */ IBitmap* input,
    /* [in] */ Int32 width,
    /* [in] */ Int32 height,
    /* [out] */ IBitmap** result)
{
    VALIDATE_NOT_NULL(result)

    if (input == NULL) {
        *result = NULL;
        return NOERROR;
    }

    Int32 inWidth = 0;
    input->GetWidth(&inWidth);
    Int32 inHeight = 0;
    input->GetHeight(&inHeight);
    AutoPtr<IBitmap> output;
    CBitmap::CreateBitmap(width, height, BitmapConfig_ARGB_8888, (IBitmap**)&output);
    AutoPtr<ICanvas> canvas;
    CCanvas::New(output, (ICanvas**)&canvas);
    AutoPtr<IPaint> paint;
    CPaint::New((IPaint**)&paint);
    AutoPtr<IBitmapShader> p;
    CBitmapShader::New(input, ShaderTileMode_CLAMP, ShaderTileMode_CLAMP, (IBitmapShader**)&p);
    paint->SetShader(IShader::Probe(p));
    paint->SetAntiAlias(TRUE);
    AutoPtr<IRectF> srcRect;
    CRectF::New(0, 0, inWidth, inHeight, (IRectF**)&srcRect);
    AutoPtr<IRectF> dstRect;
    CRectF::New(0, 0, width, height, (IRectF**)&dstRect);
    AutoPtr<IMatrix> m;
    CMatrix::New((IMatrix**)&m);
    Boolean bRes = FALSE;
    m->SetRectToRect(srcRect, dstRect, MatrixScaleToFit_CENTER, &bRes);
    canvas->SetMatrix(m);
    canvas->DrawCircle(inWidth / 2, inHeight / 2, inWidth / 2, paint);
    *result = output;
    REFCOUNT_ADD(*result)
    return NOERROR;
}

} // namespace SystemUI
} // namepsace Droid
} // namespace Elastos