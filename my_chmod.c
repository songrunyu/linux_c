/*
 * =====================================================================================
 *
 *       Filename:  my_chmod.c
 *
 *    Description:实现chmod 
 *
 *        Version:  1.0
 *        Created:  2013年07月13日 10时37分52秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char **argv)
{
	int 	mode;    //权限
	int 	mode_u;  //所有者权限
	int 	mode_g;  //所属组权限
	int 	mode_o;  //其他用户权限
	char 	*path;   //文件名

	if(argc < 3)             //检查参数合法性
	{
		printf("%s <mode num> <target file>\n",argv[0]);
		exit(0);
	}

	mode = atoi(argv[1]);   //获取命令行参数
	if(mode > 777 || mode < 0)
	{
		printf("mode num wrong!\n");
		exit(0);
	}

	mode_u = mode/100;
	mode_g = (mode - (mode_u)*100)/10;
	mode_o = (mode) - (mode_u*100) - (mode_g*10);
	mode   = (mode_u*8*8)+(mode_g*8)+(mode_o);    //八进制转换
	path   = argv[2];

	if(chmod(path,mode)==-1)
	{
		perror("chmod error");
		exit(1);
	}


	return EXIT_SUCCESS;
}

