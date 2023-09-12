#include "TextureMover.h"

/*---------------------------------------------- TextureMover ----------------------------------------------*/

TextureMover::TextureMover() :
	addStep(*this), pTexture(nullptr), loadedPack(nullptr), packQueue(), repeat()
{}

TextureMover::TextureMover(BaseTexture* texture) :
	addStep(*this), pTexture(texture), loadedPack(nullptr), packQueue(), repeat()
{}

TextureMover::TextureMover(BaseTexture* texture, int numRepeats) :
	addStep(*this), pTexture(texture), loadedPack(nullptr), packQueue(), repeat(numRepeats)
{}

TextureMover::TextureMover(Preset& preset) :
	addStep(*this), pTexture(nullptr), loadedPack(nullptr), packQueue(preset.tempPackQueue), repeat()
{}

TextureMover::TextureMover(BaseTexture* texture, Preset& preset, int numRepeats) :
	addStep(*this), pTexture(texture), loadedPack(nullptr), packQueue(preset.tempPackQueue), repeat(numRepeats)
{}

TextureMover::~TextureMover() /**/
{
	pTexture = nullptr;
	loadedPack = nullptr;
}

void TextureMover::execute()
{
	if (loadedPack)
	{
		bool reachedDestination = move();
		if (reachedDestination)
		{
			if (packQueue.atEnd())
				handleRepeats();
			else
				loadNextPack();
		}
	}
	else if (!packQueue.empty())
	{
		reset();
	}
}

void TextureMover::operator()()
{
	if (pTexture)
		execute();
}


bool TextureMover::move() /***********************************/
{
	LDPoint nextPoint = TT_Tools::makeNextPathPoint(++loadedPack->mPath);
	double newLength = TT_Tools::lengthToDestination(nextPoint, loadedPack->mPath);
	
	bool reachedEnd = newLength <= loadedPack->mPath.moveSpeed;
	if (reachedEnd)
	{ 
		LDPoint finalDest = (loadedPack->mPath.inPlace) ? loadedPack->mPath.origin : loadedPack->mPath.destination;

		pTexture->setXY(finalDest);
	}
	else
	{	
		if (loadedPack->mWave.active)			 // 				 								   
			applyWave(nextPoint);			 //
			 					 //
		if (loadedPack->mPath.inPlace)			 // functions that modify nextPoint's xy
			adjustInPlace(nextPoint);		 // and/or set external related values 
								 //
		if (loadedPack->mRotation.active)		 //
			applyRotation();			 //
								 //

		pTexture->setXY(nextPoint);
	}

	return reachedEnd;
}


void TextureMover::applyWave(LDPoint& nextPoint)
{
	using Packs::Wave;

	if (loadedPack->mWave.type == Wave::SINE)
	{
		if (loadedPack->mWave.inverted)
			nextPoint.flipXY() *= SCREEN_RATIO;

		int yOffset = TT_Tools::makeWaveYOffset(nextPoint.x, loadedPack->mWave);
		nextPoint.y += (loadedPack->mPath.destination.x < loadedPack->mPath.origin.x) ? -yOffset : yOffset; /**/

		if (loadedPack->mWave.inverted)
			nextPoint.flipXY() /= SCREEN_RATIO;
	}
}

void TextureMover::applyRotation()
{
	// rendExOptions center* null-check happens in setRotationCenter(...)
	if (loadedPack->mRotation.center != NULL_CON_POINT)
		pTexture->getRendExOptions().setRotationCenter(loadedPack->mRotation.center);

	pTexture->getRendExOptions().setRotationAngle(
		pTexture->getRendExOptions().getRotationAngle() + loadedPack->mRotation.rotationSpeed);
}

void TextureMover::applyFlip()
{
	pTexture->getRendExOptions().setFlipType(loadedPack->mFlip.flipType);
}

void TextureMover::adjustInPlace(LDPoint& nextPoint)
{
	if (loadedPack->mWave.active)
	{
		if (!loadedPack->mWave.inverted)
			nextPoint.x = loadedPack->mPath.origin.x;
		else
			nextPoint.y = loadedPack->mPath.origin.y;
	}
	else
		nextPoint = loadedPack->mPath.origin;
}

void TextureMover::makeOnLoadTextureAdjustments()
{
	pTexture->setXY(loadedPack->mPath.origin);

	if (loadedPack->mRotation.active)
		pTexture->getRendExOptions().setRotationAngle(loadedPack->mRotation.startingAngle);

	if (loadedPack->mFlip.active)
		pTexture->getRendExOptions().setFlipType(loadedPack->mFlip.flipType);
}

std::string TextureMover::exportQueueSettings()
{
	std::string queueSettings;

	for (auto& pack : packQueue.queue)
		queueSettings += pack.getPackSettings() + "\n\n";

	return queueSettings;
}

void TextureMover::savePreset(const std::string& presetName)
{
	Preset::save(exportQueueSettings(), presetName);
}

void TextureMover::loadNextPack()
{
	pTexture->resetRendEx();

	loadedPack = &packQueue.getCurrent(); 

	makeOnLoadTextureAdjustments();

	++packQueue;
}

void TextureMover::unloadPack()
{
	loadedPack = nullptr;
}

void TextureMover::reset()
{
	packQueue.resetToBeginning();
	loadNextPack();
}

void TextureMover::handleRepeats()
{
	(repeat) ? reset() : unloadPack();
}


/*---------------------------------------------- Repeat ----------------------------------------------*/

TextureMover::Repeat::Repeat() : numRepeats(0), counter(0)
{}

TextureMover::Repeat::Repeat(int numExecutions) : numRepeats(numExecutions), counter(0)
{}

