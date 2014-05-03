#ifndef __WORM_UTILS__
#define __WORM_UTILS__

// Masks
#define LEFT_EMPTY  0x01
#define DOWN_EMPTY  0x02
#define RIGHT_EMPTY 0x04
#define UP_EMPTY    0x08
#define ALL_EMPTY   0x0F

// Key maping
#define KEYBOARD_UP      'w'
#define KEYBOARD_LEFT    'a'
#define KEYBOARD_DOWN    's'
#define KEYBOARD_RIGHT   'd'
#define KEYBOARD_EXIT    'x'

// Window
#define MIN_WINDOW_WIDTH    18
#define MIN_WINDOW_HEIGHT   5
#define WINDOW_WIDTH        (COLS-1)
#define WINDOW_HEIGHT       (LINES-1)

extern const char* const SERVER_FIFO;
extern const char* const CLIENT_FIFO;

int rnd(int range);
int chatpos(int x, int y);
int emptypos(int x, int y);
int checkpos(int x, int y);

int capture_keyboard(void);
void setup_window(void);
void terminate_window(void);
void update_score(int score);
void update_board(void);

#endif // __WORM_UTILS__
