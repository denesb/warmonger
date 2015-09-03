.pragma library

function drawHexagon(ctx, tileSize) {
    var w = tileSize.width;
    var h = tileSize.height;

    var p0 = Qt.point(0, h/4);
    var p1 = Qt.point(w/2, 0);
    var p2 = Qt.point(w, h/4);
    var p3 = Qt.point(w, 3 * h/4);
    var p4 = Qt.point(w/2, h - 1);
    var p5 = Qt.point(0, 3 * h/4);

    ctx.beginPath();
    ctx.moveTo(p0.x, p0.y);
    ctx.lineTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.lineTo(p3.x, p3.y);
    ctx.lineTo(p4.x, p4.y);
    ctx.lineTo(p5.x, p5.y);
    ctx.closePath();
};

/*
 * MapNode class.
 * @contructor
 */
var MapNode = function(mapNodeQObj, pos, map) {
    this.pos = pos;
    this.map = map;
    this.qobj = mapNodeQObj;
    this.settlement = undefined;
    this.unit = undefined;

    this.focused = false;

    var surface = this.map.qobj.world.surface;
    var prefix = "images:";
    var terrainType = this.qobj.terrainType.objectName

    this.terrainImage = prefix + surface.bigMap[terrainType];
    this.blurredBorderColor = surface.style["blurredBorder"];
    this.focusedBorderColor = surface.style["focusedBorder"];

    // init
    this.qobj.terrainTypeChanged.connect(
        this.onTerrainTypeChanged.bind(this)
    );
};

MapNode.prototype.setSettlement = function(settlement) {
    this.settlement = settlement;
    this.map.markDirty(this);
};

MapNode.prototype.setUnit = function(unit) {
    this.unit = unit;
    this.map.markDirty(this);
};

MapNode.prototype.translate = function(point) {
    return Qt.point(point.x - this.pos.x, point.y - this.pos.y);
};

MapNode.prototype.beginPaint = function(ctx) {
    ctx.save();
    ctx.translate(this.pos.x, this.pos.y);
};

MapNode.prototype.endPaint = function(ctx) {
    ctx.restore();
};

MapNode.prototype.drawTerrain = function(ctx) {
    this.beginPaint(ctx);
    ctx.drawImage(this.terrainImage, 0, 0);
    this.endPaint(ctx);
};

MapNode.prototype.drawGrid = function(ctx) {
    this.beginPaint(ctx);

    var tileSize = this.map.qobj.world.surface.tileSize;

    drawHexagon(ctx, tileSize);

    ctx.lineWidth = 1;
    ctx.strokeStyle = this.blurredBorderColor;
    ctx.stroke();

    this.endPaint(ctx);
};

MapNode.prototype.drawOverlay = function(ctx) {
    if (!this.focused) return;

    this.beginPaint(ctx);

    var tileSize = this.map.qobj.world.surface.tileSize;

    drawHexagon(ctx, tileSize);

    ctx.lineWidth = 2;
    ctx.strokeStyle = this.focusedBorderColor;
    ctx.stroke();

    this.endPaint(ctx);
};

MapNode.prototype.drawContent = function(ctx) {
    this.beginPaint(ctx);

    if (this.settlement)
        this.settlement.draw(ctx);

    if (this.unit)
        this.unit.draw(ctx);

    this.endPaint(ctx);
};

MapNode.prototype.contains = function(point) {
    var localPoint = this.translate(point);

    return this.map.qobj.world.surface.hexContains(localPoint);
};

MapNode.prototype.onMouseIn = function() {
    this.focused = true;
    this.map.markDirty(this);
};

MapNode.prototype.onMouseOut = function() {
    this.focused = false;
    this.map.markDirty(this);
};

MapNode.prototype.onDisplayNameChanged = function() {
};

MapNode.prototype.onTerrainTypeChanged = function() {
    var surface = this.map.qobj.world.surface;
    var prefix = "images:";
    var terrainType = this.qobj.terrainType.objectName

    this.terrainImage = prefix + surface.bigMap[terrainType];

    this.map.markDirty(this);
};

MapNode.prototype.toString = function() {
    var str = "[MapNode(" + this.pos.x + "," + this.pos.y +
        "), qobj<" + this.qobj + ">]";
    return str;
};


/*
 * MiniMapNode class.
 * @contructor
 */
var MiniMapNode = function(mapNodeQObj, pos, map) {
    this.pos = pos;
    this.map = map;
    this.qobj = mapNodeQObj;
    this.settlement = undefined;
    this.unit = undefined;

    var surface = this.map.qobj.world.surface;
    this.color = surface.miniMap[this.qobj.terrainType.objectName];

    // init
    this.qobj.terrainTypeChanged.connect(
        this.onTerrainTypeChanged.bind(this)
    );

};

MiniMapNode.prototype.setSettlement = function(settlement) {
    this.settlement = settlement;
    this.map.markDirty(this);
};

MiniMapNode.prototype.setUnit = function(unit) {
    this.unit = unit;
    this.map.markDirty(this);
};

MiniMapNode.prototype.draw = function(ctx) {
    ctx.save();
    ctx.translate(this.pos.x, this.pos.y);

    var tileSize = this.map.qobj.world.surface.tileSize;

    drawHexagon(ctx, tileSize);

    ctx.lineWidth = 1;
    ctx.strokeStyle = this.color;
    ctx.fillStyle = this.color;

    //FIXME: figure out, why are there gaps between the hexagons
    ctx.stroke();
    ctx.fill();

    if (this.settlement) {
        this.settlement.draw(ctx);
    }

    if (this.unit) {
        this.unit.draw(ctx);
    }

    ctx.restore();
};

