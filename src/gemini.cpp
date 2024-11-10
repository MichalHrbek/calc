#include <gemini.h>
#include <config.h>
#include <ui.h>

const char* host = "generativelanguage.googleapis.com";

GeminiClient::GeminiClient()
{
    _key = read_config_string("/config/gemini/key");
    _endpoint = "/v1beta/models/gemini-1.5-flash:generateContent?key=" + _key;
    _client.setInsecure();
}

String GeminiClient::get_response(String prompt)
{
    if (!_client.connect(host, 443)) {
        announce("Connection failed!");
        return "";
    }
    _client.println("POST " + _endpoint + " HTTP/1.1");
    _client.println("Host: " + String(host));
    _client.println("Content-Type: application/json");
    _client.println("Connection: close");
    _client.println();
    _client.println("{}");
// curl https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash:generateContent?key=${GEMINI_API_KEY} \
// -H 'Content-Type: application/json' \
// -d '{
// "contents":[{
//   "parts":[
//     {"text": "Explain how AI works"},
//   ]
// }]
// }'
}