#include "MovePackQueue.h"

MovePackQueue::MovePackQueue() : currentPack(queue.end()), end(queue.end())
{}

MovePackQueue::MovePackQueue(std::list<MovePack>& loadedQueue) : currentPack(queue.end()), end(queue.end())
{
	for (auto& pack : loadedQueue)
		emplaceBack(pack);

	end = queue.end();
}

MovePackQueue::~MovePackQueue()
{} 

// MovePack created outside, so we pass in a reference to it and construct a copy to put into queue
void MovePackQueue::emplaceBack(MovePack& mp)
{
	bool wasEmpty = queue.empty();

	queue.emplace_back(mp);

	if (wasEmpty)
		currentPack = queue.begin();
}


MovePack& MovePackQueue::getCurrent()
{
	return *currentPack;
}

MovePack& MovePackQueue::getBack()
{
	return queue.back();
}

int MovePackQueue::size()
{
	return queue.size();
}

bool MovePackQueue::empty()
{
	return queue.empty();
}

bool MovePackQueue::atEnd()
{
	return std::next(currentPack) == end;
}

void MovePackQueue::resetToBeginning()
{
	for (auto& pack : queue)
	{
		pack.mPath.resetDistOut();
	}

	currentPack = queue.begin();
}

void MovePackQueue::operator++()
{
	if (!atEnd())
		++currentPack;
}


