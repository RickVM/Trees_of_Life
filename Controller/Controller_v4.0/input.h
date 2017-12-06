#pragma once

#include <Arduino.h>

class Input
{ 
  private:
    long _oldTime;
  
  public:
    int numInputs;
    int requiredDistance;
    int minimalDistance;
    int inputMethode;
      
    boolean highOrLowArray[6];
    int valueArray[6];
    int classificationArray[6];

    virtual ~Input(void) {};
    virtual void Begin() = 0;
    virtual void readInputs() = 0;
    
    int formatByte(uint8_t* values);

    void setRequiredDistance(int x, int y);
    void setInput(int pos, int x);
    boolean getInputHigh(int pos);
    int getInputValue(int pos);
    int getInputClassification(int pos);
    int getMethode(void);
};

