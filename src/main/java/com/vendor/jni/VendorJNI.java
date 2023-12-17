package com.vendor.jni;

import java.io.IOException;
import java.util.concurrent.atomic.AtomicBoolean;

import edu.wpi.first.util.RuntimeLoader;

/**
 * Demo class for loading the driver via JNI.
 */
public class VendorJNI {
  static boolean libraryLoaded = false;
  static RuntimeLoader<VendorJNI> loader = null;

  /**
   * Helper class for determining whether or not to load the driver on static initialization.
   */
  public static class Helper {
    private static AtomicBoolean extractOnStaticLoad = new AtomicBoolean(true);

    /**
     * Get whether to load the driver on static init.
     * @return true if the driver will load on static init
     */
    public static boolean getExtractOnStaticLoad() {
      return extractOnStaticLoad.get();
    }

    /**
     * Set whether to load the driver on static init.
     * @param load the new value
     */
    public static void setExtractOnStaticLoad(boolean load) {
      extractOnStaticLoad.set(load);
    }
  }

  static {
    if (Helper.getExtractOnStaticLoad()) {
      try {
        loader = new RuntimeLoader<>("VendorDriver", RuntimeLoader.getDefaultExtractionRoot(), VendorJNI.class);
        loader.loadLibrary();
      } catch (IOException ex) {
        ex.printStackTrace();
        System.exit(1);
      }
      libraryLoaded = true;
    }
  }

  /**
   * Force load the library.
   * @throws java.io.IOException thrown if the native library cannot be found
   */
  public static synchronized void forceLoad() throws IOException {
    if (libraryLoaded) {
      return;
    }
    loader = new RuntimeLoader<>("VendorDriver", RuntimeLoader.getDefaultExtractionRoot(), VendorJNI.class);
    loader.loadLibrary();
    libraryLoaded = true;
  }

  /**
   * Tells the driver to initialize.
   * This is a demo of a native JNI method from the driver.
   * 
   * @return the int returned by the driver
   * @see "VendorJNI.cpp"
   */
  public static native int initialize();
}
