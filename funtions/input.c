#include <stdio.h>
#include <string.h>
#include"../headerfile/global_v.h"
#include"../headerfile/myheaderfile.h"
 
char* input(char code_line[500]) {
    
    /* 
    1. input(여기엔 값이 잇을수도 잇고 없을수도 잇음) 을 받음
    2. input( 와 그 사이 내용, )  세 가지로 나누기
    3. 괄호 안의 내용 프린트, fgets로 입력받기
    4. 입력 받은 거 리턴
    */
    
    // input 다음 공백이 아닌 부분이 ( 가 아니면 error
    code_line = rms_arr(code_line, 0 , 'a');
    int i = rm_space(code_line, 5);
    
    if(code_line[i] != '('){ //5부터 시작하는 이유는 input에서 마지막인 t가 인덱스 4라서
        error(code_line, i, "괄호가 없습니다.");
        return "오류 발생";
    }else{
        int j, is_string = 0;
        for(j = i+1; code_line[j] != ')' || is_string; j++){
            if(code_line[j] == '\0'){
                error(code_line, j, "괄호가 닫히지 않습니다.");
                return "오류 발생";
            }else if(code_line[j] == '"') is_string = !is_string;
        }
        if(j - i > 1){
            char print_text[500];
            strcpy(print_text, "print");
            strcat(print_text, code_line + i);
            print(print_text);
        }
        fgets(input_text, sizeof(input_text), stdin);
        input_text[strcspn(input_text, "\n")] = '\0';

        return input_text;
    }
}