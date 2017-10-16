#include "Controller.h"

Controller::Controller(Input* input)
{
  this->_input = input;
  this->syncPreset = 15000;//20 seconds
}

void Controller::Begin()
{
  S1 = new UART(1, BAUD_RATE);
  S2 = new UART(2, BAUD_RATE);
  S3 = new UART(3, BAUD_RATE);
  S1->Begin();
  S2->Begin();
  S3->Begin();
  this->Reset();
}

void Controller::Logic(void)
{
  
  if (syncingFailed)
  {
    this->LetGo();
    delay(4000);
    syncingFailed = false;
    this->Reset();
  }
  else if (syncing)
  {
    //Check if somebody let go
    if (this->checkLetGo())
    {
      this->syncing = false;
      this->syncingFailed = true;
    }
    else
    {
      this->syncTime = millis();
      if (this->syncTime - this->oldSyncTime >= this->syncPreset)
      {
        //Sync completed
        this->Flash();
        this->Reset();
        delay(12000);//12 seconds
      }
      else
      {
        //Put here the code for syncing
        this->Pulse();
      }
    }
  }
  else if (_input->getInputHigh(0) == true && _input->getInputHigh(1) == true /*&& _input->getInputHigh(2) == true
           && _input->getInputHigh(3) == true && _input->getInputHigh(4) == true && _input->getInputHigh(5) == true*/)//All active, do sync
  {
    this->syncing = true;
    //Set pulse time the same
    for (int i = 0; i < 6; i++)
    {
      this->pulseTime[i] = 2500;
    }
    this->oldSyncTime = millis();
    this->Pulse();//Pulse to update old time values
    //Calculate the difference here
    this->calculateAdjustments();

  }
  else if (_input->getInputHigh(0) == true || _input->getInputHigh(1) == true || _input->getInputHigh(2) == true
           || _input->getInputHigh(3) == true || _input->getInputHigh(4) == true || _input->getInputHigh(5) == true)
  {
    this->Pulse();
  }
  else
  {
    //Idle, do nothing
  }
}

void Controller::calculateAdjustments(void)
{
  long a = oldTime[0] + oldTime[1];
  a /= 2;
  adjustmentTimes[0] = a - oldTime[0];
  adjustmentTimes[1] = a - oldTime[1];
  adjustmentSteps[0] = adjustmentTimes[0] / 5;
  adjustmentSteps[1] = adjustmentTimes[1] / 5;
  pulseTime[0] += adjustmentSteps[0];
  pulseTime[1] += adjustmentSteps[1];
}

bool Controller::checkLetGo()
{
  bool rv = true;
  if (_input->getInputHigh(0) == true && _input->getInputHigh(1) == true /*&& _input->getInputHigh(2) == true
      && _input->getInputHigh(3) == true && _input->getInputHigh(4) == true && _input->getInputHigh(5) == true*/)//All active, nobody let go
  {
    rv = false;
  }
  return rv;
}

/*
   Pulse if the boolean is active, keep track of the time between the pulses
*/
void Controller::Pulse(void)//Two inputs at the moment
{
  this->currentTime = millis();
  long timeDifference = oldTime[0] - oldTime[1];
  if (timeDifference < 20 && timeDifference > -20)
  {
    pulseTime[0] = 2500;
    pulseTime[1] = 2500;
    oldTime[0] += timeDifference;
  }
  if (_input->getInputHigh(0))
  {
    if (this->currentTime - this->oldTime[0] > this->pulseTime[0])
    {
      S1->sendCommand("pulse");
      this->oldTime[0] = this->currentTime;
    }
  }
  if (_input->getInputHigh(1))
  {
    if (this->currentTime - this->oldTime[1] > this->pulseTime[1])
    {
      S2->sendCommand("pulse");
      this->oldTime[1] = this->currentTime;
    }
  }
  if (_input->getInputHigh(2))
  {
    if (this->currentTime - this->oldTime[2] > this->pulseTime[2])
    {
      //S3->sendCommand("pulse");
      this->oldTime[2] = this->currentTime;
    }
  }
}

void Controller::LetGo(void)
{
  S1->sendCommand("backward");
  S2->sendCommand("backward");
  //S3->sendCommand("backward");
}

void Controller::Flash(void)
{
  S1->sendCommand("flash");
  S2->sendCommand("flash");
  //S3->sendCommand("flash");
}

void Controller::Reset(void)
{
  for (int i = 0; i < 6; i++)
  {
    this->pulseTime[i] = 2500;
    countSync[i] = 0;
  }
  
  this->syncing = false;
  this->syncingFailed = false;
  this->syncTime = 0;
  this->oldSyncTime = 0;
  this->oldTimeSyncAdjust = 0;
}

