#include "RandNum.cpp"

class Source {

  public:
      Source();
      ~Source();
      void GetBitStr(int* vv, int len_vv);
      void GetNormalSeq(double* nn, int len_nn);
      void CountError(int* vv, int* vv_rcv, int len_vv);
      int GetErrorBitNum();
      int GetTotalBitNum();

  private:
      int total_bit_number;
      int error_bit_number;

      CLCRandNum generator;

};

