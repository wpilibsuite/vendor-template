package $javapackage$;

import edu.wpi.first.wpilibj.DigitalInput;
import edu.wpi.first.wpilibj.SensorBase;

public class ReversibleDigitalInput extends SensorBase {
  private DigitalInput m_digitalInput;
  private boolean m_reverse = true;
  
  public ReversibleDigitalInput(int channel) {
    m_digitalInput = new DigitalInput(channel);
  }
  
  public void free() {
    m_digitalInput.free();
  }
  
  public void setReverseInput(boolean reverse) {
    m_reverse = reverse;
  }
  
  public boolean get() {
    boolean val = m_digitalInput.get();
    return m_reverse ? !val : val;
  }
}
