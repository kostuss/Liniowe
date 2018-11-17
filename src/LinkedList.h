#ifndef AISDI_LINEAR_LINKEDLIST_H
#define AISDI_LINEAR_LINKEDLIST_H

#include <cstddef>
#include <initializer_list>
#include <stdexcept>

namespace aisdi
{

template <typename Type>
class LinkedList
{
public:
  using difference_type = std::ptrdiff_t;
  using size_type = std::size_t;
  using value_type = Type;
  using pointer = Type*;
  using reference = Type&;
  using const_pointer = const Type*;
  using const_reference = const Type&;

  class ConstIterator;
  class Iterator;
  using iterator = Iterator;
  using const_iterator = ConstIterator;

private:
    class Node;
    Node* first;
    Node* last;
    size_type list_size;


    void startlist()
    {
        first = new Node();
        last = new Node(first,nullptr);
        first->next = last;
        list_size = 0;
    }

public:
    using node_pointer = Node*;
    using const_node_pointer= const Node*;


  LinkedList()
  {
        startlist();
  }

  LinkedList(std::initializer_list<Type> l)
  {
     // (void) l;
      startlist();
      auto i = l.begin();
      while( i != l.end() )
      {
          append( *i );
          i++;
      }
  }

  LinkedList(const LinkedList& other)
  {
    startlist();
    const_iterator i;
    for(i=other.begin(); i!=other.end();i++)
    {
        append(*i);
    }
  }

  LinkedList(LinkedList&& other)
  {
     // (void) other;
     // throw std::runtime_error("Todo");

      list_size = other.list_size;
      first = other.first;
      last = other.last;
      other.list_size = 0;
      other.first = nullptr;
      other.last = nullptr;

  }

    ~LinkedList()
    {
        while(first != nullptr)
        {
            const Node* temp = first;
            first = first->next;
            delete temp;
        }
        first = last = nullptr;
        list_size = 0;
    }


  LinkedList& operator=(const LinkedList& other)
  {

      if(this == &other)
          return *this;

      while(first != nullptr)
      {
          Node* temp = first;
          first = first->next;
          delete temp;
      }
      first = last = nullptr;
      list_size = 0;

      startlist();
    const_iterator i;
    for(i=other.begin();i!=other.end();i++)
    {
        append(i.operator*());
    }
      return *this;
  }

  LinkedList& operator=(LinkedList&& other)
  {

      if(this == &other)
          return *this;

      Node* position = first;
      while(position != nullptr)
      {
          Node* temp = position;
          position = position->next;
          delete temp;
      }

      list_size = other.list_size;
      first = other.first;
      last = other.last;
      other.list_size = 0;
      other.first = other.last = nullptr;

      return *this;

  }

  bool isEmpty() const
  {
    return list_size==0;
  }

  size_type getSize() const
  {
      return list_size;
  }

  void append(const Type& item)
  {
      if(list_size == 0)
      {
          Node* new_node = new Node(item,first,last);
          first->next = new_node;
          last->previous = new_node;
          list_size++;
          return;
      }

      node_pointer new_node =new Node(item, last->previous, last);
      last->previous->next = new_node;
      last->previous = new_node;
      list_size++;

  }

  void prepend(const Type& item)
  {
    node_pointer new_node =new Node(item, first, first->next);
    first->next->previous=new_node;
    first->next=new_node;
    list_size++;
  }

  void insert(const const_iterator& insertPosition, const Type& item)
  {
      if(insertPosition==begin()){prepend(item); return;}
      if(insertPosition==end()){append(item); return;}

      node_pointer new_node =new Node(item, (insertPosition-1).current, insertPosition.current);
      (insertPosition-1).current->next=new_node;
      list_size++;
  }

  Type popFirst()
  {
      if (getSize()==0) {throw std::out_of_range ("empty ");}

    node_pointer toremove= first->next;
    value_type result=first->next->element;
    first->next = toremove->next;
    toremove->next->previous=first;
    delete toremove;
    list_size--;

    return result;
  }

  Type popLast()
  {
    if (getSize()==0) {throw std::out_of_range ("empty");}
    value_type result=last->previous->element;
    node_pointer toremove= last->previous;
    last->previous= toremove->previous;
    toremove->previous->next=last;
    delete toremove;
    list_size--;
    return result;
  }

  void erase(const const_iterator& possition)
  {

      if (isEmpty()) {if(possition==begin()) throw std::out_of_range("You want to erase begin of list");}
      if (possition==end()) {throw std::out_of_range("You want to erase end of list"); }
//        Node* toremove;
//        toremove=possition.current;

      possition.current->previous->next=possition.current->next;
      possition.current->next->previous=possition.current->previous;

        delete possition.current;
        list_size--;

  }

