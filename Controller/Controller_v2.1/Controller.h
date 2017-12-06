#include <Arduino.h>
#include "Communication.h"
#define BAUD_RATE 57600

class Controller {
  public:
    Controller(void);
    void Begin();
    void Logic(uint32_t y);
    void Logic1(uint8_t* y);

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

    long pulseTime[6] = {10000, 10000, 1500, 1500, 1500, 1500};
    long currentTime;
    long oldTime[6] = {0, 0, 0, 0, 0, 0};

    void Pulse(void);
    void Sync(void);
    void LetGo(void);
    void Flash(void);

    void setBooleans(uint32_t y);
    void setBooleans1(uint8_t* y);
    void turnInputsOff(void);
    bool checkLetGo(void);

    boolean syncing;
    boolean syncingFailed;

    long syncTime;
    long oldSyncTime;
    long syncPreset;
    long oldTimeSyncAdjust;

    void calculateAdjustments(void);
    long adjustmentTimes[6];
    long adjustmentSteps[6];
    int countSync[6];
    void Reset(void);

};

