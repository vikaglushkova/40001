#ifndef STREAM_GUARD_HPP
#define STREAM_GUARD_HPP
#include <ios>

namespace shapes
{
    class StreamGuard   //класс нужен для сохранения потока и автоматически восстанавливает их при выходе из области видимости
    {
    public:
        explicit StreamGuard(std::basic_ios< char >& s);   //конструктор, принимает поток по ссылке, сохраняет текущие настройки потокаа в поля  precision_ и flags_   explicit - запрещает неявное преобразование (компилятор автоматически преобразует один тип в другой без явного указания меня)
        ~StreamGuard();
    private:
        std::basic_ios< char >& stream_; //ссылка на поток, для сохранения и восстановления его настроек
        std::streamsize precision_;  //точность вывода чисел
        std::basic_ios< char >::fmtflags flags_;   //флаги форматирования
    };
}
#endif

