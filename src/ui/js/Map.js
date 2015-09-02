.pragma library

.import 'MapItem.js' as MapItem

function neighbourPos(dir, tileSize, pos) {
    var d;

    if (dir === 'West') {
        d = Qt.size(-tileSize.width, 0);
    } else if (dir === 'NorthWest') {
        d = Qt.size(-tileSize.width/2, -tileSize.height * 3/4);
    } else if (dir === 'NorthEast') {
        d = Qt.size(tileSize.width/2, -tileSize.height * 3/4);
    } else if (dir === 'East') {
        d = Qt.size(tileSize.width, 0);
    } else if (dir === 'SouthEast') {
        d = Qt.size(tileSize.width/2, tileSize.height * 3/4);
    } else if (dir === 'SouthWest') {
        d = Qt.size(-tileSize.width/2, tileSize.height * 3/4);
    } else {
        var err = "Unknown direction: " + dir;
        console.error(err);
        throw err;
    }

    return Qt.point(
        pos.x + d.width,
        pos.y + d.height
    );
};

var directions = [
    'West',
    'NorthWest',
    'NorthEast',
    'SouthEast',
    'East',
    'SouthWest'
];

var MouseEvents = {
    clicked: 'clicked',
    pressed: 'pressed',
    doubleClicked: 'doubleClicked',
    entered: 'entered',
    exited: 'exited',
    positionChanged: 'positionChanged',
    pressAndHold: 'pressAndHold',
    pressed: 'pressed',
    released: 'released',
    wheel: 'wheel'
};

/*
 * Map class.
 * @contructor
 */
var Map = function(W, canvas, mouseArea) {
    this.W = W;
    this.qobj = W.map;
    this.canvas = canvas;
    this.mouseArea = mouseArea;

    this.lastMouseEvent = undefined;
    this.lastMousePos = undefined;

    this.mapNodes = [];
    this.settlements = [];
    this.units = [];

    this.ready = false;
    this.geometryChanged = true;

    this.boundingRect = Qt.rect(0, 0, 0, 0);
    this.size = Qt.size(0, 0);

    // init
    this.qobj.settlementAdded.connect(this.onSettlementCreated.bind(this));
    this.qobj.unitAdded.connect(this.onUnitCreated.bind(this));

    this.createNodes(this.qobj.mapNodes[0], Qt.point(0, 0), {});
    this.createSettlements();
    this.createUnits();
};

Map.prototype.createNodes = function(mapNodeQObj, pos, visitedNodes) {
    this.createMapNode(mapNodeQObj, pos);

    visitedNodes[mapNodeQObj] = true;
    var tileSize = this.qobj.world.surface.tileSize;
    var neighbours = mapNodeQObj.neighbours;
    for (var direction in neighbours) {
        var neighbour = neighbours[direction];

        if (neighbour == undefined || neighbour in visitedNodes) continue;

        this.createNodes(
            neighbour,
            neighbourPos(direction, tileSize, pos),
            visitedNodes
        );
    }
};

Map.prototype.createMapNode = function(mapNodeQObj, pos) {
    var mapNodeJObj = this.newMapNode(mapNodeQObj, pos);
    this.addMapNode(mapNodeJObj);

    return mapNodeJObj;
};

Map.prototype.createSettlements = function() {
    var settlements = this.qobj.settlements;
    for (var i = 0; i < settlements.length; i++) {
        this.createSettlement(settlements[i]);
    }
};

Map.prototype.createSettlement = function(settlementQObj) {
    var mapNodeJObj = this.getMapNodeByQObj(settlementQObj.mapNode);

    var settlementJObj = this.newSettlement(settlementQObj, mapNodeJObj);
    this.addSettlement(settlementJObj);

    return settlementJObj;
};

Map.prototype.createUnits = function() {
    var units = this.qobj.units;
    for (var i = 0; i < units.length; i++) {
        this.createUnit(units[i]);
    }
};

Map.prototype.createUnit = function(unitQObj) {
    var mapNodeJObj = this.getMapNodeByQObj(unitQObj.mapNode);

    var unitJObj = this.newUnit(unitQObj, mapNodeJObj);
    this.addUnit(unitJObj);

    return unitJObj;
};

