#ifndef global_v_H
#define global_v_H

extern char code_line[110][510]; //최대 100줄까지, 한줄당 최대 500글자 >> 혹시몰라서 여분 10개 만들어 놓음
extern char v_name[100][300]; //변수의 이름 저장, v_name[n]의 이름을 가지고 있는 변수는 int/char/double/bool_v[n]의 값을 가짐 >> 대응시키기
extern int int_v[100], bool_v[100]; /* bool 은 0 또는 1로 저장  */
extern char char_v[100][300];
extern double double_v[100];
extern int v_type[100]; 
extern int v_count; //지금까지 만든 변수 개수
extern char arr[500];
extern char input_text[500];


extern int idx; //print에서 output[idx] 이렇게 쓰임 (index)

#endif