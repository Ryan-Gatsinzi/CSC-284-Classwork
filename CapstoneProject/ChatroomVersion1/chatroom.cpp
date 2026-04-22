/*******************
 * Author: Ryan Gatsinzi Udatinya
 * Filename: chatroom.cpp
 * last_updated: 21_04_2026
 * Desc: This is the firs't version of a chatroom program that displays a chat box, an input box and a menu on the side
 * 
 *******************/

#include <curses.h>
#include <vector>
#include <string.h>
#include <string>
#include <stdlib.h>
#include "Display.cpp"

#define MAX_INPUT 256
#define MAX_LINES 1000
#define WHITE_BLACK 1
#define YELLOW_BLACK 2
#define CYAN_BLACK 3

using namespace std;

void draw_bordered_window(WINDOW *win, const char *title)
{
    box(win, 0, 0);
    int width = getmaxx(win);
    mvwprintw(win, 0, (width - strlen(title)) / 2, "%s", title);
    wrefresh(win);
}

void draw_output(WINDOW *win, Display &disp)
{
    int maxY, maxX;
    int lineCount = disp.getData().size(), scrollPos = disp.getScroll();
    getmaxyx(win, maxY, maxX);
    werase(win);
    box(win, 0, 0);
    int color = WHITE_BLACK; // default color is white text, black background

    // Show visible lines
    for (int i = 0; i < maxY - 2 && i < disp.getData().size(); ++i)
    {
        if (disp.getId() == 1)
            color = (i <= 2 && disp.getScroll() == 0 ? YELLOW_BLACK : WHITE_BLACK);
        else if (disp.getId() == 2)
        {
            color = CYAN_BLACK;
        }

        wattron(win, COLOR_PAIR(color));

        if (disp.getId() == 1 && i > 3)
        {
            mvwprintw(win, i + 1, 1, "%s", string("You: " + disp.getData()[i + scrollPos]).c_str());
        }
        else if (disp.getId() == 2 && i == disp.getHighlight())
        {
            wattron(win, A_REVERSE);
            mvwprintw(win, i + 1, 1, "%s", disp.getData()[i + scrollPos].c_str());
            wattroff(win, A_REVERSE);
        }
        else
        {
            mvwprintw(win, i + 1, 1, "%s", disp.getData()[i + scrollPos].c_str());
        }

        wattroff(win, COLOR_PAIR(color));
    }

    // Simulated scroll bar
    if (lineCount > maxY - 2)
    {
        int barHeight = maxY - 2;
        int barPos = (scrollPos * barHeight) / lineCount;
        mvwaddch(win, barPos + 1, maxX - 2, ACS_CKBOARD);
    }

    wrefresh(win);
}

void scrollChat(WINDOW *inputWin, Display &chatboxDisp, bool &isScrolling, bool &onChat)
{
    int ch = wgetch(inputWin);

    if (ch == KEY_UP && chatboxDisp.getScroll() > 0)
    {
        chatboxDisp.setScroll(chatboxDisp.getScroll() - 1);
        isScrolling = true;
    }
    else if (ch == KEY_DOWN && chatboxDisp.getScroll() < chatboxDisp.getCount() - (chatboxDisp.getHeight() - 2))
    {
        chatboxDisp.setScroll(chatboxDisp.getScroll() + 1);

        isScrolling = true;
    }
    else if ((ch == '\n' || ch == '\r' || ch == KEY_ENTER))
    {
        isScrolling = false;
    }
    else if (ch == KEY_RIGHT)
    {
        onChat = false;
    }
}

void scrollRoom(WINDOW *roomlistWin, Display &roomlistDisp, bool &onChat)
{
    curs_set(0); // turn cursor visiblity off
    int ch = wgetch(roomlistWin);
    if (ch == KEY_UP)
    {
        roomlistDisp.setHighlight((roomlistDisp.getHighlight() - 1) >= 0 ? roomlistDisp.getHighlight() - 1 : roomlistDisp.getCount() - 1);
    }
    else if (ch == KEY_DOWN)
    {
        roomlistDisp.setHighlight((roomlistDisp.getHighlight() + 1) <= roomlistDisp.getCount() - 1 ? roomlistDisp.getHighlight() + 1 : 0);
    }
    else if (ch == KEY_LEFT)
    {
        onChat = true;
    }

    // else if ((ch == '\n' || ch == '\r' || ch == KEY_ENTER)){
    //     change room functionality - to be done later
    // }
}

int main()
{
    WINDOW *chatboxWin, *roomlistWin, *inputWin;
    char input[MAX_INPUT];

    // Buffers
    vector<string> lines1 = {"Welcome to the chat!", "This is a placeholder message.", "More messages will appear here later", ""};
    vector<string> lines2 = {"General", "Sport", "Programming", "Music", "Random"};

    // Init
    initscr();
    cbreak();
    noecho();
    // keypad(roomListWin, TRUE);
    start_color();

    init_pair(1, COLOR_WHITE, COLOR_BLACK);
    init_pair(2, COLOR_YELLOW, COLOR_BLACK);
    init_pair(3, COLOR_CYAN, COLOR_BLACK);

    Display chatboxDisp(1, LINES * 80 / 100, COLS * 80 / 100, lines1);

    Display roomlistDisp(2, LINES * 80 / 100, COLS - (COLS * 80 / 100), lines2);

    Display inputboxDisp(3, LINES - (LINES * 80 / 100), COLS);

    chatboxWin = newwin(chatboxDisp.getHeight(), chatboxDisp.getWidth(), 0, 0);

    roomlistWin = newwin(roomlistDisp.getHeight(), roomlistDisp.getWidth(), 0, chatboxDisp.getWidth());

    inputWin = newwin(inputboxDisp.getHeight(), inputboxDisp.getWidth(), chatboxDisp.getHeight(), 0);

    scrollok(chatboxWin, TRUE);
    keypad(inputWin, TRUE);
    keypad(roomlistWin, TRUE);

    bool isScrolling = false;
    bool onChat = true;

    while (1)
    {
        // Draw outputs
        draw_output(chatboxWin, chatboxDisp);
        draw_output(roomlistWin, roomlistDisp);

        // Draw input
        werase(inputWin);
        box(inputWin, 0, 0);
        // mvwprintw(inputWin, 1, 2, "Enter text for Window %d (type 'exit' to quit): ", toWin1 ? 1 : 2);
        mvwprintw(inputWin, 1, 2, "Enter text for Window %d (type 'exit' to quit): ", 1);
        wrefresh(inputWin);

        // Input
        if (!isScrolling)
        {
            echo();
            mvwgetnstr(inputWin, 2, 2, input, MAX_INPUT - 1);
            noecho();

            if (strcmp(input, "exit") == 0)
                break;
        }

        // Handle scrolling commands (when input is empty)
        if (strlen(input) == 0)
        {

            if (onChat)
            {
                scrollChat(inputWin, chatboxDisp, isScrolling, onChat);
            }

            while (!onChat)
            {
                scrollRoom(roomlistWin, roomlistDisp, onChat);
                draw_output(roomlistWin, roomlistDisp);
            }

            curs_set(1); // turn on cursor
            continue;
        }

        // Store line
        if (chatboxDisp.getCount() < MAX_LINES)
        {
            chatboxDisp.getData().push_back(input);

            if (chatboxDisp.getCount() > chatboxDisp.getHeight() - 2)
                chatboxDisp.setScroll(chatboxDisp.getCount() - (chatboxDisp.getHeight() - 2));
        }
    }

    delwin(chatboxWin);
    delwin(roomlistWin);
    delwin(inputWin);
    endwin();
    return 0;
}