Map.prototype.addMapNode = function(mapNode) {
    this.mapNodes.push(mapNode);
};

Map.prototype.addSettlement = function(settlement) {
    this.settlements.push(settlement);
};

Map.prototype.addUnit = function(unit) {
    this.units.push(unit);
};

Map.prototype.markDirty = function(mapNode) {
    this.markTileDirty(mapNode.pos);
};

Map.prototype.markTileDirty = function(pos) {
    var tileSize = this.qobj.world.surface.tileSize;

    this.canvas.markDirty(Qt.rect(
        pos.x,
        pos.y,
        tileSize.width,
        tileSize.height
    ));
};

Map.prototype.onSettlementCreated = function(settlementQObj) {
    var settlementJObj = this.createSettlement(settlementQObj);
    this.markDirty(settlementJObj);
};

Map.prototype.onUnitCreated = function(unitQObj) {
    var unitJObj = this.createUnit(unitQObj);
    this.markDirty(unitJObj);
};

Map.prototype.getMapNodeByQObj = function(mapNodeQObj) {
    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        if (mapNode.qobj && mapNode.qobj == mapNodeQObj) {
            return mapNode;
        }
    }

    return undefined;
};

Map.prototype.getTileNeighbours = function(tilePos) {
    var tileSize = this.qobj.world.surface.tileSize;
    var neighbours = {}
    for (var i = 0; i < directions.length; i++) {
        var direction = directions[i];
        var pos = neighbourPos(direction, tileSize, tilePos);
        var neighbour = this.getMapNodeByPos(pos);

        if (neighbour != undefined) {
            neighbours[direction] = neighbour;
        }
    }

    return neighbours;
};

Map.prototype.getMapNodeByPos = function(pos) {
    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        if (mapNode.pos.x == pos.x && mapNode.pos.y == pos.y)
            return mapNode;
    }

    return undefined;
};

Map.prototype.findMapNodeAt = function(pos) {
    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        if (mapNode.contains(pos)) return mapNode;
    }

    return undefined;
};

Map.prototype.calculatePosOfMapNodeQObj = function(mapNodeQObj) {
    var direction;
    var neighbourQObj;
    for (direction in mapNodeQObj.neighbours) {
        neighbourQObj = mapNodeQObj.neighbours[direction];
        if (neighbourQObj != undefined) {
            break;
        }
    }

    var neighbourJObj = this.getMapNodeByQObj(neighbourQObj);

    // calculate the position of this node based on the neighbour's position
    var oppositeDirection = mapNodeQObj.oppositeDirection(direction);
    var tileSize = this.qobj.world.surface.tileSize;
    var pos = neighbourPos(oppositeDirection, tileSize, neighbourJObj.pos);

    return pos;
};

Map.prototype.calculateBoundingRect = function() {
    if (this.mapNodes.length == 0)
        return Qt.rect(0, 0, 0, 0);

    var initialPos = this.mapNodes[0].pos;

    var topLeft = Qt.point(initialPos.x, initialPos.y);
    var bottomRight = Qt.point(initialPos.x, initialPos.y);

    for (var i = 0; i < this.mapNodes.length; i++) {
        var pos = this.mapNodes[i].pos;

        topLeft.x = Math.min(pos.x, topLeft.x);
        bottomRight.x = Math.max(pos.x, bottomRight.x);

        topLeft.y = Math.min(pos.y, topLeft.y);
        bottomRight.y = Math.max(pos.y, bottomRight.y);
    }

    // x,y is the top-left corner of the mapNode so we need to add the tile size
    var tileSize = this.qobj.world.surface.tileSize;
    bottomRight.x += tileSize.width;
    bottomRight.y += tileSize.height;

    // leave a half-tile padding
    topLeft.x -= tileSize.width/2;
    topLeft.y -= tileSize.height/2;
    bottomRight.x += tileSize.width/2;
    bottomRight.y += tileSize.height/2;

    return Qt.rect(
        topLeft.x,
        topLeft.y,
        bottomRight.x - topLeft.x, 
        bottomRight.y - topLeft.y
    );
};

