#include "Source.cc"
#include "CConvCodec.cc"

#define MAX_N0_DEDAOSHU 4.1
#define MIN_N0_DEDAOSHU 1

int main(int argc, char* argv[]) {

    CConvCodec CConv = CConvCodec();

    int msg_len;
    int code_len;
    int *msg;
    int *msg_enc;
    int *msg_hat;
    double *nor_seq;
    double *msg_nor_d;
    double *bit_prob;
    double deviation;
    double variance;
    double N0[31];
    double err_bit_rate[31];

    CConv.InitCConvCode();
    CConv.InitTrellis();

    msg_len = CConv.m_codedim;
    code_len = CConv.m_codelen;

    msg = new int[msg_len];
    msg_enc = new int[code_len];
    msg_hat = new int[msg_len];
    nor_seq = new double[code_len];
    msg_nor_d = new double[code_len];
    bit_prob = new double[code_len];

    int index = 0;
    for (double n0dedaoshu = MIN_N0_DEDAOSHU; n0dedaoshu <= MAX_N0_DEDAOSHU; n0dedaoshu += 0.1) {
        Source source = Source();
        N0[index] = 1 / n0dedaoshu;
        variance = N0[index] / 2;
        deviation = sqrt(variance);

        while (source.GetErrorBitNum() <= 1000) {
            source.GetBitStr(msg, msg_len);
            CConv.Encoder(msg, msg_enc);    //编码
            source.GetNormalSeq(nor_seq, code_len);
            int projection;
            for (int i = 0 ; i < code_len; i++) {
                projection = msg_enc[i] == 1 ? -1 : 1;
                msg_nor_d[i] = (double)projection + nor_seq[i] * deviation;
                if (msg_nor_d[i] > 0) {
                    bit_prob[i] = 1.0 / (1.0 + exp(-2.0 * msg_nor_d[i] / variance));
                } else {
                    bit_prob[i] = 1.0 - 1.0 / (1.0 + exp(2.0 * msg_nor_d[i] / variance));
                }
            }
            CConv.SoftInHardOut(bit_prob, msg_hat);
            source.CountError(msg, msg_hat, msg_len);
        }
        err_bit_rate[index++] = (double)source.GetErrorBitNum() / (double)source.GetTotalBitNum();
    }

    for (int i = 0; i < 31; i++)
        cout << 1 / N0[i] << " ";
    cout << endl;
    for (int i = 0; i < 31; i++)
        cout  << err_bit_rate[i] << " ";
    cout << endl;


    CConv.Free();
    delete [] msg;
    delete [] msg_enc;
    delete [] msg_hat;
    delete [] nor_seq;
    delete [] msg_nor_d;
    delete [] bit_prob;

    return 0;

}
