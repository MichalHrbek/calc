#pragma once
#include <WiFiClientSecure.h>
#include <vector>

struct GeminiMessage
{
	String prompt;
	String response;
};

class GeminiClient
{
	private:
		WiFiClientSecure _wifi;
		std::vector<GeminiMessage> _chat;
		String _key;
		String _endpoint;
	public:
		GeminiClient();
		String get_response(const String& prompt);
};