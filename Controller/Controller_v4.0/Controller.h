#include <Arduino.h>
#include "UART.h"
#include "I2C.h"
#include "input.h"

#define BAUD_RATE 57600

class Controller {
  public:
    Controller(Input* input, int comMethod, int numAnimators, int id, int numInputs);
    void Begin();
    void Logic(void);

  private:
    //Objects
    Communication* COM;
    Input* _input;

    int communicationMethod;
    int numberAnimators;
    int numberInputs;
    int id;

    long pulseTime[6] = {2500, 2500, 2500, 2500, 2500, 2500};
    long currentTime;
    long oldTime[6] = {0, 0, 0, 0, 0, 0};

    void syncingFailedLogic(void);
    void waitSyncLogic(void);
    void syncLogic(void);
    void syncStop(void);

    void Pulse(void);
    void Sync(void);
    void LetGo(void);
    void Flash(void);

    bool checkLetGo(void);
    bool checkSync(void);

    boolean syncing;
    boolean syncingFailed;

    long syncTime;
    long oldSyncTime;
    long syncPreset;

    void calculateAdjustments(void);
    long adjustmentTimes[6];
    long adjustmentSteps[6];
    long finalAdjustment[6];
    void Reset(void);
    boolean syncWait;
    String M;
    int countLetGo;
};

