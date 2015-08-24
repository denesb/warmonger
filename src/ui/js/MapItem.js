.pragma library

.import 'Util.js' as Util

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

    return this.map.ui.hexContains(localPoint);
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

/*
 * MapNode class.
 * @contructor
 */
var MapNode = function(pos, mapNodeQObj, map) {
    MapItem.call(this, pos, map);

    this.qobj = mapNodeQObj;
    this.terrainImage = this.map.qobj.world.getResourcePath(
        this.qobj.terrainType.objectName
    );
};

MapNode.prototype = Object.create(MapItem.prototype);
MapNode.prototype.constructor = MapNode;

MapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;

    ctx.save();

    ctx.translate(this.pos.x, this.pos.y)

    ctx.drawImage(this.terrainImage, 0, 0);
    if (this.focused)
        ctx.drawImage(worldQObj.getResourcePath("border_highlighted"), 0, 0);
    else
        ctx.drawImage(worldQObj.getResourcePath("border"), 0, 0);

    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.tileSize;

    ctx.strokeText(this.qobj.objectName, 10, tileSize.height/2);

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
    var str = "[MapNode(" + this.pos.x + "," + this.pos.y + "), qobj: "
        + this.qobj + "]";
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
    this.terrainTypeColors = {
        'plains': '#39661e',
        'forest': '#173504',
        'hills': '#cbce16',
        'mountains': '#ce7516'
    };
};

MiniMapNode.prototype = Object.create(MapItem.prototype);
MiniMapNode.prototype.constructor = MiniMapNode;

MiniMapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    var tileSize = worldQObj.tileSize;

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

    var style = this.terrainTypeColors[this.terrainType.objectName];

    ctx.lineWidth = 4;
    ctx.strokeStyle = style;
    ctx.fillStyle = style;

    //ctx.stroke();
    ctx.fill();

    ctx.restore();
};

MiniMapNode.prototype.toString = function() {
    var str = "[MiniMapNode(" + this.pos.x + "," + this.pos.y + "), qobj: "
        + this.qobj + "]";
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
};

PhantomMapNode.prototype = Object.create(MapItem.prototype);
PhantomMapNode.prototype.constructor = PhantomMapNode;

PhantomMapNode.prototype.onPaint = function(ctx) {
    var worldQObj = this.map.qobj.world;
    ctx.save();

    ctx.translate(this.pos.x, this.pos.y)

    if (this.focused)
        ctx.drawImage(worldQObj.getResourcePath("border_highlighted"), 0, 0);
    else
        ctx.drawImage(worldQObj.getResourcePath("border"), 0, 0);

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
