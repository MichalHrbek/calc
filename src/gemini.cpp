#include "gemini.h"
#include "config.h"
#include "ui.h"
#include "utils.h"

const char* host = "generativelanguage.googleapis.com";

GeminiClient::GeminiClient()
{
	_key = read_config_string("/config/gemini/key");
	_endpoint = "/v1beta/models/gemini-1.5-flash:generateContent?key=" + _key;
	_client.setInsecure();
}

#define PROMPT_FORMAT "{\"contents\":[{\"parts\":[{\"text\": \"%s\"},]}]}"
String GeminiClient::get_response(const String& prompt)
{

	if (!_client.connect(host, 443)) {
		announce("Connection failed!");
		return "";
	}
	_client.println("POST " + _endpoint + " HTTP/1.1");
	_client.println("Host: " + String(host));
	_client.println("Content-Type: application/json");
	_client.printf("Content-Length: %u\n", sizeof(PROMPT_FORMAT)-3+prompt.length());
	_client.println("Connection: close");
	_client.println();
	
	_client.printf(PROMPT_FORMAT, prompt.c_str());
	_client.println();
	
	Serial.printf("Content-Length: %u\n", sizeof(PROMPT_FORMAT)-3+prompt.length());
	Serial.printf(PROMPT_FORMAT, prompt.c_str());
	while (_client.connected() || _client.available()) {
		String line = _client.readStringUntil('\n');
		if (line.length() == 0) break;
		Serial.println(line);
	}

	_client.stop();
	return "";
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

void GeminiClient::show()
{
}
