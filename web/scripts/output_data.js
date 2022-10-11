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

async function updateAnchors()
{
    const nameA0 = document.getElementById("A0Name");
    const nameA1 = document.getElementById("A1Name");
    const nameA2 = document.getElementById("A2Name");

    const posA0 = document.getElementById("A0PosOutput");
    const posA1 = document.getElementById("A1PosOutput");
    const posA2 = document.getElementById("A2PosOutput");

    const anchors = await get('/anchors');

    nameA0.textContent = anchors[0].name;
    nameA1.textContent = anchors[1].name;
    nameA2.textContent = anchors[2].name;

    posA0.textContent = "{" + anchors[0].pos.x + ", " + anchors[0].pos.y + ", " + anchors[0].pos.z + "}";
    posA1.textContent = "{" + anchors[1].pos.x + ", " + anchors[1].pos.y + ", " + anchors[1].pos.z + "}";
    posA2.textContent = "{" + anchors[2].pos.x + ", " + anchors[2].pos.y + ", " + anchors[2].pos.z + "}";
}

async function updateGrid()
{
    const lowBound = document.getElementById("LowBound");
    const upBound = document.getElementById("UpBound");

    const gridBounds = await get('/bounds');

    lowBound.textContent = "{" + gridBounds.mins.x + ", " + gridBounds.mins.y + ", " + gridBounds.mins.z + "}";
    upBound.textContent = "{" + gridBounds.maxs.x + ", " + gridBounds.maxs.y + ", " + gridBounds.maxs.z + "}";
}

function updateInfo()
{
    updateAnchors();
    updateGrid();
}

function init()
{
    const update = document.getElementById("update");
    update.onclick = updateInfo;
}

window.onload = init;