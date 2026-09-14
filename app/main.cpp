#include <iostream>
#include "chat_core/chat_store.hpp"
#include "chat_core/message.hpp"

using namespace chat;

int main() {
    ChatStore store;
    
    // Создаём комнаты и сообщения заранее
    store.create_room("room1");
    store.create_room("room2");
    store.post("room1", Message("alice", "привет"));
    store.post("room2", Message("bob", "хай"));
    store.post("room2", Message("carol", "здравствуй"));
    
    std::cout << "=== Демонстрация лямбд ===\n\n";
    
    // Лямбда 1: захват по ссылке [&total]
    std::size_t total = 0;
    store.for_each_room([&total](const std::string& name, const std::vector<Message>& msgs) {
        std::cout << "  " << name << ": " << msgs.size() << " сообщений\n";
        total += msgs.size();
    });
    std::cout << "Итого (по ссылке): " << total << "\n\n";
    
    // Лямбда 2: захват по значению [total] mutable
    std::size_t total2 = 0;
    store.for_each_room([total2](const std::string& name, const std::vector<Message>& msgs) mutable {
        std::cout << "  " << name << ": " << msgs.size() << " сообщений\n";
        total2 += msgs.size();
    });
    std::cout << "Итого (по значению): " << total2 << "\n";
    
    return 0;
}