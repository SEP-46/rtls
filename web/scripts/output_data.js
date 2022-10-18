'use strict';

// Tag Ghost Array
let tagGhostArr = new Array(5);
const bw = 400;
const bh = 400;
const offsetGrid = 25;

async function get(endpoint)
{
    const response = await fetch(endpoint);
    return response.json();
}

async function put(endpoint, data)
{
    const response = await fetch(endpoint, {
        method: 'PUT',
        headers: {'Content-Type': 'application/json'},
        body: JSON.stringify(data)
    });
    return response.json();
}

let socket = new WebSocket("ws://localhost:9002");
socket.onopen = function()
{
    console.log('socket conected');
}
socket.onerror = function(error)
{
    alert(`[error] ${error.message}`);
}
socket.onclose = function()
{
    console.log('socket closed');
}
socket.onmessage = function(event)
{
    const data = JSON.parse(event.data)
    lastTagData = data;

    drawGrid();
    drawTagLoc(data);

    const TagPosOutput = document.getElementById("TagPosOutput");
    TagPosOutput.textContent = "{" + data.pos.x.toFixed(3) + ", " + data.pos.y.toFixed(3) + ", " + data.pos.z.toFixed(3) + "}";

    const TagVelocityOutput = document.getElementById("TagVelocityOutput");
    TagVelocityOutput.textContent = "{" + data.vel.x.toFixed(3) + ", " + data.vel.y.toFixed(3) + ", " + data.vel.z.toFixed(3) + "}";

    const tagSpeedMag = Math.sqrt((data.vel.x*data.vel.x) + (data.vel.y*data.vel.y) + (data.vel.z*data.vel.z)).toFixed(3);
    const TagSpeedOutput = document.getElementById("TagSpeedOutput");
    TagSpeedOutput.textContent = tagSpeedMag;
}

let lastTagData = undefined;
let lastAnchorData = [];
let lastBoundsData = {};

async function resetAnchors()
{
    const anchors = await get('/anchors');
    lastAnchorData = anchors;

    for (let i = 0; i < 3; i++)
    {
        const idInput = document.getElementById("A"+i+"Id"); // Hidden element for storing anchor ID
        const nameInput = document.getElementById("A"+i+"NameInput");
        const xInput = document.getElementById("A"+i+"XInput");
        const yInput = document.getElementById("A"+i+"YInput");
        const zInput = document.getElementById("A"+i+"ZInput");
        idInput.value = anchors[i].id;
        nameInput.value = anchors[i].name;
        xInput.value = anchors[i].pos.x;
        yInput.value = anchors[i].pos.y;
        zInput.value = anchors[i].pos.z;
    }
}

async function resetGrid()
{
    const gridBounds = await get('/bounds');
    lastBoundsData = gridBounds;

    document.getElementById("BoundsMinX").value = gridBounds.mins.x;
    document.getElementById("BoundsMinY").value = gridBounds.mins.y;
    document.getElementById("BoundsMinZ").value = gridBounds.mins.z;
    document.getElementById("BoundsMaxX").value = gridBounds.maxs.x;
    document.getElementById("BoundsMaxY").value = gridBounds.maxs.y;
    document.getElementById("BoundsMaxZ").value = gridBounds.maxs.z;
}

async function resetAlgorithm()
{
    const algorithm = await get('/algorithm');

    document.getElementById("Algorithm").value = algorithm.name;
}

function resetInfo()
{
    resetAnchors();
    resetGrid();
    resetAlgorithm();
}

// https://stackoverflow.com/questions/808826/draw-arrow-on-canvas-tag
function canvas_arrow(context, fromx, fromy, tox, toy) {
  const headlen = 10; // length of head in pixels
  const dx = tox - fromx;
  const dy = toy - fromy;
  const angle = Math.atan2(dy, dx);
  context.moveTo(fromx, fromy);
  context.lineTo(tox, toy);
  context.lineTo(tox - headlen * Math.cos(angle - Math.PI / 6), toy - headlen * Math.sin(angle - Math.PI / 6));
  context.moveTo(tox, toy);
  context.lineTo(tox - headlen * Math.cos(angle + Math.PI / 6), toy - headlen * Math.sin(angle + Math.PI / 6));
}

// https://stackoverflow.com/questions/1969240/mapping-a-range-of-values-to-another
function translate(value, leftMin, leftMax, rightMin, rightMax)
{
    // Figure out how 'wide' each range is
    const leftSpan = leftMax - leftMin;
    const rightSpan = rightMax - rightMin;

    // Convert the left range into a 0-1 range (float)
    const valueScaled = (value - leftMin) / leftSpan;

    // Convert the 0-1 range into a value in the right range.
    return rightMin + (valueScaled * rightSpan);
}

function worldSpaceToMapSpace(c, x, y) {
    const gridBounds = lastBoundsData;

    return {
        "x": translate(x, gridBounds.mins.x, gridBounds.maxs.x, offsetGrid, c.width - offsetGrid),
        "y": translate(y, gridBounds.mins.y, gridBounds.maxs.y, c.height - offsetGrid, offsetGrid)
    };
}

