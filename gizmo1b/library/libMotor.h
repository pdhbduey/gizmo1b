#ifndef _LIB_MOTOR_H_
#define _LIB_MOTOR_H_

class LibMotor
{
public:
    enum Direction {
        POSITIVE,
        NEGATIVE,
    };
    enum Microsteps {
        MICROSTEPS_0,
        MICROSTEPS_1,
        MICROSTEPS_2,
        MICROSTEPS_3,
        MICROSTEPS_4,
        MICROSTEPS_5,
        MICROSTEPS_6,
        MICROSTEPS_7,
    };
    enum Status {
        OKAY,
        ERROR_INVALID_DIRECTION,
        ERROR_STEPS_COUNT,
        ERROR_INVALID_MICROSTEPS,
    };
    enum MotorStatus {
        READY,
        BUSY,
        ERROR_TIME_OUT,
    };
    LibMotor();
    virtual ~LibMotor();
    void initialize(); // Setup motor to basic operation (Hold Current, Etc)
    int moveRelative(int direction, int stepsCount); // DIR[0,1], STEPS[1-1,000,000]
    int moveAbs(int position);
    int setMicroSteps(int value); // Set Microsteps 1/2^N for N = [0-7]
    int setSpeed(float speed);
    int setAcceleration(float acceleration);
    int setDeceleration(float deceleration);
    int setHoldCurrent(float holdCurrent);
    int getMotorStatus();
private:
    static bool s_isInitialized;
};

#endif // _LIB_MOTOR_H_
