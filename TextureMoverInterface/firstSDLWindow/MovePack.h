#pragma once
#include "Parse.h"
#include "Flags.h"
#include "App.h"
#include <array>
#include <sstream>

using namespace Utils;

namespace Packs
{
	class MovePack;

	const LDPoint NULL_CON_POINT = { INT_MAX, INT_MAX };

	enum TT_Name
	{
		BASE,
		PATH,
		WAVE,
		ROTATION,
		FLIP,
		ENV
	};

	struct TransformType
	{
		TransformType(bool activate);
		~TransformType() = default;

		bool active;

		virtual const TT_Name name();
		virtual const std::stringstream getSettings();
		virtual void assignToPack(MovePack& pack);
		virtual void clear();
		virtual void parseAndAssign(std::string& rawPresetLine);
	};

	struct Path : public TransformType
	{
		Path();
		Path(LDPoint orig, LDPoint dest, int sp, bool in_place = false);
		Path(LDPoint dest, int sp, bool in_place = false);
		Path& operator++();

		LDPoint origin, destination;

		int moveSpeed;
		int distOut;
		bool inPlace;
		void resetDistOut();

		const TT_Name name() override;
		const std::stringstream getSettings() override;
		void assignToPack(MovePack& pack) override;
		void clear() override;
		void parseAndAssign(std::string& rawPresetLine) override;
	};

	struct Wave : public TransformType
	{
		enum Type { NONE, SINE, TRIANGLE };

		Wave();
		Wave(Type t, int amp, float per, bool invert = false);

		Type type;
		// static int castWaveType(Type t);
		int amplitude;
		float period;
		bool inverted;

		const TT_Name name() override;
		const std::stringstream getSettings() override;
		void assignToPack(MovePack& pack) override;
		void clear() override;
		void parseAndAssign(std::string& rawPresetLine) override;
	};

	struct Rotation : public TransformType /* <- This and Flip need direct access to BaseTexture::RenderExOptions */
	{
		Rotation();
		Rotation(float rotateSpeed);
		Rotation(float rotateSpeed, double startAng);
		Rotation(float rotateSpeed, double startAng, LDPoint cent);

		double startingAngle; /**/
		float rotationSpeed;
		LDPoint center;

		const TT_Name name() override;
		const std::stringstream getSettings() override;
		void assignToPack(MovePack& pack) override;
		void clear() override;
		void parseAndAssign(std::string& rawPresetLine) override;
	};

	struct Flip : public TransformType
	{
		enum Type { NONE, HORIZONTAL, VERTICAL, DIAGONAL };

		Flip();
		Flip(Type ft);

		SDL_RendererFlip flipType;
		static SDL_RendererFlip castFlipType(Type t);

		const TT_Name name() override;
		const std::stringstream getSettings() override;
		void assignToPack(MovePack& pack) override;
		void clear() override;
		void parseAndAssign(std::string& rawPresetLine) override;
	};

	struct Env : public TransformType
	{
		Env();
		Env(int srcW, int srcH);

		const TT_Name name() override;
		const std::stringstream getSettings() override;
		void parseAndAssign(std::string& rawPresetLine) override;

		int srcScreenWidth, srcScreenHeight;

		static Env make();
	};

	//struct Scale : public TransformType
	//{
	//};


	//struct Clip : public TransformType
	//{
	//	Clip();
	//	Clip(SDL_Rect cr);

	//	SDL_Rect clipRect;

	//	void assignToPack(MovePackNew& pack) override;
	//	void clear() override;
	//};

	//struct TT_Settings
	//{
	//	std::map<std::string, std::string> ttSettings;

	//	template <class T, typename...Args>
	//	TT_Settings(T&& t, Args&&...args);

	//	template <typename...Args>
	//	void addPackSettings(Args&&...args);

	//	template <class T>
	//	void addPackSettings(T&& t);

	//	void addPackSettings();
	//};
	
	// CHANGE TTName() Methods to return these vars instead of literals



	struct TT_Tools
	{
		static LDPoint makeNextPathPoint(const Path& path);
		static double lengthToDestination(const LDPoint& testPoint, const Path& path);
		static int makeWaveYOffset(int x, const Wave& wave);
	};

	class MovePack // Holds things that are executed at the same time
	{
	public:
		// friend class TextureMover;

		MovePack();
		~MovePack();
		MovePack(const MovePack& rhs);
		void operator=(const MovePack& rhs);

		void setPath(LDPoint orig, LDPoint dest, int sp, bool in_place);
		void setPath(const Path&& p);
		void setWave(Wave::Type t, int amp, float per);
		void setWave(const Wave&& w);
		void setRotation(double angle, int rotateSpeed, LDPoint cent);
		void setRotation(const Rotation&& r);
		void setFlip(Flip::Type ft);
		void setFlip(const Flip&& f);

		std::string getPackSettings();

		virtual bool proxyRef(); /**/

		Path mPath;				// 
		Wave mWave;				//  Make private & accessible to TextureMover
		Rotation mRotation;		// 
		Flip mFlip;				// 

	private:




		//enum TransformTypeMemberNames { mPATH, mWAVE, mROTATION, mFLIP };
		//std::array<TransformType&, 4> mTransformTypesIterable;
	};


};

