
#define IMESSAGEDIGEST_METHODS_DECL()                                                 \
    CARAPI Reset();                                                                   \
                                                                                      \
    CARAPI Update(                                                                    \
        /* [in] */ Byte arg0);                                                        \
                                                                                      \
    CARAPI UpdateEx(                                                                  \
        /* [in] */ ArrayOf<Byte>* input,                                              \
        /* [in] */ Int32 offset,                                                      \
        /* [in] */ Int32 len);                                                        \
                                                                                      \
    CARAPI UpdateEx2(                                                                 \
        /* [in] */ ArrayOf<Byte>* input);                                             \
                                                                                      \
    CARAPI Digest(                                                                    \
        /* [out, callee] */ ArrayOf<Byte>** hashValue);                               \
                                                                                      \
    CARAPI DigestEx(                                                                  \
        /* [in] */ ArrayOf<Byte>* buf,                                                \
        /* [in] */ Int32 offset,                                                      \
        /* [in] */ Int32 len,                                                         \
        /* [out] */ Int32* number);                                                   \
                                                                                      \
    CARAPI DigestEx2(                                                                 \
        /* [in] */ ArrayOf<Byte>* input,                                              \
        /* [out, callee] */ ArrayOf<Byte>** hashValue);                               \
                                                                                      \
    CARAPI ToString(                                                                  \
        /* [out] */ String* str);                                                     \
                                                                                      \
    CARAPI GetAlgorithm(                                                              \
        /* [out] */ String* algorithm);                                               \
                                                                                      \
    CARAPI GetProvider(                                                               \
        /* [out, callee] */ IProvider** provider);                                    \
                                                                                      \
    CARAPI GetDigestLength(                                                           \
        /* [out] */ Int32* length);                                                   \
                                                                                      \
    CARAPI UpdateEx3(                                                                 \
        /* [in] */ IByteBuffer* input);                                               \
                                                                                      \
                                                                                      \
    CARAPI EngineUpdateEx2(                                                           \
        /* [in] */ IByteBuffer* input);                                               \
                                                                                      \
    CARAPI EngineDigestEx(                                                            \
        /* [in] */ ArrayOf<Byte>* buf,                                                  \
        /* [in] */ Int32 offset,                                                        \
        /* [in] */ Int32 len,                                                           \
        /* [out] */ Int32* digest);


#define IMESSAGEDIGEST_METHODS_IMPL(className, superClass)                      \
ECode className::Reset()                                                        \
{                                                                               \
    return superClass::Reset();                                                 \
}                                                                               \
                                                                                \
ECode className::Update(                                                        \
        /* [in] */ Byte arg0)                                                   \
{                                                                               \
    return superClass::Update(arg0);                                            \
}                                                                               \
                                                                                \
ECode className::UpdateEx(                                                      \
    /* [in] */ ArrayOf<Byte>* input,                                            \
    /* [in] */ Int32 offset,                                                    \
    /* [in] */ Int32 len)                                                       \
{                                                                               \
    return superClass::UpdateEx(input, offset, len);                            \
}                                                                               \
                                                                                \
ECode className::UpdateEx2(                                                     \
    /* [in] */ ArrayOf<Byte>* input)                                            \
{                                                                               \
    return superClass::UpdateEx2(input);                                        \
}                                                                               \
                                                                                \
ECode className::Digest(                                                        \
    /* [out, callee] */ ArrayOf<Byte>** hashValue)                              \
{                                                                               \
    return superClass::Digest(hashValue);                                       \
}                                                                               \
                                                                                \
ECode className::DigestEx(                                                      \
    /* [in] */ ArrayOf<Byte>* buf,                                              \
    /* [in] */ Int32 offset,                                                    \
    /* [in] */ Int32 len,                                                       \
    /* [out] */ Int32* number)                                                  \
{                                                                               \
    return superClass::DigestEx(buf, offset, len, number);                      \
}                                                                               \
                                                                                \
ECode className::DigestEx2(                                                     \
    /* [in] */ ArrayOf<Byte>* input,                                            \
    /* [out, callee] */ ArrayOf<Byte>** hashValue)                              \
{                                                                               \
    return superClass::DigestEx2(input, hashValue);                             \
}                                                                               \
                                                                                \
ECode className::ToString(                                                      \
    /* [out] */ String* str)                                                    \
{                                                                               \
    return superClass::ToString(str);                                           \
}                                                                               \
                                                                                \
ECode className::GetAlgorithm(                                                  \
    /* [out] */ String* algorithm)                                              \
{                                                                               \
    return superClass::GetAlgorithm(algorithm);                                 \
}                                                                               \
                                                                                \
ECode className::GetProvider(                                                   \
    /* [out, callee] */ IProvider** provider)                                   \
{                                                                               \
    return superClass::GetProvider(provider);                                   \
}                                                                               \
                                                                                \
ECode className::GetDigestLength(                                               \
        /* [out] */ Int32* length)                                              \
{                                                                               \
    return superClass::GetDigestLength(length);                                 \
}                                                                               \
                                                                                \
ECode className::UpdateEx3(                                                     \
        /* [in] */ IByteBuffer* input)                                          \
{                                                                               \
    return superClass::UpdateEx3(input);                                        \
}                                                                               \
                                                                                \
ECode className::EngineUpdateEx2(                                               \
        /* [in] */ IByteBuffer* input)                                          \
{                                                                               \
    return superClass::EngineUpdateEx2(input);                                  \
}                                                                               \
ECode className::EngineDigestEx(                                                \
    /* [in] */ ArrayOf<Byte>* buf,                                              \
    /* [in] */ Int32 offset,                                                    \
    /* [in] */ Int32 len,                                                       \
    /* [out] */ Int32* digest)                                                  \
{                                                                               \
    return superClass::EngineDigestEx(buf, offset, len, digest);                \
}
