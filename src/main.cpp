#include <WiFiMulti.h>
#include <ArduinoWebsockets.h>
#include <ArduinoJson.h>
#include <Preferences.h>
#include <WiFi.h>

//#include <../include/wifi.cpp>
#include <../lib/ocs_arduino_library/src/opencommunitysafety.cpp>

using namespace websockets;
using namespace ocs;

WiFiServer server(80);
// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;
// Variable to store the HTTP request
String header;

Preferences pref;
String deviceId = "";

ocs::OpenCommunitySafety ocsClass;
const int gpio_in_01 = 32;
const int gpio_out_01 = 21;
const String websockets_server_host = "wss://open-community-safety.herokuapp.com/ws/device";

String responseRoot()
{
    return R"(<!DOCTYPE html>
<html lang="en">
  <head>
    <meta charset="utf-8" />
    <meta name="viewport" content="width=device-width,initial-scale=1" />
    <title>Open Community Safety</title>
    <link rel="stylesheet" href="build/bundle.css" />
    <script defer src="build/bundle.js"></script>
  </head>
  <body></body>
</html>)";
}

String responsebundleJS()
{
    return R"(var app=function(){"use strict";function t(){}function e(t){return t()}function n(){return Object.create(null)}function s(t){t.forEach(e)}function o(t){return"function"==typeof t}function i(t,e){return t!=t?e==e:t!==e||t&&"object"==typeof t||"function"==typeof t}function a(t,e){t.appendChild(e)}function l(t){t.parentNode.removeChild(t)}function u(t){return document.createElement(t)}function c(){return t=" ",document.createTextNode(t);var t}function d(t,e,n,s){return t.addEventListener(e,n,s),()=>t.removeEventListener(e,n,s)}function r(t,e,n){null==n?t.removeAttribute(e):t.getAttribute(e)!==n&&t.setAttribute(e,n)}function p(t,e){t.value=null==e?"":e}let f;function h(t){f=t}function v(t){(function(){if(!f)throw new Error("Function called outside component initialization");return f})().$$.on_mount.push(t)}const m=[],g=[],b=[],w=[],y=Promise.resolve();let $=!1;function C(t){b.push(t)}const _=new Set;let k=0;function I(){const t=f;do{for(;k<m.length;){const t=m[k];k++,h(t),S(t.$$)}for(h(null),m.length=0,k=0;g.length;)g.pop()();for(let t=0;t<b.length;t+=1){const e=b[t];_.has(e)||(_.add(e),e())}b.length=0}while(m.length);for(;w.length;)w.pop()();$=!1,_.clear(),h(t)}function S(t){if(null!==t.fragment){t.update(),s(t.before_update);const e=t.dirty;t.dirty=[-1],t.fragment&&t.fragment.p(t.ctx,e),t.after_update.forEach(C)}}const E=new Set;function P(t,e){-1===t.$$.dirty[0]&&(m.push(t),$||($=!0,y.then(I)),t.$$.dirty.fill(0)),t.$$.dirty[e/31|0]|=1<<e%31}function H(i,a,u,c,d,r,p,v=[-1]){const m=f;h(i);const g=i.$$={fragment:null,ctx:null,props:r,update:t,not_equal:d,bound:n(),on_mount:[],on_destroy:[],on_disconnect:[],before_update:[],after_update:[],context:new Map(a.context||(m?m.$$.context:[])),callbacks:n(),dirty:v,skip_bound:!1,root:a.target||m.$$.root};p&&p(g.root);let b=!1;if(g.ctx=u?u(i,a.props||{},((t,e,...n)=>{const s=n.length?n[0]:e;return g.ctx&&d(g.ctx[t],g.ctx[t]=s)&&(!g.skip_bound&&g.bound[t]&&g.bound[t](s),b&&P(i,t)),e})):[],g.update(),b=!0,s(g.before_update),g.fragment=!!c&&c(g.ctx),a.target){if(a.hydrate){const t=function(t){return Array.from(t.childNodes)}(a.target);g.fragment&&g.fragment.l(t),t.forEach(l)}else g.fragment&&g.fragment.c();a.intro&&((x=i.$$.fragment)&&x.i&&(E.delete(x),x.i(w))),function(t,n,i,a){const{fragment:l,on_mount:u,on_destroy:c,after_update:d}=t.$$;l&&l.m(n,i),a||C((()=>{const n=u.map(e).filter(o);c?c.push(...n):s(n),t.$$.on_mount=[]})),d.forEach(C)}(i,a.target,a.anchor,a.customElement),I()}var x,w;h(m)}function A(e){let n,o,i,f,h,v,m,g,b,x,w,y,$,C,_,k,I,S,E,P,H,A,G,L,N,O,j,M,T,F,W,Y,q,z,B,R,U,J,K,Q,V,X,Z,tt,et,nt,st,ot,it,at,lt,ut,ct,dt,rt,pt,ft,ht,vt,mt,gt,bt,xt,wt,yt,$t,Ct,_t,kt,It,St,Et,Pt,Ht,At,Dt,Gt;return{c(){var t,e,s,a;n=u("div"),o=u("h1"),o.textContent="OPEN COMMUNITY SAFETY",i=c(),f=u("div"),h=u("button"),h.textContent="Get settings",v=c(),m=u("button"),m.textContent="Save settings",g=c(),b=u("h2"),b.textContent="Settings",x=c(),w=u("div"),y=u("label"),y.textContent="Device ID",$=c(),C=u("input"),_=c(),k=u("div"),I=u("label"),I.textContent="Websocket Host",S=c(),E=u("input"),P=c(),H=u("div"),A=u("label"),A.textContent="Physical button label",G=c(),L=u("input"),N=c(),O=u("br"),j=c(),M=u("div"),T=u("h3"),T.textContent="WIFI",F=c(),W=u("div"),Y=u("div"),q=u("label"),q.textContent="SSID 1",z=c(),B=u("input"),R=c(),U=u("label"),U.textContent="SSID 2",J=c(),K=u("input"),Q=c(),V=u("label"),V.textContent="SSID 3",X=c(),Z=u("input"),tt=c(),et=u("div"),nt=u("label"),nt.textContent="Password 1",st=c(),ot=u("input"),it=c(),at=u("label"),at.textContent="Password 2",lt=c(),ut=u("input"),ct=c(),dt=u("label"),dt.textContent="Password 3",rt=c(),pt=u("input"),ft=c(),ht=u("br"),vt=c(),mt=u("div"),gt=u("h3"),gt.textContent="Geolocation",bt=c(),xt=u("div"),wt=u("div"),yt=u("label"),yt.textContent="Latitude",$t=c(),Ct=u("input"),_t=c(),kt=u("div"),It=u("label"),It.textContent="Longitude",St=c(),Et=u("input"),Pt=c(),Ht=u("div"),At=u("button"),At.textContent="Get Geolocation",t=o,e="color",null===(s="darkcyan")?t.style.removeProperty(e):t.style.setProperty(e,s,a?"important":""),r(h,"class","button button1 svelte-10a4hhp"),r(m,"class","button button1 svelte-10a4hhp"),r(f,"class","button_ali svelte-10a4hhp"),r(y,"for","fname"),r(C,"type","text"),r(C,"name","deviceId"),r(C,"maxlength","40"),r(C,"class","svelte-10a4hhp"),r(I,"for","fname"),r(E,"type","text"),r(E,"name","websockethost"),r(E,"class","svelte-10a4hhp"),r(A,"for","fname"),r(L,"type","text"),r(L,"name","pbl"),r(L,"maxlength","75"),r(L,"class","svelte-10a4hhp"),r(q,"for","fname"),r(B,"type","text"),r(B,"name","ssid1"),r(B,"class","svelte-10a4hhp"),r(U,"for","fname"),r(K,"type","text"),r(K,"name","ssid2"),r(K,"class","svelte-10a4hhp"),r(V,"for","fname"),r(Z,"type","text"),r(Z,"name","ssid3"),r(Z,"class","svelte-10a4hhp"),r(Y,"class","flex-item svelte-10a4hhp"),r(nt,"for","lname"),r(ot,"type","password"),r(ot,"name","pwd1"),r(ot,"class","svelte-10a4hhp"),r(at,"for","lname"),r(ut,"type","password"),r(ut,"name","pwd2"),r(ut,"class","svelte-10a4hhp"),r(dt,"for","lname"),r(pt,"type","password"),r(pt,"name","pwd3"),r(pt,"class","svelte-10a4hhp"),r(et,"class","flex-item svelte-10a4hhp"),r(W,"class","flex-container svelte-10a4hhp"),r(yt,"for","fname"),r(Ct,"type","text"),r(Ct,"name","geox"),r(Ct,"class","svelte-10a4hhp"),r(wt,"class","flex-item svelte-10a4hhp"),r(It,"for","lname"),r(Et,"type","text"),r(Et,"name","geoy"),r(Et,"class","svelte-10a4hhp"),r(kt,"class","flex-item svelte-10a4hhp"),r(xt,"class","flex-container svelte-10a4hhp"),r(At,"class","button button1 svelte-10a4hhp"),r(Ht,"class","button_ali svelte-10a4hhp"),r(n,"class","bg")},m(t,s){!function(t,e,n){t.insertBefore(e,n||null)}(t,n,s),a(n,o),a(n,i),a(n,f),a(f,h),a(f,v),a(f,m),a(n,g),a(n,b),a(n,x),a(n,w),a(w,y),a(w,$),a(w,C),p(C,e[0].deviceId),a(n,_),a(n,k),a(k,I),a(k,S),a(k,E),p(E,e[0].websocketHost),a(n,P),a(n,H),a(H,A),a(H,G),a(H,L),p(L,e[0].input01.name),a(n,N),a(n,O),a(n,j),a(n,M),a(M,T),a(M,F),a(M,W),a(W,Y),a(Y,q),a(Y,z),a(Y,B),p(B,e[0].ssid[0].ssid),a(Y,R),a(Y,U),a(Y,J),a(Y,K),p(K,e[0].ssid[1].ssid),a(Y,Q),a(Y,V),a(Y,X),a(Y,Z),p(Z,e[0].ssid[2].ssid),a(W,tt),a(W,et),a(et,nt),a(et,st),a(et,ot),p(ot,e[0].ssid[0].pwd),a(et,it),a(et,at),a(et,lt),a(et,ut),p(ut,e[0].ssid[1].pwd),a(et,ct),a(et,dt),a(et,rt),a(et,pt),p(pt,e[0].ssid[2].pwd),a(n,ft),a(n,ht),a(n,vt),a(n,mt),a(mt,gt),a(mt,bt),a(mt,xt),a(xt,wt),a(wt,yt),a(wt,$t),a(wt,Ct),p(Ct,e[0].latitude),a(xt,_t),a(xt,kt),a(kt,It),a(kt,St),a(kt,Et),p(Et,e[0].longitude),a(mt,Pt),a(mt,Ht),a(Ht,At),Dt||(Gt=[d(h,"click",e[2]),d(m,"click",D),d(C,"input",e[3]),d(E,"input",e[4]),d(L,"input",e[5]),d(B,"input",e[6]),d(K,"input",e[7]),d(Z,"input",e[8]),d(ot,"input",e[9]),d(ut,"input",e[10]),d(pt,"input",e[11]),d(Ct,"input",e[12]),d(Et,"input",e[13]),d(At,"click",e[1])],Dt=!0)},p(t,[e]){1&e&&C.value!==t[0].deviceId&&p(C,t[0].deviceId),1&e&&E.value!==t[0].websocketHost&&p(E,t[0].websocketHost),1&e&&L.value!==t[0].input01.name&&p(L,t[0].input01.name),1&e&&B.value!==t[0].ssid[0].ssid&&p(B,t[0].ssid[0].ssid),1&e&&K.value!==t[0].ssid[1].ssid&&p(K,t[0].ssid[1].ssid),1&e&&Z.value!==t[0].ssid[2].ssid&&p(Z,t[0].ssid[2].ssid),1&e&&ot.value!==t[0].ssid[0].pwd&&p(ot,t[0].ssid[0].pwd),1&e&&ut.value!==t[0].ssid[1].pwd&&p(ut,t[0].ssid[1].pwd),1&e&&pt.value!==t[0].ssid[2].pwd&&p(pt,t[0].ssid[2].pwd),1&e&&Ct.value!==t[0].latitude&&p(Ct,t[0].latitude),1&e&&Et.value!==t[0].longitude&&p(Et,t[0].longitude)},i:t,o:t,d(t){t&&l(n),Dt=!1,s(Gt)}}}function D(){alert("Se guardará la configuracion")}function G(t,e,n){var s={latitude:0,longitude:0,deviceId:"",input01:{name:""},ssid:[{ssid:"",pwd:""},{ssid:"",pwd:""},{ssid:"",pwd:""}]};return v((async()=>{})),[s,function(){navigator.geolocation?navigator.geolocation.getCurrentPosition((t=>{n(0,s={latitude:t.coords.latitude,longitude:t.coords.longitude})})):x.innerHTML="Geolocation is not supported by this browser."},async function(){alert("Se leerá la configuracion");let t=await fetch("/getsettings");n(0,s=await t.json()),console.log("Retorna settings",s)},function(){s.deviceId=this.value,n(0,s)},function(){s.websocketHost=this.value,n(0,s)},function(){s.input01.name=this.value,n(0,s)},function(){s.ssid[0].ssid=this.value,n(0,s)},function(){s.ssid[1].ssid=this.value,n(0,s)},function(){s.ssid[2].ssid=this.value,n(0,s)},function(){s.ssid[0].pwd=this.value,n(0,s)},function(){s.ssid[1].pwd=this.value,n(0,s)},function(){s.ssid[2].pwd=this.value,n(0,s)},function(){s.latitude=this.value,n(0,s)},function(){s.longitude=this.value,n(0,s)}]}return new class extends class{$destroy(){!function(t,e){const n=t.$$;null!==n.fragment&&(s(n.on_destroy),n.fragment&&n.fragment.d(e),n.on_destroy=n.fragment=null,n.ctx=[])}(this,1),this.$destroy=t}$on(t,e){const n=this.$$.callbacks[t]||(this.$$.callbacks[t]=[]);return n.push(e),()=>{const t=n.indexOf(e);-1!==t&&n.splice(t,1)}}$set(t){var e;this.$$set&&(e=t,0!==Object.keys(e).length)&&(this.$$.skip_bound=!0,this.$$set(t),this.$$.skip_bound=!1)}}{constructor(t){super(),H(this,t,G,A,i,{})}}({target:document.body,props:{name:"Edwin"}})}();)";
}

