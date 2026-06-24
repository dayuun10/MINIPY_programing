#include<stdio.h>
#include<string.h>
#include"../headerfile/global_v.h"
#include"../headerfile/myheaderfile.h"

//print()를 호출하기 전 이미 print 다음에 (가 나오는지 검사함
void print(char output[500]){ //예) print ("hello world") >> output
    char args[50][500], prt_txt[500];
    int i = 0, num_oprt = 0, col = 0, is_str = 0; //num_oprt: 연산자 개수

    for(i = 0;; i++){
        if(output[i] == '('){
            i++;
            break;
        }
    }
    strcpy(prt_txt, rms_arr(output, i, 'a')); // "print" 이후부터 print_text의 좌우 여백 삭제
    
    i = 0;
    for(int j = 0; prt_txt[j] != ')' || is_str; j++, col++){ // , 단위로 나눠서 args에 넣기
        if(prt_txt[j] == ',' && !is_str){ //다음 행으로 
            args[i][col] = '\0';
            i++;
            col = -1; // 반복이 끝나고 1 더해져서 다음 반복 떄 0에 넣으려면 -1로 해야됨
        }else if(prt_txt[j] == '\0'){ // 끝날 때까지 괄호가 안닫히면 오류
            error(output, 5 + j+1, "괄호가 닫히지 않습니다. ");
            return;
        }else{
            args[i][col] = prt_txt[j];
        }

        if(prt_txt[j] == '"'){
            is_str = !is_str;
        }
    }
    args[i][col] = '\0';

    for(int k = 0; k < i + 1; k++){ 
        strcpy(args[k], rms_arr(args[k], 0, 'a')); //좌우 여백 지우기
    }
    for(int k = 0; k < i + 1; k++){ //agrs 하나씩 읽기
        
        char a_type = get_type(args[k], 0);
        if(a_type == 'i' || a_type == 'd' || a_type == '1' || a_type == '0' ){
            printf("%s ", args[k]);
        }else if(a_type == 's'){
            args[k][strlen(args[k]) - 1] = '\0'; //마지막 따움표 제거
            printf("%s ", args[k] + 1); // 첫번째 따움표 건너뛰고 출력
        }else if(a_type == 'o'){ //연산
            num_oprt = 0;

            for(int l = 0; args[k][l] != '\0'; l++){ //연산자 개수 세기
                char temp = args[k][l];
                if(l > 0 && args[k][l-1] >= '0' && args[k][l-1] <= '9' && (args[k][l] == '+' || args[k][l] == '-' )|| //부호가 아닌 연산자 +, -
                args[k][l] == '*' || args[k][l] == '/' || args[k][l] == '%' ){ 
                    num_oprt ++;
                }
            }
            double a, b; 
            char oprt;
            int pos = 0, read_len = 0; //pos: 누적 읽은 문자 개수, read_len: 지금 읽은 문자 개수


            sscanf(args[k],"%lf%n", &a, &pos);
            for(int l = 0; l < num_oprt; l++){

                sscanf(args[k]+pos, "%c%lf%n", &oprt, &b, &read_len);
                pos += read_len;

                if(oprt == '+') a += b;
                else if(oprt == '-') a-= b;
                else if(oprt == '*') a *= b;
                else if(oprt == '/') a /= b;
                else if(oprt == '%'){
                    if((int)a == a && (int)b == b) a = (int)a % (int)b;
                    else {
                        error(output, strstr(output, args[k]) - output, "실수형 자료형에선 % 연산자를 사용할 수 없습니다. ");
                        return;
                    }
                }
            }
            printf("%g ",a);
        }else if(a_type == 'v'){
            int l;
            for(l = 0; strcmp(v_name[l], args[k]); l++){ //이름 위치 찾기
            }
            if(v_type[l] == 0) printf("%d ",int_v[l]);
            else if(v_type[l] == 1) printf("%g ",double_v[l]);
            else if(v_type[l] == 2) printf("%s ",char_v[l]);
            else if(v_type[l] == 3) printf("%s ",(bool_v[l] == 1)? "True": "False");
        }else if(a_type == 'e') return;
    }
    
}