#include "shape_commands.hpp"
#include <functional>
#include <map>
#include <algorithm>
#include <iomanip>
#include <numeric>
#include <string>
#include <iterator>
#include "stream_guard.hpp"

namespace
{
    using namespace shapes;

    double computeArea(const Polygon& poly) //константная ссылка потому что игаем копировааия большого вектора, функция не должна изменять полиго и площадь не дробное число
    {
        if (poly.points.size() < 3) return 0.0;      //<3 фигня, площадь 0

        // исползуем accumulate с индексами для последовательности
        std::vector<size_t> indices(poly.points.size());
        std::iota(indices.begin(), indices.end(), 0); // создаем последовательность    iota - иницилизация значений

        // функтор который можно вызвать как функцию        //   size_t i - индекс текущей вершины           double sum - аккумулированная сумма на текущем шаге
        auto areaAccumulator = [&poly](double sum, size_t i) {    //лямбда-функция для вычесления площади полигона по гауссу
            size_t j = (i + 1) % poly.points.size();
            return sum + poly.points[i].x * poly.points[j].y - poly.points[j].x * poly.points[i].y; //гаусс
            };

        double sum = std::accumulate(indices.begin(), indices.end(), 0.0, areaAccumulator);
        return 0.5 * std::abs(sum);
    }

    struct AreaCalculator   //второй функтор, который берет сумму + площать
    {
        double operator()(double acc, const Polygon& poly) const
        {
            return acc + computeArea(poly);
        }
    };

    struct VertexCountPredicate
    {
        size_t count;   //нужное кол во вершин
        VertexCountPredicate(size_t c) : count(c) {}   //конструктор (когда Создаем переменную типа VertexCountPredicate, передаем объескт  в функцию и создаем временный объект)
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() == count;   //проверка условий
        }
    };

    struct EvenVertexPredicate    //пропускает только полигоны с 4,6,8... вершинами т.е четными
    {
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() % 2 == 0;   //проверка на четность
        }
    };

    struct OddVertexPredicate  //для нечетных кол во вершин
    {
        bool operator()(const Polygon& poly) const
        {
            return poly.points.size() % 2 != 0;  //логично
        }
    };

    struct AreaCompare     //поиск наименьшей площади
    {
        bool operator()(const Polygon& a, const Polygon& b) const
        {
            return computeArea(a) < computeArea(b);
        }
    };

    struct VertexCountCompare //меньше вершин
    {
        bool operator()(const Polygon& a, const Polygon& b) const
        {
            return a.points.size() < b.points.size();
        }
    };

    struct SameChecker //совместимость полигонов
    {
        Polygon target;  // тип оригинал для сравнения
        SameChecker(const Polygon& t) : target(t) {}  //конструктор (вызов при создании в doSame объекта,... он принимает полигон Т по константной ссылке, Инициализирует поле target копией переданного полигона,   target теперь эталон для сравнения)

        bool operator()(const Polygon& poly) const
        {
            if (poly.points.size() != target.points.size()) return false;  //если разное кол во вершин то полигоны не совместимы
            if (poly.points.empty()) return true;  //проверка на пустость

            int dx = poly.points[0].x - target.points[0].x; //на сколько нужно сместить target чтоб совпала первая точка
            int dy = poly.points[0].y - target.points[0].y;

            return std::equal(   //проверка лямбда-функции на совпадеие точки Р с Т после сдвига
                poly.points.begin(), poly.points.end(),
                target.points.begin(),
                [dx, dy](const Point& p, const Point& t) {
                    return p.x == t.x + dx && p.y == t.y + dy;
                }
            );            //лямбда проверяет для каждой пары точек условие p = t + shift. std::equal применяет эту лямбду ко всем точкам полигонов
        }
    };
}

void shapes::doArea(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;   //читаем параметр команд и создаем калькулятор площадей
    in >> arg;

    // ИЗМЕНЕНО: Используем карту обработчиков вместо цепочки if-else для большей модульности
    std::map<std::string, std::function<double()>> handlers; //map - ассоциативный контейнер, который хранит пары "ключ-значение" в отсортированом порядке
    double result = 0.0;

    handlers["EVEN"] = [&shapes]() {
        EvenVertexPredicate pred;   //фильтр для четных вершин
        std::vector<Polygon> filtered;
        std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filtered), pred);   //копируем их в новый вектор
        return std::accumulate(filtered.begin(), filtered.end(), 0.0, AreaCalculator{});  //и суммируем отфильтрованных полигонов
        };

    handlers["ODD"] = [&shapes]() {   //тоже ссамое но нечетные верршин
        OddVertexPredicate pred;
        std::vector<Polygon> filtered;
        std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filtered), pred);
        return std::accumulate(filtered.begin(), filtered.end(), 0.0, AreaCalculator{});
        };

    handlers["MEAN"] = [&shapes]() {   //проврка что есть хоть один, суммирует площади всех полигонов, и делит на общее кол во
        if (shapes.empty()) throw std::logic_error("<EMPTY POLYGONS>");
        return std::accumulate(shapes.begin(), shapes.end(), 0.0, AreaCalculator{}) / shapes.size();
        };

    auto it = handlers.find(arg);
    if (it != handlers.end()) {
        result = it->second();
    }
    else
    {
        try
        {
            size_t num = std::stoul(arg);   //преобразование строки в число, если строка не число - исключение
            if (num < 3) throw std::invalid_argument("<INVALID VERTEX COUNT>");

            VertexCountPredicate pred(num);   //тоже самое, но по конкретному числу вершин
            std::vector<Polygon> filtered;
            std::copy_if(shapes.begin(), shapes.end(), std::back_inserter(filtered), pred);
            result = std::accumulate(filtered.begin(), filtered.end(), 0.0, AreaCalculator{});
        }
        catch (const std::invalid_argument&)   //std::stoul не смог преобразовать строку (не число), перехватывает исключение и выдает ошибку
        {
            throw std::invalid_argument("<INVALID ARGUMENT>");
        }
    }

    StreamGuard guard(out);     //сохраняем текущие настройки покота, устанавливает фиксированный формат и точность 1 знак после запятой, в деструкторе восстанавливает исходные настройки
    out << std::fixed << std::setprecision(1) << result << '\n';
}

