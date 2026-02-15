#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <time.h>
#include <ctype.h>  
#include <string.h> 
#include <stdint.h>

#define RED       "\033[38;2;255;0;0m"
#define GRN       "\033[38;2;0;255;0m"
#define YEL       "\033[38;2;255;255;0m"
#define BLU       "\033[38;2;0;0;255m"
#define VERY_DARK_BLUE "\033[38;2;0;0;17m"
#define DARK_RED  "\033[38;2;128;0;0m"
#define MAG       "\033[38;2;255;0;255m"
#define CYN       "\033[38;2;0;255;255m"
#define WHT       "\033[38;2;255;255;255m"
#define GRAY      "\033[38;2;128;128;128m"
#define RESET     "\033[0m"
#define BLK "\033[38;2;0;0;0m"
#define BG_RED "\033[48;2;255;0;0m"
#define BG_YELLOW "\033[43m"
#define BG_GREY "\033[48;2;186;186;186m"

unsigned short int Y_MAX;
unsigned short int X_MAX;



struct field 
{
    bool isbomb;
    bool isshown;
    bool isflaged;
    bool ishighlighted;
    unsigned char bomb_count;
};


void render(struct field **minefield)
{

    printf("\033[2J\033[H\033[0m");// clear the screen
    for(int i=0; i<Y_MAX; i++)
    {
        
        for(int b=0; b<X_MAX; b++)
        {

            printf(" ");
            if(minefield[i][b].ishighlighted)
            {
               printf("\033[48;2;255;255;255m"); 
            }
            if(minefield[i][b].isflaged)
            {
				if(minefield[i][b].ishighlighted)
				{
					printf(BLU "⚑" RESET);	
				}
				else
				{
					printf(BG_RED "⚑" RESET);
				}
                
            }

            else if(minefield[i][b].isshown)
            {
                if(minefield[i][b].isbomb)
                {
                    printf(RED "B" RESET);
                    continue;
                }

                switch(minefield[i][b].bomb_count)
                {
                    case 0:
						printf(GRAY "0" RESET);
                        break;
                    case 1:
                        printf(BLU);
                        printf("1");
                        printf(RESET);
                        break;
                    case 2:
                        printf(GRN);
                        printf("2");
                        printf(RESET);
                        break;
                    case 3:
                        printf(RED);
                        printf("3");
                        printf(RESET);
                        break;
                    case 4:
                        printf(VERY_DARK_BLUE);
                        printf("4");
                        printf(RESET);
                        break;
                    case 5:
                        printf(DARK_RED);
                        printf("5");
                        printf(RESET);
                        break;
                    case 6:
                        printf(CYN);
                        printf("6");
                        printf(RESET);
                        break;
                    case 7:
                
                        printf(BLK);
                        printf("7");
                        printf(RESET);
                        break;
                    case 8:
                        printf(GRAY);
                        printf("8");
                        printf(RESET);
                        break;

                }
                
                    
                    
            } 
            else
            {
                printf("?" RESET);
            }
             

        }
        
        printf( RESET "\n");
    }
}

void randomize(struct field **minefield, int bomb_count)
{
    for(int i=0; i<bomb_count; i++)
    {
        trybombagain:
        int r = rand() % ((X_MAX * Y_MAX)-1);
        int y = r/X_MAX;
        int x = r%X_MAX;
        if(minefield[y][x].isbomb)
        {
            goto trybombagain;
        }
        minefield[y][x].isbomb = 1;
        for(int dy=-1; dy<=1; dy++)
        {
            for(int dx=-1; dx<=1; dx++)
            {
                if(dx==0 && dy==0) continue;
                if(dx+x < 0 || dx+x >= X_MAX || dy+y < 0 || dy+y >= Y_MAX) continue;
                minefield[dy+y][dx+x].bomb_count++;
            }
        }
    }
}

bool checkwin(struct field **minefield)
{
    for(int y = 0; y<Y_MAX; y++)
    {
        for (int x = 0; x<X_MAX; x++)
        {
            if(!minefield[y][x].isbomb && !minefield[y][x].isshown)
            {
                
                return false; // cant be a win
            }
        }
    }
    return true;
}

