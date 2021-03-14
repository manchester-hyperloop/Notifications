//
//  notifications.hpp
//  Hyperloop_Embedded
//
//  Created by Harry O'Brien on 14/03/2020.
//  Copyright © 2021 Hyperloop Manchester. All rights reserved.
//

#ifndef lib_Notifications_Notifications_hpp
#define lib_Notifications_Notifications_hpp

#include "LinkedList.hpp"
#include <Callback.hpp>

// Abstract definition of the 'Observer' class so that 'Subject' class knows that it exists.
template <typename T>
class Observer;

/**
 * The subject class is the object that we subscribe to to get live updates of variable state change.
 * This class should be inherited.
 */
template <typename T>
class Subject
{
  /// The views that are subscribed to this subject. Stored in a linked list meaning that they can be removed and added easily.
  LinkedList<Observer<T>> views;

  /// The value that, on change, will cause a notification to be sent to all subscribers
  T latestValue;

public:
  /// Default constructor. Stored value will is undefined at this point.
  Subject() {}

  /// Constructor taking an initial value.
  Subject(T initialValue) : latestValue(initialValue) {}

  /// Attach a subscriber to this subject
  /// @param obs : The observer to add to the list of subscribers
  void attach(Observer<T> *obs)
  {
    views.append(obs);
  }

  /// Remove a subscriber from this subject
  /// @param obs : The observer to remove
  void remove(Observer<T> *obs)
  {
    views.remove(obs);
  }

  /// Sets the latest value and notifies the subscribers of this change
  /// @param val : The value to set as the latest value
  void setLatestVal(T val)
  {
    latestValue = val;
    notify();
  }

  /// Same as 'setLatestVal(T val)' but uses the '=' operator
  /// @param val : The value to set as the latest value
  void operator=(T val)
  {
    setLatestVal(val);
  }

  /// Retrieves the latest value
  T getLatestVal()
  {
    return latestValue;
  }

  /// Notifies all subscribers of the updated value by calling
  void notify()
  {
    // Return if we have no one to notify
    if (views.getElementCount() <= 0)
      return;

    // Go to the start of the linked list
    views.moveToHead();

    // We need the keyword 'typename' here because the compiler doesn't fully understand what the type 'LinkedList<Observer<T>>::Element' is yet.
    typename LinkedList<Observer<T>>::Element *e = views.getActiveElement();

    // we know that 'e->store' is of type 'Observer<T>*' so we can call the Observer<T>::update() method on it.
    e->store->update();

    // notify all subscribers
    while (views.hasNext())
    {
      // Navigating the linked list
      views.next();

      // Notifying subscribers
      e = views.getActiveElement();
      e->store->update();
    }
  }
};

/**
The Observer class listens for updates from any classes that inherit the subject class and the observer subscribes to.
Naturally, an observer will not get any information from a subject that it has not subscribed to.
*/
template <typename T>
class Observer
{
  Subject<T> *subj;
  Callback<T> *cb;

public:
  /**
  Initialises the observer and attaches itself to the subject
  @param _subj : The subject that we are subscribing to
  @param _cb : The callback that will be called when an update is sent from the subject
  */
  Observer(Subject<T> *_subj, Callback<T> *_cb) : subj(_subj),
                                                  cb(_cb)
  {
    subj->attach(this);
  }

  /// On deconstruction, we remove ourself from the list of subscribers
  ~Observer()
  {
    subj->remove(this);
  }

  /// execute the callback function with the latest value received
  void update()
  {
    cb->run(subj->getLatestVal());
  }

private:
  /// get the subject that we are subscribed to
  Subject<T> *getSubject()
  {
    return subj;
  }
};

#endif /* lib_Notifications_Notifications_hpp */