Map.prototype.adjustMapCoordinates = function() {
    if ((this.boundingRect.x == 0) && (this.boundingRect.y == 0))
        return; // nothing to do here

    var dx = this.boundingRect.x;
    var dy = this.boundingRect.y;

    this.boundingRect.x = 0;
    this.boundingRect.y = 0;

    for (var i = 0; i < this.mapNodes.length; i++) {
        var mapNode = this.mapNodes[i];
        mapNode.pos.x -= dx;
        mapNode.pos.y -= dy;
    }

    var rect = this.canvas.canvasWindow;
    this.canvasWindow = Qt.rect(
        rect.x + dx,
        rect.y + dy,
        rect.width,
        rect.height
    );
};

Map.prototype.updateGeometry = function() {
    this.boundingRect = this.calculateBoundingRect();
    this.size = Qt.size(
        this.boundingRect.width,
        this.boundingRect.height
    );

    this.canvas.canvasSize = Qt.size(
        Math.max(this.size.width, this.canvas.canvasWindow.width),
        Math.max(this.size.height, this.canvas.canvasWindow.height)
    );

    this.adjustMapCoordinates();
};

Map.prototype.translateToLocal = function(pos) {
    return Qt.point(
        pos.x + this.canvas.canvasWindow.x,
        pos.y + this.canvas.canvasWindow.y
    );
};

Map.prototype.onPressed = function(mouse) {
    this.lastMouseEvent = MouseEvents.pressed;
    this.lastMousePos = Qt.point(mouse.x, mouse.y);
};

Map.prototype.onReleased = function(mouse) {
    if (this.lastMouseEvent == MouseEvents.pressed) {
        var pos = this.translateToLocal(Qt.point(mouse.x, mouse.y));
        this.onClicked(pos);
    }
    else {
        this.mouseArea.cursorShape = Qt.ArrowCursor;
    }
};

Map.prototype.onPositionChanged = function(mouse) {
    this.lastMouseEvent = MouseEvents.positionChanged;

    var pos = Qt.point(mouse.x, mouse.y);
    var localPos = this.translateToLocal(pos);

    if (this.mouseArea.pressed) {
        var posDiff = Qt.size(
            this.lastMousePos.x - pos.x,
            this.lastMousePos.y - pos.y
        );
        this.mouseArea.cursorShape = Qt.ClosedHandCursor;
        this.onPanned(localPos, posDiff);
    }
    else {
        if (this.onHovered) this.onHovered(localPos);
    }

    this.lastMousePos = pos;
};

Map.prototype.onPaint = function(region) {
    if (!this.ready) return;
    if (this.geometryChanged) {
        this.updateGeometry();
        this.geometryChanged = false;
    }

    var ctx = this.canvas.getContext("2d");

    this.draw(ctx, region);
};

Map.prototype.toString = function() {
    var str = "[Map<" + this.qobj + ">]";
    return str;
};


/*
 * BigMap class
 * @contructor
 */
var BigMap = function(W, canvas, mouseArea) {
    Map.call(this, W, canvas, mouseArea);

    this.loadQueue = [];

    // init
    this.loadResources();
    this.canvas.requestPaint();
};

BigMap.prototype = Object.create(Map.prototype);
BigMap.prototype.constructor = EditableMap;

BigMap.prototype.newMapNode = function(pos, mapNodeQObj) {
    return new MapItem.MapNode(pos, mapNodeQObj,this);
};

BigMap.prototype.newSettlement = function(settlementQObj, mapNodeJObj) {
    return new MapItem.Settlement(settlementQObj, mapNodeJObj, this);
};

BigMap.prototype.newUnit = function(unitQObj, mapNodeJObj) {
    return new MapItem.Unit(unitQObj, mapNodeJObj, this);
};

BigMap.prototype.moveFocus = function(mapNode) {
    if (this.focusedNode !== mapNode) {
        if (mapNode) {
            if (this.focusedNode) this.focusedNode.onMouseOut();

            this.focusedNode = mapNode;
            this.focusedNode.onMouseIn();
        }
        else if (this.focusedNode) {
            this.focusedNode.onMouseOut();
            this.focusedNode = undefined;
        }
    }

    if (this.mapNodeFocused != undefined)
        this.mapNodeFocused(mapNode);
};

