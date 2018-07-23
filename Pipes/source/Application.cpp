#include "../headers/Application.hpp"

#include <iostream>
#include <cstdio>
#include <string>
Uint32 onTick(Uint32 interval, void *param);

// Screen resolution.
const int Application::SCREEN_WIDTH = 800;
const int Application::SCREEN_HEIGHT = 600;

Application::Application(int argc, char *argv[]) {
    SDL_Init(SDL_INIT_EVERYTHING);
    TTF_Init();
    SDL_ShowCursor(SDL_ENABLE);

    // Create window and renderer.
    window = SDL_CreateWindow("Pipes",
            SDL_WINDOWPOS_UNDEFINED,
            SDL_WINDOWPOS_UNDEFINED,
            SCREEN_WIDTH,
            SCREEN_HEIGHT,
            SDL_WINDOW_SHOWN);

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED |
            SDL_RENDERER_PRESENTVSYNC);

    screen_image = SDL_GetWindowSurface(window);

    // Init board. (We still need to feed it a solution. Possibly decide based on args, as a difficulty setting?)
    std::shared_ptr<Board> gameBoard(new Board());

    delay = MEDIUM;

    // Game status.
    inGame = false;
    gameOver = false;
    gameWon = false;
    exit = false;
    junk = false;

    stateChange = false;

    m_x = 0;
    m_y = 0;
    m_over_start = false;
    m_over_exit = false;
    m_over_ac = false;
    m_over_flow = false;
    m_over_junk = false;

    tile_idx_x = -1;
    tile_idx_y = -1;
}

Application::~Application() {
    Mix_FreeChunk(bubble1);

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    Mix_Quit();
    SDL_Quit();
}

