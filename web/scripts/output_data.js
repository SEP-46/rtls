'use strict';

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
    if(timeForGridRefresh == 0)
    {
        drawGrid();
        timeForGridRefresh = 11;
    }
    timeForGridRefresh -= 1;

    const data = JSON.parse(event.data)

    const TagPosOutput = document.getElementById("TagPosOutput");
    TagPosOutput.textContent = "{" + data.pos.x.toFixed(3) + ", " + data.pos.y.toFixed(3) + ", " + data.pos.z.toFixed(3) + "}";
    drawTagLoc(data);

    const TagVelocityOutput = document.getElementById("TagVelocityOutput");
    TagVelocityOutput.textContent = "{" + data.vel.x.toFixed(3) + ", " + data.vel.y.toFixed(3) + ", " + data.vel.z.toFixed(3) + "}";

    const tagSpeedMag = Math.sqrt((data.vel.x*data.vel.x) + (data.vel.y*data.vel.y) + (data.vel.z*data.vel.z)).toFixed(3);
    const TagSpeedOutput = document.getElementById("TagSpeedOutput");
    TagSpeedOutput.textContent = tagSpeedMag + " m/s";
}

async function resetAnchors()
{
    const anchors = await get('/anchors');

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

    document.getElementById("BoundsMinX").value = gridBounds.mins.x;
    document.getElementById("BoundsMinY").value = gridBounds.mins.y;
    document.getElementById("BoundsMinZ").value = gridBounds.mins.z;
    document.getElementById("BoundsMaxX").value = gridBounds.maxs.x;
    document.getElementById("BoundsMaxY").value = gridBounds.maxs.y;
    document.getElementById("BoundsMaxZ").value = gridBounds.maxs.z;
}

function resetInfo()
{
    resetAnchors();
    resetGrid();
}

async function drawTagLoc(jsonArr)
{
    const gridBounds = await get('/bounds');
    let mapPosToGridHeight = bh/gridBounds.maxs.y;
    let mapPosToGridWidth = bh/gridBounds.maxs.y;
    
    let c = document.getElementById("myCanvas");
    let ctx = c.getContext("2d");
    ctx.fillStyle = "green";
    ctx.beginPath();
    ctx.arc
    (
        offsetGrid + mapPosToGridWidth*jsonArr["pos"]["x"],
        bh + offsetGrid - mapPosToGridHeight*jsonArr["pos"]["y"],
        3, 0, 2 * Math.PI
    );
    ctx.fill();

    //Ghost
    if (timeForGhostUpdate == 0)
    {
        for (let x = tagGhostNo; x > 0; x -= 1)
        {
            if (typeof tagGhostArr[x-1] !== 'undefined')
            {
                tagGhostArr[x] = tagGhostArr[x-1];
            }
        }
        tagGhostArr[0] = jsonArr;

        for (let x = 0; x <= tagGhostNo; x += 1)
        {
            if (typeof tagGhostArr[x] !== 'undefined')
            {
                ctx.beginPath();
                ctx.arc
                (
                    offsetGrid + mapPosToGridWidth*tagGhostArr[x]["pos"]["x"],
                    bh + offsetGrid - mapPosToGridHeight*tagGhostArr[x]["pos"]["y"],
                    3, 0, 2 * Math.PI
                );
                ctx.fill();
            }
        }

        timeForGhostUpdate = 11;
    }
    timeForGhostUpdate -= 1;
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

async function drawGrid() {
    let c = document.getElementById("myCanvas");
    let ctx = c.getContext("2d");

    ctx.beginPath();
    ctx.rect(0, 0, bw + 2 * offsetGrid, bh + 2 * offsetGrid);
    ctx.fillStyle = "black";
    ctx.fill();

    ctx.beginPath();
    ctx.rect(offsetGrid, offsetGrid, bw, bh);
    ctx.fillStyle = "white";
    ctx.fill();

    for (let x = offsetGrid; x <= bw + offsetGrid; x += 40) {
        ctx.moveTo(0.5 + x, offsetGrid);
        ctx.lineTo(0.5 + x, bh + offsetGrid);
    }

    for (let x = offsetGrid; x <= bh + offsetGrid; x += 40) {
        ctx.moveTo(offsetGrid, 0.5 + x);
        ctx.lineTo(offsetGrid + bw, 0.5 + x);
    }
    ctx.strokeStyle = "black";
    ctx.stroke();

    const anchors = await get('/anchors');
    const gridBounds = await get('/bounds');

    let mapPosToGridHeight = bh / gridBounds.maxs.y;
    let mapPosToGridWidth = bh / gridBounds.maxs.y;

    ctx.font = "10px Arial";
    ctx.fillStyle = "white";
    ctx.fillText(gridBounds.mins.x, offsetGrid, bw + (1.8 * offsetGrid));
    ctx.fillText(gridBounds.maxs.x, bw + offsetGrid, bw + (1.8 * offsetGrid));
    ctx.fillText(gridBounds.mins.y, (0.2 * offsetGrid), bh + offsetGrid);
    ctx.fillText(gridBounds.maxs.y, (0.2 * offsetGrid), 10 + (0.5 * offsetGrid));

    ctx.fillStyle = "red";
    for (let x = 0; x <= 2; x += 1) {
        ctx.beginPath();
        ctx.arc
            (
                offsetGrid + mapPosToGridWidth * anchors[x].pos.x,
                bh + offsetGrid - mapPosToGridHeight * anchors[x].pos.y,
                2, 0, 2 * Math.PI
            );
        ctx.fill();
    }

    ctx.font = "10px Arial";
    for (let x = 0; x <= 2; x += 1) {
        ctx.fillText
            (
                anchors[x].name,
                1.1 * offsetGrid + mapPosToGridWidth * anchors[x].pos.x,
                (bh + offsetGrid / 2) - (mapPosToGridHeight * anchors[x].pos.y)
            );
    }
}

async function updateInfo()
{
    await updateAnchors();
    await updateGrid();
    resetInfo();
}

function init()
{
    const reset = document.getElementById("ResetButton");
    reset.onclick = resetInfo;

    const update = document.getElementById("UpdateButton");
    update.onclick = updateInfo;

    resetInfo();
}

window.onload = init;

// HUD Settings
    // Counters
    let timeForGridRefresh = 0;
    let timeForGhostUpdate = 0;
    // Box width
    let bw = 400;
    // Box height
    let bh = 400;
    // Grid Offset (inside main white box)
    let offsetGrid = 25;
    // Tag Ghost Array
    let tagGhostNo = 5;
    let tagGhostArr = new Array(tagGhostNo);