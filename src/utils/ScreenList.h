#ifndef SCREENLIST_H
#define SCREENLIST_H
#include <vector>

class IGameScreen;
class MainGame;

class ScreenList
{
public:
    ScreenList(MainGame *game);
    ~ScreenList();

    IGameScreen* move_next();
    IGameScreen* move_previous();

    void set_screen(int screen_index);
    void add_screen(IGameScreen *screen);

    IGameScreen* get_current_screen();

protected:
    MainGame *_game;
    std::vector<IGameScreen*> _screens;
    int _current_screen_index;
};

#endif // SCREENLIST_H