void Application::start() {
    if ( !loadMedia() ) {
        printf("Failed to load necessary files");
    }
    else {

        // The rectangle of the Start button
        // 200 x 60: xPos should be 300, yPos should be 350(ish?)
        SDL_Rect start_button_rect = SDL_Rect();
        set_rect(start_button_rect, 200, 60, 300, 350);

        // Apply the title screen image
        SDL_BlitSurface(title_image, NULL, screen_image, NULL);
        // Draw a "Start" Button
        SDL_BlitSurface(start_image_blue, NULL, screen_image, &start_button_rect);

        SDL_Rect flow_button_rect = SDL_Rect();
        set_rect(flow_button_rect, 200, 60, 150, 430);
        // Draw button to change flow speed (initially medium)
        SDL_BlitSurface(flow_button_med, NULL, screen_image, &flow_button_rect);

        SDL_Rect junk_button_rect = SDL_Rect();
        set_rect(junk_button_rect, 200, 60, 450, 430);
        // Draw button to toggle junk tiles (initially off)
        SDL_BlitSurface(junk_off, NULL, screen_image, &junk_button_rect);


        while (!exit) {
            input();
            SDL_Delay(10);
            if (stateChange && inGame) {
                if (gameStart) {
                    // Connect tick function to timer so it's called periodically
                    Uint32 (*onTick_ptr)(Uint32, void*);
                    onTick_ptr = onTick;

                    timerID = SDL_AddTimer(delay*1000, onTick_ptr, NULL);
                    gameStart = false;
                    gameOver = false;
                    gameWon = false;

                    timer = SDL_GetTicks();
                    running = true;
                }
                if (gameBoard->won()) {
                    SDL_BlitSurface(game_bg_won, NULL, screen_image, NULL);
                }
                else {
                    SDL_BlitSurface(game_bg, NULL, screen_image, NULL);
                }
                m_over_start = false;


                // tile selection
                if (tile_idx_y >= 0 && tile_idx_y < BOARD_SIZE
                    && tile_idx_x >= 0 && tile_idx_x < BOARD_SIZE) {
                        SDL_Rect tile_rect = SDL_Rect();
                        set_rect(tile_rect, 60, 60, tile_idx_x * 60 + 200, tile_idx_y * 60);

                        if (!gameBoard->tiles[tile_idx_x][tile_idx_y]->isFlipped()) {
                            SDL_BlitSurface(mystery_tile, NULL, screen_image, &tile_rect);
                        }
                        else {
                            SDL_BlitSurface(get_tile_img(*gameBoard->tiles[tile_idx_x][tile_idx_y]),
                                            NULL,
                                            screen_image,
                                            &tile_rect);
                        }
                }
                if (m_over_ac) {
                    SDL_Rect ac_rect = SDL_Rect();
                    set_rect(ac_rect, 200, 60, 0, 540);
                    SDL_BlitSurface(auto_comp_green, NULL, screen_image, &ac_rect);
                }
                else {
                    SDL_Rect ac_rect = SDL_Rect();
                    set_rect(ac_rect, 200, 60, 0, 540);
                    SDL_BlitSurface(auto_comp, NULL, screen_image, &ac_rect);
                }
            }
            if (inGame) {
                // Blit flipped tiles
                for (int i = 0; i < BOARD_SIZE; ++i) {
                    for (int j = 0; j < BOARD_SIZE; ++j) {
                        if (gameBoard->tiles[i][j]->isFlipped()) {
                            SDL_Rect tile_rect = SDL_Rect();
                            set_rect(tile_rect, 60, 60, i*60+200, j*60);
                            SDL_BlitSurface(get_tile_img(*gameBoard->tiles[i][j]),
                                            NULL,
                                            screen_image,
                                            &tile_rect);
                        }
                    }
                }
                // Blit pocket tile
                SDL_Rect tile_rect = SDL_Rect();
                set_rect(tile_rect, 60, 60, 70, 400);
                SDL_BlitSurface(get_tile_img(*gameBoard->pocket),
                                NULL,
                                screen_image,
                                &tile_rect);
                setUserTimer();
            }
            if (inGame) {
                if (gameBoard->loss()) {
                    if (!gameOver) {
                        gameOver = true;
                        if (!gameBoard->won()) {
                                Mix_PlayChannel(-1, wah, 0);
                        }
                    }
                    SDL_Rect game_over_rect = SDL_Rect();
                    set_rect(game_over_rect, 350, 120, 325, 150);

                    SDL_BlitSurface(game_over, NULL, screen_image, &game_over_rect);

                    SDL_Rect exit_rect = SDL_Rect();
                    set_rect(exit_rect, 200, 60, 400, 400);
                    if (m_over_exit) {
                        SDL_BlitSurface(exit_button_green, NULL, screen_image, &exit_rect);
                    }
                    else {
                    SDL_BlitSurface(exit_button, NULL, screen_image, &exit_rect);
                    }
                    // Remove timer so onTick won't continue
                    SDL_RemoveTimer(timerID);
                    running = false;

                }
                if (inGame && gameBoard->won()) {
                    if (!gameWon) {
                        gameWon = true;
                        Mix_PlayChannel(-1, flush, 0);
                    }
                    SDL_Rect game_won_rect = SDL_Rect();
                    set_rect(game_won_rect, 350, 120, 325, 150);

                    SDL_BlitSurface(game_won, NULL, screen_image, &game_won_rect);

                    SDL_Rect exit_rect = SDL_Rect();
                    set_rect(exit_rect, 200, 60, 400, 400);
                    if (m_over_exit) {
                        SDL_BlitSurface(exit_button_green, NULL, screen_image, &exit_rect);
                    }
                    else {
                        SDL_BlitSurface(exit_button, NULL, screen_image, &exit_rect);
                    }
                    SDL_RemoveTimer(timerID);
                    running = false;
                }
            }

            if (stateChange && !inGame) {
                SDL_BlitSurface(title_image, NULL, screen_image, NULL);
                // Determine which 'Start Button' image to display
                if (m_over_start) {
                    SDL_BlitSurface(start_image_green, NULL, screen_image, &start_button_rect);
                }
                else {
                    SDL_BlitSurface(start_image_blue, NULL, screen_image, &start_button_rect);
                }

                // Determine which 'Flow Button' image to display
                if (m_over_flow) {
                    SDL_BlitSurface(flow_button, NULL, screen_image, &flow_button_rect);
                }
                else {
                    SDL_BlitSurface(get_flow_button_img(), NULL, screen_image, &flow_button_rect);
                }

                // Determine which 'Junk Button' image to display
                if (m_over_junk) {
                    if (junk) {
                        SDL_BlitSurface(junk_on_g, NULL, screen_image, &junk_button_rect);
                    }
                    else {
                        SDL_BlitSurface(junk_off_g, NULL, screen_image, &junk_button_rect);
                    }
                }
                else {
                    if (junk) {
                        SDL_BlitSurface(junk_on, NULL, screen_image, &junk_button_rect);
                    }
                    else {
                        SDL_BlitSurface(junk_off, NULL, screen_image, &junk_button_rect);
                    }
                }
                gameBoard.reset(new Board(junk));
                gameBoard->tiles[BOARD_SIZE - 1][BOARD_SIZE - 1]->flip();
                gameBoard->tiles[BOARD_SIZE - 1][BOARD_SIZE - 1]->flow();
                stateChange = false;
            }



            // Update the window
            SDL_UpdateWindowSurface(window);
        }
        close();
    }
}

