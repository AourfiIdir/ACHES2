#include <stdio.h>
#include "arbre.h"
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "Node.h"
#include "minimax.h"
// #include "minimax.h"
#define PLAYER1 1
#define PLAYER2 -1
#define CELL_SIZE 100
#define CELLS 3
#define SCREEN_WIDTH 600
#define SCREEN_HEIGHT 600
enum Page
{
    MENU_PAGE,
    GAME_PAGE1,
    GAME_PAGE2,
    GAME_ACHI
};
int broke;
enum Page current_page = MENU_PAGE;
int board[3][3];
int game_over = 0;
int gaming;
int turn = PLAYER1;
int rounds=6;
// nodes define
Node *n11 = NULL;
Node *n12 = NULL;
Node *n13 = NULL;
Node *n21 = NULL;
Node *n22 = NULL;
Node *n23 = NULL;
Node *n31 = NULL;
Node *n32 = NULL;
Node *n33 = NULL;

// sdl variables define
SDL_Window *window = NULL;
SDL_Renderer *render = NULL;
SDL_Texture *backgroundTexture1 = NULL;
SDL_Texture *backgroundTexture2 = NULL;
SDL_Texture *backgroundTexture3 = NULL;

SDL_Surface *loadedSurface1 = NULL;
SDL_Surface *loadedSurface2 = NULL;
SDL_Surface *loadedSurface3 = NULL;


//achi board
void achi_board(int board[3][3]);
int achi_event2(int player,int board[3][3], int board_width,int board_height,int cellsize);
int achi_event3(int player, int board[3][3], int board_width, int board_height, int cell_size);


//____________background
void load_background()
{
    if (backgroundTexture1 == NULL || backgroundTexture2 == NULL || backgroundTexture3 == NULL)
    {
        loadedSurface1 = IMG_Load("imgs/menu.jpg");
        loadedSurface2 = IMG_Load("imgs/game1.jpg");
        loadedSurface3 = IMG_Load("imgs/game2.jpg");
        if (loadedSurface1 == NULL || loadedSurface2 == NULL || loadedSurface3 == NULL)
        {
            printf("cant load the background");
        }
        backgroundTexture1 = SDL_CreateTextureFromSurface(render, loadedSurface1);
        backgroundTexture2 = SDL_CreateTextureFromSurface(render, loadedSurface2);
        backgroundTexture3 = SDL_CreateTextureFromSurface(render, loadedSurface3);
        SDL_FreeSurface(loadedSurface1);
        SDL_FreeSurface(loadedSurface2);
        SDL_FreeSurface(loadedSurface3);
        if (backgroundTexture1 == NULL || backgroundTexture2 == NULL || backgroundTexture3 == NULL)
        {
            printf("cant load texture background");
        }
    }
}
void render_backgrounds()
{
    switch (current_page)
    {
    case MENU_PAGE:
        SDL_RenderCopy(render, backgroundTexture1, NULL, NULL);
        break;
    case GAME_PAGE1:
        SDL_RenderCopy(render, backgroundTexture2, NULL, NULL);
        break;
    case GAME_PAGE2:
        SDL_RenderCopy(render, backgroundTexture3, NULL, NULL);
        break;
    case GAME_ACHI:
        SDL_RenderCopy(render, backgroundTexture3, NULL, NULL);
        break;
    }
    SDL_RenderPresent(render);
}
//____________pages
void renders_board(int board[3][3])
{
    // Calculate offsets to center the board
    int x_offset = (600 - (CELLS * CELL_SIZE)) / 2;
    int y_offset = (600 - (CELLS * CELL_SIZE)) / 2;

    //Clear the screen before drawing the new board state
    SDL_RenderCopy(render, backgroundTexture2, NULL, NULL);
    //render_page1();
    // Draw grid lines
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // White color for grid lines
    for (int i = 1; i < CELLS; i++)
    {
        // Vertical
        SDL_RenderDrawLine(render, x_offset + i * CELL_SIZE, y_offset, x_offset + i * CELL_SIZE, y_offset + CELLS * CELL_SIZE);
        // Horizontal
        SDL_RenderDrawLine(render, x_offset, y_offset + i * CELL_SIZE, x_offset + CELLS * CELL_SIZE, y_offset + i * CELL_SIZE);
    }

    // Draw board states (i.e., the game pieces)
    for (int i = 0; i < CELLS; i++)
    {
        for (int j = 0; j < CELLS; j++)
        {
            if (board[i][j] == PLAYER1)
            {
                SDL_SetRenderDrawColor(render, 0, 255, 0, 255); // Green color for PLAYER1
                SDL_Rect rectx;
                rectx.h = CELL_SIZE;
                rectx.w = CELL_SIZE;
                rectx.x = x_offset + j * CELL_SIZE;
                rectx.y = y_offset + i * CELL_SIZE;
                SDL_RenderFillRect(render, &rectx); // Fill the cell with color
            }
            else if (board[i][j] == PLAYER2)
            {
                SDL_SetRenderDrawColor(render, 255, 0, 0, 255); // Red color for PLAYER2
                SDL_Rect recto;
                recto.h = CELL_SIZE;
                recto.w = CELL_SIZE;
                recto.x = x_offset + j * CELL_SIZE;
                recto.y = y_offset + i * CELL_SIZE;
                SDL_RenderFillRect(render, &recto); // Fill the cell with color
            }
        }
    }

    SDL_RenderPresent(render); // Present the rendered frame to the screen
}



