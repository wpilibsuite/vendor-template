package com.vendor.jni;

import org.junit.jupiter.api.Test;

public class VendorJNITest {
  @Test
  void jniLinkTest() {
    // Test to verify that the JNI test link works correctly.
    VendorJNI.initialize();
  }
}