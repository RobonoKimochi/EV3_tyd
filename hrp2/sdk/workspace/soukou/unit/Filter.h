#ifndef EV3_UNIT_FILTER_H_
#define EV3_UNIT_FILTER_H_

class Filter {
public:
    // static Filter& getInstance(){
    //     static Filter singleton;
    //     return singleton;
    // }
    Filter(){;};
    ~Filter(){;};
    float MovingAverage(float in);     // 移動平均
    float LowPassFilter(float in);     // ローパスフィルタ
private:
    // Filter(){;};
    // ~Filter(){;};
    float old_out = 0;
};

#endif  // EV3_UNIT_FILTER_H_
