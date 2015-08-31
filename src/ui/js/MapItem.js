.pragma library

/*
 * MapItem class.
 * @contructor
 */
var MapItem = function(pos, map) {
    this.pos = pos;
    this.map = map;
    this.focused = false;
};

MapItem.prototype.onPaint = function(ctx) {
};

MapItem.prototype.translate = function(point) {
    return Qt.point(point.x - this.pos.x, point.y - this.pos.y);
};

MapItem.prototype.contains = function(point) {
    var localPoint = this.translate(point);

    return this.map.W.hexContains(localPoint);
};

MapItem.prototype.onMouseIn = function() {
    this.focused = true;
};

MapItem.prototype.onMouseOut = function() {
    this.focused = false;
};

MapItem.prototype.toString = function() {
    var str = "[MapItem(" + this.pos.x + "," + this.pos.y + ")]";
    return str;
};

function drawBorder(ctx, tileSize, color) {
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

    ctx.lineWidth = 1;
    ctx.strokeStyle = color;
    ctx.stroke();
};

/*
 * MapNode class.
 * @contructor
 */
var MapNode = function(pos, mapNodeQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = mapNodeQObj;

    var surface = this.map.qobj.world.surface;
    var prefix = "images:";
    var terrainType = this.qobj.terrainType.objectName

    this.terrainImage = prefix + surface.bigMap[terrainType];
    this.blurredBorderColor = surface.bigMap["blurredBorder"];
    this.focusedBorderColor = surface.bigMap["focusedBorder"];
};

MapNode.prototype = Object.create(MapItem.prototype);
MapNode.prototype.constructor = MapNode;

MapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.surface.tileSize;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    ctx.drawImage(this.terrainImage, 0, 0);
    if (this.focused)
        drawBorder(ctx, tileSize, this.focusedBorderColor);

    ctx.restore();
};

MapNode.prototype.onMouseIn = function() {
    this.focused = true;
    this.map.markDirty(this);
};

MapNode.prototype.onMouseOut = function() {
    this.focused = false;
    this.map.markDirty(this);
};

MapNode.prototype.toString = function() {
    var str = "[MapNode(" + this.pos.x + "," + this.pos.y +
        "), qobj<" + this.qobj + ">]";
    return str;
};

/*
 * EditableMapNode class.
 * @contructor
 */
var EditableMapNode = function(pos, mapNodeQObj, map) {
    MapNode.call(this, pos, mapNodeQObj, map);

    this.qobj.displayNameChanged.connect(
        this.onDisplayNameChanged.bind(this)
    );
    this.qobj.terrainTypeChanged.connect(
        this.onTerrainTypeChanged.bind(this)
    );
};

EditableMapNode.prototype = Object.create(MapNode.prototype);
EditableMapNode.prototype.constructor = EditableMapNode;

EditableMapNode.prototype.onDisplayNameChanged = function() {
};

EditableMapNode.prototype.onTerrainTypeChanged = function() {
    var surface = this.map.qobj.world.surface;
    var prefix = "images:";
    var terrainType = this.qobj.terrainType.objectName

    this.terrainImage = prefix + surface.bigMap[terrainType];

    this.map.markDirty(this);
};

EditableMapNode.prototype.toString = function() {
    var str = "[EditableMapNode(" + this.pos.x + "," + this.pos.y +
        "), qobj<" + this.qobj + ">]";
    return str;
};


/*
 * MiniMapNode class.
 * @contructor
 */
var MiniMapNode = function(pos, mapNodeQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = mapNodeQObj;
    this.terrainType = this.qobj.terrainType;

    var surface = this.map.qobj.world.surface;
    this.style = surface.miniMap[this.terrainType.objectName];
};

MiniMapNode.prototype = Object.create(MapItem.prototype);
MiniMapNode.prototype.constructor = MiniMapNode;

MiniMapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.surface.tileSize;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    var w = tileSize.width;
    var h = tileSize.height;

    var p0 = Qt.point(-1, h/4 - 1);
    var p1 = Qt.point(w/2 - 1, -1);
    var p2 = Qt.point(w, h/4 - 1);
    var p3 = Qt.point(w, 3 * h/4 - 1);
    var p4 = Qt.point(w/2 - 1, h - 1);
    var p5 = Qt.point(-1, 3 * h/4 - 1);

    ctx.beginPath();
    ctx.moveTo(p0.x, p0.y);
    ctx.lineTo(p1.x, p1.y);
    ctx.lineTo(p2.x, p2.y);
    ctx.lineTo(p3.x, p3.y);
    ctx.lineTo(p4.x, p4.y);
    ctx.lineTo(p5.x, p5.y);
    ctx.closePath();

    var style = this.style;

    ctx.lineWidth = 4;
    ctx.strokeStyle = style;
    ctx.fillStyle = style;

    //ctx.stroke();
    ctx.fill();

    ctx.restore();
};

MiniMapNode.prototype.toString = function() {
    var str = "[MiniMapNode(" + this.pos.x + "," + this.pos.y + "), qobj<" +
        this.qobj + ">]";
    return str;
};

