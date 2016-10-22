package $javapackage$;

@SuppressWarnings("MethodName")
public class $implreplace$JNI {
  static boolean libraryLoaded = false;
  
  static {
    if (!libraryLoaded) {
      try {
        System.loadLibrary("$driverreplace$");
      } catch (UnsatisfiedLinkError e) {
        e.printStackTrace();
        System.exit(1);
      }
      libraryLoaded = true;
    }
  }
  
  public static native int writeStringToConsole(String toWrite);
}