String responseCSS()
{
    return R"(.flex-container.svelte-10a4hhp{display:-webkit-flex;display:flex;-webkit-align-items:stretch;align-items:stretch;width:auto;height:auto}.flex-item.svelte-10a4hhp{width:50%;margin:10px}input.svelte-10a4hhp{width:100%;padding:12px 20px;margin:8px 0;box-sizing:border-box;border:none;background-color:#3cbc8d;color:white}.button.svelte-10a4hhp{border:none;color:white;padding:16px 32px;text-align:center;text-decoration:none;display:inline-block;font-size:16px;margin:4px 2px;transition-duration:0.4s;cursor:pointer}.button1.svelte-10a4hhp{background-color:white;color:black;border:2px solid #3cbc8d}.button1.svelte-10a4hhp:hover{background-color:#3cbc8d;color:white}.button_ali.svelte-10a4hhp{text-align:end})";
}

String responsegetSettings()
{
    String outputJson = "";
    serializeJson(ocsClass.toJson(), outputJson);
    Serial.println(outputJson);
    return outputJson;
}

const char *echo_org_ssl_ca_cert = R"(-----BEGIN CERTIFICATE-----
MIIEDzCCAvegAwIBAgIBADANBgkqhkiG9w0BAQUFADBoMQswCQYDVQQGEwJVUzEl
MCMGA1UEChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMp
U3RhcmZpZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwHhcNMDQw
NjI5MTczOTE2WhcNMzQwNjI5MTczOTE2WjBoMQswCQYDVQQGEwJVUzElMCMGA1UE
ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjEyMDAGA1UECxMpU3RhcmZp
ZWxkIENsYXNzIDIgQ2VydGlmaWNhdGlvbiBBdXRob3JpdHkwggEgMA0GCSqGSIb3
DQEBAQUAA4IBDQAwggEIAoIBAQC3Msj+6XGmBIWtDBFk385N78gDGIc/oav7PKaf
8MOh2tTYbitTkPskpD6E8J7oX+zlJ0T1KKY/e97gKvDIr1MvnsoFAZMej2YcOadN
+lq2cwQlZut3f+dZxkqZJRRU6ybH838Z1TBwj6+wRir/resp7defqgSHo9T5iaU0
X9tDkYI22WY8sbi5gv2cOj4QyDvvBmVmepsZGD3/cVE8MC5fvj13c7JdBmzDI1aa
K4UmkhynArPkPw2vCHmCuDY96pzTNbO8acr1zJ3o/WSNF4Azbl5KXZnJHoe0nRrA
1W4TNSNe35tfPe/W93bC6j67eA0cQmdrBNj41tpvi/JEoAGrAgEDo4HFMIHCMB0G
A1UdDgQWBBS/X7fRzt0fhvRbVazc1xDCDqmI5zCBkgYDVR0jBIGKMIGHgBS/X7fR
zt0fhvRbVazc1xDCDqmI56FspGowaDELMAkGA1UEBhMCVVMxJTAjBgNVBAoTHFN0
YXJmaWVsZCBUZWNobm9sb2dpZXMsIEluYy4xMjAwBgNVBAsTKVN0YXJmaWVsZCBD
bGFzcyAyIENlcnRpZmljYXRpb24gQXV0aG9yaXR5ggEAMAwGA1UdEwQFMAMBAf8w
DQYJKoZIhvcNAQEFBQADggEBAAWdP4id0ckaVaGsafPzWdqbAYcaT1epoXkJKtv3
L7IezMdeatiDh6GX70k1PncGQVhiv45YuApnP+yz3SFmH8lU+nLMPUxA2IGvd56D
eruix/U0F47ZEUD0/CwqTRV/p2JdLiXTAAsgGh1o+Re49L2L7ShZ3U0WixeDyLJl
xy16paq8U4Zt3VekyvggQQto8PT7dL5WXXp59fkdheMtlb71cZBDzI0fmgAKhynp
VSJYACPq4xJDKVtHCN2MQWplBqjlIapBtJUhlbl90TSrE9atvNziPTnNvT51cKEY
WQPJIrSPnNVeKtelttQKbfi3QBFGmh95DmK/D5fs4C8fF5Q=
-----END CERTIFICATE-----
)";

