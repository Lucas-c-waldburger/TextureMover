#pragma once
#include <list>
#include "MovePack.h"

using namespace Packs;

// Holds :
	// std::list of constructed MovePacks
	// iterator to MovePack at the current step in queue, iterator to end of queue
	// function returning a reference to the MovePack at the current step
	// function to reset step to beginning
	// basic STL container methods (empty(), size())
// Responsible for :
	// keeping track of current step
	// telling features of queue (size, etc)
	// pushing back constructed MovePacks

class MovePackQueue
{
public:
	friend class TextureMover;

	MovePackQueue();
	~MovePackQueue();

	void emplaceBack(MovePack& mp);
	MovePack& getCurrent();
	MovePack& getBack();

	int size();
	bool empty();
	bool atEnd();
	void resetToBeginning();

	// using QueueSettings = std::vector<std::vector<std::string>>;
	// QueueSettings exportQueueSettings();

	void operator++();

private:
	std::list<MovePack> queue;
	std::list<MovePack>::iterator currentPack, end; // Should make end const?

	MovePackQueue(std::list<MovePack>& loadedQueue);
};

