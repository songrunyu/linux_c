/*
 * =====================================================================================
 *
 *       Filename:  ls_getopt.c
 *
 *    Description:  用getopt实现ls 
 *
 *        Version:  1.0
 *        Created:  2013年07月20日 12时03分47秒
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  songrunyu, songrunyu1993@gmail.com
 *        Company:  the software 1203
 *
 * =====================================================================================
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include <dirent.h>
#define SIZE 100
#define MAXROWLEN 130                                              //一行显示的最多字符数

int 	i = 0;
//int 	g_leave_len = MAXROWLEN;                                  //一行剩余长度
//int 	g_maxlen = 0;                                                 //存放最长文件名的长度


void my_err(const char *err_string, int line)                     //错误处理函数，打印出错误所在行的行数和错误信息
{
	fprintf(stderr, "line:%d", line);
	perror(err_string);
	exit(1);
}


int file_attribute(char *path, char *filename, int lflag)
{
   	int 		n;
	int 		len;
    	struct stat 	buf;                //保存文件状态结构体
    	char 		realname[100];      //缓存区
	struct passwd 	*pw;                //用于获得用户的ID，从而获取用户名等信息的结构体
    	struct group 	*gr;                //用于获取组用户的ID，从而获得组的相关信息的结构体
    	struct tm 	*t;                 //用于获取时间信息

    	if(lflag == 0)                      //只打印出文件名
    	{
		if(i >= 5)                  //调整输出格式
		{
			printf("\n");
			i = 0;
		}
		if(strlen(filename) < 26)          
		{ 
			printf("%-26s",filename);
			i++;
		}
		
            	return 0;
    	}

      	if(stat(path, &buf)<0)              //获取文件的信息，stat成功的返回值是0，出错返回-1
	{

		my_err("stat",__LINE__);
        }
        switch(buf.st_mode & S_IFMT)        //判断并打印文件的类型
	{
		case S_IFREG:
			printf("-");
			break;  
		case S_IFDIR: 
			printf("d");
			break;
		case S_IFCHR: 
			printf("c");
			break;
		case S_IFBLK: 
			printf("b");
			break;
		case S_IFIFO: 
			printf("p");
			break;
		case S_IFLNK:
			printf("l");
			break;
		case S_IFSOCK: 
			printf("s");
			break;
	}
                                     
        for(n=8; n>=0; n--)                        //打印文件权限
        {                   
		if(buf.st_mode & (1<<n))         //文件的权限为二进制表示，通过与1左移n位获取权限
		{
			switch(n % 3)
		        {
				case 2:
					printf("r");
			                break;
				case 1:
					printf("w");
			                break;
				case 0:
					printf("x");
			                break;
				default:
			                break;
			     
			}
		}
		else
		{
			printf("-");
		}
	}
	       
	printf("  %d", (int)buf.st_nlink);      //打印硬连接数//

	pw = getpwuid(buf.st_uid);
	printf("  %-9s", pw->pw_name);       //通过getpwuid,根据用户的id获取用户名等信息

	gr = getgrgid(buf.st_gid);       //getgrgid，通过用户的组id获取组用户名等组信息
	printf("  %-9s", gr->gr_name);

	printf("  %ld", buf.st_size);      //打印文件的大小

	t = localtime(&buf.st_atime);      //转化时间日期//
	printf("  %d-%d-%d %d:%d", t->tm_year+1900, t->tm_mon+1, t->tm_mday, t->tm_hour, t->tm_min);

	printf("  %-s ",filename);   

	memset(realname,'\0',SIZE);      //清空缓存区//
	if(S_ISLNK(buf.st_mode))          //判断是否是链接文件//
	{
		printf(" -> ");
		if(readlink(filename,realname,SIZE)==-1)  //通过readlink,获取原文件的文件名存到缓冲区//
		{
			my_err("readlink",__LINE__);
		}
		printf("  %-s",realname);      //打印原文件名//
			
	}
	printf("\n");
        return 0;
}







int list_dir(char *path, int aflag, int lflag)
{

    struct stat       buf;         
    char 	      name[100];
    DIR 	      *dir;        //目录文件的文件描述符
    struct dirent     *pdr;        //目录文件的结构体

    if(stat(path, &buf) < 0)
    {
            my_err("stat",__LINE__);
    } 

    if(S_ISDIR(buf.st_mode))                                         //宏，用来判断是否是一个目录文件，不是则打印文件的信息
    {
/*            dir = opendir(path);                             

            while ((pdr = readdir(dir)) != NULL)                    //获取最长文件名              
            {
		    if(g_maxlen < strlen(pdr->d_name))
		    {
			    g_maxlen = strlen(pdr->d_name);
		    }
	    }
	    closedir(dir);
*/		
	    dir = opendir(path);
	    while ((pdr = readdir(dir)) != NULL)                  
	    {
		    if(aflag == 0)                                   //如果不需要读取隐藏文件
	            {       
			    if(pdr->d_name[0]=='.')                  //当前目录隐藏文件则跳过继续
				  continue;
		            strcpy(name, path);      
		            strcat(name, "/");
		            strcat(name, pdr->d_name);               // 获取目录中单个文件的路径信息
		            file_attribute(name, pdr->d_name, lflag);// 打印文件的信息
		         
		    }
		    else                                             //读取隐藏文件时，打印隐藏文件
		    {
	                    strcpy(name, path);
	                    strcat(name, "/");
	                    strcat(name, pdr->d_name);
	                    file_attribute(name, pdr->d_name, lflag);
	            }
	     }
             closedir(dir);                                           //关闭目录//
        }
    else
    {
            file_attribute(path, path, lflag);                         //不是目录时，打印文件的信息//
    }
    return 0;
}





int main(int argc,char **argv)
{

    	int 	aflag =0;                    //设立标志位，隐藏打印
   	int 	lflag =0;                    //设立标志位，详细打印
    	char 	ch;
    	int 	i;

    	while((ch = getopt(argc, argv, "al")) != -1)  //getopt用于获取命令行的参数 
    	{
		switch(ch)
        	{
			case 'a':
	             		aflag =1;
	             		break;
	         	case 'l':
	             		lflag =1;
	             		break;
	         	default:
	             		break;
	        }
        }

	if(argc == optind )                          //默认打印当前目录
	{
		list_dir("./", aflag, lflag);
        }
	else                                
	{
		for(i=optind; i<argc; i++)           //为目录时，进入list_dir函数，再继续判断是否是目录或文件
                {
			list_dir(argv[i], aflag, lflag);
		}
        }
        
	printf("\n");
        return 0;
}
