#ifndef __FAKEFUNCTIONS_H__
#define __FAKEFUNCTIONS_H__

enum Functions {
	NO_FUNCTION,
	START,
	OPEN_IN_GAME_MENU,
	CLOSE_IN_GAME_MENU,
	ACTIVATE_VSYNC
};

class FakeFunctions
{
public:
	FakeFunctions();
	~FakeFunctions();

	void ExecuteFunction(Functions function);

private:
	void DoStart();
	void DoOpenGameMenu();
	void DoCloseGameMenu();
	void DoActivateVsync();

};

#endif //!__FAKEFUNCTIONS_H__