#include<stdio.h>
#include<string.h>
#include"headerfile/global_v.h"
#include"headerfile/myheaderfile.h"

char code_line[110][510]; //최대 100줄까지, 한줄당 최대 500글자 >> 혹시몰라서 여분 10개 만들어 놓음
char v_name[100][300]; //변수의 이름 저장, v_name[n]의 이름을 가지고 있는 변수는 int/char/double/bool_v[n]의 값을 가짐 >> 대응시키기
int int_v[100], bool_v[100]; /* bool 은 0 또는 1로 저장  */
char char_v[100][300];
double double_v[100];
int v_type[100]; 
int v_count = 0; //지금까지 만든 변수 개수
char arr[500];
char input_text[500];
void reset_global();

int idx = 0; //print에서 output[idx] 이렇게 쓰임 (index)

int main(){
    input_cmd();
}

void reset_global(){ //전역변수 초기화
    memset(code_line, 0, sizeof(code_line));

    memset(v_name, 0, sizeof(v_name));
    memset(int_v, 0, sizeof(int_v));
    memset(bool_v, 0, sizeof(bool_v));
    memset(char_v, 0, sizeof(char_v));
    memset(double_v, 0, sizeof(double_v));
    memset(v_type, 0, sizeof(v_type));

    memset(arr, 0, sizeof(arr));
    memset(input_text, 0, sizeof(input_text));

    v_count = 0;
    idx = 0;
}

void input_cmd(){

reset_global();

    printf("\
┌───────────────────────────────┐\n\
\n\
\tMINIPY\n\
\trun_code 입력시 실행\n\
\n\
└───────────────────────────────┘\n\n"
);
    for(int i = 0; i < 110; i++){ //실행하기 전까지 명령어 입력 받기
        if(i > 99) {
            printf("최대 100줄까지 입력 가능합니다. ");
            break;
        }

        printf(">>> ");
        fgets(code_line[i], sizeof(code_line[i]), stdin); //한 줄 전체 입력 받기 위해  
        code_line[i][strcspn(code_line[i], "\n")] = '\0'; // \n 지우기

        if(strcmp(code_line[i], "run_code") == 0){ // run_code가 입력됐을 때 입력 창 나가기
            interpret(code_line, i); 
            break;
        }
    }
    printf("\n============================= PYTHON 실행 종료 =============================\n");
    input_cmd();
}



void interpret(char code_line[110][510],int i){
    printf("\n============================= PYTHON 명령어 실행 =============================\n\n");
    for(int n = 0; n < i; n++){
        //주석처리 무시하기
        if(strchr(code_line[n], '#') != NULL){ //#이 있으면
            int is_string = 0;
            for(int j = 0; code_line[n][j] != '\0'; j++){
                if(code_line[n][j] == '"') is_string = !is_string; //0은 1로 1은 0으로, "#" 은 문자열로, #은 주석처리로 인식하기 위해
                else if(code_line[n][j] == '#' && !is_string) { //주석처리가 있으면
                    while(code_line[n][j] != '\0'){
                        code_line[n][j] = '\0'; //#부터 끝까지 공백으로 만들기
                        j++;
                    }
                }
            }
        }

        if(strncmp(code_line[n], "print", 5) == 0 && (code_line[n][rm_space(code_line[n],5)] == '(')) {
            print(code_line[n]);
            printf("\n");
        } // print ( 로 시작하면 print 함수로 넘기기
        else variable(code_line[n]);

    }
    
}

int rm_space(char code_line[500], int n){ //공백 건너뛰기
    for(; code_line[n] == ' '; n++){}
    return n;
}

char* rms_arr(char code_line[500], int n, char l_r_a){ //공백을 삭제한 배열 리턴 >> rms_arr("  an apple ") >> "an apple" 리턴 (앞뒤 공백만 삭제)
    
    int cnt = 1, l, r ; //cnt: 공백 삭제할 문자열 길이
    memset(arr, 0, sizeof(arr));
    for(; code_line[cnt-1] != '\0'; cnt++){} //공백 삭제할 문자열 길이 구하기
    if(l_r_a == 'l'){//왼쪽 공백만 삭제
        for(l = n; code_line[l] == ' '; l++){}
        strncpy(arr, code_line + l, cnt - l);
        arr[cnt - l] = '\0';
    }else if(l_r_a == 'r'){//오른쪽 공백만 삭제
        for(r = cnt - 2; r >= n && code_line[r] == ' '; r--){}
        strncpy(arr, code_line + n, r - n + 1);
        arr[r - n + 1] = '\0';
    }else{//좌우 공백 모두 삭제
        for(l = n; code_line[l] == ' '; l++){}
        for(r = cnt - 2; r >= l && code_line[r] == ' '; r--){}
        strncpy(arr, code_line + l, r - l + 1);
        arr[r - l + 1] = '\0';
    }

    return arr;
}

void rm_space_idx(char arr[500], int n){ //idx에 공백 건너 뛴 후 인덱스 넣기
    idx = rm_space(arr, n);
}

int find_char(char str[], int start, char target){
    for(int i = start; str[i] != '\0'; i++){
        if(str[i] == target) return i;
    }
    return -1;
}



void error(char error_code[500], int n, char error_message[100]){
    printf("\n에러 발생: %s\n",error_message);
    printf("%s\n",error_code);
    for(int i = 0; i < n; i ++) printf(" ");
    printf("^\n");
}




