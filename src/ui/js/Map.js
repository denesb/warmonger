function onPaint() {
    var ctx = mapCanvas.getContext('2d');
    var mm = mapModel;

    console.log(mm.rowCount);
    console.log(mm.columnCount);

    ctx.beginPath();
    ctx.moveTo(100, 150);
    ctx.lineTo(450, 50);
    ctx.stroke();
}
