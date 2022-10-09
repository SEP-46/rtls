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
    let messageArr = event.data.split(',');

    const tagPosX = parseFloat(messageArr[0]).toFixed(3);
    const tagPosY = parseFloat(messageArr[1]).toFixed(3);
    const tagPosZ = parseFloat(messageArr[2]).toFixed(3);

    const timeNew = Date.now();
    const tagSpeedX = 1000*(tagPosX - CalcSpeed.xOld)/(timeNew - CalcSpeed.timeOld);
    const tagSpeedY = 1000*(tagPosY - CalcSpeed.yOld)/(timeNew - CalcSpeed.timeOld);
    const tagSpeedZ = 1000*(tagPosZ - CalcSpeed.zOld)/(timeNew - CalcSpeed.timeOld);
    CalcSpeed.xOld = tagPosX;
    CalcSpeed.yOld = tagPosY;
    CalcSpeed.zOld = tagPosZ;
    CalcSpeed.timeOld = timeNew;
    const tagSpeedMag = Math.sqrt((tagSpeedX*tagSpeedX)+(tagSpeedY*tagSpeedY)+(tagSpeedZ*tagSpeedZ)).toFixed(3);

    var TagPosOutput = document.getElementById("TagPosOutput");
    TagPosOutput.textContent = "{" + tagPosX + ", " + tagPosY + ", " + tagPosZ + "}";

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
    var A0 = document.getElementById("A0PosOutput");
    var A1 = document.getElementById("A1PosOutput");
    var A2 = document.getElementById("A2PosOutput");

    const anchors = await fetchAsync('/anchors');
    A0.textContent = "{" + anchors[0].pos.x + ", " + anchors[0].pos.y + ", " + anchors[0].pos.z + "}";
    A1.textContent = "{" + anchors[1].pos.x + ", " + anchors[1].pos.y + ", " + anchors[1].pos.z + "}";
    A2.textContent = "{" + anchors[2].pos.x + ", " + anchors[2].pos.y + ", " + anchors[2].pos.z + "}";
}

function init()
{
    var bUpdate = document.getElementById("bUpdate");
	bUpdate.onclick = updateAnchors;	
}

window.onload = init;