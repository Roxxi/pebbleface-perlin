module.exports = [
  { 
    "type": "heading", 
    "defaultValue": "Example Header Item" 
  }, 
  { 
    "type": "text", 
    "defaultValue": "Example text item." 
  },
  {
    "type": "section",
    "items": [
      {
        "type": "heading",
        "defaultValue": "Settings"
      },
      {
        "type": "toggle",
        "messageKey": "showdate",
        "label": "Show Date",
        "defaultValue": true
      },
      {
        "type": "toggle",
        "messageKey": "showbatt",
        "label": "Show Battery",
        "defaultValue": true
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
      {
        "type": "toggle",
        "messageKey": "randomwallpaper",
        "label": "Hourly Wallpaper Change",
        "defaultValue": true
      }
    ]
  },
  {
    "type": "submit",
    "defaultValue": "Save Settings"
  }


];
