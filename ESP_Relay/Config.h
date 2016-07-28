String buildConfig(){
  String content = "<html><body><form action='/setConfig' method='POST'>Set WiFI SSID and Pass<br>";
  content += "SSID:<input type='text' name='SSID' placeholder='SSID'><br>";
  content += "Password:<input type='password' name='PASSWORD' placeholder='password'><br>";
  content += "<input type='submit' name='SUBMIT' value='Submit'></form><br>";
  content += "</body></html>";
  return content;
}
