
#include "elastos/droid/view/CKeyCharacterMapHelper.h"
#include "elastos/droid/view/CKeyCharacterMap.h"

using namespace Elastos::Droid::View;

ECode CKeyCharacterMapHelper::Load(
    /* [in] */ Int32 deviceId,
    /* [out] */ IKeyCharacterMap** kcm)
{
    return CKeyCharacterMap::Load(deviceId, kcm);
}

ECode CKeyCharacterMapHelper::DeviceHasKey(
    /* [in] */ Int32 keyCode,
    /* [out] */ Boolean* hasKey)
{
    return CKeyCharacterMap::DeviceHasKey(keyCode, hasKey);
}

ECode CKeyCharacterMapHelper::GetDeadChar(
    /* [in] */ Int32 accent,
    /* [in] */ Int32 c,
    /* [out] */ Int32* deadChar)
{
    VALIDATE_NOT_NULL(deadChar);
    *deadChar = CKeyCharacterMap::GetDeadChar(accent, c);

    return NOERROR;
}

ECode CKeyCharacterMapHelper::DeviceHasKeys(
    /* [in] */ const ArrayOf<Int32>& keyCodes,
    /* [in] */ ArrayOf<Boolean>** hasKeys)
{
    return CKeyCharacterMap::DeviceHasKeys(keyCodes, hasKeys);
}
