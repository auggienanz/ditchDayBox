// countDownTimer.h
// From http://playground.arduino.cc/Main/CountDownTimer

unsigned long Watch, _micro, time = micros();
unsigned int Clock = 0, R_clock;
boolean Reset = false, Stop = false, Paused = false;
volatile boolean timeFlag = false;

bool CountDownTimer();
void ResetTimer();
void StartTimer();
void StopTimer();
void StopTimerAt(unsigned int hours, unsigned int minutes, unsigned int seconds);
void PauseTimer();
void ResumeTimer();
void SetTimer(unsigned int hours, unsigned int minutes, unsigned int seconds);
void SetTimer(unsigned int seconds);
int ShowHours();
int ShowMinutes();
int ShowSeconds();
unsigned long ShowMilliSeconds();
unsigned long ShowMicroSeconds();
boolean TimeHasChanged();
boolean TimeCheck(unsigned int hours, unsigned int minutes, unsigned int seconds);

boolean CountDownTimer()
{
    static unsigned long duration = 1000000; // 1 second
    timeFlag = false;

    if (!Stop && !Paused) // if not Stopped or Paused, run timer
    {
        // check the time difference and see if 1 second has elapsed
        if ((_micro = micros()) - time > duration ) 
        {
            Clock--;
            timeFlag = true;

            if (Clock == 0) // check to see if the clock is 0
                Stop = true; // If so, stop the timer

         // check to see if micros() has rolled over, if not,
         // then increment "time" by duration
            _micro < time ? time = _micro : time += duration; 
        }
    }
    return !Stop; // return the state of the timer
}

void ResetTimer()
{
    SetTimer(R_clock);
    Stop = false;
}

void StartTimer()
{
    Watch = micros(); // get the initial microseconds at the start of the timer
    Stop = false;
    Paused = false;
}

void StopTimer()
{
    Stop = true;
}

void StopTimerAt(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
    if (TimeCheck(hours, minutes, seconds) )
        Stop = true;
}

void PauseTimer()
{
    Paused = true;
}

void ResumeTimer() // You can resume the timer if you ever stop it.
{
    Paused = false;
}

void SetTimer(unsigned int hours, unsigned int minutes, unsigned int seconds)
{
    // This handles invalid time overflow ie 1(H), 0(M), 120(S) -> 1, 2, 0
    unsigned int _S = (seconds / 60), _M = (minutes / 60);
    if(_S) minutes += _S;
    if(_M) hours += _M;

    Clock = (hours * 3600) + (minutes * 60) + (seconds % 60);
    R_clock = Clock;
    Stop = false;
}

void SetTimer(unsigned int seconds)
{
 // StartTimer(seconds / 3600, (seconds / 3600) / 60, seconds % 60);
 Clock = seconds;
 R_clock = Clock;
 Stop = false;
}

int ShowHours()
{
    return Clock / 3600;
}

int ShowMinutes()
{
    return (Clock / 60) % 60;
}

int ShowSeconds()
{
    return Clock % 60;
}

unsigned long ShowMilliSeconds()
{
    return (_micro - time)/ 1000.0;
}

unsigned long ShowMicroSeconds()
{
    return _micro - Watch;
}

boolean TimeHasChanged()
{
    return timeFlag;
}

// output true if timer equals requested time
boolean TimeCheck(unsigned int hours, unsigned int minutes, unsigned int seconds) 
{
    return (hours == ShowHours() && minutes == ShowMinutes() && seconds == ShowSeconds());
}
