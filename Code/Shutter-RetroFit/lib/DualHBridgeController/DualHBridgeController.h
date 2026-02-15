#ifndef DUAL_H_BRIDGE_CONTROLLER
#define DUAL_H_BRIDGE_CONTROLLER

class DualHBridgeController {
    public:
    DualHBridgeController();
    void ToggleDriveDirection();
    void DriveMotor(int freq, int duty);
    private:
    void SetPWM(int freq, int duty);
};
#endif