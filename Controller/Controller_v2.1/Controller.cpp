#include "Controller.h"

Controller::Controller(void)
{
  this->syncing = false;
  this->syncingFailed = false;
  this->syncTime = 0;
  this->oldSyncTime = 0;
  this->syncPreset = 15000;//20 seconds
  this->oldTimeSyncAdjust = 0;
}

void Controller::Begin()
{
  S1 = new Communication(1, BAUD_RATE);
  S2 = new Communication(2, BAUD_RATE);
  S3 = new Communication(3, BAUD_RATE);
  S1->Begin();
  S2->Begin();
  S3->Begin();
  this->turnInputsOff();
}

void Controller::Logic(uint32_t y)
{
  //Set wich inputs are active as booleans in the memory
  setBooleans(y);
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
      this->turnInputsOff();
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
  else if (x1 == true && x2 == true /*&& x3 == true
           && x4 == true && x5 == true && x6 == true*/)//All active, do sync
  {
    this->syncing = true;
    //Set pulse time the same
    for (int i = 0; i < 6; i++)
    {
      this->pulseTime[i] = 1500;
    }
    this->oldSyncTime = millis();
    this->Pulse();//Pulse to update old time values
    //Calculate the difference here
    this->calculateAdjustments();

  }
  else if (x1 == true || x2 == true || x3 == true
           || x4 == true || x5 == true || x6 == true)
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
  Serial.print("The oldtime values are: ");
  Serial.print(oldTime[0]);
  Serial.print(", ");
  Serial.println(oldTime[1]);
  long a = oldTime[0] + oldTime[1];
  a /= 2;
  Serial.print("Average : ");
  Serial.println(a);
  adjustmentTimes[0] = a - oldTime[0];
  adjustmentTimes[1] = a - oldTime[1];
  Serial.print("The adjustment times are: ");
  Serial.print(adjustmentTimes[0]);
  Serial.print(", ");
  Serial.println(adjustmentTimes[1]);
  adjustmentSteps[0] = adjustmentTimes[0] / 10;
  adjustmentSteps[1] = adjustmentTimes[1] / 10;
  Serial.print("The adjustment steps are: ");
  Serial.print(adjustmentSteps[0]);
  Serial.print(", ");
  Serial.println(adjustmentSteps[1]);
}

void Controller::setBooleans(uint32_t y)
{
  this->turnInputsOff();
  if ((y & (1 << 0)) == 1) {
    this->x1 = true;
  }
  if ((y & (1 << 1)) == 2) {
    this->x2 = true;
  }
  if ((y & (1 << 2)) == 4) {
    this->x3 = true;
  }
  if ((y & (1 << 3)) == 8) {
    this->x4 = true;
  }
  if ((y & (1 << 4)) == 16) {
    this->x5 = true;
  }
  if ((y & (1 << 5)) == 32) {
    this->x6 = true;
  }
}

void Controller::turnInputsOff(void)
{
  this->x1 = false;
  this->x2 = false;
  this->x3 = false;
  this->x4 = false;
  this->x5 = false;
  this->x6 = false;
}

bool Controller::checkLetGo()
{
  bool rv = true;
  if (x1 == true && x2 == true /*&& x3 == true
      && x4 == true && x5 == true && x6 == true*/)//All active, nobody let go
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
  if (x1)
  {
    if (this->currentTime - this->oldTime[0] > this->pulseTime[0])
    {
      S1->sendMessage("pulse");
      this->oldTime[0] = this->currentTime + adjustmentSteps[0];
      if (countTimes[0] == 10)
      {
        adjustmentSteps[0] = 0;
      }
      else
      {
        countTimes[0]++;
      }
    }
  }
  if (x2)
  {
    if (this->currentTime - this->oldTime[1] > this->pulseTime[1])
    {
      S2->sendMessage("pulse");
      this->oldTime[1] = this->currentTime + adjustmentSteps[1];
      if (countTimes[1] == 10)
      {
        adjustmentSteps[1] = 0;
      }
      else
      {
        countTimes[1]++;
      }
    }
  }
  if (x3)
  {
    if (this->currentTime - this->oldTime[2] > this->pulseTime[2])
    {
      //S3->sendMessage("pulse");
      this->oldTime[2] = this->currentTime;
    }
  }
}

void Controller::LetGo(void)
{
  S1->sendMessage("backward");
  S2->sendMessage("backward");
  //S3->sendMessage("backward");
}

void Controller::Flash(void)
{
  S1->sendMessage("flash");
  S2->sendMessage("flash");
  //S3->sendMessage("flash");
}

void Controller::Reset(void)
{
  for (int i = 0; i < 6; i++)
  {
    countTimes[i] = 0;
    this->pulseTime[i] = 1500;

  }
  this->syncing = false;

}

