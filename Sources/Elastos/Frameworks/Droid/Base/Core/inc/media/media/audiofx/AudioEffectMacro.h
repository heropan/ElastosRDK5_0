
#ifndef __AUDIOEFFECTMACRO_H__
#define __AUDIOEFFECTMACRO_H__

namespace Elastos {
namespace Droid {
namespace Media {
namespace Audiofx {

#define IAUDIOEFFECT_METHODS_DECL()                                       \
    CARAPI ReleaseResources();                                            \
                                                                          \
    CARAPI GetDescriptor(                                                 \
        /* [out] */ IAudioEffectDescriptor** descriptor);                 \
                                                                          \
    CARAPI SetEnabled(                                                    \
        /* [in]  */ Boolean enabled,                                      \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameter(                                                  \
        /* [in] */ ArrayOf<Byte>* param,                                  \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameterEx(                                                \
        /* [in] */ Int32 param,                                           \
        /* [in] */ Int32 value,                                           \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameterEx2(                                               \
        /* [in] */ Int32 param,                                           \
        /* [in] */ Int16 value,                                           \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameterEx3(                                               \
        /* [in] */ Int32 param,                                           \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameterEx4(                                               \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Int32>* value,                                 \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameterEx5(                                               \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Int16>* value,                                 \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI SetParameterEx6(                                               \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI GetParameter(                                                  \
        /* [in] */ ArrayOf<Byte>* param,                                  \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI GetParameterEx(                                                \
        /* [in] */ Int32 param,                                           \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI GetParameterEx2(                                               \
        /* [in] */ Int32 param,                                           \
        /* [in] */ ArrayOf<Int32>* value,                                 \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI GetParameterEx3(                                               \
        /* [in] */ Int32 param,                                           \
        /* [in] */ ArrayOf<Int16>* value,                                 \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI GetParameterEx4(                                               \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Int32>* value,                                 \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI GetParameterEx5(                                               \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Int16>* value,                                 \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI GetParameterEx6(                                               \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* status);                                       \
                                                                          \
    CARAPI Command(                                                       \
        /* [in] */ Int32 cmdCode,                                         \
        /* [in] */ ArrayOf<Byte>* command,                                \
        /* [in] */ ArrayOf<Byte>* reply,                                  \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI GetId(                                                         \
        /* [out] */ Int32* Id);                                           \
                                                                          \
    CARAPI GetEnabled(                                                    \
        /* [out] */ Boolean* enabled);                                    \
                                                                          \
    CARAPI HasControl(                                                    \
        /* [out] */ Boolean* control);                                    \
                                                                          \
    CARAPI SetEnableStatusListener(                                       \
        /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener);   \
                                                                          \
    CARAPI SetControlStatusListener(                                      \
        /* [in] */ IAudioEffectOnControlStatusChangeListener* listener);  \
                                                                          \
    CARAPI SetParameterListener(                                          \
        /* [in] */ IAudioEffectOnParameterChangeListener* listener);      \
                                                                          \
    CARAPI CheckState(                                                    \
        /* [in] */ const String& methodName);                             \
                                                                          \
    CARAPI CheckStatus(                                                   \
        /* [in] */ Int32 status);                                         \
                                                                          \
    CARAPI ByteArrayToInt32(                                              \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI ByteArrayToInt32Ex(                                            \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [in] */ Int32 offset,                                          \
        /* [out] */ Int32* result);                                       \
                                                                          \
    CARAPI Int32ToByteArray(                                              \
        /* [in] */ Int32 value,                                           \
        /* [out, callee] */ ArrayOf<Byte>** result);                      \
                                                                          \
    CARAPI ByteArrayToInt16(                                              \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [out] */ Int16* result);                                       \
                                                                          \
    CARAPI ByteArrayToInt16Ex(                                            \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [in] */ Int32 offset,                                          \
        /* [out] */ Int16* result);                                       \
                                                                          \
    CARAPI Int16ToByteArray(                                              \
        /* [in] */ Int16 value,                                           \
        /* [out, callee] */ ArrayOf<Byte>** result);                      \
                                                                          \
    CARAPI ConcatArrays(                                                  \
        /* [in] */ ArrayOf<Byte>* array1,                                 \
        /* [in] */ ArrayOf<Byte>* array2,                                 \
        /* [out, callee] */ ArrayOf<Byte>** result);


#define IAUDIOEFFECT_METHODS_IMPL(className, superClass)                  \
    ECode className::ReleaseResources()                                   \
    {                                                                     \
        return superClass::ReleaseResources();                            \
    }                                                                     \
                                                                          \
    ECode className::GetDescriptor(                                       \
        /* [out] */ IAudioEffectDescriptor** descriptor)                  \
    {                                                                     \
        return superClass::GetDescriptor(descriptor);                     \
    }                                                                     \
                                                                          \
    ECode className::SetEnabled(                                          \
        /* [in]  */ Boolean enabled,                                      \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetEnabled(enabled, result);                   \
    }                                                                     \
                                                                          \
    ECode className::SetParameter(                                        \
        /* [in] */ ArrayOf<Byte>* param,                                  \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameter(param, value, result);            \
    }                                                                     \
                                                                          \
    ECode className::SetParameterEx(                                      \
        /* [in] */ Int32 param,                                           \
        /* [in] */ Int32 value,                                           \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameterEx(param, value, result);          \
    }                                                                     \
                                                                          \
    ECode className::SetParameterEx2(                                     \
        /* [in] */ Int32 param,                                           \
        /* [in] */ Int16 value,                                           \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameterEx2(param, value, result);         \
    }                                                                     \
                                                                          \
    ECode className::SetParameterEx3(                                     \
        /* [in] */ Int32 param,                                           \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameterEx3(param, value, result);         \
    }                                                                     \
                                                                          \
    ECode className::SetParameterEx4(                                     \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Int32>* value,                                 \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameterEx4(param, value, result);         \
    }                                                                     \
                                                                          \
    ECode className::SetParameterEx5(                                     \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Int16>* value,                                 \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameterEx5(param, value, result);         \
    }                                                                     \
                                                                          \
    ECode className::SetParameterEx6(                                     \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::SetParameterEx6(param, value, result);         \
    }                                                                     \
                                                                          \
    ECode className::GetParameter(                                        \
        /* [in] */ ArrayOf<Byte>* param,                                  \
        /* [out] */ ArrayOf<Byte>* value,                                 \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameter(param, value, status);            \
    }                                                                     \
                                                                          \
    ECode className::GetParameterEx(                                      \
        /* [in] */ const Int32 param,                                     \
        /* [out] */ ArrayOf<Byte>* value,                                 \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameterEx(param, value, status);          \
    }                                                                     \
                                                                          \
    ECode className::GetParameterEx2(                                     \
        /* [in] */ Int32 param,                                           \
        /* [out] */ ArrayOf<Int32>* value,                                \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameterEx2(param, value, status);         \
    }                                                                     \
                                                                          \
    ECode className::GetParameterEx3(                                     \
        /* [in] */ Int32 param,                                           \
        /* [out] */ ArrayOf<Int16>* value,                                \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameterEx3(param, value, status);         \
    }                                                                     \
                                                                          \
    ECode className::GetParameterEx4(                                     \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [out] */ ArrayOf<Int32>* value,                                \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameterEx4(param, value, status);         \
    }                                                                     \
                                                                          \
    ECode className::GetParameterEx5(                                     \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [out] */ ArrayOf<Int16>* value,                                \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameterEx5(param, value, status);         \
    }                                                                     \
                                                                          \
    ECode className::GetParameterEx6(                                     \
        /* [in] */ ArrayOf<Int32>* param,                                 \
        /* [in] */ ArrayOf<Byte>* value,                                  \
        /* [out] */ Int32* status)                                        \
    {                                                                     \
        return superClass::GetParameterEx6(param, value, status);         \
    }                                                                     \
                                                                          \
    ECode className::Command(                                             \
        /* [in] */ Int32 cmdCode,                                         \
        /* [in] */ ArrayOf<Byte>* command,                                \
        /* [in] */ ArrayOf<Byte>* reply,                                  \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::Command(cmdCode, command, reply, result);      \
    }                                                                     \
                                                                          \
    ECode className::GetId(                                               \
        /* [out] */ Int32* Id)                                            \
    {                                                                     \
        return superClass::GetId(Id);                                     \
    }                                                                     \
                                                                          \
    ECode className::GetEnabled(                                          \
        /* [out] */ Boolean* enabled)                                     \
    {                                                                     \
        return superClass::GetEnabled(enabled);                           \
    }                                                                     \
                                                                          \
    ECode className::HasControl(                                          \
        /* [out] */ Boolean* control)                                     \
    {                                                                     \
        return superClass::HasControl(control);                           \
    }                                                                     \
                                                                          \
    ECode className::SetEnableStatusListener(                             \
        /* [in] */ IAudioEffectOnEnableStatusChangeListener* listener)    \
    {                                                                     \
        return superClass::SetEnableStatusListener(listener);             \
    }                                                                     \
                                                                          \
    ECode className::SetControlStatusListener(                            \
        /* [in] */ IAudioEffectOnControlStatusChangeListener* listener)   \
    {                                                                     \
        return superClass::SetControlStatusListener(listener);            \
    }                                                                     \
                                                                          \
    ECode className::SetParameterListener(                                \
        /* [in] */ IAudioEffectOnParameterChangeListener* listener)       \
    {                                                                     \
        return superClass::SetParameterListener(listener);                \
    }                                                                     \
                                                                          \
    ECode className::CheckState(                                          \
        /* [in] */ const String& methodName)                              \
    {                                                                     \
        return superClass::CheckState(methodName);                        \
    }                                                                     \
                                                                          \
    ECode className::CheckStatus(                                         \
        /* [in] */ Int32 status)                                          \
    {                                                                     \
        return superClass::CheckStatus(status);                           \
    }                                                                     \
                                                                          \
    ECode className::ByteArrayToInt32(                                    \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::ByteArrayToInt32(valueBuf, result);            \
    }                                                                     \
                                                                          \
    ECode className::ByteArrayToInt32Ex(                                  \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [in] */ Int32 offset,                                          \
        /* [out] */ Int32* result)                                        \
    {                                                                     \
        return superClass::ByteArrayToInt32Ex(valueBuf, offset, result);  \
    }                                                                     \
                                                                          \
    ECode className::Int32ToByteArray(                                    \
        /* [in] */ Int32 value,                                           \
        /* [out, callee] */ ArrayOf<Byte>** result)                       \
    {                                                                     \
        return superClass::Int32ToByteArray(value, result);               \
    }                                                                     \
                                                                          \
    ECode className::ByteArrayToInt16(                                    \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [out] */ Int16* result)                                        \
    {                                                                     \
        return superClass::ByteArrayToInt16(valueBuf, result);            \
    }                                                                     \
                                                                          \
    ECode className::ByteArrayToInt16Ex(                                  \
        /* [in] */ ArrayOf<Byte>* valueBuf,                               \
        /* [in] */ Int32 offset,                                          \
        /* [out] */ Int16* result)                                        \
    {                                                                     \
        return superClass::ByteArrayToInt16Ex(valueBuf, offset, result);  \
    }                                                                     \
                                                                          \
    ECode className::Int16ToByteArray(                                    \
        /* [in] */ Int16 value,                                           \
        /* [out, callee] */ ArrayOf<Byte>** result)                       \
    {                                                                     \
        return superClass::Int16ToByteArray(value, result);               \
    }                                                                     \
                                                                          \
    ECode className::ConcatArrays(                                        \
        /* [in] */ ArrayOf<Byte>* array1,                                 \
        /* [in] */ ArrayOf<Byte>* array2,                                 \
        /* [out, callee] */ ArrayOf<Byte>** result)                       \
    {                                                                     \
        return superClass::ConcatArrays(array1, array2, result);          \
    }

} // namespace Audiofx
} // namespace Media
} // namepsace Droid
} // namespace Elastos

#endif // __AUDIOEFFECTMACRO_H__