void Application::close() {
    //SDL_FreeSurface(...);
}

void Application::input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {

        // Clicking 'x' or pressing F4.
        if (event.type == SDL_QUIT) {
            exit = true;
        }

        // Track mouse motion
        if (event.type == SDL_MOUSEMOTION) {
            m_x = event.motion.x;
            m_y = event.motion.y;

            if (!inGame) {
                // Check if its over start button
                if (m_x >= 300 && m_x <= 500 && m_y >= 350 && m_y <= 410) {
                    m_over_start = true;
                    stateChange = true;
                }
                else {
                    m_over_start = false;
                    stateChange = true;
                }
                // Check if its over flow button
                if (m_x >= 150 && m_x <= 350 && m_y >= 430 && m_y <= 490) {
                    m_over_flow = true;
                    stateChange = true;
                }
                else {
                    m_over_flow = false;
                    stateChange = true;
                }
                // Check if its over junk button
                if (m_x >= 450 && m_x <= 650 && m_y >= 430 && m_y <= 490) {
                    m_over_junk = true;
                    stateChange = true;
                }
                else {
                    m_over_junk = false;
                    stateChange = true;
                }
            }
            //in game
            else {
                if ((gameBoard->loss() || gameBoard->won()) && m_x >= 400 && m_x <= 600 && m_y >= 400 && m_y <= 460) {
                    // Check if over exit button
                    m_over_exit = true;
                    stateChange = true;
                } else {
                    m_over_exit = false;
                    stateChange = true;
                }
                tile_idx_y = m_y / 60;
                tile_idx_x = (m_x - 200) / 60;

                if (tile_idx_y >= 0 && tile_idx_y < BOARD_SIZE
                        && tile_idx_x >= 0 && tile_idx_x < BOARD_SIZE) {
                            stateChange = true;
                }
                if (!gameBoard->loss() && !gameBoard->won() && m_x >= 0 && m_x <= 200 && m_y >= 540 && m_y <= 600) {
                    m_over_ac = true;
                    stateChange = true;
                }
                else {
                    m_over_ac = false;
                    stateChange = true;
                }

            }
        }


        // Track mouse click
        if (event.type == SDL_MOUSEBUTTONDOWN) {
            if (!inGame) {
                // Toggle starting a game
                if (m_over_start) {
                    inGame = true;
                    stateChange = true;
                    gameStart = true;
                }
                // Toggle flow speed (SLOW-7/MED-5/FAST-3)
                if (m_over_flow) {
                    if (delay - 2 < 3) {
                        delay = Delay(SLOW);
                    }
                    else {
                        delay = Delay(delay - 2);
                    }
                }
                if (m_over_junk) {
                    junk = !junk;
                }
            }
            // in game
            else {
                if (gameBoard->loss() && m_over_exit) {
                    inGame = false;
                    stateChange = true;
                }
                else if (tile_idx_y >= 0 && tile_idx_y < BOARD_SIZE
                    && tile_idx_x >= 0 && tile_idx_x < BOARD_SIZE) {
                        if (gameBoard->tiles[tile_idx_x][tile_idx_y]->isFlipped()) {
                            bool b = gameBoard->swap(*gameBoard->tiles[tile_idx_x][tile_idx_y]);
                            if (b) {
                                Mix_PlayChannel(-1, placePipe, 0);
                            }
                        }
                        else {
                            Mix_PlayChannel(-1, click, 0);
                            gameBoard->tiles[tile_idx_x][tile_idx_y]->flip();

                        }
                        stateChange = true;
                }
                else if (m_over_ac && !gameBoard->won() && !gameBoard->loss()) {
                    // Play sound
                    Mix_PlayChannel(-1, bubble2, 0);
                    gameBoard->autoComplete();
                    stateChange = true;
                }
            }
        }

        // Pressing a key.
        if (event.type == SDL_KEYDOWN) {
            if (inGame) {
                // Key controls within the game
                switch (event.key.keysym.sym) {
                    // Pressing ESC exits from the game.
                    case SDLK_ESCAPE:
                        inGame = false;
                        stateChange = true;
                        break;

                }
            }
            else {
                // Key controls within the title screen
                switch (event.key.keysym.sym) {
                    // Pressing ESC closes the window.
                    case SDLK_ESCAPE:
                        exit = true;
                        break;
                    case SDLK_RETURN:
                        inGame = true;
                        stateChange = true;
                        gameStart = true;
                        break;
                }
            }
        }

        if (event.type == SDL_USEREVENT) {
            if (inGame) {
                if (!gameBoard->loss() && !gameBoard->won()) {
                        gameBoard->increaseFlow();
                        if (!gameBoard->won()) {
                            Mix_PlayChannel(-1, bubble1, 0);
                        }
                }
            }
        }
    }
}

