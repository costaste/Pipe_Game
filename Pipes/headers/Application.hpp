#ifndef SRC_APPLICATION_H_
#define SRC_APPLICATION_H_

#ifdef __linux__

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <SDL2/SDL_ttf.h>

#elif _WIN32

#include <SDL.h> // SDL library.
#include <SDL_mixer.h>
#include <SDL_ttf.h>

#else
#error OS not supported
#endif

#include <string>
#include <sstream>
#include <memory>
#include "../headers/Board.hpp"

enum Delay {
	SLOW = 7,
	MEDIUM = 5,
	FAST = 3
};

class Application {
 private:
    // Window and renderer.
    SDL_Window* window;
    SDL_Renderer* renderer;

    // The image on the window
    SDL_Surface* screen_image;

    // The title image
    SDL_Surface* title_image;

    // The game background skeleton
    SDL_Surface* game_bg;
    // Game background for 'won' state
    SDL_Surface* game_bg_won;

    // Various images used within game:
    SDL_Surface* start_image_blue;
    SDL_Surface* start_image_green;
    SDL_Surface* mystery_tile;
    SDL_Surface* junk_tile;
    SDL_Surface* game_over;
    SDL_Surface* exit_button;
    SDL_Surface* exit_button_green;
    SDL_Surface* game_won;
    SDL_Surface* auto_comp;
    SDL_Surface* auto_comp_green;
    SDL_Surface* flow_button;
    SDL_Surface* flow_button_slow;
    SDL_Surface* flow_button_med;
    SDL_Surface* flow_button_fast;
    SDL_Surface* junk_on;
    SDL_Surface* junk_on_g;
    SDL_Surface* junk_off;
    SDL_Surface* junk_off_g;
    // pipe images
    SDL_Surface* pipe_bot_left;
    SDL_Surface* pipe_bot_left_f;
    SDL_Surface* pipe_bot_right;
    SDL_Surface* pipe_bot_right_f;
    SDL_Surface* pipe_top_left;
    SDL_Surface* pipe_top_left_f;
    SDL_Surface* pipe_top_right;
    SDL_Surface* pipe_top_right_f;
    SDL_Surface* pipe_straight_h;
    SDL_Surface* pipe_straight_h_f;
    SDL_Surface* pipe_straight_v;
    SDL_Surface* pipe_straight_v_f;

    //Timer
    SDL_Surface *seconds;

    // Game Sounds
    Mix_Chunk* bubble1;
    Mix_Chunk* flush;
    Mix_Chunk* wah;
    Mix_Chunk* bubble2;
    Mix_Chunk* click;
    Mix_Chunk* placePipe;

    // Timer for callback function
    SDL_TimerID timerID;

    // Delay for callback function timer (Delay is enum in Application.hpp)
    Delay delay;

    // font for displayed timer and running flag
    TTF_Font * timerFont;
    bool running = false;
    std::stringstream time;
    Uint32 timer = 0;

    // Board Instance
    std::unique_ptr<Board> gameBoard;

    // Game status.
    bool inGame; // True once the player advances from the title screen.
    bool exit;  // True when player wants to exit game.

    bool gameStart;
    bool junk;
    bool gameOver; // True when inGame and the player has lost
    bool gameWon; // True when inGame and the player has won

    bool stateChange; // True when indicating to the main loop something important changed

    // Mouse location
    short m_x;
    short m_y;
    // Over start button (when !inGame)
    bool m_over_start;

    // Over the exit button (when inGame)
    bool m_over_exit;

    // Over the auto complete (when inGame)
    bool m_over_ac;

    // Over flow button (when !inGame)
    bool m_over_flow;

    // Over junk button (when !inGame)
    bool m_over_junk;

    // Which tile mouse is over (when inGame)
    int tile_idx_x;
    int tile_idx_y;

    // Loads necessary files such as images
    bool loadMedia();

    // Helper func to handle logic of which tile img to display
    SDL_Surface* get_tile_img(Tile &t);

    // Helper func to determine which difficulty is selected
    SDL_Surface* get_flow_button_img();

    // set dimensions of a sdl rectangle for displaying things
    void set_rect(SDL_Rect &rect, int w, int h, int x, int y);

    void setUserTimer();

    void input();
    void render();
    void close();

 public:
    static const int SCREEN_WIDTH;
    static const int SCREEN_HEIGHT;

    Application(int argc, char *argv[]);
    ~Application();
    void start();

};

#endif  // SRC_APPLICATION_H_
