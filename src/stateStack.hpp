
#ifndef STATE_STACK_HPP
#define STATE_STACK_HPP

//#include <SFML/NonCopyable.hpp>
#include <SFML/System.hpp>

#include "state.hpp"
#include "stateIdentifiers.hpp"

class StateStack : private sf::NonCopyable
{
public:
    enum Action
    {
        Push,
        Pop,
        Clear,
    };

    explicit StateStack(State::Context context);

    template <class T>
    void registerState(States::ID stateID);

    void update(sf::Time dt);
    void draw();
    void handleEvent(const sf::Event& event);

    void pushState(States::ID stateID);
    void popState();
    void clearStates();

    bool isEmpty() const;
private:
    State::Ptr createState(States::ID stateID);
    void applyPendingChanges();

    struct PendingChange
    {
        Action action;
        States::ID stateID;
    };

    std::vector<State::Ptr> stack;
    std::vector<PendingChange> pendingList;
    State::Context context;
    std::map<States::ID, std::function<State::Ptr()>> factories;
};

#endif
