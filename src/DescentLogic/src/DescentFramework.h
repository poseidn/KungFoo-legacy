#pragma once

#include <memory>

#include <DescentEngine/src/Log.h>
#include <DescentEngine/src/Engines.h>
#include <DescentEngine/src/Rectangle.h>

#include <DescentEngine/src/StateEngine/StateTransitions.h>
#include <DescentEngine/src/Physics/PhysicsEngine.h>

#include <DescentEngine/src/ResourceEngine/ResourceEngine.h>

#include <DescentEngine/src/GameLoop.h>
#include <DescentEngine/src/EntityEngine/Entity.h>
#include <DescentEngine/src/EntityEngine/EntityFactory.h>
#include <DescentEngine/src/RandomEngine.h>
#include <DescentEngine/src/ScriptEngine/ScriptEngine.h>

#include <DescentEngine/src/SoundEngine/SoundEngine.h>

#include <DescentEngine/src/CinematicEngine/CinematicEngine.h>
#include <DescentEngine/src/CinematicEngine/CinematicAspect.h>

#include <DescentEngine/src/Visuals/SpriteVisual.h>
#include <DescentEngine/src/Visuals/ParticleSystemVisual.h>
#include <DescentEngine/src/Input/InputSystem.h>

#include <DescentEngine/src/AnimationEngine/ParticleAnimation.h>

// states
#include <DescentLogic/src/Game/GameState.h>
#include <DescentLogic/src/Menu/MenuState.h>
#include <DescentLogic/src/Intro/IntroState.h>

// include all needed aspects
#include <DescentLogic/src/Common/Aspects/QuitAspect.h>
#include <DescentLogic/src/Common/Aspects/DebugFunctionAspect.h>
#include <DescentLogic/src/Common/Aspects/ScriptAspect.h>

#include <DescentLogic/src/Game/Aspects/StatusAspect.h>
#include <DescentLogic/src/Game/Aspects/AttackAspect.h>
#include <DescentLogic/src/Game/Aspects/MovePlayerAspect.h>
#include <DescentLogic/src/Game/Aspects/ExecuteMoveIntentsAspect.h>
#include <DescentLogic/src/Game/Aspects/ForwardScrollingAspect.h>
#include <DescentLogic/src/Game/Aspects/DieAspect.h>
#include <DescentLogic/src/Game/Aspects/FreeScrollingAspect.h>
#include <DescentLogic/src/Game/Aspects/StartGameAspect.h>
#include <DescentLogic/src/Game/Aspects/EnemyAIAspect.h>
#include <DescentLogic/src/Game/Aspects/DebugVisualCollisionsAspect.h>
#include <DescentLogic/src/Game/Aspects/SpecialMoveDetectionAspect.h>
#include <DescentLogic/src/Game/Aspects/ScoringAspect.h>
#include <DescentLogic/src/Game/Aspects/ComboDetectionAspect.h>
#include <DescentLogic/src/Game/Aspects/DisplayEventsAspect.h>
#include <DescentLogic/src/Game/Aspects/RemoveEntitiesAspect.h>
#include <DescentLogic/src/Game/Aspects/LevelEditorAspect.h>

#include <DescentLogic/src/Menu/Aspects/StartMenuAspect.h>
#include <DescentLogic/src/Menu/Aspects/NavigateMenuAspect.h>
#include <DescentLogic/src/Menu/Aspects/InputMenuAspect.h>

#include <DescentLogic/src/Intro/Aspects/InputIntroAspect.h>
#include <DescentLogic/src/Intro/Aspects/AdvanceIntroAspect.h>

#include <DescentLogic/src/Game/Aspects/InputTranslationAspect.h>

#include <DescentLogic/src/LevelFactory.h>

#include <DescentLogic/src/Entities/PlayerEntity.h>
#include <DescentLogic/src/Entities/TextureIds.h>
#include <DescentLogic/src/Entities/GameTemplates.h>

#include <DescentLogic/src/Cinematics/IntroCinematic.h>

