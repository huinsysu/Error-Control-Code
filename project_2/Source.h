#include "RandNum.cpp"

class Source {

  public:
      Source();
      ~Source();
      void GetBitStr(int* msg_seq, int msg_len);
      void BSC(int* enc_seq, int* rcv_seq, int seq_len, double pro);
      void CountError(int* enc_seq, int* dec_seq, int seq_len, int gx_len);
      int GetTotalError();
      int GetUndetectedError();

  private:
      int total_error;
      int undetected_error;
      CLCRandNum generator;

};

