int soundctl_unlock(char *callsign, char *key, size_t sz) {
/*
ONE REQUEST ONE RESPONSE

task(callsign, soundctl_access_token) {

POST https://soundctl.auth0.com/tokeninfo
Content-Type: 'application/json'
{
  "id_token": $soundctl_access_token
}

}

200 OK

{
  "app_metadata": {
    stations = ["radio45"]
}

if (stations.include? callsign) return true
return false
*/
  return 1;
}
