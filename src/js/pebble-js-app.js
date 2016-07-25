 var mConfig = {};

Pebble.addEventListener("ready", function(e) {
  loadLocalData();
  returnConfigToPebble();
});

Pebble.addEventListener("showConfiguration", function(e) {
	Pebble.openURL(mConfig.configureUrl);
});

Pebble.addEventListener("webviewclosed",
  function(e) {
    if (e.response) {
      var config = JSON.parse(e.response);
      saveLocalData(config);
      returnConfigToPebble();
    }
  }
);

function saveLocalData(config) {

  localStorage.setItem("showdate", parseInt(config.showdate)); 
  localStorage.setItem("bluetoothvibe", parseInt(config.bluetoothvibe)); 
  localStorage.setItem("hourlyvibe", parseInt(config.hourlyvibe)); 
  localStorage.setItem("showbatt", parseInt(config.showbatt)); 
  localStorage.setItem("randomtime", parseInt(config.randomtime)); 
  
  loadLocalData();

}
function loadLocalData() {
  
	mConfig.showdate = parseInt(localStorage.getItem("showdate"));
	mConfig.bluetoothvibe = parseInt(localStorage.getItem("bluetoothvibe"));
	mConfig.hourlyvibe = parseInt(localStorage.getItem("hourlyvibe"));
	mConfig.showbatt = parseInt(localStorage.getItem("showbatt"));
	mConfig.randomtime = parseInt(localStorage.getItem("randomtime"));
	mConfig.configureUrl = "http://www.themapman.com/pebblewatch/perlin2.html";


	if(isNaN(mConfig.showdate)) {
		mConfig.showdate = 0;
	}
	if(isNaN(mConfig.bluetoothvibe)) {
		mConfig.bluetoothvibe = 1;
	}
	if(isNaN(mConfig.hourlyvibe)) {
		mConfig.hourlyvibe = 0;
	}
	if(isNaN(mConfig.showbatt)) {
		mConfig.showbatt = 0;
	}
	if(isNaN(mConfig.randomtime)) {
		mConfig.randomtime = 0;
	}
}
function returnConfigToPebble() {
  Pebble.sendAppMessage({
    "showdate":parseInt(mConfig.showdate), 
    "bluetoothvibe":parseInt(mConfig.bluetoothvibe), 
    "hourlyvibe":parseInt(mConfig.hourlyvibe),
    "showbatt":parseInt(mConfig.showbatt),
    "randomtime":parseInt(mConfig.randomtime),
  });    
}