BigMap.prototype.onHovered = function(pos) {
   var mapNode = this.findMapNodeAt(pos);
   this.moveFocus(mapNode);
};

BigMap.prototype.onPanned = function(pos, posDiff) {
    var window = this.canvas.canvasWindow;
    var x = window.x + posDiff.width;
    var y = window.y + posDiff.height;

    this.moveWindowTo(Qt.point(x, y));
};

BigMap.prototype.onWindowPosChanged = function(windowPos) {
    this.moveWindowTo(windowPos);
};

BigMap.prototype.draw = function(ctx, region) {
    ctx.clearRect(
        region.x,
        region.y,
        region.width,
        region.height
    );

    //FIXME: only draw those mapNodes overlapping/contained in region
    var mapNodes = this.mapNodes;

    this.drawMapNodes(ctx, mapNodes);
    this.drawGrid(ctx, mapNodes);
    this.drawOverlay(ctx, mapNodes);
    this.drawContent(ctx, mapNodes);
};

BigMap.prototype.drawMapNodes = function(ctx, mapNodes) {
    for (var i = 0; i < mapNodes.length; i++) {
        var mapNode = mapNodes[i];
        mapNode.drawTerrain(ctx);
    }
};

BigMap.prototype.drawGrid = function(ctx, mapNodes) {
    for (var i = 0; i < mapNodes.length; i++) {
        var mapNode = mapNodes[i];
        mapNode.drawGrid(ctx);
    }
};

BigMap.prototype.drawOverlay = function(ctx, mapNodes) {
    for (var i = 0; i < mapNodes.length; i++) {
        var mapNode = mapNodes[i];
        mapNode.drawOverlay(ctx);
    }
};

BigMap.prototype.drawContent = function(ctx, mapNodes) {
    for (var i = 0; i < mapNodes.length; i++) {
        var mapNode = mapNodes[i];
        mapNode.drawContent(ctx);
    }
};

BigMap.prototype.moveWindowTo = function(pos) {
    var window = this.canvas.canvasWindow;
    var size = this.canvas.canvasSize;

    var maxX = size.width - window.width;
    var maxY = size.height - window.height;

    var x = pos.x;
    if (x < 0) x = 0;
    if (x > maxX) x = maxX;

    var y = pos.y;
    if (y < 0) y = 0;
    if (y > maxY) y = maxY;

    this.canvas.canvasWindow = Qt.rect(x, y, window.width, window.height);
    this.canvas.markDirty(this.canvas.canvasWindow);
};

BigMap.prototype.loadResources = function() {
    var surface = this.qobj.world.surface;
    var pathRoot = "images:";
    var path;

    for (var object in surface.bigMap) {
        var objectPath = surface.bigMap[object];
        path = pathRoot + objectPath;
        this.canvas.loadImage(path);
        this.loadQueue.push(path);
    }
};

BigMap.prototype.onResourceLoaded = function() {
    var image;
    var loaded = [];

    for (var i = 0; i < this.loadQueue.length; i++) {
        image = this.loadQueue[i];
        if (this.canvas.isImageError(image)) {
            console.error("Error loading image " + image);
            loaded.push(image);
        }

        if (this.canvas.isImageLoaded(image)) {
            loaded.push(image);
        }
    }

    for (var i = 0; i < loaded.length; i++) {
        var index = this.loadQueue.indexOf(loaded[i]);
        this.loadQueue.splice(index, 1);
    }

    if (this.loadQueue.length == 0) {
        console.info("All resources loaded");
        this.onResourcesLoaded();
    }
};

BigMap.prototype.onResourcesLoaded = function() {
    this.ready = true;
    this.canvas.requestPaint();
};

BigMap.prototype.toString = function() {
    var str = "[BigMap<" + this.qobj + ">]";
    return str;
};

/*
 * EditableMap class
 * @contructor
 */
