#include "RandNum.cpp"

class Source {

  public:
      Source();
      ~Source();
      void GetBitStr(int* vv, int len_vv);
      void GetNormalSeq(double* nn, int len_nn);
      void CountError(int* vv, int* vv_rec, int len_vv);
      int GetErrorBitNum();
      int GetErrorFrameNum();
      int GetTotalBitNum();
      int GetTotalFrameNum();

  private:
      int total_frame_number;
      int total_bit_number;
      int error_frame_number;
      int error_bit_number;

      CLCRandNum generator;

};

