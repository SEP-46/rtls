async function fetchAsync (url)
{
    let response = await fetch(url, {mode: "no-cors"});
    console.log(response);
    let data = await response.text();
    return data;
}

async function updateAnchors()
{
    var A0 = document.getElementById("A0PosOutput");
    // var A1 = document.getElementById("A1PosOutput");
    // var A2 = document.getElementById("A2PosOutput");

    A0.textContent = await fetchAsync('http://localhost/anchors');
    // A1.textContent = await fetchAsync('http://localhost/anchors');
    // A2.textContent = await fetchAsync('http://localhost/anchors');
}

function init()
{
    var clickme = document.getElementById("clickme");
	clickme.onclick = updateAnchors;	
}

window.onload = init;