// render the init window
void render_font(const char *message, int x, int y)
{
    // loading the font
    TTF_Font *font = TTF_OpenFont("space.ttf", 24);
    if (font == NULL)
    {
        printf("Problem in accessing the font: %s\n", TTF_GetError());
        return;
    }

    // loading the color
    SDL_Color color = {255, 255, 255, 255};

    // loading all this into a surface
    SDL_Surface *text_s = TTF_RenderText_Solid(font, message, color);
    if (text_s == NULL)
    {
        printf("Error in creating the text surface: %s\n", TTF_GetError());
        TTF_CloseFont(font);
        return;
    }

    SDL_Texture *text_t = SDL_CreateTextureFromSurface(render, text_s);
    if (text_t == NULL)
    {
        printf("Error in creating the text texture: %s\n", SDL_GetError());
        SDL_FreeSurface(text_s);
        TTF_CloseFont(font);
        return;
    }

    // Get actual width and height of the text
    int text_width, text_height;
    SDL_QueryTexture(text_t, NULL, NULL, &text_width, &text_height);

    SDL_Rect textRect = {x, y, text_width, text_height};

    SDL_RenderCopy(render, text_t, NULL, &textRect);
    SDL_RenderPresent(render);

    // Destroy resources
    SDL_DestroyTexture(text_t);
    SDL_FreeSurface(text_s);
    TTF_CloseFont(font);
}

void render_menu()
{
    const char *welcome = "welcome to achi game";
    render_font(welcome, 100, 100);
    const char *p2p = "p2p";
    render_font(p2p, 250, 200);
    const char *ia = "IA";
    render_font(ia, 260, 260);
    const char *achi = "ACHI";
    render_font(achi, 260, 300);
    SDL_RenderPresent(render);
}
void render_page1()
{
    renders_board(board);
    const char *come_back = "come back";
    render_font(come_back, 0, 0);
}
void render_page2()
{
    renders_board(board);
    const char *come_back = "come back";
    render_font(come_back, 0, 0);
}
void render_page3(){
    achi_board(board);
    const char *come_back = "come back";
    render_font(come_back, 0, 0);
}
void render_pages()
{

    switch (current_page)
    {
    case MENU_PAGE:
        render_menu();
        break;
    case GAME_PAGE1:
        render_page1();
        break;
    case GAME_PAGE2:
        render_page2();
        break;
    case GAME_ACHI:
        render_page3();
        break;
    }
}

