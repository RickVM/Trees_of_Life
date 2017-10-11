#include <Arduino.h>
#include "Communication.h"

#define BAUD_RATE 57600

class Controller {
  public:
    Controller(void);
    void Begin();
    void Logic(uint32_t y);

  private:
    //Objects
    Communication* S1;
    Communication* S2;
    Communication* S3;

    boolean x1;
    boolean x2;
    boolean x3;
    boolean x4;
    boolean x5;
    boolean x6;
    
    unsigned long pulseTime[6] = {1500, 1500, 1500, 1500, 1500, 1500};
    unsigned long currentTime;
    unsigned long oldTime[6] = {0, 0, 0, 0, 0, 0};

    void Pulse(void);
    void Sync(void);
    void LetGo(void);
    void Flash(void);

    void setBooleans(uint32_t y);
    void turnInputsOff(void);
    bool checkLetGo(void);

    boolean syncing;
    boolean syncingFailed;

    unsigned long syncTime;
    unsigned long oldSyncTime;
    unsigned long syncPreset;
    unsigned long oldTimeSyncAdjust;
    
};

