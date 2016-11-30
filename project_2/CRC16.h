#ifndef CRC16_H_
#define CRC16_H_

#include <iostream>

using namespace std;

class CRC16 {

  private:
      int* gx;
      int* data_enc;
      int* state_temp;
      int* state_pre;
      int deg_gx;
  public:
      CRC16();
      void crc_enc(int* data_in, int* data_out, int len);
      void crc_dec(int* dec_in, int* dec_out, int len_dec_in);
      void Malloc(int* gx_in, int deg_gx_in);
      void Free();

};

#endif
