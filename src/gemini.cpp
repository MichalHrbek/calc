#include "gemini.h"
#include "config.h"
#include "ui.h"
#include <HTTPClient.h>

GeminiClient::GeminiClient()
{
	_key = read_config_string("/config/gemini/key");
	_endpoint = "https://generativelanguage.googleapis.com/v1beta/models/gemini-1.5-flash-8b:generateContent?key=" + _key;
	_wifi.setInsecure();
}

String GeminiClient::get_response(const String& prompt)
{
	HTTPClient _http;
	if (!_http.begin(_wifi, _endpoint))
	{
		announce("Client error");
		return "";
	}
	_http.addHeader("Content-Type", "application/json");
	String payload = "{\"contents\":[{\"parts\":[{\"text\": \"" + prompt + "\"},]}]}";
	int response_code = _http.POST(payload);

	if (response_code == 200)
	{
		String payload = _http.getString();
		int start = payload.indexOf('"', payload.indexOf("\"text\":")+7)+1;
		if (start == -1)
		{
			Serial.print("No response");
			return "";
		}
		String response = payload.substring(start);
		for (size_t i = 1; i < response.length(); i++)
		{
			// Escaped characters
			if (response.charAt(i-1) == '\\')
			{
				response.setCharAt(i-1, 127);
				// Newlines
				if (response.charAt(i) == 'n') response.setCharAt(i, '\n');
				continue;
			}
			// Unescaped quote = end
			if (response.charAt(i) == '"')
			{
				response = response.substring(0,i);
				_chat.push_back({prompt, response});
				break;
			}
		}
	}
	else if (response_code>0)
	{
		Serial.print("HTTP Response code: ");
		Serial.println(response_code);
		String payload = _http.getString();
		Serial.println(payload);
	}
	else
	{
		Serial.print("Error code: ");
		Serial.println(_http.errorToString(response_code));
	}
	_chat.push_back({prompt, ""});
	return "";
}
