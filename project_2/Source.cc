#include "Source.h"
#include <iostream>
using namespace std;

Source::Source() {

    total_error = 0;
    undetected_error = 0;
    generator = CLCRandNum();
    generator.SetSeed(0);

}

Source::~Source() {
}

void Source::GetBitStr(int* msg_seq, int msg_len) {

    for (int i = 0; i < msg_len; i++) {
        double randNum = generator.Uniform();
        msg_seq[i] = randNum < 0.5 ? 0 : 1;
    }

}

void Source::BSC(int* enc_seq, int* rcv_seq, int seq_len, double pro) {

    for (int i = 0; i < seq_len; i++) {
        double randNum = generator.Uniform();
        if (randNum < pro) {
            rcv_seq[i] = (enc_seq[i] + 1) % 2;
        } else {
            rcv_seq[i] = enc_seq[i];
        }
    }

}

void Source::CountError(int* enc_seq, int* dec_seq, int seq_len, int gx_deg) {

    for (int i = 0; i < seq_len; i++) {
        if (i < gx_deg) {
            if (dec_seq[i] != 0) {
                total_error++;
                break;
            }
        } else {
            if (enc_seq[i] != dec_seq[i]) {
                total_error++;
                undetected_error++;
                break;
            }
        }
    }

}

int Source::GetTotalError() {

    return total_error;

}

int Source::GetUndetectedError() {

    return undetected_error;

}