class DescentFrameworkBase: boost::noncopyable {
public:
	DescentFrameworkBase(bool asLevelEditor = false, bool demoMode = false, bool muted = false,
			bool forwardScrool = true, bool withIntro = true, bool showFps = false) :
			m_asLevelEditor(asLevelEditor), m_demoMode(demoMode), m_muted(muted), m_forwardScroll(
					forwardScrool), m_withIntro(withIntro), m_showFps(showFps) {

		m_snd.setMuted(m_muted);

		m_delayInput = new DelayedInput();

#ifdef USE_SDL
		SDLInput * m_hardwareInput = new SDLInput();
#endif

#ifdef USE_ANDROID
		AndroidInput * m_hardwareInput = new AndroidInput();
#endif

		m_input.reset(new InputSystem(m_hardwareInput, m_delayInput));
	}

	~DescentFrameworkBase() {
		m_resources.freeResources();
	}

	void initialize() {

		GameTemplates::registerTemplates(m_ee);

#ifdef USE_SDL
		// general SDL init
		if (SDL_Init(SDL_INIT_EVENTTHREAD) < 0)
		{

			fprintf(stderr, "Video initialization failed: %s\n",
					SDL_GetError());
			exit(1);
		}
#endif
	}
	/*
	 * needs to be fixed in android build
	 * probably use glXMakeContextCurrent in the render thread*/
	void initRenderEngine(RenderEngine::interface_type::InterfaceDataType & co) {
		m_re.init(co);
	}

