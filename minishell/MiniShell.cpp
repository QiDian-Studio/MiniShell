#include <iostream>
#include <string>
#include <ctype.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
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
    Parse();
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
      execvp(argv[0], argv);
      exit(0);
    }
    //父进程
    wait(NULL);
  }
}