// Render objects on screen.
void Application::render() {
    // Clear screen (background color).
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);



    // Swap buffers.
    SDL_RenderPresent(renderer);
}

// Load in media (images)
bool Application::loadMedia() {
    bool success = true;
    // Images
    title_image = SDL_LoadBMP("images/title_screen.bmp");
    game_bg = SDL_LoadBMP("images/game_background.bmp");
    game_bg_won = SDL_LoadBMP("images/game_background_won.bmp");

    mystery_tile = SDL_LoadBMP("images/mystery_tile.bmp");
    junk_tile = SDL_LoadBMP("images/junk_tile.bmp");

    start_image_blue = SDL_LoadBMP("images/start_button.bmp");
    start_image_green = SDL_LoadBMP("images/start_button_g.bmp");
    flow_button = SDL_LoadBMP("images/flow_button.bmp");
    flow_button_slow = SDL_LoadBMP("images/flow_button_slow.bmp");
    flow_button_med = SDL_LoadBMP("images/flow_button_medium.bmp");
    flow_button_fast = SDL_LoadBMP("images/flow_button_fast.bmp");
    junk_on = SDL_LoadBMP("images/junk_tiles_on.bmp");
    junk_on_g = SDL_LoadBMP("images/junk_tiles_on_g.bmp");
    junk_off = SDL_LoadBMP("images/junk_tiles_off.bmp");
    junk_off_g = SDL_LoadBMP("images/junk_tiles_off_g.bmp");

    pipe_bot_left = SDL_LoadBMP("images/pipe_bot_left.bmp");
    pipe_bot_left_f = SDL_LoadBMP("images/pipe_bot_left_f.bmp");
    pipe_bot_right = SDL_LoadBMP("images/pipe_bot_right.bmp");
    pipe_bot_right_f = SDL_LoadBMP("images/pipe_bot_right_f.bmp");
    pipe_top_left = SDL_LoadBMP("images/pipe_top_left.bmp");
    pipe_top_left_f = SDL_LoadBMP("images/pipe_top_left_f.bmp");
    pipe_top_right = SDL_LoadBMP("images/pipe_top_right.bmp");
    pipe_top_right_f = SDL_LoadBMP("images/pipe_top_right_f.bmp");
    pipe_straight_h = SDL_LoadBMP("images/pipe_straight_h.bmp");
    pipe_straight_h_f = SDL_LoadBMP("images/pipe_straight_h_f.bmp");
    pipe_straight_v = SDL_LoadBMP("images/pipe_straight_v.bmp");
    pipe_straight_v_f = SDL_LoadBMP("images/pipe_straight_v_f.bmp");

    game_over = SDL_LoadBMP("images/game_over.bmp");
    exit_button = SDL_LoadBMP("images/exit_button.bmp");
    exit_button_green = SDL_LoadBMP("images/exit_button_g.bmp");
    game_won = SDL_LoadBMP("images/game_won.bmp");
    auto_comp = SDL_LoadBMP("images/auto_comp.bmp");
    auto_comp_green = SDL_LoadBMP("images/auto_comp_g.bmp");
    game_won = SDL_LoadBMP("images/game_won.bmp");
    // Sounds
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 70 * 1024) < 0) {
        printf("SDL_mixer count not initialiaze! error: %s\n", Mix_GetError());
        success = false;
    }

    bubble1 = Mix_LoadWAV("sounds/bubbling1.wav");
    flush = Mix_LoadWAV("sounds/flush.wav");
    wah = Mix_LoadWAV("sounds/whah_whah.wav");
    bubble2 = Mix_LoadWAV("sounds/bubbling2.wav");
    click = Mix_LoadWAV("sounds/click.wav");
    placePipe = Mix_LoadWAV("sounds/place_pipe.wav");

    // Fonts
    timerFont = TTF_OpenFont( "fonts/open-sans/OpenSans-Regular.ttf", 28 );

    if (title_image == NULL) {
        printf("Failed to load resource: title_screen\n");
        success = false;
    }
    if (game_bg == NULL) {
        printf("Failed to load resource: game_background\n");
        success = false;
    }
    if (game_bg_won == NULL) {
        printf("Failed to load resource: game_background_won");
        success = false;
    }
    if (mystery_tile == NULL) {
        printf("Failed to load resource: mystery_tile\n");
        success = false;
    }
    if (junk_tile == NULL) {
        printf("Failed to load resource: junk_tile\n");
        success = false;
    }
    if (start_image_blue == NULL) {
        printf("Failed to load resource: start_button\n");
        success = false;
    }
    if (start_image_green == NULL) {
        printf("Failed to load resource: start_button_g\n");
        success = false;
    }
    if (flow_button == NULL) {
        printf("Failed to load resource: flow_button\n");
        success = false;
    }
    if (flow_button_slow == NULL) {
        printf("Failed to load resource: flow_button_slow\n");
        success = false;
    }
    if (flow_button_med == NULL) {
        printf("Failed to load resource: flow_button_med\n");
        success = false;
    }
    if (flow_button_fast == NULL) {
        printf("Failed to load resource: flow_button_fast\n");
        success = false;
    }
    if (junk_on == NULL) {
        printf("Failed to load resource: junk_on\n");
        success = false;
    }
    if (junk_on_g == NULL) {
        printf("Failed to load resource: junk_on_g\n");
        success = false;
    }
    if (junk_off == NULL) {
        printf("Failed to load resource: junk_off\n");
        success = false;
    }
    if (junk_off_g == NULL) {
        printf("Failed to load resource: junk_off_g\n");
        success = false;
    }
    if (pipe_bot_left == NULL) {
        printf("Failed to load resource: pipe_bot_left\n");
        success = false;
    }
    if (pipe_bot_left_f == NULL) {
        printf("Failed to load resource: pipe_bot_left_f\n");
        success = false;
    }
    if (pipe_bot_right == NULL) {
        printf("Failed to load resource: pipe_bot_right\n");
        success = false;
    }
    if (pipe_bot_right_f == NULL) {
        printf("Failed to load resource: pipe_bot_right_f\n");
        success = false;
    }
    if (pipe_top_left == NULL) {
        printf("Failed to load resource: pipe_top_left\n");
        success = false;
    }
    if (pipe_top_left_f == NULL) {
        printf("Failed to load resource: pipe_top_left_f\n");
        success = false;
    }
    if (pipe_top_right == NULL) {
        printf("Failed to load resource: pipe_top_right\n");
        success = false;
    }
    if (pipe_top_right_f == NULL) {
        printf("Failed to load resource: pipe_top_right_f\n");
        success = false;
    }
    if (pipe_straight_h == NULL) {
        printf("Failed to load resource: pipe_straight_h\n");
        success = false;
    }
    if (pipe_straight_h_f == NULL) {
        printf("Failed to load resource: pipe_straight_h_f\n");
        success = false;
    }
    if (pipe_straight_v == NULL) {
        printf("Failed to load resource: pipe_straight_v\n");
        success = false;
    }
    if (pipe_straight_v_f == NULL) {
        printf("Failed to load resource: pipe_straight_v_f\n");
        success = false;
    }
    if (game_over == NULL) {
        printf("Failed to load resource: game_over\n");
        success = false;
    }
    if (exit_button == NULL) {
        printf("Failed to load resource: exit_button\n");
        success = false;
    }
    if (exit_button_green == NULL) {
        printf("Failed to load resource: exit_button_g\n");
        success = false;
    }
    if (exit_button_green == NULL) {
        printf("Failed to load resource: exit_button_g\n");
        success = false;
    }
    if (game_won == NULL) {
        printf("Failed to load resource: game_won\n");
    }
    if (auto_comp == NULL) {
        printf("Failed to load resource: auto_comp\n");
    }
    if (auto_comp_green == NULL) {
        printf("Failed to load resource: auto_comp_g\n");
    }

    if (bubble1 == NULL) {
        printf("Failed to load resource: bubbling1\n");
        success = false;
    }
    if (flush == NULL) {
        printf("Failed to load resource: flush\n");
        success = false;
    }
    if (wah == NULL) {
        printf("Failed to load resource: whah_whah");
        success = false;
    }
    if (bubble2 == NULL) {
        printf("Failed to load resource: bubbling2\n");
        success = false;
    }
    if (click == NULL) {
        printf("Failed to load resource: click\n");
        success = false;
    }
    if (placePipe == NULL) {
        printf("Failed to load resource: place_pipe\n");
        success = false;
    }
    if (timerFont == NULL) {
        printf("Failed to load resource: timerFont\n");
        printf(TTF_GetError());
        success = false;
    }

    return success;
}