	void executeBase() {
		logging::resetLogFile();

		logging::Info() << "Starting framework";
		logging::Info() << "Engines initialized";

		initialize();

		m_script.reset(new ScriptEngine(getResourceEngine()));

		// common stuff
		m_script->addObjectRegister([](lua_State * luaState)
		{
			luabind::module(luaState)[
			luabind::class_<TextVisual>("TextVisual")
			.def("setSizeFactor", &TextVisual::setSizeFactor )
			];
		});

		m_engines.reset(
				new Engines(m_re, m_resources, m_an, m_ee, m_snd, m_rand, *(m_input.get()), m_physics,
						*(m_script.get())));

		logging::Info() << "GameState created";
		m_gameState.reset(new GameState(*m_engines));

		if (m_asLevelEditor) {
			m_gameState->m_levelName = m_levelName;
		}

		if (m_demoMode) {

			float delayFactor = 1.0f;
			float gameStartOffset = 0.0f;

			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Kick,
							delayFactor * (1.0f + gameStartOffset)));

			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Jump,
							delayFactor * (2.0f + gameStartOffset)));

			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Kick,
							delayFactor * (5.0f + gameStartOffset)));
			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Kick,
							delayFactor * (5.5f + gameStartOffset)));
			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Kick,
							delayFactor * (6.4f + gameStartOffset)));

			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Jump,
							delayFactor * (7.0f + gameStartOffset)));
			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Kick,
							delayFactor * (8.0f + gameStartOffset)));

			m_delayInput->addCommand(
					DelayedInput::InputCommand(DelayedInput::InputCommand::ActionEnum::Exit,
							delayFactor * (15.0f + gameStartOffset)));
		}

		/*
		 *
		 * cinematic !!
		 m_nullState.reset(new NullState());

		 m_cinema.reset(new CinematicEngine(*m_engines));

		 IntroCinematic::setup(*m_cinema);

		 StateEngine<NullState> * stateNull = new StateEngine<NullState>(*m_nullState);
		 stateNull->registerAspect(new CinematicAspect<NullState, CinematicEngine>(*m_cinema));

		 m_strans.addState(stateNull);
		 */

		m_cinema.reset(new CinematicEngine(*m_engines));

		m_introState.reset(new IntroState(*m_engines));
		StateEngine<IntroState> * introEngine = new StateEngine<IntroState>(*m_introState);
		introEngine->registerAspect(new InputIntroAspect<IntroState>());
		introEngine->registerAspect(new AdvanceIntroAspect(*m_cinema));
		introEngine->registerAspect(new CinematicAspect<IntroState, CinematicEngine>(*m_cinema));
		introEngine->registerAspect(new DebugFunctionAspect<IntroState>());

		if (m_withIntro && (!m_asLevelEditor)) {
			m_strans.addState("intro", introEngine);
		}

		m_menuState.reset(new MenuState(*m_engines));
		StateEngine<MenuState> * menuEngine = new StateEngine<MenuState>(*m_menuState);
		m_script->addObjectRegister([](lua_State * luaState)
		{
			luabind::module (luaState)[
			//
				luabind::class_<MenuState>("MenuState")];
			});

		menuEngine->registerAspect(new InputMenuAspect<MenuState>());
		menuEngine->registerAspect(new NavigateMenuAspect());
		menuEngine->registerAspect(new StartMenuAspect());
		menuEngine->registerAspect(new QuitAspect<MenuState>());
		menuEngine->registerAspect(new DebugFunctionAspect<MenuState>());
		menuEngine->registerAspect(new ScriptAspect<MenuState>("boot_env", { ScriptImplements::Step }));

		if (!m_asLevelEditor)
			m_strans.addState("menu", menuEngine);

		StateEngine<GameState> * stateEngine = new StateEngine<GameState>(*m_gameState);

		m_script->addObjectRegister([](lua_State * luaState)
		{
			luabind::module(luaState)[
			luabind::class_<GameState>("GameState")
			//
			//.def(luabind::constructor<>())
			//

			// int getTotalScore(PlayerId id)
				.def("getTotalScore", &GameState::getTotalScore )
			//
				.def("getTextScore", &GameState::getTextScore )
			// .def("Fatal", &LuaLogging::Fatal)*/
				];
			});
		stateEngine->registerAspect(new RemoveEntitiesAspect());
		stateEngine->registerAspect(new InputTranslationAspect<GameState>());
		stateEngine->registerAspect(new StatusAspect());
		stateEngine->registerAspect(new MovePlayerAspect(m_forwardScroll));
		stateEngine->registerAspect(new EnemyAIAspect());
		stateEngine->registerAspect(new ForwardScrollingAspect(m_forwardScroll));
		stateEngine->registerAspect(new ScoringAspect());
		stateEngine->registerAspect(new StartGameAspect());
		stateEngine->registerAspect(new ComboDetectionAspect());
		stateEngine->registerAspect(new DisplayEventsAspect());
		stateEngine->registerAspect(new DebugFunctionAspect<GameState>());
		stateEngine->registerAspect(new ScriptAspect<GameState>("praise_score_increase", { {
				ScriptImplements::Step, ScriptImplements::OnScoreIncrease } }));
		//stateEngine->registerAspect(new DebugVisualCollisionsAspect());

		if (m_asLevelEditor) {

		}

		// must be done, after the movement intentions are complete
		// the move intents will be excuted by the game loop, after all the aspects
		// have been proceesed
		stateEngine->registerAspect(new QuitAspect<GameState>());
		stateEngine->registerAspect(new AttackAspect());
		stateEngine->registerAspect(new DieAspect());
		//stateEngine->registerAspect(new DebugVisualCollisionsAspect());

		stateEngine->registerAspect(new SpecialMoveDetectionAspect());

		if (!m_asLevelEditor) {
			m_strans.addState("game", stateEngine);
		}

		if (m_asLevelEditor) {
			m_levelState.reset(new GameState(*m_engines));

			StateEngine<GameState> * levelEngine = new StateEngine<GameState>(*m_levelState);

			levelEngine->registerAspect(new InputTranslationAspect<GameState>());
			levelEngine->registerAspect(new FreeScrollingAspect());
			levelEngine->registerAspect(new QuitAspect<GameState>());
			levelEngine->registerAspect(new LevelEditorAspect(m_levelName));

			m_strans.addState("lvl_edit", levelEngine);

			m_input->assignPlayerToInput(0, 0);

			/*InputSystem::PlayerInputId pInputId = 0;
			 auto inpCont = gs.get
			 for (auto & it : gs.getDeviceItems()) {
			 if (it.m_isEnabled) {
			 it.m_isEnabled = true;
			 logging::Info() << "Container " << it.m_ContainerId << " is assigned to PlayerId " << pInputId;
			 inpSystem.assignPlayerToInput(pInputId, it.m_ContainerId);
			 pInputId++;
			 }
			 }*/
		}

		//LevelFactory::createRandom(engines);

		// create "player"
		// todo: does not work with MT
		//fix me here
		//auto texPlayer = resources.loadImage("player_anim.png", 4);

		/*		ParticleSystemVisual * pv = new ParticleSystemVisual(m_re.getScreenTransform(), 10,
		 Vector3(5.0, 5.0, 1.0), Vector3(0.0, 0.0, 0.0));
		 m_re.addParticleSystemVisual(pv);*/

		//ParticleSphere pInit(Vector2(3.0, 3.0), 10.0f, m_engines->randomEngine(), 0.1f, 0.2f);
		/*ParticleStream pInit(Vector2(3.0, 3.0), Vector2(0.0, 1.0), 6.0f, m_engines->randomEngine(), 0.0f,
		 0.2f);
		 ParticleSystemVisual *pv = new ParticleSystemVisual(m_re.getScreenTransform(), 10, 0.7f, pInit);
		 m_re.addParticleSystemVisual(pv);

		 ParticleAnimation * partAn = new ParticleAnimation(*pv);
		 m_an.addParticleAnimation(partAn);*/

		//m_factory->collisionTestLevel(*m_gameState);
		//m_factory->renderTestLevel(*m_gameState);
		m_gameLoop.reset(new GameLoop(m_re, m_strans, m_ee, m_an, getInputSystem(), m_physics, m_showFps));

		//m_gl->setInitialContentFunc(initialContentFunc);

		logging::Info() << "Aspects registered";

		// get the music going
		auto music1 = m_resources.loadMusic("musik1");
		m_snd.playMusic(music1);

	}

	ResourceEngine & getResourceEngine() {
		return m_resources;
	}

	InputSystem & getInputSystem() {
		return *m_input.get();
	}

	SoundEngine & getSoundEngine() {
		return m_snd;
	}

	GameState * getGameState() {
		return m_gameState.get();
	}

	std::string m_levelName;

