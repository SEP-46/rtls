'use strict';

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
    var clickme = document.getElementById("clickme");
	clickme.onclick = updateAnchors;	
}

window.onload = init;