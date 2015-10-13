// hullプログラムに入力できる形にデータを整形する。
// ./vtk_padata file_name [file_name]
// 入力ファイルはvtk形式
// 出力は.pdata (独自形式)

// .pdataの形式
// x1 y1 z1
// x2 y2 z2
// ...

// 制作 : H27 5J 江澤

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void convert_file(char *file_name){
    FILE *r_fp, *w_fp;
    char s[100];        // 1行200文字以内を仮定
    char f_word[20];    // 1単語20文字以内を仮定
    char wf_name[200];  // 書き込み用ファイル名
    int d_size;         // データ数(必要な行数)
    int ret;            // retはファイル読み込みの戻り値

    // 読み込みファイルを開く
    if ((r_fp = fopen(file_name, "r")) == NULL) {
        printf("file: %s\n", file_name);
        printf("read file open error!!\n");
        exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
    }

    // 書き込み用ファイルを開く
    char *nex_file_name;
    nex_file_name = strtok(file_name, ".");     // ファイル名の拡張子を除いて取り出し
    strcpy(wf_name, nex_file_name);
    sprintf(wf_name, "%s.pdata", wf_name);      // (ファイル名).pdataの形式に直す
    if ((w_fp = fopen(wf_name, "w")) == NULL){
        printf("file: %s\n", wf_name);
        printf("write file open error!!\n");
        exit(EXIT_FAILURE); /* (3)エラーの場合は通常、異常終了する */
    }

    // データの読み込み(行単位)
    while (fgets(s, 256, r_fp)!= NULL) {
        // 行の最初の単語を取得
        sscanf(s, "%s%*c", f_word);

        if (strcmp( f_word, "POINTS") == 0){
            sscanf(s, "%s%d%*c", f_word, &d_size);
            printf("d_size: %d\n", d_size);
            break;
        }
    }

    // データの書き込み
    puts("writing data...");
    while(d_size-- > 0){
        if(fgets(s, 256, r_fp)!= NULL){
            fprintf(w_fp, "%s", s);
        }else{
            // 行の読み込みにミス
            puts("File broken!!!");
        }
    }

    fclose(r_fp);
    fclose(w_fp);
}

int main(int argc, char *argv[]){
    int i;

    if (argc < 2){
        puts("実行形式が違います");
        puts("./vtk_pdata file_name");
        puts("の形式で実行してください");
        exit(EXIT_FAILURE);
    }

    for(i=1; i<argc; i++){
        printf("%s を変換します。", argv[i]);
        convert_file(argv[i]);
    }

    return 0;
}

