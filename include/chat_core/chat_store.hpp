#pragma once

#include "message.hpp"
#include <map>
#include <string>
#include <vector>

namespace chat {

class ChatStore {
public:
    void create_room(const std::string& room_name) {
        // emplace создаёт пару <ключ, значение> прямо внутри map.
        // Копирования не происходит, вектор создаётся на месте.
        rooms_.emplace(room_name, std::vector<Message>{});
    }

    bool post(const std::string& room_name, Message msg) {
        auto it = rooms_.find(room_name);
        if (it == rooms_.end()) return false;
        
        // std::move перемещает данные из msg, избегая дорогого копирования строк.
        it->second.push_back(std::move(msg));
        return true;
    }

    const std::vector<Message>* history(const std::string& room_name) const {
        auto it = rooms_.find(room_name);
        if (it == rooms_.end()) return nullptr;
        return &it->second;
    }

    std::size_t room_count() const { 
        return rooms_.size(); 
    }

    // Шаблонный метод для обхода всех комнат
    template <typename Func>
    void for_each_room(Func f) {
        // Разбор квалификаторов в "const auto& [room, messages]":
        // 1. auto       : компилятор сам выведет тип элемента map (это std::pair<const std::string, std::vector<Message>>).
        // 2. & (ссылка) : КРИТИЧЕСКИ ВАЖНО. Без ссылки мы бы скопировали и имя комнаты, и весь вектор сообщений. 
        //                 Ссылка делает обход мгновенным и бесплатным по памяти.
        // 3. const      : Мы запрещаем изменять саму структуру map (например, случайно перезаписать вектор или удалить комнату).
        //
        // В итоге внутри цикла:
        // - room     имеет тип: const std::string&  (ключи в map всегда const)
        // - messages имеет тип: const std::vector<Message>&
        for (const auto& [room, messages] : rooms_) {
            f(room, messages);
        }
    }

private:
    std::map<std::string, std::vector<Message>> rooms_;
};

} // namespace chat