int init_window(void)
{
    SDL_Init(SDL_INIT_VIDEO);
    // init window
    window = SDL_CreateWindow("test", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 600, 600, SDL_WINDOW_SHOWN);
    if (window == NULL)
    {
        printf("window initialise is false");
        EXIT_FAILURE;
    }
    // init render
    render = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (render == NULL)
    {
        printf("false declaration");
        EXIT_FAILURE;
    }
    // init the image loader
    if (!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG))
    {
        printf("SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError());
        return 1;
    }
    // init the ttf library
    if (TTF_Init() == -1)
    {
        printf("the ttf didnt load");
    }
    // creating a surface
    //  Load the background image only once (outside of renders()) //if we get onely one page
    /*
    if (backgroundTexture == NULL) {
        SDL_Surface* loadedSurface = IMG_Load("imgs/background.jpg");
        if (loadedSurface == NULL) {
            printf("Unable to load image! SDL_image Error: %s\n", IMG_GetError());
            // Handle the error (e.g., exit the program)
        }
        backgroundTexture = SDL_CreateTextureFromSurface(render, loadedSurface);
        SDL_FreeSurface(loadedSurface);
    }
    SDL_RenderCopy(render, backgroundTexture, NULL, NULL);
    */

    // Clear the screen
    /*
    SDL_SetRenderDrawColor(render, 0, 255, 0, 255);
    SDL_RenderClear(render);
    // Render other elements (if any)
    */
    return EXIT_SUCCESS;
}
void destroy_window()
{
    /*
    SDL_DestroyTexture(text_t);//not yet created
    TTF_CloseFont(font);
    TTF_Quit();
    */
    SDL_DestroyTexture(backgroundTexture1);
    SDL_DestroyTexture(backgroundTexture2);
    SDL_DestroyTexture(backgroundTexture3);
    SDL_DestroyRenderer(render);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void pull_event1()
{
    int x, y;
    SDL_Event event;
    SDL_PollEvent(&event);
    switch (event.type)
    {
    case SDL_QUIT:
        gaming = 1;
        break;
    case SDL_MOUSEBUTTONDOWN:

        SDL_GetMouseState(&x, &y);
        if (x >= 250 && x <= (250 + 40) &&
            y >= 200 && y <= (200 + 30))
        {
            current_page = GAME_PAGE1;
            broke = 1;
        }
        else if (x >= 260 && x <= (260 + 40) &&
                 y >= 260 && y <= (260 + 30))
        {
            current_page = GAME_PAGE2;
            broke = 1; // get to the first loop to load the background
        }
        else if (x >= 0 && x <= (0 + 60) &&
                 y >= 0 && y <= (0 + 30))
        {
            current_page = MENU_PAGE;
            broke = 1;
        }else if (x >= 260 && x <= (260 + 60) &&
                 y >= 300 && y <= (300 + 30))
        {
            current_page = GAME_ACHI;
            broke = 1;
        }
    }
}
//placement p2p
int pull_event2(int player, int board_width, int board_height, int cell_size) {
    int x, y;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        int board_x = 150;
        int board_y = 150;

        switch (event.type) {
        case SDL_QUIT:
            gaming = 1; // Assuming 'gaming' is a flag to end the game
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            // Check if the click is within the game board area
            if (x >= board_x && x <= board_x + board_width &&
                y >= board_y && y <= board_y + board_height) {
                int row = (y - board_y) / cell_size;
                int col = (x - board_x) / cell_size;

                // Check if the cell is within bounds and is empty
                if (row >= 0 && row < board_height / cell_size &&
                    col >= 0 && col < board_width / cell_size &&
                    board[row][col] == 0) {
                    board[row][col] = player;
                    return 1; // Move was made, turn should change
                }
            } else if (x >= 0 && x <= 60 &&
                       y >= 0 && y <= 30) {
                current_page = MENU_PAGE;
                broke = 1;
            }
            break;

        default:
            break;
        }
    }
    return 0; // No valid move was made, turn should not change
}
//movement ia
int pull_event3(int player, int board_width, int board_height, int cell_size)
{
    static int phase = 0;
    static int src_row = -1, src_col = -1;
    int x, y;
    SDL_Event event;

    if (SDL_PollEvent(&event))
    {
        int board_x = 150;
        int board_y = 150;

        switch (event.type)
        {
        case SDL_QUIT:
            gaming = 1; // Exit the game
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            // Check if the click is within the game board area
            if (x >= board_x && x <= board_x + board_width &&
                y >= board_y && y <= board_y + board_height)
            {
                int row = (y - board_y) / cell_size;
                int col = (x - board_x) / cell_size;

                // Debugging
                printf("Mouse clicked at (%d, %d), Board position (%d, %d)\n", x, y, row, col);

                if (phase == 0)
                {
                    // Select the piece to move
                    if (row >= 0 && row < board_height / cell_size &&
                        col >= 0 && col < board_width / cell_size &&
                        board[row][col] == player)
                    {
                        // Clear the previously selected piece before making a new selection
                        if (src_row != -1 && src_col != -1)
                        {
                            board[src_row][src_col] = player; // Restore the previous piece
                            // Optionally, you can call a render function here to update the board visually
                        }

                        src_row = row;
                        src_col = col;
                        board[src_row][src_col] = 0; // Clear the selected cell
                        phase = 1; // Move to the next phase (select destination)
                        printf("Selected piece at (%d, %d)\n", src_row, src_col);
                    }
                }
                else if (phase == 1)
                {
                    // Select the destination cell
                    if (row >= 0 && row < board_height / cell_size &&
                        col >= 0 && col < board_width / cell_size &&
                        board[row][col] == 0 &&
                        ((abs(row - src_row) == 1 && col == src_col) ||
                         (abs(col - src_col) == 1 && row == src_row)))
                    {
                        // Move the piece
                        board[row][col] = player;
                        printf("Moved piece to (%d, %d)\n", row, col);
                        src_row = -1; // Reset the selection
                        src_col = -1;
                        phase = 0; // Reset the phase
                        return 1; // Move was made, turn should change
                    }
                    else
                    {
                        // Invalid move or destination, reset the selection
                        board[src_row][src_col] = player; // Restore the piece
                        printf("Invalid move, reset piece to (%d, %d)\n", src_row, src_col);
                        src_row = -1;
                        src_col = -1;
                        phase = 0; // Reset the phase
                    }
                }
            }
            else if (x >= 0 && x <= 60 &&
                     y >= 0 && y <= 30)
            {
                current_page = MENU_PAGE;
                broke = 1;
            }
            break;

        default:
            break;
        }
    }
    return 0; // No valid move was made, turn should not change
}
int pull_event4(int player, int board_width, int board_height, int cell_size) {
    int x, y;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        int board_x = 150;
        int board_y = 150;

        switch (event.type) {
        case SDL_QUIT:
            gaming = 1; // Assuming 'gaming' is a flag to end the game
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            // Check if the click is within the game board area
            if (x >= board_x && x <= board_x + board_width &&
                y >= board_y && y <= board_y + board_height) {
                int row = (y - board_y) / cell_size;
                int col = (x - board_x) / cell_size;

                // Check if the cell is within bounds and is empty
                if (row >= 0 && row < board_height / cell_size &&
                    col >= 0 && col < board_width / cell_size &&
                    board[row][col] == 0 && player == PLAYER1) {
                    board[row][col] = player;
                    return 1; // Move was made, turn should change
                }
            } else if (x >= 0 && x <= 60 &&
                       y >= 0 && y <= 30) {
                current_page = MENU_PAGE;
                broke = 1;
            }
            break;

        default:
            break;
        }
    }
    return 0; // No valid move was made, turn should not change
}


