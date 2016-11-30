#include "Source.h"
#include <iostream>
using namespace std;

Source::Source() {

    total_frame_number = 0;
    total_bit_number = 0;
    error_frame_number = 0;
    error_bit_number = 0;
    generator = CLCRandNum();
    generator.SetSeed(0);

}

Source::~Source() {
}

void Source::GetBitStr(int* vv, int len_vv) {

    for (int i = 0; i < len_vv; i++) {
        double randNum = generator.Uniform();
        vv[i] = randNum < 0.5 ? 0 : 1;
        total_bit_number++;
    }
    total_frame_number++;

}

void Source::GetNormalSeq(double* nn, int len_nn) {

    generator.Normal(nn, len_nn);

}

void Source::CountError(int* vv, int* vv_rec, int len_vv) {

    int flag = 0;
    for (int i = 0; i < len_vv; i++) {
        if (vv[i] != vv_rec[i]) {
            error_bit_number++;
            flag++;
        }
    }
    if (flag > 0) error_frame_number++;

}

int Source::GetErrorBitNum() {

    return error_bit_number;

}

int Source::GetErrorFrameNum() {

    return error_frame_number;

}

int Source::GetTotalBitNum() {

    return total_bit_number;

}

int Source::GetTotalFrameNum() {

    return total_frame_number;

}

