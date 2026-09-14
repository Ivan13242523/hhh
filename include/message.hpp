#pragma once

#include <chrono>
#include <string>
#include <utility>

// Message хранит автора, текст и отметку времени.
//
// Правило нуля: ни одна из пяти специальных функций (деструктор,
// копирующий/перемещающий конструктор, копирующее/перемещающее
// присваивание) не объявлена вручную. Компилятор сгенерирует их
// автоматически, потому что все поля — std::string и
// std::chrono::system_clock::time_point — уже умеют корректно
// копироваться и перемещаться. std::string имеет noexcept-
// перемещающий конструктор, поэтому и перемещение Message
// получается noexcept. Деструктор не нужен: std::string сам
// освобождает память. Ни одного new/delete здесь нет.
namespace chat {


class Message {
public:
    Message() = default;

    Message(std::string author, std::string text)
        : author_(std::move(author)),
          text_(std::move(text)),
          timestamp_(std::chrono::system_clock::now()) {}

    const std::string& author() const { return author_; }
    const std::string& text() const { return text_; }
    auto timestamp() const { return timestamp_; }

private:
    std::string author_;
    std::string text_;
    std::chrono::system_clock::time_point timestamp_;
};

} 