//movement ia
int pull_event5(int player, int board_width, int board_height, int cell_size)
{
    static int phase = 0;
    static int src_row = -1, src_col = -1;
    int x, y;
    SDL_Event event;

    if (SDL_PollEvent(&event))
    {
        int board_x = 150;
        int board_y = 150;

        switch (event.type)
        {
        case SDL_QUIT:
            gaming = 1; // Exit the game
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            // Check if the click is within the game board area
            if (x >= board_x && x <= board_x + board_width &&
                y >= board_y && y <= board_y + board_height)
            {
                int row = (y - board_y) / cell_size;
                int col = (x - board_x) / cell_size;

                // Debugging
                printf("Mouse clicked at (%d, %d), Board position (%d, %d)\n", x, y, row, col);

                if (phase == 0)
                {
                    // Select the piece to move
                    if (row >= 0 && row < board_height / cell_size &&
                        col >= 0 && col < board_width / cell_size &&
                        board[row][col] == player)
                    {
                        // Clear the previously selected piece before making a new selection
                        if (src_row != -1 && src_col != -1)
                        {
                            board[src_row][src_col] = player; // Restore the previous piece
                            // Optionally, you can call a render function here to update the board visually
                        }

                        src_row = row;
                        src_col = col;
                        board[src_row][src_col] = 0; // Clear the selected cell
                        phase = 1; // Move to the next phase (select destination)
                        printf("Selected piece at (%d, %d)\n", src_row, src_col);
                    }
                }
                else if (phase == 1)
                {
                    // Select the destination cell
                    if (row >= 0 && row < board_height / cell_size &&
                        col >= 0 && col < board_width / cell_size &&
                        board[row][col] == 0 &&
                        ((abs(row - src_row) == 1 && col == src_col) ||
                         (abs(col - src_col) == 1 && row == src_row)))
                    {
                        // Move the piece
                        board[row][col] = player;
                        printf("Moved piece to (%d, %d)\n", row, col);
                        src_row = -1; // Reset the selection
                        src_col = -1;
                        phase = 0; // Reset the phase
                        return 1; // Move was made, turn should change
                    }
                    else
                    {
                        // Invalid move or destination, reset the selection
                        board[src_row][src_col] = player; // Restore the piece
                        printf("Invalid move, reset piece to (%d, %d)\n", src_row, src_col);
                        src_row = -1;
                        src_col = -1;
                        phase = 0; // Reset the phase
                    }
                }
            }
            else if (x >= 0 && x <= 60 &&
                     y >= 0 && y <= 30)
            {
                current_page = MENU_PAGE;
                broke = 1;
            }
            break;

        default:
            break;
        }
    }
    return 0; // No valid move was made, turn should not change
}
int x_move;
int main(int argc, char *argv[])
{
    // create_all_nodes(n11,n12,n13,n21,n22,n23,n31,n32,n33);
    n11 = Creat_Node(0, 11);
    n12 = Creat_Node(0, 12);
    n13 = Creat_Node(0, 13);
    n21 = Creat_Node(0, 21);
    n22 = Creat_Node(0, 22);
    n23 = Creat_Node(0, 23);
    n31 = Creat_Node(0, 31);
    n32 = Creat_Node(0, 32);
    n33 = Creat_Node(0, 33);
    // linking_all_nodes(n11,n12,n13,n21,n22,n23,n31,n32,n33);
    linking(n11->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n12->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n13->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n21->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n22->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n23->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n31->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n32->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    linking(n33->pos, n11, n12, n13, n21, n22, n23, n31, n32, n33);
    gaming = init_window();
    load_background();
    while (gaming == 0)
    { // render background and pages should not be in the same loop
        init_board(board);
        rounds=6;
        pull_event1();
        render_backgrounds();
        broke = 0;
        //playing p2p
        while (gaming == 0 && broke == 0 && current_page == GAME_PAGE1) { //game loop for first option
        render_pages();

        if (check_winner_board(board) == 0)
        {
            if (rounds > 0)
            {
                if (pull_event2(turn, 300, 300, 100))
                {
                    // Change turn only after a valid move
                    turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
                    rounds--;
                }
            }
            else
            {
                if (pull_event3(turn, 300, 300, 100))
                {
                    // Change turn only after a valid move
                    turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
                }
            }
        }
        //display winner font
        if (check_winner_board(board) != 0) {
            int winner_num = check_winner_board(board);
            const char *winner;
            if (winner_num == PLAYER1) {
                winner = "The winner is Player One";
            } else {
                winner = "The winner is Player Two";
            }
            render_font(winner, 150, 60);
            pull_event1();
        }
    }    //ia
       while (gaming == 0 && broke == 0 && current_page == GAME_PAGE2) { // Game loop for first option
    render_pages();

    if (check_winner_board(board) == 0) {
        if (turn == PLAYER1 && pull_event4(turn, 300, 300, 100)) {
            // Change turn only after a valid move
            turn = PLAYER2;
            if (rounds > 0) {
                rounds--;
            }

            // Check for a winner after the player's move
            if (check_winner_board(board) == 0) {
                // AI's turn
                int best_row, best_col;
                get_best_move(board, &best_row, &best_col);
                board[best_row][best_col] = PLAYER2;
                turn = PLAYER1;
            }
        }
    }

    // Check for a winner after AI's move
    int winner_num = check_winner_board(board);
    if (winner_num != 0) {
        const char *winner;
        if (winner_num == PLAYER1) {
            winner = "The winner is Player One";
        } else {
            winner = "The winner is Player Two";
        }
        render_font(winner, 150, 60);
        pull_event1(); // Assuming this function waits for user input to continue
    }
}
        //achi
        //playing p2p
        while (gaming == 0 && broke == 0 && current_page == GAME_ACHI) { //game loop for first option
        render_pages();

        if (check_winner_board(board) == 0)
        {
            if (rounds > 0)
            {
                if (achi_event2(turn,board, 300, 300, 100))
                {
                    // Change turn only after a valid move
                    turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
                    rounds--;
                }
                
            }
            else
            {
                if (achi_event3(turn,board,300, 300, 100))
                {
                    // Change turn only after a valid move
                    turn = (turn == PLAYER1) ? PLAYER2 : PLAYER1;
                }
            }
        }
        //display winner font
        if (check_winner_board(board) != 0) {
            int winner_num = check_winner_board(board);
            const char *winner;
            if (winner_num == PLAYER1) {
                winner = "The winner is Player One";
            } else {
                winner = "The winner is Player Two";
            }
            render_font(winner, 150, 60);
            pull_event1();
        }
    }


        //menu
        while (gaming == 0 && broke == 0 && current_page == MENU_PAGE)
        {
            pull_event1();
            render_pages();
        }
    }
    destroy_window();
    return 0;
}

void achi_board(int board[3][3]) {
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // white color

    // Calculate offsets to center the board
    int x_offset = (600 - (CELLS * CELL_SIZE)) / 2;
    int y_offset = (600 - (CELLS * CELL_SIZE)) / 2;
    //Clear the screen before drawing the new board state
    SDL_RenderCopy(render, backgroundTexture3, NULL, NULL);
    //render_page1();
    // Draw grid lines
    SDL_SetRenderDrawColor(render, 255, 255, 255, 255); // White color for grid lines
    // Draw the board lines
    SDL_RenderDrawLine(render, x_offset, y_offset, x_offset + 300, y_offset + 300);
    SDL_RenderDrawLine(render, x_offset + 300, y_offset, x_offset, y_offset + 300);
    SDL_RenderDrawLine(render, x_offset + 150, y_offset, x_offset + 150, y_offset + 300);
    SDL_RenderDrawLine(render, x_offset, y_offset + 150, x_offset + 300, y_offset + 150);

    // Draw pieces
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            int x = x_offset + (i * CELL_SIZE);
            int y = y_offset + (j * CELL_SIZE);

            if (board[i][j] == PLAYER1) {
                SDL_SetRenderDrawColor(render, 0, 255, 0, 255); // Green color for PLAYER1
                // Draw X for PLAYER1 (you may want to use lines or a texture here)
                SDL_RenderDrawLine(render, x, y, x + CELL_SIZE, y + CELL_SIZE);
                SDL_RenderDrawLine(render, x + CELL_SIZE, y, x, y + CELL_SIZE);
            } else if (board[i][j] == PLAYER2) {
                SDL_SetRenderDrawColor(render, 0, 0, 255, 255); // Blue color for PLAYER2
                // Draw O for PLAYER2 (circle or similar shape)
                SDL_Rect orect;
                orect.x = x + CELL_SIZE / 4;
                orect.y = y + CELL_SIZE / 4;
                orect.w = CELL_SIZE / 2;
                orect.h = CELL_SIZE / 2;
                SDL_RenderDrawRect(render, &orect); // Draw rectangle as placeholder for O
            }
        }
    }
}


