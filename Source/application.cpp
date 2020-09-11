#include "../Header/application.h"
#include "../Header/utility.h"
#include "../Header/state.h"
#include "../Header/state_identifiers.h"
#include "../Header/title_state.h"
#include "../Header/game_state.h"
#include "../Header/menu_state.h"
#include "../Header/pause_state.h"


const sf::Time Application::TimePerFrame = sf::seconds(1.f/60.f);


// Initialize variables and state stack with context
Application::Application() :
  mWindow(sf::VideoMode(640, 480), "States", sf::Style::Close), mTextures(), mFonts(),
  mPlayer(), mStateStack(State::Context(mWindow, mTextures, mFonts, mPlayer)),
  mStatisticsText(), mStatisticsUpdateTime(), mStatisticsNumFrames(0){

	mWindow.setKeyRepeatEnabled(false);
  mWindow.setFramerateLimit(60);

	mFonts.load(Fonts::Main, "Media/Sansation.ttf");
	mTextures.load(Textures::TitleScreen, "Media/Textures/TitleScreen.png");

	mStatisticsText.setFont(mFonts.get(Fonts::Main));
	mStatisticsText.setPosition(5.f, 5.f);
	mStatisticsText.setCharacterSize(10u);

  // Register all states with map and factory functions
	registerStates();

  // Push pending change to pending list vector
	mStateStack.pushState(States::Title);
}

void Application::run(){
	sf::Clock clock;
	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	while (mWindow.isOpen()){
		sf::Time dt = clock.restart();
		timeSinceLastUpdate += dt;
		while (timeSinceLastUpdate > TimePerFrame){
			timeSinceLastUpdate -= TimePerFrame;

			processInput();
			update(TimePerFrame);

			if (mStateStack.isEmpty()){
        mWindow.close();
      }

		}

		updateStatistics(dt);
		render();
	}
}

void Application::processInput(){
	sf::Event event;
	while (mWindow.pollEvent(event)){
		mStateStack.handleEvent(event);

		if (event.type == sf::Event::Closed){
      mWindow.close();
    }

	}
}

void Application::update(sf::Time dt){
	mStateStack.update(dt);
}

void Application::render(){
	mWindow.clear();

	mStateStack.draw();

	mWindow.setView(mWindow.getDefaultView());
	mWindow.draw(mStatisticsText);

	mWindow.display();
}

void Application::updateStatistics(sf::Time dt){
	mStatisticsUpdateTime += dt;
	mStatisticsNumFrames += 1;
	if (mStatisticsUpdateTime >= sf::seconds(1.0f)){
		mStatisticsText.setString("FPS: " + toString(mStatisticsNumFrames));

		mStatisticsUpdateTime -= sf::seconds(1.0f);
		mStatisticsNumFrames = 0;
	}
}

// All states are registered in mFactories map of mStateStack
void Application::registerStates(){
	mStateStack.registerState<TitleState>(States::Title);
	mStateStack.registerState<MenuState>(States::Menu);
	mStateStack.registerState<GameState>(States::Game);
	mStateStack.registerState<PauseState>(States::Pause);
}
