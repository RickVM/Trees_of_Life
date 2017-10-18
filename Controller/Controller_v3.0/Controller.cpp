#include "Controller.h"

Controller::Controller(Input* input, int comMethod, int numAnimators, int id)
{
  this->_input = input;
  this->communicationMethod = comMethod;
  this->numberAnimators = numAnimators;
  this->syncPreset = 25000;
  this->id = id;
}

void Controller::Begin()
{
  switch (communicationMethod) {
    case 1://Serial
      COM = new UART(BAUD_RATE);
      break;
    case 2://I2C
      COM = new I2C(this->id);
      break;
    default:
      break;
  };
  COM->Begin();
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
  long average = 0;
  for (int i = 0; i < this->numberAnimators; i++)
  {
    average += oldTime[i];
  }
  average /= this->numberAnimators;
  for (int k = 0; k < this->numberAnimators; k++)
  {
    adjustmentTimes[k] = average - oldTime[k];
    adjustmentSteps[k] = adjustmentTimes[k] / 10;
    pulseTime[k] += adjustmentSteps[k];
  }
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
  for (int i = 0; i < this->numberAnimators; i++)
  {
    if (_input->getInputHigh(i))
    {
      if (this->currentTime - this->oldTime[i] > this->pulseTime[i])
      {
        COM->sendCommand((i + 1), "pulse");
        this->oldTime[i] = this->currentTime;
      }
    }
  }
}

void Controller::LetGo(void)
{
  for (int i = 1; i <= this->numberAnimators; i++)
  {
    COM->sendCommand(i, "backward");
  }
}

void Controller::Flash(void)
{
  for (int i = 1; i <= this->numberAnimators; i++)
  {
    COM->sendCommand(i, "flash");
  }
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
}

