#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <ctype.h>
#include <string.h>
#include <stdbool.h>

#define MAX_WIDTH 4
#define MAX_HEIGHT 4
#define RED_COLOR "\033[0;31m"
#define CYAN_COLOR "\033[0;36m"
#define GREE_COLOR "\033[0;32m"
#define RESET_COLOR "\033[0;37m"

typedef unsigned short int USI;
typedef signed short int SSI;
typedef const unsigned short int CUSI;

enum movement_direction {up='w', right='d', down='s', left='a', exit_='e', help_='h', reset_='r', start_='y', stop_='n'};

size_t points = 0;

size_t ** generate_map(void) {
    size_t ** map = (size_t**)malloc(sizeof(size_t*)*MAX_HEIGHT);
    for (USI y=0; y<MAX_HEIGHT; y++) {
        map[y] = (size_t*)malloc(sizeof(size_t)*MAX_WIDTH);
        for (USI x=0; x<MAX_WIDTH; x++)
            map[y][x] = 0;
    }
    return map;
}

void render_map(size_t ** map, CUSI new_x, CUSI new_y) {
    char charint_[8];
    printf("Points: %lu\n", points);
    printf("+----+----+----+----+\n");
    for (USI y=0; y<MAX_HEIGHT; y++) {
        for (USI x=0; x<MAX_WIDTH; x++) {
            printf("|");
            switch (map[y][x]) {
                case 0:
                    printf("    ");
                    break;
                    
                default:
                    (y == new_y && x == new_x) ? printf(CYAN_COLOR) : printf(RESET_COLOR);
                    sprintf(charint_, "%lu", map[y][x]);
                    printf("%4s", charint_);
                    printf(RESET_COLOR);
                    break;
            }
        }
        printf("|\n+----+----+----+----+\n");
    }
}

void clear_all(size_t ** map) {
    for (USI i=0; i<MAX_HEIGHT; i++)
        free(map[i]);
    free(map);
}

USI get_random_in_range(CUSI start, CUSI end) {
    return (USI)((rand() % (end - start + 1)) + start);
}

size_t ** move_tiles_up(size_t **  map, bool * restrict __changed) {
    for (USI x=0; x<MAX_WIDTH; x++) {
        for (USI y1=0; y1<MAX_HEIGHT; y1++) {
            for (USI y2=y1+1; y2<MAX_HEIGHT; y2++) {
                if (map[y1][x] == map[y2][x] || map[y1][x] == 0) {
                    map[y1][x] += map[y2][x];
                    points += map[y1][x];
                    map[y2][x] = 0;
                    if (map[y1][x] != map[y2][x]) {
                        *__changed = true;
                    }
                } else if (map[y1][x] != map[y2][x] && map[y1][x] != 0 && map[y2][x] != 0) {
                    break;
                }
            }
        }
    }
    return map;
}

size_t ** move_tiles_left(size_t **  map, bool * restrict __changed) {
    for (USI y=0; y<MAX_HEIGHT; y++) {
        for (USI x1=0; x1<MAX_WIDTH; x1++) {
            for (USI x2=x1+1; x2<MAX_WIDTH; x2++) {
                if (map[y][x1] == map[y][x2] || map[y][x1] == 0) {
                    map[y][x1] += map[y][x2];
                    points += map[y][x1];
                    map[y][x2] = 0;
                    if (map[y][x1] != map[y][x2]) {
                        *__changed = true;
                    }
                } else if (map[y][x1] != map[y][x2] && map[y][x1] != 0 && map[y][x2] != 0) {
                    break;
                }
            }
        }
    }
    return map;
}

size_t ** move_tiles_right(size_t **  map, bool * restrict __changed) {
    for (USI y=0; y<MAX_HEIGHT; y++) {
        for (SSI x1=MAX_WIDTH-1; x1>=0; x1--) {
            for (SSI x2=x1-1; x2>=0; x2--) {
                if (map[y][x1] == map[y][x2] || map[y][x1] == 0) {
                    map[y][x1] += map[y][x2];
                    points += map[y][x1];
                    map[y][x2] = 0;
                    if (map[y][x1] != map[y][x2]) {
                        *__changed = true;
                    }
                } else if (map[y][x1] != map[y][x2] && map[y][x1] != 0 && map[y][x2] != 0) {
                    break;
                }
            }
        }
    }
    return map;
}

size_t ** move_tiles_down(size_t ** restrict  map, bool * restrict __changed) {
    for (SSI x=MAX_WIDTH-1; x>=0; x--) {
        for (SSI y1=MAX_HEIGHT-1; y1>=0; y1--) {
            for (SSI y2=y1-1; y2>=0; y2--) {
                if (map[y1][x] == map[y2][x] || map[y1][x] == 0) {
                    map[y1][x] += map[y2][x];
                    points += map[y1][x];
                    map[y2][x] = 0;
                    if (map[y1][x] != 0) {
                        *__changed = true;
                    }
                } else if (map[y1][x] != map[y2][x] && map[y1][x] != 0 && map[y2][x] != 0) {
                    break;
                }
            }
        }
    }
    return map;
}


