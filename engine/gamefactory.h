#ifndef _GAMEFACTORY_H
#define _GAMEFACTORY_H


class IWindow;
class IGame;
class IInput;

IWindow *	get_game_window( IGame*);
IGame*		get_game();
IInput*		get_game_input();


void		release_game_window( IWindow** );
void		release_game( IGame** );
void		release_game_input( IInput** );


#endif