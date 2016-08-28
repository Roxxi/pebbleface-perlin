module.exports = [
  { 
    "type": "heading", 
    "defaultValue": "Perlin Configuration" 
  }, 
  { 
    "type": "text", 
    "defaultValue": "Let's make it right for you" 
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Watchface Display"
      },
      {
        "type": "toggle",
        "messageKey": "showbatt",
        "label": "Show Battery",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "showdate",
        "label": "Show Date",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "randomwallpaper",
        "label": "Hourly Wallpaper Change",
        "defaultValue": true
      }
    ]
  },
  {  
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Watchface Alerts"
      },
      {
        "type": "toggle",
        "messageKey": "hourlyvibe",
        "label": "Hourly Bell",
        "defaultValue": false
      },
      {
        "type": "toggle",
        "messageKey": "bluetoothvibe",
        "label": "Vibe on Away",
        "description": "Watch vibrates when bluetooth connection is lost",
        "defaultValue": false
      },   
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }


];
