#include "ScreenList.h"
#include "IGameScreen.h"
#include <stdio.h>

ScreenList::ScreenList(MainGame *game)
    : _game(game),
    _current_screen_index(-1)
{
}

ScreenList::~ScreenList()
{
    for(size_t i = 0; i < _screens.size(); i++)
    {
        _screens[i]->destroy();
        delete _screens[i];
    }
    _screens.clear();
    _current_screen_index = -1;
}

IGameScreen* ScreenList::move_next()
{
    IGameScreen *current_screen = get_current_screen();
    if(current_screen->get_next_screen_index() != -1)
    {
        _current_screen_index = current_screen->get_next_screen_index();
    }

    return get_current_screen();
}

IGameScreen* ScreenList::move_previous()
{
    IGameScreen *current_screen = get_current_screen();
    if(current_screen->get_previous_screen_index() != -1)
    {
        _current_screen_index = current_screen->get_previous_screen_index();
    }

    return get_current_screen();
}

void ScreenList::set_screen(int screen_index)
{
    _current_screen_index = screen_index;
}

void ScreenList::add_screen(IGameScreen *screen)
{
    screen->set_screen_index(_screens.size());
    _screens.push_back(screen);
    screen->build();
    screen->set_parent_game(_game);
}

IGameScreen *ScreenList::get_current_screen()
{
    if(_current_screen_index == -1)
        return NULL;

    return _screens[_current_screen_index];
}
