#pragma once
#include "MovePackQueue.h"
#include "BaseTexture.h"
#include <fstream>
#include <direct.h>
#include <cerrno>

// Holds :
	// List<MovePack> wrapper MovePackQueue
	// MovePack* to point to current step in the queue
	// Pointer to SDL_Texture wrapper BaseTexture
// Responsible for :
	// providing interface to add MovePacks to queue
	// assigning *queue.currentStep to loadedPack
	// carrying out functions that use the data inside a MovePack to physically move the pTexture


class TextureMover
{
public:
	TextureMover();
	TextureMover(BaseTexture* texture);
	TextureMover(BaseTexture* texture, int numRepeats);

	class Preset;
	TextureMover(Preset& preset);
	TextureMover(BaseTexture* texture, Preset& preset, int numRepeats);

	~TextureMover();
	void operator()();

	class AddStep
	{
	public:
		friend class TextureMover;

		template <typename T, typename... Args>
		void operator()(T&& t, Args&&...tTypes);
		template <typename T, typename... Args>
		void operator()(T&& t);

	private:
		AddStep(TextureMover& moverRef);

		TextureMover& moverRef;
		MovePack tempPack;
		void clearTempPack();
		//void resolveCtor();
	} addStep;

	class Repeat
	{
	public:
		friend class TextureMover;

		enum { LOOP_FOREVER = -1, }; /**/

	private:
		Repeat();
		Repeat(int numRepeats);

		int numRepeats;
		int counter;
		operator bool();
	} repeat;

	class Preset // MAKE LAST DATA LINE INCLUDE MISC DATA LIKE SCREEN_WIDTH/SCREEN_HEIGHT
	{
	public:
		friend class TextureMover;

		Preset(const std::string& presetNm);

	private:
		std::string presetName;
		std::list<MovePack> tempPackQueue;
		Env presetEnv;

		static const std::string FOLDER_NAME;
		static bool save(const std::string& settings, const std::string& presetName);
		static bool load(std::list<MovePack>& srcQueue, const std::string& presetName);
		static bool makePresetsFolder();
		static void dispatchLineToTT(MovePack& srcMovePack, std::string& dataLine);
		static int clipName(std::string& dataLine);
	};


	void savePreset(const std::string& presetName);

private:
	BaseTexture* pTexture;
	MovePack* loadedPack;
	MovePackQueue packQueue;

	void loadNextPack();
	void unloadPack();
	void reset();

	void execute();
	bool move();

	void applyWave(LDPoint& nextPoint);
	void applyRotation();
	void applyFlip();
	void adjustInPlace(LDPoint& nextPoint);
	void makeOnLoadTextureAdjustments();

	void handleRepeats();

	std::string exportQueueSettings();

	// use this to have TextureMover be able to communicate with MovePackQueue about when it is "null"
	struct : public MovePack { bool proxyRef() override { return true; } } proxyPackRef; /**/
};



template<typename T, typename ...Args>
inline void TextureMover::AddStep::operator()(T&& t, Args && ...tTypes)
{
	t.assignToPack(tempPack);
	this->operator()(std::forward<Args>(tTypes)...);
}

template<typename T, typename ...Args>
inline void TextureMover::AddStep::operator()(T&& t)
{
	if (tempPack.mPath.origin == NULL_CON_POINT)
	{
		// If no origin, we check if there is a pack at the end of the queue that we can pull it's destination from and use that for newMovePack.mPath's origin
		if (moverRef.packQueue.empty())
			throw std::invalid_argument("Must include origin point when adding first step");
		else
			tempPack.mPath.origin = moverRef.packQueue.getBack().mPath.destination;
	}

	t.assignToPack(tempPack);
	moverRef.packQueue.emplaceBack(tempPack);
	clearTempPack();
}
