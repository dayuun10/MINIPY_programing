#include<stdio.h>
#include<string.h>
#include"../headerfile/global_v.h"
#include"../headerfile/myheaderfile.h"

char get_type(char v[100], int n){ 
    /* 
    자료형 판단 함수, (문자열, 몇 인덱스부터 시작할지)\
    정수: i | 실수: d
    문자: s | 변수: v
    함수: f | True: 1
    오류: e | False: 0
    연산식: o
    리턴
     */

    int count_dot = 0, i = n, is_operation = 0;

    v = rms_arr(v, i, 'a'); //앞뒤 공백 지우기
    i = 0;

    if(v[i] >= '0' && v[i] <= '9' ||  //0 ~ 9거나
    v[i] == '-' && v[i+1] >= '0' && v[i+1] <= '9' ||  // -로 시작하는 숫자거나
    v[i] == '+' && v[i+1] >= '0' && v[i+1] <= '9' ){ // +로 시작하는 숫자
        for(int k = i; v[k] != '\0'; k++){
            if(v[k] == '.'){
                if(count_dot == 0){
                    count_dot++;
                    if(v[k+1] < '0' || v[k+1] > '9') { // 3. >> 오류 
                        error(v, k , " . 뒤엔 숫자가 나와야 합니다. ");
                        return 'e';
                    }
                }else { //점의 개수가 2개 이상 >> 3.. / 3.3.3 >> 오류
                    error(v, k, "잘못된 숫자 형식입니다. 점(.)은 한 번만 사용할 수 있습니다.");
                    return 'e';
                }
            }else if(k > 0 && v[k-1] >= '0' && v[k-1] <= '9' && (v[k] == '+' || v[k] == '-' )|| //부호가 아닌 연산자 +, -
                v[k] == '*' || v[k] == '/' || v[k] == '%' ){ // 연산자를 만나면
                if(v[rm_space(v, k+1)] < '0' || v[rm_space(v, k+1)] > '9'){ //공백을 무시하고 다음 글자가 숫자가 아니면
                    error(v, rm_space(v, k+1), "연산자 뒤엔 숫자가 나와야 합니다. ");
                    return 'e';
                }
                is_operation ++; 
                count_dot = 0;
                    
                
            }else if(v[k] == ' '){
                    //지금위지의 전과 공백을 무시한 후가 자료형이 같으면 오류
                    // 3 3 >> 자료형이 숫자로 같음
                    // + + >> 자료형이 문자로 같음
                    // 3 + >> 자료형이 다름 
                    // 숫자인지 아닌지만 해도 될 것 같아서 이것만 함, ^: XOR 비트 연산자 사용
                    if(!(v[k-1] >= '0' && v[k-1] <= '9') ^ (v[rm_space(v,k+1)] >= '0' && v[rm_space(v,k+1)] <= '9')){ 
                        error(v, k, "이 사이엔 공백을 넣을 수 없습니다. ");
                        return 'e';
                    }
                }
        }
        if(is_operation){//연산식이면
            return 'o';
        }else if(count_dot) return 'd'; //점이 있으면 
        return 'i';
    }


    else if(v[i] == '"'){ //문자열
        for(; v[i] != '\0'; i++){}
        if(v[i-1] != '"'){
            error(v, i, " \" 가 없습니다. ");
            return 'e';
        }else return 's';
    }

    else if(!strncmp(v + i, "True", 4) && v[i+4] == '\0'){ //True
        return '1';
    }else if(!strncmp(v + i, "False", 5) && v[i+5] == '\0'){ //False
        return '0';
    }else if(!strncmp(v + i,"input", 5) && (v[i+5] == ' ' || v[i+5] == '(' )){ //input 또는 input( 로 시작하면 일단 input 함수 호출
        // 어차피 인풋 함수 호출하면 그 안에서 따로 문법검사 할거라 이정도만 해도 상관없음
        return 'f';
    }else{
        for(int j = 0; j < v_count; j++){
            i++;
            if(!strcmp(v_name[j], v)) return 'v'; //변수 이름 목록에 있으면 
        }
        error(v,i, "변수가 선언되지 않았습니다. ");
        return 'e';
    }
} 