SDL_Surface* Application::get_tile_img(Tile &t) {
    if (t.isJunk()) {
        return junk_tile;
    }
    bool filled = t.hasFlow();
    switch (t.getO1()) {
        case NORTH:
            switch (t.getO2()) {
                case NORTH:
                    printf("malformed tile\n");
                    return NULL;
                case SOUTH:
                    if (filled) {
                        return pipe_straight_v_f;
                    }
                    return pipe_straight_v;
                case EAST:
                    if (filled) {
                        return pipe_top_right_f;
                    }
                    return pipe_top_right;
                case WEST:
                    if (filled) {
                        return pipe_top_left_f;
                    }
                    return pipe_top_left;
            }
        case SOUTH:
            switch (t.getO2()) {
                case NORTH:
                    if (filled) {
                        return pipe_straight_v_f;
                    }
                    return pipe_straight_v;
                case SOUTH:
                    printf("malformed tile\n");
                    return NULL;
                case EAST:
                    if (filled) {
                        return pipe_bot_right_f;
                    }
                    return pipe_bot_right;
                case WEST:
                    if (filled) {
                        return pipe_bot_left_f;
                    }
                    return pipe_bot_left;
            }
        case EAST:
           switch (t.getO2()) {
                case NORTH:
                    if (filled) {
                        return pipe_top_right_f;
                    }
                    return pipe_top_right;
                case SOUTH:
                    if (filled) {
                        return pipe_bot_right_f;
                    }
                    return pipe_bot_right;
                case EAST:
                    printf("malformed tile\n");
                    return NULL;
                case WEST:
                    if (filled) {
                        return pipe_straight_h_f;
                    }
                    return pipe_straight_h;
            }
        case WEST:
            switch (t.getO2()) {
                case NORTH:
                    if (filled) {
                        return pipe_top_left_f;
                    }
                    return pipe_top_left;
                case SOUTH:
                    if (filled) {
                        return pipe_bot_left_f;
                    }
                    return pipe_bot_left;
                case EAST:
                    if (filled) {
                        return pipe_straight_h_f;
                    }
                    return pipe_straight_h;
                case WEST:
                    printf("malformed tile\n");
                    return NULL;
            }
        default:
              printf("malformed tile\n");
            return NULL;
    }
}

