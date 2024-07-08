/*
 *  mysystem.c : system 関数のクローン
 */

#include <stdio.h>
#include <stdlib.h>    // exit, system のため
#include <unistd.h>    // fork, execXX のため
#include <sys/wait.h>  // wait のため
#include "mysystem.h"  // インタフェース

// system関数のクローン
int mysystem(char *command) {
  int status = 0;
  int pid;
  if (command == NULL) {                              //NULLポインタを渡された場合
    return 1;                                         //1を返す
  }
  if ((pid = fork()) < 0) {                           //分身を作る
    return -1;                                        //forkできなかった場合は-1を返す
  }
  if (pid != 0) {                                     //親プロセス
    while (wait(&status) != pid)                      //子プロセスの終了を待つ
      ;
  } else {                                            //子プロセス
    execl("/bin/sh", "/bin/sh", "-c", command, NULL); //shを実行
    exit(127);                                        //エラー終了
  }
  return status;
}

/* 実行例
$ make
cc -Wall -std=c99 -o mysysmain mysysmain.c  mysystem.c     <-- コンパイル時エラーなし
$ ls -l                                                    <-- コマンドラインで実行した結果
total 292
-rw-r--r-- 1 hina    143  7  4 09:43 Makefile
-rw-r--r-- 1 hina   2795  7  4 09:43 README.md
-rw-r--r-- 1 hina 238232  7  4 09:43 README.pdf
-rwxr-xr-x 1 hina  33816  7  4 10:08 mysysmain*
-rw-r--r-- 1 hina    925  7  4 09:43 mysysmain.c
-rw-r--r-- 1 hina    879  7  4 10:07 mysystem.c
-rw-r--r-- 1 hina     90  7  4 09:43 mysystem.h
$ ./mysysmain "ls -l"
mysystem:                                                  <-- mysistemの結果
total 584
-rw-r--r--  1 hina  staff     143  7  4 09:43 Makefile
-rw-r--r--  1 hina  staff    2795  7  4 09:43 README.md
-rw-r--r--  1 hina  staff  238232  7  4 09:43 README.pdf
-rwxr-xr-x  1 hina  staff   33816  7  4 10:11 mysysmain
-rw-r--r--  1 hina  staff     925  7  4 09:43 mysysmain.c
-rw-r--r--  1 hina  staff     894  7  4 10:11 mysystem.c
-rw-r--r--  1 hina  staff      90  7  4 09:43 mysystem.h
retval = 00000000
system:                                                    <-- systemの結果
total 584
-rw-r--r--  1 hina  staff     143  7  4 09:43 Makefile
-rw-r--r--  1 hina  staff    2795  7  4 09:43 README.md
-rw-r--r--  1 hina  staff  238232  7  4 09:43 README.pdf
-rwxr-xr-x  1 hina  staff   33816  7  4 10:11 mysysmain
-rw-r--r--  1 hina  staff     925  7  4 09:43 mysysmain.c
-rw-r--r--  1 hina  staff     894  7  4 10:11 mysystem.c
-rw-r--r--  1 hina  staff      90  7  4 09:43 mysystem.h
retval = 00000000
$ ./mysysmain                                              <-- 引数なしの場合      
使い方 : ./mysysmain コマンド文字列                            <-- Usageメッセージを表示
$ ./mysysmain "A"                                          <-- コマンドを起動できない場合  
mysystem:
/bin/sh: A: command not found
retval = 00007f00
system:
sh: A: command not found
retval = 00007f00
$ ./mysysmain "cat A"                                      <-- 起動したコマンドがエラーを起こす場合
mysystem:
cat: A: No such file or directory
retval = 00000100
system:
cat: A: No such file or directory
retval = 00000100
*/