// Placement p2p
int achi_event2(int player, int board[3][3], int board_width, int board_height, int cell_size) {
    int x, y;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        int board_x = 150;
        int board_y = 150;

        switch (event.type) {
        case SDL_QUIT:
            gaming = 0; // Assuming 'gaming' is a flag to end the game
            return 0; // Exit the function when quitting

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            // Check if the click is within the game board area
            if (x >= board_x && x <= board_x + board_width &&
                y >= board_y && y <= board_y + board_height) {
                int col = (y - board_y) / cell_size;
                int row = (x - board_x) / cell_size;

                // Check if the cell is within bounds and is empty
                if (row >= 0 && row < 3 &&
                    col >= 0 && col < 3 &&
                    board[row][col] == 0) {
                    board[row][col] = player;
                    return 1; // Move was made, turn should change
                }
            } else if (x >= 0 && x <= 60 &&
                       y >= 0 && y <= 30) {
                current_page = MENU_PAGE;
                broke = 1;
                return 0; // No move was made, but menu was accessed
            }
            break;

        default:
            break;
        }
    }
    return 0; // No valid move was made, turn should not change
}




int achi_event3(int player, int board[3][3], int board_width, int board_height, int cell_size) {
    static int phase = 0; // 0: Selecting piece, 1: Selecting destination
    static int src_row = -1, src_col = -1; // Track selected piece position
    int x, y;
    SDL_Event event;

    if (SDL_PollEvent(&event)) {
        int board_x = 150; // Top-left x-coordinate of the board
        int board_y = 150; // Top-left y-coordinate of the board

        switch (event.type) {
        case SDL_QUIT:
            gaming = 1; // Exit the game
            break;

        case SDL_MOUSEBUTTONDOWN:
            SDL_GetMouseState(&x, &y);

            // Check if the click is within the game board area
            if (x >= board_x && x <= board_x + board_width &&
                y >= board_y && y <= board_y + board_height) {
                int col = (y - board_y) / cell_size;
                int row = (x - board_x) / cell_size;

                // Debugging info
                printf("Mouse clicked at (%d, %d), Board position (%d, %d)\n", x, y, row, col);

                if (phase == 0) {
                    // Phase 0: Select a piece
                    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == player) {
                        src_row = row;
                        src_col = col;
                        board[src_row][src_col] = 0; // Temporarily clear the selected cell
                        printf("Selected piece at (%d, %d)\n", src_row, src_col);
                        phase = 1; // Move to the destination selection phase
                    }
                } else if (phase == 1) {
                    // Phase 1: Select a destination
                    if (row >= 0 && row < 3 && col >= 0 && col < 3 && board[row][col] == 0 &&
                        (abs(row - src_row) <= 1 && abs(col - src_col) <= 1)) { // Include diagonals
                        board[row][col] = player; // Move the piece
                        printf("Moved piece to (%d, %d)\n", row, col);
                        src_row = -1; // Reset selection
                        src_col = -1;
                        phase = 0; // Reset to initial phase
                        return 1; // Move was made, turn should change
                    } else {
                        // Invalid move, reset selection
                        board[src_row][src_col] = player; // Restore the piece
                        printf("Invalid move, piece reset to (%d, %d)\n", src_row, src_col);
                        src_row = -1;
                        src_col = -1;
                        phase = 0; // Reset to initial phase
                    }
                }
            } else if (x >= 0 && x <= 60 && y >= 0 && y <= 30) {
                current_page = MENU_PAGE; // Handle menu navigation
                broke = 1;
            }
            break;

        default:
            break;
        }
    }
    return 0; // No valid move was made, turn should not change
}