var EditableMap = function(W, canvas, mouseArea) {
    BigMap.call(this, W, canvas, mouseArea);

    this.focusedNode = undefined;
    this.mapNodeClicked = undefined;
    this.mapNodeFocused = undefined;

    this.editMode = EditableMap.SelectMode;

    this.currentTerrainType = undefined;
    this.terrainTypeMap = {};
    this.currentSettlementType = undefined;
    this.settlementTypeMap = {};
    this.currentUnitType = undefined;
    this.unitTypeMap = {};

    this.onSelectMapItems = undefined;
    this.onEditMapNode = undefined;
    this.onEditSettlement = undefined;

    // init
    this.qobj.mapNodeAdded.connect(this.onMapNodeCreated.bind(this));

    this.terrainTypeMap = this.buildTypeMap(this.qobj.world.terrainTypes);
    this.settlementTypeMap = this.buildTypeMap(this.qobj.world.settlementTypes);
    this.unitTypeMap = this.buildTypeMap(this.qobj.world.unitTypes);

    this.geometryChanged = true;
};

EditableMap.prototype = Object.create(BigMap.prototype);
EditableMap.prototype.constructor = EditableMap;

EditableMap.SelectMode = "SelectMode";
EditableMap.CreateMapNodeMode = "CreateMapNodeMode";
EditableMap.CreateSettlementMode = "CreateSettlementMode";
EditableMap.CreateUnitMode = "CreateUnitMode";
EditableMap.EditMapNodeMode = "EditMapNodeMode";
EditableMap.EditSettlementMode = "EditSettlementMode";
EditableMap.EditUnitMode = "EditUnitMode";

EditableMap.prototype.buildTypeMap = function(typeList) {
    var typeMap = {};

    for (var i = 0; i < typeList.length; i++) {
        var type = typeList[i];
        typeMap[type.objectName] = type;
    }

    return typeMap;
};

EditableMap.prototype.getTilePos = function(pos) {
    var tileSize = this.qobj.world.surface.tileSize;
    
    // see if there any mapNodes nearby, and if yes, calculate the position
    // of the upperLeft corner of this tile
    var neighbour = undefined;
    var neighbourDirection = undefined;
    for (var i = 0; i < directions.length; i++) {
        var direction = directions[i];
        var npos = neighbourPos(direction, tileSize, pos);

        var mapNode = this.findMapNodeAt(npos);

        if (mapNode !== undefined) {
            neighbour = mapNode;
            neighbourDirection = direction;
            break;
        }
    }

    if (neighbour === undefined) return undefined;

    var direction = neighbour.qobj.oppositeDirection(neighbourDirection);
    return neighbourPos(direction, tileSize, neighbour.pos);
};

EditableMap.prototype.createNewMapNode = function(pos) {
    if (this.currentTerrainType == undefined) return;

    var tilePos = this.getTilePos(pos);
    if (tilePos == undefined) return;

    var neighbours = this.getTileNeighbours(tilePos);

    var neighboursMap = {};
    for (var direction in neighbours) {
        neighboursMap[direction] = neighbours[direction].qobj;
    }

    this.qobj.createMapNode(this.currentTerrainType, neighboursMap);
};

EditableMap.prototype.createNewSettlement = function(mapNodeJObj) {
    if (this.currentSettlementType == undefined) return;

    this.qobj.createSettlement(this.currentSettlementType, mapNodeJObj.qobj);
};

EditableMap.prototype.createNewUnit = function(mapNodeJObj) {
    if (this.currentUnitType == undefined) return;

    this.qobj.createUnit(this.currentUnitType, mapNodeJObj.qobj);
};

EditableMap.prototype.selectMapItems = function(mapNodeJObj, settlementJObj, unitJObj) {
    var mapNodeQObj = undefined;
    var settlementQObj = undefined;
    var unitQObj = undefined;

    if (mapNodeJObj) {
        mapNodeQObj = mapNodeJObj.qobj;
    }

    if (settlementJObj) {
        settlementQObj = settlementJObj.qobj;
    }

    if (unitJObj) {
        unitQObj = unitJObj.qobj;
    }

    if (this.onSelectMapItems) {
        this.onSelectMapItems(mapNodeQObj, settlementQObj, unitQObj);
    }
};

