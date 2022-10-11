'use strict';

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
    const jsonArr = JSON.parse(event.data)

    const tagPosX = parseFloat(jsonArr["pos"]["x"]).toFixed(3);
    const tagPosY = parseFloat(jsonArr["pos"]["y"]).toFixed(3);
    const tagPosZ = parseFloat(jsonArr["pos"]["z"]).toFixed(3);
    var TagPosOutput = document.getElementById("TagPosOutput");
    TagPosOutput.textContent = "{" + tagPosX + ", " + tagPosY + ", " + tagPosZ + "}";

    const tagVelX = parseFloat(jsonArr["vel"]["x"]).toFixed(3);
    const tagVelY = parseFloat(jsonArr["vel"]["y"]).toFixed(3);
    const tagVelZ = parseFloat(jsonArr["vel"]["z"]).toFixed(3);
    var TagPosOutput = document.getElementById("TagVelocityOutput");
    TagPosOutput.textContent = "{" + tagVelX + ", " + tagVelY + ", " + tagVelZ + "}";

    const timeNew = Date.now();
    const tagSpeedX = 1000*(tagPosX - CalcSpeed.xOld)/(timeNew - CalcSpeed.timeOld);
    const tagSpeedY = 1000*(tagPosY - CalcSpeed.yOld)/(timeNew - CalcSpeed.timeOld);
    const tagSpeedZ = 1000*(tagPosZ - CalcSpeed.zOld)/(timeNew - CalcSpeed.timeOld);
    CalcSpeed.xOld = tagPosX;
    CalcSpeed.yOld = tagPosY;
    CalcSpeed.zOld = tagPosZ;
    CalcSpeed.timeOld = timeNew;
    const tagSpeedMag = Math.sqrt((tagSpeedX*tagSpeedX)+(tagSpeedY*tagSpeedY)+(tagSpeedZ*tagSpeedZ)).toFixed(3);
    var TagSpeedOutput = document.getElementById("TagSpeedOutput");
    TagSpeedOutput.textContent = tagSpeedMag + " m/s";
}

class CalcSpeed
{
    constructor()
    {
        xOld = 0.000;
        yOld = 0.000;
        zOld = 0.000;
        timeOld = Date.now();
    }
}

async function fetchAsync(endpoint)
{
    let response = await fetch(endpoint, {mode: "no-cors"});
    let data = await response.json();
    return data;
}

async function updateAnchors()
{
    var nameA0 = document.getElementById("A0Name");
    var nameA1 = document.getElementById("A1Name");
    var nameA2 = document.getElementById("A2Name");

    var posA0 = document.getElementById("A0PosOutput");
    var posA1 = document.getElementById("A1PosOutput");
    var posA2 = document.getElementById("A2PosOutput");

    const anchors = await fetchAsync('/anchors');

    nameA0.textContent = anchors[0].name;
    nameA1.textContent = anchors[1].name;
    nameA2.textContent = anchors[2].name;

    posA0.textContent = "{" + anchors[0].pos.x + ", " + anchors[0].pos.y + ", " + anchors[0].pos.z + "}";
    posA1.textContent = "{" + anchors[1].pos.x + ", " + anchors[1].pos.y + ", " + anchors[1].pos.z + "}";
    posA2.textContent = "{" + anchors[2].pos.x + ", " + anchors[2].pos.y + ", " + anchors[2].pos.z + "}";
}

async function updateGrid()
{
    var lowBound = document.getElementById("LowBound");
    var upBound = document.getElementById("UpBound");

    const gridBounds = await fetchAsync('/bounds');

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
    var bUpdate = document.getElementById("bUpdate");
	bUpdate.onclick = updateInfo;	
}

window.onload = init;