protected:

	RenderEngine m_re;
	//std::unique_ptr<GameLoop> m_gl;
	// todo: do the dest calls correctly and call SDL_quit afterwards
	std::unique_ptr<GameLoop> m_gameLoop;
	AnimationEngine m_an;
	ResourceEngine m_resources;
	EntityEngine m_ee;
	std::unique_ptr<InputSystem> m_input;
	StateTransitions m_strans;
	SoundEngine m_snd;
	RandomEngine m_rand;
	PhysicsEngine m_physics;

	//InputEgine * m_sdlInput;
	DelayedInput * m_delayInput;

	std::unique_ptr<Engines> m_engines;
	std::unique_ptr<GameState> m_gameState;
	std::unique_ptr<GameState> m_levelState;
	std::unique_ptr<MenuState> m_menuState;
	std::unique_ptr<IntroState> m_introState;
	std::unique_ptr<NullState> m_nullState;
	std::unique_ptr<CinematicEngine> m_cinema;
	std::unique_ptr<ScriptEngine> m_script;
	bool m_asLevelEditor;
	bool m_demoMode;
	bool m_muted;
	bool m_forwardScroll;
	bool m_withIntro;
	bool m_showFps;
}
;

#ifdef USE_SDL

class DescentFramework: public DescentFrameworkBase
{
public:

	DescentFramework( bool asLevelEditor = false, bool demoMode = false, bool muted = false, bool forwardScrool = true, bool withIntro = true, bool showFps = true) :
	DescentFrameworkBase ( asLevelEditor, demoMode,muted, forwardScrool, withIntro, showFps) {}

	void execute()
	{
		executeBase();
		assert ( m_gameLoop);

		// preload all images, needed for valgrind executes
		// disabled for now ...
		//preloadImages();

		m_gameLoop->run();
	}

};

#endif

#ifdef USE_ANDROID

class AndroidFramework: public DescentFrameworkBase
{
public:
	AndroidFramework( ) : DescentFrameworkBase ( false)
	{
	}

	void setJavaInterface ( JNIEnv * env, jobject obj)
	{
		m_javaInterface = JavaInterface( env, obj);
		getResourceEngine().setJavaInterface ( & m_javaInterface);
		getSoundEngine().setJavaInterface ( & m_javaInterface);

	}

	void step ( float timeDelta)
	{
		//logging::Info() << "Stepping android with delta " << timeDelta;
		m_gameLoop->step(timeDelta);
	}

private:
	JavaInterface m_javaInterface;
};

#endif
