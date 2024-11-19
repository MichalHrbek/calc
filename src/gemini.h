#pragma once
#include <WiFiClientSecure.h>
#include <vector>

enum struct ChatUser: uint8_t
{
	USER = 0,
	GEMINI = 1,
};

struct GeminiMessage
{
	ChatUser author;
	String content;
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
		void show();
};