void dig(struct field **minefield,int x, int y)
{
        if(minefield[y][x].isflaged)
        {
            
        }
        else if(minefield[y][x].isbomb)
        {
            for(int y1 = 0; y1<Y_MAX; y1++)
            {
                for(int x1 = 0; x1<X_MAX; x1++)
                {
                    if(minefield[y1][x1].isbomb)
                    {
                        minefield[y1][x1].isshown = true;
                    }
                }
            }
            render(minefield);
            printf("\nGAME OVER\n");
            exit(1);
        }
        else if(minefield[y][x].bomb_count == 0)
        {
            minefield[y][x].isshown = 1;
            for(int dy=-1; dy<=1; dy++)
            {
                for(int dx=-1; dx<=1; dx++)
                {
                    if(dx==0 && dy==0) continue;
                    if(dx+x < 0 || dx+x >= X_MAX || dy+y < 0 || dy+y >= Y_MAX) continue;
                    if (minefield[dy+y][dx+x].bomb_count == 0)
                    {
                        if ((minefield[dy+y][dx+x].isshown == 0))
                        {
                            
                            dig(minefield,dx+x,dy+y);
                        }
                        
                    }
                    else 
                    {
                        minefield[dy+y][dx+x].isshown = 1;
                    }
                    
                }
            }
        }
        else {
        minefield[y][x].isshown = 1;
        }
}

void print_help() {
    printf("Minesweeper-c\n\n");
    printf("Usage:\n");
    printf("  minesweeper <height> <width> <mines>\n\n");
    printf("Controls (in-game):\n");
    printf("  W / A / S / D  - Move cursor\n");
    printf("  F              - Flag / Deflag\n");
    printf("  Enter          - Reveal cell\n");
}

int main(int argc, char *argv[])
{

    if(argc != 4)
    {
    	print_help();
    	return 1;
    }

    char *endptr;
    uint32_t minecount;

    // Convert Y_MAX
    errno = 0;
    unsigned long val = strtoul(argv[1], &endptr, 10);
    if (errno == ERANGE || val > UINT32_MAX || *endptr != '\0') {
        printf("Invalid Y_MAX: %s\n", argv[1]);
        return 1;
    }
    Y_MAX = (uint32_t)val;

    // Convert X_MAX 
    errno = 0;
    val = strtoul(argv[2], &endptr, 10);
    if (errno == ERANGE || val > UINT32_MAX || *endptr != '\0') {
        printf("Invalid X_MAX: %s\n", argv[2]);
        return 1;
    }
    X_MAX = (uint32_t)val;

    //  Convert minecount 
    errno = 0;
    val = strtoul(argv[3], &endptr, 10);
    if (errno == ERANGE || val > UINT32_MAX || *endptr != '\0') {
        printf("Invalid minecount: %s\n", argv[3]);
        return 1;
    }


    minecount = (uint32_t)val;


	
	if( minecount >= X_MAX * Y_MAX)
	{
		printf("Cannot fit %u mines in the given field", minecount);
		return 1;
	}
    
    struct field **minefield = malloc(Y_MAX*sizeof(struct field*));
    for(uint32_t i=0; i<Y_MAX; i++)
    {
        minefield[i] = calloc(X_MAX, sizeof(struct field)); 

    }


    srand(time(NULL));
    randomize(minefield,minecount);
    
    printf("\033[2J\033[H\033[0m");// clear the screen
    
    int x = 0;
    int y = 0;
    minefield[y][x].ishighlighted = true;
    render(minefield);
    while (1)
    {
        struct termios old, raw;
        tcgetattr(STDIN_FILENO, &old);
        raw = old;
        raw.c_lflag &= ~(ICANON | ECHO);
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
        int c = getchar();   // key pressed
        
        
        switch(c)
        {
            case 'w':
                if(!(y-1 < 0))
                {
                    minefield[y][x].ishighlighted = false;
                    y-=1;
                    minefield[y][x].ishighlighted = true;
                }
                break;
            case 's':
                if(!(y+1 >= Y_MAX))
                {
                    minefield[y][x].ishighlighted = false;
                    y+=1;
                    minefield[y][x].ishighlighted = true;
                }
                break;

            case 'a':
                if(!(x-1 < 0))
                {
                    minefield[y][x].ishighlighted = false;
                    x-=1;
                    minefield[y][x].ishighlighted = true;
                }
                break;
            case 'd':
                if(!(x+1 >= X_MAX))
                {
                    minefield[y][x].ishighlighted = false;
                    x+=1;
                    minefield[y][x].ishighlighted = true;
                }
                break;
            case 10:
                dig(minefield,x,y);
                break;
            case 'f':
                if(!minefield[y][x].isshown)
                {
                    minefield[y][x].isflaged = !minefield[y][x].isflaged;
                }
                break;
            
        }
        
        render(minefield);
        if(checkwin(minefield))
        {
            printf("\nYOU WON !\n");
            break;
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &old);
    }
    return 0;
}
