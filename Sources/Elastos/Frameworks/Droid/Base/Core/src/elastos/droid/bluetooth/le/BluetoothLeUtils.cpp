
#include "elastos/droid/bluetooth/le/BluetoothLeUtils.h"

namespace Elastos {
namespace Droid {
namespace Bluetooth {
namespace LE {

//=====================================================================
//                           BluetoothLeUtils
//=====================================================================
String BluetoothLeUtils::ToString(
    /* [in] */ IMap* map)
{
    // ==================before translated======================
    // if (map == null) {
    //     return "null";
    // }
    // if (map.isEmpty()) {
    //     return "{}";
    // }
    // StringBuilder buffer = new StringBuilder();
    // buffer.append('{');
    // Iterator<Map.Entry<T, byte[]>> it = map.entrySet().iterator();
    // while (it.hasNext()) {
    //     Map.Entry<T, byte[]> entry = it.next();
    //     Object key = entry.getKey();
    //     buffer.append(key).append("=").append(Arrays.toString(map.get(key)));
    //     if (it.hasNext()) {
    //         buffer.append(", ");
    //     }
    // }
    // buffer.append('}');
    // return buffer.toString();
    return String("");
}

Boolean BluetoothLeUtils::Equals(
    /* [in] */ IMap* map,
    /* [in] */ IMap* otherMap)
{
    // ==================before translated======================
    // if (map == otherMap) {
    //     return true;
    // }
    // if (map == null || otherMap == null) {
    //     return false;
    // }
    // if (map.size() != otherMap.size()) {
    //     return false;
    // }
    // Set<T> keys = map.keySet();
    // if (!keys.equals(otherMap.keySet())) {
    //     return false;
    // }
    // for (T key : keys) {
    //     if (!Objects.deepEquals(map.get(key), otherMap.get(key))) {
    //         return false;
    //     }
    // }
    // return true;
    return TRUE;
}

String BluetoothLeUtils::ToString(
    /* [in] */ ISparseArray* array)
{
    // ==================before translated======================
    // if (array == null) {
    //     return "null";
    // }
    // if (array.size() == 0) {
    //     return "{}";
    // }
    // StringBuilder buffer = new StringBuilder();
    // buffer.append('{');
    // for (int i = 0; i < array.size(); ++i) {
    //     buffer.append(array.keyAt(i)).append("=").append(Arrays.toString(array.valueAt(i)));
    // }
    // buffer.append('}');
    // return buffer.toString();
    assert(0);
    return String("");
}

Boolean BluetoothLeUtils::Equals(
    /* [in] */ ISparseArray* array,
    /* [in] */ ISparseArray* otherArray)
{
    // ==================before translated======================
    // if (array == otherArray) {
    //     return true;
    // }
    // if (array == null || otherArray == null) {
    //     return false;
    // }
    // if (array.size() != otherArray.size()) {
    //     return false;
    // }
    //
    // // Keys are guaranteed in ascending order when indices are in ascending order.
    // for (int i = 0; i < array.size(); ++i) {
    //     if (array.keyAt(i) != otherArray.keyAt(i) ||
    //             !Arrays.equals(array.valueAt(i), otherArray.valueAt(i))) {
    //         return false;
    //     }
    // }
    // return true;
    assert(0);
    return TRUE;
}

ECode BluetoothLeUtils::CheckAdapterStateOn(
    /* [in] */ IBluetoothAdapter* adapter)
{
    VALIDATE_NOT_NULL(adapter);
    // ==================before translated======================
    // if (adapter == null || adapter.getState() != BluetoothAdapter.STATE_ON) {
    //     throw new IllegalStateException("BT Adapter is not turned ON");
    // }
    assert(0);
    return NOERROR;
}

} // namespace LE
} // namespace Bluetooth
} // namespace Droid
} // namespace Elastos