bool is_stuck(size_t ** restrict map) {
    for (USI y=0; y<MAX_HEIGHT; y++) {
        for (USI x=0; x<MAX_WIDTH; x++) {
            if (map[y][x] == 0) {
                return false;
            } else if (y == 0) {
                if (x == 0) {
                    if (map[y][x] == map[y][x+1] || map[y][x] == map[y+1][x]) {
                        return false;
                    }
                } else if (x == MAX_WIDTH-1) {
                    if (map[y][x] == map[y][x-1] || map[y][x] == map[y+1][x]) {
                        return false;
                    }
                }
            } else if (y == MAX_HEIGHT-1) {
                if (x == 0) {
                    if (map[y][x] == map[y][x+1] || map[y][x] == map[y-1][x]) {
                        return false;
                    }
                } else if (x == MAX_WIDTH-1) {
                    if (map[y][x] == map[y][x-1] || map[y][x] == map[y-1][x]) {
                        return false;
                    }
                }
            } else {
                if ((y > 0 && map[y][x] == map[y-1][x]) || (x < MAX_WIDTH-1 && map[y][x] == map[y][x+1]) || (y < MAX_HEIGHT-1 && map[y][x] == map[y+1][x]) || (x > 0 && map[y][x] == map[y][x-1]))  {
                    return false;
                }
            }
        }
    }
    return true;
}

int main(void) {
    srand((unsigned int)time(NULL));
    size_t ** map = generate_map();
    char operation = 'a';
    USI new_x = get_random_in_range(0, MAX_WIDTH-1);
    USI new_y = get_random_in_range(0, MAX_HEIGHT - 1);

    bool _changed = false;
    bool is_playing = false;

    map[new_y][new_x] = get_random_in_range(1, 2) * 2;
    
    printf(GREE_COLOR);
    printf("Operation:\nw = up\nd = right\ns = down\na = left\ne = exit\nh = help\ny = start game\nn = stop game\n");
    printf(RESET_COLOR);
    
    while (scanf(" %c", &operation) != EOF) {
        switch (operation) {
            case up:
                if (!is_playing) {
                    printf(RED_COLOR);
                    printf("The game hasn't started yet.\n");
                    printf(RESET_COLOR);
                }
                map = move_tiles_up(map, &_changed);
                break;
                
            case right:
                if (!is_playing) {
                    printf(RED_COLOR);
                    printf("The game hasn't started yet.\n");
                    printf(RESET_COLOR);
                }
                map = move_tiles_right(map, &_changed);
                break;
                
            case down:
                if (!is_playing) {
                    printf(RED_COLOR);
                    printf("The game hasn't started yet.\n");
                    printf(RESET_COLOR);
                }
                map = move_tiles_down(map, &_changed);
                break;
                
            case left:
                if (!is_playing) {
                    printf(RED_COLOR);
                    printf("The game hasn't started yet.\n");
                    printf(RESET_COLOR);
                }
                map = move_tiles_left(map, &_changed);
                break;
                
            case exit_:
                clear_all(map);
                return EXIT_SUCCESS;
            
            case help_:
                printf(GREE_COLOR);
                printf("Operation:\nw = up\nd = right\ns = down\na = left\ne = exit\nh = help\ny = start game\nn = stop game\n");
                printf(RESET_COLOR);
                break;
            
            case start_:
                if (!is_playing) {
                    render_map(map, new_x, new_y);
                } else {
                    printf(RED_COLOR);
                    printf("The game is already running.\n");
                    printf(RESET_COLOR);
                    break;
                }
                is_playing = true;
                break;
            
            case stop_:
                if (!is_playing) {
                    printf(RED_COLOR);
                    printf("The game is not running yet.\n");
                    printf(RESET_COLOR);
                }
                break;
                
            default:
                printf("Invalid decision.\n");
                break;
        }
        
        if (_changed) {
            for (USI i=0; i != 1024;) {
                do {
                    new_x = get_random_in_range(0, MAX_WIDTH-1);
                    new_y = get_random_in_range(0, MAX_HEIGHT-1);
                    i++;
                } while (map[new_y][new_x] != 0 && i < 1022);
                
                if (map[new_y][new_x] != 0) {
                    for (USI y=0; y<MAX_HEIGHT; y++) {
                        for (USI x=0; x<MAX_WIDTH; x++) {
                            if (map[y][x] == 0) {
                                new_y = y;
                                new_x = x;
                                y = 10;
                                i = 1024;
                                break;
                            }
                        }
                    }
                } else {
                    i = 1024;
                }
                
                map[new_y][new_x] = get_random_in_range(1, 2) * 2;
                
            }
        }
        
        _changed = false;
        
        render_map(map, new_x, new_y);
        
        if (is_stuck(map)) {
            printf(RED_COLOR);
            printf("Game Over\n");
            printf(RESET_COLOR);
            break;
        }
    }
    
    clear_all(map);
    return EXIT_SUCCESS;
}