EditableMap.prototype.editMapNode = function(mapNodeJObj) {
    if (this.onEditMapNode) {
        this.onEditMapNode(mapNodeJObj.qobj);
    }
};

EditableMap.prototype.editSettlement = function(settlementJObj) {
    if (settlementJObj == undefined) return;

    if (this.onEditSettlement) {
        this.onEditSettlement(settlementJObj.qobj);
    }
};

EditableMap.prototype.editUnit = function(unitJObj) {
    if (unitJObj == undefined) return;

    if (this.onEditUnit) {
        this.onEditUnit(unitJObj.qobj);
    }
};

EditableMap.prototype.onHovered = function(pos) {
    var mapNode = this.findMapNodeAt(pos);
    this.moveFocus(mapNode);

    var tileSize = this.qobj.world.surface.tileSize;

    if (this.ghostHexagon != undefined)
        this.markTileDirty(this.ghostHexagon);

    if (mapNode == undefined) {
        this.ghostHexagon = this.getTilePos(pos);
    } else {
        this.ghostHexagon = undefined;
    }

    if (this.ghostHexagon != undefined)
        this.markTileDirty(this.ghostHexagon);
};

EditableMap.prototype.onClicked = function(pos) {
    var mapNode = this.findMapNodeAt(pos);
    var settlement = undefined;
    var unit = undefined;
    if (mapNode) {
        settlement = mapNode.settlement;
        unit = mapNode.unit;
    }

    if (this.editMode == EditableMap.SelectMode) {
        this.selectMapItems(mapNode, settlement, unit);
    } else if (this.editMode == EditableMap.CreateMapNodeMode) {
        this.createNewMapNode(pos);
    } else if (this.editMode == EditableMap.CreateSettlementMode) {
        this.createNewSettlement(mapNode);
    } else if (this.editMode == EditableMap.CreateUnitMode) {
        this.createNewUnit(mapNode);
    } else if (this.editMode == EditableMap.EditMapNodeMode) {
        this.editMapNode(mapNode);
    } else if (this.editMode == EditableMap.EditSettlementMode) {
        this.editSettlement(settlement);
    } else if (this.editMode == EditableMap.EditUnitMode) {
        this.editUnit(unit);
    } else {
        console.error("Uknown edit mode: " + this.editMode);
    }
};

EditableMap.prototype.onMapNodeCreated = function(mapNodeQObj) {
    var pos = this.calculatePosOfMapNodeQObj(mapNodeQObj);

    var mapNodeJObj = this.createMapNode(mapNodeQObj, pos);

    this.ghostHexagon = undefined;
    this.geometryChanged = true;
    this.canvas.requestPaint();
};

EditableMap.prototype.draw = function(ctx, region) {
    BigMap.prototype.draw.call(this, ctx, region);

    if (this.ghostHexagon == undefined) return;

    ctx.save();
    ctx.translate(this.ghostHexagon.x, this.ghostHexagon.y);

    var surface = this.qobj.world.surface;
    var tileSize = surface.tileSize;
    var focusedColor = surface.style["focusedBorder"];

    MapItem.drawHexagon(ctx, tileSize);

    ctx.fillStyle = focusedColor;

    ctx.fill();

    ctx.restore();
};

EditableMap.prototype.setEditMode = function(editMode) {
    this.editMode = editMode;
};

EditableMap.prototype.setTerrainType = function(terrainTypeName) {
    this.currentTerrainType = this.terrainTypeMap[terrainTypeName];
};

EditableMap.prototype.setSettlementType = function(settlementTypeName) {
    this.currentSettlementType = this.settlementTypeMap[settlementTypeName];
};

EditableMap.prototype.setUnitType = function(unitTypeName) {
    this.currentUnitType = this.unitTypeMap[unitTypeName];
};

EditableMap.prototype.toString = function() {
    var str = "[EditableMap<" + this.qobj + ">]";
    return str;
};


/*
 * MiniMap class
 * @contructor
 */
var MiniMap = function(W, canvas, mouseArea) {
    Map.call(this, W, canvas, mouseArea);

    this.pos = Qt.point(0, 0);
    this.window = Qt.rect(0, 0, 0, 0);
    this.scaleFactor = 1;
    this.windowPosChanged = undefined;

    // init
    this.qobj.mapNodeAdded.connect(this.onMapNodeCreated.bind(this));

    this.ready = true;
    this.canvas.requestPaint();
};

