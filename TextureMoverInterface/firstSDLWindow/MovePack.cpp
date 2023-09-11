#include "MovePack.h"


/*---------------------------------------------- TransformType ----------------------------------------------*/

Packs::TransformType::TransformType(bool activate) : active(activate)
{}

const Packs::TT_Name Packs::TransformType::name()
{
	return TT_Name::BASE;
}

const std::stringstream Packs::TransformType::getSettings()
{
	std::stringstream oss;

	oss << name();

	return oss;
}

void Packs::TransformType::assignToPack(Packs::MovePack& pack)
{}

void Packs::TransformType::clear()
{
	active = false;
}

void Packs::TransformType::parseAndAssign(std::string& rawPresetLine)
{
	// throw std::invalid_argument("Attempting to initialize base Transform Type");
}


/*------- Path ------*/

Packs::Path::Path() : TransformType(false), origin(0, 0), destination(0, 0), moveSpeed(0), inPlace(false), distOut(0)
{}

Packs::Path::Path(LDPoint orig, LDPoint dest, int sp, bool in_place) :
	TransformType(false), origin(orig), destination(dest), moveSpeed(sp), inPlace(in_place), distOut(0)
{}

Packs::Path::Path(LDPoint dest, int sp, bool in_place) :
	TransformType(false), origin(NULL_CON_POINT), destination(dest), moveSpeed(sp), inPlace(in_place), distOut(0)
{}

Packs::Path& Packs::Path::operator++()
{
	distOut += moveSpeed;
	return *this;
}

void Packs::Path::resetDistOut()
{
	distOut = 0;
}

const Packs::TT_Name Packs::Path::name()
{
	return TT_Name::PATH;
}

const std::stringstream Packs::Path::getSettings()
{
	std::stringstream oss;

	oss << name() << ' '
		<< active << ' '
		<< inPlace << ' '
		<< origin << ' '
		<< destination << ' '
		<< moveSpeed;

	return oss;
}


void Packs::Path::assignToPack(MovePack& pack)
{
	pack.setPath(std::forward<Path>(*this));
}

void Packs::Path::clear()
{
	active = false;
	inPlace = false;
	origin = destination = { 0, 0 };
	moveSpeed = 0;
	distOut = 0;
}

void Packs::Path::parseAndAssign(std::string& rawPresetLine)
{
	std::stringstream dataSS; 
	std::string tempData;

	dataSS << rawPresetLine;

	auto next = [&]() { dataSS >> tempData; };

	next();
	active = parse<bool>(tempData);
	next();
	inPlace = parse<bool>(tempData);
	next();
	origin = parse<LDPoint>(tempData);
	next();
	destination = parse<LDPoint>(tempData);
	next();
	moveSpeed = parse<int>(tempData);
	distOut = 0;

	if (!dataSS.eof())
		std::cout << "Unexpected data at end of Path stream. Skipped\n";
}


/*------ Wave ------*/

Packs::Wave::Wave() : TransformType(false), type(NONE), amplitude(0), period(0.0), inverted(false)
{}

Packs::Wave::Wave(Type t, int amp, float per, bool invert) : TransformType(false), type(t), amplitude(amp), period(per), inverted(invert)
{}

const Packs::TT_Name Packs::Wave::name()
{
	return TT_Name::WAVE;
}

const std::stringstream Packs::Wave::getSettings()
{
	std::stringstream oss;

	oss << name() << ' '
		<< active << ' '
		<< type << ' '
		<< amplitude << ' '
		<< period << ' '
		<< inverted;

	return oss;
}

void Packs::Wave::assignToPack(MovePack& pack)
{
	pack.setWave(std::forward<Wave>(*this));
}

void Packs::Wave::clear()
{
	active = false;
	type = NONE;
	amplitude = 0;
	period = 0.0;
	inverted = false;
}

void Packs::Wave::parseAndAssign(std::string& rawPresetLine)
{
	std::stringstream dataSS;
	std::string tempData;

	dataSS << rawPresetLine;

	auto next = [&]() { dataSS >> tempData; };

	next();
	active = parse<bool>(tempData);
	next();
	type = static_cast<Wave::Type>(parse<int>(tempData)); /**/
	next();
	amplitude = parse<int>(tempData);
	next();
	period = parse<float>(tempData);
	next();
	inverted = parse<bool>(tempData);

	if (!dataSS.eof())
		std::cout << "Unexpected data at end of Wave stream. Skipped\n";
}


