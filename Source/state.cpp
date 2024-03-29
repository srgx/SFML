#include "../Header/state.h"
#include "../Header/state_stack.h"


// Context contructor
State::Context::Context(sf::RenderWindow& window, TextureHolder& textures, FontHolder& fonts,
	MusicPlayer& music, SoundPlayer& sounds, KeyBinding& keys1, KeyBinding& keys2)
: window(&window)
, textures(&textures)
, fonts(&fonts)
, music(&music)
, sounds(&sounds)
, keys1(&keys1)
, keys2(&keys2){
    //
}

State::State(StateStack& stack, Context context) :
mStack(&stack), mContext(context){
  //
}

State::~State(){
  //
}




// Request stack push from current state(add to queue)
void State::requestStackPush(States::ID stateID){
	mStack->pushState(stateID);
}

// Request stack pop from current state(add to queue)
void State::requestStackPop(){
	mStack->popState();
}

// Request stack clear from current state(add to queue)
void State::requestStateClear(){
	mStack->clearStates();
}

// Get current context
State::Context State::getContext() const{
	return mContext;
}

void State::onActivate(){
	//
}

void State::onDestroy(){
	//
}