TextureMover::Repeat::operator bool()
{
	if (numRepeats == LOOP_FOREVER)
		return true;

	return ++counter < numRepeats;
}


/*---------------------------------------------- AddStep ----------------------------------------------*/

TextureMover::AddStep::AddStep(TextureMover& moverRef) : moverRef(moverRef)
{}

void TextureMover::AddStep::clearTempPack()
{	
	tempPack.mPath.clear();
	tempPack.mWave.clear();
	tempPack.mRotation.clear();
	tempPack.mFlip.clear();
}




//bool TextureMoverNew::AddStep::ctorChecks()
//{
//	bool validStepConstruction = true;
//
//	if (tempPack.mPath.active)
//	{
//		if (tempPack.mInPlace.active)
//			throw std::invalid_argument("addStep function can only accept a Path OR an InPlace transform type");
//
//		if (tempPack.mPath.origin == NULL_CON_POINT)
//		{
//			// check if there is a pack at the end of the queue that we can pull it's destination from and use that for newMovePack.mPath's origin
//			if (moverRef.packQueue.empty())
//				throw std::invalid_argument("Must include origin point when adding first step");
//			else
//			{
//				tempPack.mPath.origin = moverRef.packQueue.getBack().mPath.destination;
//			}
//		}
//	}
//
//	else if (tempPack.mInPlace.active)
//	{
//		if (tempPack.mInPlace.origin == NULL_CON_POINT)
//		{
//			// check if there is a pack at the end of the queue that we can pull it's destination from and use that for newMovePack.mPath's origin
//			if (moverRef.packQueue.empty())
//				throw std::invalid_argument("Must include origin point when adding first step");
//			else
//				tempPack.mInPlace.origin = moverRef.packQueue.getBack().mPath.destination;
//		}
//	}
//
//	else
//		throw std::invalid_argument("addStep function must include a Path");
//}


/*---------------------------------------------- Preset ----------------------------------------------*/

const std::string TextureMover::Preset::FOLDER_NAME = "presets";

TextureMover::Preset::Preset(const std::string& presetNm) : presetName(presetNm)
{
	load(tempPackQueue, presetName);
}


bool TextureMover::Preset::save(const std::string& settings, const std::string& presetName)
{
	if (makePresetsFolder())
	{
		std::ofstream fout("./" + FOLDER_NAME + '/' + presetName + ".txt");

		if (fout.is_open())
		{
			std::cout << "OPENED!";

			fout << settings;

			fout.close();

			return true;
		}
	}

	return false;
}

// NEED TO HANDLE ios_base::failiure EXCEPTIONS
bool TextureMover::Preset::load(std::list<MovePack>& srcQueue, const std::string& presetName)
{
	bool success = false;

	std::ifstream fin("./" + FOLDER_NAME + '/' + presetName + ".txt");

	if (fin.is_open())
	{
		std::cout << "Presets file opened successfully";

		std::string ttLine = "temp";

		while (fin)
		{
			MovePack pack;

			while (std::getline(fin, ttLine) && ttLine.size() != 0) // get all lines from one movepack
			{
				try
				{
					std::cout << "\nTT_LINE: " << ttLine << '\n';
					dispatchLineToTT(pack, ttLine);
				}

				catch (std::runtime_error& ex)
				{
					std::cout << ex.what();
					return false;
				}
			}

			srcQueue.emplace_back(pack);
		}

		if (fin.eof())
		{
			std::cout << "End of file";
			success = true;
		}
		else
			std::cout << "Non-eof termination";

		fin.close();
	}

	else
		std::cout << "Preset file could not be opened.";

	return success;
}

bool TextureMover::Preset::makePresetsFolder()
{
	errno = _mkdir("presetsFolder");

	switch (errno)
	{
	case 0:
		std::cout << "Folder created successfully\n"; return true;
	case EEXIST:
		std::cout << "Folder already exsists\n"; return true;
	default:
		std::cout << "Other error in folder creation\n"; return false;
	}
}

void TextureMover::Preset::dispatchLineToTT(MovePack& srcMovePack, std::string& dataLine)
{
	int ttName = clipName(dataLine);
	std::cout << "\nTT_NAME: " << ttName << '\n';
	using Packs::TT_Name;
	switch (ttName)
	{
	case TT_Name::PATH:
		srcMovePack.mPath.parseAndAssign(dataLine); break;

	case TT_Name::WAVE:
		srcMovePack.mWave.parseAndAssign(dataLine); break;

	case TT_Name::ROTATION:
		srcMovePack.mRotation.parseAndAssign(dataLine); break;

	case TT_Name::FLIP:
		srcMovePack.mFlip.parseAndAssign(dataLine); break;

	// case TT_Name::ENV:
		
	case TT_Name::BASE:
		throw std::runtime_error("TT_Name recorded as BASE: cannot initialize a base TranformType"); break;

	default:
		throw std::runtime_error("Unrecognized TT_Name in data line");
	}
}

int TextureMover::Preset::clipName(std::string& dataLine)
{
	int ttName = -1;

	size_t ws = dataLine.find(' ');
	if (ws != std::string::npos)
		ttName = std::stoi(dataLine.substr(0, ws));

	dataLine.erase(0, ws + 1);

	return ttName;
}

// Preset class

// reads from preset .txt file
// does a conversion based on data from each TransformType (from TTName() at the beginning of each fin row)
// makes MovePack object
// overloaded MovePack constructor calls build methods from each TransformType
// whole thing happens in TextureMover class ( mover.loadPreset(const std::string& fileName) ? )
	// overloaded constructor?