  void erase(const const_iterator& firstIncluded, const const_iterator& lastExcluded)
  {
      // zmienić na swoje
      firstIncluded.current->previous->next = lastExcluded.current;
      lastExcluded.current->previous = firstIncluded.current->previous;

      size_t counter = 0;
      auto it = firstIncluded;
      Node* temp;


      while(it != lastExcluded)
      {
          temp = it.current->next;
          delete it.current;
          it.current = temp;
          counter++;
      }
      list_size -= counter;
/*
      if (firstIncluded==lastExcluded) { return;}
      //first_to_remove->previous->next=last_to_remove;
      //last_to_remove->previous=first_to_remove->previous;

      const_iterator i;
      for (i = firstIncluded;i!=lastExcluded  ;i++ ) {
          erase(i);
      }
      */
  }

  iterator begin()
  {
    return Iterator(first->next);
  }

  iterator end()
  {
    return Iterator(last);
  }

  const_iterator cbegin() const
  {
    return ConstIterator(first->next);
  }

  const_iterator cend() const
  {
    return ConstIterator(last);
  }

  const_iterator begin() const
  {
    return cbegin();
  }

  const_iterator end() const
  {
    return cend();
  }

};

template <typename Type>
class LinkedList<Type>::Node
{
public:

    using node_pointer= typename LinkedList<Type>::node_pointer;
    Type element;
    Node* previous;
    Node* next;

    Node(node_pointer previous1 = nullptr, node_pointer next1 = nullptr):
            previous(previous1), next(next1)
    {}

    Node(const Type& element, node_pointer previous = nullptr, node_pointer next = nullptr):
            element(element), previous(previous), next(next)
    {}

    ~Node()
    {}
    
};

template <typename Type>
class LinkedList<Type>::ConstIterator
{
private:
    Node* current;
public:

    friend class LinkedList<Type>;
  using iterator_category = std::bidirectional_iterator_tag;
  using value_type = typename LinkedList::value_type;
  using difference_type = typename LinkedList::difference_type;
  using pointer = typename LinkedList::const_pointer;
  using reference = typename LinkedList::const_reference;



  ConstIterator()= default;

  explicit ConstIterator(Node* init_pointed)
  {
      current= init_pointed;
  }


  reference operator*() const
  {
      if( current->next == nullptr || current->previous == nullptr)
      {
          throw std::out_of_range("OUT OF RANGE");
      }
      else
      {
          return current->element;
      }
  }

  ConstIterator& operator++()
  {
        if(current->next == nullptr)
            throw std::out_of_range("OUT OF RANGE");
        else
    current = current->next;
    return *this;
  }

  ConstIterator operator++(int)
  {
        if (current->next == nullptr)
            throw std::out_of_range("OUT OF RANGE");
    ConstIterator result = ConstIterator(current);
    current=current->next;
    return result;
  }

  ConstIterator& operator--()
  {
    if(current->previous->previous== nullptr) {throw std::out_of_range("decrementing from begining");}
    current = current->previous;
    return *this;
  }

  ConstIterator operator--(int)
  {
      if(current->previous->previous== nullptr) {throw std::out_of_range("decrementing from begining");}
      ConstIterator result =ConstIterator(current);
      current=current->previous;
      return result;
  }

  ConstIterator operator+(difference_type d) const
  {
      Node* temp = current;
      while(d > 0)
      {
          if(temp->next == nullptr)
              throw std::out_of_range("OUT OF RANGE");
          temp = temp->next;
          d--;
      }

      return ConstIterator(temp);
      
  }

  ConstIterator operator-(difference_type d) const
  {
      Node* temp = current;
      while(d > 0)
      {
          if(temp->previous== nullptr)
              throw std::out_of_range("OUT OF RANGE");
          temp = temp->previous;
          d--;
      }
      return ConstIterator(temp);
  }


  bool operator==(const ConstIterator& other) const
  {
    return current==other.current;
  }

  bool operator!=(const ConstIterator& other) const
  {
      return current!=other.current;
  }


};

template <typename Type>
class LinkedList<Type>::Iterator : public LinkedList<Type>::ConstIterator
{
public:
  using pointer = typename LinkedList::pointer;
 // using pointer= typename LinkedList::node_pointer;
  using reference = typename LinkedList::reference;

  explicit Iterator(Node* node= nullptr) :
   ConstIterator(node)
  {}

  Iterator(const ConstIterator& other)
    : ConstIterator(other)
  {}

  Iterator(pointer other)
    : ConstIterator(other)
  {}

  Iterator& operator++()
  {
    ConstIterator::operator++();
    return *this;
  }

  Iterator operator++(int)
  {
    auto result = *this;
    ConstIterator::operator++();
    return result;
  }

  Iterator& operator--()
  {
    ConstIterator::operator--();
    return *this;
  }

  Iterator operator--(int)
  {
    auto result = *this;
    ConstIterator::operator--();
    return result;
  }

  Iterator operator+(difference_type d) const
  {
    return ConstIterator::operator+(d);
  }

  Iterator operator-(difference_type d) const
  {
    return ConstIterator::operator-(d);
  }

  reference operator*() const
  {
    // ugly cast, yet reduces code duplication.
    return const_cast<reference>(ConstIterator::operator*());
  }
};

}

#endif // AISDI_LINEAR_LINKEDLIST_H
