#include <stdio.h>
#include <string.h>

void clear (void)
{    
  while ( getchar() != '\n' );
}

void print_membs(){
        char ch = 'f'; 

        printf("Group members: \n");
        printf("Arslan Saeed\n");
        printf("Ishraq Khan\n");
        printf("Raymund Rodriguez\n");
        printf("Siddharth Rajan\n");
        printf("Press enter to exit: \n"); 

        while(ch != '\n'){
            scanf("%c", &ch);
        }
        

        return;
}


int main(){
    char exit[] = "Exit*";
    char command[30];

    scanf("%s", command);
    clear(); 

    if(strncmp(command, exit, 30) == 0){
        print_membs(); 
    }
    
    return 0; 

}