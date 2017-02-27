# Design Overview

Warmonger is designed to be extremely flexible and to support a wide range of
possible games. I tried very hard to make it as flexible as possible and
hardcode as little as possible and make it very generic, within the frame
of turn-based strategy games.
The reason for this is that I like generic solutions and even I couldn't make
up my mind about how the game should exactly work. Going and rewrinting C++
code and static classes every time I change my mind is out of the question so
I designed warmonger so that it assumes as little as possible leaving all the
game logic to scripts. So warmonger is basically designed to be a fairly generic
turned-based strategy game engine. It provides a framework to make games.

Of course there are some assumptions made. Warmonger is designed for
[Heroes of Might and Magic](https://en.wikipedia.org/wiki/Heroes_of_Might_and_Magic)
and [Total War](https://en.wikipedia.org/wiki/Total_War_(series)) style strategy
games. That is action is happening on two levels: a campaign and battle maps.
Both campaigns and battles are turn-based.

The main building blocks of warmonger are:
 * World
 * WorldSurface
 * CampignMap
 * BattleMap

## World

In warmonger the world defines the game rules and content. Think of it as a mod
except in warmonger there is no first-class or built-in content and third-party
mods, there are worlds and all worlds are equal.

A world is composed of static data and scripts that define the available
content and it's behaviour. In a more formal way a world defines the available
entities and components (see [ECS](https://en.wikipedia.org/wiki/Entity%E2%80%93component%E2%80%93system))
the available events and how entities react to events (systems). The game has
only a very limited number of built-in components, these all concern the
graphical representation of entities and their interactions with the user.

## WorldSurface

The world-surface provides the graphical components of a world. A world can
have any number of associated world-surfaces, kind-of like "themes" or "skins".
This separation allows the easy coexistence of many look-and-feels for the same
game.

## CampaignMap

The game takes place on the campaign-map. A map is composed of hexagon-shaped
tiles.

## BattleMap

Battles are resolved on a generated BattleMap. This is usually a "zoom-out" of
the campaign-map tile where the conflicting parties are positioned.
