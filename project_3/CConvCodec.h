#ifndef CConvCodec_H_
#define CConvCodec_H_

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cstring>
#include <cmath>

using namespace std;

class CConvCodec {

  public:
      //卷积码维度k，长度n，校验数目n-k
      int m_codedim;
      int m_codelen;
      int m_codechk;
      double m_coderate;

      //卷积码有m_len_input个输入，m_len_output个输出，m_num_register个寄存器
      int m_len_input;
      int m_num_input;
      int m_len_output;
      int m_num_output;
      int m_num_register;

      //Trellis中的状态和边的数目
      int m_num_state;
      int m_num_edge;

      //卷积码网格图参数
      //编码和译码均均基于Trellis
      //Trellis从文件中读入
      int *m_left_vertex;
      int *m_right_vertex;
      int *m_in_label;
      int *m_out_label;
      int m_len_trellis;

      //用于BCJR译码算法的参数
      double **m_alpha;
      double **m_beta;
      double **m_gamma;

      CConvCodec();
      ~CConvCodec();

      void InitCConvCode();
      void InitTrellis();
      void Free();
      void Encoder(int *uu, int *cc);
      void SoftInHardOut(double *in_bit_probs, int *uu_hat);

};

#endif
