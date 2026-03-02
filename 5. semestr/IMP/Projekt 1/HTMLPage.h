#ifndef HTMLPAGE_H
#define HTMLPAGE_H

#include <Arduino.h>

class HTMLPage {
public:
    static const char* getHTMLContent();
};

const char* HTMLPage::getHTMLContent() {
    return R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<title>ESP32 Metronome Control</title>
<style>
  body {
    font-family: 'Arial', sans-serif;
    background-color: #f0f0f0;
    color: #333;
    text-align: center;
    padding: 20px;
  }

  h2 {
    color: #4CAF50;
    font-size: 2.5rem;
    margin-bottom: 20px;
  }

  button {
    background-color: #4CAF50;
    border: none;
    color: white;
    padding: 15px 32px;
    text-align: center;
    display: inline-block;
    font-size: 16px;
    margin: 10px;
    cursor: pointer;
    border-radius: 8px;
    transition: background-color 0.3s ease;
  }

  button:hover {
    background-color: #45a049;
  }

  input[type=range] {
    width: 100%;
    margin: 10px 0;
  }

  label {
    font-size: 1.2rem;
    margin-right: 10px;
  }

  select {
    padding: 10px;
    font-size: 1rem;
    border-radius: 5px;
    border: 1px solid #ccc;
    margin: 10px;
  }

  .dot {
    height: 60px;
    width: 60px;
    background-color: yellow;
    border-radius: 50%;
    display: inline-block;
    margin: 10px;
    cursor: pointer;
    transition: transform 0.2s ease, background-color 0.3s ease;
  }

  .dot.accent {
    background-color: orange;
  }

  .dot.silent {
    background-color: gray;
  }

  .dot-container {
    display: flex;
    justify-content: center;
    margin-top: 20px;
  }

  .tempo-control {
    display: flex;
    justify-content: center;
    align-items: center;
    gap: 20px;
    margin: 20px 0;
  }

  .tempo-buttons {
    display: flex;
    flex-direction: column;
    gap: 10px;
  }

  .volume-control, .tempo-slider {
    margin: 20px 0;
    width: 300px;
    margin-left: auto;
    margin-right: auto;
  }

  .tempo-slider-container {
    display: flex;
    align-items: center;
    justify-content: center;
  }

  .tempo-slider-container input[type=range] {
    width: 200px;
  }
</style>
</head>
<body>
<h2>ESP32 WiFi Metronome</h2>

<div class="container">
  <button onclick="startMetronome()">Start Metronome</button>
  <button onclick="stopMetronome()">Stop Metronome</button>

  <br><br>

  <div class="tempo-control">
    <div class="tempo-buttons">
      <button onclick="changeTempo(-1)">-1</button>
      <button onclick="changeTempo(-5)">-5</button>
    </div>

    <div class="tempo-slider-container">
      <label for="tempo">Tempo (BPM):</label>
      <span id="tempoValue">120</span> BPM
      <input type="range" id="tempoSlider" min="40" max="240" oninput="sliderSetTempo(this.value)" value="120">
    </div>

    <div class="tempo-buttons">
      <button onclick="changeTempo(1)">+1</button>
      <button onclick="changeTempo(5)">+5</button>
    </div>
  </div>

  <div class="volume-control">
    <label for="volume">Hlasitost:</label>
    <input type="range" id="volume" min="0" max="100" oninput="setVolume(this.value)" value="50">
    <span id="volumeValue">50</span> %
  </div>

  <label for="timeSignature">Beats/bar:</label>
  <select id="timeSignature" onchange="setTimeSignature(this.value)">
    <option value="1">1</option>
    <option value="2">2</option>
    <option value="3">3</option>
    <option value="4" selected>4</option>
    <option value="5">5</option>
    <option value="6">6</option>
    <option value="7">7</option>
    <option value="8">8</option>
  </select>

  <label for="clicksPerBeat">Clicks/beat:</label>
  <select id="clicksPerBeat" onchange="setClicksPerBeat(this.value)">
    <option value="1" selected>1</option>
    <option value="2">2</option>
    <option value="3">3</option>
    <option value="4">4</option>
  </select>
</div>

<div id="dotsContainer" class="dot-container"></div>

<script>
let currentTimeSignature = 4;
let currentClicksPerBeat = 1; // Výchozí počet kliků na jednu dobu
let currentTempo = 120; // Výchozí tempo
let metronomeRunning = false;
let dots = [];

function setTempo(tempo) {
  document.getElementById("tempoValue").textContent = tempo;
  document.getElementById("tempoSlider").value = tempo; // Synchronizace se sliderem
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/set-tempo?tempo=" + tempo, true);
  xhr.send();
}

function sliderSetTempo(tempo) {
  tempo = parseInt(tempo);
  currentTempo = tempo;
  setTempo(currentTempo); 
}

function setVolume(volume) {
  let volumePercent = volume;
  document.getElementById("volumeValue").textContent = volumePercent;

  let volumeValue = Math.round((volumePercent / 100) * 255); // Převod procent na hodnotu 0-255
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/set-volume?volume=" + volumeValue, true);
  xhr.send();
}

function changeTempo(change) {
  currentTempo = Math.max(40, Math.min(240, currentTempo + change));
  setTempo(currentTempo);
}

function setTimeSignature(timeSignature) {
  if (!metronomeRunning) {
    currentTimeSignature = parseInt(timeSignature);
    generateDots();

    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/set-time-signature?timeSignature=" + timeSignature, true);
    xhr.send();
  }
}

function setClicksPerBeat(clicksPerBeat) {
  if (!metronomeRunning) {
    currentClicksPerBeat = parseInt(clicksPerBeat);

    var xhr = new XMLHttpRequest();
    xhr.open("GET", "/set-clicks-per-beat?clicksPerBeat=" + clicksPerBeat, true);
    xhr.send();
  }
}

function generateDots() {
  const dotsContainer = document.getElementById("dotsContainer");
  dotsContainer.innerHTML = ""; 
  dots = [];

  for (let i = 0; i < currentTimeSignature; i++) {
    let dot = document.createElement("div");
    dot.classList.add("dot");
    dot.dataset.state = "normal"; 
    dot.dataset.index = i;

    dot.onclick = function() {
      toggleDotState(dot);
    };

    dots.push({ state: "normal", element: dot });
    dotsContainer.appendChild(dot);
  }
}

function toggleDotState(dot) {
  const index = dot.dataset.index;

  if (dots[index].state === "normal") {
    dots[index].state = "accent";
    dot.classList.add("accent");
    dot.classList.remove("silent");
  } else if (dots[index].state === "accent") {
    dots[index].state = "silent";
    dot.classList.add("silent");
    dot.classList.remove("accent");
  } else {
    dots[index].state = "normal";
    dot.classList.remove("silent");
    dot.classList.remove("accent");
  }

  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/set-beat?index=" + index + "&state=" + dots[index].state, true);
  xhr.send();
}

function startMetronome() {
  metronomeRunning = true;
  document.getElementById("timeSignature").disabled = true; 
  document.getElementById("clicksPerBeat").disabled = true;
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/start", true);
  xhr.send();
}

function stopMetronome() {
  metronomeRunning = false;
  document.getElementById("timeSignature").disabled = false; 
  document.getElementById("clicksPerBeat").disabled = false; 
  resetDots(); 
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/stop", true);
  xhr.send();
}

function resetDots() {
  dots.forEach(function(dot) {
    dot.state = "normal";
    dot.element.classList.remove("accent");
    dot.element.classList.remove("silent");
  });
}

window.onload = function() {
  generateDots();
};
</script>

</body>
</html>

)rawliteral";
}

#endif
