/*
   Main class for the logic of the program
   Cycle time for a loop with ultrasoon is ~500 millis
   Cycle time with buttons is ~75 millis
*/

#include "Controller.h"

#define SYNC_DELAY 1000
#define PULSE_TIME 2500
#define CYCLES 5

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
  this->syncPreset = 15000;
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
    this->syncingFailedLogic();
  }
  else if (this->syncWait) //Check for syncing
  {
    this->waitSyncLogic();
  }
  //Else check if syncing has started
  else if (syncing)
  {
    this->syncLogic();
  }
  //If not syncing look if all inputs are high to start syncing
  else if (this->checkSync())//All active, do sync
  {
    //Start with the sync delay loop
    this->oldSyncTime = millis();
    this->syncTime = millis();
    Serial.println("Entering sync wait loop");
    this->syncWait = true;
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

/*
   Function that inplements the logic for when syncing fails.
*/
void Controller::syncingFailedLogic(void)
{
  this->LetGo();
  delay(10000);
  syncingFailed = false;
  this->Reset();
}

/*

*/
void Controller::waitSyncLogic(void)
{
  //Check if somebody let go
  if (this->checkLetGo())
  {
    //Count until 5 cycles then, a cycle is 500 millis
    if (this->countLetGo == CYCLES)
    {
      this->syncing = false;
      this->syncingFailed = true;
      this->countLetGo = 0;
    }
    else
    {
      Serial.print("Cycle : ");
      Serial.println(countLetGo);
      this->countLetGo++;
      this->syncPreset += 500;
    }
  }
  else
  {
    this->syncTime = millis();
    this->Pulse();
    if (this->syncTime - this-> oldSyncTime > SYNC_DELAY)
    {

      this->syncing = true;
      this->Pulse();//Pulse to update old time values
      this->calculateAdjustments();//Calculate the difference here
      this->syncTime = millis();
      this->oldSyncTime = millis();
      this->syncWait = false;
      Serial.println("Exit sync wait loop");
    }
  }
}

void Controller::syncLogic(void)
{
  //Check if somebody let go
  if (this->checkLetGo())
  {
    //Count until 5 cycles then
    if (this->countLetGo == CYCLES)
    {
      this->syncing = false;
      this->syncingFailed = true;
      this->countLetGo = 0;
    }
    else
    {
      Serial.print("Cycle : ");
      Serial.println(countLetGo);
      Serial.print("Millis : ");
      Serial.println(millis());
      this->countLetGo++;
      switch (_input->getMethode()) {
        case 1://Buttons adjustime is the 75millis
          this->syncPreset += 75;
          break;
        case 2://Ultrasonic, adjusttime is than 500 millis
          this->syncPreset += 500;
          break;
        default:
          //Not inplemented
          break;
      };
    }
  }
  else
  {
    //Execute sync
    this->countLetGo = 0;
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
      this->Pulse();
    }
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
  bool rv = false;
  int temp = 0;
  for (int i = 0; i < this->numberAnimators; i++)
  {
    temp += _input->getInputHigh(i);
  }
  if (temp < numberAnimators)
  {
    delay(150);
    _input->readInputs();//Read the inputs again
    temp = 0;
    for (int i = 0; i < this->numberAnimators; i++)
    {
      temp += _input->getInputHigh(i);
    }
    if (temp < this->numberAnimators)
    {
      rv = true;
    }
  }
  return rv;
}

bool Controller::checkSync()
{
  bool rv = false;
  int temp = 0;
  for (int i = 0; i < this->numberAnimators; i++)
  {
    temp += _input->getInputHigh(i);
  }
  if (temp == this->numberAnimators)
  {
    delay(25);
    _input->readInputs();
    temp = 0;
    for (int i = 0; i < this->numberAnimators; i++)
    {
      temp += _input->getInputHigh(i);
    }
    if (temp == this->numberAnimators)
    {
      rv = true;
    }
  }
  return rv;
}


void Controller::syncStop(void)
{
  this->currentTime = millis();
 /* for (int i = 1; i < this->numberAnimators; i++)
  {
    finalAdjustment[i] = oldTime[0] - oldTime[i];
    Serial.print("Time difference: ");
    Serial.print(finalAdjustment[i]);
    Serial.print(" of : ");
    Serial.println(i);
  }

   if (this->checkSyncStop())
    {
     for (int i = 0; i < 6; i++)
     {
       pulseTime[i] = PULSE_TIME;
     }
     for (int k = 1; k < 6; k++)
     {
       pulseTime[k] += finalAdjustment[k];
       finalSync[k] = true;
     }
    }
  */
  
  long timeDifference = oldTime[0] - oldTime[1];
  if (timeDifference < 20 && timeDifference > -20 && timeDifference != 0)//Stop adjusting whenever the difference between pulses is less than 20
  {
    for (int i = 0; i < 6; i ++)
    {
      pulseTime[i] = PULSE_TIME;
    }
    //Add a final correction to get them perfectly synced.
  }
}

void Controller::Pulse(void)//Two inputs at the moment
{
  this->syncStop();
  for (int i = 0; i < this->numberAnimators; i++)
  {
    if (_input->getInputHigh(i))
    {
      if (this->currentTime - this->oldTime[i] > this->pulseTime[i])
      {
        switch (_input->getInputClassification(i)) {
          case 1:
            this->M = "pulse1";
            break;
          case 2:
            this->M = "pulse2";
            break;
          case 3:
            this->M = "pulse3";
            break;
        };
        COM->sendCommand((i + 1), M);
        this->oldTime[i] = this->currentTime;
        /*if (finalAdjustment[i] < 20 && finalAdjustment[i] > -20 && finalAdjustment[i] != 0)//Stop adjusting whenever the difference between pulses is less than 20
        {
          for (int i = 0; i < 6; i ++)
          {
            pulseTime[i] = PULSE_TIME;
          }
          //Add a final correction to get them perfectly synced.
        }*/
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
    this->pulseTime[i] = PULSE_TIME;
    this->oldTime[i] = 0;
    this->finalAdjustment[i] = 0;
  }
  this->syncing = false;
  this->syncingFailed = false;
  this->syncTime = 0;
  this->oldSyncTime = 0;
  this->syncWait = false;
  this->M = "";
  this->countLetGo = 0;
}


