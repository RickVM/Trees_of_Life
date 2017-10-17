#include <Arduino.h>
#include "UART.h"
#include "input.h"

#define BAUD_RATE 57600

class Controller {
  public:
    Controller(Input* input);
    void Begin();
    void Logic(void);

  private:
    //Objects
    Communication* S1;
    Communication* S2;
    Communication* S3;
    Input* _input;
    
    long pulseTime[6] = {2500, 2500, 2500, 2500, 2500, 2500};
    long currentTime;
    long oldTime[6] = {0, 0, 0, 0, 0, 0};

    void Pulse(void);
    void Sync(void);
    void LetGo(void);
    void Flash(void);

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