/*
 * PhantomMapNode class
 * @contructor
 */
var PhantomMapNode = function(pos, map) {
    MapItem.call(this, pos, map);

    this.neighbours = {};
    this.isPhantom = true;

    var surface = this.map.qobj.world.surface;
    var prefix = "images:";

    this.blurredBorderColor = surface.bigMap["blurredBorder"];
    this.focusedBorderColor = surface.bigMap["focusedBorder"];
};

PhantomMapNode.prototype = Object.create(MapItem.prototype);
PhantomMapNode.prototype.constructor = PhantomMapNode;

PhantomMapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.surface.tileSize;
    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    if (this.focused)
        drawBorder(ctx, tileSize, this.focusedBorderColor);

    ctx.restore();
};

PhantomMapNode.prototype.onMouseIn = function() {
    this.focused = true;
    this.map.markDirty(this);
};

PhantomMapNode.prototype.onMouseOut = function() {
    this.focused = false;
    this.map.markDirty(this);
};

PhantomMapNode.prototype.toString = function() {
    var str = "[PhantomMapNode(" + this.pos.x + "," + this.pos.y + ")]";
    return str;
};


/*
 * Settlement class.
 * @contructor
 */
var Settlement = function(pos, settlementQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = settlementQObj;
    this.settlementType = this.qobj.settlementType;

    var surface = this.map.qobj.world.surface;
    this.settlementImage = "images:" +
        surface.bigMap[this.settlementType.objectName];
};

Settlement.prototype = Object.create(MapItem.prototype);
Settlement.prototype.constructor = Settlement;

Settlement.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    ctx.drawImage(this.settlementImage, 0, 0);

    ctx.restore();
};

Settlement.prototype.toString = function() {
    var str = "[Settlement(" + this.pos.x + "," + this.pos.y + "), qobj<" +
        this.qobj + "> on mapNode " + this.qobj.mapNode + "]";
    return str;
};


/*
 * MiniSettlement class.
 * @contructor
 */
var MiniSettlement = function(pos, settlementQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = settlementQObj;

    // init
    this.qobj.ownerChanged.connect(this.onOwnerChanged.bind(this));
};

MiniSettlement.prototype = Object.create(MapItem.prototype);
MiniSettlement.prototype.constructor = MiniSettlement;

MiniSettlement.prototype.onOwnerChanged = function() {
    this.map.markDirty(this);
};

MiniSettlement.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.surface.tileSize;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    var w = tileSize.width;
    var h = tileSize.height;

    var size = w/2 - w/10;
    var x = w/20;
    var y = h/2 - size/2;

    ctx.fillStyle = this.qobj.owner.color;

    ctx.fillRect(x, y, size, size);

    ctx.restore();
};

MiniSettlement.prototype.toString = function() {
    var str = "[MiniSettlement(" + this.pos.x + "," + this.pos.y + "), qobj<" +
        this.qobj + "> on mapNode " + this.qobj.mapNode + "]";
    return str;
};


/*
 * Unit class.
 * @contructor
 */
var Unit = function(pos, unitQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = unitQObj;
    this.unitType = this.qobj.unitType;

    var surface = this.map.qobj.world.surface;
    this.unitImage = "images:" + surface.bigMap[this.unitType.objectName];

    // init
    this.qobj.unitTypeChanged.connect(this.onUnitTypeChanged.bind(this));
};

Unit.prototype = Object.create(MapItem.prototype);
Unit.prototype.constructor = Unit;

Unit.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    ctx.drawImage(this.unitImage, 0, 0);

    ctx.restore();
};

Unit.prototype.onUnitTypeChanged = function() {
    this.unitType = this.qobj.unitType;

    var surface = this.map.qobj.world.surface;
    this.unitImage = "images:" + surface.bigMap[this.unitType.objectName];

    this.map.markDirty(this);
};

Unit.prototype.toString = function() {
    var str = "[Unit(" + this.pos.x + "," + this.pos.y + "), qobj<" +
        this.qobj + "> on mapNode " + this.qobj.mapNode + "]";
    return str;
};


/*
 * MiniUnit class.
 * @contructor
 */
var MiniUnit = function(pos, unitQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = unitQObj;

    // init
    this.qobj.ownerChanged.connect(this.onOwnerChanged.bind(this));
};

MiniUnit.prototype = Object.create(MapItem.prototype);
MiniUnit.prototype.constructor = MiniUnit;

MiniUnit.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.surface.tileSize;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y);

    var w = tileSize.width;
    var h = tileSize.height;

    var size = w/4;
    var x = w/2 + w/5;
    var y = h/2 - size/2;

    ctx.fillStyle = this.qobj.owner.color;

    ctx.fillRect(x, y, size, size);

    ctx.restore();
};

MiniUnit.prototype.onOwnerChanged = function() {
    this.map.markDirty(this);
};

MiniUnit.prototype.toString = function() {
    var str = "[MiniUnit(" + this.pos.x + "," + this.pos.y + "), qobj<" +
        this.qobj + "> on mapNode " + this.qobj.mapNode + "]";
    return str;
};
