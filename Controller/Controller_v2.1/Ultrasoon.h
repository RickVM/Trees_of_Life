#include <Arduino.h>

class Ultrasoon {
  private:
    void PingDistance(void);

  public:
    Ultrasoon(void);
    void Begin(void);
    uint8_t ReadInput(void);
};

