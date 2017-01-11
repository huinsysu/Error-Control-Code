#include "CConvCodec.h"

CConvCodec::CConvCodec() {}

CConvCodec::~CConvCodec() {}

void CConvCodec::InitCConvCode() {

    char buffer[10];
    //ifstream fin(file_name);
    ifstream fin("CCon_1.txt");

    fin.getline(buffer, 10);
    m_codedim = atoi(buffer);
    fin.getline(buffer, 10);
    m_len_input = atoi(buffer);
    fin.getline(buffer, 10);
    m_len_output = atoi(buffer);
    fin.getline(buffer, 10);
    m_num_register = atoi(buffer);
    fin.close();

    m_codelen = m_codedim / m_len_input * m_len_output;
    m_codechk = m_codelen - m_codedim;
    m_coderate = (double)m_codedim / (double)m_codelen;

    m_num_input = pow(2, m_len_input);
    m_num_output = pow(2, m_len_output);

    m_num_state = pow(2, m_num_register);
    m_num_edge = m_num_state * m_num_input;

}

void CConvCodec::InitTrellis() {

    int index = 0;    //表示读入数据的第几行
    char buffer[20];
    char str[10];
    //ifstream fin(file_name);
    ifstream fin("Trellis_1.txt");

    m_left_vertex = new int[m_num_edge];
    m_right_vertex = new int[m_num_edge];
    m_in_label = new int[m_num_edge];
    m_out_label = new int[m_num_edge];
    m_len_trellis = m_codedim / m_len_input;

    while (fin.getline(buffer, 20)) {
        int temp_index = 0;    //表示str数组的下标
        int flag = 0;    //表示要将数字写入的变量是哪个
        for (int i = 0; i <= strlen(buffer); i++) {
            if (buffer[i] == '\0') {
                str[temp_index] = '\0';
                m_out_label[index] = atoi(str);
                index++;
            } else if (buffer[i] == ' ') {
                str[temp_index] = '\0';
                if (flag == 0) {
                    m_left_vertex[index] = atoi(str);
                } else if (flag == 1) {
                    m_right_vertex[index] = atoi(str);
                } else if (flag == 2) {
                    m_in_label[index] = atoi(str);
                }
                flag++;
                temp_index = 0;
            } else {
                str[temp_index++] = buffer[i];
            }
        }
    }
    fin.close();

    m_alpha = new double*[m_len_trellis + 1];
    m_beta = new double*[m_len_trellis + 1];
    m_gamma = new double*[m_len_trellis + 1];

    for (int i = 0; i <= m_len_trellis; i++) {
        m_alpha[i] = new double[m_num_state];
        m_beta[i] = new double[m_num_state];
        m_gamma[i] = new double[m_num_edge];
    }

}

void CConvCodec::Free() {

    delete [] m_left_vertex;
    delete [] m_right_vertex;
    delete [] m_in_label;
    delete [] m_out_label;

    for (int i = 0; i <= m_len_trellis; i++) {
        delete [] m_alpha[i];
        delete [] m_beta[i];
    }
    for (int i = 0; i <= m_len_trellis; i++) {
        delete [] m_gamma[i];
    }
    delete [] m_alpha;
    delete [] m_beta;
    delete [] m_gamma;

}

void CConvCodec::Encoder(int *uu, int *cc) {

    int cur_state = 0;

    for (int i = 0; i < m_len_trellis; i++) {
        int output;
        int input = 0;
        int start_input_index = i * m_len_input;
        int start_output_index = i * m_len_output;

        for (int j = 0; j < m_len_input; j++) {
	    if (uu[start_input_index + j] == 1) {
	        input += pow(2, m_len_input - 1 - j);
	    }
        }

        for (int j = 0; j < m_num_edge; j++) {
            if (m_left_vertex[j] == cur_state && m_in_label[j] == input) {
                output = m_out_label[j];
                cur_state = m_right_vertex[j];
                break;
            }
        }

        for (int j = m_len_output - 1; j >= 0; j--) {
            cc[start_output_index + j] = output % 2;
            output /= 2;
        }
    }

}

