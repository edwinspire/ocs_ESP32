<script>
  import { onMount } from "svelte/internal";
  var deviceSettings = {
    latitude: 0,
    longitude: 0,
    deviceId: "",
    input01: { name: "" },
    ssid: [{ssid: '', pwd: ''}, {ssid: '', pwd: ''}, {ssid: '', pwd: ''}]
  };

  function getLocation() {
    if (navigator.geolocation) {
      navigator.geolocation.getCurrentPosition((position) => {
        deviceSettings = {
          latitude: position.coords.latitude,
          longitude: position.coords.longitude,
        };
      });
    } else {
      x.innerHTML = "Geolocation is not supported by this browser.";
    }
  }

  async function getSettings() {
    alert("Se leerá la configuracion");

    let response = await fetch("/getsettings");
    deviceSettings = await response.json();

    console.log("Retorna settings", deviceSettings);
  }

  function setSettings() {
    alert("Se guardará la configuracion");
  }

  onMount(async () => {});
</script>

<div class="bg">
  <h1 style="color: darkcyan;">OPEN COMMUNITY SAFETY</h1>
  <div class="button_ali">
    <button class="button button1" on:click={getSettings}>Get settings</button>
    <button class="button button1" on:click={setSettings}>Save settings</button>
  </div>

  <h2>Settings</h2>
  <div>
    <label for="fname">Device ID</label>
    <input
      type="text"
      name="deviceId"
      maxlength="40"
      bind:value={deviceSettings.deviceId}
    />
  </div>

  <div>
    <label for="fname">Websocket Host</label>
    <input
      type="text"
      name="websockethost"
      bind:value={deviceSettings.websocketHost}
    />
  </div>

  <div>
    <label for="fname">Physical button label</label>
    <input
      type="text"
      name="pbl"
      maxlength="75"
      bind:value={deviceSettings.input01.name}
    />
  </div>

  <br />
  <div>
    <h3>WIFI</h3>
    <div class="flex-container">
      <div class="flex-item">
        <label for="fname">SSID 1</label>
        <input
          type="text"
          name="ssid1"
          bind:value={deviceSettings.ssid[0].ssid}
        />
        <label for="fname">SSID 2</label>
        <input
          type="text"
          name="ssid2"
          bind:value={deviceSettings.ssid[1].ssid}
        />
        <label for="fname">SSID 3</label>
        <input
          type="text"
          name="ssid3"
          bind:value={deviceSettings.ssid[2].ssid}
        />
      </div>

      <div class="flex-item">
        <label for="lname">Password 1</label>
        <input
          type="password"
          name="pwd1"
          bind:value={deviceSettings.ssid[0].pwd}
        />
        <label for="lname">Password 2</label>
        <input
          type="password"
          name="pwd2"
          bind:value={deviceSettings.ssid[1].pwd}
        />
        <label for="lname">Password 3</label>
        <input
          type="password"
          name="pwd3"
          bind:value={deviceSettings.ssid[2].pwd}
        />
      </div>
    </div>
  </div>
  <br />
  <div>
    <h3>Geolocation</h3>
    <div class="flex-container">
      <div class="flex-item">
        <label for="fname">Latitude</label>
        <input type="text" name="geox" bind:value={deviceSettings.latitude} />
      </div>

      <div class="flex-item">
        <label for="lname">Longitude</label>
        <input type="text" name="geoy" bind:value={deviceSettings.longitude} />
      </div>
    </div>
    <div class="button_ali">
      <button class="button button1" on:click={getLocation}
        >Get Geolocation</button
      >
    </div>
  </div>
</div>

<style>
  .flex-container {
    display: -webkit-flex;
    display: flex;
    -webkit-align-items: stretch;
    align-items: stretch;
    width: auto;
    height: auto;
  }

  .flex-item {
    width: 50%;
    margin: 10px;
  }
  input {
    width: 100%;
    padding: 12px 20px;
    margin: 8px 0;
    box-sizing: border-box;
    border: none;
    background-color: #3cbc8d;
    color: white;
  }
  .button {
    border: none;
    color: white;
    padding: 16px 32px;
    text-align: center;
    text-decoration: none;
    display: inline-block;
    font-size: 16px;
    margin: 4px 2px;
    transition-duration: 0.4s;
    cursor: pointer;
  }

  .button1 {
    background-color: white;
    color: black;
    border: 2px solid #3cbc8d;
  }

  .button1:hover {
    background-color: #3cbc8d;
    color: white;
  }

  .button_ali {
    text-align: end;
  }
</style>
