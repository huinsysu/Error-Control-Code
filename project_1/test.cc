/*
由pdf可知N0 = 高斯噪声的方差*2;
Nb和Ns均为1
*/

#include "Source.cc"
#define MAX_N0_DEDAOSHU 10
#define MIN_N0_DEDAOSHU 1
#define BIT_NUM 8

int main() {

    int source_str[10];
    int mapped_str[10];
    double normal_str[10];
    double mapped_normal_str[10];
    int result_str[10];

    double err_bit_pro[91];
    double err_frame_pro[91];
    double N0[91];

    int index = 0;
    for (double n0dedaoshu = MIN_N0_DEDAOSHU; n0dedaoshu <= MAX_N0_DEDAOSHU; n0dedaoshu += 0.1) {
        Source src = Source();
        N0[index] = 1 / n0dedaoshu;
        double deviation = sqrt(N0[index] / 2);

        if (n0dedaoshu <= 8) {
            while (src.GetErrorBitNum() <= 10000) {
                src.GetBitStr(source_str, BIT_NUM);
                src.GetNormalSeq(normal_str, BIT_NUM);
                for (int i = 0; i < BIT_NUM; i++) {
                    mapped_str[i] = 2 * source_str[i] - 1;
                    mapped_normal_str[i] = (double)mapped_str[i] + deviation * normal_str[i];
                    result_str[i] = mapped_normal_str[i] > 0 ? 1 : 0;
                }
                src.CountError(source_str, result_str, BIT_NUM);
            }
        } else {
            while (src.GetErrorBitNum() <= 100) {
                src.GetBitStr(source_str, BIT_NUM);
                src.GetNormalSeq(normal_str, BIT_NUM);
                for (int i = 0; i < BIT_NUM; i++) {
                    mapped_str[i] = 2 * source_str[i] - 1;
                    mapped_normal_str[i] = (double)mapped_str[i] + deviation * normal_str[i];
                    result_str[i] = mapped_normal_str[i] > 0 ? 1 : 0;
                }
                src.CountError(source_str, result_str, BIT_NUM);
            }
        }
        err_bit_pro[index] = (double)src.GetErrorBitNum() / (double)src.GetTotalBitNum();
        err_frame_pro[index] = (double)src.GetErrorFrameNum() / (double)src.GetTotalFrameNum();

        index++;
    }

    for (int i = 0; i < 90; i++)
        cout << 1 / N0[i] << " ";
    cout << endl;
    for (int i = 0; i < 90; i++)
        cout  << err_bit_pro[i] << " ";
    cout << endl;
    for (int i = 0; i < 90; i++)
        cout  << err_frame_pro[i] << " ";
    cout << endl;

    return 0;
}

