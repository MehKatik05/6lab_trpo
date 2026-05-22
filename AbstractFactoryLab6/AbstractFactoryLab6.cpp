//Разработайте иерархию классов Суп →(Борщ, Тыквенный суп),
//ВтороеБлюдо →(ЖаренаяКартошка, Котлета).При помощи паттерна AbstractFactory
//разработайте классы Диета →(МяснаяДиета, ВегетарианскаяДиета).Продемонстрируйте
//работу паттерна.
#include <iostream>

using namespace std;

//Абстрактный суп
class Soup {
public:
    virtual ~Soup() {}
    virtual void serve() const = 0;
    virtual int getCalories() const = 0;
};

//Борщ
class Borscht : public Soup {
public:
    void serve() const override {
        cout << "Подаётся борщ." << std::endl;
    }
    int getCalories() const override { return 150; }
};

//Тыквенный суп
class PumpkinSoup : public Soup {
public:
    void serve() const override {
        cout << "Подаётся тыквенный суп." << std::endl;
    }
    int getCalories() const override { return 120; }
};

//Вторые блюда
class SecondCourse {
public:
    virtual ~SecondCourse() {}
    virtual void serve() const = 0;
    virtual int getCalories() const = 0;
};

//Жареная картошка
class FriedPotatoes : public SecondCourse {
public:
    void serve() const override {
        cout << "Подаётся жареная картошка." << endl;
    }
    int getCalories() const override { return 300; }
};

//Котлета
class Cutlet : public SecondCourse {
public:
    void serve() const override {
        cout << "Подаётся котлета." << endl;
    }
    int getCalories() const override { return 250; }
};