#include "CRC16.h"

CRC16::CRC16() {}

void CRC16::crc_enc(int* data_in, int* data_out, int len) {

    int state_num = deg_gx;
    int dec_len = deg_gx + len;

    for (int i = 0; i < state_num; i++) {
        state_temp[i] = state_pre[i] = 0;
    }

    for (int i = len - 1; i >= 0; i--) {
        int feedback = (data_in[i] + state_pre[state_num - 1]) % 2;

        for (int j = 0 ; j < state_num; j++) {
            if (j == 0) {
                state_temp[j] = feedback;
            } else {
                state_temp[j] = (feedback * gx[j] + state_pre[j - 1]) % 2;
            }
        }
        for (int j = 0; j < state_num; j++) {
            state_pre[j] = state_temp[j];
        }
    }

    for (int i = 0; i < state_num; i++) {
        data_out[i] = state_temp[i];
    }
    for (int i = state_num; i < dec_len; i++) {
        data_out[i] = data_in[i - state_num];
    }

}

void CRC16::crc_dec(int* dec_in, int* dec_out, int len_dec_in) {

    int state_num = deg_gx;

    for (int i = 0; i < state_num; i++) {
        state_temp[i] = state_pre[i] = 0;
    }

    for (int i = len_dec_in - 1; i >= 0; i--) {
        int feedback = state_pre[state_num - 1];
        for (int j = 0; j < state_num; j++) {
            if (j == 0) {
                state_temp[j] = (feedback + dec_in[i]) % 2;
            } else {
                state_temp[j] = (state_pre[j - 1] + feedback * gx[j]) % 2;
            }
        }
        for (int j = 0; j < state_num; j++) {
            state_pre[j] = state_temp[j];
        }
    }

    for (int i = 0; i < len_dec_in; i++) {
        if (i < state_num) {
            dec_out[i] = state_temp[i];
        } else {
            dec_out[i] = dec_in[i];
        }
    }

}

void CRC16::Malloc(int* gx_in, int deg_gx_in) {

    deg_gx = deg_gx_in;
    gx = new int [deg_gx + 1];
    state_temp = new int [deg_gx];
    state_pre = new int [deg_gx];

    for (int i = 0; i <= deg_gx; i++) {
        gx[i] = gx_in[i];
    }

}

void CRC16::Free() {

    delete [] gx;
    delete [] state_temp;
    delete [] state_pre;

}
