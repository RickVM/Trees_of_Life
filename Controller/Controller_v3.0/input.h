#pragma once

#include <Arduino.h>

class Input
{
  public:
    int numInputs;
    int requiredDistance;
    
    boolean highOrLowArray[6];
    int valueArray[6];
    int classificationArray[6];

    virtual ~Input(void) {};
    virtual void Begin() = 0;
    virtual void readInputs() = 0;
    
    int formatByte(uint8_t* values);

    void setRequiredDistance(int x);
    void setInput(int pos, int x);
    boolean getInputHigh(int pos);
    int getInputValue(int pos);
    int getInputClassification(int pos);
};

