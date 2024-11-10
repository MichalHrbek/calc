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
        WiFiClientSecure _client;
        std::vector<GeminiMessage> _chat;
        String _key;
        String _endpoint;
    public:
        GeminiClient();
        String get_response(String prompt);
};