#include <stdio.h>
#include <string.h>
#include"../headerfile/global_v.h"
#include"../headerfile/myheaderfile.h"
#include <ctype.h>
 
void input(char code_line[500]) {
    // 1. 'x = input(...)' 에서 변수 이름 뽑기
    char var_name[300];
    int name_len = 0;
    int i = 0;

    // 변수 이름 읽기
    while (code_line[i] != '=' && code_line[i] != ' ' && code_line[i] != '\0'
           && name_len < (int)sizeof(var_name) - 1) {
        var_name[name_len++] = code_line[i++];
    }
    var_name[name_len] = '\0';

    // '=' 찾아서 넘어가기
    while (code_line[i] != '=' && code_line[i] != '\0') i++;
    if (code_line[i] == '\0') {
        error(code_line, i, "= 가 없습니다");
        return;
    }
    i++; // '=' 넘기기

    // 공백 건너뛰기
    while (code_line[i] == ' ') i++;

    // 2. input( 확인
    if (strncmp(&code_line[i], "input(", 6) != 0) {
        error(code_line, i, "input() 형식이 아닙니다");
        return;
    }
    i += 6; // "input(" 넘기기

    // 3. 프롬프트 있으면 출력
    if (code_line[i] == '"') {
        i++; // 여는 따옴표 넘기기
        while (code_line[i] != '"' && code_line[i] != '\0') {
            printf("%c", code_line[i++]);
        }
        if (code_line[i] == '"') i++; // 닫는 따옴표 넘기기
    }
    // 프롬프트 없으면 그냥 넘어감 (i는 ')' 위치)

    // 닫는 괄호 ')' 확인
    while (code_line[i] == ' ') i++;
    if (code_line[i] != ')') {
        error(code_line, i, "input() 의 닫는 괄호 ')' 가 없습니다");
        return;
    }

    // 4. 사용자 입력 받기
    char input_buf[300];
    if (fgets(input_buf, sizeof(input_buf), stdin) == NULL) {
        error(code_line, 0, "입력을 읽지 못했습니다");
        return;
    }

    // \n 제거
    input_buf[strcspn(input_buf, "\n")] = '\0';

    // 5. 기존 변수 찾기
    int slot = v_count;
    for (int k = 0; k < v_count; k++) {
        if (strcmp(v_name[k], var_name) == 0) {
            slot = k;
            break;
        }
    }

    // 6. 타입 추론
    //  - 부호(-) 다음에 숫자가 1개 이상 있고, 소수점은 최대 1개,
    //    소수점 앞뒤로 숫자가 있어야 숫자로 인정
    int new_type = 2; // 기본값: string
    {
        int j = 0;
        int digits = 0;   // 전체 숫자 개수
        int dots = 0;     // 소수점 개수
        int after_dot = 0; // 소수점 뒤 숫자 개수
        int valid = 1;

        if (input_buf[j] == '-') j++; // 부호 허용

        for (; input_buf[j] != '\0'; j++) {
            if (isdigit((unsigned char)input_buf[j])) {
                digits++;
                if (dots > 0) after_dot++;
            } else if (input_buf[j] == '.') {
                dots++;
            } else {
                valid = 0;
                break;
            }
        }

        // 숫자가 하나도 없거나, 소수점이 2개 이상이거나,
        // 소수점이 있는데 뒤에 숫자가 없으면 string 으로 처리
        if (valid && digits >= 1 && dots <= 1 && !(dots == 1 && after_dot == 0)) {
            new_type = (dots == 1) ? 1 : 0; // 소수점 있으면 float, 없으면 int
        }
    }

    // 7. 기존 변수면 타입 체크
    if (slot != v_count) {
        if (v_type[slot] != new_type) {
            char* type_names[] = {"int", "float", "string", "bool"};
            char msg[200];
            sprintf(msg, "TypeError: '%s'는 %s 타입인데 %s로 바꿀 수 없습니다",
                var_name, type_names[v_type[slot]], type_names[new_type]);
            error(code_line, 0, msg);
            return;
        }
    }
 
    // 8. 새 변수면 저장 공간이 남아있는지 확인 (배열 크기 100)
    if (slot == v_count && v_count >= 100) {
        error(code_line, 0, "변수 개수가 100개를 초과했습니다");
        return;
    }

    // 9. 값 저장
    strcpy(v_name[slot], var_name);
    v_type[slot] = new_type;

    if (new_type == 0) {
        if (sscanf(input_buf, "%d", &int_v[slot]) != 1) {
            error(code_line, 0, "정수로 변환하지 못했습니다");
            return;
        }
    } else if (new_type == 1) {
        if (sscanf(input_buf, "%lf", &double_v[slot]) != 1) {
            error(code_line, 0, "실수로 변환하지 못했습니다");
            return;
        }
    } else {
        strcpy(char_v[slot], input_buf);
    }

    if (slot == v_count) v_count++;
}