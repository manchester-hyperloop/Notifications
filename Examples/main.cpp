
#include <iostream>
#include <string>
#include "SubjectObserver.hpp"
#include "Callback.hpp"

using namespace Util;

class MySubject
{
    Subject<int> counter;
    Subject<std::string> name;

public:
    MySubject() : counter(0), name("Bob") {}

    Subject<int> *getCounterModel()
    {
        return &counter;
    }

    Subject<std::string> *getNameModel()
    {
        return &name;
    }

    void run()
    {
        counter = counter.getLatestVal() + 1;
        name = "Alice";
        counter = counter.getLatestVal() + 1;
        counter = counter.getLatestVal() + 1;
        name = "Jimbo";
        counter = 0;
    }
};

// A subscriber. Only subscribes to one subject.
class SubscriberOne
{
    MethodCallback<SubscriberOne, int> counterCallback;
    Observer<int> counterSubscription;

    void counterUpdate(int latest)
    {
        std::cout << "Sub one: " << latest << std::endl;
    }

public:
    //	Observer( *mod, Callback<T> *_cb)
    SubscriberOne(Subject<int> *subject) : counterCallback(&SubscriberOne::counterUpdate, this),
                                           counterSubscription(subject, &counterCallback)
    {
    }
};

// MARK: Subscriber Two

// Another subscriber, subscribes to two subjects (counter and name)
class SubscriberTwo
{
    MethodCallback<SubscriberTwo, int> counterCallback;
    MethodCallback<SubscriberTwo, std::string> nameCallback;

    Observer<int> counterSubscription;
    Observer<std::string> nameSubscription;

    void counterUpdate(int latest)
    {
        std::cout << "Sub two: " << latest << std::endl;
    }

    void nameUpdate(std::string latest)
    {
        std::cout << "Sub two: The subject's new name is " << latest << std::endl;
    }

public:
    SubscriberTwo(Subject<int> *counterSubject, Subject<std::string> *nameSubject) : counterCallback(&SubscriberTwo::counterUpdate, this),
                                                                                     nameCallback(&SubscriberTwo::nameUpdate, this),
                                                                                     counterSubscription(counterSubject, &counterCallback),
                                                                                     nameSubscription(nameSubject, &nameCallback)

    {
    }
};

int main(int argc, const char *argv[])
{

    MySubject sc;

    SubscriberOne s1(sc.getCounterModel());
    SubscriberTwo s2(sc.getCounterModel(), sc.getNameModel());

    sc.run();

    return 0;
}
