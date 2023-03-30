

class Detection{
public:
    Detection();
    void executeDetectionState();
    void waitFacingDirection();
private:
    uint8_t nbPoles_ = 0;
    uint8_t initialDirection_; // 0 = Front, 1 = Right
};

