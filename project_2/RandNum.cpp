/********************************************************************************

 RandNum.cpp: implementation of the CRandNum class.

********************************************************************************/

/*******************************************************************************
Programmed by Xiao Ma, Sun Yat-sen University. 
If you have any suggestions, please contact me at maxiao@mail.sysu.edu.cn
The program can only be employed for academic research.
******************************************************************************/


//#include "stdafx.h"
#include "RandNum.h"



/********************************************************************
 Construction/Destruction
*********************************************************************/

CRandNum::CRandNum()
{

}

CRandNum::~CRandNum()
{

}



/**********************************************************************************
***********************************************************************************
The following generator employs the linear-congruential method,
and specifically uses a choice of multiplier that was proposed
as a standard by Stephen K. Park et al. in "Technical correspondence,"
Communications of the ACM36(1993), number 7, 108-110
***********************************************************************************
***********************************************************************************/






/*******************************************************************************
 Construction/Destruction
*******************************************************************************/

CLCRandNum::CLCRandNum()
{

}


CLCRandNum::~CLCRandNum()
{

}


/*******************************************************************
定义一些常数
*******************************************************************/
const int CLCRandNum::A = 48271;
const long CLCRandNum::M = 2147483647;
const int CLCRandNum::Q = M / A;
const int CLCRandNum::R = M % A;




/******************************************************************
函数：SetSeed(int flag)
功能：为随机数设置种子
输入参数：flag
          flag = -1 -----默认种子
		  flag = 0  -----随机种子
		  flag = 1  -----自选种子      
******************************************************************/

void CLCRandNum::SetSeed(int flag)
{
	if (flag < 0)
		state = 17;
	else if (flag == 0){
		state = 0;
		while (state == 0){
			srand((unsigned)time(NULL));
			state = rand();
		}
	}
	else {
		fprintf(stdout, "\nEnter the initial state: ");
		fscanf(stdin, "%ld", &state);
	}

	return;
}

/**************************************************************************
函数：PrintState(FILE *fp)
功能：向文件中输出种子值
输入参数：文件指针
**************************************************************************/

void CLCRandNum::PrintState(FILE *fp)
{
	fprintf(fp, "\n***init_state = %ld***\n", state);

	return;
}



/**************************************************************************
函数：Uniform()
功能：产生0-1间均匀分布的随机数
***************************************************************************/

double CLCRandNum::Uniform()
{
	double u;
	
	int tmpState = A * ( state % Q ) - R * ( state / Q );
	if ( tmpState >= 0)
		state = tmpState;
	else
		state = tmpState + M;

	u = state / (double)M;

    return u;
}


/***************************************************************************
函数：Normal(double *nn, int len_nn)
功能：产生长为len_nn的服从均值为0，方差为1的正态分布的随机数
输入参数：
      len_nn  ----- 随机数长度
输出参数
      *nn     ----- 保存产生的随机数
****************************************************************************/

void CLCRandNum::Normal(double *nn, int len_nn)
{
	double x1, x2, w;
	int t;

	for (t = 0; 2*t + 1 < len_nn; t++){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[2*t] = x1 * w;
		nn[2*t+1] = x2 * w;
	}

	if (len_nn % 2 == 1){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[len_nn-1] = x1 * w;
	}

	return;
}




/***************************************************************************
**************************************************************************
The following generator employs the Wichman-Hill algorithm
****************************************************************************
***************************************************************************/



/*******************************************************************************
 Construction/Destruction
*******************************************************************************/

CWHRandNum::CWHRandNum()
{

}

CWHRandNum::~CWHRandNum()
{

}


/******************************************************************
函数：SetSeed(int flag)
功能：为随机数设置种子
输入参数：flag
          flag = -1 -----默认种子
		  flag = 0  -----随机种子
		  flag = 1  -----自选种子      
******************************************************************/

void CWHRandNum::SetSeed(int flag)
{
	if (flag < 0){
		X = 13;
		Y = 37;
		Z = 91;
	}
	else if (flag == 0){
		X = 0;
		Y = 0;
		Z = 0;
		while (X == 0 || Y == 0 || Z == 0){
			srand((unsigned)time(NULL));
			X = rand();
			Y = rand();
			Z = rand();
		}
	}
	else {
		fprintf(stdout, "\nEnter the initial state (X Y Z): ");
		fscanf(stdin, "%d %d %d", &X, &Y, &Z);
	}
	
	return;
}


/**************************************************************************
函数：PrintState(FILE *fp)
功能：向文件中输出种子值
输入参数：文件指针
**************************************************************************/

void CWHRandNum::PrintState(FILE *fp)
{
	fprintf(fp, "\n***init_state (X Y Z) = %d %d %d***\n", X, Y, Z);

	return;
}


/**************************************************************************
函数：Uniform()
功能：产生0-1间均匀分布的随机数
***************************************************************************/

double CWHRandNum::Uniform()
{
	double U;
	
	X =	171 * X % 30269;
	Y = 172 * Y % 30307;
	Z = 170 * Z % 30323;

	U = X / 30269.0 + Y / 30307.0 + Z / 30323.0;
	U = U - int(U);

    return U;
}


/***************************************************************************
函数：Normal(double *nn, int len_nn)
功能：产生长为len_nn的服从均值为0，方差为1的正态分布的随机数
输入参数：
      len_nn  ----- 随机数长度
输出参数
      nn  ----- 保存产生的随机数
****************************************************************************/

void CWHRandNum::Normal(double *nn, int len_nn)
{
	double x1, x2, w;
	int t;

	for (t = 0; 2*t + 1 < len_nn; t++){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[2*t] = x1 * w;
		nn[2*t+1] = x2 * w;
	}

	if (len_nn % 2 == 1){
		w = 2.0;
		while (w > 1.0){
			x1 = 2.0 * Uniform() - 1.0;
			x2 = 2.0 * Uniform() - 1.0;

			w = x1 * x1 + x2 * x2;
		}

		w = sqrt(-2.0 * log(w) / w);
	
		nn[len_nn-1] = x1 * w;
	}

	return;
}
