package $javapackage$;

@SuppressWarnings("MethodName")
public class $vendorreplace$JNIWrapper {
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
}