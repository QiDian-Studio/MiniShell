#include <iostream>
#include <string>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <fcntl.h>
//using namespace std;
std::string str;//存储的指令
char* argv[1024];//以数组形式传入参数
int argc = 0;;//参数个数
//界面函数
void DoFace()
{
  std::cout << "[misaki@localhost]$ ";
  //清空缓冲区
  str.clear();
  //以行为结尾读入数据
  std::getline(std::cin, str);
}

//解析指令到argv数组中
void Parse()
{
  argc = 0;
  for(size_t i = 0; i < str.size();)
  {
    //如果是空白字符，替换为'\0'
    if(isspace(str[i]))
    {
      str[i] = '\0';
      i++;
    }
    else 
    {
      //加入到argv中
      argv[argc++] = &str[i];
      //过滤当前指令
      while(!isspace(str[i]) && i < str.size())
      {
        i++;
      }
    }
  }
  argv[argc] = NULL;
}
int main()
{
  while(1)
  {
    //调用界面
    DoFace();
    //创建子进程
    //进行进程替换，execvp
    //解析是否有重定向
    size_t i = 0;//0表示没有重定向，1表示清空重定向，2表示追加重定向
    int isDup = 0;
    std::string file;
    //ls >    test.txt
    while(str[i] != '\0')
    {
      if(str[i] == '>') // >
      {
        isDup = 1;
        str[i] = ' ';
        i++;
        if(str[i] == '>') // >>
        {
          isDup = 2;
          str[i] = ' ';
          i++;
        }
        //将重定向符后面的空格全部略过
        while(str[i] != '\0' && isspace(str[i]))
        {
          i++;
        }
        //获取文件名
        file = str.substr(i);
        std::cout << file << std::endl;
        //将文件名全部替换为空格
        while(str[i] != '\0' && !isspace(str[i]))
        {
          str[i] = ' ';
          i++;
        }
      }
      i++;
    }
    Parse();
    int fd = 1;
    if(isDup == 1)
    {
      fd = open(file.c_str(), O_CREAT | O_TRUNC | O_WRONLY, 0664);
    }
    if(isDup == 2)
    {
      fd = open(file.c_str(), O_CREAT | O_APPEND | O_WRONLY, 0664);
    }
    int pid = fork();
    //创建子进程失败
    if(pid < 0)
    {
      std::cerr << "create process error" << std::endl;
      return -1;
    }
    //子进程
    if(pid == 0)
    {
      dup2(fd, 1);
      execvp(argv[0], argv);
      exit(0);
    }
    //父进程
    wait(NULL);
  }
}
