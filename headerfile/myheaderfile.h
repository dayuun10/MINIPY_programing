#define myheader_H

void input_cmd();
void interpret(char input[110][510],int i);
void print(char output[500]);
void error(char error_code[500], int n, char error_message[]);
void variable(char code_line[]);
int rm_space(char input[500], int n);
void rm_space_idx(char arr[500], int n);
int find_char(char str[], int start, char target); //배열에서 start 인덱스부터 시작해서 처음으로 만난 target의 인덱스 번호 출력
char* rms_arr(char code_line[500], int n, char l_r_a);
char get_type(char v[100], int n);
void input(char code_line[500]);