void shapes::doMax(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out) //поток out - поток вывода?????????????????  //максимальное значение
{                                                                                       //ostream& out обеспечивает гибкость вывода результатов
    std::string arg;
    in >> arg;

    if (shapes.empty()) throw std::logic_error("<EMPTY POLYGONS>");  //проверяем наличие

    if (arg == "AREA")
    {
        AreaCompare comp;    //создаем компаратор для сравнения по площади
        auto it = std::max_element(shapes.begin(), shapes.end(), comp);  //находим полиго с макс площадью
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << computeArea(*it) << '\n';       //вычисляет площадь найденного полигона
    }   //1 знак после ,
    else if (arg == "VERTEXES")
    {
        VertexCountCompare comp;     //сравнивает кол во вершин, выводит просто число (кол во вершин)
        auto it = std::max_element(shapes.begin(), shapes.end(), comp);
        out << it->points.size() << '\n';
    }
    else
    {
        throw std::invalid_argument("<INVALID ARGUMENT>");        //если аргумент не AREA и не VERTEXES - ошибка
    }
}

void shapes::doMin(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    if (shapes.empty()) throw std::logic_error("<EMPTY POLYGONS>");

    if (arg == "AREA")
    {
        AreaCompare comp;
        auto it = std::min_element(shapes.begin(), shapes.end(), comp);  // ищем меньший элемент, согласно корпаратору
        StreamGuard guard(out);
        out << std::fixed << std::setprecision(1) << computeArea(*it) << '\n';
    }
    else if (arg == "VERTEXES")
    {
        VertexCountCompare comp;
        auto it = std::min_element(shapes.begin(), shapes.end(), comp);
        out << it->points.size() << '\n';   //вывод кол во вершин, через вектор points
    }
    else
    {
        throw std::invalid_argument("<INVALID ARGUMENT>");
    }
}

void shapes::doCount(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    std::string arg;
    in >> arg;

    size_t result = 0;

    if (arg == "EVEN")
    {
        EvenVertexPredicate pred;     //создаем предикат для четных вершин
        result = std::count_if(shapes.begin(), shapes.end(), pred);
    }
    else if (arg == "ODD")
    {
        OddVertexPredicate pred;
        result = std::count_if(shapes.begin(), shapes.end(), pred);   //для нечетных
    }
    else
    {
        try
        {
            size_t num = std::stoul(arg);   //преобрауем строку в число......если строка не число
            if (num < 3) throw std::invalid_argument("<INVALID VERTEX COUNT>");   //>3 то ошибка

            VertexCountPredicate pred(num);  //предикант для конкректного числа вершин
            result = std::count_if(shapes.begin(), shapes.end(), pred);  //подсчитываем полигон с N вершинами
        }
        catch (const std::invalid_argument&)   //перехватываем исключение и выдаем ошибку из-за то что std::stoul не смог преобразовать строку
        {
            throw std::invalid_argument("<INVALID ARGUMENT>");
        }
    }

    out << result << '\n';
}

void shapes::doRmecho(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target)) throw std::invalid_argument("<INVALID POLYGON>");  //читаем полигон, который нужно обработать

    struct IsTargetAndPrevious
    {
        const Polygon& target;   //эталон полигона
        mutable bool previousWasTarget = false;
        bool operator()(const Polygon& poly) const
        {
            if (poly == target)
            {                          //сбрасывааем флаг, и осставляем полигон (false - не удалять)
                if (previousWasTarget)
                {
                    return true; // удаляем этот дубликат
                }
                previousWasTarget = true;   //запоминаем что нашлb target
                return false; // оставляем первый полигон
            }
            previousWasTarget = false;
            return false;
        }
    };

    auto pred = IsTargetAndPrevious{ target };    //создаем объект функтора с целевым полигоном. target перейдается по ссылке в функтор
    auto it = std::remove_if(shapes.begin(), shapes.end(), pred);   //remove проходи по всем элементам вектора, для каждого вызывая pred(poly), Если pred возвращает true - помечает элемент на удаление, НЕ МЕНЯЕМ РАЗМЕР ВЕКОРА(только предупорядочиваем)
    size_t removedCount = std::distance(it, shapes.end());//shapes.end() - физический конец вектора
    shapes.erase(it, shapes.end());  //удаляем элементы из физической памяти, меняем размер и удаляем все элементы от it до конца  --- это про vector::erase
    out << removedCount << '\n';    //вывод удаленныъ полигонов
}      //it указывает на новый логический конец вектора(после удаленных)

void shapes::doSame(std::vector<Polygon>& shapes, std::istream& in, std::ostream& out)
{
    Polygon target;
    if (!(in >> target)) throw std::invalid_argument("<INVALID POLYGON>");  //Читаем полигон, с которым будем сравнивать.... если не получается то ошибка

    SameChecker checker(target);  //Создаем функтор SameChecker с целевым полигоном
    size_t count = std::count_if(shapes.begin(), shapes.end(), checker);  //проходи по всем полигонам ввекторе shapes, для каждого вызываем checker(poly), Если SameChecker возвращает true - увеличивает счетчик,  Возвращает количество совместимых полигонов
    out << count << '\n';   //выводим кол во найденныъ полигонов
}