void CConvCodec::SoftInHardOut(double *in_bit_probs, int *uu_hat) {

    int **state_used;    //用来记录哪个状态有边到达
    state_used = new int*[m_len_trellis + 1];
    for (int i = 0 ; i <= m_len_trellis; i++) {
        state_used[i] = new int[m_num_state];
        for (int j = 0; j < m_num_state; j++) {
            state_used[i][j] = 0;
        }
    }
    state_used[0][0] = 1;

    for (int i = 0; i <= m_len_trellis; i++) {
        for (int j = 0; j < m_num_state; j++) {
	    m_alpha[i][j] = i == 0 ? 1.0 : 0.0;
	    m_beta[i][j] = i == m_len_trellis ? 1.0 : 0.0;
	}
    }

    for (int i = 0; i <= m_len_trellis; i++) {
        for (int j = 0; j < m_num_edge; j++) {
            m_gamma[i][j] = -1;
        }
    }    

    int output;
    double prob;
    for (int i = 0; i < m_len_trellis; i++) {    //计算Trellis中每一条边gamma的值
        int start_index = i * m_len_output;
        for (int j = 0; j < m_num_state; j++) {
            if (state_used[i][j] == 1) {
                for (int k = 0; k < m_num_input; k++) {
                    for (int m = 0; m < m_num_edge; m++) {
                        if (m_left_vertex[m] == j && m_in_label[m] == k) {
                            output = m_out_label[m];
                            state_used[i + 1][m_right_vertex[m]] = 1;
                            break;
                        }
                    }
                    prob = 1.0;
                    for (int n = m_len_output - 1; n >= 0; n--) {
                        if (output % 2 == 0) {
                            prob *= in_bit_probs[start_index + n];
                        } else {
                            prob *= (1 - in_bit_probs[start_index + n]);
                        }
                        output /= 2;
                    }
                    m_gamma[i + 1][j * m_num_input + k] = prob / m_num_input;
                }
            }
        }
    }
    
    for (int i = 0; i < m_len_trellis; i++) {    //计算每一个状态节点的alpha值
        for (int j = 0; j < m_num_state; j++) {
            if (state_used[i][j] == 1) {
                int next_state;
                for (int k = 0; k < m_num_input; k++) {
                    for (int m = 0 ; m < m_num_edge; m++) {
                        if (m_left_vertex[m] == j && m_in_label[m] == k) {
                            next_state = m_right_vertex[m];
                            break;
                        }
                    }
                    if (i == 0) {
                       m_alpha[i + 1][next_state] += m_gamma[i + 1][j * m_num_input + k];
                    } else {
                       m_alpha[i + 1][next_state] += m_gamma[i + 1][j * m_num_input + k] * m_alpha[i][j];

                    }
                }
            }
        }
	double sum_of_alpha = 0.0;
	for (int j = 0; j < m_num_state; j++) {
	    if (state_used[i + 1][j] == 1) {
	          sum_of_alpha += m_alpha[i + 1][j];
	    }
	}
	for (int j = 0; j < m_num_state; j++) {
	    if (state_used[i + 1][j] == 1) {
	        m_alpha[i + 1][j] /= sum_of_alpha;
	    }
	}
    }
    
    for (int i = m_len_trellis - 1; i >= 0; i--) { //计算每一个状态节点的beta值
        double sum_of_beta = 0.0;
        for (int j = 0; j < m_num_state; j++) {
            if (state_used[i][j] == 1) {
                int next_state;
                for (int k = 0; k < m_num_input; k++) {
                    for (int m = 0; m < m_num_edge; m++) {
                        if (m_left_vertex[m] == j && m_in_label[m] == k) {
                            next_state = m_right_vertex[m];
                            break;
                        }
                    }
                    if (i == m_len_trellis - 1) {
                        m_beta[i][j] += m_gamma[i + 1][j * m_num_input + k];
                    } else {
                        m_beta[i][j] += m_gamma[i + 1][j * m_num_input + k] * m_beta[i + 1][next_state];
                    }
                }
		sum_of_beta += m_beta[i][j];
            }
        }
	for (int j = 0; j < m_num_state; j++) {
	    if (state_used[i][j] == 1) {
	        m_beta[i][j] /= sum_of_beta;
	    }
	}
    }

    double *input_prob = new double[m_num_input];
    for (int i = 0; i < m_len_trellis; i++) {
        int start_index = i * m_len_input;
        for (int j = 0; j < m_num_input; j++) {
            input_prob[j] = 0;
        }

        for (int j = 0; j < m_num_edge; j++) {
            if (state_used[i][m_left_vertex[j]] == 1) {
                input_prob[m_in_label[j]] += m_alpha[i][m_left_vertex[j]] * m_gamma[i + 1][m_left_vertex[j] * m_num_input + m_in_label[j]] * m_beta[i + 1][m_right_vertex[j]];
            }
        }
        
        double max = -1.0;
        int max_input;
        for (int j = 0; j < m_num_input; j++) {
            if (input_prob[j] > max) {
                max = input_prob[j];
                max_input = j;
            }
        }
        for (int j = m_len_input - 1; j >=0; j--) {
            uu_hat[start_index + j] = max_input % 2;
            max_input /= 2;
        }
    }

    for (int i = 0; i <= m_len_trellis; i++) {
        delete [] state_used[i];
    }
    delete [] state_used;

}
