
#include "elastos/droid/graphics/Shader.h"
#include "elastos/droid/graphics/CMatrix.h"
#include <skia/core/SkShader.h>

namespace Elastos {
namespace Droid {
namespace Graphics {

// {AE795BA0-60FF-4BED-ADF2-9BDE205AB72F}
extern const InterfaceID EIID_Shader =
    { 0xae795ba0, 0x60ff, 0x4bed, { 0xad, 0xf2, 0x9b, 0xde, 0x20, 0x5a, 0xb7, 0x2f } };

CAR_INTERFACE_IMPL(Shader, Object, IShader);
Shader::~Shader()
{
    NativeDestructor(mNativeInstance);
}

ECode Shader::GetLocalMatrix(
    /* [in, out] */ IMatrix* localM,
    /* [out] */ Boolean* has)
{
    VALIDATE_NOT_NULL(has);
    if (mLocalMatrix != NULL) {
        localM->Set(mLocalMatrix);
        Boolean result;
        mLocalMatrix->IsIdentity(&result);
        *has = !result;
        return NOERROR;
    }
    *has = FALSE;
    return NOERROR;
}

ECode Shader::SetLocalMatrix(
    /* [in] */ IMatrix* localM)
{
    mLocalMatrix = localM;
    NativeSetLocalMatrix(mNativeInstance,
            localM == NULL ? 0 : ((Matrix*)(IMatrix*)localM->Probe(EIID_Matrix))->mNativeInstance);
    return NOERROR;
}

void Shader::Init(
    /* [in] */ Int64 ni)
{
    mNativeInstance = ni;
}

AutoPtr<IShader> Shader::Copy()
{
    AutoPtr<IShader> copy = new Shader();
    CopyLocalMatrix(copy);
    return copy;
}

void Shader::CopyLocalMatrix(
    /* [in] */ IShader* dest)
{
    if (mLocalMatrix != NULL) {
        AutoPtr<IMatrix> lm = new Matrix();
        Boolean has = FALSE;
        GetLocalMatrix(lm, &has);
        dest->SetLocalMatrix(lm);
    } else {
        dest->SetLocalMatrix(NULL);
    }
}

Int64 Shader::GetNativeInstance()
{
    return mNativeInstance;
}

void Shader::NativeDestructor(
    /* [in] */ Int64 shaderHandle)
{
    SkShader* shader = reinterpret_cast<SkShader*>(shaderHandle);
    SkSafeUnref(shader);
}

void Shader::NativeSetLocalMatrix(
    /* [in] */ Int64 shaderHandle,
    /* [in] */ Int64 matrixHandle)
{
    SkShader* shader       = reinterpret_cast<SkShader*>(shaderHandle);
    const SkMatrix* matrix = reinterpret_cast<SkMatrix*>(matrixHandle);
    if (shader) {
        if (matrix) {
            shader->setLocalMatrix(*matrix);
        } else {
            shader->resetLocalMatrix();
        }
        shader->setGenerationID(shader->getGenerationID() + 1);
    }
}

} // namespace Graphics
} // namepsace Droid
} // namespace Elastos
