/**
 * Copyright (C) 2015-2018 Botond Dénes
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include "warmonger/Context.h"

#include <QDir>
#include <QGuiApplication>
#include <QStringList>
#include <fmt/format.h>

#include "io/File.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {

#if 0
static std::ostream& operator<<(std::ostream& os, Context::State state);
static std::unique_ptr<QObject> defaultStateTransitionFunction(const Context& ctx, QObject* const);
static std::unique_ptr<QObject> fromMainMenuToNewRandomMap(const Context& ctx, QObject* const);
static std::unique_ptr<QObject> fromNewRandomMapToGameplay(const Context& ctx, QObject* const);

struct StateTransition
{
    Context::State from;
    Context::State to;
    std::function<std::unique_ptr<QObject>(const Context& ctx, QObject* const)> transition;

    bool matches(Context::State from, Context::State to) const
    {
        return this->from == from && this->to == to;
    }

    std::unique_ptr<QObject> operator()(const Context& ctx, QObject* const old) const
    {
        return this->transition(ctx, old);
    }
};

const std::vector<StateTransition> stateTransitions{
    {Context::State::NewRandomMap, Context::State::MainMenu, defaultStateTransitionFunction},
    {Context::State::Gameplay, Context::State::MainMenu, defaultStateTransitionFunction},
    {Context::State::MainMenu, Context::State::NewRandomMap, fromMainMenuToNewRandomMap},
    {Context::State::NewRandomMap, Context::State::Gameplay, fromNewRandomMapToGameplay},
};

const std::unordered_map<Context::State, const char*> statePropertyNames{
    {Context::State::MainMenu, nullptr},
    {Context::State::NewRandomMap, "randomMapGenerator"},
    {Context::State::Gameplay, "game"},
};
#endif

void Context::_register_methods()
{
    godot::register_property<Context, godot::String>("version", nullptr, &Context::getVersion, "");
}

Context::Context()
    : world(nullptr)
    , worldSurface(nullptr)
{
#if 0
    this->world->setParent(this);
    this->worldSurface->setParent(this);
#endif
}

void Context::_init()
{
}

godot::String Context::getVersion() const
{
    const auto str = version.toStdString();
    return str.c_str();
}

#if 0
void Context::setState(State nextState)
{
    if (this->state == nextState)
        return;

    const auto prevState = this->state;

    const auto it = std::find_if(stateTransitions.cbegin(),
        stateTransitions.cend(),
        [=](const StateTransition& stateTransition) { return stateTransition.matches(prevState, nextState); });

    if (it == stateTransitions.cend())
    {
        auto msg = fmt::format("Invalid state transition (`{}'->`{}') requested", prevState, nextState);
        wError << msg;
        throw utils::ValueError(msg);
    }

    if (auto* prevName = statePropertyNames.at(prevState))
        this->setProperty(prevName, {});

    const auto prevContextObject = std::unique_ptr<QObject>(std::exchange(this->specialContextObject, nullptr));
    if (auto nextContextObject = (*it)(*this, prevContextObject.get()))
    {
        nextContextObject->setParent(this);
        this->specialContextObject = nextContextObject.release();

        auto* nextName = statePropertyNames.at(nextState);
        assert(nextName);

        this->setProperty(nextName, QVariant::fromValue(this->specialContextObject));
    }

    wInfo.format("Context state transition: {} -> {}", prevState, nextState);

    this->state = nextState;
}

void NewRandomMapContext::setSize(unsigned size)
{
    if (this->size == size)
        return;

    this->size = size;
    emit sizeChanged();
}

void NewRandomMapContext::setNumOfPlayers(unsigned numOfPlayers)
{
    if (this->numOfPlayers == numOfPlayers)
        return;

    if (numOfPlayers > this->world.maxNumberOfFactions())
    {
        wError.format("Requested number of players {} is larger than the maximum possible number of players ({})",
            numOfPlayers,
            this->world.maxNumberOfFactions());
        return;
    }

    this->numOfPlayers = numOfPlayers;
    emit numOfPlayersChanged();

    adjustPlayers();
}

QVariantList NewRandomMapContext::readPlayers() const
{
    QVariantList vlist;
    vlist.reserve(this->players.size());
    std::transform(this->players.begin(),
        this->players.end(),
        std::back_inserter(vlist),
        [](const std::unique_ptr<core::Faction>& f) { return QVariant::fromValue(f.get()); });
    return vlist;
}

std::unique_ptr<core::Map> NewRandomMapContext::generateMap()
{
    return this->world.getRules()->generateMap(1, this->size, std::move(this->players));
}

void NewRandomMapContext::adjustPlayers()
{
    if (this->numOfPlayers == this->players.size())
        return;

    if (this->numOfPlayers < this->players.size())
    {
        while (this->players.size() != this->numOfPlayers)
        {
            this->players.pop_back();
        }
    }
    else
    {
        while (this->players.size() != this->numOfPlayers)
        {
            this->players.emplace_back(std::make_unique<core::Faction>(nullptr));
            auto conf = core::nextAvailableBannerConfiguration(this->world, this->players);
            auto& player = *this->players.back();

            player.setBanner(conf.banner);
            player.setPrimaryColor(conf.primaryColor);
            player.setSecondaryColor(conf.secondaryColor);
        }
    }

    emit this->playersChanged();
}

GameplayContext::GameplayContext(std::unique_ptr<core::Map> map, QObject* parent)
    : QObject(parent)
    , map(*map)
{
    map->setParent(this);
    map.release();
}

static std::ostream& operator<<(std::ostream& os, Context::State state)
{
    switch (state)
    {
        case Context::State::MainMenu:
            os << "Context::State::MainMenu";
            break;
        case Context::State::NewRandomMap:
            os << "Context::State::NewRandomMap";
            break;
        case Context::State::Gameplay:
            os << "Context::State::Gameplay";
            break;
    }
    return os;
}

static std::unique_ptr<QObject> defaultStateTransitionFunction(const Context&, QObject* const)
{
    return nullptr;
}

static std::unique_ptr<QObject> fromMainMenuToNewRandomMap(const Context& ctx, QObject* const)
{
    return std::make_unique<NewRandomMapContext>(*ctx.getWorld());
}

static std::unique_ptr<QObject> fromNewRandomMapToGameplay(const Context&, QObject* const old)
{
    auto randomMapContext = qobject_cast<NewRandomMapContext*>(old);
    assert(randomMapContext);

    return std::make_unique<GameplayContext>(randomMapContext->generateMap());
}

#endif

} // namespace warmonger