SDL_Surface* Application::get_flow_button_img() {
    switch (delay) {
        case SLOW:
            return flow_button_slow;
        case MEDIUM:
            return flow_button_med;
        case FAST:
            return flow_button_fast;
    }
}

void Application::set_rect(SDL_Rect &rect, int w, int h, int x, int y) {
    rect.w = w;
    rect.h = h;
    rect.x = x;
    rect.y = y;
}

void Application::setUserTimer() {
     if (!time) {
        time << "Time: " << 0 << 0;
     }
     if (running) {
         time.str("");
         time.clear();
         //Render the time surface
         Uint32 currentTime = SDL_GetTicks();
         Uint32 elapsed = (currentTime - timer) / 1000;
         if (elapsed / 60 < 10) {
            time << "Timer: " << 0 << elapsed / 60;
         } else {
            time << "Timer: " << elapsed / 60;
         }
         if (elapsed % 60 < 10) {
            time << ":" << 0 << elapsed % 60 ;
         } else {
            time << ":" << elapsed % 60 ;
         }
     }
     SDL_Rect timer_rect = SDL_Rect();
     set_rect(timer_rect, 0, 0, 0, 100);
     SDL_Color textColor={0,0,0};
     seconds = TTF_RenderText_Solid(timerFont, time.str().c_str() , textColor );
     SDL_BlitSurface(seconds, NULL, screen_image, &timer_rect);
     SDL_FreeSurface( seconds );
}

Uint32 onTick(Uint32 interval, void *param) {
    SDL_Event event;
    SDL_UserEvent userevent;

    std::cout << "onTick called" << std::endl;

    userevent.type = SDL_USEREVENT;
    userevent.code = 0;

    event.type = SDL_USEREVENT;
    event.user = userevent;

    SDL_PushEvent(&event);
    return interval;
}


