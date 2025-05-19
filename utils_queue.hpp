#ifndef UTILS_QUEUE_HPP
#define UTILS_QUEUE_HPP

#include "macros.hpp"
#include <cstddef>

template <typename T> struct QueueElement {
  QueueElement<T> *next = nullptr;
  T data;
};

template <typename T> class Queue {
public:
  Queue() = default;
  ~Queue() {
    QueueElement<T> *curr = tail;
    while (curr != nullptr) {
      QueueElement<T> *next = curr->next;
      delete curr;
      curr = next;
    };
  };

  void enqueue(T element) {
    if (tail == nullptr) {
      tail = new QueueElement<T>;
      tail->data = element;
      tail->next = nullptr;
    } else {
      QueueElement<T> *curr = tail;

      while (curr->next != nullptr) {
        curr = curr->next;
      }

      QueueElement<T> *qe = new QueueElement<T>;

      qe->data = element;
      qe->next = nullptr;
      curr->next = qe;

      head = qe;
    }
    ++current_length;
  };

  T dequeue() {
    if (tail == nullptr) {
      return 0;
    }

    T ret_data = tail->data;
    QueueElement<T> *prev = tail;
    tail = tail->next;

    delete prev;
    --current_length;
    return ret_data;
  };

  size_t size() { return current_length; };

  bool empty() {
    if (current_length == 0) {
      return true;
    }
    return false;
  };

private:
  size_t current_length;

  QueueElement<T> *tail = nullptr;
  QueueElement<T> *head = nullptr;
};

#endif