WiFiMulti wifiMulti;

void setup()
{

    Serial.begin(115200);
    delay(10);

    ocsClass.setSSID("edwinspire", "Caracol1980");
    ocsClass.setSSID("edwinspiremovil", "libpq1983");

    ocsClass.setup(websockets_server_host, deviceId, gpio_in_01, gpio_out_01, echo_org_ssl_ca_cert);

    delay(1000);

    int size = sizeof(ocsClass.listSSID) / sizeof(ocsClass.listSSID[0]);

    for (byte i = 0; i < size; i = i + 1)
    {
        if (ocsClass.listSSID[i].ssid.length() > 5)
        {
            Serial.println("Add SSID => " + ocsClass.listSSID[i].ssid);
            wifiMulti.addAP(ocsClass.listSSID[i].ssid.c_str(), ocsClass.listSSID[i].pwd.c_str());
        }
    }

    Serial.println("Connecting Wifi...");

    if (wifiMulti.run() == WL_CONNECTED)
    {
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());

        server.begin();

        ocsClass.connectWS();
    }
}

void loop()
{

    if (wifiMulti.run() != WL_CONNECTED)
    {
        Serial.println("WiFi not connected!");
        delay(1000);
    }

    WiFiClient client = server.available(); // Listen for incoming clients

    if (client)
    { // If a new client connects,
        currentTime = millis();
        previousTime = currentTime;
        Serial.println("New Client."); // print a message out in the serial port
        String currentLine = "";       // make a String to hold incoming data from the client
        volatile int returnPage = 0;

        while (client.connected() && currentTime - previousTime <= timeoutTime)
        { // loop while the client's connected
            currentTime = millis();
            if (client.available())
            {                           // if there's bytes to read from the client,
                char c = client.read(); // read a byte, then
                Serial.write(c);        // print it out the serial monitor
                header += c;
                if (c == '\n')
                { // if the byte is a newline character
                    // if the current line is blank, you got two newline characters in a row.
                    // that's the end of the client HTTP request, so send a response:
                    if (currentLine.length() == 0)
                    {

                        switch (returnPage)
                        {
                        case 0:
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            client.println(responseRoot());
                            break;
                        case 1:
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:application/javascript");
                            client.println("Connection: close");
                            client.println();

                            client.println(responsebundleJS());
                            break;
                        case 2:
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:text/css");
                            client.println("Connection: close");
                            client.println();

                            client.println(responseCSS());
                            break;
                        case 3:
                            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                            // and a content-type so the client knows what's coming, then a blank line:
                            client.println("HTTP/1.1 200 OK");
                            client.println("Content-type:application/json");
                            client.println("X-Powered-By: ESP32");
                            client.println("Connection: close");
                            client.println();

                            client.println(responsegetSettings());
                            break;
                        default:
                            client.println("HTTP/1.1 404 OK");
                            client.println("Content-type:text/html");
                            client.println("Connection: close");
                            client.println();

                            break;
                        }

                        // The HTTP response ends with another blank line
                        client.println();
                        // Break out of the while loop
                        break;
                    }
                    else
                    { // if you got a newline, then clear currentLine
                        currentLine = "";
                    }
                }
                else if (c != '\r')
                {                     // if you got anything else but a carriage return character,
                    currentLine += c; // add it to the end of the currentLine
                }

                if (currentLine.endsWith("GET / HTTP"))
                {
                    // digitalWrite(5, HIGH); // GET /H turns the LED on
                    // ArmSystem();

                    Serial.println("GET  ROOT ");
                    returnPage = 0;
                }

                if (currentLine.endsWith("GET /build/bundle.js"))
                {
                    // digitalWrite(5, HIGH); // GET /H turns the LED on
                    // ArmSystem();
                    returnPage = 1;
                    Serial.println("GET  +++++++++ bundle");
                }

                if (currentLine.endsWith("GET /build/bundle.css"))
                {
                    // digitalWrite(5, HIGH); // GET /H turns the LED on
                    // ArmSystem();
                    returnPage = 2;
                    Serial.println("GET  +++++++++ CSS");
                }

                if (currentLine.endsWith("GET /getsettings HTTP"))
                {
                    // digitalWrite(5, HIGH); // GET /H turns the LED on
                    // ArmSystem();
                    returnPage = 3;
                    Serial.println("GET  +++++++++ CSS");
                }
            }
        }
        // Clear the header variable
        header = "";
        // Close the connection
        client.stop();
        Serial.println("Client disconnected.");
        Serial.println("");
    }

    ocsClass.loop();
    // WifiLoop();
}