async function drawTagLoc(data)
{
    let c = document.getElementById("Visualisation");
    let ctx = c.getContext("2d");

    const tagMapPos = worldSpaceToMapSpace(c, data.pos.x, data.pos.y);

    ctx.fillStyle = "green";
    ctx.beginPath();
    ctx.arc(tagMapPos.x, tagMapPos.y, 3, 0, 2 * Math.PI);
    ctx.fill();

    // Velocity arrow
    if (data.vel.x != 0 || data.vel.y != 0)
    {
       const tagVelEndPos = worldSpaceToMapSpace(c, data.pos.x + data.vel.x * 0.3, data.pos.y + data.vel.y * 0.3);
       canvas_arrow(ctx, tagMapPos.x, tagMapPos.y, tagVelEndPos.x, tagVelEndPos.y);
       ctx.strokeStyle = "black";
       ctx.stroke();
    }

    //Ghost
    for (let i = 0; i < tagGhostArr.length; i += 1)
    {
        if (typeof tagGhostArr[i] !== 'undefined')
        {
            const ghostMapPos = worldSpaceToMapSpace(c, tagGhostArr[i].pos.x, tagGhostArr[i].pos.y);

            ctx.beginPath();
            ctx.arc(ghostMapPos.x, ghostMapPos.y, 2, 0, 2 * Math.PI);
            ctx.fill();
        }
    }
}

async function updateAnchors()
{
    for (let i = 0; i < 3; i++)
    {
        const idInput = document.getElementById("A"+i+"Id"); // Hidden element for storing anchor ID
        const nameInput = document.getElementById("A"+i+"NameInput");
        const xInput = document.getElementById("A"+i+"XInput");
        const yInput = document.getElementById("A"+i+"YInput");
        const zInput = document.getElementById("A"+i+"ZInput");

        const data = {
            "name": nameInput.value,
            "x": xInput.valueAsNumber,
            "y": yInput.valueAsNumber,
            "z": zInput.valueAsNumber
        };
        await put('/anchors/'+idInput.value, data);
    }
}

async function updateGrid()
{
    const data = {
        "mins": {
            "x": document.getElementById("BoundsMinX").valueAsNumber,
            "y": document.getElementById("BoundsMinY").valueAsNumber,
            "z": document.getElementById("BoundsMinZ").valueAsNumber
        },
        "maxs": {
            "x": document.getElementById("BoundsMaxX").valueAsNumber,
            "y": document.getElementById("BoundsMaxY").valueAsNumber,
            "z": document.getElementById("BoundsMaxZ").valueAsNumber
        }
    };
    await put('/bounds', data);
}

async function updateAlgorithm()
{
    const data = { "name": document.getElementById("Algorithm").value };
    await put('/algorithm', data);
}

function updateTagGhost()
{
    for (let i = tagGhostArr.length - 1; i > 0; i -= 1)
    {
        if (typeof tagGhostArr[i-1] !== 'undefined')
        {
            tagGhostArr[i] = tagGhostArr[i-1];
        }
    }
    tagGhostArr[0] = lastTagData;
}

async function drawGrid()
{
    let c = document.getElementById("Visualisation");
    let ctx = c.getContext("2d");

    ctx.beginPath();
    ctx.rect(0, 0, c.width, c.height);
    ctx.fillStyle = "black";
    ctx.fill();

    ctx.beginPath();
    ctx.rect(offsetGrid, offsetGrid, c.width - 2 * offsetGrid, c.height - 2 * offsetGrid);
    ctx.fillStyle = "white";
    ctx.fill();

    for (let x = offsetGrid; x <= c.width - offsetGrid; x += 40) {
        ctx.moveTo(0.5 + x, offsetGrid);
        ctx.lineTo(0.5 + x, c.height - offsetGrid);
    }

    for (let x = offsetGrid; x <= bh + offsetGrid; x += 40) {
        ctx.moveTo(offsetGrid, 0.5 + x);
        ctx.lineTo(c.width - offsetGrid, 0.5 + x);
    }
    ctx.strokeStyle = "black";
    ctx.stroke();

    const anchors = lastAnchorData;
    const gridBounds = lastBoundsData;

    let mapPosToGridHeight = bh / gridBounds.maxs.y;
    let mapPosToGridWidth = bh / gridBounds.maxs.y;

    ctx.font = "10px Arial";
    ctx.fillStyle = "white";
    ctx.fillText(gridBounds.mins.x, offsetGrid, c.height - 0.2 * offsetGrid);
    ctx.fillText(gridBounds.maxs.x, c.width - offsetGrid, c.height - 0.2 * offsetGrid);
    ctx.fillText(gridBounds.mins.y, 0.2 * offsetGrid, c.height - offsetGrid);
    ctx.fillText(gridBounds.maxs.y, 0.2 * offsetGrid, offsetGrid);

    ctx.fillStyle = "red";
    for (let i = 0; i <= 2; i += 1) {
        const anchorMapPos = worldSpaceToMapSpace(c, anchors[i].pos.x, anchors[i].pos.y);
        ctx.beginPath();
        ctx.arc(anchorMapPos.x, anchorMapPos.y, 2, 0, 2 * Math.PI);
        ctx.fill();
    }

    ctx.font = "10px Arial";
    for (let i = 0; i <= 2; i += 1) {
        const anchorMapPos = worldSpaceToMapSpace(c, anchors[i].pos.x, anchors[i].pos.y);
        ctx.fillText(anchors[i].name, 0.2 * offsetGrid + anchorMapPos.x, -0.4 * offsetGrid + anchorMapPos.y);
    }
}

async function updateInfo()
{
    await updateAnchors();
    await updateGrid();
    await updateAlgorithm();
    resetInfo();
}

function init()
{
    const reset = document.getElementById("ResetButton");
    reset.onclick = resetInfo;

    const update = document.getElementById("UpdateButton");
    update.onclick = updateInfo;

    resetInfo();

    setInterval(updateTagGhost, 500);
}

window.onload = init;