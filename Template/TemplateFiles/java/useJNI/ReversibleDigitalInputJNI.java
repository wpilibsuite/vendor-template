package $javapackage$;

@SuppressWarnings("MethodName")
public class ReversibleDigitalInputJNI extends $vendorreplace$JNIWrapper {  
  public static native int InitializeReversibleDigitalInput(int channel);
  public static native void FreeReversibleDigitalInput(int handle);
  public static native void SetReversal(int handle, boolean reverse);
  public static native boolean GetReversibleDigitalInput(int handle);
}