/*
   Main class for the logic of the program
*/

#include "Controller.h"

/*
   Controller constructor
   Expects an Input object for acces to the value of the buttons/ultrasoons/etc..
   Expects a number for selecting a communication methode with the slaves
   Expects the amount of animators (slaves) that it controlles
   And expect and id that is needed for communication over the I2C bus
*/
Controller::Controller(Input* input, int comMethod, int numAnimators, int id)
{
  this->_input = input;
  this->communicationMethod = comMethod;
  this->numberAnimators = numAnimators;
  this->syncPreset = 25000;
  this->id = id;
}

/*
   This function always needs to be called when using this class
   it functions like the setup function in an ino file
*/
void Controller::Begin()
{
  switch (communicationMethod) {
    case 1://Serial
      COM = new UART(BAUD_RATE);
      break;
    case 2://I2C
      COM = new I2C(this->id);
      break;
    default://Default is I2C
      COM = new I2C(this->id);
      break;
  };
  COM->Begin();//Begin communication
  this->Reset();//Reset/init all the vars
}

/*
   Function that holds the logic/flow of the program
*/
void Controller::Logic(void)
{
  //First check if syning has failed (somebody let go)
  if (syncingFailed)
  {
    this->LetGo();
    delay(4000);
    syncingFailed = false;
    this->Reset();
  }
  //Else check if syncing has started
  else if (syncing)
  {
    //Check if somebody let go
    if (this->checkLetGo())
    {
      /*
         The person can reconnect with the tree within a given time
      */
      delay(150);//Built this better with a timer and not sending a pulse to the hand that is connected.
      if (this->checkLetGo())
      {
        this->syncing = false;
        this->syncingFailed = true;
      }
    }
    else
    {
      //Execute sync
      this->syncTime = millis();
      //Check if sync is complete
      if (this->syncTime - this->oldSyncTime >= this->syncPreset)
      {
        //Sync completed
        this->Flash();
        this->Reset();
        delay(12000);//12 seconds
      }
      else
      {
        //Just do a pulse
        this->Pulse();
      }
    }
  }
  //If not syncing look if all inputs are high to start syncing
  else if (_input->getInputHigh(0) == true && _input->getInputHigh(1) == true && _input->getInputHigh(2) == true
           /*&& _input->getInputHigh(3) == true && _input->getInputHigh(4) == true && _input->getInputHigh(5) == true*/)//All active, do sync
  {
    this->syncing = true;
    this->Pulse();//Pulse to update old time values
    this->calculateAdjustments();//Calculate the difference here
    /*
       Built a delay so syncing starts later, first some more pulses
    */
    this->oldSyncTime = millis();
  }
  //Else check if one of the inputs is high
  else if (_input->getInputHigh(0) == true || _input->getInputHigh(1) == true || _input->getInputHigh(2) == true
           || _input->getInputHigh(3) == true || _input->getInputHigh(4) == true || _input->getInputHigh(5) == true)
  {
    this->Pulse();
  }
  else
  {
    /*
       Idle, rest state
       To inplement
    */
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
  if (_input->getInputHigh(0) == true && _input->getInputHigh(1) == true && _input->getInputHigh(2) == true
      /*&& _input->getInputHigh(3) == true && _input->getInputHigh(4) == true && _input->getInputHigh(5) == true*/)//All active, nobody let go
  {
    rv = false;
  }
  return rv;
}

void Controller::Pulse(void)//Two inputs at the moment
{
  this->currentTime = millis();
  long timeDifference = oldTime[0] - oldTime[1];
  if (timeDifference < 20 && timeDifference > -20)//Stop adjusting whenever the difference between pulses is less than 20
  {
    for (int i = 0; i < 6; i ++)
    {
      pulseTime[i] = 2500;
    }
    //Add a final correction to get them perfectly synced.
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

