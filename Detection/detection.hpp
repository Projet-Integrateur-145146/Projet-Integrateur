

class Detection{
public:
    Detection();
    void executeDetectionState();
    void waitFacingDirection();
private:
    uint8_t nbPoles_ = 0;
    uint8_t facingDirection_; // 0 = Top, =1 for each 45 degrees turn [0,7]
};

