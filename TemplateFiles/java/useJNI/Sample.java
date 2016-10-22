package $javapackage$;

public class Sample {
  public int writeTextToConsole(String text) {
    return $implreplace$JNI.writeStringToConsole(text);
  }
}