/*------ Rotation ------*/

Packs::Rotation::Rotation() : TransformType(false), rotationSpeed(0.0), startingAngle(0.0), center(NULL_CON_POINT)
{}

Packs::Rotation::Rotation(float rotateSpeed) : TransformType(false), rotationSpeed(rotateSpeed), startingAngle(0.0), center(NULL_CON_POINT)
{}

Packs::Rotation::Rotation(float rotateSpeed, double startAng) : TransformType(false), startingAngle(startAng), rotationSpeed(rotateSpeed), center(NULL_CON_POINT)
{}

Packs::Rotation::Rotation(float rotateSpeed, double startAng, LDPoint cent) : TransformType(false), startingAngle(startAng), rotationSpeed(rotateSpeed), center(cent)
{}

const Packs::TT_Name Packs::Rotation::name()
{
	return TT_Name::ROTATION;
}

const std::stringstream Packs::Rotation::getSettings()
{
	std::stringstream oss;

	oss << name() << ' '
		<< active << ' '
		<< rotationSpeed << ' '
		<< startingAngle << ' '
		<< center;

	return oss;
}

void Packs::Rotation::assignToPack(MovePack& pack)
{
	pack.setRotation(std::forward<Rotation>(*this));
}

void Packs::Rotation::clear()
{
	active = false;
	rotationSpeed = 0.0;
	startingAngle = 0.0;
	center = NULL_CON_POINT;
}

void Packs::Rotation::parseAndAssign(std::string& rawPresetLine)
{
	std::stringstream dataSS;
	std::string tempData;

	dataSS << rawPresetLine;

	auto next = [&]() { dataSS >> tempData; };

	next();
	active = parse<bool>(tempData);
	next();
	rotationSpeed = parse<float>(tempData);
	next();
	startingAngle = parse<double>(tempData);
	next();
	center = parse<LDPoint>(tempData);

	if (!dataSS.eof())
		std::cout << "Unexpected data at end of Rotation stream. Skipped\n";
}


/*------- Flip -------*/

Packs::Flip::Flip() : TransformType(false), flipType(SDL_RendererFlip::SDL_FLIP_NONE)
{}

Packs::Flip::Flip(Type ft) : TransformType(false), flipType(castFlipType(ft))
{}

SDL_RendererFlip Packs::Flip::castFlipType(Type t)
{
	switch (t)
	{
	case HORIZONTAL:
		return SDL_RendererFlip::SDL_FLIP_HORIZONTAL;
	case VERTICAL:
		return SDL_RendererFlip::SDL_FLIP_VERTICAL;
	case DIAGONAL:
		return static_cast<SDL_RendererFlip>(SDL_RendererFlip::SDL_FLIP_HORIZONTAL | SDL_RendererFlip::SDL_FLIP_VERTICAL); /**/
	case NONE: default:
		return SDL_RendererFlip::SDL_FLIP_NONE;
	}
}

const Packs::TT_Name Packs::Flip::name()
{
	return TT_Name::FLIP;
}

void Packs::Flip::assignToPack(MovePack& pack)
{
	pack.setFlip(std::forward<Flip>(*this));
}

void Packs::Flip::clear()
{
	active = false;
	flipType = SDL_FLIP_NONE;
}

const std::stringstream Packs::Flip::getSettings()
{
	std::stringstream oss;

	oss << name() << ' '
		<< active << ' '
		<< flipType;

	return oss;
}

void Packs::Flip::parseAndAssign(std::string& rawPresetLine)
{
	std::stringstream dataSS;
	std::string tempData;

	dataSS << rawPresetLine;

	auto next = [&]() { dataSS >> tempData; };

	next();
	active = parse<bool>(tempData);
	next();
	Type tempType = static_cast<Flip::Type>(parse<int>(tempData));
	flipType = castFlipType(tempType);

	if (!dataSS.eof())
		std::cout << "Unexpected data at end of Flip stream. Skipped\n";
}




/*------------------------------------------------ Env ------------------------------------------------*/

Packs::Env::Env() : TransformType(false), srcScreenWidth(0), srcScreenHeight(0)
{}

