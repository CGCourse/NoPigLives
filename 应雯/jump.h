#ifndef _JUMP_H_
#define _JUMP_H_
class jumpTrigger
{
private:
    float barValue;
    bool isReady;
    bool isTrigger = false;
    bool isStart = false;
    bool isFall = false;
    float ysite;
    float ynow;
    float t;
    float g = 50;
    long startTime;
public:

    jumpTrigger(float y)
    {
        barValue = 0;
        isReady = false;
        isTrigger = false;
        isFall = false;
        ysite = y;
        ynow = ysite;
    }

    bool getTrigger()
    {
        return isTrigger;
    }

    bool getFall()
    {
        return isFall;
    }

    void land(float y);
    void fall();
    void drawBar();
    float TimerFunction();
    void key(unsigned char k, int x, int y);
};

#endif

