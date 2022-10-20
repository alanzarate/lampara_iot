var stateButton = 0;

function imprimir(){
    console.log(stateButton);
    if(stateButton == 0){
        stateButton = 1;
    }else{
        stateButton = 0;
    }
    if(stateButton == 0){
        document.getElementById("pulsador-button").classList.remove("pulsador-activado");
        document.body.style.backgroundColor = "white";
    }else{
        document.getElementById("pulsador-button").classList.add("pulsador-activado");
        document.body.style.backgroundColor = "#FAEFB9";
    }
}


function getData(){
  var xhr = new XMLHttpRequest();
  xhr.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var myObj = JSON.parse(this.responseText);
      console.log(myObj);
      var tempC = myObj.temperatureC;
      var tempF = myObj.temperatureF;
      var hum = myObj.humidity;
      var rs = myObj.rssi;
      var ip = myObj.ip;
      var hn = myObj.hostname;
      var wifiS = myObj.wifiStatus;
        var ssid = myObj.ssid;
        var psk = myObj.psk;
        var bssid = myObj.bssid;

      changeDataVisible(tempC, tempF, hum);
      signalChanger(parseInt( rs));
      buildTable(ip, hn, wifiS, ssid, psk, bssid);
     
    }
  }; 
  xhr.open("GET", "/readings", true);
  xhr.send();
}
function changeDataVisible(tempC, tempF, hum){
    gaugeTempC.value = tempC;
    gaugeTempF.value = tempF;
    gaugeHum.value = hum;
}


// adc chart 

var chartADC = new Highcharts.Chart({
  chart:{ renderTo:'chart-ADC' },
  title: { text: 'Valores de LDR' },
  series: [{
    showInLegend: false,
    data: []
  }],
  plotOptions: {
    line: { animation: false,
      dataLabels: { enabled: true }
    },
    series: { color: '#18009c' }
  },
  xAxis: {
    type: 'datetime',
    dateTimeLabelFormats: { second:'%S' }
  },
  yAxis: {
    title: { text: 'Datos de LDR' }
  },
  credits: { enabled: false }
});
setInterval(function ( ) {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var x = (new Date()).getTime(),
          y = parseFloat(this.responseText);
      //console.log(this.responseText);
      if(chartADC.series[0].data.length > 40) {
        chartADC.series[0].addPoint([x, y], true, true, true);
      } else {
        chartADC.series[0].addPoint([x, y], true, false, true);
      }
    }
  };
  xhttp.open("GET", "/ADC", true);
  xhttp.send();
}, 1000 ) ;

 
  function sendSliderValue(){
    var val = document.getElementById('pwmInput2').value;
        var xhttp = new XMLHttpRequest();
        xhttp.open("PUT", "/SLIDER?VALUE="+val, true);
        xhttp.send(); 
  }
 

function updateTextInput2(val) {
		console.log(val);
		document.getElementById('pwmInput2').value = val; 
		document.getElementById('textInput2').value = val; 
	}

// page4

function consigue4(){
  alert(document.getElementById('hora_pick').value);
  var a   = document.getElementById('appt').value;
  if(a.length === 0){
    alert('dos esta vacio');
  }
}


function convierte(){
        
        var time1 = document.getElementById('time1').value;
        var time2 = document.getElementById('time2').value;
        if(time1.length != 0 && time2.length != 0){
          var arrTime1 = time1.split(":");
          var arrTime2 = time2.split(":");
          var now = new Date();
          var mls1 = new Date(now.getFullYear(), now.getMonth(), now.getDate(), arrTime1[0],arrTime1[1],0,0) - now;
          var mls2 = new Date(now.getFullYear(), now.getMonth(), now.getDate(),arrTime2[0],arrTime2[1],0,0) - now;
          if(mls1 < 0){
            mls1 += 86400000;
          }
          if(mls2 < 0){
            mls2 += 86400000;
          }

          if(mls1 > mls2){
            mls2 += 86400000;
          }
          console.log(mls1);
          console.log(mls1/3600000);
          console.log(mls2);
          console.log(mls2/3600000);
        }else{
          if(time1.length === 0){
            alert('Ingresa una hora de encedido valida');
          }
          if(time2.length === 0){
            alert('Ingresa una hora de apagado valida');
          }
        }
        
        
      }
  

      // page 6 
      var chartLDR = new Highcharts.Chart({
        chart:{ renderTo:'chart-LDR2' },
        title: { text: 'Valores de LDR' },
        series: [{
          showInLegend: false,
          data: []
        }],
        plotOptions: {
          line: { animation: false,
            dataLabels: { enabled: true }
          },
          series: { color: '#18009c' }
        },
        xAxis: {
          type: 'datetime',
          dateTimeLabelFormats: { second:'%S' }
        },
        yAxis: {
          title: { text: 'Datos de LDR' }
        },
        credits: { enabled: false }
      });