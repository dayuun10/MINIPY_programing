#include<stdio.h>
#include<string.h>
#include"../headerfile/global_v.h"
#include"../headerfile/myheaderfile.h"


void variable(char code_line[]){
    // 0. 변수 개수 한도(100) 넘으면 더 안 만듦
    if (v_count >= 100)
        return;

    // 1. '=' 가 몇 번째 글자인지 찾기
    int equal_pos = 0; // equal_pos는 =의 위치, 인덱스
    while (code_line[equal_pos] != '=' && code_line[equal_pos] != '\0')
        equal_pos++; // code_line은 전체 코드 길이
    
    if(equal_pos == 0){
        return; //그냥 엔터친거는 무시
    }else if (code_line[equal_pos] == '\0'){
        error(code_line, 0, "존재하지 않는 명령어입니다. print, input, 변수 선언만 가능합니다. ");
        return; // '=' 없으면 대입문 아님 → 무시
    }

    // 2. '=' 왼쪽에서 변수 이름 뽑기
    char var_name[300];
    int name_len = 0;
    int name_ended = 0;   // 이름이 끝났는지 표시 (공백을 한 번이라도 만났는지)

    for(int j = 0; j < equal_pos; j++){
        if(code_line[j] == ' ' || code_line[j] == '='){
            name_ended = 1;          // 공백 만남 → 이름 끝난 걸로 표시  
        }
        else{
            if(name_ended == 1){     // 이름 끝났는데 또 글자가 나옴 → 중간에 공백
                error(code_line, j, "변수 이름에는 띄어쓰기 또는 등호를 쓸 수 없습니다");
                return;              // 더 진행하지 말고 함수 종료
            }
            var_name[name_len] = code_line[j];
            name_len++;
        }
    }
    var_name[name_len] = '\0';

    // 3. '=' 오른쪽으로 가서 공백 건너뛰기
    int value_start = equal_pos + 1;
    while (code_line[value_start] == ' ')
        value_start++;
/* 
a = 3   
a = 2
a = "a"
>> 이렇게 변수 자료형이 바뀌면 에러 뜨게
 */
int slot = v_count;
for (int k = 0; k < v_count; k++)
{
    if (strcmp(v_name[k], var_name) == 0)
    {
        slot = k;
        break;
    }
}
if (slot != v_count) {  // 기존 변수일 때만
    int new_type;
    if (code_line[value_start] == '"') new_type = 2;
    else if (strncmp(&code_line[value_start], "True", 4) == 0 ||
             strncmp(&code_line[value_start], "False", 5) == 0) new_type = 3;
    else if (strchr(&code_line[value_start], '.') != NULL) new_type = 1;
    else new_type = 0;

    if (v_type[slot] != new_type) {
        // 타입 이름 출력용
        char* type_names[] = {"int", "float", "string", "bool"};
        char msg[200];
        sprintf(msg, "%s타입 변수 '%s'은(는) %s로 바꿀 수 없습니다",
            var_name, type_names[v_type[slot]], type_names[new_type]);
        error(code_line, equal_pos, msg);
        return;
    }
}

strcpy(v_name[slot], var_name);

    strcpy(v_name[slot], var_name);

    char type_v = get_type(code_line, value_start);
    if(type_v == 's'){ // 문자열
        int str_len = 0;
        v_type[slot] = 2;
        value_start++;

        //하나씩 입력하기
        while (code_line[value_start] != '"' && code_line[value_start] != '\0' && str_len < 299)
        {
            char_v[slot][str_len] = code_line[value_start];
            str_len++;
            value_start++;
        }
        char_v[slot][str_len] = '\0';
    }else if(type_v == '1'){ //True
        v_type[slot] = 3;
        bool_v[slot] = 1;

    }else if(type_v == '0'){ // False
        v_type[slot] = 3;
        bool_v[slot] = 0;

    }else if(type_v == 'd'){//double
        v_type[slot] = 1;
        sscanf(&code_line[value_start], "%lf", &double_v[slot]);

    }else if(type_v == 'i'){//int
        sscanf(&code_line[value_start], "%d", &int_v[slot]);
    }else if(type_v == 'f'){
        strncpy(code_line, code_line + value_start, strlen(code_line) - value_start);
        int str_len = 0;
        v_type[slot] = 2;
        value_start++;

        //하나씩 입력하기
        while (code_line[value_start] != '"' && code_line[value_start] != '\0' && str_len < 299)
        {
            char_v[slot][str_len] = code_line[value_start];
            str_len++;
            value_start++;
        } 
    }

    if (slot == v_count)
        v_count++; // 새 변수일 때만 개수 +1 (재대입은 그대로)

    /* if (code_line[value_start] == '"')
    { // 문자열
        v_type[slot] = 2;
        value_start++;
        int str_len = 0;
        while (code_line[value_start] != '"' && code_line[value_start] != '\0' && str_len < 299)
        {
            char_v[slot][str_len] = code_line[value_start];
            str_len++;
            value_start++;
        }
        char_v[slot][str_len] = '\0';
    }
    else if (strncmp(&code_line[value_start], "True", 4) == 0 &&
             (code_line[value_start + 4] == '\0' || code_line[value_start + 4] == ' '))
    { // True (뒤에 글자 더 붙은 Truexxx 는 제외)
        v_type[slot] = 3;
        bool_v[slot] = 1;
    }
    else if (strncmp(&code_line[value_start], "False", 5) == 0 &&
             (code_line[value_start + 5] == '\0' || code_line[value_start + 5] == ' '))
    { // False
        v_type[slot] = 3;
        bool_v[slot] = 0;
    }
    else if (strchr(&code_line[value_start], '.') != NULL)
    { // 점 있으면 실수
        v_type[slot] = 1;
        sscanf(&code_line[value_start], "%lf", &double_v[slot]);
    }
    else
    { // 나머지는 정수
        v_type[slot] = 0;
        sscanf(&code_line[value_start], "%d", &int_v[slot]);
    }

    if (slot == v_count)
        v_count++; // 새 변수일 때만 개수 +1 (재대입은 그대로) */
}
