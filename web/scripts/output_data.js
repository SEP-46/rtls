'use strict';

async function get(endpoint)
{
    const response = await fetch(endpoint);
    const data = await response.json();
    return data;
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

    for (let i = 0; i < 3; i++) {
        const nameInput = document.getElementById("A"+i+"NameInput");
        const xInput = document.getElementById("A"+i+"XInput");
        const yInput = document.getElementById("A"+i+"YInput");
        const zInput = document.getElementById("A"+i+"ZInput");
        nameInput.value = anchors[i].name;
        xInput.value = anchors[i].pos.x;
        yInput.value = anchors[i].pos.y;
        zInput.value = anchors[i].pos.z;
    }
}

async function resetGrid()
{
    const gridBounds = await get('/bounds');

    const boundsMinXInput = document.getElementById("BoundsMinX");
    const boundsMaxXInput = document.getElementById("BoundsMaxX");
    const boundsMinYInput = document.getElementById("BoundsMinY");
    const boundsMaxYInput = document.getElementById("BoundsMaxY");
    const boundsMinZInput = document.getElementById("BoundsMinZ");
    const boundsMaxZInput = document.getElementById("BoundsMaxZ");
    boundsMinXInput.value = gridBounds.mins.x; boundsMinYInput.value = gridBounds.mins.y; boundsMinZInput.value = gridBounds.mins.z;
    boundsMaxXInput.value = gridBounds.maxs.x; boundsMaxYInput.value = gridBounds.maxs.y; boundsMaxZInput.value = gridBounds.maxs.z;
}

function resetInfo()
{
    resetAnchors();
    resetGrid();
}

function init()
{
    const reset = document.getElementById("ResetButton");
    reset.onclick = resetInfo;
    resetInfo();
}

window.onload = init;