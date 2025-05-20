#include "WebPages.h"

const char* index_html = R"index_html(
<!DOCTYPE HTML><html>
<head>
  <title>ESP Web Server</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <link rel="icon" href="data:,">
  <link rel="stylesheet" type="text/css" href="style.css">
  <link rel="stylesheet" href="https://use.fontawesome.com/releases/v5.7.2/css/all.css" integrity="sha384-fnmOCqbTlWIlj8LyTjo7mOUStjsKC4pOpQbqyi7RrhN7udi9RwhKkMHpvLbHG9Sr" crossorigin="anonymous">
  <script src="https://cdnjs.cloudflare.com/ajax/libs/three.js/107/three.min.js"></script>
</head>
<body>
  <div class="topnav">
    <h1> MPU6050 </h1>
  </div>
  <div class="content">
    <div class="cards">
      <div class="card">
        <p class="card-title">GYROSCOPE</p>
        <p><span class="reading">X: <span id="gyroX"></span> rad</span></p>
        <p><span class="reading">Y: <span id="gyroY"></span> rad</span></p>
        <p><span class="reading">Z: <span id="gyroZ"></span> rad</span></p>
      </div>
      <div class="card">
        <p class="card-title">ACCELEROMETER</p>
        <p><span class="reading">X: <span id="accX"></span> ms<sup>2</sup></span></p>
        <p><span class="reading">Y: <span id="accY"></span> ms<sup>2</sup></span></p>
        <p><span class="reading">Z: <span id="accZ"></span> ms<sup>2</sup></span></p>
      </div>
      <div class="card">
        <p class="card-title">TEMPERATURE</p>
        <p><span class="reading"><span id="temp"></span> &deg;C</span></p>
        <p class="card-title">3D ANIMATION</p>
        <button id="reset" onclick="resetPosition(this)">RESET POSITION</button>
        <button id="resetX" onclick="resetPosition(this)">X</button>
        <button id="resetY" onclick="resetPosition(this)">Y</button>
        <button id="resetZ" onclick="resetPosition(this)">Z</button>
      </div>
    </div>
    <div class="cube-content">
      <div id="3Dcube"></div>
    </div>
  </div>
<script src="script.js"></script>
</body>
</html>
)index_html";

const char* style_css = R"styles_css(
html {
  font-family: Arial;
  display: inline-block;
  text-align: center;
}
p {
  font-size: 1.2rem;
}
body {
  margin: 0;
}
.topnav {
  overflow: hidden;
  background-color: #003366;
  color: #FFD43B;
  font-size: 1rem;
}
.content {
  padding: 20px;
}
.card {
  background-color: white;
  box-shadow: 2px 2px 12px 1px rgba(140,140,140,.5);
}
.card-title {
  color:#003366;
  font-weight: bold;
}
.cards {
  max-width: 800px;
  margin: 0 auto;
  display: grid; grid-gap: 2rem;
  grid-template-columns: repeat(auto-fit, minmax(200px, 1fr));
}
.reading {
  font-size: 1.2rem;
}
.cube-content{
  width: 100%;
  background-color: white;
  height: 300px; margin: auto;
  padding-top:2%;
}
#reset{
  border: none;
  color: #FEFCFB;
  background-color: #003366;
  padding: 10px;
  text-align: center;
  display: inline-block;
  font-size: 14px; width: 150px;
  border-radius: 4px;
}
#resetX, #resetY, #resetZ{
  border: none;
  color: #FEFCFB;
  background-color: #003366;
  padding-top: 10px;
  padding-bottom: 10px;
  text-align: center;
  display: inline-block;
  font-size: 14px;
  width: 20px;
  border-radius: 4px;
}
)styles_css";

const char* script_js = R"script_js(
let scene, camera, rendered, cube;

function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}

function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}

function init3D(){
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xffffff);

  camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);

  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

  document.getElementById('3Dcube').appendChild(renderer.domElement);

  // Create a geometry
  const geometry = new THREE.BoxGeometry(5, 1, 4);

  // Materials of each face
  var cubeMaterials = [
    new THREE.MeshBasicMaterial({color:0x03045e}),
    new THREE.MeshBasicMaterial({color:0x023e8a}),
    new THREE.MeshBasicMaterial({color:0x0077b6}),
    new THREE.MeshBasicMaterial({color:0x03045e}),
    new THREE.MeshBasicMaterial({color:0x023e8a}),
    new THREE.MeshBasicMaterial({color:0x0077b6}),
  ];

  const material = new THREE.MeshFaceMaterial(cubeMaterials);

  cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  camera.position.z = 5;
  renderer.render(scene, camera);
}

// Resize the 3D object when the browser window changes size
function onWindowResize(){
  camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube"));
  //camera.aspect = window.innerWidth /  window.innerHeight;
  camera.updateProjectionMatrix();
  //renderer.setSize(window.innerWidth, window.innerHeight);
  renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

}

window.addEventListener('resize', onWindowResize, false);

// Create the 3D representation
init3D();

// Create events for the sensor readings
if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('gyro_readings', function(e) {
    //console.log("gyro_readings", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("gyroX").innerHTML = obj.gyroX;
    document.getElementById("gyroY").innerHTML = obj.gyroY;
    document.getElementById("gyroZ").innerHTML = obj.gyroZ;

    // Change cube rotation after receiving the readinds
    cube.rotation.x = obj.gyroY;
    cube.rotation.z = obj.gyroX;
    cube.rotation.y = obj.gyroZ;
    renderer.render(scene, camera);
  }, false);

  source.addEventListener('temperature_reading', function(e) {
    console.log("temperature_reading", e.data);
    document.getElementById("temp").innerHTML = e.data;
  }, false);

  source.addEventListener('accelerometer_readings', function(e) {
    console.log("accelerometer_readings", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("accX").innerHTML = obj.accX;
    document.getElementById("accY").innerHTML = obj.accY;
    document.getElementById("accZ").innerHTML = obj.accZ;
  }, false);
}

function resetPosition(element){
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/"+element.id, true);
  console.log(element.id);
  xhr.send();
}
)script_js";