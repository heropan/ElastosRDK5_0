
namespace Elastos {
namespace Droid {
namespace Webkit {
namespace Base {

//===============================================================
//              CommandLine::JavaCommandLine
//===============================================================

CommandLine::JavaCommandLine::JavaCommandLine(
    /* [in] */ ArrayOf<String>* args)
    : mArgsBegin(1)
{
    if (args == NULL || args->GetLength() == 0 || (*args)[0] == NULL) {
        mArgs.add("");
    }
    else {
        mArgs.add(args[0]);
        AppendSwitchesInternal(args, 1);
    }
    // Invariant: we always have the argv[0] program name element.
    assert mArgs.size() > 0;
}

/**
 * Returns the switches and arguments passed into the program, with switches and their
 * values coming before all of the arguments.
 */
AutoPtr< ArrayOf<String> > CommandLine::JavaCommandLine::GetCommandLineArguments()
{
    return mArgs.toArray(new String[mArgs.size()]);
}

//@Override
Boolean CommandLine::JavaCommandLine::HasSwitch(
    /* [in] */ String switchString)
{
    return mSwitches.containsKey(switchString);
}

//@Override
String CommandLine::JavaCommandLine::GetSwitchValue(
    /* [in] */ String switchString)
{
    // This is slightly round about, but needed for consistency with the NativeCommandLine
    // version which does not distinguish empty values from key not present.
    String value = mSwitches.get(switchString);
    return value == NULL || value.IsEmpty() ? NULL : value;
}

//@Override
void CommandLine::JavaCommandLine::AppendSwitch(
    /* [in] */ String switchString)
{
    AppendSwitchWithValue(switchString, NULL);
}

/**
 * Appends a switch to the current list.
 * @param switchString the switch to add.  It should NOT start with '--' !
 * @param value the value for this switch.
 */
//@Override
void CommandLine::JavaCommandLine::AppendSwitchWithValue(
    /* [in] */ String switchString,
    /* [in] */ String value)
{
    mSwitches.put(switchString, value == null ? "" : value);

    // Append the switch and update the switches/arguments divider mArgsBegin.
    String combinedSwitchString = SWITCH_PREFIX + switchString;
    if (value != null && !value.isEmpty())
        combinedSwitchString += SWITCH_VALUE_SEPARATOR + value;

    mArgs.add(mArgsBegin++, combinedSwitchString);
}

//@Override
void CommandLine::JavaCommandLine::AppendSwitchesAndArguments(
    /* [in] */ ArrayOf<String>* array)
{
    AppendSwitchesInternal(array, 0);
}

// Add the specified arguments, but skipping the first |skipCount| elements.
void CommandLine::JavaCommandLine::AppendSwitchesInternal(
    /* [in] */ ArrayOf<String>* array,
    /* [in] */ Int32 skipCount)
{
    Boolean parseSwitches = TRUE;
    Int32 length = array->GetLength();
    for (Int32 i = 0; i < length; ++i) {
        arg String = (*array)[i];
        if (skipCount > 0) {
            --skipCount;
            continue;
        }

        if (arg.Equals(SWITCH_TERMINATOR)) {
            parseSwitches = false;
        }

        if (parseSwitches && arg.StartsWith(SWITCH_PREFIX)) {
            String[] parts = arg.Split(SWITCH_VALUE_SEPARATOR, 2);
            String value = parts.length > 1 ? parts[1] : null;
            AppendSwitchWithValue(parts[0].Substring(SWITCH_PREFIX.GetLength()), value);
        }
        else {
            mArgs.add(arg);
        }
    }
}

//===============================================================
//               CommandLine::NativeCommandLine
//===============================================================

//@Override
Boolean CommandLine::NativeCommandLine::HasSwitch(
    /* [in] */ String switchString)
{
    return NativeHasSwitch(switchString);
}

//@Override
String CommandLine::NativeCommandLine::GetSwitchValue(
    /* [in] */ String switchString)
{
    return NativeGetSwitchValue(switchString);
}

//@Override
void CommandLine::NativeCommandLine::AppendSwitch(
    /* [in] */ String switchString)
{
    NativeAppendSwitch(switchString);
}

//@Override
void CommandLine::NativeCommandLine::AppendSwitchWithValue(
    /* [in] */ String switchString,
    /* [in] */ String value)
{
    NativeAppendSwitchWithValue(switchString, value);
}

//@Override
void CommandLine::NativeCommandLine::AppendSwitchesAndArguments(
    /* [in] */ ArrayOf<String>* array)
{
    NativeAppendSwitchesAndArguments(array);
}

//@Override
Boolean CommandLine::NativeCommandLine::IsNativeImplementation()
{
    return TRUE;
}

//===============================================================
//                         CommandLine
//===============================================================

static const String CommandLine::TAG("CommandLine");
static const String CommandLine::SWITCH_PREFIX("--");
static const String CommandLine::SWITCH_TERMINATOR(SWITCH_PREFIX);
static const String CommandLine::SWITCH_VALUE_SEPARATOR("=");

CommandLine::CommandLine()
{
}

/**
 * Return the value associated with the given switch, or {@code defaultValue} if the switch
 * was not specified.
 * @param switchString The switch key to lookup. It should NOT start with '--' !
 * @param defaultValue The default value to return if the switch isn't set.
 * @return Switch value, or {@code defaultValue} if the switch is not set or set to empty.
 */
String CommandLine::GetSwitchValue(
    /* [in] */ String switchString,
    /* [in] */ String defaultValue)
{
    String value = getSwitchValue(switchString);
    return TextUtils.isEmpty(value) ? defaultValue : value;
}

/**
 * Determine if the command line is bound to the native (JNI) implementation.
 * @return true if the underlying implementation is delegating to the native command line.
 */
Boolean CommandLine::IsNativeImplementation()
{
    return FALSE;
}

/**
 * @returns true if the command line has already been initialized.
 */
Boolean CommandLine::IsInitialized()
{
    return sCommandLine.get() != null;
}

// Equivalent to CommandLine::ForCurrentProcess in C++.
AutoPtr<CommandLine> CommandLine::GetInstance()
{
    CommandLine commandLine = sCommandLine.get();
    assert commandLine != null;
    return commandLine;
}

/**
 * Initialize the singleton instance, must be called exactly once (either directly or
 * via one of the convenience wrappers below) before using the static singleton instance.
 * @param args command line flags in 'argv' format: args[0] is the program name.
 */
void CommandLine::Init(
    /* [in] */ ArrayOf<String>* args)
{
    AutoPtr<CommandLine> instance = new JavaCommandLine(args);
    SetInstance(instance);
}

/**
 * Initialize the command line from the command-line file.
 *
 * @param file The fully qualified command line file.
 */
void CommandLine::InitFromFile(
    /* [in] */ String file)
{
    // Arbitrary clamp of 8k on the amount of file we read in.
    AutoPtr< ArrayOf<Int8> > buffer = ReadUtf8FileFully(file, 8 * 1024);
    Init(buffer == NULL ? NULL : TokenizeQuotedAruments(buffer));
}

/**
 * Resets both the java proxy and the native command lines. This allows the entire
 * command line initialization to be re-run including the call to onJniLoaded.
 */
void CommandLine::Reset()
{
    SetInstance(NULL);
}

/**
 * Public for testing (TODO: why are the tests in a different package?)
 * Parse command line flags from a flat buffer, supporting double-quote enclosed strings
 * containing whitespace. argv elements are derived by splitting the buffer on whitepace;
 * double quote characters may enclose tokens containing whitespace; a double-quote literal
 * may be escaped with back-slash. (Otherwise backslash is taken as a literal).
 * @param buffer A command line in command line file format as described above.
 * @return the tokenized arguments, suitable for passing to init().
 */
AutoPtr< ArrayOf<String> > CommandLine::TokenizeQuotedAruments(
    /* [in] */ ArrayOf<Int8>* buffer)
{
    ArrayList<String> args = new ArrayList<String>();
    StringBuilder arg = null;
    final char noQuote = '\0';
    final char singleQuote = '\'';
    final char doubleQuote = '"';
    char currentQuote = noQuote;
    for (char c : buffer) {
        // Detect start or end of quote block.
        if ((currentQuote == noQuote && (c == singleQuote || c == doubleQuote)) ||
            c == currentQuote) {
            if (arg != null && arg.length() > 0 && arg.charAt(arg.length() - 1) == '\\') {
                // Last char was a backslash; pop it, and treat c as a literal.
                arg.setCharAt(arg.length() - 1, c);
            } else {
                currentQuote = currentQuote == noQuote ? c : noQuote;
            }
        } else if (currentQuote == noQuote && Character.isWhitespace(c)) {
            if (arg != null) {
                args.add(arg.toString());
                arg = null;
            }
        } else {
            if (arg == null) arg = new StringBuilder();
            arg.append(c);
        }
    }
    if (arg != null) {
        if (currentQuote != noQuote) {
            Log.w(TAG, "Unterminated quoted string: " + arg);
        }
        args.add(arg.toString());
    }
    return args.toArray(new String[args.size()]);
}

void CommandLine::EnableNativeProxy()
{
    // Make a best-effort to ensure we make a clean (atomic) switch over from the old to
    // the new command line implementation. If another thread is modifying the command line
    // when this happens, all bets are off. (As per the native CommandLine).
    sCommandLine.set(new NativeCommandLine());
}

AutoPtr< ArrayOf<String> > CommandLine::GetJavaSwitchesOrNull()
{
    CommandLine commandLine = sCommandLine.get();
    if (commandLine != null) {
        assert !commandLine.isNativeImplementation();
        return ((JavaCommandLine) commandLine).getCommandLineArguments();
    }
    return null;
}

void CommandLine::SetInstance(
    /* [in] */ CommandLine* commandLine)
{
    CommandLine oldCommandLine = sCommandLine.getAndSet(commandLine);
    if (oldCommandLine != null && oldCommandLine.isNativeImplementation()) {
        nativeReset();
    }
}

/**
 * @param fileName the file to read in.
 * @param sizeLimit cap on the file size.
 * @return Array of chars read from the file, or null if the file cannot be read
 *         or if its length exceeds |sizeLimit|.
 */
AutoPtr< ArrayOf<Int8> > CommandLine::ReadUtf8FileFully(
    /* [in] */ String fileName,
    /* [in] */ Int32 sizeLimit)
{
    Reader reader = null;
    File f = new File(fileName);
    long fileLength = f.length();

    if (fileLength == 0) {
        return null;
    }

    if (fileLength > sizeLimit) {
        Log.w(TAG, "File " + fileName + " length " + fileLength + " exceeds limit "
                + sizeLimit);
        return null;
    }

    try {
        char[] buffer = new char[(int) fileLength];
        reader = new InputStreamReader(new FileInputStream(f), "UTF-8");
        int charsRead = reader.read(buffer);
        // Debug check that we've exhausted the input stream (will fail e.g. if the
        // file grew after we inspected its length).
        assert !reader.ready();
        return charsRead < buffer.length ? Arrays.copyOfRange(buffer, 0, charsRead) : buffer;
    } catch (FileNotFoundException e) {
        return null;
    } catch (IOException e) {
        return null;
    } finally {
        try {
            if (reader != null) reader.close();
        } catch (IOException e) {
            Log.e(TAG, "Unable to close file reader.", e);
        }
    }
}

void CommandLine::NativeReset()
{
}

Boolean CommandLine::NativeHasSwitch(
    /* [in] */ String switchString)
{
}

String CommandLine::NativeGetSwitchValue(
    /* [in] */ String switchString)
{
}

void CommandLine::NativeAppendSwitch(
    /* [in] */ String switchString);
void CommandLine::NativeAppendSwitchWithValue(
    /* [in] */ String switchString,
    /* [in] */ String value);
void CommandLine::NativeAppendSwitchesAndArguments(
    /* [in] */ ArrayOf<String>* array);

} // namespace Base
} // namespace Webkit
} // namespace Droid
} // namespace Elastos