MiniMap.prototype = Object.create(Map.prototype);
MiniMap.prototype.constructor = MiniMap;

MiniMap.prototype.newMapNode = function(mapNodeQObj, pos) {
    return new MapItem.MiniMapNode(mapNodeQObj, pos, this);
};

MiniMap.prototype.newSettlement = function(settlementQObj, mapNodeJObj) {
    return new MapItem.MiniSettlement(settlementQObj, mapNodeJObj, this);
};

MiniMap.prototype.newUnit = function(unitQObj, mapNodeJObj) {
    return new MapItem.MiniUnit(unitQObj, mapNodeJObj, this);
};

MiniMap.prototype.calculateScaleFactor = function() {
    var normalSize = Math.max(this.size.width, this.size.height);
    var canvSize = Math.min(
        this.canvas.canvasSize.width,
        this.canvas.canvasSize.height
    );

    return canvSize/normalSize;
};

MiniMap.prototype.updateGeometry = function() {
    this.boundingRect = this.calculateBoundingRect();
    this.size = Qt.size(
        this.boundingRect.width,
        this.boundingRect.height
    );
    this.scaleFactor = this.calculateScaleFactor();

    this.adjustMapCoordinates();
};

MiniMap.prototype.draw = function(ctx, region) {
    ctx.clearRect(
        0,
        0,
        this.canvas.canvasSize.width,
        this.canvas.canvasSize.height
    );

    ctx.save();
    ctx.scale(this.scaleFactor, this.scaleFactor);

    for (var i = 0; i < this.mapNodes.length; i++) {
        this.mapNodes[i].draw(ctx);
    }

    ctx.beginPath();
    ctx.rect(
        this.window.x,
        this.window.y,
        this.window.width,
        this.window.height
    );
    ctx.strokeStyle = '#000000';
    ctx.lineWidth = 4;
    ctx.stroke();

    ctx.restore();
};

MiniMap.prototype.onPressed = function(mouse) {
    this.lastMouseEvent = MouseEvents.pressed;
    this.lastMousePos = Qt.point(mouse.x, mouse.y);

    var pos = this.translateToLocal(Qt.point(mouse.x, mouse.y));
    this.centerOn(pos);

    var windowPos = Qt.point(this.window.x, this.window.y);
    if (this.windowPosChanged) this.windowPosChanged(windowPos);
};

MiniMap.prototype.onClicked = function(pos) {
};

MiniMap.prototype.onPanned = function(pos, posDiff) {
    this.centerOn(pos);

    var windowPos = Qt.point(this.window.x, this.window.y);
    if (this.windowPosChanged) this.windowPosChanged(windowPos);
};

MiniMap.prototype.onMapNodeCreated = function(mapNodeQObj) {
    var pos = this.calculatePosOfMapNodeQObj(mapNodeQObj);
    var mapNodeJObj = this.createMapNode(mapNodeQObj, pos, this);

    this.geometryChanged = true;
    this.canvas.requestPaint();
};

MiniMap.prototype.setWindow = function(window) {
    this.window = window;
    this.canvas.requestPaint();
};

MiniMap.prototype.centerOn = function(pos) {
    var reverseScaleFactor = 1/this.scaleFactor;
    var mapPos = Qt.point(
        pos.x * reverseScaleFactor,
        pos.y * reverseScaleFactor
    );
    var x = mapPos.x - this.window.width/2;
    var y = mapPos.y - this.window.height/2;

    var maxX = this.size.width - this.window.width;
    var maxY = this.size.height - this.window.height;

    if (x < 0) x = 0;
    else if (x > maxX) x = maxX;

    if (y < 0) y = 0;
    else if (y > maxY) y = maxY;

    this.window = Qt.rect(x, y, this.window.width, this.window.height);

    this.canvas.requestPaint();
};

MiniMap.prototype.toString = function() {
    var str = "[MiniMap<" + this.qobj + ">]";
    return str;
};