Packs::Env::Env(int srcW, int srcH) : TransformType(false), srcScreenWidth(srcW), srcScreenHeight(srcH)
{}

const Packs::TT_Name Packs::Env::name()
{
	return TT_Name::ENV;
}

const std::stringstream Packs::Env::getSettings()
{
	std::stringstream oss;

	oss << name() << ' '
		<< active << ' '
		<< srcScreenWidth << ' '
		<< srcScreenHeight;

	return oss;
}

void Packs::Env::parseAndAssign(std::string& rawPresetLine)
{
	std::stringstream dataSS;
	std::string tempData;

	dataSS << rawPresetLine;

	auto next = [&]() { dataSS >> tempData; };

	next();
	active = parse<bool>(tempData);
	next();
	srcScreenWidth = parse<int>(tempData);
	next();
	srcScreenHeight = parse<int>(tempData);

	if (!dataSS.eof())
		std::cout << "Unexpected data at end of Env stream. Skipped\n";
}

Packs::Env Packs::Env::make()
{
#if defined(SCREEN_WIDTH) && defined(SCREEN_HEIGHT)
	return Env(SCREEN_WIDTH, SCREEN_HEIGHT);
#endif

	App::Display display{};
	return Env(display.WIDTH(), display.HEIGHT());
}

/*---------------------------------------------- MovePack ----------------------------------------------*/

Packs::MovePack::MovePack() : mPath(), mWave(), mRotation(), mFlip() 
{}

void Packs::MovePack::operator=(const MovePack& rhs)
{
	*this = rhs;
}

Packs::MovePack::~MovePack()
{}

Packs::MovePack::MovePack(const MovePack& rhs)
{
	this->mPath = rhs.mPath;
	this->mWave = rhs.mWave;
	this->mRotation = rhs.mRotation;
	this->mFlip = rhs.mFlip;
}

void Packs::MovePack::setPath(LDPoint orig, LDPoint dest, int sp, bool in_place)
{
	mPath = Path(orig, dest, sp, in_place);
	mPath.active = true;
}


void Packs::MovePack::setPath(const Path&& p)
{
	mPath = p;
	mPath.active = true;
}

void Packs::MovePack::setWave(Wave::Type t, int amp, float per)
{
	mWave = Wave(t, amp, per);
	mWave.active = true;
}

void Packs::MovePack::setWave(const Wave&& w)
{
	mWave = w;
	mWave.active = true;
}

void Packs::MovePack::setRotation(double ang, int rotateSpeed, LDPoint cent)
{
	mRotation = Rotation(ang, rotateSpeed, cent);
	mRotation.active = true;
}

void Packs::MovePack::setRotation(const Rotation&& r)
{
	mRotation = r;
	mRotation.active = true;
}

void Packs::MovePack::setFlip(Flip::Type ft)
{
	mFlip = Flip(ft);
	mFlip.active = true;
}

void Packs::MovePack::setFlip(const Flip&& f)
{
	mFlip = f;
	mFlip.active = true;
}

std::string Packs::MovePack::getPackSettings()
{
	return std::string(mPath.getSettings().str() + '\n' +
					   mWave.getSettings().str() + '\n' +
					   mRotation.getSettings().str() + '\n' +
					   mFlip.getSettings().str());
}

bool Packs::MovePack::proxyRef()
{
	return false;
}


/*---------------------------------------------- TransformTools ----------------------------------------------*/

LDPoint Packs::TT_Tools::makeNextPathPoint(const Path& path)
{
	return Equations::projectPointAlongLine(path.origin, path.destination, path.distOut);
}

double Packs::TT_Tools::lengthToDestination(const LDPoint& testPoint, const Path& path)
{
	return Equations::getLineLength(testPoint, path.destination);
}

int Packs::TT_Tools::makeWaveYOffset(int x, const Wave& wave)
{
	int y;
	switch (wave.type)
	{
	case Wave::SINE:
		y = Equations::sineWaveFunc(x, wave.amplitude, wave.period); break;
	case Wave::TRIANGLE:
		y = Equations::triangleWaveFunc(x, wave.amplitude, wave.period); break;
	case Wave::NONE: default:
		throw std::invalid_argument("Invalid Wave Type");
	}

	return y;
}


