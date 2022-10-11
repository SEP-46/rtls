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
    const data = JSON.parse(event.data)

    const TagPosOutput = document.getElementById("TagPosOutput");
    TagPosOutput.textContent = "{" + data.pos.x.toFixed(3) + ", " + data.pos.y.toFixed(3) + ", " + data.pos.z.toFixed(3) + "}";

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