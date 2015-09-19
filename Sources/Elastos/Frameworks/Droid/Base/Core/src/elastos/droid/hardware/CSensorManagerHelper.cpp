
#include "hardware/CSensorManagerHelper.h"
#include "ext/frameworkext.h"
#include <elastos/core/Math.h>

namespace Elastos {
namespace Droid {
namespace Hardware {

AutoPtr<ArrayOf<Float> > CSensorManagerHelper::mTempMatrix = ArrayOf<Float>::Alloc(16);
Mutex CSensorManagerHelper::mTempMatrixLock;

ECode CSensorManagerHelper::GetRotationMatrix(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Float>* I,
    /* [in] */ ArrayOf<Float>* gravity,
    /* [in] */ ArrayOf<Float>* geomagnetic,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    // TODO: move this to native code for efficiency
    Float Ax = (*gravity)[0];
    Float Ay = (*gravity)[1];
    Float Az = (*gravity)[2];
    const Float Ex = (*geomagnetic)[0];
    const Float Ey = (*geomagnetic)[1];
    const Float Ez = (*geomagnetic)[2];
    Float Hx = Ey*Az - Ez*Ay;
    Float Hy = Ez*Ax - Ex*Az;
    Float Hz = Ex*Ay - Ey*Ax;
    const Float normH = (Float)Elastos::Core::Math::Sqrt(Hx*Hx + Hy*Hy + Hz*Hz);
    if (normH < 0.1f) {
        // device is close to free fall (or in space?), or close to
        // magnetic north pole. Typical values are  > 100.
        *state = FALSE;
        return NOERROR;
    }
    const Float invH = 1.0f / normH;
    Hx *= invH;
    Hy *= invH;
    Hz *= invH;
    const Float invA = 1.0f / (Float)Elastos::Core::Math::Sqrt(Ax*Ax + Ay*Ay + Az*Az);
    Ax *= invA;
    Ay *= invA;
    Az *= invA;
    const Float Mx = Ay*Hz - Az*Hy;
    const Float My = Az*Hx - Ax*Hz;
    const Float Mz = Ax*Hy - Ay*Hx;
    if (R != NULL) {
        if (R->GetLength() == 9) {
            (*R)[0] = Hx;     (*R)[1] = Hy;     (*R)[2] = Hz;
            (*R)[3] = Mx;     (*R)[4] = My;     (*R)[5] = Mz;
            (*R)[6] = Ax;     (*R)[7] = Ay;     (*R)[8] = Az;
        } else if (R->GetLength() == 16) {
            (*R)[0]  = Hx;    (*R)[1]  = Hy;    (*R)[2]  = Hz;   (*R)[3]  = 0;
            (*R)[4]  = Mx;    (*R)[5]  = My;    (*R)[6]  = Mz;   (*R)[7]  = 0;
            (*R)[8]  = Ax;    (*R)[9]  = Ay;    (*R)[10] = Az;   (*R)[11] = 0;
            (*R)[12] = 0;     (*R)[13] = 0;     (*R)[14] = 0;    (*R)[15] = 1;
        }
    }
    if (I != NULL) {
        // compute the inclination matrix by projecting the geomagnetic
        // vector onto the Z (gravity) and X (horizontal component
        // of geomagnetic vector) axes.
        const Float invE = 1.0f / (Float)Elastos::Core::Math::Sqrt(Ex*Ex + Ey*Ey + Ez*Ez);
        const Float c = (Ex*Mx + Ey*My + Ez*Mz) * invE;
        const Float s = (Ex*Ax + Ey*Ay + Ez*Az) * invE;
        if (I->GetLength() == 9) {
            (*I)[0] = 1;     (*I)[1] = 0;     (*I)[2] = 0;
            (*I)[3] = 0;     (*I)[4] = c;     (*I)[5] = s;
            (*I)[6] = 0;     (*I)[7] =-s;     (*I)[8] = c;
        } else if (I->GetLength() == 16) {
            (*I)[0] = 1;     (*I)[1] = 0;     (*I)[2] = 0;
            (*I)[4] = 0;     (*I)[5] = c;     (*I)[6] = s;
            (*I)[8] = 0;     (*I)[9] =-s;     (*I)[10]= c;
            (*I)[3] = (*I)[7] = (*I)[11] = (*I)[12] = (*I)[13] = (*I)[14] = 0;
            (*I)[15] = 1;
        }
    }

    *state = TRUE;
    return NOERROR;
}

ECode CSensorManagerHelper::GetInclination(
    /* [in] */ ArrayOf<Float>* I,
    /* [out] */ Float* inclination)
{
    assert(inclination != NULL);
    if (I->GetLength() == 9) {
        *inclination = (Float)Elastos::Core::Math::Atan2((*I)[5], (*I)[4]);
    } else {
        *inclination = (Float)Elastos::Core::Math::Atan2((*I)[6], (*I)[5]);
    }

    return NOERROR;
}

ECode CSensorManagerHelper::RemapCoordinateSystem(
    /* [in] */ ArrayOf<Float>* inR,
    /* [in] */ Int32 X,
    /* [in] */ Int32 Y,
    /* [in] */ ArrayOf<Float>* outR,
    /* [out] */ Boolean* state)
{
    assert(state != NULL);
    if (inR == outR) {
        {
            AutoLock lock(mTempMatrixLock);
            // we don't expect to have a lot of contention
            if (RemapCoordinateSystemImpl(inR, X, Y, mTempMatrix)) {
                const Int32 size = outR->GetLength();
                for (Int32 i=0 ; i<size ; i++)
                    (*outR)[i] = (*mTempMatrix)[i];

                *state = TRUE;
                return NOERROR;
            }
        }
    }

    *state = RemapCoordinateSystemImpl(inR, X, Y, outR);
    return NOERROR;
}

Boolean CSensorManagerHelper::RemapCoordinateSystemImpl(
    /* [in] */ ArrayOf<Float>* inR,
    /* [in] */ Int32 X,
    /* [in] */ Int32 Y,
    /* [in] */ ArrayOf<Float>* outR)
{
    /*
     * X and Y define a rotation matrix 'r':
     *
     *  (X==1)?((X&0x80)?-1:1):0    (X==2)?((X&0x80)?-1:1):0    (X==3)?((X&0x80)?-1:1):0
     *  (Y==1)?((Y&0x80)?-1:1):0    (Y==2)?((Y&0x80)?-1:1):0    (Y==3)?((X&0x80)?-1:1):0
     *                              r[0] ^ r[1]
     *
     * where the 3rd line is the vector product of the first 2 lines
     *
     */

    const Int32 length = outR->GetLength();
    if (inR->GetLength() != length)
        return FALSE;   // invalid parameter
    if ((X & 0x7C)!=0 || (Y & 0x7C)!=0)
        return FALSE;   // invalid parameter
    if (((X & 0x3)==0) || ((Y & 0x3)==0))
        return FALSE;   // no axis specified
    if ((X & 0x3) == (Y & 0x3))
        return FALSE;   // same axis specified

    // Z is "the other" axis, its sign is either +/- sign(X)*sign(Y)
    // this can be calculated by exclusive-or'ing X and Y; except for
    // the sign inversion (+/-) which is calculated below.
    Int32 Z = X ^ Y;

    // extract the axis (remove the sign), offset in the range 0 to 2.
    const Int32 x = (X & 0x3)-1;
    const Int32 y = (Y & 0x3)-1;
    const Int32 z = (Z & 0x3)-1;

    // compute the sign of Z (whether it needs to be inverted)
    const Int32 axis_y = (z+1)%3;
    const Int32 axis_z = (z+2)%3;
    if (((x^axis_y)|(y^axis_z)) != 0)
        Z ^= 0x80;

    const Boolean sx = (X>=0x80);
    const Boolean sy = (Y>=0x80);
    const Boolean sz = (Z>=0x80);

    // Perform R * r, in avoiding actual muls and adds.
    const Int32 rowLength = ((length==16)?4:3);
    for (Int32 j=0 ; j<3 ; j++) {
        const Int32 offset = j*rowLength;
        for (Int32 i=0 ; i<3 ; i++) {
            if (x==i)   (*outR)[offset+i] = sx ? -(*inR)[offset+0] : (*inR)[offset+0];
            if (y==i)   (*outR)[offset+i] = sy ? -(*inR)[offset+1] : (*inR)[offset+1];
            if (z==i)   (*outR)[offset+i] = sz ? -(*inR)[offset+2] : (*inR)[offset+2];
        }
    }
    if (length == 16) {
        (*outR)[3] = (*outR)[7] = (*outR)[11] = (*outR)[12] = (*outR)[13] = (*outR)[14] = 0;
        (*outR)[15] = 1;
    }

    return TRUE;
}

ECode CSensorManagerHelper::GetOrientation(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Float>* values,
    /* [out, callee] */ ArrayOf<Float>** orientation)
{
    VALIDATE_NOT_NULL(orientation);
    /*
     * 4x4 (length=16) case:
     *   /  R[ 0]   R[ 1]   R[ 2]   0  \
     *   |  R[ 4]   R[ 5]   R[ 6]   0  |
     *   |  R[ 8]   R[ 9]   R[10]   0  |
     *   \      0       0       0   1  /
     *
     * 3x3 (length=9) case:
     *   /  R[ 0]   R[ 1]   R[ 2]  \
     *   |  R[ 3]   R[ 4]   R[ 5]  |
     *   \  R[ 6]   R[ 7]   R[ 8]  /
     *
     */
    if (R->GetLength() == 9) {
        (*values)[0] = (Float)Elastos::Core::Math::Atan2((*R)[1], (*R)[4]);
        (*values)[1] = (Float)Elastos::Core::Math::Asin(-(*R)[7]);
        (*values)[2] = (Float)Elastos::Core::Math::Atan2(-(*R)[6], (*R)[8]);
    } else {
        (*values)[0] = (Float)Elastos::Core::Math::Atan2((*R)[1], (*R)[5]);
        (*values)[1] = (Float)Elastos::Core::Math::Asin(-(*R)[9]);
        (*values)[2] = (Float)Elastos::Core::Math::Atan2(-(*R)[8], (*R)[10]);
    }

    *orientation = values;
    REFCOUNT_ADD(*orientation);
    return NOERROR;
}

ECode CSensorManagerHelper::GetAltitude(
    /* [in] */ Float p0,
    /* [in] */ Float p,
    /* [out] */ Float* altitude)
{
    assert(altitude != NULL);
    const Float coef = 1.0f / 5.255f;
    *altitude = 44330.0f * (1.0f - (Float)Elastos::Core::Math::Pow(p/p0, coef));
    return NOERROR;
}

ECode CSensorManagerHelper::GetAngleChange(
    /* [in] */ ArrayOf<Float>* angleChange,
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Float>* prevR)
{
    Float rd1=0,rd4=0, rd6=0,rd7=0, rd8=0;
    Float ri0=0,ri1=0,ri2=0,ri3=0,ri4=0,ri5=0,ri6=0,ri7=0,ri8=0;
    Float pri0=0, pri1=0, pri2=0, pri3=0, pri4=0, pri5=0, pri6=0, pri7=0, pri8=0;

    if(R->GetLength() == 9) {
        ri0 = (*R)[0];
        ri1 = (*R)[1];
        ri2 = (*R)[2];
        ri3 = (*R)[3];
        ri4 = (*R)[4];
        ri5 = (*R)[5];
        ri6 = (*R)[6];
        ri7 = (*R)[7];
        ri8 = (*R)[8];
    } else if(R->GetLength() == 16) {
        ri0 = (*R)[0];
        ri1 = (*R)[1];
        ri2 = (*R)[2];
        ri3 = (*R)[4];
        ri4 = (*R)[5];
        ri5 = (*R)[6];
        ri6 = (*R)[8];
        ri7 = (*R)[9];
        ri8 = (*R)[10];
    }

    if(prevR->GetLength() == 9) {
        pri0 = (*prevR)[0];
        pri1 = (*prevR)[1];
        pri2 = (*prevR)[2];
        pri3 = (*prevR)[3];
        pri4 = (*prevR)[4];
        pri5 = (*prevR)[5];
        pri6 = (*prevR)[6];
        pri7 = (*prevR)[7];
        pri8 = (*prevR)[8];
    } else if(prevR->GetLength() == 16) {
        pri0 = (*prevR)[0];
        pri1 = (*prevR)[1];
        pri2 = (*prevR)[2];
        pri3 = (*prevR)[4];
        pri4 = (*prevR)[5];
        pri5 = (*prevR)[6];
        pri6 = (*prevR)[8];
        pri7 = (*prevR)[9];
        pri8 = (*prevR)[10];
    }

    // calculate the parts of the rotation difference matrix we need
    // rd[i][j] = pri[0][i] * ri[0][j] + pri[1][i] * ri[1][j] + pri[2][i] * ri[2][j];

    rd1 = pri0 * ri1 + pri3 * ri4 + pri6 * ri7; //rd[0][1]
    rd4 = pri1 * ri1 + pri4 * ri4 + pri7 * ri7; //rd[1][1]
    rd6 = pri2 * ri0 + pri5 * ri3 + pri8 * ri6; //rd[2][0]
    rd7 = pri2 * ri1 + pri5 * ri4 + pri8 * ri7; //rd[2][1]
    rd8 = pri2 * ri2 + pri5 * ri5 + pri8 * ri8; //rd[2][2]

    (*angleChange)[0] = (Float)Elastos::Core::Math::Atan2(rd1, rd4);
    (*angleChange)[1] = (Float)Elastos::Core::Math::Asin(-rd7);
    (*angleChange)[2] = (Float)Elastos::Core::Math::Atan2(-rd6, rd8);

    return NOERROR;
}

ECode CSensorManagerHelper::GetRotationMatrixFromVector(
    /* [in] */ ArrayOf<Float>* R,
    /* [in] */ ArrayOf<Float>* rotationVector)
{
    Float q0;
    Float q1 = (*rotationVector)[0];
    Float q2 = (*rotationVector)[1];
    Float q3 = (*rotationVector)[2];

    if (rotationVector->GetLength() == 4) {
        q0 = (*rotationVector)[3];
    } else {
        q0 = 1 - q1*q1 - q2*q2 - q3*q3;
        q0 = (q0 > 0) ? (Float)Elastos::Core::Math::Sqrt(q0) : 0;
    }

    Float sq_q1 = 2 * q1 * q1;
    Float sq_q2 = 2 * q2 * q2;
    Float sq_q3 = 2 * q3 * q3;
    Float q1_q2 = 2 * q1 * q2;
    Float q3_q0 = 2 * q3 * q0;
    Float q1_q3 = 2 * q1 * q3;
    Float q2_q0 = 2 * q2 * q0;
    Float q2_q3 = 2 * q2 * q3;
    Float q1_q0 = 2 * q1 * q0;

    if(R->GetLength() == 9) {
        (*R)[0] = 1 - sq_q2 - sq_q3;
        (*R)[1] = q1_q2 - q3_q0;
        (*R)[2] = q1_q3 + q2_q0;

        (*R)[3] = q1_q2 + q3_q0;
        (*R)[4] = 1 - sq_q1 - sq_q3;
        (*R)[5] = q2_q3 - q1_q0;

        (*R)[6] = q1_q3 - q2_q0;
        (*R)[7] = q2_q3 + q1_q0;
        (*R)[8] = 1 - sq_q1 - sq_q2;
    } else if (R->GetLength() == 16) {
        (*R)[0] = 1 - sq_q2 - sq_q3;
        (*R)[1] = q1_q2 - q3_q0;
        (*R)[2] = q1_q3 + q2_q0;
        (*R)[3] = 0.0f;

        (*R)[4] = q1_q2 + q3_q0;
        (*R)[5] = 1 - sq_q1 - sq_q3;
        (*R)[6] = q2_q3 - q1_q0;
        (*R)[7] = 0.0f;

        (*R)[8] = q1_q3 - q2_q0;
        (*R)[9] = q2_q3 + q1_q0;
        (*R)[10] = 1 - sq_q1 - sq_q2;
        (*R)[11] = 0.0f;

        (*R)[12] = (*R)[13] = (*R)[14] = 0.0f;
        (*R)[15] = 1.0f;
    }

    return NOERROR;
}

ECode CSensorManagerHelper::GetQuaternionFromVector(
    /* [in] */ ArrayOf<Float>* Q,
    /* [in] */ ArrayOf<Float>* rv)
{
    if (rv->GetLength() == 4) {
        (*Q)[0] = (*rv)[3];
    } else {
        (*Q)[0] = 1 - (*rv)[0]*(*rv)[0] - (*rv)[1]*(*rv)[1] - (*rv)[2]*(*rv)[2];
        (*Q)[0] = ((*Q)[0] > 0) ? (Float)Elastos::Core::Math::Sqrt((*Q)[0]) : 0;
    }
    (*Q)[1] = (*rv)[0];
    (*Q)[2] = (*rv)[1];
    (*Q)[3] = (*rv)[2];

    return NOERROR;
}

} // namespace Hardware
} // namespace Droid
} // namespace Elastos