MiniMapNode.prototype.onTerrainTypeChanged = function() {
    var surface = this.map.qobj.world.surface;
    this.style = surface.miniMap[this.qobj.terrainType.objectName];

    this.map.markDirty(this);
};

MiniMapNode.prototype.toString = function() {
    var str = "[MiniMapNode(" + this.pos.x + "," + this.pos.y + "), qobj<" +
        this.qobj + ">]";
    return str;
};


/*
 * Settlement class.
 * @contructor
 */
var Settlement = function(settlementQObj, mapNodeJObj, map) {
    this.qobj = settlementQObj;
    this.mapNode = mapNodeJObj;
    this.map = map;

    var surface = this.map.qobj.world.surface;
    this.settlementImage = "images:" +
        surface.bigMap[this.qobj.settlementType.objectName];

    // init
    this.mapNode.setSettlement(this);

    this.qobj.settlementTypeChanged.connect(
        this.onSettlementTypeChanged.bind(this)
    );
    this.qobj.ownerChanged.connect(this.onOwnerChanged.bind(this));
};

Settlement.prototype.draw = function(ctx) {
    ctx.drawImage(this.settlementImage, 0, 0);
};

Settlement.prototype.onSettlementTypeChanged = function() {
    var surface = this.map.qobj.world.surface;
    this.settlementImage = "images:" +
        surface.bigMap[this.qobj.settlementType.objectName];

    this.map.markDirty(this.mapNode);
};

Settlement.prototype.onOwnerChanged = function() {
    this.map.markDirty(this.mapNode);
};

Settlement.prototype.toString = function() {
    var str = "[Settlement<" + this.qobj + "> on " +
        this.mapNode.toString() + " ]";
    return str;
};


/*
 * MiniSettlement class.
 * @contructor
 */
var MiniSettlement = function(settlementQObj, mapNodeJObj, map) {
    this.qobj = settlementQObj;
    this.mapNode = mapNodeJObj;
    this.map = map;

    // init
    this.mapNode.setSettlement(this);

    this.qobj.ownerChanged.connect(this.onOwnerChanged.bind(this));
};

MiniSettlement.prototype.onOwnerChanged = function() {
    this.map.markDirty(this.mapNode);
};

MiniSettlement.prototype.draw = function(ctx) {
    var tileSize = this.map.qobj.world.surface.tileSize;

    var w = tileSize.width;
    var h = tileSize.height;

    var size = w/2 - w/10;
    var x = w/20;
    var y = h/2 - size/2;

    ctx.fillStyle = this.qobj.owner.color;

    ctx.fillRect(x, y, size, size);
};

MiniSettlement.prototype.toString = function() {
    var str = "[MiniSettlement<" + this.qobj + "> on " +
        this.mapNode.toString() + " ]";
    return str;
};


/*
 * Unit class.
 * @contructor
 */
var Unit = function(unitQObj, mapNodeJObj, map) {
    this.qobj = unitQObj;
    this.mapNode = mapNodeJObj;
    this.map = map;

    var surface = this.map.qobj.world.surface;
    this.unitImage = "images:" +
        surface.bigMap[this.qobj.unitType.objectName];

    // init
    this.mapNode.setUnit(this);

    this.qobj.unitTypeChanged.connect(this.onUnitTypeChanged.bind(this));
    this.qobj.ownerChanged.connect(this.onOwnerChanged.bind(this));
};

Unit.prototype.draw = function(ctx) {
    ctx.drawImage(this.unitImage, 0, 0);
};

Unit.prototype.onUnitTypeChanged = function() {
    var surface = this.map.qobj.world.surface;
    this.unitImage = "images:" +
        surface.bigMap[this.qobj.unitType.objectName];

    this.map.markDirty(this.mapNode);
};

Unit.prototype.onOwnerChanged = function() {
    this.map.markDirty(this.mapNode);
};

Unit.prototype.toString = function() {
    var str = "[Unit<" + this.qobj + "> on " +
        this.mapNode.toString() + " ]";
    return str;
};


/*
 * MiniUnit class.
 * @contructor
 */
var MiniUnit = function(unitQObj, mapNodeJObj, map) {
    this.qobj = unitQObj;
    this.mapNode = mapNodeJObj;
    this.map = map;

    // init
    this.mapNode.setUnit(this);

    this.qobj.ownerChanged.connect(this.onOwnerChanged.bind(this));
};

MiniUnit.prototype.draw = function(ctx) {
    var tileSize = this.map.qobj.world.surface.tileSize;

    var w = tileSize.width;
    var h = tileSize.height;

    var size = w/4;
    var x = w/2 + w/5;
    var y = h/2 - size/2;

    ctx.fillStyle = this.qobj.owner.color;
    ctx.fillRect(x, y, size, size);
};

MiniUnit.prototype.onOwnerChanged = function() {
    this.map.markDirty(this.mapNode);
};

MiniUnit.prototype.toString = function() {
    var str = "[MiniUnit<" + this.qobj + "> on " +
        this.mapNode.toString() + " ]";
    return str;
};
