#include "Ch3GameMode.h"
#include "Ch3Character.h"
#include "Ch3PlayerController.h"

ACh3GameMode::ACh3GameMode()
{
	DefaultPawnClass = ACh3Character::StaticClass();
	PlayerControllerClass = ACh3PlayerController::StaticClass();
}
