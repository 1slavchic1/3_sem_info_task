#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>
#include <pwd.h>
#include <limits.h>
#include <stdlib.h>


int main(int argc, const char** argv) 			// где argc - количество принимамыемых аргументов, argv - имя файла
{
	DIR* dir;					//создаем переменную типа DIR*, то есть поток данных о директории 
	struct dirent* curr_file_info;			//структурав которую сложим инфу из потока
	struct stat sb;					//сюда сложим готовую статистику файла
	char* full_path = NULL;				//создаем переменную строковую под путь к файлу	- пустую
	struct passwd  *pw_d;
	if(argc < 2)					//если argс меньше 2 то не задан файл, свойства которого нужно описать - выдаем ошибку
	{
		printf("No arguments supplied\n");
		return -1;
	}
	dir = opendir(argv[1]);				//открываем поток данных о элементе argv  
	if(dir == NULL)					//если он не получилось его получить (файл не найден) то выводим ошибку 	
	{
		printf("Error - %d\n", errno);
		return -1;
	}
	full_path = realpath(argv[1], NULL);			//команда получает местахождение файла argv	
	while( ( curr_file_info = readdir(dir) ) != NULL )	//всю информацию из потока данных (dir) о файле складываем в curr_file_info и зацикливаем это пока все не прочтем
	{
		printf("--------------------------------------------\n");		
		printf("File path:  %s/%s\n", full_path, curr_file_info->d_name);	//пропустили строку и выводим путь к файлу + его имя взятое из структуры  curr_file_info
		full_path = realpath(argv[1], NULL);					//опять получает местахождение файла argv
                char* tempp = full_path;						//создали переменную tempp которой присвоили путь(нужно, чтобы получить путь с именем)
                tempp = strcat(tempp, "/");						//добавили слэш к tempp	- в конец строчки
                tempp = strcat(tempp, curr_file_info->d_name);				//потом добавили имя файла, взятое из  curr_file_info
		if (stat(tempp, &sb) == -1) 						//получаем сатистику файла с именем, указанным строкой в tempp и "кладем" её в sb, а если нет то выводим сообщение об ошибке 
		{
        	printf("Statistics couldn't be taken.");
        	return -1;
    		}	
    	printf("File size:                %lld bytes\n", (long long) sb.st_size);		//выводим статистику из sb
    	pw_d = getpwuid ( sb.st_uid ); 
  	printf ( "File owner:               %s \n", pw_d->pw_name); 
    	printf("Mode:                     %lo (octal)\n", (unsigned long) sb.st_mode);
    	printf("Last file access:         %s", ctime(&sb.st_atime));
        printf("Last file modification:   %s", ctime(&sb.st_mtime));
        printf("File create time:         %s", ctime(&sb.st_ctime));
	}
	free(full_path);				//освобождаем переменную full_path	
	closedir(dir);					//закрыть поток
	printf("%s %s\n", argv[0], argv[1]);
	return 0;
}
