#include "Source.cc"
#include "CRC16.cc"
#define MSG_LEN 1024
#define GX_DEG 16

int main() {

    int msg_num;
    int msg_len = MSG_LEN;
    int gx_deg = GX_DEG;
    int long_seq_len = MSG_LEN + GX_DEG;
    int msg_seq[MSG_LEN];
    int enc_seq[MSG_LEN + GX_DEG];
    int rcv_seq[MSG_LEN + GX_DEG];
    int dec_seq[MSG_LEN + GX_DEG];

    int gx[GX_DEG + 1] = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1};
    double pro[4] = {0.0001, 0.001, 0.01, 0.1};

    int total_error;
    int total_undetected_error;
    double rate;

    CRC16 myCrc = CRC16();
    myCrc.Malloc(gx, gx_deg);

    for (int i = 0; i < 4; i++) {
        Source mySrc = Source();
        msg_num = 1000000;
        for (int j = 0; j < msg_num; j++) {
            mySrc.GetBitStr(msg_seq, msg_len);
            myCrc.crc_enc(msg_seq, enc_seq, msg_len);
            mySrc.BSC(enc_seq, rcv_seq, long_seq_len, pro[i]);
            myCrc.crc_dec(rcv_seq, dec_seq, long_seq_len);
            mySrc.CountError(enc_seq, dec_seq, long_seq_len, gx_deg);
        }

        total_error = mySrc.GetTotalError();
        total_undetected_error = mySrc.GetUndetectedError();

        cout << "消息总数为：" << msg_num << endl;
        cout << "BSC信道出错概率为：" << pro[i] << "时，错误的消息总数为：";
        cout << total_error << "，没有检测到的错误消息总数为：" << total_undetected_error << endl;
        cout << "没有检测到的错误概率为： ";
        rate = double(total_undetected_error)/total_error;
        cout << rate << endl << endl;
    }

    return 0;

}

