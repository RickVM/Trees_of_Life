#include <Arduino.h>

class Ultrasoon {
  private:
    int numInputs;
    uint8_t* trigPins;
    uint8_t* echoPins;
    uint8_t* 
    
    void PingDistance(void);


  public:
    Ultrasoon(uint8_t numInputs, uint8_t* trigPins, uint8_t* echoPins);
    void Begin(void);
    uint8_t ReadInput(void);
};

