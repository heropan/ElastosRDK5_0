
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Net {

String GURLUtils::GetOrigin(
	/* [in] */ String url)
{
	return NativeGetOrigin(url);
}

String GURLUtils::GetScheme(
	/* [in] */ String url)
{
	return NativeGetScheme(url);
}

// question: how to translate a native function.
//private static native String nativeGetOrigin(String url);
String GURLUtils::NativeGetOrigin(
	/* [in] */ String url)
{
	return String("");
}

//private static native String nativeGetScheme(String url);
String GURLUtils::NativeGetScheme(
	/* [in] */ String url)
{
	return String("");
}

} // namespace Net
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
