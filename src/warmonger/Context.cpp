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

#include "io/File.h"
#include "utils/Constants.h"
#include "utils/Logging.h"
#include "utils/QVariantUtils.h"
#include "utils/Settings.h"
#include "utils/Utils.h"

namespace warmonger {

static void format_arg(fmt::BasicFormatter<char>& f, const char*&, Context::State state);
static std::unique_ptr<QObject> defaultStateTransitionFunction(const Context& ctx, const QObject* const);
static std::unique_ptr<QObject> fromMainMenuToNewRandomMap(const Context& ctx, const QObject* const);
static std::unique_ptr<QObject> fromNewRandomMapToGameplay(const Context& ctx, const QObject* const);

struct StateTransition
{
    Context::State from;
    Context::State to;
    std::function<std::unique_ptr<QObject>(const Context& ctx, const QObject* const)> transition;

    bool matches(Context::State from, Context::State to) const
    {
        return this->from == from && this->to == to;
    }

    std::unique_ptr<QObject> operator()(const Context& ctx, const QObject* const old) const
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

Context::Context(
    std::unique_ptr<core::World>&& world, std::unique_ptr<ui::WorldSurface>&& worldSurface, QObject* parent)
    : QObject(parent)
    , world(world.release())
    , worldSurface(worldSurface.release())
    , disabledPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Disabled, this))
    , activePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Active, this))
    , inactivePalette(new ui::Palette(QGuiApplication::palette(), QPalette::Inactive, this))
    , normalPalette(new ui::Palette(QGuiApplication::palette(), QPalette::Normal, this))
{
    this->setObjectName("W");
    this->world->setParent(this);
    this->worldSurface->setParent(this);
}

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

std::unique_ptr<core::Map> NewRandomMapContext::generateMap() const
{
    return nullptr;
}

GameplayContext::GameplayContext(std::unique_ptr<core::Map> map, QObject* parent)
    : QObject(parent)
    , map(*map)
{
    map->setParent(this);
    map.release();
}

static void format_arg(fmt::BasicFormatter<char>& f, const char*&, Context::State state)
{
    switch (state)
    {
        case Context::State::MainMenu:
            f.writer().write("Context::State::MainMenu");
            break;
        case Context::State::NewRandomMap:
            f.writer().write("Context::State::NewRandomMap");
            break;
        case Context::State::Gameplay:
            f.writer().write("Context::State::Gameplay");
            break;
    }
}

static std::unique_ptr<QObject> defaultStateTransitionFunction(const Context&, const QObject* const)
{
    return nullptr;
}

static std::unique_ptr<QObject> fromMainMenuToNewRandomMap(const Context& ctx, const QObject* const)
{
    return std::make_unique<NewRandomMapContext>(*ctx.getWorld());
}

static std::unique_ptr<QObject> fromNewRandomMapToGameplay(const Context&, const QObject* const old)
{
    auto randomMapContext = std::unique_ptr<const NewRandomMapContext>(qobject_cast<const NewRandomMapContext*>(old));
    assert(randomMapContext);

    return std::make_unique<GameplayContext>(randomMapContext->generateMap());
}

} // namespace warmonger
