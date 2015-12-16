//на вход будем получать текстовый файл с написанными построчно временами паузы и названиями выполняемых программ через пробел 
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <string.h>
#include <stdlib.h>

char NextLine = 10;                         //создаем две глобальные переменные    
int MaxStringSize = 100;

int useless(char *str)
{
    int pause;                                  //создаем переменную под время паузы
    char *argument=malloc(MaxStringSize);       //выделяем память под argument и command - 100 байт и последнему выделяем с это место с указателя p
    char *command=malloc(MaxStringSize),*p;
    pid_t pid;	                                //создали переменную pid типа идентефикатор процесса 
    if((pid=fork())<0)                          //присвоили pidу айди процесса ребенка, а если не удалось создать процесс - ребенок выводим ошибочку
    {
	    printf("%s\n","Can't create child process.");
	    exit(-1);
    }
    if (pid>0) return 0;
    pause=strtol(str,&p,10);                      //присвоили времени паузы значение указанное нами  в десятичной системе счисления,  *p - первую из букв
    sleep(pause);                                 //ждем паузу указанное время
    strcpy(command,p+1);                          //копируем в command имя команды с символа p+1
    int i = strlen(command)-1;                    //присваем i длина комманды - 1 (последний символ в command - символ перевода строки)
    while(command[i] != '/')
    {
        i--;
    }
    char kek[20];                                 //создали переменную кек
    strncpy(kek, command, i);                     //скопировали в кек часть commandа после слеша, то есть имя комманды

    if(execl(command, kek, NULL)<0)               //указываем в экзек command и выполняем из нее кек, если не получается, то выводим ошибку и выходим
    {
        printf("Error - %d", errno);
        printf("%s%s%s\n","Program ",command," failed to load.");
        exit(0);
    }
    exit(-1);
}

int main()
{ 
    int file;                           //создаем для имени файла
    char c,*str,*cur;                   //обозначаем 3 переменные строковые
    ssize_t size;                       //обозначаем переменную сайз типа сайз
    if((file=open("Input.txt",O_RDONLY))<0)       //открыли файл импут текст только для чтения, file присваеваем значение файлового дескриптового импут-текста
    {
    	    printf("%s\n","Can't open file!");
            exit(-1);
    }
    str=malloc(MaxStringSize);              //выделили память под строку
    cur=str;                                //присвоили тот же объем памяти cur 
    size=1;
    while (size!=0)
    {
        size=read(file,&c,1);               //читает 1 байт в из файла в с
	      if((c == NextLine)||(size==0))      //задаем цикл который берет по строчке из файла и используем зелесс к нему
	      { 	    
          *cur=0;
          if(strlen(str)>0)                 
          { 
		      useless(str);
          }
          cur=str;
	      }
	      else
	      {
		      *cur=c;
		      cur++;
	      }
	   }    
    if(close(file)<0)
    {
      printf("%s\n","Can't close file!");
	    exit(-1);
    }
}
